#include <Windows.h>
#include <iostream>
#include <string>

std::string FormatNumber(int Number, int MinLen = 3)
{
	auto SNum = std::to_string(Number);
	while (SNum.length() < MinLen)
		SNum = "0" + SNum;
	return SNum;
}

void UnlockConsole()
{
	AllocConsole();

	FILE* DIN;
	FILE* DOUT;
	FILE* DERR;
	freopen_s(&DIN, "CONIN", "r", stdin);
	freopen_s(&DOUT, "CONOUT$", "w", stdout);
	freopen_s(&DERR, "CONOUT$", "w", stderr);
}

CONST INT32 SRC_RECTV[4] = { 0, 0, 800, 600 };
DWORD SRC_RECT = (DWORD)&SRC_RECTV;
INT32 MM_IMG;
void __declspec(naked) __stdcall DrawImage(void* G, void* Image)
{
	__asm
	{
		mov ecx, [esp + 4]
		mov eax, SRC_RECT
		mov edx, [esp + 8]

		push 00
		push 00
		push edx
		call DRAW_IMG
		ret 0x8
	}
}