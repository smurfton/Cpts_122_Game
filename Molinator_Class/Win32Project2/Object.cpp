#include "Small_Scene_Render.h"

Object::Object(Transformation trans, char vertexFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device) {
	//initialize the object
	d3d_device = device;

	//initialize the transformation
	transform.location.x = trans.location.x;
	transform.location.y = trans.location.y;
	transform.location.z = trans.location.z;
	transform.normalVector.x = trans.normalVector.x;
	transform.normalVector.y = trans.normalVector.y;
	transform.normalVector.z = trans.normalVector.z;
	transform.rotation = trans.rotation;
	transform.scale = trans.scale;

	//copy over the strings for filenames
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		this->vertexFileName[i] = vertexFileName[i];
		this->textureFileName[i] = textureFileName[i];
	}

	loadFBX(&model, vertexFileName); //load vertices into our model vector
	setTextureGradient(model); //initialize the texture gradient

	//create the texture for the object
	D3DXCreateTextureFromFile(*d3d_device, textureFileName, &texture); 

	triangleCount = model.size()/3; //count triangles in our model

	//create the vertex buffer for our new object
	(*d3d_device)->CreateVertexBuffer(triangleCount*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);

	//copy over the transformation into the transformedModel and the vertex buffer
	update();
}
Object::~Object() { //free direct3D resources
	//free texture resources
	texture->Release();
	//free buffer resources
	buffer->Release();
}

Object::Object(const Object &copy) { //copy constructor

	//copy over the fileNames
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		vertexFileName[i] = copy.vertexFileName[i];
		textureFileName[i] = copy.textureFileName[i];
	}

	//copy over the vectors
	Vertex vertice, tVertice;
	for (int i = 0; i < copy.model.size(); i++) {
		//model
		vertice.x = copy.model.at(i).x;
		vertice.y = copy.model.at(i).y;
		vertice.z = copy.model.at(i).z;
		vertice.tx = copy.model.at(i).tx;
		vertice.ty = copy.model.at(i).ty;
		model.push_back(vertice);

		//transformedModel
		tVertice.x = copy.transformedModel.at(i).x;
		tVertice.y = copy.transformedModel.at(i).y;
		tVertice.z = copy.transformedModel.at(i).z;
		tVertice.tx = copy.transformedModel.at(i).tx;
		tVertice.ty = copy.transformedModel.at(i).ty;
		transformedModel.push_back(tVertice);
	}
	//copy over the device pointer
	d3d_device = copy.d3d_device;
	
	//copy over the trianglecount
	triangleCount = copy.triangleCount;
	
	//copy over the transformation
	transform.location.x = copy.transform.location.x;
	transform.location.y = copy.transform.location.y;
	transform.location.z = copy.transform.location.z;
	transform.normalVector.x = copy.transform.normalVector.x;
	transform.normalVector.y = copy.transform.normalVector.y;
	transform.normalVector.z = copy.transform.normalVector.z;
	transform.rotation = copy.transform.rotation;
	transform.scale = copy.transform.scale;

	//create a buffer for new object
	(*d3d_device)->CreateVertexBuffer(triangleCount*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);

	//create a texture for copy
	D3DXCreateTextureFromFile(*d3d_device, textureFileName, &texture); 

	update();
}

//general functionality
void Object::update() {
	//apply the transformation to the object. store the transformation into transformedModel
	//apply transformations
	transformVector(transform, model, transformedModel);

	void *pVoid;
	buffer->Lock(0,0, (void **)&pVoid, 0);
	memcpy(pVoid, &transformedModel[0], transformedModel.size()*sizeof(Vertex));
	buffer->Unlock();
}

void Object::drawObject() {

	//set the texture
	(*d3d_device)->SetTexture(0, texture);

	//set the stream source
	(*d3d_device)->SetStreamSource(0, buffer, 0, sizeof(Vertex));

	//draw the primitive
	(*d3d_device)->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
}