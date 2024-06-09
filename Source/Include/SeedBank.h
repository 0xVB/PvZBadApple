#include "Definitions.h"
#include "SeedPacket.h"

class SeedBank : public GameObject
{
public:
	int PacketCount;
	SeedPacket SeedPackets[10];
	int CutSceneDarken;
	int ConveyorBeltTimer;

	void ChangeLawn(Lawn* L)
	{
		MyLawn = L;
		for (int i = 0; i < PacketCount; i++)
			SeedPackets[i].MyLawn = L;
	}

	static SeedBank* Clone(SeedBank* S)
	{
		SeedBank* New = (SeedBank*)operator new(sizeof(SeedBank));
		memcpy(New, S, sizeof(SeedBank));
		return New;
	}
};