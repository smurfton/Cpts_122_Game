#include "Small_Scene_Render.h"

Enemy::Enemy(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color, Vector traverse) {
	//initialize the object
	d3d_device = device;

	//copy over the strings for filenames
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		this->vertexFileName[i] = vertexFileName[i];
	}

	//initialize the ID
	this->ID = ID;

	//loadFBX(box, &model, vertexFileName); //load vertices into our model vector
	loadFbx(); //load vertices into model
	generateHitBox(); //generate a hitbox for the model
	
	//setTextureGradient(model); //initialize the texture gradient

	SecureZeroMemory(&material, sizeof(material));
	material.Ambient = color;
	material.Diffuse = color;

	triangleCount = model.size()/3; //count triangles in our model

	//create the vertex buffer for our new object
	(*d3d_device)->CreateVertexBuffer(triangleCount*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);

	//initialize variables
	forward = true;
	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;

	//copy over the transformation into the transformedModel and the vertex buffer
	initializePath(traverse);
	update();
}
Enemy::Enemy(const Enemy &copy) {
	//copy over the fileNames
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		vertexFileName[i] = copy.vertexFileName[i];
	}

	//copy over the vectors
	Vertex vertice, tVertice;
	for (int i = 0; i < copy.model.size(); i++) {
		//model
		vertice.x = copy.model.at(i).x;
		vertice.y = copy.model.at(i).y;
		vertice.z = copy.model.at(i).z;
		vertice.normal = copy.model.at(i).normal;

		model.push_back(vertice);

		//transformedModel
		tVertice.x = copy.transformedModel.at(i).x;
		tVertice.y = copy.transformedModel.at(i).y;
		tVertice.z = copy.transformedModel.at(i).z;
		tVertice.normal = copy.transformedModel.at(i).normal;

		transformedModel.push_back(tVertice);
	}

	//gotta deep copy this
	for (int i = 0; i < 8; i++) {
		box.vertices[i] = copy.box.vertices[i];
		untransformedBox.vertices[i] = copy.untransformedBox.vertices[i];
	}

	//deep copy the path
	for (int i = 0; i < copy.traverse.size(); i++) {
		traverse.push_back(copy.traverse.at(i));
	}


	//copy over the device pointer
	d3d_device = copy.d3d_device;
	
	//copy over the trianglecount
	triangleCount = copy.triangleCount;

	//copy over the transform
	transform = copy.transform;

	//create a buffer for new object
	(*d3d_device)->CreateVertexBuffer(triangleCount*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);

	//copy over the material
	material = copy.material;

	//copy over ID
	ID = copy.ID;

	//copy over direction
	forward = copy.forward;
	velocity = copy.velocity;

	update();
}