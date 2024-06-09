#include "Definitions.h"

void __declspec(naked) __stdcall SPKT_CHANGE_TYPE(SeedPacket* Packet, SeedType NewType, SeedType ImitaterType)
{
	__asm
	{
		push esi// Backup esi
		push edi// Backup edi

		mov esi, [esp + 0xC]// Packet
		mov edi, [esp + 0x10]// NewType
		mov edx, [esp + 0x14]// ImitaterType
		mov eax, 0x489B50// Function address
		call eax
		
		pop edi// Restore edi
		pop esi// Restore esi
		ret 0xC// Pop params
	}
}

class SeedPacket: public GameObject
{
public:
	int CooldownTimer;
	int MaxCooldown;
	int Index;
	int XOffset;
	SeedType PacketType;
	SeedType ImitaterType;
	int SlotMachineCountdown;
	SeedType SlotMachingNextSeed;
	float SlotMachinePosition;
	bool Active;
	bool OnCooldown;
	int TimesUsed;

	static SeedPacket* Clone(SeedPacket* Other, SeedPacket* To)
	{
		if (!To)
			To = (SeedPacket*)operator new(sizeof(SeedPacket));
		memcpy(To, Other, sizeof(SeedPacket));
		return To;
	}

	void SetType(SeedType NewType, SeedType NewImitaterType = (SeedType) - 1)
	{
		SPKT_CHANGE_TYPE(this, NewType, NewImitaterType);
	}
};