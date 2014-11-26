#include "Small_Scene_Render.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    SecureZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

	initInterface(hWnd); //initialize the d3d device


	//create the main message loop
	MSG msg;

	while (1) {

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // peak at top message until queue is empty
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT) //if the message was to exit, we exit
            break;

		//else we now can generate our code for directX
		/*Begin Scene Render Code*/
		render();
	}

	//we have exitted. Clean up after ourselves
	cleanD3D();

	//end the program
	return msg.message;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

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
	initVertices(); //initialize the vertice points
	d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE); //turn off 3D lighting (I don't know how to use it yet) in the device
	d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer

}

void cleanD3D() {
	//release all allocated devices in cleanup procedure
	ground_buffer->Release();
	virtex_buffer->Release();
	d3d_device->Release();
	d3d_interface->Release();
}

void render() {
	//clear the current scene
	d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //clear the buffer
	d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //clear the z buffer, too

	//begin the next scene
	d3d_device->BeginScene();

	//set our vertex format
	d3d_device->SetFVF(CUSTOMFVF);

	/*Begin transformation pipeline! Please move to separate functions to simplify*/
	initView();
	
	
	drawGround();
	drawPyramid();

	//end the scene
	d3d_device->EndScene();

	//present nothing (we are throwing away buffers after use)
	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void initVertices() {
	Vertex vertices[] = {
		//vertices of the pyramid
   { 0.0f, 0.0f, 0.0f,0x0000FF00},{ 1.0f, 2.0f, 0.0f,0x00FF0000},{ 2.0f, 0.0f, 0.0f,0x000000FF},
   { 2.0f, 0.0f, 0.0f,0x000000FF},{ 1.0f, 2.0f, 0.0f,0x00FF0000},{ 1.0f, 0.0f, 2.0f,0x0000FF00},
   { 1.0f, 0.0f, 2.0f,0x0000FF00},{ 1.0f, 2.0f, 0.0f,0x00FF0000},{ 0.0f, 0.0f, 0.0f,0x000000FF},
   { 0.0f, 0.0f, 0.0f,0x000000FF},{ 2.0f, 0.0f, 0.0f,0x00FF0000},{ 1.0f, 0.0f, 2.0f,0x0000FF00},
	};
	
	d3d_device->CreateVertexBuffer(MAX_TRIANGLES*3*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &virtex_buffer, NULL); //create the buffer

	void *pVoid; //used for referencing memory copy location

	virtex_buffer->Lock(0, 0, (void**)&pVoid, 0); //lock the memory location where we are copying vertex information
	memcpy(pVoid, vertices, sizeof(vertices)); //copy the memory into the VRAM buffer
	virtex_buffer->Unlock(); //unlock the memory now that it has been successfully copied
}

void drawPyramid() {
	D3DXMATRIX rotMatrix;
	D3DXMATRIX transMatrix;
	D3DXMATRIX worldMatrix;
	static float rotAngle = 0.0f;

	D3DXMatrixRotationY(&rotMatrix, D3DXToRadian(rotAngle)); //rotate the object at a constant angle
	D3DXMatrixTranslation(&transMatrix, 2, 0, 0); //translate the object 2 units along the x axis
	D3DXMatrixMultiply(&worldMatrix, &rotMatrix, &transMatrix);
	d3d_device->SetTransform(D3DTS_WORLD, &worldMatrix); //set the total transformation matrix

	
	d3d_device->SetStreamSource(0, virtex_buffer, 0, sizeof(Vertex)); //set the virtex stream source
	//d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
	rotAngle += .7;
}

void initView() {
	D3DXMATRIX viewMatrix;
	D3DXMATRIX lensMatrix;

	D3DXMatrixLookAtLH(&viewMatrix,
					   &D3DXVECTOR3 (1.0f, -1.0f, 10.0f),    // the camera position
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

	d3d_device->SetTransform(D3DTS_VIEW, &viewMatrix); //set the camera view point

	D3DXMatrixPerspectiveFovLH(&lensMatrix, //Set the camera lens
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane
	d3d_device->SetTransform(D3DTS_PROJECTION, &lensMatrix); //set the camera lens 

}
void initGround() {
	Vertex vertices[] = {
		{0,0,0, 0x00FFFFFF},{0,0,-50, 0x00FFFFFF},{20,0,0, 0x00FFFFFF},
		{20,0,0, 0x00FFFFFF},{20,0,-50, 0x00FFFFFF},{0,0,-50, 0x00FFFFFF},
	};
	//creat the buffer for the ground
	d3d_device->CreateVertexBuffer(MAX_TRIANGLES*3*sizeof(Vertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &ground_buffer, NULL);
	void *pVoid;
	ground_buffer->Lock(0, 0, (void**) &pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	ground_buffer->Unlock();

}
void drawGround() {
	d3d_device->SetStreamSource(1, ground_buffer, 0,sizeof(Vertex));
	d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
}