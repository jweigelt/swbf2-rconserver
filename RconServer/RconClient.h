#pragma once
#include <WS2tcpip.h>
#include <vector>
#include <functional>
#include <thread>
#include "Logger.h"
#include "bf2server.h"
#include "md5.h"

class RconClient
{
public:
	RconClient(SOCKET &socket, function<void(RconClient *c)> onDisconnect);
	~RconClient();
	void Stop();
	void Start();
	void OnChatInput(string const & msg);

private:
	SOCKET socket;
	bool CheckLogin();
	bool connected;
	function<void(RconClient *c)> onDisconnect = NULL;
	thread* workThread;

	void HandleCommand(string const & command);
	void Send(vector<string> &response);
	void HandleConnection();

};