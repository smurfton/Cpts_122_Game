#ifndef MOLINATOR_H
#define MOLINATOR_H

//include statements
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include <vector>
#include <fbxsdk.h>
#include <string>
#include <cmath>

using std::vector;
using std::string;

//#defines
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_TEX1) //this describes a vertex format, this represents XYZ coordinates
#define MAX_TRIANGLES 100000
#define MAX_FILE_LENGTH 15
	//define the controls
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define JUMP ' '
#define DIG 'x'

//include libraries
#pragma comment(lib,"d3d9.lib")

//structure declarations
typedef struct VERTEX {
	float x, y, z;
	float tx, ty;
}Vertex;

typedef struct POSITION {
	float x;
	float y;
	float z;
}Position;

/*function prototypes*/
void initView(LPDIRECT3DDEVICE9 &device);

//Windows Application Function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //windows message management function

//Fbx Data Extraction Function
HRESULT loadFBX (vector<Vertex> *pVertexVector, char *pFileName);

//Vector Resources
void combineVectors(vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, vector<Vertex> &v2, char dir, float amount);
void translate(vector<Vertex> &v1, vector<Vertex> &v2, Position currentLocation, Position newLocation);
void scale(vector<Vertex> &v1, vector<Vertex> &v2, float factor);
void rotate(vector<Vertex> &v1, vector<Vertex> &v2, Position axis, float radians);
void calculateTransform(Position currentOrientation, Position desiredOrientation, Position *normal, float *radians);

extern LPDIRECT3DDEVICE9 d3d_device;

#include "Object.h"
#include "Enemy.h"
#include "Player.h"
#include "Environment.h"
#include "Game.h"

#endif