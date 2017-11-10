#include <stdlib.h>
#include "Globals.h"
#include "MainWnd.h"
#include "MDIChWnd.h"
#include "Resource.h"

/* MDI child window class and title */
static const char MDIChildWndClass[] = "Win16 MDI Example Application - Child";

/* Window procedure for our MDI child window */
LRESULT CALLBACK MDIChildWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_COMMAND:
    {
      WORD id = wParam;

      switch (id)
      {
        case ID_FILE_SAVE:
        {
          MDIChildSave(hWnd);
          return 0;
        }

        case ID_FILE_SAVEAS:
        {
          MDIChildSaveAs(hWnd);
          return 0;
        }
      }

      break;
    }

    /* An MDI child window is being activated */
    case WM_MDIACTIVATE:
    {
      HMENU hParentMenu, hParentFileMenu;
      UINT enableMenu;
      HWND hActivatedChild = (HWND)lParam;

      /* If this window is the one being activated, enable its menus */
      if (hWnd == hActivatedChild)
      {
        enableMenu = MF_ENABLED;
      }
      else
      {
        enableMenu = MF_GRAYED;
      }

      /* Get menu of MDI frame window */
      hParentMenu = GetMenu(g_hMainWindow);

      /* Enable / disable the "window" menu */
      EnableMenuItem(hParentMenu, 1, MF_BYPOSITION | enableMenu);

      /* Enable / disable the save and close menu items */
      hParentFileMenu = GetSubMenu(hParentMenu, 0);
      EnableMenuItem(hParentFileMenu, ID_FILE_SAVE, MF_BYCOMMAND | enableMenu);
      EnableMenuItem(hParentFileMenu, ID_FILE_SAVEAS, MF_BYCOMMAND | enableMenu);
      EnableMenuItem(hParentFileMenu, ID_FILE_CLOSE, MF_BYCOMMAND | enableMenu);
      EnableMenuItem(hParentFileMenu, ID_FILE_CLOSEALL, MF_BYCOMMAND | enableMenu);

      /* Redraw the updated menu */
      DrawMenuBar(g_hMainWindow);

      return 0;
    }

    case WM_CREATE:
    {
      /* Allocate child window data */
      HLOCAL hChildData = LocalAlloc(LHND, sizeof(MdiChildData));

      /* Fail window creation if allocation failed */
      if (!hChildData)
        return -1;

      /* Associate child window data with window */
      SetWindowLong(hWnd, DWL_USER, (UINT)hChildData);

      return 0;
    }

    case WM_DESTROY:
    {
      /* Free child window data */
      HLOCAL hChildData = (HLOCAL)GetWindowLong(hWnd, DWL_USER);

      if (hChildData)
        LocalFree(hChildData);

      return 0;
    }
  }

  return DefMDIChildProc(hWnd, msg, wParam, lParam);
}

/* Register a class for our main window */
BOOL RegisterMDIChildWindowClass()
{
  WNDCLASS wc;

  /* Class for our MDI child window */
  wc.style         = 0;
  wc.lpfnWndProc   = &MDIChildWndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = g_hInstance;
  wc.hIcon         = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_APPICON));
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINMENU);
  wc.lpszClassName = MDIChildWndClass;

  return (RegisterClass(&wc)) ? TRUE : FALSE;
}

/* Create a new MDI child window */
void MDIChildNew(HWND hMDIClient)
{
  MDICREATESTRUCT mcs;
  static unsigned int counter = 0;
  char title[16];
  HWND hWndChild;

  /* Increment counter, but ensure it doesn't become longer than the buffer */
  counter = (counter % 9999) + 1;

  /* Create the window title */
  wsprintf(title, "Untitled - %u", counter);

  /* Set MDI child properties */
  mcs.szTitle = title;
  mcs.szClass = MDIChildWndClass;
  mcs.hOwner = g_hInstance;
  mcs.x = mcs.cx = CW_USEDEFAULT;
  mcs.y = mcs.cy = CW_USEDEFAULT;
  mcs.style = 0;

  /* Create the MDI child */
  hWndChild = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mcs);

  if (hWndChild)
  {
    HLOCAL hChildData;
    MdiChildData* childData;

    /* Mark document as unsaved */
    hChildData = (HLOCAL)GetWindowLong(hWndChild, DWL_USER);
    childData = (MdiChildData*)LocalLock(hChildData);
    childData->IsUnSaved = TRUE;
    LocalUnlock(hChildData);
  }
  else
    MessageBox(NULL, "Error creating new document.", "Error", MB_ICONHAND | MB_OK);
}

/* Open a document in a new MDI child window */
void MDIChildOpen(HWND hMDIClient)
{
  OPENFILENAME ofn = { 0 };
  char fileName[_MAX_PATH] = "";
  char fileTitle[_MAX_FNAME + _MAX_EXT] = "";
  MDICREATESTRUCT mcs;

  /* Set open file dialog properties */
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hMDIClient;
  ofn.lpstrFilter = "Text Documents (*.txt)\0.txt\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = fileName;
  ofn.lpstrFileTitle = fileTitle;
  ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
  ofn.nMaxFile = _MAX_PATH;
  ofn.Flags = OFN_NOTESTFILECREATE | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = "txt";

  /* Show open file dialog */
  if (GetOpenFileName(&ofn))
  {
    HWND hWndChild;

    /* Set MDI child properties */
    mcs.szTitle = fileTitle;
    mcs.szClass = MDIChildWndClass;
    mcs.hOwner = g_hInstance;
    mcs.x = mcs.cx = CW_USEDEFAULT;
    mcs.y = mcs.cy = CW_USEDEFAULT;
    mcs.style = 0;

    /* Create the MDI child */
    hWndChild = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mcs);

    if (hWndChild)
    {
      /* TODO: Add file opening code */
    }
    else
      MessageBox(NULL, "Error opening document.", "Error", MB_ICONHAND | MB_OK);
  }
}

/* Save a document in an MDI child window */
void MDIChildSave(HWND hMDIChild)
{
  HLOCAL hChildData;
  MdiChildData* childData;

  /* Mark document as unsaved */
  hChildData = (HLOCAL)GetWindowLong(hMDIChild, DWL_USER);
  childData = (MdiChildData*)LocalLock(hChildData);

  /* If this is an unsaved document, do a save as */
  if (childData->IsUnSaved)
  {
    MDIChildSaveAs(hMDIChild);
  }
  else
  {
    /* TODO: Add file saving code */
  }

  LocalUnlock(hChildData);
}

/* Save a document in an MDI child window with a filename */
void MDIChildSaveAs(HWND hMDIChild)
{
  OPENFILENAME ofn = { 0 };
  char fileName[_MAX_PATH] = "";
  char fileTitle[_MAX_FNAME + _MAX_EXT] = "";

  /* Set save file dialog properties */
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hMDIChild;
  ofn.lpstrFilter = "Text Documents (*.txt)\0.txt\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = fileName;
  ofn.lpstrFileTitle = fileTitle;
  ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
  ofn.nMaxFile = _MAX_PATH;
  ofn.Flags = OFN_NOTESTFILECREATE | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = "txt";

  if (GetSaveFileName(&ofn))
  {
    HLOCAL hChildData;
    MdiChildData* childData;

    /* TODO: Add file saving code */

    /* Update window title with the filename */
    SetWindowText(hMDIChild, ofn.lpstrFileTitle);

    /* Mark document as saved */
    hChildData = (HLOCAL)GetWindowLong(hMDIChild, DWL_USER);
    childData = (MdiChildData*)LocalLock(hChildData);
    childData->IsUnSaved = FALSE;
    LocalUnlock(hChildData);
  }
}
