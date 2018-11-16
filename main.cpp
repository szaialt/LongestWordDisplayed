#include <windows.h>
#include <utility>
#include <string.h>
#include <sstream>
#include "exceptions.h"
#include "filereader.h"
#include "filewriter.h"
#include "findworddialog.h"
#include "offsetfiledialog.h"

using namespace std;

char fileName[1024];

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FindWordWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OffsetWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

string GetInputText( HWND dlg, int resid  ) {
  HWND hc = GetDlgItem( dlg, resid );
  int n = GetWindowTextLength( hc ) + 1;
  string s( n, 0 );
  GetWindowText( hc, &s[0], n );
  return s;
}

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

const int edit_line_id = 502;
static HWND hwnd_edit_line;
const int ok_id = 503;
const int my_word_id = 504;
const int offset_id = 505;

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
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

   static HWND hwnd_name_label, hwnd_OK_button, hwnd_my_word_button;
   static HWND hwnd_offset_button;
   int x, w, y, h;
   y = 20; h = 20;
   x = 10; w = 100;
   hwnd_name_label = CreateWindow("static", "ST_U",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              x, y, w, h,
                              hwnd, (HMENU)(501),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_name_label, "Name of file:");

   ShowWindow (hwnd_name_label, nFunsterStil);

   x += w; w = 500;
   hwnd_edit_line = CreateWindow("edit", "",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP
                              | ES_LEFT | WS_BORDER,
                              x, y, w, h,
                              hwnd, (HMENU)(edit_line_id),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
   SetWindowText(hwnd_edit_line, "example.srt");
   ShowWindow (hwnd_edit_line, nFunsterStil);
   
   w = 60;
   y += 2*h;
   hwnd_OK_button = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), 
   TEXT("OK"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
   x, y, w, h, hwnd, (HMENU)(ok_id), GetModuleHandle(NULL), NULL);
   ShowWindow (hwnd_OK_button, nFunsterStil);
   
   w = 150;
   x += w + 20;
   hwnd_my_word_button = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), 
   TEXT("Search my word..."), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
   x, y, w, h, hwnd, (HMENU)(my_word_id), GetModuleHandle(NULL), NULL);
   ShowWindow (hwnd_OK_button, nFunsterStil);
   
   w = 120;
   x += w + 20;
   hwnd_offset_button = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), 
   TEXT("Offset file..."), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
   x, y, w, h, hwnd, (HMENU)(offset_id), GetModuleHandle(NULL), NULL);
   ShowWindow (hwnd_OK_button, nFunsterStil);
   
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    FileReader reader = FileReader();
    switch (message)                  /* handle the messages */
    {case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			
			case ok_id:
				{
					//Step 5: User click on the button
					if (HIWORD(wParam) == BN_CLICKED)
					{
						UINT nButton	= (UINT) LOWORD(wParam) ;
						HWND hButtonWnd = (HWND) lParam ;

                        
                        GetWindowText(hwnd_edit_line, fileName, sizeof(fileName));

					
				     
					    try {
                            reader.readFile(fileName);
                            char* oftest = reader.findOftestWord().first;
                            int times = reader.findOftestWord().second;
                            stringstream strs;
                            strs << times;
                            string temp_str = strs.str();
                            char* timesString = (char*) temp_str.c_str();
                           
                            MessageBox(hwnd, strcat(strcat(strcat(oftest, " found "), timesString), " times."), "The oftest word", MB_OK) ;
                            }
                        catch(FileNotFoundException ex){
                            MessageBox(hwnd, "File not found.", "fileName", MB_OK) ;
                        }
                    }
				}
				break ;
				case my_word_id:{
                     
                     GetWindowText(hwnd_edit_line, fileName, sizeof(fileName));
                     try {
                         DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_FINDPWDDLG), hwnd, FindWordWindowProcedure);
                     }
                     catch(FileNotFoundException ex){
                            MessageBox(hwnd, "File not found.", "fileName", MB_OK) ;
                        }
                     }
                     break;
                case offset_id:{
                     GetWindowText(hwnd_edit_line, fileName, sizeof(fileName));
                     try {
                         DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OFFSETPWDDLG), hwnd, OffsetWindowProcedure);
                     }
                     catch(FileNotFoundException ex){
                            MessageBox(hwnd, "File not found.", "fileName", MB_OK) ;
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

    return 0;
}

    
BOOL CALLBACK FindWordWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char word[1024] ;
    switch (message)                  /* handle the messages */
    {case WM_COMMAND:
		{
		switch(LOWORD(wParam))
		{
			
			
		case IDOK:{
             UINT nButton	= (UINT) LOWORD(wParam) ;
		     HWND hButtonWnd = (HWND) lParam ;

             const char* word = GetInputText( hwnd, IDC_WORDEDIT ).c_str();
             try {
                 FileReader reader = FileReader();
                 int times = reader.findWord(word);
                 stringstream strs;
                 strs << times;
                 string temp_str = strs.str();
                 char* timesString = (char*) temp_str.c_str();

                 MessageBox(hwnd, strcat(strcat(strcat(const_cast<char*>(word), " found "), timesString), " times."), "Your word", MB_OK) ;
             }
             catch(FileNotFoundException ex){
                            MessageBox(hwnd, "File not found.", "fileName", MB_OK) ;
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

    return 0;
}

BOOL CALLBACK OffsetWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {case WM_COMMAND:
		{
		switch(LOWORD(wParam))
		{
			
			
		case IDOK:{
             UINT nButton	= (UINT) LOWORD(wParam) ;
		     HWND hButtonWnd = (HWND) lParam ;
            
             const char* offset = GetInputText( hwnd, IDC_OFFSETEDIT ).c_str();
             
             try {
             FileWriter writer = FileWriter();
             writer.overWriteFile(fileName, offset);
             }
             catch(FileNotFoundException ex){
                            MessageBox(hwnd, "File not found.", "fileName", MB_OK) ;
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

    return 0;
}

