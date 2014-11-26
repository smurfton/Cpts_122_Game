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


//#defines
#define SCREEN_WIDTH 800 //define screen width
#define SCREEN_HEIGHT 600 //define screen height
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_TEX1) //this describes a vertex format, this represents XYZ coordinates
#define MAX_TRIANGLES 100000
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define JUMP ' '
#define INI_X 0.0f
#define INI_Y 1.0f
#define INI_Z 10.0f
#define FALL 'x'
#define TURN_RIGHT 'e'
#define TURN_LEFT 'q'

#pragma comment(lib,"d3d9.lib")

//structure definitions
typedef struct VERTEX { //the vertex of a triangle
	float x, y, z;
	//DWORD color;
	float tx, ty;
} Vertex;

//this struct declares a position or a vector
typedef struct POSITION {
	float x, y, z;
} Position;

//this struct contains information about the state of each object
typedef struct TRANSFORMATION {
	Position location;
	Position normalVector;
	float rotation;
	float scale;
} Transformation;


/*function prototypes*/

//windows message management function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

//direct3D initialization and utility functions
void initInterface(HWND window); //initializes the direct3D interface
void cleanD3D(); //will clean all D3D devices implemented. Should be called last
void render(); // render the scene!

//model initialization functions
void initVertices();
void initializeEnvironment ();
void initializeCharacter();

//model drawing functions
void drawVertices();

//FBX File extraction function
HRESULT loadFBX (vector<Vertex> *pVertexVector, char *pFileName);

//vector utility functions
void addVectors(vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, char dir, float amount);
void scale(vector<Vertex> &v1, float factor);
void transformVector(Transformation transform, vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, vector<Vertex> &v2, Position newLocation);
Transformation calculateTransform(Position desiredLocation, Position desiredOrientation, float scale);
void copyVector (vector<Vertex> &v1, vector<Vertex> &v2);

//texture utility functions
void setNewTexture(IDirect3DTexture9 *texture);

//buffer update functions
void updateCamera();
void updateCharacter();

//player movement functions

void moveForward();
void moveLeft();
void moveBack();
void moveRight();
void moveUp();
void fall();
void turnRight();
void turnLeft();

//global variables
extern LPDIRECT3D9 d3d_interface; //this is the direct3D interface
extern LPDIRECT3DDEVICE9 d3d_device; //this is the direct3D device to be used
extern D3DPRESENT_PARAMETERS d3d_interface_parameters; //this is the parameters object for the interface
extern LPDIRECT3DVERTEXBUFFER9 virtex_buffer; //declare the buffer to store vertex information into
extern LPDIRECT3DVERTEXBUFFER9 character_buffer;
extern LPDIRECT3DVERTEXBUFFER9 environment_buffer;
extern UINT env_size, char_size;
extern IDirect3DTexture9 *texture;
extern IDirect3DTexture9 *ground;
extern FbxManager *manager;
extern Position cameraPosition;
extern float cameraAngle;
extern Position cameraViewVector;
extern vector<Vertex> player;
extern Transformation playerOrientation;

#endif