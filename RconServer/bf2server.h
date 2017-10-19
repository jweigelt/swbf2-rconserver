#pragma once
#include <Windows.h>

//Offsets for GOG's BattlefrontII.exe
#define OFFSET_CHATINPUT (DWORD)0x1B0030
#define OFFSET_CHATSNPRINTF (DWORD)0x1B2F67

#define ASM_NOP 0x90
#define ASM_JMP 0xE9
#define ASM_CALL 0xFF
#define ASM_DWORD_PTR 0x15

#define MESSAGETYPE_CHAT 1
#define MESSAGETYPE_COMMAND 0

/**
 *	Initializes server-access
 **/
void bf2server_init();

/**
 *	Calls swbf2's chat/command handling function.
 *	@param messageType 0 (command) or 1(chat)
 *	@param param1 unknown param, set 0
 *	@param message command or chat message
 *	@param param3 unknown param, set 0
 **/
void bf2server_command(DWORD messageType, DWORD param1, const wchar_t* message, DWORD param3);

/**
 *	Attaches a codecave to swbf2's chat-output
 **/
void bf2server_set_chat_cc();

/**
 *	Called when new chat is received
 **/
int __cdecl bf2server_chat_cc(char* buf, size_t sz, const char* fmt, ...);