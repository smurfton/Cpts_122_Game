#include "Small_Scene_Render.h"

extern Game ourGame;

Object::Object() {
	//do nothing
}
Object::Object(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color) {
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

	//copy over the transformation into the transformedModel and the vertex buffer
	update();

}
Object::~Object() { //free direct3D resources
	//free buffer resources
	buffer->Release();
}

Object::Object(const Object &copy) { //copy constructor

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

	update();
}
//general functionality
void Object::update() {

	//only recalculate if we are initializing them
	if (transformedModel.size() == 0) {
		//apply transformations
		transformVector(transform, model, transformedModel); 

		//calculate the vertex normals
		calculateNormal(transformedModel);

		//create the hitbox
		box = transformHitBox(untransformedBox, transform); 
		
		
	}

	//copy the information into the buffer
		void *pVoid;
		buffer->Lock(0,0, (void **)&pVoid, 0);
		memcpy(pVoid, &transformedModel[0], transformedModel.size()*sizeof(Vertex));
		buffer->Unlock();

}
void Object::drawObject() {

	//set the material
	(*d3d_device)->SetMaterial(&material);

	//set the stream source
	(*d3d_device)->SetStreamSource(0, buffer, 0, sizeof(Vertex));

	//draw the primitive
	(*d3d_device)->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
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
Position Object::getLocation() {
	return transform.translation;
}

void Object::setLocation(float x, float y, float z) {
	transform.translation.x = x;
	transform.translation.y = y;
	transform.translation.z = z;

	
	//apply transformations
	transformVector(transform, model, transformedModel); 

	//calculate the vertex normals
	calculateNormal(transformedModel);

	//create the hitbox
	box = transformHitBox(untransformedBox, transform); 
}