#pragma once
#include <WS2tcpip.h>
#include <vector>
#include <functional>
#include <thread>
#include "Logger.h"
#include "bf2server.h"

class RconClient
{
public:
	RconClient(SOCKET &socket, function<void(RconClient *c)> onDisconnect, string const &passwordHash);
	~RconClient();
	void Stop();
	void Start();

private:
	SOCKET socket;
	bool CheckLogin();
	bool connected;
	string passwordHash;
	function<void(RconClient *c)> onDisconnect = NULL;
	thread* workThread;

	void HandleCommand(string const & command);
	void SendResponse(vector<string> &response);
	void SendStatus();
	void SendPlayers();
	void HandleConnection();
	wstring s2ws(string const & s);
};