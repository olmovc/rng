/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic Win32 application.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include "main.h"

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))
#define M_ROWS	50
#define M_COLS	20
#define BUFF_LEN	M_ROWS*M_COLS

/** Prototypes **************************************************************/

static LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
static void Main_OnPaint(HWND);
static void Main_OnCommand(HWND, int, HWND, UINT);
static void Main_OnDestroy(HWND);
static LRESULT WINAPI AboutDlgProc(HWND, UINT, WPARAM, LPARAM);


BOOL OpenBinFile(HWND);
int ReadBinToBuf(void);
/** Global variables ********************************************************/

static HANDLE ghInstance;
char filename[MAX_PATH];

unsigned char buffer[BUFF_LEN];
int countPart = 0;
int currentPart = 0;


/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    ghInstance = hInstance;

    /* Initialize common controls. Also needed for MANIFEST's */
    /*
     * TODO: set the ICC_???_CLASSES that you need.
     */
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES /*|ICC_COOL_CLASSES|ICC_DATE_CLASSES|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES|... */;
    InitCommonControlsEx(&icc);

    /* Load Rich Edit control support */
    /*
     * TODO: uncomment one of the lines below, if you are using a Rich Edit control.
     */
    // LoadLibrary(_T("riched32.dll"));  // Rich Edit v1.0
    // LoadLibrary(_T("riched20.dll"));  // Rich Edit v2.0, v3.0

    /*
     * TODO: uncomment line below, if you are using the Network Address control (Windows Vista+).
     */
    // InitNetworkAddressControl();

    /* Register the main window class */
    wc.lpszClassName = _T("viewbinClass");
    wc.lpfnWndProc = MainWndProc;
    wc.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    wc.hInstance = ghInstance;
    wc.hIcon = LoadIcon(ghInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MNU_MAIN);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    if (!RegisterClass(&wc))
        return 1;

    /* Create the main window */
    hwnd = CreateWindow(_T("viewbinClass"),
        _T("viewbin"),
        WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
        0,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        ghInstance,
        NULL
    );
    if (!hwnd) return 1;

    /* Show and paint the main window */
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* Pump messages until we are done */
#if 0
    /* "Politically correct" code -- SEE MICROSOFT DOCUMENTATION */
    for (;;)
    {
        BOOL fRet = GetMessage(&msg, NULL, 0, 0);
        if (fRet == -1)  /* Error */
        {
            /* TODO: handle the error from GetMessage() */
            __debugbreak();
            return -1;
        }
        else if (fRet == 0)  /* WM_QUIT */
        {
            break;
        }
        else  /* Not error or WM_QUIT */
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
#else
    /* "Commonly seen" code */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#endif

    return msg.wParam;
}

/****************************************************************************
 *                                                                          *
 * Function: MainWndProc                                                    *
 *                                                                          *
 * Purpose : Process application messages.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_MSG(hwnd, WM_PAINT, Main_OnPaint);
        HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
        HANDLE_MSG(hwnd, WM_DESTROY, Main_OnDestroy);
        /* TODO: enter more messages here */
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnPaint                                                   *
 *                                                                          *
 * Purpose : Process a WM_PAINT message.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnPaint(HWND hwnd)
{
	if ((buffer[0] == 0) && (buffer[1] == 0) && (buffer[2] == 0))
	{
		return;
	}

	PAINTSTRUCT ps;
	//RECT rc;

	BeginPaint(hwnd, &ps);
	SetBkColor(ps.hdc, RGB(0, 0, 0));
	SetTextColor(ps.hdc, RGB(55, 78, 188));
	TextOut(ps.hdc, 0, 15, "hello_world", 8);
//BOOL TextOut(
	//HDC hdc,           // handle to DC
	//int nXStart,       // x-coordinate of starting position
	//int nYStart,       // y-coordinate of starting position
	//LPCTSTR lpString,  // character string
	//int cbString       // number of characters
//);

	//GetClientRect(hwnd, &rc);
	//DrawText(ps.hdc, _T("Hello, Windows!"), -1, &rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	EndPaint(hwnd, &ps);
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnCommand                                                 *
 *                                                                          *
 * Purpose : Process a WM_COMMAND message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case IDM_ABOUT:
			DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
			return;
		case IDR_MNU_OPEN:
			if (OpenBinFile(hwnd) == FALSE)
			{
				return;
			}
			if (ReadBinToBuf() > 0)
			MessageBoxA(hwnd, filename, "ddffd", 0);
			break;

		/* TODO: Enter more commands here */
	}
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnDestroy                                                 *
 *                                                                          *
 * Purpose : Process a WM_DESTROY message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

/****************************************************************************
 *                                                                          *
 * Function: AboutDlgProc                                                   *
 *                                                                          *
 * Purpose : Process messages for the About dialog.  The dialog is          *
             shown when the user selects "About" in the "Help" menu.        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            /*
             * Nothing special to initialize.
             */
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
            {
                case IDOK:
                case IDCANCEL:
                    /*
                     * OK or Cancel was clicked, close the dialog.
                     */
                    EndDialog(hDlg, TRUE);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}
/*

*/
BOOL OpenBinFile(HWND hwnd)
{
	OPENFILENAME ofn;	// common dialog box structure
	char szFile[MAX_PATH];	// buffer for file name
	//HWND hwnd;              // owner window
	//HANDLE hf;	// file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "bin\0*.bin\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) != TRUE)
		return FALSE;

	strcpy(filename, ofn.lpstrFile);
	return TRUE;
	//if (GetOpenFileName(&ofn)==TRUE) 
	//hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
	//0, (LPSECURITY_ATTRIBUTES) NULL,
	//OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	//(HANDLE) NULL);

}

int ReadBinToBuf(void)
{
 FILE * pFile = fopen(filename,"rb");
  //if (pFile == NULL)
  //{
		//buffer[0] = 0;
		//buffer[1] = 0;
		//buffer[2] = 0;
    //fclose(pFile);
		//return 0;
  //}
	  // obtain file size:
	
  fseek(pFile , 0, SEEK_END);

  long lSize = ftell(pFile);
	//if ((int)lSize > BUFF_LEN) {
		//fclose(pFile);
		//return 0;
	//}
	int iSize = (int)lSize;
  fseek(pFile , 0 , SEEK_SET);
	countPart = iSize / 1000;

	if (currentPart > countPart)
		currentPart = 0;

	currentPart++;
	fseek(pFile , currentPart*BUFF_LEN , SEEK_SET);
	size_t result;
	result = fread(buffer,1,BUFF_LEN,pFile);
	if (result != BUFF_LEN) {
		fclose(pFile);
		return 0;
	}
 
	fclose(pFile);
  return 1;
}
