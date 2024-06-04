#pragma once
#include "PvZBadApple.h"
#include <filesystem>
#include <fstream>

std::ifstream BadAppleHandle;
byte* CURR_FRAME_DATA;

// Load all generic background images
void LoadImages(LawnApp* App)
{
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

LawnApp* __stdcall LoadAssets()
{
	auto App = LawnApp::GetApp();
	LoadImages(App);
	bool Exists = std::filesystem::exists(COMPRESSED_DIR);
	if (!Exists)
	{
		std::ofstream Handle(COMPRESSED_DIR, std::ios::out | std::ios::trunc);
		if (!Handle.is_open()) throw "Unable to open handle to compress BadApple.";

		// Compress & load bad apple
		for (int F = 1; F < FRAME_COUNT; F += FRAME_SKIP)
		{
			if (F > FRAME_COUNT) break;
			auto IName = std::string(IMAGE_PPR) + FormatNumber(F);
			auto Frame = App->GetImage(IName, false);
			auto CFrame = CompressedImage::Compress(FRAME_SIZE, (BYTE*)Frame->Bits);

			Handle.write((char*)CFrame, 4 + CFrame->Size);
			std::cout << "Loaded Frame " << F / FRAME_SKIP << ": " << IName << '\n';
			
			delete CFrame;
			Frame->~DDImage();
		}
		Handle.close();
		std::cout << "Frame Loading successful! Handle closed.";
	}
	BadAppleHandle = std::ifstream(COMPRESSED_DIR, std::ios::in | std::ios::binary);
	if (!BadAppleHandle.is_open()) throw "Unable to open handle to compress BadApple.";
	return App;
}

void ReadChunk(std::streampos Start, std::streamsize Size)
{
	BadAppleHandle.seekg(Start);
	BadAppleHandle.read((char*)CURR_FRAME_DATA, Size);
}

byte* GetFrameData(int Frame)
{
	ReadChunk(Frame * FRAME_SIZE, FRAME_SIZE);
	return CURR_FRAME_DATA;
}

byte* LoadBadApple()
{
	delete CURR_FRAME_DATA;
	auto Size = std::filesystem::file_size(COMPRESSED_DIR);
	CURR_FRAME_DATA = (BYTE*)operator new(Size);
	BadAppleHandle.seekg(0);
	BadAppleHandle.read((char*)CURR_FRAME_DATA, Size);
	BadAppleHandle.close();
}

int LastFrame = -1;
LawnApp* __stdcall DrawBG(Sexy::Graphics* G)
{
	Lawn* L = LawnApp::GetApp()->Lawn;
	if (L->Background >= 8 || L->Background < 0 || L->MainCounter == 0) return LawnApp::GetApp();

	auto NormBG = DICT[L->Background];
	auto AltBG = IDICT[L->Background];

	float Timer = L->MainCounter / 100.0;
	int CurrentFrameIndex = (int)(Timer * FPS / FRAME_SKIP) % FRAME_COUNT;
	if (LastFrame != CurrentFrameIndex)
	{
		LastFrame = CurrentFrameIndex;
		byte* FrameData = GetFrameData(CurrentFrameIndex);
		for (int i = 0; i < FRAME_SIZE; i++)
		{
			float Alpha = FrameData[i] / 255.0;
			SAMPL->SetPixel(i, BColor(NormBG->Bits[i], AltBG->Bits[i], Alpha));
			SAMPL->BitsChangedCount++;
		}
	}
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