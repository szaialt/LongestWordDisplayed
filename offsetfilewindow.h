#ifndef OFFSETFILEWINDOW_H
#define OFFSETFILEWINDOW_H

#include <windows.h>

LRESULT CALLBACK OffsetWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class OffsetFileWindow {
     public:
             OffsetFileWindow(char* file);
             int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil);
            
             
};

#endif //#ifndef OFFSETFILEWINDOW_H
