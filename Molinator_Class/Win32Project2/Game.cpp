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
void Game::addObject(char verticeFileName[MAX_FILE_LENGTH], wchar_t textureFileName[MAX_FILE_LENGTH]) {
	//create the new object
	Object newObject(elements.size(), verticeFileName, textureFileName, &d3d_device);
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

	//draw all the elements of the game
	for (int i = 0; i < elements.size(); i++) {

		//update element
		elements.at(i).update();

		//draw element
		elements.at(i).drawObject();
	}
	cam.update();

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

//function returns true if a collision occurs
bool Game::checkCollision(Cube transformedHitBox, int ID) {

	bool collision = false;
	//loop through all of the objects in the elements vector
	for (int i = 0; i < elements.size(); i++) {

		//ensure we are not checking if an object is colliding with itself
		if (elements.at(i).getID() != ID) {

			//check for collision
			if (checkVerticesCube(elements.at(i).getHitBox(), transformedHitBox) || checkVerticesCube(transformedHitBox, elements.at(i).getHitBox())) {
				//if there is a collision, set flag to true
				collision = true;
			}
		}
	}

	return collision;
}

//rewrite check collision function
/*void Game::checkCollision(Object &moving) {


	moving.box = transformHitBox(moving.box, moving.transform);

	for (int i = 0; i < elements.size(); i++) {

		//ensure we are not checking if an object is colliding with itself
		if (elements.at(i).getID() != moving.ID) {

			//check for collision
			while (!(checkVerticesCube(elements.at(i).getHitBox(), moving.box) || checkVerticesCube(moving.box, elements.at(i).getHitBox()))) {
				
				//if there is a collision, calculate how far we can go
				moving.proposedMovement.translation.x -= .1 * (moving.proposedMovement.translation.x - moving.transform.translation.x);
				moving.proposedMovement.translation.y -= .1 * (moving.proposedMovement.translation.y - moving.transform.translation.y);
				moving.proposedMovement.translation.z -= .1 * (moving.proposedMovement.translation.z - moving.transform.translation.z);

				
				moving.proposedMovement.rotation.x -= .1 * (moving.proposedMovement.rotation.x - moving.transform.rotation.x);
				moving.proposedMovement.rotation.y -= .1 * (moving.proposedMovement.rotation.y - moving.transform.rotation.y);
				moving.proposedMovement.rotation.z -= .1 * (moving.proposedMovement.rotation.z - moving.transform.rotation.z);
				
				moving.proposedMovement.scaling.x -= .1 * (moving.proposedMovement.scaling.x - moving.transform.scaling.x);
				moving.proposedMovement.scaling.y -= .1 * (moving.proposedMovement.scaling.y - moving.transform.scaling.y);
				moving.proposedMovement.scaling.z -= .1 * (moving.proposedMovement.scaling.z - moving.transform.scaling.z);
				//while loop
				//try a proposed movement slightly less
				moving.box = transformHitBox(moving.box, moving.transform);
			}
		}
	}
}*/