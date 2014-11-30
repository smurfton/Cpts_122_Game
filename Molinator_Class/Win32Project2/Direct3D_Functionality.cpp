#include "Small_Scene_Render.h"

void initInterface(HWND window) {
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
	//initVertices(); //initialize the vertice points

	//Initialize models
	initializeEnvironment();
	initializeCharacter();

	//set render states
	d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE); //turn off 3D lighting (I don't know how to use it yet) in the device
	d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer

	//load textures
	
	D3DXCreateTextureFromFile(d3d_device, L"red.png", &texture); 
	D3DXCreateTextureFromFile(d3d_device, L"ground.png", &ground); 

}
void setNewTexture(IDirect3DTexture9 *texture) {   //Texture handle
	d3d_device->SetTexture(0, texture);
}


void setTextureGradient(vector<Vertex> &v1) {
	int size = v1.size();
	for (int i = 0; i < (int)sqrt(size); i++) {
		for (int j = 0; j < (int)sqrt(size); j++) {
			v1.at(i*sqrt(size) + j).tx = (float)j/(float)sqrt(size);
			v1.at(i*sqrt(size) + j).ty = (float)i/(float)sqrt(size);
		}
	}
	int dataCompleted =  ((int)sqrt(size) * (int)sqrt(size));

	//loop through residual information
	int remainder = size - dataCompleted;
	for (int i = 0; i < remainder; i++) {
		v1.at(dataCompleted+i).tx = 1/(i+1);
		v1.at(dataCompleted+i).ty = i/(3*(i+1));
	}
}

void cleanD3D() {
	if (environment_buffer != nullptr) environment_buffer->Release();
	//release all allocated devices in cleanup procedure
	if (texture != nullptr) texture->Release();
	if (ground != nullptr) ground->Release();
	if (character_buffer != nullptr) character_buffer->Release();
	if (virtex_buffer != nullptr) virtex_buffer->Release();
	if (d3d_device != nullptr) d3d_device->Release();
	if (d3d_interface != nullptr) d3d_interface->Release();
}

void render() {
	//clear the current scene
	d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0); //clear the buffer
	d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //clear the z buffer, too

	//begin the next scene
	d3d_device->BeginScene();

	//set our vertex format
	d3d_device->SetFVF(CUSTOMFVF);

	/*Begin transformation pipeline! Please move to separate functions to simplify*/
	updateCamera();
	
	setNewTexture(ground);
	//draw the environment
	d3d_device->SetStreamSource(0, environment_buffer, 0, sizeof(Vertex));
	d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, env_size);

	updateCharacter();
	setNewTexture(texture);
	//draw the character
	d3d_device->SetStreamSource(0, character_buffer, 0, sizeof(Vertex));
	d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, char_size);

	//end the scene
	d3d_device->EndScene();

	//present nothing (we are throwing away buffers after use)
	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void updateCamera() {
	D3DXMATRIX viewMatrix;
	D3DXMATRIX lensMatrix;

	D3DXMatrixLookAtLH(&viewMatrix,
					   &D3DXVECTOR3 (10, 5, 5),    // the camera position
					   &D3DXVECTOR3 (0, 0, 0),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

	d3d_device->SetTransform(D3DTS_VIEW, &viewMatrix); //set the camera view point

	D3DXMatrixPerspectiveFovLH(&lensMatrix, //Set the camera lens
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane
	d3d_device->SetTransform(D3DTS_PROJECTION, &lensMatrix); //set the camera lens 
}