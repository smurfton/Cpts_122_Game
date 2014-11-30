#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
	Camera();
	Camera(LPDIRECT3DDEVICE9 *device);
	~Camera(); //doesnt do anything here
	Camera(const Camera &copy);
	Camera &operator =(const Camera &rhs);

	//getters
	Position getLookAt();
	Position getLocation();
	float getAngle();
	float getNearFrame();
	float getFarFrame();

	//setters
	void setLookAt(Position newView);
	void setLocation(Position newLocation);
	void setAngle(float newAngle);
	void setNearFrame(float newNearFrame);
	void setFarFrame(float newFarFrame);
	void setDevice(LPDIRECT3DDEVICE9 *device);

	void update();

private:
	LPDIRECT3DDEVICE9 *d3d_device;
	Position lookAt;
	Position location;
	float angle, nearFrame, farFrame;
};

#endif