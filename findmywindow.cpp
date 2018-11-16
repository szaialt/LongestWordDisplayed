#include <windows.h>
#include <utility>
#include <string.h>
#include <sstream>
#include "exceptions.h"
#include "filereader.h"
#include "findmywindow.h"

using namespace std;

FindMyWindow::FindMyWindow(char* file){
     
     file = file;
     
     }
     
/*  This function is called by the Windows function DispatchMessage()  */

    static const int find_word_ok_id = 403;
    static const int find_word_edit_line_id = 404;
    HWND hwnd_word_edit_line;
    
LRESULT CALLBACK FindWordWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char word[1024] ;
    switch (message)                  /* handle the messages */
    {case WM_COMMAND:
		{
		switch(LOWORD(wParam))
		{
			
			
		case find_word_ok_id:{
             UINT nButton	= (UINT) LOWORD(wParam) ;
		     HWND hButtonWnd = (HWND) lParam ;

             char word[1024];
             GetWindowText(hwnd_word_edit_line, word, sizeof(word));
             try {
                 FileReader reader = FileReader();
                 int times = reader.findWord(word);
                 stringstream strs;
                 strs << times;
                 string temp_str = strs.str();
                 char* timesString = (char*) temp_str.c_str();
                 //char* timesString = itoa(times, "%d", 10);

                 MessageBox(hwnd, strcat(strcat(strcat(word, " found "), timesString), " times."), "Your word", MB_OK) ;
             }
             catch(FileNotFoundException ex){
                            MessageBox(hwnd, "File not found.", "fileName", MB_OK) ;
                        }
                        catch(BadFileFormatException ex){
                            MessageBox(hwnd, "Bad file format.", "fileName", MB_OK) ;
                        }
             }
             break;
			}
		}
		break ;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    //return DialogBox(hExe,MAKEINTRESOURCE(hwnd),NULL,getFileName);
    return 0;
}
     
     
int WINAPI FindMyWindow::WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil){
     HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    HWND hwnd_word_label, hwnd_OK_button;
    char szClassName[ ] = "Find a word";
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = FindWordWindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(0x00ffbbbb);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           700,                 /* The programs width */
           175,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
     

   int x, w, y, h;
   y = 20; h = 20;
   x = 10; w = 100;
   hwnd_word_label = CreateWindow("static", "ST_U",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              x, y, w, h,
                              hwnd, (HMENU)(501),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_word_label, "Search for the word:");

   ShowWindow (hwnd_word_label, nFunsterStil);

   x += w; w = 500;
   hwnd_word_edit_line = CreateWindow("edit", "",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP
                              | ES_LEFT | WS_BORDER,
                              x, y, w, h,
                              hwnd, (HMENU)(find_word_edit_line_id),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_word_edit_line, "");
   ShowWindow (hwnd_word_edit_line, nFunsterStil);
   
   w = 60;
   y += 2*h;
   hwnd_OK_button = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), 
   TEXT("OK"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
   x, y, w, h, hwnd, (HMENU)(find_word_ok_id), GetModuleHandle(NULL), NULL);
   ShowWindow (hwnd_OK_button, nFunsterStil);                            
}



  
