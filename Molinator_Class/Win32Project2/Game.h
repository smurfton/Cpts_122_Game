#ifndef GAME_H
#define GAME_H

class Game {
public:
	/*public variables*/
	Camera cam;
	vector<Object> elements;

	/*public functions*/
	Game ();
	Game(HWND window); //initialize
	~Game(); //deconstruct

	//general functionality
	void addObject(char verticeFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH]);
	void render();
	void initializeLight();
	void initializeMaterial();
	void initialize(HWND window);
	void populateElements();

	bool checkCollision(Cube transformedHitBox, int ID);
	//void checkCollision(Object &moving);

private:
	string elementsFileName;
	LPDIRECT3D9 d3d_interface; 
	LPDIRECT3DDEVICE9 d3d_device; 
	D3DPRESENT_PARAMETERS d3d_interface_parameters;
	vector<Cube> hitBoxes;
};
#endif