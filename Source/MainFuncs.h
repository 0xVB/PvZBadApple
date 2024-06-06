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
	if (Frame == LastFrame) return;
	if (Frame < LastFrame)
	{
		LastFrame = 0;
		LastFrameCompressedData = COMPRESSED_DATA;
	}

	while (Frame != LastFrame)
		SeekNextFrame(Frame == (LastFrame + 1));
}

LawnApp* __stdcall DrawBG(Sexy::Graphics* G)
{
	Lawn* L = LawnApp::GetApp()->Lawn;
	if (L->Background >= 8 || L->Background < 0 || L->MainCounter == 0) return LawnApp::GetApp();

	auto NormBG = DICT[L->Background];
	auto AltBG = IDICT[L->Background];
	float Timer = L->MainCounter / 100.0;

	int CurrentFrameIndex = (int)(Timer * FPS / FRAME_SKIP) % FRAME_COUNT;
	SeekFrameData(CurrentFrameIndex);

	std::cout << "Frame: " << CurrentFrameIndex << "\n";
	for (int i = 0; i < FRAME_SIZE; i++)
		SAMPL->Bits[i] = BColor(NormBG->Bits[i], AltBG->Bits[i], CURR_FRAME_DATA[i] / 255.0);

	DrawImage(G, SAMPL);
	return LawnApp::GetApp();
}

namespace MiddleMen
{
	void __declspec(naked) Draw()
	{
		__asm
		{
			push [esp+0xA8]
			call DrawBG
			pop ecx
			mov ecx, 0x41652D
			jmp ecx
		}
	}

	void __declspec(naked) Load()
	{
		__asm
		{
			call LoadAssets
			pop edx
			mov ecx, eax
			push 0x64F348
			jmp edx
		}
	}
}