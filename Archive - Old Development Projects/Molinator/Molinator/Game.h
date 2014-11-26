class Game {
public:
	Game(HWND window);
	~Game();

	//getters
	LPDIRECT3DDEVICE9 &getDevice();

	//general functionality
	void render();

private:
	LPDIRECT3D9 d3d_interface;
	//LPDIRECT3DDEVICE9 d3d_device;
	D3DPRESENT_PARAMETERS d3d_interface_parameters;
	IDirect3DSurface9 *mainMenu;
	wchar_t fileName[MAX_FILE_LENGTH];
};