#include "Small_Scene_Render.h"

Game::Game () {
	//dont do anything
}

void Game::initialize(HWND window) { 
	//create the interface
	d3d_interface = Direct3DCreate9(D3D_SDK_VERSION);

	SecureZeroMemory(&d3d_interface_parameters, sizeof(D3DPRESENT_PARAMETERS));

	d3d_interface_parameters.hDeviceWindow = window; // set the window to be used by the device
    d3d_interface_parameters.BackBufferFormat = D3DFMT_X8R8G8B8; //set the back buffer format to 32? bit color
    d3d_interface_parameters.BackBufferWidth = SCREEN_WIDTH; //declare the global screen width
    d3d_interface_parameters.BackBufferHeight = SCREEN_HEIGHT; //declar ethe global screen height
    d3d_interface_parameters.Windowed = TRUE; // set the interface to windowed mode
    d3d_interface_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD; //buffer is discarded after displayed
	d3d_interface_parameters.EnableAutoDepthStencil = TRUE; //set depth buffer up
	d3d_interface_parameters.AutoDepthStencilFormat = D3DFMT_D16; //declare the depth buffer to be 16 bit pixels

	d3d_interface->CreateDevice(D3DADAPTER_DEFAULT, //default adapter
								D3DDEVTYPE_HAL, //type of device
								window, //window used by device
								D3DCREATE_SOFTWARE_VERTEXPROCESSING, //dunno
								&d3d_interface_parameters, //parameters created previously
								&d3d_device); //device to store creation in
	

	//set render states
	d3d_device->SetRenderState(D3DRS_LIGHTING, TRUE); 
	d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer

	d3d_device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light

	//initialize light
	initializeLight();

	//create the camera
	cam.setDevice(&d3d_device);

	elementsFileName = "elements.ali";
}

Game::Game(HWND window) { 
	//create the interface
	d3d_interface = Direct3DCreate9(D3D_SDK_VERSION);

	SecureZeroMemory(&d3d_interface_parameters, sizeof(D3DPRESENT_PARAMETERS));

	d3d_interface_parameters.hDeviceWindow = window; // set the window to be used by the device
    d3d_interface_parameters.BackBufferFormat = D3DFMT_X8R8G8B8; //set the back buffer format to 32? bit color
    d3d_interface_parameters.BackBufferWidth = SCREEN_WIDTH; //declare the global screen width
    d3d_interface_parameters.BackBufferHeight = SCREEN_HEIGHT; //declar ethe global screen height
    d3d_interface_parameters.Windowed = TRUE; // set the interface to windowed mode
    d3d_interface_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD; //buffer is discarded after displayed
	d3d_interface_parameters.EnableAutoDepthStencil = TRUE; //set depth buffer up
	d3d_interface_parameters.AutoDepthStencilFormat = D3DFMT_D16; //declare the depth buffer to be 16 bit pixels

	d3d_interface->CreateDevice(D3DADAPTER_DEFAULT, //default adapter
								D3DDEVTYPE_HAL, //type of device
								window, //window used by device
								D3DCREATE_SOFTWARE_VERTEXPROCESSING, //dunno
								&d3d_interface_parameters, //parameters created previously
								&d3d_device); //device to store creation in
	

	//set render states
	d3d_device->SetRenderState(D3DRS_LIGHTING, TRUE); 
	d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer

	d3d_device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light

	//initialize light
	initializeLight();

	//create the camera
	cam.setDevice(&d3d_device);
}
Game::~Game() { //deconstruct

	//free all the objects
	/*for (int i = 0; i < elements.size(); i++) {
		elements.at(i).~Object(); //release each elements dependencies
	}*/

	//free the device
	d3d_device->Release();

	//free the interface
	d3d_interface->Release();
}

//general functionality
void Game::addObject(D3DMATERIAL9 material, Transformation trans, char verticeFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH]) {
	//create the new object
	Object newObject(material, trans, verticeFileName, textureFileName, &d3d_device);
	//push the object onto the vector
	elements.push_back(newObject);
}
void Game::render() {
	d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0); //clear the buffer
	d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //clear the z buffer, too

	//begin the next scene
	d3d_device->BeginScene();

	//set our vertex format
	d3d_device->SetFVF(CUSTOMFVF);

	/*Begin transformation pipeline! Please move to separate functions to simplify*/
	//update the camera
	cam.update();

	//draw all the elements of the game
	for (int i = 0; i < elements.size(); i++) {

		//update element
		elements.at(i).update();

		//draw element
		elements.at(i).drawObject();
	}

	//end the scene
	d3d_device->EndScene();

	//present nothing (we are throwing away buffers after use)
	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void Game::initializeLight() {
	D3DLIGHT9 light;    // create the light struct

    SecureZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);    // set the light's color
    D3DXVECTOR3 vecDir = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);
	
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

    d3d_device->SetLight(0, &light);    // send the light struct properties to light #0
    d3d_device->LightEnable(0, TRUE);    // turn on light #0
}


void Game::populateElements() {
	D3DMATERIAL9 material;
	Transformation transform;
	string input;
	wchar_t textureFile[MAX_FILE_LENGTH];
	char modelFile[MAX_FILE_LENGTH];

	//open the file
	std::fstream inputFile;

	inputFile.open(elementsFileName, std::fstream::in);

	while (inputFile) {
		//scan in the material information
		
		//diffuse
		//input >> material.Diffuse;
		//ambient

		//scan in the transformation information

		//scan in the model File name

		//scan in the texture file name

	}

}