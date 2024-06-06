#include "MainFuncs.h"

BOOL APIENTRY DllMain(
    HMODULE ModuleHandle,
    DWORD  CallReason,
    LPVOID Reserved
)
{
    if (CallReason != DLL_PROCESS_ATTACH) return TRUE;
	Hook((DWORD)DRAW_BACKDROP_HOOK, MiddleMen::Draw);

    if (ENABLE_CONSOLE)
        UnlockConsole();

    // BGRA
    auto App = LawnApp::GetApp();
    App->Loaded = false;
    std::cout << "Loaded: " << App->Loaded << "\n";
    if (App->Loaded)
        LoadAssets();
    else
        Hook((DWORD)LOAD_HOOK, MiddleMen::Load);

	return TRUE;
}