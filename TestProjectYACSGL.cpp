// TestProjectYACSGL.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "TestProjectYACSGL.h"
#include "YACSGL.h"
#include "YACSGL_pixel.h"
#include "YACSGL_font_5x7.h"
#include "YACSGL_font_8x16.h"
#include "YACSGL_font.h"

#define MAX_LOADSTRING 100

#define VIRTUAL_FRAME_X     264
#define VIRTUAL_FRAME_Y     176

//#define VIRTUAL_FRAME_X     1024
//#define VIRTUAL_FRAME_Y     768

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static uint8_t frame_buffer_2D[VIRTUAL_FRAME_Y][VIRTUAL_FRAME_X / 8] = { 0 };
static uint8_t* frame_buffer = (uint8_t*)frame_buffer_2D;
//static uint8_t frame_buffer[(VIRTUAL_FRAME_X * VIRTUAL_FRAME_Y) / 8] = { 0 };
YACSGL_frame_t  frame{ VIRTUAL_FRAME_X, VIRTUAL_FRAME_Y, frame_buffer };

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTPROJECTYACSGL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPROJECTYACSGL));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTPROJECTYACSGL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTPROJECTYACSGL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HBRUSH				brushWhite;
            HPEN				penWhite, penBlack, penRed;
            HBITMAP				bufferBMP, blankBMP;

            penWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            penRed = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            penBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            brushWhite = CreateSolidBrush(RGB(255, 255, 255));
            COLORREF rgbBlack = 0x00000000;

            blankBMP = CreateCompatibleBitmap(hdc, VIRTUAL_FRAME_X, VIRTUAL_FRAME_Y);
            SelectObject(hdc, blankBMP);
            SelectObject(hdc, brushWhite);
            SelectObject(hdc, penRed);
            Rectangle(hdc, 0, 0, VIRTUAL_FRAME_X, VIRTUAL_FRAME_Y);

            bufferBMP = CreateCompatibleBitmap(hdc, VIRTUAL_FRAME_X, VIRTUAL_FRAME_Y);
            SelectObject(hdc, bufferBMP);
            SelectObject(hdc, penBlack);

            //SetPixel(hdc, 10, 10, rgbBlack);

            //MoveToEx(hdc, 0, 0, NULL);
            //LineTo(hdc, 80, 120);

            //RectNoBackground(hdc, 0, 0, 10 , 10);


            // TODO: Ajoutez ici le code de dessin qui utilise hdc...
            /*YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X-1, VIRTUAL_FRAME_Y-1, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X / 2, VIRTUAL_FRAME_Y-1, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X / 3, VIRTUAL_FRAME_Y-1, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, (VIRTUAL_FRAME_X * 2) / 3, VIRTUAL_FRAME_Y-1, YACSGL_P_WHITE);

            YACSGL_line(&frame, 0, 0, 0, VIRTUAL_FRAME_Y - 1, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X - 1, 0, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y / 2, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y / 3, YACSGL_P_WHITE);
            YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X - 1, (VIRTUAL_FRAME_Y*2) / 3, YACSGL_P_WHITE);*/

            //YACSGL_line(&frame, 0, 0, VIRTUAL_FRAME_X, 0, YACSGL_P_WHITE);

            //YACSGL_line(&frame, 0, 0, 0, VIRTUAL_FRAME_Y, YACSGL_P_WHITE);

            //YACSGL_rect_line(&frame, 0, 0, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y - 1, YACSGL_P_WHITE);

            YACSGL_font_txt_disp(&frame, 100, 100, YACSGL_P_WHITE, &YACSGL_font_5x7, "This is a test \rwith a new line", YACSGL_NEWLINE_DISABLED);

            YACSGL_font_txt_disp(&frame, 0, 130, YACSGL_P_WHITE, &YACSGL_font_5x7, "This is a test \rwith a new line", YACSGL_NEWLINE_ENABLED);
            YACSGL_font_txt_disp(&frame, 10, 10, YACSGL_P_WHITE, &YACSGL_font_8x16, "This is a test \rwith a new line", YACSGL_NEWLINE_ENABLED);

			YACSGL_font_txt_disp(&frame, 240, 130, YACSGL_P_WHITE, &YACSGL_font_5x7, "This is a test \rwith a new line", YACSGL_NEWLINE_ENABLED);
			YACSGL_font_txt_disp(&frame, 0, 168, YACSGL_P_WHITE, &YACSGL_font_5x7, "This is a test gqp \rwith a new line", YACSGL_NEWLINE_ENABLED);
            /*YACSGL_rect_line(&frame, 10, 10, 19, 19, YACSGL_P_WHITE);

            YACSGL_rect_fill(&frame, 30, 30, 39, 39, YACSGL_P_WHITE);

            YACSGL_circle_line(&frame, 50, 50, 30, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, 80, 80, 30, YACSGL_P_WHITE);

            

            YACSGL_rect_line(&frame, 50, 50, 1000, 1000, YACSGL_P_WHITE);

			YACSGL_rect_fill(&frame, 100, 100, VIRTUAL_FRAME_X, 110, YACSGL_P_WHITE);

			YACSGL_rect_fill(&frame, 150, 150, 160, VIRTUAL_FRAME_Y, YACSGL_P_WHITE);

			YACSGL_rect_line(&frame, 170, 170, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y - 1, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, VIRTUAL_FRAME_X - 20, 80, 25, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, 5, 40, 25, YACSGL_P_WHITE);

            YACSGL_circle_line(&frame, VIRTUAL_FRAME_X - 20, 160, 25, YACSGL_P_WHITE);

            YACSGL_circle_line(&frame, VIRTUAL_FRAME_X - 20, 20, 25, YACSGL_P_WHITE); 

            YACSGL_circle_line(&frame, 0, 0, 10, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, 0, 0, 7, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, VIRTUAL_FRAME_X - 1, 0, 7, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y - 1, 7, YACSGL_P_WHITE);

            YACSGL_circle_line(&frame, 0, VIRTUAL_FRAME_Y - 1, 25, YACSGL_P_WHITE);

            YACSGL_circle_fill(&frame, 0, VIRTUAL_FRAME_Y - 1, 7, YACSGL_P_WHITE);*/
            
            //YACSGL_rect_fill(&frame, 0, 0, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y - 1, YACSGL_P_WHITE);

            //YACSGL_rect_fill(&frame, 0, 0, VIRTUAL_FRAME_X - 1, VIRTUAL_FRAME_Y - 1, YACSGL_P_BLACK);

            /* Draw the prepare frame buffer */
            for (uint16_t y = 0; y < VIRTUAL_FRAME_Y; y++)
            {
                for (uint16_t x = 0; x < VIRTUAL_FRAME_X; x++)
                {
                    uint8_t temp_byte = frame_buffer[y * (VIRTUAL_FRAME_X / 8) + x / 8];
                    if (temp_byte & 0b1 << (7 - (x % 8)))
                    {
                        SetPixel(hdc, x, y, rgbBlack);
                    }
                }
            }



            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
