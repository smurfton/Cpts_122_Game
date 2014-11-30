#include "Small_Scene_Render.h"


void moveForward() {
	playerOrientation.location.x += .1;
}
void moveLeft() {
	playerOrientation.location.z += .1;
}
void moveBack() {
	playerOrientation.location.x -= .1;
}
void moveRight() {
	playerOrientation.location.z -= .1;
}
void moveUp() {
	playerOrientation.location.y += .1;
}
void fall() {
	playerOrientation.location.y -= .1;
}
	
void turnRight() {
}
void turnLeft() {
}

void dig() {
	//push underground
	playerOrientation.location.y -= 2;
}

void undig() {
	
	playerOrientation.location.y += 2;
}
