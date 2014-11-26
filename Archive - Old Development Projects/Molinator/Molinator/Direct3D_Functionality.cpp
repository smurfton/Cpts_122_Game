#include "Molinator.h"

/*****Function was moved to Game Object******/
/*This function initializes our handle to the direct3D device*/
/*void initializeInterface(HWND window) {

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
}*/

/*****Function was moved to Game Object******/
/*This function renders a single frame to the bakc buffer. The buffer is then rotated forward and displayed*/
/*void render() {
	//clear the current scene
	d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 1.0f, 0); //clear the buffer
	d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //clear the z buffer, too

	//begin the next scene
	d3d_device->BeginScene();

	//set our vertex format
	d3d_device->SetFVF(CUSTOMFVF);

	//Begin transformation pipeline! Please move to separate functions to simplify
	//Place Render Information Here!!!



	//End Render Information

	//end the scene
	d3d_device->EndScene();

	//present nothing (we are throwing away buffers after use)
	d3d_device->Present(NULL, NULL, NULL, NULL);
}*/

/*****Function was moved to Game Object******/
/*This function cleans up any intialized devices used by direct3D on exit*/
/*void cleanD3D() {

	//release vertex buffers
	if (character_Buffer != nullptr) character_Buffer->Release();
	if (enemy_Buffer != nullptr) enemy_Buffer->Release();
	if (environment_Buffer != nullptr) environment_Buffer->Release();

	//release textures
	if (char_texture != nullptr) char_texture->Release();
	if (enemy_texture != nullptr) enemy_texture->Release();
	if (environment_texture != nullptr) environment_texture->Release();
	if (ground_texture != nullptr) ground_texture->Release();

	//release the device and interface
	if (d3d_device != nullptr) d3d_device->Release();
	if (d3d_interface != nullptr) d3d_interface->Release();

	//release static images
	if (mainMenu != nullptr) mainMenu->Release();
}*/

/*****Function was moved to Object Class******/
/*This function loads texture files for use by the device*/
/*void loadNewTexture(wchar_t fileName[], IDirect3DTexture9 *texture) {
	D3DXCreateTextureFromFile(d3d_device,   //Direct3D Device
                              fileName,       //File Name
                             &texture);    //Texture handle
}*/