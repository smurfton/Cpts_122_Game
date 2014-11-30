#ifndef SMALL_SCENE_RENDER_H
#define SMALL_SCENE_RENDER_H

//includes
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <vector>
#include <fbxsdk.h>
#include <string>

using std::vector;
using std::string;


/*# defines*/
#define SCREEN_WIDTH 800 //define screen width
#define SCREEN_HEIGHT 600 //define screen height
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_TEX1) //this describes a vertex format, this represents XYZ coordinates
#define MAX_FILE_LENGTH 15

//game controls
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define JUMP ' '
#define FALL 'x'
#define TURN_RIGHT 'e'
#define TURN_LEFT 'q'
#define DIG '1'
#define UNDIG '2'

//not sure why we need this to be honest
#pragma comment(lib,"d3d9.lib")



/*structure definitions*/

//the vertex of a triangle
typedef struct VERTEX { 
	float x, y, z;
	//DWORD color;
	float tx, ty;
} Vertex;

//this struct declares a position or a vector
typedef struct POSITION {
	float x, y, z;
} Position, Vector;

//this struct contains information about the state of each object
typedef struct TRANSFORMATION {
	Position location;
	Vector normalVector;
	float rotation;
	float scale;
} Transformation;


/*function prototypes*/

//windows message management function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

//FBX File extraction function
HRESULT loadFBX (vector<Vertex> *pVertexVector, char *pFileName);
void setTextureGradient(vector<Vertex> &v1);

//vector utility functions
void addVectors(vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, char dir, float amount);
void scale(vector<Vertex> &v1, float factor);
void transformVector(Transformation transform, vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, vector<Vertex> &v2, Position newLocation);
Transformation calculateTransform(Position desiredLocation, Position desiredOrientation, float scale);
void copyVector (vector<Vertex> &v1, vector<Vertex> &v2);

//include the classes
#include "Object.h"
#include "Player.h"
#include "Camera.h"
#include "Game.h"

#endif