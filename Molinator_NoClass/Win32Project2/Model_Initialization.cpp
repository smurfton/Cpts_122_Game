#include "Small_Scene_Render.h"

void initializeCharacter() {
	loadFBX(&player, "Person.FBX");

	char_size = player.size()/3;
	//transformation of the character is done in updateCharacter();
	d3d_device->CreateVertexBuffer(char_size*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &character_buffer, NULL);
	updateCharacter();
}

/*void initVertices() {

	vector<Vertex> vertices, terrain, person;
	loadFBX(&vertices, "Test.FBX");
	//loadFBX(&terrain, "terrain.FBX");
	loadFBX(&person, "Person.FBX");
	scale(terrain, 10);
	scale(person, .1);
	translate(terrain, 'x', 5);
	translate(person, 'z', 5);
	addVectors(vertices, terrain);
	
	d3d_device->CreateVertexBuffer(MAX_TRIANGLES*3*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &virtex_buffer, NULL); //create the buffer
	d3d_device->CreateVertexBuffer(MAX_TRIANGLES*3*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &character_buffer, NULL);

	void *pVoid, *pVoid2; //used for referencing memory copy location

	virtex_buffer->Lock(0, 0, (void**)&pVoid, 0); //lock the memory location where we are copying vertex information
	memcpy(pVoid, &vertices[0], vertices.size() * sizeof(Vertex)); //copy the memory into the VRAM buffer
	virtex_buffer->Unlock(); //unlock the memory now that it has been successfully copied

	character_buffer->Lock(0,0,(void**)&pVoid2,0);
	memcpy(pVoid2, &person[0], person.size() * sizeof(Vertex));
	character_buffer->Unlock();

}*/

void initializeEnvironment () {
	vector<Vertex> ground, unified;
	//load model into temporary vector
	//In the future, extract this to a file to simplify the program
	loadFBX(&ground, "Ground.FBX");

	//transform the model and push it to the unifying vector
	Position newLocation = {0,0,0}, newOrientation = {1,1,0};

	transformVector(calculateTransform(newLocation,newOrientation, 10), ground, unified);

	//create the buffer
	env_size = unified.size()/3;
	d3d_device->CreateVertexBuffer(env_size*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &environment_buffer, NULL);

	//copy data into the buffer. These models are static so this only needs to be done during initialization
	void *pVoid;
	environment_buffer->Lock(0, 0, (void **)&pVoid, 0);
	memcpy(pVoid, &unified[0], unified.size()*sizeof(Vertex));
	environment_buffer->Unlock();
}