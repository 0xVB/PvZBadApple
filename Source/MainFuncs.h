#pragma once
#include "Include/Compression.h"
#include "PvZBadApple.h"

#include <filesystem>
#include <fstream>

int LastFrame = 0;
BYTE* LastFrameCompressedData = nullptr;

std::ifstream BadAppleHandle;
// Load all generic background images
void LoadImages(LawnApp* App)
{
	if (CURR_FRAME_DATA) delete CURR_FRAME_DATA;
	CURR_FRAME_DATA = (byte*)operator new(FRAME_SIZE);
	SAMPL = App->GetImage(std::string(BG_DIR) + "LawnDay", false);
	LAWND = App->GetImage(std::string(BG_DIR) + "LawnDay", false);
	LAWNN = App->GetImage(std::string(BG_DIR) + "LawnNight", false);

	POOLD = App->GetImage(std::string(BG_DIR) + "PoolDay", false);
	POOLN = App->GetImage(std::string(BG_DIR) + "PoolNight", false);

	ROOFD = App->GetImage(std::string(BG_DIR) + "RoofDay", false);
	ROOFN = App->GetImage(std::string(BG_DIR) + "RoofNight", false);

	ZEND = App->GetImage(std::string(BG_DIR) + "ZenDay", false);
	ZENN = App->GetImage(std::string(BG_DIR) + "ZenNight", false);
	std::cout << "Loaded alternate backgrounds\n";

	SAMPL->BitsChanged = true;
	SAMPL->IsVolatile = true;
	SAMPL->HasAlpha = true;
	SAMPL->HasTrans = true;

	BGDICT[0] = 1;
	BGDICT[1] = 0;
	BGDICT[2] = 3;
	BGDICT[3] = 2;
	BGDICT[4] = 5;
	BGDICT[5] = 4;

	IDICT[0] = LAWNN;
	IDICT[1] = LAWND;
	IDICT[2] = POOLN;
	IDICT[3] = POOLD;
	IDICT[4] = ROOFN;
	IDICT[5] = ROOFD;
	IDICT[6] = ZENN;
	IDICT[7] = ZEND;
	DICT[0] = LAWND;
	DICT[1] = LAWNN;
	DICT[2] = POOLD;
	DICT[3] = POOLN;
	DICT[4] = ROOFD;
	DICT[5] = ROOFN;
	DICT[6] = ZEND;
	DICT[7] = ZENN;
}

BYTE* LoadBadApple()
{
	if (COMPRESSED_DATA) delete COMPRESSED_DATA;
	auto Size = std::filesystem::file_size(COMPRESSED_DIR);

	COMPRESSED_DATA = (BYTE*)operator new(Size);
	BadAppleHandle.seekg(0);
	BadAppleHandle.read((char*)COMPRESSED_DATA, Size);
	BadAppleHandle.close();
	LastFrameCompressedData = COMPRESSED_DATA;

	return COMPRESSED_DATA;
}

LawnApp* __stdcall LoadAssets()
{
	auto App = LawnApp::GetApp();
	bool Exists = std::filesystem::exists(COMPRESSED_DIR);
	if (!Exists)
	{
		std::cout << "Compressed Bad Apple doesn't exist! Compressing...\n";
		COMPRESSED_DATA = (BYTE*)operator new(429496730);// 0.4 Gigabytes
		CURR_FRAME_DATA = (BYTE*)operator new (FRAME_SIZE);

		std::ofstream Handle(COMPRESSED_DIR, std::ios::out | std::ios::trunc);
		if (!Handle.is_open()) std::cout << "Unable to open handle to compress BadApple.";

		UINT TotalSize = 0;
		BYTE* Data = COMPRESSED_DATA;
		// Compress & load bad apple
		for (int F = 1; F < FRAME_COUNT; F += FRAME_SKIP)
		{
			if (F > FRAME_COUNT) break;
			auto IName = std::string(IMAGE_PPR) + FormatNumber(F);
			auto Frame = App->GetImage(IName, false);
			Frame->ToRBV(CURR_FRAME_DATA);// Output raw brightness values
			
			BYTE* End = VBCompression::Compress(FRAME_SIZE, CURR_FRAME_DATA, Data);
			UINT FrameSize = (UINT)(End - Data);
			
			Data = End;
			TotalSize += FrameSize;
			Frame->~DDImage();
		}

		Handle.write((const char*)COMPRESSED_DATA, TotalSize);
		Handle.close();
	}

	LoadImages(App);
	BadAppleHandle = std::ifstream(COMPRESSED_DIR, std::ios::in | std::ios::binary);
	if (!BadAppleHandle.is_open()) throw "Unable to open handle to compress BadApple.";
	LoadBadApple();

	return App;
}

void SeekNextFrame(bool DoDecompress = false)
{
	UINT PixelsJumped = 0;
	BYTE* Curr = LastFrameCompressedData;
	BYTE* CurrOut = CURR_FRAME_DATA;

	while (PixelsJumped < FRAME_SIZE)
	{
		auto S = VBCompression::Segment::DecompressFrom(Curr);

		PixelsJumped += S.RepeatCount;
		Curr += S.CSize;

		if (DoDecompress)
		{
			S.DecompressTo(CurrOut);
			CurrOut += S.RepeatCount;
		}
	}

	LastFrameCompressedData = Curr;
	LastFrame++;
}

void SeekFrameData(int Frame)
{
	for (int i = 0; i < FRAME_SIZE; i++)
		CURR_FRAME_DATA[i] = (i >= (1600 * (Frame % 300))) * 255;
}

bool PixelIsMasked(int X, int Y)
{
	return CURR_FRAME_DATA[Y * 800 + X] >= 128;
}

namespace Raw
{
	void __declspec(naked) __stdcall DrawBoard(Lawn* L, Sexy::Graphics* G)
	{
		__asm
		{
			pop edx
			pop ecx
			push edx
			mov eax, fs:[0x0]
			jmp DRAW_BACKDROP_JBK
		}
	}

	void __declspec(naked) __stdcall UpdateBoard(Lawn* L)
	{
		__asm
		{
			// Fix params & ret addr
			pop edx
			pop ecx
			push edx

			// Jump over the hook
			push ebp
			mov ebp, esp
			and esp, 0xFFFFFFF8
			mov eax, fs:[0x0]
			jmp UPDATE_JBK
		}
	}

	void __declspec(naked) __stdcall BoardMouseDown(Lawn* L, int X, int Y, int ClickCode)
	{
		__asm
		{
			pop edx// Return addr
			pop ecx// this
			push edx// Return addr

			// Overwritten code
			push ebp
			mov ebp, esp
			and esp, -0x8
			jmp BOARD_MDOWN_JBK// Go to function
		}
	}

	void __declspec(naked) __stdcall BoardMouseUp(Lawn* L, int X, int Y, int ClickCode)
	{
		__asm
		{
			pop edx// Return addr
			pop ecx// this
			push edx// Return addr

			// Overwritten code
			mov eax, [esp + 0x8]
			push ebp
			jmp BOARD_MUP_JBK// Go to function
		}
	}
}

Lawn* AlternateLawn = nullptr;
Lawn* PreviousLawn = nullptr;
bool BlackIsNormal = true;// When set to false, the actual lawn will be displayed on white instead of black
void __stdcall DrawBG(Sexy::Graphics* G)
{
	auto App = LawnApp::GetApp();
	Lawn* L = App->Lawn;

	// If the background is zen garden or
	// If the game hasn't started or
	// If the alternate lawn hasn't been created yet,
	// then draw normally and return
	if (L->Background >= 6 || L->Background < 0 || L->MainCounter == 0 || !AlternateLawn)
	{
		Raw::DrawBoard(L, G);
		return;
	}

	// Draw the alternate lawn
	App->Lawn = AlternateLawn;
	Raw::DrawBoard(AlternateLawn, G);
	App->Lawn = L;

	// Copy its drawing data (kind of like taking a screenshot of it)
	DDIMG Image = ((DDIMG)G->State.DestImage);
	memcpy(SAMPL->Bits, Image->GetBits(), FRAME_SIZE * 4);// Put the drawing data in SAMPL

	// Draw the actual lawn
	Raw::DrawBoard(L, G);


	// Calculate the current frame
	float Timer = L->MainCounter / 100.0;
	int CurrentFrameIndex = (int)(Timer * FPS / FRAME_SKIP) % FRAME_COUNT;
	std::cout << "Frame: " << CurrentFrameIndex << "\n";

	// Obtain the current frame from Bad Apple
	SeekFrameData(CurrentFrameIndex);

	for (int i = 0; i < FRAME_SIZE; i++)// Mask each pixel in the frame
	{
		if (BlackIsNormal)// Mask it using the white pixels
			SAMPL->Bits[i].A = CURR_FRAME_DATA[i];
		else// Mask it using the black pixels
			SAMPL->Bits[i].A = 255 - CURR_FRAME_DATA[i];
	}

	// Finally, draw the masked alternate lawn on top
	DrawImage(G, SAMPL);
	return;
}

void __stdcall UpdateGame(Lawn* L)
{
	if (PreviousLawn != L)
	{
		PreviousLawn = L;
		if (AlternateLawn)
		{
			AlternateLawn->DestroyClone();
			AlternateLawn = nullptr;
		}
	}

	if (L->MainCounter == 1)
	{
		AlternateLawn = Lawn::Clone(L);
		AlternateLawn->Background = (BackgroundType)BGDICT[L->Background];
		AlternateLawn->LoadBackground();
		for (int i = 0; i < AlternateLawn->SeedBank->PacketCount; i++)
		{
			auto Packet = &AlternateLawn->SeedBank->SeedPackets[i];
			if (Packet->PacketType > MAX_SEED_CONVERT) continue;
			Packet->SetType(SEED_CONVERT[Packet->PacketType]);
		}
	}

	Raw::UpdateBoard(L);

	if (AlternateLawn)
	{
		L->App->Lawn = AlternateLawn;
		Raw::UpdateBoard(AlternateLawn);
		L->App->Lawn = L;
	}
}

void __stdcall GameMDown(Lawn* L, int X, int Y, int ClickCode)
{
	Lawn* O = L;
	if (PixelIsMasked(X, Y))
		L = AlternateLawn;

	L->App->Lawn = L;
	Raw::BoardMouseDown(L, X, Y, ClickCode);
	L->App->Lawn = O;
}

void __stdcall GameMUp(Lawn* L, int X, int Y, int ClickCode)
{
	Lawn* O = L;
	if (PixelIsMasked(X, Y))
		L = AlternateLawn;

	L->App->Lawn = L;
	Raw::BoardMouseUp(L, X, Y, ClickCode);
	L->App->Lawn = O;
}

namespace MiddleMen
{
	void __declspec(naked) Draw()
	{
		__asm
		{
			jmp DrawBG
		}
	}

	void __declspec(naked) Load()
	{
		__asm
		{
			call LoadAssets
			mov ecx, eax
			push 0x64F348
			jmp LOAD_JBK
		}
	}

	void __declspec(naked) Update()
	{
		__asm
		{
			pop edx// Return address
			push ecx// Push the lawn
			push edx// Push the return address
			jmp UpdateGame// Go to update game
		}
	}

	void __declspec(naked) MouseDown()
	{
		__asm
		{
			pop edx// Return addr
			push ecx// this
			push edx// Return addr
			jmp GameMDown
		}
	}

	void __declspec(naked) MouseUp()
	{
		__asm
		{
			pop edx// Return addr
			push ecx// this
			push edx// Return addr
			jmp GameMUp
		}
	}
}