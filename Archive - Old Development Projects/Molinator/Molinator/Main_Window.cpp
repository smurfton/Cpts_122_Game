#include "Molinator.h"


LPDIRECT3DDEVICE9 d3d_device;
/*Position character_Position;
FbxManager *manager;
LPDIRECT3D9 d3d_interface;
D3DPRESENT_PARAMETERS d3d_interface_parameters;
LPDIRECT3DVERTEXBUFFER9 environment_Buffer;
LPDIRECT3DVERTEXBUFFER9 character_Buffer;
LPDIRECT3DVERTEXBUFFER9 enemy_Buffer;
IDirect3DTexture9 *char_texture;
IDirect3DTexture9 *ground_texture;
IDirect3DTexture9 *enemy_texture;
IDirect3DTexture9 *environment_texture;
IDirect3DSurface9 *mainMenu;*/

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
				   int nCmdShow) {
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

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Molinator Alpha-0.0.1",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

	Game theGame(hWnd);
	Object main;
	//main.setTextureFile(L"Main.png");
	main.setModelFile(L"Ground.FBX");
	main.createBuffer(d3d_device);
	main.loadBuffer(d3d_device);
	d3d_device->SetStreamSource(0, main.getBuffer(), 0, sizeof(Vertex));

	//create the main message loop
	MSG msg;

	bool keepGoing = true;
	while (keepGoing) {

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // peak at top message until queue is empty
        {
			if(msg.message == WM_QUIT) //if the message was to exit, we exit
				keepGoing = false;
				
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		} else {
			//else we now can generate our code for directX
			theGame.render();
		}
	}

	//end the program
	return msg.message;

}

/*Main message loop management function*/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		//message is a quit message
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	 }
	return DefWindowProc (hWnd, message, wParam, lParam);
}