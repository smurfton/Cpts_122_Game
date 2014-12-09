#include "Small_Scene_Render.h"


Obstacle::Obstacle(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color, Vector traverse) {
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
Obstacle::Obstacle(const Obstacle &copy) {
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
void Obstacle::update() {
	updateVelocity();
	updatePosition();

	//apply transformations
	transformVector(transform, model, transformedModel); 

	//calculate the vertex normals
	calculateNormal(transformedModel);

	//create the hitbox
	box = transformHitBox(untransformedBox, transform); 

	//copy the information into the buffer
		void *pVoid;
		buffer->Lock(0,0, (void **)&pVoid, 0);
		memcpy(pVoid, &transformedModel[0], transformedModel.size()*sizeof(Vertex));
		buffer->Unlock();
	
}
void Obstacle::initializePath(Vector traverseEnd) {
	traverse.push_back(transform.translation); //first point on path is the starting location
	//calculate the new vector
	Vector end;
	end.x = transform.translation.x+traverseEnd.x;
	end.y = transform.translation.y+traverseEnd.y;
	end.z = transform.translation.z+traverseEnd.z;
	traverse.push_back(end);
}

void Obstacle::updateVelocity() {
	//check if at endpoints
	if (transform.translation == traverse.at(0)) { 
		//forward is true
		forward = true;
	} else if (transform.translation == traverse.at(1)) {
		forward = false;
	}
	if (forward) {
		velocity = traverse.at(1) - traverse.at(0);
	} else {
		velocity = traverse.at(0) - traverse.at(1);
	}

}

void Obstacle::updatePosition() {

	Vector translation = transform.translation;

	translation.x += .01 * velocity.x;
	translation.y += .01 * velocity.y;
	translation.z += .01 * velocity.z;

	//find the min and max XYZ
	float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
	if (traverse.at(0).x > traverse.at(1).x) {
		maxX = traverse.at(0).x;
		minX = traverse.at(1).x;
	} else {
		maxX = traverse.at(1).x;
		minX = traverse.at(0).x;
	
	}
	if (traverse.at(0).y > traverse.at(1).y) {
		maxY = traverse.at(0).y;
		minY = traverse.at(1).y;
	} else {
		maxY = traverse.at(1).y;
		minY = traverse.at(0).y;
	
	}
	if (traverse.at(0).z > traverse.at(1).z) {
		maxZ = traverse.at(0).z;
		minZ = traverse.at(1).z;
	} else {
		maxZ = traverse.at(1).z;
		minZ = traverse.at(0).z;
	
	}

	if ((translation.x > maxX || translation.x < minX) || (translation.y > maxY) || (translation.y < minY) || (translation.z > maxZ) || (translation.z < minZ)) {
		if (forward) {
			//set the new position to the endpoints
			translation = traverse.at(1);
		} else {
			translation = traverse.at(0);
		}
	}
	transform.translation = translation;
}