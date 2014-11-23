#ifndef SMALL_SCENE_RENDER_H
#define SMALL_SCENE_RENDER_H

//includes
#include <d3d9.h>
#include <Windows.h>
#include <d3dx9.h>

//#defines
#define SCREEN_WIDTH 800 //define screen width
#define SCREEN_HEIGHT 600 //define screen height
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE) //this describes a vertex format, this represents XYZ coordinates
#define MAX_TRIANGLES 100


#pragma comment(lib,"d3d9.lib")

//structure definitions
typedef struct VERTEX { //the vertex of a triangle
	float x, y, z;
	DWORD color;
} Vertex;

//function prototypes
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //windows message management function
void initInterface(HWND window); //initializes the direct3D interface
void initView();
void cleanD3D(); //will clean all D3D devices implemented. Should be called last
void render(); // render the scene!
void initVertices();
void drawPyramid();

//global variables
LPDIRECT3D9 d3d_interface; //this is the direct3D interface
LPDIRECT3DDEVICE9 d3d_device; //this is the direct3D device to be used
D3DPRESENT_PARAMETERS d3d_interface_parameters; //this is the parameters object for the interface
LPDIRECT3DVERTEXBUFFER9 virtex_buffer; //declare the buffer to store vertex information into



#endif