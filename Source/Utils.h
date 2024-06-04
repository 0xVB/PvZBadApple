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

BYTE CBits[800 * 600 * 2];
struct CompressedSegment
{
public:
	// First 2 bits are reserved for metadata
	WORD Data = 0;

	void SetRepCount(WORD RepCount)// Sets the data without altering the first 2 bits
	{
		Data = (Data & 0xC000) | (RepCount & 0x3FFF);
	}

	void SetGeneric(bool Gen)// Sets the first bit to Gen
	{
		if (Gen)
			Data |= 0x8000; // Set the first bit
		else
			Data &= ~0x8000; // Clear the first bit
	}

	void SetWhite(bool White)// Sets the second bit to White
	{
		if (White)
			Data |= 0x4000; // Set the second bit
		else
			Data &= ~0x4000; // Clear the second bit
	}

	WORD GetRepCount()// Returns Data with the first 2 bits as zeros
	{
		return Data & 0x3FFF;
	}

	BOOL IsWhite()// Returns true if the first and second bits are true
	{
		return (Data & 0xC000) == 0xC000;
	}

	BOOL IsBlack()// Returns true if the first bit is true and the second is false
	{
		return (Data & 0x8000) && !(Data & 0x4000);
	}

	BYTE GetBrightness()
	{
		if (IsWhite()) return 255;
		if (IsBlack()) return 0;
		return *((BYTE*)(&Data) + 1);
	}

	void Assign(int Offset)
	{
		WORD* Address = (WORD*)(CBits + Offset);
		*Address = Data;
	}
};

struct CompressedImage
{
	size_t Size;
	BYTE MyBits[];

	static CompressedImage* Compress(size_t ByteCount, BYTE* Bits)
	{
		//BGRA
		int PixCount = ByteCount / 4;
		int TotalSize = 0;
		int CSegCol = -1;
		int CSegRep = 0;
		for (int i = 0; i < PixCount; i++)
		{
			int O = i * 4;
			int B = Bits[O];
			int G = Bits[O + 1];
			int R = Bits[O + 2];
			int A = Bits[O + 3];
			int C = (R + G + B) / 3;
			CompressedSegment Seg = CompressedSegment();
			if (C == CSegCol)
				CSegRep++;
			else if (C == 0 || C == 255)
			{
				if (CSegCol == -1)
				{
					CSegCol = C;
					CBits[TotalSize] = (BYTE)C;
					TotalSize++;
					continue;
				}
				// Conclude segment
				Seg.SetRepCount(CSegRep);
				Seg.SetWhite(CSegCol);
				Seg.SetGeneric(true);
				Seg.Assign(TotalSize);

				TotalSize += 2;// Add a segment
				CSegCol = C;// Set color
				CSegRep = 0;
			}
			else
			{
				// Non-generic color
				if (CSegCol != -1)
				{
					Seg.SetRepCount(CSegRep);
					Seg.SetWhite(CSegCol);
					Seg.SetGeneric(false);
					Seg.Assign(TotalSize);
					TotalSize += 2;
				}

				CBits[TotalSize] = (BYTE)C;
				CSegCol = -1;
				CSegRep = 0;
				TotalSize++;
			}
		}
		auto self = (CompressedImage*)operator new(TotalSize + 4);
		memcpy(self->MyBits, CBits, TotalSize);
		self->Size = TotalSize;
		return self;
	}
};