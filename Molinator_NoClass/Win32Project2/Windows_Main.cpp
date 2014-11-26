#include "Small_Scene_Render.h"

LPDIRECT3D9 d3d_interface; 
LPDIRECT3DDEVICE9 d3d_device; 
D3DPRESENT_PARAMETERS d3d_interface_parameters;
LPDIRECT3DVERTEXBUFFER9 virtex_buffer; 
LPDIRECT3DVERTEXBUFFER9 character_buffer;
LPDIRECT3DVERTEXBUFFER9 environment_buffer;
IDirect3DTexture9 *texture;
IDirect3DTexture9 *ground;
FbxManager *manager = NULL;
Position cameraViewVector = {-INI_X, -INI_Y, -INI_Z};
Position cameraPosition = {INI_X, INI_Y, INI_Z};
float cameraAngle = 0;
vector<Vertex> player;
Transformation playerOrientation = {{0,1.38/2,0}, {1,0,0}, 0, .1};
UINT env_size = 0, char_size = 0;


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
			//handle any keyboard input
		case WM_CHAR:
			switch (wParam) {
			case UP:
				moveForward();
				break;
			case LEFT:
				moveLeft();
				break;
			case DOWN:
				moveBack();
				break;
			case RIGHT:
				moveRight();
				break;
			case JUMP:
				moveUp();
				break;
			case FALL:
				fall();
				break;
			case TURN_RIGHT:
				turnRight();
				break;
			case TURN_LEFT:
				turnLeft();
				break;
			}
			break;

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}