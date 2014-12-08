#include "Small_Scene_Render.h"

extern Game ourGame;

Camera::Camera() {

	d3d_device = NULL;

	//set the lookAt
	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 0;

	//set the camera position
	location.x = 0;
	location.y = 3;
	location.z = 5;

	//set the camera view angle
	angle = 45;

	//set the near frame
	nearFrame = 1;

	//set the far frame
	farFrame = 100;
}

Camera::Camera(LPDIRECT3DDEVICE9 *device) {

	//set the device pointer
	d3d_device = device;

	//set the lookAt
	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 0;

	//set the camera position
	location.x = 10;
	location.y = 10;
	location.z = 10;

	//set the camera view angle
	angle = 45;

	//set the near frame
	nearFrame = 1;

	//set the far frame
	farFrame = 100;

}
Camera::~Camera() { //doesnt do anything here
}
Camera::Camera(const Camera &copy) {
	nearFrame = copy.nearFrame;
	farFrame = copy.farFrame;
	angle = copy.angle;
	d3d_device = copy.d3d_device;
	location = copy.location;
	lookAt = copy.lookAt;
}

Camera & Camera::operator =(const Camera &rhs) {
	nearFrame = rhs.nearFrame;
	farFrame = rhs.farFrame;
	angle = rhs.angle;
	d3d_device = rhs.d3d_device;
	location = rhs.location;
	lookAt = rhs.lookAt;
	return *this;
}

//getters
Position Camera::getLookAt() {
	return lookAt;
}
Position Camera::getLocation() {
	return location;
}
float Camera::getAngle() {
	return angle;
}
float Camera::getNearFrame() {
	return nearFrame;
}
float Camera::getFarFrame() {
	return farFrame;
}

//setters
void Camera::setLookAt(Position newView) {
	lookAt = newView;
}
void Camera::setLocation(Position newLocation) {
	location = newLocation;
}
void Camera::setAngle(float newAngle) {
	angle = newAngle;
}
void Camera::setNearFrame(float newNearFrame) {
	nearFrame = newNearFrame;
}
void Camera::setFarFrame(float newFarFrame) {
	farFrame = newFarFrame;
}
void Camera::setDevice(LPDIRECT3DDEVICE9 *device) {
	d3d_device = device;
}

void Camera::update() {
	D3DXMATRIX viewMatrix;
	D3DXMATRIX lensMatrix;

	//set the lookAt position to be the character position
	lookAt = ourGame.characters.at(0).getLocation();

	D3DXMatrixLookAtLH(&viewMatrix,
					   &D3DXVECTOR3 (location.x, location.y, location.z),    // the camera position
					   &D3DXVECTOR3 (lookAt.x, lookAt.y, lookAt.z),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

	(*d3d_device)->SetTransform(D3DTS_VIEW, &viewMatrix); //set the camera view point

	D3DXMatrixPerspectiveFovLH(&lensMatrix, //Set the camera lens
                               D3DXToRadian(angle),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               nearFrame,    // the near view-plane
                               farFrame);    // the far view-plane
	(*d3d_device)->SetTransform(D3DTS_PROJECTION, &lensMatrix); //set the camera lens 
}

void Camera::rotateHorizontal(float angleCW) {
	//first calculate the number of radians we are currently at
	float currentAngle = 0, newAngle = 0;
	Position newLocation;

	//check boundary conditions. Will not know if z=0 if at 0 or 180!

	
	if ((location.x-lookAt.x) > 0) {
		currentAngle = asin((location.z-lookAt.z)/sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.z-lookAt.z)*(location.z-lookAt.z)));
	} else if (location.x-lookAt.x == 0) {
		if ((location.z-lookAt.z) > 0) {
			currentAngle = D3DXToRadian(90);
		} else {
			currentAngle = D3DXToRadian(270);
		}
	} else {
		currentAngle = acos((location.z-lookAt.z)/sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.z-lookAt.z)*(location.z-lookAt.z)));
		currentAngle += D3DXToRadian(90);
	}
	//now add in the new rotation
	newAngle = currentAngle + D3DXToRadian(angleCW);

	//now recalculate the locations. y is unchanged
	newLocation.y = location.y;
	newLocation.x = cos(newAngle)*sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.z-lookAt.z)*(location.z-lookAt.z)) + lookAt.x;
	newLocation.z = sin(newAngle)*sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.z-lookAt.z)*(location.z-lookAt.z)) + lookAt.z;

	//set the location
	location = newLocation;

}
void Camera::zoom(float distanceIn) {
	//we need to change the camera location. Camera will stay on the same vector
	Vector position;
	float magnitude = 0, length = 0;
	position.x = location.x - lookAt.x;
	position.y = location.y - lookAt.y;
	position.z = location.z - lookAt.z;

	magnitude = calculateMagnitude(position);
	//we need to now construct a new vector of new length

	length = magnitude-distanceIn;
	//our final vector will have the same proportions of the position vector,
	//therefore we can simply scale the position vector

	//check to ensure we aren't at max zoom. if we are, then stop zooming

	if (length > 2.5) {
		position = scaleVector(position, length);
		location.x = position.x + lookAt.x;
		location.y = position.y + lookAt.y;
		location.z = position.z + lookAt.z;
	}
}

void Camera::rotateVertical(float angleUp) {
	//first calculate the number of radians we are currently at
	float currentAngle = 0, newAngle = 0;
	Position newLocation;

	Vector tempV;
	tempV.x = location.x - lookAt.x;
	tempV.y = location.y - lookAt.y;
	tempV.z = location.z - lookAt.z;

	float magnitude = calculateMagnitude(tempV);

	if ((location.x-lookAt.x) > 0) {
		currentAngle = asin((location.y-lookAt.y)/sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.y-lookAt.y)*(location.y-lookAt.y)));
	} else if (location.x-lookAt.x == 0) {
		if ((location.y-lookAt.y) > 0) {
			currentAngle = D3DXToRadian(90);
		} else {
			currentAngle = D3DXToRadian(270);
		}
	} else {
		currentAngle = acos((location.y-lookAt.y)/sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.y-lookAt.y)*(location.y-lookAt.y)));
		currentAngle += D3DXToRadian(90);
	}

	//now add in the new rotation
	newAngle = currentAngle + D3DXToRadian(angleUp);

	//now recalculate the locations.
	newLocation.x = cos(newAngle)*sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.y-lookAt.y)*(location.y-lookAt.y)) + lookAt.x;
	newLocation.y = sin(newAngle)*sqrt((location.x-lookAt.x)*(location.x-lookAt.x)+(location.y-lookAt.y)*(location.y-lookAt.y)) + lookAt.y;
	newLocation.z = location.z;

	//set the location
	location = newLocation;
}