#pragma once
#include <Windows.h>
#include <functional>
#include "Logger.h"

//#define GOG


#ifdef GOG
//Offsets for GOG's BattlefrontII.exe
#define OFFSET_CHATINPUT 0x1B0030
#define OFFSET_CHATSNPRINTF 0x1B2F67
#define OFFSET_RESBUFFER 0x1BA2830
#define OFFSET_ADMINPW 0x1A57E10
#define OFFSET_COMMAND_DETAILS 0x1A57E2C
#else
//530, 520, 1120, , 10CF

#define OFFSET_CHATINPUT 0x1AFB00 + 0x1000
#define OFFSET_CHATSNPRINTF 0x1B2A47 + 0x1000 
#define OFFSET_RESBUFFER 0x1BA3950 + 0x1000
#define OFFSET_ADMINPW 0x1A653A0
#define OFFSET_COMMAND_DETAILS 0x1A58EFB + 0x1000
#endif


#define ASM_NOP 0x90
#define ASM_JMP 0xE9
#define ASM_CALL 0xFF
#define ASM_DWORD_PTR 0x15

#define MESSAGETYPE_CHAT 1
#define MESSAGETYPE_COMMAND 0

#define OUTPUT_CHAR 0
#define OUTPUT_BUFFER -1

#define DETAILS_VERBOSE 1
#define DETAILS_NORMAL 0

/**
 *	Initializes server-access
 **/
void bf2server_init();

/**
 *	Calls swbf2's chat/command handling function.
 *	@param messageType 0 (command) or 1 (chat)
 *	@param sender 0 (remote) or 1 (selfhost) (using selfhost won't return result)
 *	@param message command or chat message
 *	@param responseOutput 0 (chat) or -1 (buffer)
 **/
string bf2server_command(DWORD messageType, DWORD sender, const wchar_t* message, DWORD responseOutput);

/**
 *	Attaches a codecave to swbf2's chat-output
 **/
void bf2server_set_chat_cc();

/**
 *	Called when new chat is received
 **/
int __cdecl bf2server_chat_cc(char* buf, size_t sz, const char* fmt, ...);

/**
 *	Gets the server's admin password
 **/
string bf2server_get_adminpwd();

/**
 *	Authenticate admin session
 **/
bool bf2server_login();

/**
 *	Enables / Disables detailed command out
 **/
void bf2server_set_details(BYTE mode);

/**
 *	Converts string to wstring
 **/
wstring bf2server_s2ws(string const & s);

/**
 *	Sets Chat-callback
 **/
void bf2server_set_chat_cb(function<void(string const &msg)> onChat);