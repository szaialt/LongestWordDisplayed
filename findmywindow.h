#ifndef FINDMYWINDOW_H
#define FINDMYWINDOW_H

LRESULT CALLBACK FindWordWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class FindMyWindow {
     public:
             FindMyWindow(char* file);
             int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil);
            
             
     private:
             char* file;
};

#endif //#ifndef FINDMYWINDOW_H
