#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif


#define WIN32_LEAN_AND_MEAN
//#define _CRT_RAND_S


#include <tchar.h>
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wincrypt.h>
#include <time.h>
#include "resource.h"

//#define	MAX_BUFF_LEN	500
#define	MAX_STR_BUFF	11
#define MAX_FREQ			5000
#define MIN_FREQ			100
#define	MAX_COLS			30
#define	MIN_COLS			11
#define	MAX_ROWS			30
#define	MIN_ROwS			11
#define MAX_VALUE			255
#define TIMER_1				1
#define ID_BUTTON1		4777
/*  Declare Windows procedure 1 */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI DDlgProc(HWND, UINT, WPARAM, LPARAM);
static int GetSpinValue(WPARAM,LPARAM,int,int);
int indexOf(unsigned char *array, int array_size, unsigned char number);
COLORREF ShowColorDlg(HWND);
static int GetRNG();
static void GenarateRange();

/*  Make the class name into a global variable  */
//wchar_t filename[MAX_PATH];
//unsigned char buffer[MAX_BUFF_LEN] = {0};
unsigned char * buffer;// = {0};
unsigned char buffer2[MAX_STR_BUFF] = {0,1,2,3,4,5,6,7,8,9};
TCHAR szClassName[ ] = _T("view random number<255 timer interval[100...5000] msec.");
static HCRYPTPROV hCryptProv;
static HANDLE ghInstance;
static HWND hwndMain;
static int TIMER_INTERVAL = 100;
static int colscount = 20; //colscount
static int rowscount = 10;
static int maxbufsize;// = colscount * rowscount;
 //MAX_BUFF_LEN / colscount;
static unsigned char maxValue = MAX_VALUE;
COLORREF fColor = RGB(188,5,11);


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
		InitCommonControls();
		//INITCOMMONCONTROLSEX icc;
		/* Initialize common controls. Also needed for MANIFEST's */
    /*
     * TODO: set the ICC_???_CLASSES that you need.
     */
    //icc.dwSize = sizeof(icc);
    //icc.dwICC = ICC_WIN95_CLASSES /*|ICC_COOL_CLASSES|ICC_DATE_CLASSES|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES|... */;
    //InitCommonControlsEx(&icc);
    //Ini
    /* The Window structure */
    ghInstance = hThisInstance;
    wincl.hInstance = ghInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;          /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT + 1);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           szClassName,       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           ghInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
		hwndMain = hwnd;
		//srand( time(NULL) ); //seeding for the first time only!
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
	static int cxChar, cxCaps, cyChar, x, y;//, cyClient;//, rcount = 0;iVscrollPos,
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	int count = 0;
	char strbuff[5] = {'\0'};
	//int x,y;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        //OpenFileDialog(hwnd);
        CreateWindowW(L"Button",L"O",WS_VISIBLE | WS_CHILD,0,0,20,20,hwnd,(HMENU)ID_BUTTON1,NULL,NULL);

        hdc = GetDC(hwnd);
        GetTextMetrics(hdc,&tm);
        cxChar = tm.tmAveCharWidth;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
        cyChar = tm.tmHeight + tm.tmExternalLeading ;
        ReleaseDC (hwnd, hdc);
        SetTimer(hwnd,TIMER_1,TIMER_INTERVAL,NULL);
        maxbufsize = colscount * rowscount;
				buffer = (unsigned char *)malloc(maxbufsize);
				srand((unsigned int)time(NULL));
				//rowscount = MAX_BUFF_LEN/colscount;
        //SetScrollRange (hwnd, SB_VERT, 0, rowscount, FALSE) ;
				//SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE) ;
        break;

//		case WM_SIZE:
//			cyClient = HIWORD(lParam);
//			return 0;

//		case WM_VSCROLL:
//			switch (LOWORD(wParam))
//			{
//			case SB_LINEUP:
//				iVscrollPos -= 1;
//				break;
//
//			case SB_LINEDOWN:
//				iVscrollPos += 1;
//				break;
//
//			case SB_PAGEUP:
//				iVscrollPos -= cyClient / cyChar;
//				break;
//
//			case SB_PAGEDOWN:
//				iVscrollPos += cyClient / cyChar;
//				break;
//
//			case SB_THUMBPOSITION:
//				iVscrollPos = HIWORD(wParam);
//
//			default:
//				break;
//			}
//
//			iVscrollPos = max(0, min(iVscrollPos,rowscount - 1));
//			if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))
//			{
//				SetScrollPos(hwnd,SB_VERT,iVscrollPos,TRUE);
//				InvalidateRect(hwnd, NULL,TRUE);
//			}
//			return 0;

		case WM_COMMAND:
			if (LOWORD(wParam) == ID_BUTTON1) {
					DialogBox(ghInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DDlgProc);
			}
			break;

			case WM_PAINT:
				//sprintf(strbuff,"%3u",221);
				//int rcount = 0;
				count = 0;
				BeginPaint(hwnd,&ps);
				SetBkColor(ps.hdc, RGB(0, 0, 0));
				SetTextColor(ps.hdc, RGB(5, 78, 188));
				SetTextAlign(ps.hdc,TA_RIGHT);
				x = 6*cxCaps;
				y = 2*cyChar;

				for (int i = 0; i < colscount; ++i)
				{
					for (int j = 0; j < rowscount; ++j)
						{
							if (indexOf(buffer2,MAX_STR_BUFF,buffer[count]) != -1)
								{
									SetTextColor(ps.hdc, RGB(255, 108, 18));
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

//				for (int i =0; i < MAX_BUFF_LEN; ++i)
//				{
//					//y = cyChar*(i - iVscrollPos);
//					if (indexOf(buffer2,10,buffer[i]) != -1)
//					{
//						SetTextColor(ps.hdc, RGB(255, 108, 18));
//					}
//					else
//					{
//						SetTextColor(ps.hdc, fColor);
//					}
//					//y = cyChar * (i − iVscrollPos) ;
//					sprintf(strbuff,"%3u",buffer[i]);
//					if (i % colscount == 0)
//					{
//						x = 3*cxCaps;
//						//y = cyChar;
//						//rcount++;
//						y += cyChar;//*(rcount - iVscrollPos + 1);//cyChar;
//					}
//					//y = cyChar*(i - iVscrollPos);
//					x += 3*cxCaps;
//					//y = cyChar*(i - iVscrollPos);
//					TextOutA(ps.hdc, x , y, strbuff, 3);
//					//y = cyChar*(i - iVscrollPos + 1);
//				}
				x = 6*cxCaps;
				y = 0;
				//SetBkColor(ps.hdc, RGB(125, 120, 120));

				SetTextColor(ps.hdc, RGB(255, 178, 188));
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
				break;

			case WM_TIMER:
				GetRNG();
				GenarateRange();

				InvalidateRect(hwnd, NULL, TRUE);
				break;

			case WM_DESTROY:
				free(buffer);
				KillTimer(hwnd,TIMER_1);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

int GetRNG()
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

//void GetRNG2()
//{
//	for(int i = 0; i < maxbufsize; i++)
//	{
//		buffer[i] = (unsigned char)(rand() % maxValue);
//	}
//}
//
void GenarateRange()
{
  //unsigned char n;
  for (int i = 0;i < maxbufsize;i++)
	{
		if (buffer[i] > maxValue)
			buffer[i] = (unsigned char)(rand() % maxValue);
	}
  //rand
}

int indexOf(unsigned char *array, int array_size, unsigned char number)
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


static LRESULT CALLBACK DDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char szbuff[128];// = {}
//    LPNMUPDOWN lpnmud;
			UINT code;

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        //HWND hedit = GetDlgItem(hDlg,IDC_EDIT1);
        sprintf(szbuff,
                "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu",
                buffer2[0],buffer2[1],buffer2[2],buffer2[3],buffer2[4],buffer2[5],buffer2[6],buffer2[7],buffer2[8],buffer2[9]);
        SetDlgItemTextA(hDlg,IDC_EDIT1,szbuff);
        SetDlgItemTextW(hDlg,IDC_STATIC3,L"Rows");
        sprintf(szbuff,"%d",colscount);
        SetDlgItemTextA(hDlg,IDC_STATIC2,szbuff);


        SendDlgItemMessage(hDlg,IDC_SPIN1,UDM_SETRANGE,0,(LPARAM)MAKELONG(MAX_FREQ,MIN_FREQ));
        SendDlgItemMessage(hDlg,IDC_SPIN1,UDM_SETPOS,0,(LPARAM)MAKELONG(TIMER_INTERVAL,0));
				SetDlgItemTextW(hDlg,IDC_STATIC8,L"Timer");
        sprintf(szbuff,"%d", TIMER_INTERVAL);
        SetDlgItemTextA(hDlg,IDC_LABEL1,szbuff);

        SendDlgItemMessage(hDlg,IDC_SPIN2,UDM_SETRANGE,0,(LPARAM)MAKELONG(MAX_COLS,MIN_COLS));
        SendDlgItemMessage(hDlg,IDC_SPIN2,UDM_SETPOS,0,(LPARAM)MAKELONG(colscount,0));

        SendDlgItemMessage(hDlg,IDC_SPIN4,UDM_SETRANGE,0,(LPARAM)MAKELONG(MAX_ROWS,MIN_ROwS));
        SendDlgItemMessage(hDlg,IDC_SPIN4,UDM_SETPOS,0,(LPARAM)MAKELONG(rowscount,0));
				SetDlgItemTextW(hDlg,IDC_STATIC6,L"Cols");
        sprintf(szbuff,"%d", rowscount);
        SetDlgItemTextA(hDlg,IDC_STATIC7,szbuff);


        SendDlgItemMessage(hDlg,IDC_SPIN3,UDM_SETRANGE,0,(LPARAM)MAKELONG(MAX_VALUE,1));
        SendDlgItemMessage(hDlg,IDC_SPIN3,UDM_SETPOS,0,(LPARAM)MAKELONG(MAX_VALUE,0));
				SetDlgItemTextW(hDlg,IDC_STATIC4,L"Max value");
        sprintf(szbuff,"%d", maxValue);
        SetDlgItemTextA(hDlg,IDC_STATIC5,szbuff);

        //MessageBoxA(NULL,(LPCSTR)szbuff,"gfgf",0);
        //swprintf swscanf
        return TRUE;
    }
    case WM_NOTIFY:
    {
        switch (LOWORD(wParam))
        {
        case IDC_SPIN1:
        {
            code = ((LPNMHDR) lParam)->code;
            if (code == UDN_DELTAPOS)
            {
//                lpnmud = (NMUPDOWN *) lParam;
//                int v = lpnmud->iPos + lpnmud->iDelta;
//                if (v < MIN_FREQ)
//                    v = MIN_FREQ;
//                if (v > MAX_FREQ)
//                    v = MAX_FREQ;
								//int v
								TIMER_INTERVAL = GetSpinValue(wParam, lParam, MAX_FREQ,MIN_FREQ);
                sprintf(szbuff,"%d",TIMER_INTERVAL);
                //T//IMER_INTERVAL = v;
                SetDlgItemTextA(hDlg,IDC_LABEL1,szbuff);
                //MessageBoxA(0,szbuff,"fsgfsdgds",0);
            }
            break;
        }
        case IDC_SPIN2:
					{
            code = ((LPNMHDR) lParam)->code;
            if (code == UDN_DELTAPOS)
            {
								colscount = GetSpinValue(wParam, lParam,MAX_COLS,MIN_COLS);
								//rowscount = MAX_BUFF_LEN/colscount;
                sprintf(szbuff,"%d",colscount);
                maxbufsize = rowscount*colscount;
                buffer = (unsigned char *)realloc(buffer, maxbufsize);
                //colscount = v;
                SetDlgItemTextA(hDlg,IDC_STATIC2,szbuff);
                //MessageBoxA(0,szbuff,"fsgfsdgds",0);
            }
            break;
					}
/////////////////////////////
        case IDC_SPIN4:
					{
            code = ((LPNMHDR) lParam)->code;
            if (code == UDN_DELTAPOS)
            {
								rowscount = GetSpinValue(wParam, lParam,MAX_COLS,MIN_COLS);
								//rowscount = MAX_BUFF_LEN/colscount;
                sprintf(szbuff,"%d",rowscount);
                maxbufsize = rowscount*colscount;
                buffer = (unsigned char *)realloc(buffer, maxbufsize);
                //colscount = v;
                SetDlgItemTextA(hDlg,IDC_STATIC7,szbuff);
                //MessageBoxA(0,szbuff,"fsgfsdgds",0);
            }
            break;
					}
					////////////////
        case IDC_SPIN3:
					{
            code = ((LPNMHDR) lParam)->code;
            if (code == UDN_DELTAPOS)
            {
								maxValue = (unsigned char)GetSpinValue(wParam, lParam,MAX_VALUE,0);
								//rowscount = MAX_BUFF_LEN/colscount;
                sprintf(szbuff,"%hhu",maxValue);
                //colscount = v;
                SetDlgItemTextA(hDlg,IDC_STATIC5,szbuff);
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
            char textbuff[50] = {'\0'};
            GetDlgItemTextA(hDlg,IDC_EDIT1,textbuff,49);
            char * t;
            int i = 0;
            t = strtok(textbuff," ,\t");
            while ((t != NULL) && ( i < MAX_STR_BUFF ))
            {
                buffer2[i++] = (unsigned char) atoi(t);
                t = strtok(NULL," ,\t");
            }
            //sscanf(textbuff,
            //			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            //				buffer2[0],buffer2[1],buffer2[2],buffer2[3],buffer2[4],buffer2[5],buffer2[6],buffer2[7],buffer2[8],buffer2[9]);
            KillTimer(hwndMain,TIMER_1);
            SetTimer(hwndMain,TIMER_1,(UINT)TIMER_INTERVAL,NULL);
            EndDialog(hDlg, TRUE);
            return TRUE;
        }

        case IDCANCEL:
            EndDialog(hDlg, TRUE);
            return TRUE;

				case IDC_BUTTON1:
					fColor = ShowColorDlg(hDlg);
					break;

        }
        break;
    }

    return FALSE;
}

COLORREF ShowColorDlg(HWND hwnd)
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
