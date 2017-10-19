#include "RconClient.h"

RconClient::RconClient(SOCKET & socket, function<void(RconClient*c)> onDisconnect, string const & passwordHash)
{
	this->socket = socket;
	this->passwordHash = passwordHash;
	this->onDisconnect = onDisconnect;
}

RconClient::~RconClient() { }

void RconClient::Stop()
{
	connected = false;
	closesocket(socket);
	workThread->join();
	delete workThread;
}

void RconClient::Start()
{
	workThread = new thread(&RconClient::HandleConnection, this);
}

bool RconClient::CheckLogin()
{
	char pwd[33];
	char magic = 0;
	char res;

	pwd[32] = 0x00;

	if (recv(socket, pwd, 32, 0) != 32) {
		return false;
	}

	if (recv(socket, &magic, 1, 0) != 1) {
		return false;
	}

	if (magic != 0x64) {
		return false;
	}

	if (passwordHash.compare(pwd) == 0) {
		Logger.Log(LogLevel_VERBOSE, "Client logged in.", pwd);
		res = 1;
	}
	else {
		Logger.Log(LogLevel_VERBOSE, "Client sent wrong password '%s'", pwd);
		res = 0;
	}
	send(socket, &res, 1, 0);

	return (res == 1);
}

wstring RconClient::s2ws(string const & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


void RconClient::HandleCommand(string const & command)
{
	if (command.compare("/status") == 0) {
		SendStatus();
	}

	else if (command.compare("/players") == 0) {
		SendPlayers();
	}

	else {
		bf2server_command(MESSAGETYPE_COMMAND, 0, s2ws(command).c_str(), 0);
	}
}

void RconClient::SendResponse(vector<string> &response)
{
	unsigned char rowLen = 0;
	unsigned char rows = (unsigned char)response.size();
	send(socket, (char*)&rows, 1, 0);

	for (string row : response) {
		rowLen = (unsigned char)row.length() + 1;
		send(socket, (char*)&rowLen, 1, 0);
		send(socket, row.c_str(), rowLen, 0);
	}
}

void RconClient::SendPlayers()
{
	//TODO: read from memory
	//1  "LeKeks"          Rep 0   0   0   51  192.168.178.35  ae48aab730214004b1f024d54757fccd
	//id name------------- t-- v-- v-- v-- p-- ip------------- key-----------------------------
	//123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
	vector<string> r = vector<string>();;

	SendResponse(r);
}

void RconClient::HandleConnection()
{
	unsigned char rows = 0;
	unsigned char sz = 0;
	unsigned char bytesRead = 0;
	char* buffer;
	int fragment = 0;
	bool err = false;

	if (!(connected = CheckLogin())) {
		Logger.Log(LogLevel_VERBOSE, "Client login failed.");
	}

	while (connected) {
		//we expect 1 row -> ignore rowCount
		if (recv(socket, (char*)&rows, 1, 0) != 1) {
			break;
		}

		if (recv(socket, (char*)&sz, 1, 0) != 1) {
			break;
		}
		buffer = new char[sz];

		while (sz > bytesRead) {
			if ((fragment = recv(socket, buffer + bytesRead, sz - bytesRead, 0)) == SOCKET_ERROR) {
				err = true;
				break;
			}
			buffer[sz - 1] = 0;
			bytesRead += (char)fragment;
		}

		if (err) {
			break;
		}

		Logger.Log(LogLevel_VERBOSE, "Received command: %s", buffer);
		HandleCommand(string(buffer));
		delete[] buffer;
		bytesRead = 0;
	}

	Logger.Log(LogLevel_VERBOSE, "Closing connection.");
	if (connected) {
		closesocket(socket);
		connected = false;
	}

	onDisconnect(this);
}

void RconClient::SendStatus()
{
	//TODO: read from memory
	vector<string> r = vector<string>();
	r.push_back("ServerName: Demo Server");
	r.push_back("ServerIP: 127.0.0.1");
	r.push_back("Version: 1.1");
	r.push_back("MaxPlayers: 15");
	r.push_back("Password: ");
	r.push_back("CurrentMap: tat2g_eli");
	r.push_back("NextMap: tat2g_eli");
	r.push_back("GameMode: ass");
	r.push_back("Players: 10");
	r.push_back("Scores: 0/0");
	r.push_back("Tickets: 0/0");
	r.push_back("FFEnabled: true");
	r.push_back("Heroes: true");
	SendResponse(r);
}