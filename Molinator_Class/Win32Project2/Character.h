#ifndef CHARACTER_H
#define CHARACTER_H

class Character : public Object {
public:
	Character();
	Character(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color);
	Character (const Character &copy);
	
	//setters
	void setProposedMotion();
	void setLocation(float x, float y, float z);

	//physics
	void initializePhysics();
	void updatePhysics();
	void capPhysics();
	float getYVel();
	bool checkCollision();

	//general
	bool hasMoved();
	void update();

	//movement
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void turnLeft();
	void turnRight();
	void jump();
	void fall(); 

	Transformation getValidTransformations();

private:
	PhysicsProperties motion;
};

#endif
