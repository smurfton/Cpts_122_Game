#ifndef GAME_H
#define GAME_H

class Game {
public:
	Game(HWND window); //initialize
	~Game(); //deconstruct

	//general functionality
	void addObject(Transformation trans, char verticeFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH]);
	void render();

private:
	Camera cam;
	vector<Object> elements;
	LPDIRECT3D9 d3d_interface; 
	LPDIRECT3DDEVICE9 d3d_device; 
	D3DPRESENT_PARAMETERS d3d_interface_parameters;
};
#endif