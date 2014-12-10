#ifndef SMALL_SCENE_RENDER_H
#define SMALL_SCENE_RENDER_H

//includes
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fbxsdk.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <ctime>

using std::vector;
using std::string;
using std::fstream;
using std::cin;
using std::cout;
using std::endl;
using std::exit;



/*# defines*/
#define SCREEN_WIDTH 800 //define screen width
#define SCREEN_HEIGHT 600 //define screen height
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL) //this describes a vertex format, this represents XYZ coordinates
//#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)
#define MAX_FILE_LENGTH 15

//game controls
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define JUMP ' '
#define KILL '`'
#define TURN_RIGHT 'e'
#define TURN_LEFT 'q'

//camera controls
#define ZOOMIN '+'
#define ZOOMOUT '-'
#define PANLEFT 'j'
#define PANRIGHT 'l'
#define PANUP 'i'
#define PANDOWN 'k'

//physics binds
#define MAX_ACC 30
#define MAX_VEL 150
#define MOVE_SPEED 10
#define MAX_ACC_Y 200

//not sure why we need this to be honest
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")



/*structure definitions*/

//the vertex of a triangle
typedef struct VERTEX { 
	float x, y, z;
	D3DVECTOR normal;
} Vertex;

//this struct declares a position or a vector
typedef struct POSITION {
	float x, y, z;
} Position, Vector;

//this struct contains information about the state of each object
typedef struct TRANSFORMATION {
	Vector translation, rotation, scaling;
} Transformation;

//a struct containing velocity information
typedef struct PHYSICS_PROPERTIES {
	Vector velocity;
	Vector acceleration;
} PhysicsProperties;

typedef struct CUBE {
	Vertex vertices[8];
} Cube;


/*function prototypes*/

//windows message management function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

//vector utility functions
void addVectors(vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, char dir, float amount);
void scale(vector<Vertex> &v1, float factor);
void transformVector(Transformation transform, vector<Vertex> &v1, vector<Vertex> &v2);
void translate(vector<Vertex> &v1, vector<Vertex> &v2, Position newLocation);
//Transformation calculateTransform(Position desiredLocation, Position desiredOrientation, float scale);
void copyVector (vector<Vertex> &v1, vector<Vertex> &v2);
void calculateNormal(vector<Vertex> &v1);
float calculateMagnitude(Vector v1);
Vector scaleVector(Vector v1, float length);
Cube transformHitBox(Cube box, Transformation transform);
bool checkVerticesCube(Cube c1, Cube c2);

//overload functions
Vector operator+(Vector &lhs, Vector &rhs);
Vector operator-(Vector &lhs, Vector &rhs);
bool operator==(Vector &lhs, Vector &rhs);

//menu function
void menu();

//map generation
void generateMap(int elements);

//forward declaration
class Game;
//include the classes
#include "Object.h"
#include "Player.h"
#include "Camera.h"
#include "Character.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Objective.h"
#include "Game.h"

#endif