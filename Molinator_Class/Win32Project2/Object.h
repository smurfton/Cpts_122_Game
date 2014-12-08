#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:
	Object();
	Object(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color);
	~Object(); //free direct3D resources
	Object(const Object &copy); //copy constructor

	//getters
	Cube getHitBox();
	int getID();
	Position getLocation();
	float getYVel();


	//general functionality
	virtual void update();
	void drawObject();


	//Load FBX model information
	void loadFbx();


	//Hitbox generation
	void generateHitBox();


protected:
	char vertexFileName[MAX_FILE_LENGTH];
	vector<Vertex> model, transformedModel;
	Transformation transform, proposedMovement;
	IDirect3DTexture9 *texture;
	LPDIRECT3DVERTEXBUFFER9 buffer;
	int triangleCount;
	LPDIRECT3DDEVICE9 *d3d_device; 
	D3DMATERIAL9 material;
	Cube box, untransformedBox;
	int ID;
};

#endif