#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:
	Object(int ID, char vertexFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device);
	~Object(); //free direct3D resources
	Object(const Object &copy); //copy constructor

	//setters
	void setProposedMotion();

	//getters
	Cube getHitBox();
	int getID();
	Position getLocation();

	//general functionality
	void update();
	void drawObject();

	//Fbx Management Function
	void loadFbx();

	//physics functions
	void initializePhysics();
	void updatePhysics();
	void generateHitBox();
	void capPhysics();

	bool hasMoved();

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

protected:
	wchar_t textureFileName[MAX_FILE_LENGTH];
	char vertexFileName[MAX_FILE_LENGTH];
	vector<Vertex> model, transformedModel;
	Transformation transform, proposedMovement;
	IDirect3DTexture9 *texture;
	LPDIRECT3DVERTEXBUFFER9 buffer;
	int triangleCount;
	LPDIRECT3DDEVICE9 *d3d_device; 
	D3DMATERIAL9 material;
	PhysicsProperties motion;
	Cube box, untransformedBox;
	int ID;
};

#endif