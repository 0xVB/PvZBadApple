#pragma once
#include <Windows.h>

void __stdcall Hook(DWORD Address, void* Function)
{
	DWORD OldProt;
	VirtualProtect((LPVOID)Address, 5, PAGE_EXECUTE_READWRITE, &OldProt);

	DWORD Offset = (DWORD)Function - Address - 5;// Relative offset
	*(BYTE*)Address = 0xE8;// Set to call
	*(DWORD*)(Address + 1) = Offset;

	DWORD Temp;
	VirtualProtect((LPVOID)Address, 5, OldProt, &Temp);
}