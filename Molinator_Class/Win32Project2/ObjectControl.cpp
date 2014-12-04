#include "Small_Scene_Render.h"


void Object::moveForward() {

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
void Object::moveBackward() {
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
void Object::moveLeft() {
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
void Object::moveRight() {
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
void Object::turnLeft() {
	proposedMovement.rotation.y += 5;

}
void Object::turnRight() {
	proposedMovement.rotation.y -= 5;

}
void Object::jump() {
	//if (motion.acceleration.y == -9.81f) {
		motion.velocity.y += 140;
	//}
}
void Object::fall() {
	proposedMovement.translation.y -= .1;
}