#pragma once
#include "RconServer.h"
#include "bf2server.h"
#include <Windows.h>

using namespace std;

static RconServer* server;

DWORD WINAPI Run(LPVOID p) {
	server = new RconServer(4658, 100);
	server->Start();

	bf2server_init();
	bf2server_set_details(1);

#ifdef _DEBUG
	while (!(GetAsyncKeyState(VK_ESCAPE) && GetAsyncKeyState(VK_BACK))) {
		Sleep(100);
	}
#else
	while (true) Sleep(100);
#endif

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