#ifndef MAINWND_H
#define MAINWND_H

#include <windows.h>

/* Window procedure for our main window */
LRESULT CALLBACK __export MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/* Register a class for our main window */
BOOL RegisterMainWindowClass(void);

/* Create an instance of our main window */
HWND CreateMainWindow(void);

/* Callback to close all child windows */
BOOL CALLBACK __export CloseAllProc(HWND hWnd, LPARAM lParam);

/* First ID Windows should use for menu items it attaches to the "Window" menu */
#define ID_MDI_FIRSTCHILD 400

#endif
