#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:
	Object(Transformation trans, char vertexFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device);
	~Object(); //free direct3D resources
	Object(const Object &copy); //copy constructor

	//general functionality
	void update();
	void drawObject();

private:
	wchar_t vertexFileName[MAX_FILE_LENGTH], textureFileName[MAX_FILE_LENGTH];
	vector<Vertex> model, transformedModel;
	Transformation transform;
	IDirect3DTexture9 *texture;
	LPDIRECT3DVERTEXBUFFER9 buffer;
	int triangleCount;
	LPDIRECT3DDEVICE9 *d3d_device; 
};

#endif