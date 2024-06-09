#include "MainFuncs.h"

BOOL APIENTRY DllMain(
    HMODULE ModuleHandle,
    DWORD  CallReason,
    LPVOID Reserved
)
{
    if (CallReason != DLL_PROCESS_ATTACH) return TRUE;
    Hook((DWORD)BOARD_MDOWN_HOOK, MiddleMen::MouseDown);
    Hook((DWORD)BOARD_MUP_HOOK, MiddleMen::MouseUp);
	Hook((DWORD)DRAW_BACKDROP_HOOK, MiddleMen::Draw);
    Hook((DWORD)UPDATE_HOOK, MiddleMen::Update);

    if (ENABLE_CONSOLE)
        UnlockConsole();

    auto App = LawnApp::GetApp();
    std::cout << "Loaded: " << App->Loaded << "\n";

    if (App->Loaded)
        LoadAssets();
    else
        Hook((DWORD)LOAD_HOOK, MiddleMen::Load);

	return TRUE;
}