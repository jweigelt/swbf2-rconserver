#include "bf2server.h"
#include <stdio.h>

DWORD moduleBase, chatCCAddr;

void bf2server_init() {
	moduleBase = (DWORD)GetModuleHandle(L"BattlefrontII.exe");
	chatCCAddr = (DWORD)&bf2server_chat_cc;
	bf2server_set_chat_cc();
}

void bf2server_command(DWORD messageType, DWORD param1, const wchar_t* message, DWORD param3) {
	//NOTE: function might not be threadsafe
	DWORD addr = moduleBase + OFFSET_CHATINPUT;

	__asm {
		push messageType
		push param1
		mov edx, message
		mov ecx, param3
		call dword ptr[addr];
		add esp, 8
	}
}

void bf2server_set_chat_cc() {
	DWORD op, np;
	DWORD addr = moduleBase + OFFSET_CHATSNPRINTF;

	VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &op);
	
	*(BYTE*)addr = ASM_CALL;
	*(BYTE*)(addr + 1) = ASM_DWORD_PTR;
	*(DWORD*)(addr + 2) = (DWORD)&chatCCAddr;

	VirtualProtect((void*)addr, 6, op, &np);
}

int __cdecl bf2server_chat_cc(char* buf, size_t sz, const char* fmt, ...) {
	int ret = -1;

	va_list args;
	va_start(args, fmt);
	ret = vsnprintf(buf, sz, fmt, args);
	va_end(args);

	return ret;
}