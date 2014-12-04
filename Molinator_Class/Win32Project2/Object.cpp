#include "Small_Scene_Render.h"

extern Game ourGame;

Object::Object(int ID, char vertexFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device) {
	//initialize the object
	d3d_device = device;

	//copy over the strings for filenames
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		this->vertexFileName[i] = vertexFileName[i];
		this->textureFileName[i] = textureFileName[i];
	}

	//initialize the ID
	this->ID = ID;

	//loadFBX(box, &model, vertexFileName); //load vertices into our model vector
	loadFbx(); //load vertices into model
	generateHitBox(); //generate a hitbox for the model
	
	setTextureGradient(model); //initialize the texture gradient

	//create the texture for the object
	if (textureFileName != NULL) {
		D3DXCreateTextureFromFile(*d3d_device, textureFileName, &texture); 
	}

	SecureZeroMemory(&material, sizeof(material));
	material.Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	material.Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);

	triangleCount = model.size()/3; //count triangles in our model

	//create the vertex buffer for our new object
	(*d3d_device)->CreateVertexBuffer(triangleCount*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);

	setProposedMotion();
	
	//initialize object physics
	initializePhysics();

	//copy over the transformation into the transformedModel and the vertex buffer
	update();

}
Object::~Object() { //free direct3D resources
	//free texture resources
	if (texture != nullptr) {
		texture->Release();
	}
	//free buffer resources
	buffer->Release();
}

Object::Object(const Object &copy) { //copy constructor

	//copy over the fileNames
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		vertexFileName[i] = copy.vertexFileName[i];
		if (copy.textureFileName != NULL) {
			textureFileName[i] = copy.textureFileName[i];
		}
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
		vertice.normal = copy.model.at(i).normal;

		model.push_back(vertice);

		//transformedModel
		tVertice.x = copy.transformedModel.at(i).x;
		tVertice.y = copy.transformedModel.at(i).y;
		tVertice.z = copy.transformedModel.at(i).z;
		tVertice.tx = copy.transformedModel.at(i).tx;
		tVertice.ty = copy.transformedModel.at(i).ty;
		tVertice.normal = copy.transformedModel.at(i).normal;

		transformedModel.push_back(tVertice);
	}

	//gotta deep copy this
	for (int i = 0; i < 8; i++) {
		box.vertices[i] = copy.box.vertices[i];
		untransformedBox.vertices[i] = copy.untransformedBox.vertices[i];
	}


	//copy over the device pointer
	d3d_device = copy.d3d_device;
	
	//copy over the trianglecount
	triangleCount = copy.triangleCount;

	//copy over the transform
	transform = copy.transform;

	//create a buffer for new object
	(*d3d_device)->CreateVertexBuffer(triangleCount*sizeof(Vertex)*3, 0, CUSTOMFVF, D3DPOOL_MANAGED, &buffer, NULL);

	//create a texture for copy
	if (copy.textureFileName != NULL) {
		D3DXCreateTextureFromFile(*d3d_device, textureFileName, &texture); 
	}

	//copy over the material
	material = copy.material;

	//copy over the physics properties
	motion = copy.motion;

	//initialize proposed motion
	setProposedMotion();

	//copy over ID
	ID = copy.ID;

	update();
}
//general functionality
void Object::update() {

	//update physics
	updatePhysics();

	//get valid transformations
	transform = getValidTransformations();

	//only recalculate if the model has moved OR we are initializing them
	if (hasMoved() || transformedModel.size() == 0) {
		//apply transformations
		transformVector(transform, model, transformedModel); //model

		//calculate the vertex normals
		calculateNormal(transformedModel);

		box = transformHitBox(untransformedBox, transform); //hitbox
	}

	//reset motion
	setProposedMotion();
	//update physics


	void *pVoid;
	buffer->Lock(0,0, (void **)&pVoid, 0);
	memcpy(pVoid, &transformedModel[0], transformedModel.size()*sizeof(Vertex));
	buffer->Unlock();
}
void Object::drawObject() {

	//set the texture
	if (texture != nullptr) {
		(*d3d_device)->SetTexture(0, texture);
	}

	//set the material
	(*d3d_device)->SetMaterial(&material);

	//set the stream source
	(*d3d_device)->SetStreamSource(0, buffer, 0, sizeof(Vertex));

	//update the camera
	ourGame.cam.update();


	//draw the primitive
	(*d3d_device)->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
}
void Object::initializePhysics() {

	//initialize velocity
	motion.velocity.x = 0;
	motion.velocity.y = 0;
	motion.velocity.z = 0;

	//initialize acceleration
	motion.acceleration.x = 0;

	if (strcmp("Ground.FBX", vertexFileName) ==0 ) {
		motion.acceleration.y = 0;
	} else {
		motion.acceleration.y = -9.81;
	}
	motion.acceleration.z = 0;
}
void Object::updatePhysics() {
	//update physics
	//affect the proposed transform

	//bind physics
	capPhysics();

	//first, check accelerations and use these to adjust the velocity
	if (motion.acceleration.x != 0) {
		motion.velocity.x += motion.acceleration.x;
		//dampen
		motion.acceleration.x *= .65;
	}
	if (motion.acceleration.y != 0) {
		motion.velocity.y +=  motion.acceleration.y;
		//dampen
		if (motion.acceleration.y != -9.81f) {
			motion.acceleration.y = (motion.acceleration.y+9.81) * .95 - 9.81;
		}
	}
	if (motion.acceleration.z != 0) {
		motion.velocity.z +=  motion.acceleration.z;
		//dampen
		motion.acceleration.z *= .65;

	}

	//apply velocity to transformation
	if (motion.velocity.x != 0) {
		proposedMovement.translation.x += .01 * motion.velocity.x;
		//dampen
		motion.velocity.x *= .75;
	}
	
	if (motion.velocity.y != 0) {
		proposedMovement.translation.y += .01 * motion.velocity.y;
		//dampen
		motion.velocity.y *= .75;
	}
	
	if (motion.velocity.z != 0) {
		proposedMovement.translation.z += .01 * motion.velocity.z;
		//dampen
		motion.velocity.z *= .75;
	}


}
void Object::loadFbx() {
	//create our manager
	FbxManager *manager = FbxManager::Create();

	// create the settings
	FbxIOSettings * ios = FbxIOSettings::Create(manager, IOSROOT );
	manager->SetIOSettings(ios);
	
	// make the importer
	FbxImporter* importer = FbxImporter::Create(manager, "");

	//initialize importer
	importer->Initialize(vertexFileName, -1, manager->GetIOSettings());

	//create the scene
	FbxScene *scene = FbxScene::Create(manager, vertexFileName);

	//import scene
	importer->Import(scene);

	//destroy importer
	importer->Destroy();

	//get the root node for our scene
	FbxNode *root = scene->GetRootNode();

	//create our object 
	FbxNode *object = root->GetChild(0);

	//gather information about our object

	//transformation information
	FbxDouble3 translation = object->LclTranslation.Get();
	transform.translation.x = translation.mData[0];
	transform.translation.y = translation.mData[1];
	transform.translation.z = translation.mData[2];

	FbxDouble3 rotation = object->LclRotation.Get();
	transform.rotation.x = rotation.mData[0];
	transform.rotation.y = rotation.mData[1];
	transform.rotation.z = rotation.mData[2];

	FbxDouble3 scaling = object->LclScaling.Get();
	transform.scaling.x = scaling.mData[0];
	transform.scaling.y = scaling.mData[1];
	transform.scaling.z = scaling.mData[2];


	//material information
	FbxSurfaceMaterial *mat = object->GetMaterial(0);
	//string amb = mat->sAmbient;
	//string diff = mat->sDiffuse;

	//store this info into the material object

	//get the mesh
	FbxMesh *mesh = (FbxMesh *)object->GetNodeAttribute();

	FbxVector4 *verticeInfo = mesh->GetControlPoints();

	//grab all the vertex information from the file!
	for (int i = 0; i < mesh->GetPolygonCount(); i++) { //loop through each polygon
		
		//loop through each vertice of the polygon
		for (int j = 0; j < mesh->GetPolygonSize(i); j++) {

			//get the index of the polygon were working with from the verticeInfo object
			int indexPolygon = mesh->GetPolygonVertex(i, j);
			Vertex vertice;

			//store the accessed information into the vertice
			vertice.x = (float)verticeInfo[indexPolygon].mData[0];
			vertice.y = (float)verticeInfo[indexPolygon].mData[1];
			vertice.z = (float)verticeInfo[indexPolygon].mData[2];

			//push vertice onto the vector of vertices
			model.push_back(vertice);
			
		}

	}
}
void Object::generateHitBox() {
	//find the min and max values for X Y and Z and construct a cube structure as an untransformed hitbox
	float minX = model.at(0).x, maxX = model.at(0).x, minY = model.at(0).y, maxY = model.at(0).y, minZ = model.at(0).z, maxZ = model.at(0).z;
	
	//loop through every vertice in the model file
	for (int i = 1; i < model.size(); i++) {
		//check if new X is largest
		if (model.at(i).x > maxX) {
			maxX = model.at(i).x;
		} else if (model.at(i).x < minX) { //else check if its the smallest
			minX = model.at(i).x;
		}

		//now check y
		if (model.at(i).y > maxY) {
			maxY = model.at(i).y;
		} else if (model.at(i).y < minY) { //else check if its the smallest
			minY = model.at(i).y;
		}

		//now check z
		if (model.at(i).z > maxZ) {
			maxZ = model.at(i).z;
		} else if (model.at(i).z < minZ) { //else check if its the smallest
			minZ = model.at(i).z;
		}
	}

	//now create a cube with vertices
	Vertex v1, v2, v3, v4, v5, v6, v7, v8;

	//maxX face
	v1.x = maxX;
	v1.y = maxY;
	v1.z = maxZ;

	v2.x = maxX;
	v2.y = maxY;
	v2.z = minZ;

	v3.x = maxX;
	v3.y = minY;
	v3.z = maxZ;

	v4.x = maxX;
	v4.y = minY;
	v4.z = minZ;

	//minX face
	
	v5.x = minX;
	v5.y = maxY;
	v5.z = maxZ;

	v6.x = minX;
	v6.y = maxY;
	v6.z = minZ;

	v7.x = minX;
	v7.y = minY;
	v7.z = maxZ;

	v8.x = minX;
	v8.y = minY;
	v8.z = minZ;

	//push vertices into box structure
	box.vertices[0] = v1;
	box.vertices[1] = v2;
	box.vertices[2] = v3;
	box.vertices[3] = v4;
	box.vertices[4] = v5;
	box.vertices[5] = v6;
	box.vertices[6] = v7;
	box.vertices[7] = v8;

	//copy info to untransformed box
	untransformedBox.vertices[0] = v1;
	untransformedBox.vertices[1] = v2;
	untransformedBox.vertices[2] = v3;
	untransformedBox.vertices[3] = v4;
	untransformedBox.vertices[4] = v5;
	untransformedBox.vertices[5] = v6;
	untransformedBox.vertices[6] = v7;
	untransformedBox.vertices[7] = v8;

	//our untransformed hitbox has now been created
}
//getters
Cube Object::getHitBox() {
	return box;
}
int Object::getID() {
	return ID;
}
//setters
void Object::setProposedMotion() {
	/*
	proposedMovement.rotation.x = 0;
	proposedMovement.rotation.y = 0;
	proposedMovement.rotation.z = 0;
	proposedMovement.scaling.x = 1;
	proposedMovement.scaling.y = 1;
	proposedMovement.scaling.z = 1;
	proposedMovement.translation.x = 0;
	proposedMovement.translation.y = 0;
	proposedMovement.translation.z = 0;*/
	proposedMovement = transform;
}
bool Object::hasMoved() {
	bool hasIt = false;
	if (proposedMovement.rotation.x != 0 || proposedMovement.rotation.y != 0 || proposedMovement.rotation.z != 0) { //if it got rotated
		hasIt = true;
	} else if (proposedMovement.translation.x != 0 || proposedMovement.translation.y != 0 || proposedMovement.translation.z != 0) {
		hasIt = true;
	} else if (proposedMovement.scaling.x != 1 || proposedMovement.scaling.y != 1 || proposedMovement.scaling.z != 1) {
		hasIt = true;
	}
	return hasIt;

}
Position Object::getLocation() {
	return transform.translation;
}
Transformation Object::getValidTransformations() {
	Transformation temp, okayTransform = transform;
	float allowedValue = 0;

	//check translations
	//x
	if (proposedMovement.translation.x != transform.translation.x ) { //don't check if there is no proposed movement
		temp = transform;
		temp.translation.x = proposedMovement.translation.x; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox

		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.translation.x = temp.translation.x; //if no collision, apply transform
		} else {
			//if there is a collision, velocity in this direction should be set to 0
			motion.velocity.x = 0;
			//scale  and try to move a bit to stop glitchiness
			proposedMovement.translation.x = transform.translation.x + .3 * (proposedMovement.translation.x - transform.translation.x); //move it down by a small amount
			temp.translation.x = proposedMovement.translation.x; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((ourGame.checkCollision(box, ID) == false)) {
				okayTransform.translation.x = temp.translation.x; //if no collision, apply transform
			}
		}
	}

	//y
	if (proposedMovement.translation.y != transform.translation.y ) { //don't check if there is no proposed movement
		temp = transform;
		temp.translation.y = proposedMovement.translation.y; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.translation.y = temp.translation.y; //if no collision, apply transform
		} else {
			//if there is a collision, velocity in this direction should be set to 0
			motion.velocity.y = 0;
			//scale  and try to move a bit to stop glitchiness
			proposedMovement.translation.y = transform.translation.y + .3 * (proposedMovement.translation.y - transform.translation.y); //move it down by a small amount
			temp.translation.y = proposedMovement.translation.y; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((ourGame.checkCollision(box, ID) == false)) {
				okayTransform.translation.y = temp.translation.y; //if no collision, apply transform
			}
		}
	}

	//z
	if (proposedMovement.translation.z != transform.translation.z ) { //don't check if there is no proposed movement
		temp = transform;
		temp.translation.z = proposedMovement.translation.z; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.translation.z = temp.translation.z; //if no collision, apply transform
		} else {
			//if there is a collision, velocity in this direction should be set to 0
			motion.velocity.z = 0;

			//scale  and try to move a bit to stop glitchiness
			proposedMovement.translation.z = transform.translation.z + .3 * (proposedMovement.translation.z - transform.translation.z); //move it down by a small amount
			temp.translation.z = proposedMovement.translation.z; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((ourGame.checkCollision(box, ID) == false)) {
				okayTransform.translation.z = temp.translation.z; //if no collision, apply transform
			}
		}
		
		/* else {
			//scale  and retry
			proposedMovement.translation.z = transform.translation.z + .01 * (proposedMovement.translation.z - transform.translation.z); //move it down by a small amount
			temp.translation.z = proposedMovement.translation.z; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((ourGame.checkCollision(box, ID) == false)) {
				okayTransform.translation.z = temp.translation.z; //if no collision, apply transform
			}
		}*/
	}

	//check scales
	//x
	if (proposedMovement.scaling.x != transform.scaling.x ) { //don't check if there is no proposed movement
		temp = transform;
		temp.scaling.x = proposedMovement.scaling.x; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.scaling.x = temp.scaling.x; //if no collision, apply transform
		}
	}

	//y
	if (proposedMovement.scaling.y != transform.scaling.y ) { //don't check if there is no proposed movement
		temp = transform;
		temp.scaling.y = proposedMovement.scaling.y; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.scaling.y = temp.scaling.y; //if no collision, apply transform
		}
	}

	//z	
	if (proposedMovement.scaling.z != transform.scaling.z ) { //don't check if there is no proposed movement
		temp = transform;
		temp.scaling.z = proposedMovement.scaling.z; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.scaling.z = temp.scaling.z; //if no collision, apply transform
		}
	}

	//check rotations
	//x
	if (proposedMovement.rotation.x != transform.rotation.x ) { //don't check if there is no proposed movement
		temp = transform;
		temp.rotation.x = proposedMovement.rotation.x; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.rotation.x = temp.rotation.x; //if no collision, apply transform
		}
	}

	//y
	if (proposedMovement.rotation.y != transform.rotation.y ) { //don't check if there is no proposed movement
		temp = transform;
		temp.rotation.y = proposedMovement.rotation.y; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.rotation.y = temp.rotation.y; //if no collision, apply transform
		}
	}

	//z	
	if (proposedMovement.rotation.z != transform.rotation.z ) { //don't check if there is no proposed movement
		temp = transform;
		temp.rotation.z = proposedMovement.rotation.z; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (ourGame.checkCollision(box, ID) == false) { //check collision
			okayTransform.rotation.z = temp.rotation.z; //if no collision, apply transform
		}
	}
	return okayTransform;
}
void Object::capPhysics() {
	//set bounds on physics properties

	//bind the acceleration
	if (motion.acceleration.x > MAX_ACC) {
		motion.acceleration.x = MAX_ACC;
	} else if (motion.acceleration.x < -MAX_ACC) {
		motion.acceleration.x = -MAX_ACC;
	}
	if (motion.acceleration.y > MAX_ACC) {
		motion.acceleration.y = MAX_ACC;
	} else if (motion.acceleration.y < -MAX_ACC) {
		motion.acceleration.y = -MAX_ACC;
	}
	if (motion.acceleration.z > MAX_ACC) {
		motion.acceleration.z = MAX_ACC;
	} else if (motion.acceleration.z < -MAX_ACC) {
		motion.acceleration.z = -MAX_ACC;
	}

	//bind the velocity
	if (motion.velocity.x > MAX_VEL) {
		motion.velocity.x = MAX_VEL;
	} else if (motion.velocity.x < -MAX_VEL) {
		motion.velocity.x = -MAX_VEL;
	}
	if (motion.velocity.y > MAX_VEL) {
		motion.velocity.y = MAX_VEL;
	} else if (motion.velocity.y < -MAX_VEL) {
		motion.velocity.y = -MAX_VEL;
	}
	if (motion.velocity.z > MAX_VEL) {
		motion.velocity.z = MAX_VEL;
	} else if (motion.velocity.z < -MAX_VEL) {
		motion.velocity.z = -MAX_VEL;
	}
}

float Object::getYVel() {
	return motion.velocity.y;
}