#pragma once
#include "RconServer.h"
#include "bf2server.h"
#include <Windows.h>

using namespace std;

static RconServer* server;

DWORD WINAPI Run(LPVOID p) {
	server = new RconServer(4658, 100, "1234");
	server->Start();

	bf2server_init();

	Sleep(500);

	if (bf2server_login()) {
		Logger.Log(LogLevel_INFO, "Login OK.");
	}
	else {
		Logger.Log(LogLevel_INFO, "Login failed.");
	}

	bf2server_set_details(1);

	while (!GetAsyncKeyState(VK_ESCAPE)) {
		Sleep(100);
		string r;
	}

	server->Stop();
	delete server;
	server = NULL;
	FreeLibraryAndExitThread((HMODULE)p, 0);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, IN LPVOID dwReserved) {

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, Run, hModule, 0, 0);
		break;

	case DLL_PROCESS_DETACH:
		if (server != NULL) {
			server->Stop();
			delete server;
			server = NULL;
		}
		break;
	}

	return TRUE;
}