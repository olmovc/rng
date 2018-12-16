#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <wincrypt.h>

#define	MAX_BUFF_LEN	500
#define	COLS_COUNT	20
#define TIMER_1		1
/*  Declare Windows procedure 1 */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

int GetRNG();
//int OpenFileDialog(HWND);
//void ReadFileToBuffer();
/*  Make the class name into a global variable  */
wchar_t filename[MAX_PATH];
unsigned char buffer[MAX_BUFF_LEN] = {0};
unsigned char buffer2[10] = {0,1,2,3,5,9,11,12,13,14};
TCHAR szClassName[ ] = _T("view random number<255 timer=2,5 sec v=0,1,2,3,5,9,11,12,13,14.test only.");
static HCRYPTPROV hCryptProv;
int indexOf(unsigned char *array, int array_size, unsigned char number);



int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
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
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

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
	static int cxChar, cxCaps, cyChar, x, y;
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	char strbuff[4] = {'\0'};
	//int x,y;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        //OpenFileDialog(hwnd);
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc,&tm);
        cxChar = tm.tmAveCharWidth ;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
        cyChar = tm.tmHeight + tm.tmExternalLeading ;
        ReleaseDC (hwnd, hdc);
        SetTimer(hwnd,TIMER_1,2000,NULL);
        break;
			case WM_PAINT:
				//sprintf(strbuff,"%3u",221);
				BeginPaint(hwnd,&ps);
				SetBkColor(ps.hdc, RGB(0, 0, 0));
				SetTextColor(ps.hdc, RGB(5, 78, 188));
				SetTextAlign(ps.hdc,TA_RIGHT);
				x = 3*cxCaps;
				y = cyChar;
				for (int i =0; i < MAX_BUFF_LEN; ++i)
				{
					if (indexOf(buffer2,10,buffer[i]) != -1)
					{
						SetTextColor(ps.hdc, RGB(255, 108, 18));
					}
					else
					{
						SetTextColor(ps.hdc, RGB(5, 78, 188));
					}
					sprintf(strbuff,"%3u",buffer[i]);
					if (i % COLS_COUNT == 0)
					{
						x = 3*cxCaps;
						y += cyChar;
					}
					x += 3*cxCaps;
					TextOutA(ps.hdc, x , y, strbuff, 3);
				}
				x = 6*cxCaps;
				y = 0;
				//SetBkColor(ps.hdc, RGB(125, 120, 120));

				SetTextColor(ps.hdc, RGB(255, 178, 188));
				for(int i = 0; i < COLS_COUNT; ++i)
				{
					sprintf(strbuff,"%3u",i);
					//x += 3*cxCaps;
					TextOutA(ps.hdc,x ,0, strbuff, 3);
						x += 3*cxCaps;
					//= 3*cxCaps;
				}

				//SetBkColor(ps.hdc, RGB(0, 0, 0));
				SetTextColor(ps.hdc, RGB(155, 178, 188));
				SetTextAlign(ps.hdc,TA_LEFT);
				y = cyChar;
				for(int i = 0; i < MAX_BUFF_LEN/ COLS_COUNT; ++i)
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
//				if (GetRNG() > 0)
//					MessageBoxW(hwnd,L"err",L"ERRRRR",0);
				//InvalidateRect(hwnd, NULL, FALSE);
				InvalidateRect(hwnd, NULL, TRUE);
				break;

			case WM_DESTROY:
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

    if (!CryptGenRandom(hCryptProv, MAX_BUFF_LEN, buffer))
    {
        return 1;
    }
    return 0;

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
