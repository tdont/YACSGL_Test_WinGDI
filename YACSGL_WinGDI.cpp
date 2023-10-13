// YACSGL_WinGDI.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "YACSGL_WinGDI.h"

#include "YACSGL.h"
#include "YACSWL.h"

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


static constexpr uint16_t VIRTUAL_FRAME_X = 640u;
static constexpr uint16_t VIRTUAL_FRAME_Y = 480u;
static uint8_t frame_buffer[YACSGL_BUFFER_SIZE_COMPUTE(VIRTUAL_FRAME_X, VIRTUAL_FRAME_Y)];
YACSGL_frame_t  frame{ VIRTUAL_FRAME_X , VIRTUAL_FRAME_Y, 0, 0, frame_buffer };
COLORREF render_pixels[VIRTUAL_FRAME_X * VIRTUAL_FRAME_Y];

YACSWL_widget_t root_widget;

YACSWL_label_t label1;
YACSWL_label_t label2;
YACSWL_label_t label3;

YACSWL_radio_btn_t radio1;
YACSWL_radio_btn_t radio2;
YACSWL_radio_btn_t radio3;
YACSWL_radio_btn_t* radios[] = { &radio1, &radio2, &radio3 };


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    YACSWL_widget_init(&root_widget);
    YACSWL_widget_set_size(&root_widget, VIRTUAL_FRAME_X, VIRTUAL_FRAME_Y);

    YACSWL_label_init(&label1);
    YACSWL_label_set_autosized(&label1, false);
    YACSWL_label_set_text(&label1, "My first label!");
    YACSWL_widget_set_z_order(YACSWL_WDGT(label1), 1u);
    YACSWL_widget_set_pos(YACSWL_WDGT(label1), 10u, 100u);
    YACSWL_widget_set_size(YACSWL_WDGT(label1), 160u, 30u);
    YACSWL_widget_set_margins(YACSWL_WDGT(label1), 10u, 10u, 10u, 10u);

    YACSWL_label_init(&label2);
    YACSWL_label_set_text(&label2, "A blinking label!");
    YACSWL_widget_set_z_order(YACSWL_WDGT(label2), 3u);
    YACSWL_widget_set_pos(YACSWL_WDGT(label2), 30u, 150u);
    YACSWL_widget_set_margins(YACSWL_WDGT(label2), 10u, 10u, 10u, 10u);
    YACSWL_widget_set_border_width(YACSWL_WDGT(label2), 5u);

    YACSWL_label_init(&label3);
    YACSWL_label_set_text(&label3, "A moving label!");
    YACSWL_widget_set_z_order(YACSWL_WDGT(label3), 2u);
    YACSWL_widget_set_border_width(YACSWL_WDGT(label3), 0u);
    YACSWL_widget_set_pos(YACSWL_WDGT(label3), 0u, 200u);
    YACSWL_widget_set_transparent_background(YACSWL_WDGT(label3), true);

    YACSWL_radio_btn_init(&radio1);
    YACSWL_radio_btn_set_text(&radio1, "First radio button");
    YACSWL_widget_set_pos(YACSWL_WDGT(radio1), 40u, 10u);

    YACSWL_radio_btn_init(&radio2);
    YACSWL_radio_btn_set_autosized(&radio2, false);
    YACSWL_radio_btn_set_text(&radio2, "Second radio button");
    YACSWL_widget_set_z_order(YACSWL_WDGT(label3), 2u);
    YACSWL_widget_set_pos(YACSWL_WDGT(radio2), 40u, 45u);
    YACSWL_widget_set_border_width(YACSWL_WDGT(radio2), 5u);
    YACSWL_widget_set_size(YACSWL_WDGT(radio2), 200u, 50u);

    YACSWL_radio_btn_init(&radio3);
    YACSWL_radio_btn_set_text(&radio3, "Third radio button");
    YACSWL_widget_set_pos(YACSWL_WDGT(radio3), 40u, 80u);

    YACSWL_widget_add_child(&root_widget, YACSWL_WDGT(label1));
    YACSWL_widget_add_child(&root_widget, YACSWL_WDGT(label2));
    YACSWL_widget_add_child(&root_widget, YACSWL_WDGT(label3));
    YACSWL_widget_add_child(&root_widget, YACSWL_WDGT(radio1));
    YACSWL_widget_add_child(&root_widget, YACSWL_WDGT(radio2));
    YACSWL_widget_add_child(&root_widget, YACSWL_WDGT(radio3));

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_YACSGLWINGDI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YACSGLWINGDI));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YACSGLWINGDI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_YACSGLWINGDI);
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

   SetTimer(hWnd, NULL, 100u, NULL);

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
    case WM_TIMER:
        {
            static int val = 0;
            YACSWL_widget_set_foreground_color(YACSWL_WDGT(label2), (YACSGL_pixel_t)val);
            val ^= 1;
            YACSWL_widget_set_background_color(YACSWL_WDGT(label2), (YACSGL_pixel_t)val);

            static int radio_index = 0;
            for (int i = 0; i < 3; i++)
            {
                if (i == radio_index)
                {
                    YACSWL_radio_btn_set_checked(radios[i], true);
                }
                else
                {
                    YACSWL_radio_btn_set_checked(radios[i], false);
                }
            }
            radio_index++;
            if (radio_index > 3)
            {
                radio_index = 0;
            }
            
            uint16_t x = YACSWL_widget_get_pos_x(YACSWL_WDGT(label3));
            x += 25u;
            if (x >= VIRTUAL_FRAME_X)
            {
                x = 0;
            }
            uint16_t y = YACSWL_widget_get_pos_y(YACSWL_WDGT(label3));
            y += 25u;
            if (y >= VIRTUAL_FRAME_Y)
            {
                y = 0;
            }
            YACSWL_widget_set_pos(YACSWL_WDGT(label3), x, y);

            memset(frame_buffer, 0, sizeof(frame_buffer));
            YACSWL_widget_draw(&root_widget, &frame);

            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        /* Draw the prepared frame buffer */
        int index = 0;
        for (uint16_t y = 0; y < VIRTUAL_FRAME_Y; y++)
        {
            for (uint16_t x = 0; x < VIRTUAL_FRAME_X; x++)
            {
                uint8_t temp_byte = frame_buffer[y * (VIRTUAL_FRAME_X / 8) + x / 8];
                if (temp_byte & 0b1 << (7 - (x % 8)))
                {
                    render_pixels[index] = 0xFFFFFFFF;
                }
                else
                {
                    render_pixels[index] = 0x00000000;
                }
                index++;
            }
        }
        
        RECT client_rect;
        GetClientRect(hWnd, &client_rect);

        // Creating temp bitmap
        HBITMAP map = CreateBitmap(VIRTUAL_FRAME_X, // width
            VIRTUAL_FRAME_Y, // height
            1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
            8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
            (void*)render_pixels); // pointer to array
        // Temp HDC to copy picture
        HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
        SelectObject(src, map); // Inserting picture into our temp HDC
        // Copy image from temp HDC to window
        BitBlt(hdc, // Destination
            (client_rect.right - VIRTUAL_FRAME_X) / 2,  // x and
            (client_rect.bottom - VIRTUAL_FRAME_Y) / 2,  // y - upper-left corner of place, where we'd like to copy
            VIRTUAL_FRAME_X, // width of the region
            VIRTUAL_FRAME_Y, // height
            src, // source
            0,   // x and
            0,   // y of upper left corner  of part of the source, from where we'd like to copy
            SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

        DeleteDC(src); // Deleting temp HDC
        
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
