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

	//initInterface(hWnd); //initialize the d3d device
	Game ourGame(hWnd);
	//add objects to the game
	Transformation transform;
	transform.location.x = 0;
	transform.location.y = 0;
	transform.location.z = 0;
	transform.normalVector.x = 0;
	transform.normalVector.y = 0;
	transform.normalVector.z = 0;
	transform.rotation = 0;
	transform.scale = .1;

	ourGame.addObject(transform, "Person.FBX", L"red.png");
	transform.scale = 10;
	transform.location.y = -1;
	ourGame.addObject(transform, "Ground.FBX", L"ground.png");

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
		//render();
		ourGame.render();
	}

	//we have exitted. Clean up after ourselves
	//cleanD3D();

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
		/*case WM_CHAR:
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
			case DIG:
				dig();
				break;
			case UNDIG:
				undig();
				break;
			}
			break;*/

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}