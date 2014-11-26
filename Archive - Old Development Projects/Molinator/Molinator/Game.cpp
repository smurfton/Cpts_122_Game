#include "Molinator.h"

extern LPDIRECT3DDEVICE9 d3d_device;

//initialization of device and interface
Game::Game(HWND window) {

	/*Device initialization - this is how we display to the screen*/
	//create the interface
	d3d_interface = Direct3DCreate9(D3D_SDK_VERSION);

	//empty out the memory
	SecureZeroMemory(&d3d_interface_parameters, sizeof(D3DPRESENT_PARAMETERS));

	//set the parameters
	d3d_interface_parameters.hDeviceWindow = window; // set the window to be used by the device
    d3d_interface_parameters.BackBufferFormat = D3DFMT_X8R8G8B8; //set the back buffer format to 32? bit color
    d3d_interface_parameters.BackBufferWidth = SCREEN_WIDTH; //declare the global screen width
    d3d_interface_parameters.BackBufferHeight = SCREEN_HEIGHT; //declar ethe global screen height
    d3d_interface_parameters.Windowed = TRUE; // set the interface to windowed mode
    d3d_interface_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD; //buffer is discarded after displayed
	d3d_interface_parameters.EnableAutoDepthStencil = TRUE; //set depth buffer up
	d3d_interface_parameters.AutoDepthStencilFormat = D3DFMT_D16; //declare the depth buffer to be 16 bit pixels

	//create the device
	d3d_interface->CreateDevice(D3DADAPTER_DEFAULT, //default adapter
								D3DDEVTYPE_HAL, //type of device
								window, //window used by device
								D3DCREATE_SOFTWARE_VERTEXPROCESSING, //dunno
								&d3d_interface_parameters, //parameters created previously
								&d3d_device); //device to store creation in

	d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE); //turn off 3D lighting (I don't know how to use it yet) in the device
	d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	char file[MAX_FILE_LENGTH] = "Main.png";
	for (int i = 0; i < MAX_FILE_LENGTH; i++) {
		fileName[i] = file[i];
	}
}
Game::~Game() {
	//we need to clean up after ourselves

	//release the device and interface
	if (d3d_device != nullptr) d3d_device->Release();
	if (d3d_interface != nullptr) d3d_interface->Release();

	//release static images
	if (mainMenu != nullptr) mainMenu->Release();

}
//getters
LPDIRECT3DDEVICE9 &Game::getDevice() {
	return d3d_device;
}
//general functionality
void Game::render() {
	//clear the current scene
	d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0); //clear the buffer
	d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //clear the z buffer, too

	//begin the next scene
	d3d_device->BeginScene();

	//set our vertex format
	d3d_device->SetFVF(CUSTOMFVF);
	initView(d3d_device);
	/*Begin transformation pipeline! Please move to separate functions to simplify*/
	/*Place Render Information Here!!!*/
	d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 50);


	/*End Render Information*/

	//end the scene
	d3d_device->EndScene();

	//present nothing (we are throwing away buffers after use)
	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void initView(LPDIRECT3DDEVICE9 &device) {
	D3DXMATRIX viewMatrix;
	D3DXMATRIX lensMatrix;

	D3DXMatrixLookAtLH(&viewMatrix,
					   &D3DXVECTOR3 (0, 0, 10),    // the camera position
                       &D3DXVECTOR3 (0, 0, 0),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

	device->SetTransform(D3DTS_VIEW, &viewMatrix); //set the camera view point

	D3DXMatrixPerspectiveFovLH(&lensMatrix, //Set the camera lens
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane
	device->SetTransform(D3DTS_PROJECTION, &lensMatrix); //set the camera lens 
}