#pragma once
#pragma comment (lib, "Ws2_32.lib")
#include <WS2tcpip.h>
#include <stdint.h>
#include <vector>
#include <mutex>
#include <thread>

#include "RconClient.h"
#include "Logger.h"

class RconServer
{
public:
	RconServer(uint16_t port, uint16_t maxClients);
	~RconServer();
	void Start();
	void Stop();
	void Listen();

private:
	SOCKET listenSocket;
	bool running = false;
	vector<RconClient*> clients = vector<RconClient*>();
	uint16_t port;
	uint16_t maxClients;
	void OnClientDisconnect(RconClient *client);
	void OnChatInput(string const & msg);
	mutex mtx;
	thread* workThread;
};