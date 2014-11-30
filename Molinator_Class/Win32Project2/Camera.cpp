#include "Small_Scene_Render.h"

Camera::Camera() {

	d3d_device = NULL;

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