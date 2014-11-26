#include "Molinator.h"

Object::Object() {
	//initialize all elements
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		modelFileName[i] = '\0';
		textureFileName[i] = '\0';
	}
	size = 1;

	buffer = NULL;
	pTexture = NULL;

	location.x = 0;
	location.y = 0;
	location.z = 0;

	orientation.x = 1;
	orientation.y = 0;
	orientation.z = 0;

}
Object::~Object() {
	//free up the texture and vertex objects
	if (buffer != nullptr) buffer->Release();
	if (pTexture != nullptr) pTexture->Release();
}

//getters
Position Object::getPosition() {
	return location;
}
Position Object::getOrientation() {
	return orientation;
}
wchar_t *Object::getModelFile() {
	return modelFileName;
}
wchar_t *Object::getTextureFile() {
	return textureFileName;
}
float Object::getScale() {
	return size;
}

//setters
void Object::setPosition(Position newPosition) {
	location.x = newPosition.x;
	location.y = newPosition.y;
	location.z = newPosition.z;
}
void Object::setOrientation(Position newOrientation) {
	orientation.x = newOrientation.x;
	orientation.y = newOrientation.y;
	orientation.z = newOrientation.z;
}
void Object::setModelFile(wchar_t * fileName) {
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		modelFileName[i] = fileName[i];
	}
	this->setVertices();
}
void Object::setTextureFile(wchar_t * fileName) {
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		textureFileName[i] = fileName[i];
	}
}
void Object::setScale(float newScale) {
	size = newScale;
}
void Object::setVertices() {
	/*Load environment variables from the resource files*/
	//convert modelFileName to a character array
	char fileName[MAX_FILE_LENGTH];
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		fileName[i] = modelFileName[i];
	}
	loadFBX(&vertices, fileName);
	for (int i = 0; i < vertices.size(); i++) {
		transformed.push_back(vertices.at(i));
	}
	this->setTransformedVertices();
}
void Object::setTransformedVertices() {
	//scale the  object
	scale(vertices, transformed, size);

	//calculate parameters for rotation
	float rotation;
	Position normal;
	Position current = {0,0,0};
	Position currentNormal = {1,0,0};
	calculateTransform(currentNormal, orientation, &normal, &rotation);

	//orient the object
	//rotate(vertices, transformed, normal, rotation);

	//place the scaled objects into correct positions
	translate(vertices, transformed, current, location);
}
//general functionality
void Object::createTexture(LPDIRECT3DDEVICE9 &device){
	D3DXCreateTextureFromFile(device,   //Direct3D Device
		textureFileName,       //File Name
		&pTexture);    //Texture handle
}
void Object::createBuffer(LPDIRECT3DDEVICE9 &device) {
	//create the  buffer
	device->CreateVertexBuffer(vertices.size()*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);
}
void Object::loadBuffer(LPDIRECT3DDEVICE9 &device) {
	//copy memory into the buffer
	void *pVoid = NULL;
	buffer->Lock(0, 0, (void **) &pVoid, 0);
	memcpy(pVoid, &transformed[0], transformed.size() * sizeof(Vertex));
	buffer->Unlock();
}
void Object::loadTexture(LPDIRECT3DDEVICE9 &device) {
	device->SetTexture(0, pTexture);
}

LPDIRECT3DVERTEXBUFFER9 &Object::getBuffer() {
	return buffer;
}