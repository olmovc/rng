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
#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN  /* speed up compilations */

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wincrypt.h>
#include <time.h>
#include <string.h>
#include "main.h"

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Prototypes **************************************************************/

static LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
//static void Main_OnPaint(HWND);
//static void Main_OnCommand(HWND, int, HWND, UINT);
//static void Main_OnDestroy(HWND);
static LRESULT WINAPI AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
static int GetSpinValue(WPARAM,LPARAM,int,int);
static int indexOf(unsigned char *array, int array_size, unsigned char number);
static COLORREF ShowColorDlg(HWND);
static int GetRNG(void);
static void GenarateRange(void);
//void OpenFDialog(HWND);
//void LoadFile(int,int);

/** Global variables ********************************************************/
#define	MAX_STR_BUFF		10
#define MAX_FREQ			5000
#define MIN_FREQ			100
#define	MAX_COLS			30
#define	MIN_COLS			11
#define	MAX_ROWS			30
#define	MIN_ROwS			11
#define MAX_VALUE			255
#define TIMER_1				1
#define ID_BUTTON1			4777

static HANDLE ghInstance;
unsigned char * buffer;// = {0};
unsigned char buffer2[MAX_STR_BUFF] = {0,1,2,3,4,5,6,7,8,9};
//TCHAR szClassName[ ] = _T("view random number<255 timer interval[100...5000] msec.");
static HCRYPTPROV hCryptProv;
static HANDLE ghInstance;
static HWND hwndMain;
static int TIMER_INTERVAL = 100;
static int colscount = 20; //colscount
static int rowscount = 10;
static int maxbufsize;// = colscount * rowscount;
 //MAX_BUFF_LEN / colscount;
static unsigned char maxValue = MAX_VALUE;
static COLORREF fColor = RGB(188,5,11);
static COLORREF fColor2 = RGB(255, 108, 18);//RGB(188,5,11);
static COLORREF fColor3 = RGB(255, 178, 188);
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
    wc.lpszClassName = _T("viewbin2");
    wc.lpfnWndProc = MainWndProc;
    wc.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    wc.hInstance = ghInstance;
    wc.hIcon = LoadIcon(ghInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNTEXT+1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MNU_MAIN);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    if (!RegisterClass(&wc))
        return 1;

    /* Create the main window */
    hwnd = CreateWindow(_T("viewbin2"),
        _T("view random number"),
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
	hwndMain = hwnd;
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
	static int cxChar, cxCaps, cyChar, x, y;	//, cyClient;//, rcount = 0;iVscrollPos,
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	int count = 0;
	char strbuff[5] = { '\0' };

	switch (msg)
	{
			//HANDLE_MSG(hwnd, WM_PAINT, Main_OnPaint);
			//HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
			//HANDLE_MSG(hwnd, WM_DESTROY, Main_OnDestroy);
			/* TODO: enter more messages here */
		case WM_CREATE:
			//CreateWindowW(L"Button", L"O", WS_VISIBLE | WS_CHILD, 0, 0, 20, 20, hwnd, (HMENU)ID_BUTTON1, NULL, NULL);

			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			ReleaseDC(hwnd, hdc);
			SetTimer(hwnd, TIMER_1, TIMER_INTERVAL, NULL);
			maxbufsize = colscount * rowscount;
			buffer = (unsigned char *)malloc(maxbufsize);
			srand((unsigned int)time(NULL));
			return 0;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDM_START:
					SetTimer(hwnd, TIMER_1, TIMER_INTERVAL, NULL);
					break;
				case IDM_ABOUT:
					DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
					break;
				case IDM_STOP:
					KillTimer(hwnd, TIMER_1);
					break;
 			//if (LOWORD(wParam) == IDM_ABOUT)
			//{
        		////case IDM_ABOUT:
				////DLG
            	//DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
				////DialogBox(ghInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DDlgProc);
			}
			break;

		case WM_PAINT:
				count = 0;
				BeginPaint(hwnd,&ps);
				SetBkColor(ps.hdc, RGB(0, 0, 0));
				//SetTextColor(ps.hdc, RGB(5, 78, 188));
				SetTextAlign(ps.hdc,TA_RIGHT);
				x = 6*cxCaps;
				y = 2*cyChar;

				for (int i = 0; i < colscount; ++i)
				{
					for (int j = 0; j < rowscount; ++j)
						{
							if (indexOf(buffer2,MAX_STR_BUFF,buffer[count]) != -1)
								{
									SetTextColor(ps.hdc, fColor2);
								}	else {
									SetTextColor(ps.hdc, fColor);
								}
							//printf("r=%d,c=%d,m=%d,c=%d",rowscount,colscount,maxbufsize,count);
						sprintf(strbuff,"%3u",buffer[count]);
						TextOutA(ps.hdc, x , y, strbuff, 3);
						count++;
						x += 3*cxCaps;//*(1 + j);
						}
				y += cyChar;//*(rcount - iVscrollPos + 1);//cyChar;
				x = 6*cxCaps;
				}
				x = 6*cxCaps;
				y = 0;
				//SetBkColor(ps.hdc, RGB(125, 120, 120));

				SetTextColor(ps.hdc, fColor3);
				for(int i = 0; i < rowscount; ++i)
				{
					sprintf(strbuff,"%3u",i);
					//x += 3*cxCaps;
					TextOutA(ps.hdc,x ,0, strbuff, 3);
						x += 3*cxCaps;
					//= 3*cxCaps;
				}

				//SetBkColor(ps.hdc, RGB(0, 0, 0));
				//SetTextColor(ps.hdc, RGB(155, 178, 188));
				SetTextAlign(ps.hdc,TA_LEFT);
				y = cyChar;
				for(int i = 0; i < colscount; ++i)
				{
					y += cyChar;
					sprintf(strbuff,"%3u",i);
					TextOutA(ps.hdc, 0,y, strbuff, 3);\

					//x += 3*cxCaps;
				}
				//TextOutA(ps.hdc, 1, 15, strbuff, 3);
				EndPaint(hwnd, &ps);
			return 0;
		case WM_TIMER:
			GetRNG();
			GenarateRange();

			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_DESTROY:
			free(buffer);
			KillTimer(hwnd, TIMER_1);
			PostQuitMessage(0);	/* send a WM_QUIT to the message queue */
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnPaint                                                   *
 *                                                                          *
 * Purpose : Process a WM_PAINT message.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 //static void Main_OnPaint(HWND hwnd)
//{
    //PAINTSTRUCT ps;
    //RECT rc;

    //BeginPaint(hwnd, &ps);
    //GetClientRect(hwnd, &rc);
    //DrawText(ps.hdc, _T("Hello, Windows!"), -1, &rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    //EndPaint(hwnd, &ps);
//}                                                                          *
 ****************************************************************************/



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

//static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
//{
    //switch (id)
    //{
        //case IDM_ABOUT:
            //DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
			//return;
        ///* TODO: Enter more commands here */
		//// case IDM_OPEN:
			////OpenFDialog(hwnd);
    //}
//}

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

//static void Main_OnDestroy(HWND hwnd)
//{
    //PostQuitMessage(0);
//}

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
	char szbuff[128];
	UINT code;

	switch (uMsg)
	{
		case WM_INITDIALOG:
			sprintf(szbuff, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", buffer2[0], buffer2[1], buffer2[2], buffer2[3], buffer2[4], buffer2[5], buffer2[6], buffer2[7], buffer2[8], buffer2[9]);
			SetDlgItemTextA(hDlg, IDR_EDIT1, szbuff);

			SendDlgItemMessage(hDlg, IDR_SPIN_TIMER, UDM_SETRANGE, 0, (LPARAM)MAKELONG(MAX_FREQ, MIN_FREQ));
			SendDlgItemMessage(hDlg, IDR_SPIN_TIMER, UDM_SETPOS, 0, (LPARAM)MAKELONG(TIMER_INTERVAL, 0));
			sprintf(szbuff, "%d", TIMER_INTERVAL);
			SetDlgItemTextA(hDlg, IDR_LABEL_TIMER, szbuff);

			SendDlgItemMessage(hDlg, IDR_SPIN_COLS, UDM_SETRANGE, 0, (LPARAM)MAKELONG(MAX_COLS, MIN_COLS));
			SendDlgItemMessage(hDlg, IDR_SPIN_COLS, UDM_SETPOS, 0, (LPARAM)MAKELONG(colscount, 0));
			sprintf(szbuff, "%d", colscount);
			SetDlgItemTextA(hDlg, IDR_LABEL_COLS, szbuff);

			SendDlgItemMessage(hDlg, IDR_SPIN_ROWS, UDM_SETRANGE, 0, (LPARAM)MAKELONG(MAX_ROWS, MIN_ROwS));
			SendDlgItemMessage(hDlg, IDR_SPIN_ROWS, UDM_SETPOS, 0, (LPARAM)MAKELONG(rowscount, 0));
			sprintf(szbuff, "%d", rowscount);
			SetDlgItemTextA(hDlg, IDR_LABEL_ROWS, szbuff);

			SendDlgItemMessage(hDlg, IDR_SPIN_MAXV, UDM_SETRANGE, 0, (LPARAM)MAKELONG(MAX_VALUE, 1));
			SendDlgItemMessage(hDlg, IDR_SPIN_MAXV, UDM_SETPOS, 0, (LPARAM)MAKELONG(MAX_VALUE, 0));
			sprintf(szbuff, "%d", maxValue);
			SetDlgItemTextA(hDlg, IDR_LABEL_MAXV, szbuff);
			return TRUE;
		case WM_NOTIFY:
		{
			switch (LOWORD(wParam))
			{
				case IDR_SPIN_TIMER:
				{
					code = ((LPNMHDR)lParam)->code;
					if (code == UDN_DELTAPOS)
					{
						TIMER_INTERVAL = GetSpinValue(wParam, lParam, MAX_FREQ, MIN_FREQ);
						sprintf(szbuff, "%d", TIMER_INTERVAL);
						//T//IMER_INTERVAL = v;
						SetDlgItemTextA(hDlg, IDR_LABEL_TIMER, szbuff);
						//MessageBoxA(0,szbuff,"fsgfsdgds",0);
					}
					break;
				}

				case IDR_SPIN_COLS:
				{
					code = ((LPNMHDR)lParam)->code;
					if (code == UDN_DELTAPOS)
					{
						colscount = GetSpinValue(wParam, lParam, MAX_COLS, MIN_COLS);
						//rowscount = MAX_BUFF_LEN/colscount;
						sprintf(szbuff, "%d", colscount);
						maxbufsize = rowscount * colscount;
						buffer = (unsigned char *)realloc(buffer, maxbufsize);
						//colscount = v;
						SetDlgItemTextA(hDlg, IDR_LABEL_COLS, szbuff);
						//MessageBoxA(0,szbuff,"fsgfsdgds",0);
					}
					break;
				}
/////////////////////////////
				case IDR_SPIN_ROWS:
				{
					code = ((LPNMHDR)lParam)->code;
					if (code == UDN_DELTAPOS)
					{
						rowscount = GetSpinValue(wParam, lParam, MAX_ROWS, MIN_ROwS);
						//rowscount = MAX_BUFF_LEN/colscount;
						sprintf(szbuff, "%d", rowscount);
						maxbufsize = rowscount * colscount;
						buffer = (unsigned char *)realloc(buffer, maxbufsize);
						//colscount = v;
						SetDlgItemTextA(hDlg, IDR_LABEL_ROWS, szbuff);
						//MessageBoxA(0,szbuff,"fsgfsdgds",0);
					}
					break;
				}
				////////////////
				case IDR_SPIN_MAXV:
				{
					code = ((LPNMHDR)lParam)->code;
					if (code == UDN_DELTAPOS)
					{
						maxValue = (unsigned char)GetSpinValue(wParam, lParam, MAX_VALUE, 0);
						//rowscount = MAX_BUFF_LEN/colscount;
						sprintf(szbuff, "%hhu", maxValue);
						//colscount = v;
						SetDlgItemTextA(hDlg, IDR_LABEL_MAXV, szbuff);
						//MessageBoxA(0,szbuff,"fsgfsdgds",0);
					}
					break;
				}

			}
			break;
		}

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
				{
					/*
					 * OK or Cancel was clicked, close the dialog.
					 */
					char textbuff[128] = { '\0' };
					GetDlgItemTextA(hDlg, IDR_EDIT1, textbuff, 49);
					char *t;
					int i = 0;
					t = strtok(textbuff, " ,\t");
					while ((t != NULL) && (i < MAX_STR_BUFF))
					{
						buffer2[i++] = (unsigned char)atoi(t);
						t = strtok(NULL, " ,\t");
					}
					//sscanf(textbuff,
					//          "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
					//              buffer2[0],buffer2[1],buffer2[2],buffer2[3],buffer2[4],buffer2[5],buffer2[6],buffer2[7],buffer2[8],buffer2[9]);
					KillTimer(hwndMain, TIMER_1);
					SetTimer(hwndMain, TIMER_1, (UINT)TIMER_INTERVAL, NULL);
					EndDialog(hDlg, TRUE);
					return TRUE;
				}

				case IDCANCEL:
					EndDialog(hDlg, TRUE);
					return TRUE;

				case IDR_COLOR1:
					fColor = ShowColorDlg(hDlg);
					break;

				case IDR_COLOR2:
					fColor2 = ShowColorDlg(hDlg);
					break;

				case IDR_COLOR3:
					fColor3 = ShowColorDlg(hDlg);
					break;


			}
			break;
	}

	return FALSE;
}


static void GenarateRange(void)
{
  //unsigned char n;
  for (int i = 0;i < maxbufsize;i++)
	{
		if (buffer[i] > maxValue)
			buffer[i] = (unsigned char)(rand() % maxValue);
	}
  //rand
}

static int indexOf(unsigned char *array, int array_size, unsigned char number)
{
    for (int i = 0; i < array_size; ++i)
    {
        if (array[i] == number)
        {
            return i;
        }
    }
    return -1;
}

static int GetSpinValue(WPARAM wParam,LPARAM lParam,int imax,int imin)
{
    LPNMUPDOWN lpnmud;
    //UINT code;
    lpnmud = (NMUPDOWN *) lParam;
    int v = lpnmud->iPos + lpnmud->iDelta;
    if (v < imin)  v = imin;
    if (v > imax)  v = imax;//    (v > MAX_FREQ) ?
		return v;
}


static COLORREF ShowColorDlg(HWND hwnd)
{
    CHOOSECOLOR cc;
    static COLORREF crCustClr[16];
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD) crCustClr;
    cc.rgbResult = RGB(0, 255, 0);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    ChooseColor(&cc);
    return cc.rgbResult;
}

static int GetRNG(void)
{
    //BYTE pbData[MAXLEN];

    if (!CryptAcquireContextW(&hCryptProv, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return 1;

    if (!CryptGenRandom(hCryptProv, maxbufsize, buffer))
    {
        CryptReleaseContext(hCryptProv, 0);
        return 1;
    }

    if (!CryptReleaseContext(hCryptProv, 0))
			return 1;

   // GenarateRange();
    return 0;

}

//void OpenFDialog(HWND hwnd)
//{
//OPENFILENAME ofn;
//TCHAR szFile[MAX_PATH];
//ZeroMemory(&ofn, sizeof(ofn));
//ofn.lStructSize = sizeof(ofn);
//ofn.lpstrFile = szFile;
//ofn.lpstrFile[0] = '\0';
//ofn.hwndOwner = hwnd;
//ofn.nMaxFile = sizeof(szFile);
//ofn.lpstrFilter = TEXT("bin files(*.bin)\0*.bin\0");
//ofn.nFilterIndex = 1;
//ofn.lpstrInitialDir = NULL;
//ofn.lpstrFileTitle = NULL;
//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//GetOpenFileName(&ofn);
 ////if(GetOpenFileName(&ofn))
////LoadFile(ofn.lpstrFile);
	//strcpy(filename,ofn.lpstrFile);
//// file size
	//FILE * file = fopen(filename,"rb");
	//if (file == NULL) 
	//{
		//return;
	//}
	//int size = 0;
	//fseek(file,0,SEEK_END);
	//size = ftell(file);
	//fclose(file);
//}

//void LoadFile(int seek, int count)
//{
	//FILE * file = fopen(filename,"rb");
	//if (file == NULL) 
	//{
		//return;
	//}
	//int size = 0;
	//fseek(file,0,SEEK_END);
	//size = ftell(file);
	//fseek(file,0,SEEK_SET);
	//fclose(file);
//}
