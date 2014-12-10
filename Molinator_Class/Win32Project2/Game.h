#ifndef GAME_H
#define GAME_H

class Game {
public:
	/*public variables*/
	Camera cam;
	vector<Object> elements;
	vector<Character> characters;
	vector<Obstacle> obstacles;

	/*public functions*/
	Game ();
	Game(HWND window); //initialize
	~Game(); //deconstruct

	//general functionality
	void addObject(char verticeFileName[MAX_FILE_LENGTH], _D3DCOLORVALUE color);
	void addCharacter(char verticeFileName[MAX_FILE_LENGTH], _D3DCOLORVALUE color);
	void addObstacle(char verticeFileName[MAX_FILE_LENGTH], _D3DCOLORVALUE color, Vector traverse);
	void render();
	void initializeLight();
	void initializeMaterial();
	void initialize(HWND window);
	void populateElements();
	void addDeath();
	int getDeaths();

	//bool checkCollision(Cube transformedHitBox, int ID);
	//void checkCollision(Object &moving);
	void updateCameraLight();

private:
	LPDIRECT3D9 d3d_interface; 
	LPDIRECT3DDEVICE9 d3d_device; 
	D3DPRESENT_PARAMETERS d3d_interface_parameters;
	vector<Cube> hitBoxes;
	int deathCount;
};
#endif