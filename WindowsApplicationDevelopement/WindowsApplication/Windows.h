// include the basic windows header files and the Direct3D header file
#include <windows.h>
#ifndef WINDOW_H
#define WINDOW_H


#include <windowsx.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

//Define screen resolution
#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1366

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(int i);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow);
#endif