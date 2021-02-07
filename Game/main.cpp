
#pragma once

#include <engine/XPlatform.h>
#include "game.h"
#include <iostream>
#include <fstream>
#include <malloc.h>
#include <type_traits>

#define MAX_WIDTH 4096
#define MAX_HEIGHT 4096

using namespace std;

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Game game;
    if (game.init(hInstance))
    {
        game.run();
    }
    game.exit();
    return 0; 
}