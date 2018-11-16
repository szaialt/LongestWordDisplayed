#include <windows.h>
#include <utility>
#include <string.h>
#include <sstream>
#include "exceptions.h"
#include "filewriter.h"
#include "offsetfilewindow.h"

using namespace std;

char* offsetInFile;

OffsetFileWindow::OffsetFileWindow(char* file){
     
     offsetInFile = file;
     
     }
     
/*  This function is called by the Windows function DispatchMessage()  */

    static const int offset_ok_id = 303;
    static const int offset_out_edit_line_id = 304;
    static const int offset_offset_edit_line_id = 305;
    HWND hwnd_out_edit_line; 
    HWND hwnd_offset_edit_line;
    
LRESULT CALLBACK OffsetWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char word[1024] ;
    switch (message)                  /* handle the messages */
    {case WM_COMMAND:
		{
		switch(LOWORD(wParam))
		{
			
			
		case offset_ok_id:{
             UINT nButton	= (UINT) LOWORD(wParam) ;
		     HWND hButtonWnd = (HWND) lParam ;

             char outfile[1024];
             GetWindowText(hwnd_out_edit_line, outfile, sizeof(outfile));
             
             char offset[1024];
             GetWindowText(hwnd_offset_edit_line, offset, sizeof(offset));
             
             try {
             FileWriter writer = FileWriter();
             writer.writeFile(offsetInFile, outfile, offset);
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
     
     
int WINAPI OffsetFileWindow::WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil){
     HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    HWND hwnd_out_label, hwnd_offset_label, hwnd_OK_button;
    char szClassName[ ] = "Find a word";
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = OffsetWindowProcedure;      /* This function is called by windows */
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
   hwnd_out_label = CreateWindow("static", "ST_U",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              x, y, w, h,
                              hwnd, (HMENU)(306),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_out_label, "Name of output file:");

   ShowWindow (hwnd_out_label, nFunsterStil);

   x += w; w = 500;
   hwnd_out_edit_line = CreateWindow("edit", "",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP
                              | ES_LEFT | WS_BORDER,
                              x, y, w, h,
                              hwnd, (HMENU)(offset_out_edit_line_id),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_out_edit_line, "");
   ShowWindow (hwnd_out_edit_line, nFunsterStil);
   
      hwnd_offset_label = CreateWindow("static", "ST_U",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              x, y, w, h,
                              hwnd, (HMENU)(307),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_offset_label, "Offset:");

   ShowWindow (hwnd_offset_label, nFunsterStil);

   x += w; w = 500;
   hwnd_offset_edit_line = CreateWindow("edit", "",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP
                              | ES_LEFT | WS_BORDER,
                              x, y, w, h,
                              hwnd, (HMENU)(offset_offset_edit_line_id),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_offset_edit_line, "");
   ShowWindow (hwnd_offset_edit_line, nFunsterStil);
   
   w = 60;
   y += 2*h;
   hwnd_OK_button = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), 
   TEXT("OK"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
   x, y, w, h, hwnd, (HMENU)(offset_ok_id), GetModuleHandle(NULL), NULL);
   ShowWindow (hwnd_OK_button, nFunsterStil);                            
}


