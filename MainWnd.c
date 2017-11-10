#include "AboutDlg.h"
#include "Globals.h"
#include "MainWnd.h"
#include "MDIChWnd.h"
#include "Resource.h"

/* Main window class and title */
static const char MainWndClass[] = "Win16 MDI Example Application";

/* Global MDI client window handle */
HWND g_hMDIClient = NULL;

/* Window procedure for our main window */
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_COMMAND:
    {
      WORD id = wParam;

      switch (id)
      {
        /* Create new MDI child with an empty document */
        case ID_FILE_NEW:
        {
          MDIChildNew(g_hMDIClient);
          return 0;
        }

        /* Create new MDI child with an existing document */
        case ID_FILE_OPEN:
        {
          MDIChildOpen(g_hMDIClient);
          return 0;
        }

        /* Send close message to MDI child on close menu item */
        case ID_FILE_CLOSE:
        {
          HWND hWndChild = (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE, 0, 0);

          if (hWndChild)
          {
            SendMessage(hWndChild, WM_CLOSE, 0, 0);
            return 0;
          }

          break;
        }

        /* Close all MDI children on close all menu item */
        case ID_FILE_CLOSEALL:
        {
          EnumChildWindows(g_hMDIClient, &CloseAllProc, 0);

          return 0;
        }

        /* Show "about" dialog on about menu item */
        case ID_HELP_ABOUT:
        {
          ShowAboutDialog(hWnd);
          return 0;
        }

        case ID_FILE_EXIT:
        {
          DestroyWindow(hWnd);
          return 0;
        }

        case ID_WINDOW_TILE:
        {
          SendMessage(g_hMDIClient, WM_MDITILE, 0, 0);
          return 0;
        }

        case ID_WINDOW_CASCADE:
        {
          SendMessage(g_hMDIClient, WM_MDICASCADE, 0, 0);
          return 0;
        }

        case ID_WINDOW_ARRANGE:
        {
          SendMessage(g_hMDIClient, WM_MDIICONARRANGE, 0, 0);
          return 0;
        }

        default:
        {
          /* If the ID is less than ID_MDI_FIRSTCHILD, it's probably a message for a child window's menu */
          if (id < ID_MDI_FIRSTCHILD)
          {
            HWND hWndChild = (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE, 0, 0);

            if (hWndChild)
            {
              SendMessage(hWndChild, WM_COMMAND, wParam, lParam);
              return 0;
            }
          }

          break;
        }
      }

      break;
    }

    case WM_GETMINMAXINFO:
    {
      /* Prevent our window from being sized too small */
      MINMAXINFO FAR* minMax = (MINMAXINFO FAR*)lParam;
      minMax->ptMinTrackSize.x = 220;
      minMax->ptMinTrackSize.y = 110;

      return 0;
    }

    case WM_SIZE:
    {
      /* Ensure MDI client fills the whole client area */
      RECT rcClient;

      GetClientRect(hWnd, &rcClient);
      SetWindowPos(g_hMDIClient, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);

      return 0;
    }

    /* Item from system menu has been invoked */
    case WM_SYSCOMMAND:
    {
      WORD id = wParam;

      switch (id)
      {
        /* Show "about" dialog on about system menu item */
        case ID_HELP_ABOUT:
        {
          ShowAboutDialog(hWnd);
          return 0;
        }
      }

      break;
    }

    case WM_CREATE:
    {
      /* Create the MDI client window */
      CLIENTCREATESTRUCT ccs;
      HWND hMDIClient;

      ccs.hWindowMenu = GetSubMenu(GetMenu(hWnd), 1);
      ccs.idFirstChild = ID_MDI_FIRSTCHILD;

      hMDIClient = CreateWindowEx(0, "MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL |
                                  WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL,
                                  g_hInstance, (LPVOID)&ccs);

      /* Fail the window creation if the MDI client creation failed */
      if (!hMDIClient)
      {
        MessageBox(NULL, "Error creating MDI client.", "Error", MB_ICONHAND | MB_OK);
        return -1;
      }

      /* Set global MDI client handle */
      g_hMDIClient = hMDIClient;

      return 0;
    }

    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
  }

  return DefFrameProc(hWnd, g_hMDIClient, msg, wParam, lParam);
}

/* Close all child windows */
BOOL CALLBACK CloseAllProc(HWND hWnd, LPARAM lParam)
{
  if (GetWindow(hWnd, GW_OWNER))
    return TRUE;

  SendMessage(hWnd, WM_CLOSE, 0, 0);

  return TRUE;
}

/* Register a class for our main window */
BOOL RegisterMainWindowClass()
{
  WNDCLASS wc;

  /* Class for our main window */
  wc.style         = 0;
  wc.lpfnWndProc   = &MainWndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = g_hInstance;
  wc.hIcon         = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_APPICON));
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINMENU);
  wc.lpszClassName = MainWndClass;

  return (RegisterClass(&wc)) ? TRUE : FALSE;
}

/* Create an instance of our main window */
HWND CreateMainWindow()
{
  /* Create instance of main window */
  HWND hWnd = CreateWindowEx(0, MainWndClass, MainWndClass, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
                             CW_USEDEFAULT, 640, 480, NULL, NULL, g_hInstance, NULL);

  if (hWnd)
  {
    /* Add "about" to the system menu */
    HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
    InsertMenu(hSysMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hSysMenu, 6, MF_BYPOSITION, ID_HELP_ABOUT, "About");
  }

  return hWnd;
}
