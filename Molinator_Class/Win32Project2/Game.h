#ifndef GAME_H
#define GAME_H

class Game {
public:
	/*public variables*/
	Camera cam;

	/*public functions*/
	Game ();
	Game(HWND window); //initialize
	~Game(); //deconstruct

	//general functionality
	void addObject(D3DMATERIAL9 material, Transformation trans, char verticeFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH]);
	void render();
	void initializeLight();
	void initializeMaterial();
	void initialize(HWND window);
	void populateElements();

private:
	string elementsFileName;
	vector<Object> elements;
	LPDIRECT3D9 d3d_interface; 
	LPDIRECT3DDEVICE9 d3d_device; 
	D3DPRESENT_PARAMETERS d3d_interface_parameters;
};
#endif