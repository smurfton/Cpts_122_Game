#include "Molinator.h"

void initializeStatic(IDirect3DSurface9 *image, wchar_t fileName[]) {
	D3DXLoadSurfaceFromFile(image, NULL, NULL, fileName, NULL, D3DX_FILTER_NONE, 0, NULL);
}
/*MOVED TO OBJECTS*/
/*This function will populate the static environment buffer.*/
/*The environment buffer should remain constant through the program*/
/*void initializeEnvironment() {
	vector<Vertex> vertices;
	//Load environment variables from the resource files
	loadFBX(&vertices, "Ground.FBX");

	//scale the environment objects
	scale(vertices, 10);

	//place the scaled objects into correct positions

	//combine all vectors into a unified vector

	//create the environment buffer
	d3d_device->CreateVertexBuffer(vertices.size()*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &environment_Buffer, NULL);

	//copy memory into the buffer
	void *pVoid;
	environment_Buffer->Lock(0, 0, (void **) pVoid, 0);
	memcpy(pVoid, &vertices[0], vertices.size() * sizeof(Vertex));
	environment_Buffer->Unlock();
}*/

/*This function initializes the character buffer*/
//the character can be translated based on movements
/*void initializeCharacter() {
	vector<Vertex> vertices;
	//Load player vertices from the resource files
	loadFBX(&vertices, "Player.FBX");

	//scale the player
	scale(vertices, .10);

	//create the environment buffer
	d3d_device->CreateVertexBuffer(vertices.size()*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &character_Buffer, NULL);

	//copy memory into the buffer
	void *pVoid;
	character_Buffer->Lock(0, 0, (void **) pVoid, 0);
	memcpy(pVoid, &vertices[0], vertices.size() * sizeof(Vertex));
	character_Buffer->Unlock();
}*/

/*This function initializes a buffer for enemies*/
/*void initializeEnemies() {
	vector<Vertex> vertices;
	//Load enemy vertices from the resource files
	loadFBX(&vertices, "Enemy.FBX");

	//scale the enemies
	scale(vertices, .10);

	//place the scaled objects into correct positions

	//combine objects into a final vector

	//create the environment buffer
	d3d_device->CreateVertexBuffer(vertices.size()*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &enemy_Buffer, NULL);

	//copy memory into the buffer
	void *pVoid;
	enemy_Buffer->Lock(0, 0, (void **) pVoid, 0);
	memcpy(pVoid, &vertices[0], vertices.size() * sizeof(Vertex));
	enemy_Buffer->Unlock();
}*/