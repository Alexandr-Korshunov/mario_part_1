#include <iostream>
#include <windows.h>
#include "Level.hpp"

int main() {
    const int windowWidth = 80;
    const int windowHeight = 25;

    Level gameLevel(windowWidth, windowHeight);
	
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	
    gameLevel.render(); 

    do {
        bool needRender = gameLevel.update(); 
        
        if (needRender) {
            gameLevel.render();
        }

        Sleep(0);
    } while (GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}