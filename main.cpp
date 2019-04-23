#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE

#endif
#include <cmath>
#include <math.h>
#include <tchar.h>
#include <windows.h>
#define FILE_MENU_Line 20
#define DDA 11
#define Midpoint 12
#define Parametric 3
#define Direct 14
#define Polar 5
#define Mid 6
#define bezier 7
#define hermite 8
#define splines 9
//#define FILE_MENU_Ellipse 2
//#define FILE_MENU_ Curve 3
#define FILE_MENU_EXIT 4
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddMenus (HWND);
HMENU hMenu;
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

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
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
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
void swap(int& x,int& y)
{
int tmp=x;
x=y;
y=tmp;
}
double round(double x)
{
return (double)(x+0.5);
}
void SimpleDDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
   int dx=xe-xs;
   int dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
   if(abs(dx)>=abs(dy))
 {
  int x=xs,xinc= dx>0?1:-1;
  double y=ys,yinc=(double)dy/dx*xinc;
  while(x!=xe)
 {
  x+=xinc;
  y+=yinc;
  SetPixel(hdc,x,round(y),color);
 }
 }
 else
{
  int y=ys,yinc= dy>0?1:-1;
  double x=xs,xinc=(double)dx/dy*yinc;
 while(y!=ye)
{
   x+=xinc;
   y+=yinc;
   SetPixel(hdc,round(x),y,color);
}
}
}
void midPoint(HDC hdc,int X1, int Y1, int X2, int Y2,COLORREF color)
{
    int dx = X2 - X1;
    int dy = Y2 - Y1;
    int d = dy - (dx/2);
    int x = X1, y = Y1;
   SetPixel(hdc,x,y,color);


    while (x < X2)
    {
        x++;


        if (d < 0)
            d = d + dy;


        else
        {
            d += (dy - dx);
            y++;
        }

        SetPixel(hdc,x,y,color);

    }
}
void parametric_line(HDC hdc,int x1,int x2,int y1,int y2,COLORREF color ){
 for(double t=0;t<=1;t+=0.001){
    int x=round(x1+t*(x2-x1));
    int y=round(y1+t*(y2-y1));
    SetPixel(hdc,x,y,color);
 }

}
void draw_direct_elipse(HDC hdc,int xc,int yc,int A,int B,COLORREF color){
for(double theta=0;theta<6.28;theta+=0.001)
{

    int x=round(xc+A*cos(theta));
    int y=round(yc+B*sin(theta));
    SetPixel(hdc,x,y,color);
}
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static int xs,ys,xe,ye, A,B;
    switch (message)                  /* handle the messages */
    {
        case DDA:
                case WM_LBUTTONDOWN:
                     xs=LOWORD(lParam);
                     ys=HIWORD(lParam);
                     break;
                case WM_LBUTTONUP:
                     xe=LOWORD(lParam);
                     ye=HIWORD(lParam);
                     HDC hdc;
                     hdc =GetDC(hwnd);
                     SimpleDDA( hdc, xs, ys, xe, ye,RGB(255,0,0));
                     ReleaseDC(hwnd,hdc);
                     break;
            break;

    case Midpoint:
                     midPoint( hdc, xs, ys, xe, ye,RGB(255,0,0));

                     break;
       case Parametric:
                   parametric_line(hdc,xs,xe,ys,ye,RGB(255,0,0));
                   break;
       case Direct:
                 A=100;
                 B=50;
                 draw_direct_elipse(hdc,xe,ye,B,A,RGB(255,0,0));
                 break;

    case WM_COMMAND:

        switch (wParam)
        {


        case FILE_MENU_EXIT:
                 DestroyWindow(hwnd);
                 break;
        }



        break;
        case WM_CREATE:
            AddMenus(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void AddMenus(HWND hwnd)
{

    hMenu =CreateMenu();
    HMENU hFileMenu =CreateMenu();
    HMENU hFileMenu2 =CreateMenu();
    HMENU hFileMenu3 =CreateMenu();
    HMENU hSubMenu =CreateMenu();
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"Draw Line");
    AppendMenu(hFileMenu,MF_STRING,DDA,"DDA");
    AppendMenu(hFileMenu,MF_STRING,Midpoint,"Midpoint");
    AppendMenu(hFileMenu,MF_STRING,Parametric,"Parametric");
    AppendMenu(hFileMenu,MF_STRING,FILE_MENU_EXIT,"Exit");

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu2,"Draw Ellipse");
    AppendMenu(hFileMenu2,MF_STRING,Direct," Direct");
    AppendMenu(hFileMenu2,MF_STRING,Polar,"  Polar");
    AppendMenu(hFileMenu2,MF_STRING,Mid,"  Midpoint");
    AppendMenu(hFileMenu2,MF_STRING,FILE_MENU_EXIT,"Exit");

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu3,"Draw Curve");
    AppendMenu(hFileMenu3,MF_STRING,bezier," bezier");
    AppendMenu(hFileMenu3,MF_STRING,hermite,"  hermite");
    AppendMenu(hFileMenu3,MF_STRING,splines,"  splines");
    AppendMenu(hFileMenu3,MF_STRING,FILE_MENU_EXIT,"Exit");


//#defin
    //AppendMenu(hFileMenu,MF_POPUP, (UINT_PTR)hSubMenu,"Open SubMenu");
    //AppendMenu(hFileMenu,MF_SEPARATOR,NULL,NULL);
    //AppendMenu(hFileMenu,MF_STRING,FILE_MENU_EXIT,"Exit");
    //AppendMenu(hSubMenu,MF_STRING,NULL,"SubMenu Item");

      //  AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"Curve");
    AppendMenu(hMenu,MF_STRING,NULL,"Help");
    SetMenu(hwnd,hMenu);
}

