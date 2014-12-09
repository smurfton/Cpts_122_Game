#ifndef OBSTACLE_H
#define OBSTACLE_H
class Obstacle : public Object {
public:
	Obstacle() {};
	Obstacle(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color, Vector traverse);
	Obstacle(const Obstacle &copy);
	void update();
	void initializePath(Vector traverse);
	void updateVelocity();
	void updatePosition();
	void setLocation(float x, float y, float z);

private:
	vector<Vector> traverse;
	Vector velocity;
	bool forward;
};

#endif