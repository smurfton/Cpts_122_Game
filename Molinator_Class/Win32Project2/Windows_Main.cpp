#include "Small_Scene_Render.h"

Game ourGame;

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
	

	HANDLE input, output;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	input = GetStdHandle(STD_INPUT_HANDLE);
	output = GetStdHandle(STD_OUTPUT_HANDLE);
	int ifd = _open_osfhandle((intptr_t)input, _O_TEXT);
    int ofd = _open_osfhandle((intptr_t)output, _O_TEXT);
    *stdin = *_fdopen(ifd, "r");
    *stdout = *_fdopen(ofd, "w");
	menu();
	FreeConsole();



	/*ifd = _open_osfhandle((intptr_t)input, _O_TEXT);
    ofd = _open_osfhandle((intptr_t)output, _O_TEXT);
    *stdin = *_fdopen(ifd, "r");
    *stdout = *_fdopen(ofd, "w");*/
	SetStdHandle(STD_INPUT_HANDLE, input);
	SetStdHandle(STD_OUTPUT_HANDLE, output);

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"The Game",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

	ourGame.initialize(hWnd);
	//add objects to the game

	ourGame.addCharacter("Person.FBX", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//ourGame.addObject("Cube.FBX", L"red.PNG");
	ourGame.addObject("Ground.FBX", D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	//ourGame.addObject("Cube2.FBX", D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	Vector temp;
	temp.x = 0;
	temp.y = 0;
	temp.z = 5;
	for (int i = 0; i < 10; i++) {
		ourGame.addObstacle("Cube.FBX", D3DXCOLOR(0,.66,0,.5), temp);
		ourGame.obstacles.at(i).setLocation(6*i, 3*i, 5*i);
	}

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
		ourGame.render();
	}
	//output the stats of the game

	//wait for user to exit
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	ifd = _open_osfhandle((intptr_t)input, _O_TEXT);
    ofd = _open_osfhandle((intptr_t)output, _O_TEXT);
    *stdin = *_fdopen(ifd, "r");
    *stdout = *_fdopen(ofd, "w");
	cout << "Number of Deaths: " << ourGame.getDeaths();
	cout <<" Enter any value to exit.";
	int trash;
	cin >> trash;
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
			case ZOOMIN:
				ourGame.cam.zoom(1);
				break;
			case ZOOMOUT:
				ourGame.cam.zoom(-1);
				break;
			case PANLEFT:
				ourGame.cam.rotateHorizontal(-1);
				break;
			case PANRIGHT:
				ourGame.cam.rotateHorizontal(1);
				break;
			case PANUP:
				ourGame.cam.rotateVertical(1);
				break;
			case PANDOWN:
				ourGame.cam.rotateVertical(-1);
				break;
				//character movement
			case UP:
				ourGame.characters.at(0).moveForward();
				break;
			case DOWN:
				ourGame.characters.at(0).moveBackward();
				break;
			case RIGHT:
				ourGame.characters.at(0).moveRight();
				break;
			case LEFT:
				ourGame.characters.at(0).moveLeft();
				break;
			case TURN_RIGHT:
				ourGame.characters.at(0).turnRight();
				break;
			case TURN_LEFT:
				ourGame.characters.at(0).turnLeft();
				break;
			case JUMP:
				if (ourGame.characters.at(0).getYVel() == 0) {
					ourGame.characters.at(0).jump();
				}
				break;
			case FALL:
				ourGame.characters.at(0).fall();
				break;

			}
			break;

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}