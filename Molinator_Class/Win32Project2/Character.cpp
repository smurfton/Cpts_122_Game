#include "Small_Scene_Render.h"

extern Game ourGame;

//initializers
Character::Character(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color) {
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

	setProposedMotion();
	
	//initialize object physics
	initializePhysics();

	//copy over the transformation into the transformedModel and the vertex buffer
	update();
}
Character::Character(const Character &copy) {
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

	//copy over the physics properties
	motion = copy.motion;

	//initialize proposed motion
	setProposedMotion();

	//copy over ID
	ID = copy.ID;

	update();
}

//physics functions
void Character::initializePhysics() {

	//initialize velocity
	motion.velocity.x = 0;
	motion.velocity.y = 0;
	motion.velocity.z = 0;

	//initialize acceleration
	motion.acceleration.x = 0;
	motion.acceleration.y = -9.81;
	motion.acceleration.z = 0;
}
void Character::updatePhysics() {
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
void Character::capPhysics() {
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
//physics supplementary functions
bool Character::hasMoved() {
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
//general functionality
void Character::update() {

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
//player movement
void Character::moveForward() {

	//calculate the forward vector position based on rotation
	Vector normal;
	normal.x = 1;
	normal.y = 0;
	normal.z = 0;

	//vector only has Y and Z components
	//rotate the vector about the Y axis
	float rotation = transform.rotation.y;
	rotation = D3DXToRadian(rotation);

	normal.x = cos(rotation);
	normal.z = sin(rotation);

	//apportion acceleration equal to magnitude of normal vectors
	motion.velocity.x += MOVE_SPEED*normal.x;
	motion.velocity.z += MOVE_SPEED*normal.z;

}
void Character::moveBackward() {
	//calculate the forward vector position based on rotation
	Vector normal;
	normal.x = 1;
	normal.y = 0;
	normal.z = 0;

	//vector only has Y and Z components
	//rotate the vector about the Y axis
	float rotation = transform.rotation.y;
	rotation = D3DXToRadian(rotation);

	normal.x = cos(rotation);
	normal.z = sin(rotation);

	//apportion acceleration equal to magnitude of normal vectors
	motion.velocity.x -= MOVE_SPEED*normal.x;
	motion.velocity.z -= MOVE_SPEED*normal.z;
}
void Character::moveLeft() {
	//calculate the forward vector position based on rotation
	Vector normal;
	normal.x = 1;
	normal.y = 0;
	normal.z = 0;

	//vector only has Y and Z components
	//rotate the vector about the Y axis
	float rotation = transform.rotation.y;
	rotation = D3DXToRadian(rotation+90);

	normal.x = cos(rotation);
	normal.z = sin(rotation);

	//assign acceleration with respect to this information
	motion.velocity.x += MOVE_SPEED*normal.x;
	motion.velocity.z += MOVE_SPEED*normal.z;
}
void Character::moveRight() {
	//calculate the forward vector position based on rotation
	Vector normal;
	normal.x = 1;
	normal.y = 0;
	normal.z = 0;

	//vector only has Y and Z components
	//rotate the vector about the Y axis
	float rotation = transform.rotation.y;
	rotation = D3DXToRadian(rotation-90);

	normal.x = cos(rotation);
	normal.z = sin(rotation);

	//assign acceleration with respect to this information
	motion.velocity.x += MOVE_SPEED*normal.x;
	motion.velocity.z += MOVE_SPEED*normal.z;
}
void Character::turnLeft() {
	proposedMovement.rotation.y += 5;

}
void Character::turnRight() {
	proposedMovement.rotation.y -= 5;

}
void Character::jump() {
	//if (motion.acceleration.y == -9.81f) {
		motion.velocity.y += 140;
	//}
}
void Character::fall() {
	proposedMovement.translation.y -= .1;
}
//transformation functions
Transformation Character::getValidTransformations() {
	Transformation temp, okayTransform = transform;
	float allowedValue = 0;

	//check translations
	//x
	if (proposedMovement.translation.x != transform.translation.x ) { //don't check if there is no proposed movement
		temp = transform;
		temp.translation.x = proposedMovement.translation.x; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox

		if (checkCollision() == false) { //check collision
			okayTransform.translation.x = temp.translation.x; //if no collision, apply transform
		} else {
			//if there is a collision, velocity in this direction should be set to 0
			motion.velocity.x = 0;
			//scale  and try to move a bit to stop glitchiness
			proposedMovement.translation.x = transform.translation.x + .3 * (proposedMovement.translation.x - transform.translation.x); //move it down by a small amount
			temp.translation.x = proposedMovement.translation.x; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((checkCollision() == false)) {
				okayTransform.translation.x = temp.translation.x; //if no collision, apply transform
			}
		}
	}

	//y
	if (proposedMovement.translation.y != transform.translation.y ) { //don't check if there is no proposed movement
		temp = transform;
		temp.translation.y = proposedMovement.translation.y; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.translation.y = temp.translation.y; //if no collision, apply transform
		} else {
			//if there is a collision, velocity in this direction should be set to 0
			motion.velocity.y = 0;
			//scale  and try to move a bit to stop glitchiness
			proposedMovement.translation.y = transform.translation.y + .3 * (proposedMovement.translation.y - transform.translation.y); //move it down by a small amount
			temp.translation.y = proposedMovement.translation.y; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((checkCollision() == false)) {
				okayTransform.translation.y = temp.translation.y; //if no collision, apply transform
			}
		}
	}

	//z
	if (proposedMovement.translation.z != transform.translation.z ) { //don't check if there is no proposed movement
		temp = transform;
		temp.translation.z = proposedMovement.translation.z; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.translation.z = temp.translation.z; //if no collision, apply transform
		} else {
			//if there is a collision, velocity in this direction should be set to 0
			motion.velocity.z = 0;

			//scale  and try to move a bit to stop glitchiness
			proposedMovement.translation.z = transform.translation.z + .3 * (proposedMovement.translation.z - transform.translation.z); //move it down by a small amount
			temp.translation.z = proposedMovement.translation.z; //transform to test
			box = transformHitBox(untransformedBox, temp); //transform hitbox
			if ((checkCollision() == false)) {
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
		if (checkCollision() == false) { //check collision
			okayTransform.scaling.x = temp.scaling.x; //if no collision, apply transform
		}
	}

	//y
	if (proposedMovement.scaling.y != transform.scaling.y ) { //don't check if there is no proposed movement
		temp = transform;
		temp.scaling.y = proposedMovement.scaling.y; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.scaling.y = temp.scaling.y; //if no collision, apply transform
		}
	}

	//z	
	if (proposedMovement.scaling.z != transform.scaling.z ) { //don't check if there is no proposed movement
		temp = transform;
		temp.scaling.z = proposedMovement.scaling.z; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.scaling.z = temp.scaling.z; //if no collision, apply transform
		}
	}

	//check rotations
	//x
	if (proposedMovement.rotation.x != transform.rotation.x ) { //don't check if there is no proposed movement
		temp = transform;
		temp.rotation.x = proposedMovement.rotation.x; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.rotation.x = temp.rotation.x; //if no collision, apply transform
		}
	}

	//y
	if (proposedMovement.rotation.y != transform.rotation.y ) { //don't check if there is no proposed movement
		temp = transform;
		temp.rotation.y = proposedMovement.rotation.y; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.rotation.y = temp.rotation.y; //if no collision, apply transform
		}
	}

	//z	
	if (proposedMovement.rotation.z != transform.rotation.z ) { //don't check if there is no proposed movement
		temp = transform;
		temp.rotation.z = proposedMovement.rotation.z; //transform to test
		box = transformHitBox(untransformedBox, temp); //transform hitbox
		if (checkCollision() == false) { //check collision
			okayTransform.rotation.z = temp.rotation.z; //if no collision, apply transform
		}
	}
	return okayTransform;
}
//setters
void Character::setProposedMotion() {
	proposedMovement = transform;
}
//getters
float Character::getYVel() {
	return motion.velocity.y;
}
bool Character::checkCollision() {

	bool collision = false;

	//loop through all of the objects in the elements vector
	for (int i = 0; i < ourGame.elements.size(); i++) {
		//check for collision
		if (checkVerticesCube(ourGame.elements.at(i).getHitBox(), box) || checkVerticesCube(box, ourGame.elements.at(i).getHitBox())) {
			//if there is a collision, set flag to true
			collision = true;
		}
	}

	//loop through all of the other characters in the game
	for (int i = 0; i < ourGame.characters.size(); i++) {

		//ensure we are not checking if an object is colliding with itself
		if (ourGame.characters.at(i).getID() != ID) {

			//check for collision
			if (checkVerticesCube(ourGame.characters.at(i).getHitBox(), box) || checkVerticesCube(box, ourGame.characters.at(i).getHitBox())) {
				//if there is a collision, set flag to true
				collision = true;
			}
		}
	}

	return collision;
}