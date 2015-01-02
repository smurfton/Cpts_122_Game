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

	//seed our random number generator
	srand((unsigned) time(NULL));

	/*ifd = _open_osfhandle((intptr_t)input, _O_TEXT);
    ofd = _open_osfhandle((intptr_t)output, _O_TEXT);
    *stdin = *_fdopen(ifd, "r");
    *stdout = *_fdopen(ofd, "w");*/
	SetStdHandle(STD_INPUT_HANDLE, input);
	SetStdHandle(STD_OUTPUT_HANDLE, output);

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Fun Run",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

	ourGame.initialize(hWnd);
	
	//randomly generate a map
	generateMap(35 + rand()%6);

	//create the main message loop
	MSG msg;
	//start the timer
	clock_t initialTime = clock();
	while (ourGame.getVictory() == false) {

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
	//calculate time required
	clock_t totalTime, finalTime = clock();
	totalTime = finalTime - initialTime;

	//wait for user to exit
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	ifd = _open_osfhandle((intptr_t)input, _O_TEXT);
    ofd = _open_osfhandle((intptr_t)output, _O_TEXT);
    *stdin = *_fdopen(ifd, "r");
    *stdout = *_fdopen(ofd, "w");
	cout << "Number of Deaths: " << ourGame.getDeaths() << endl;
	cout << "Time to complete (seconds): " << totalTime/CLOCKS_PER_SEC << endl;
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
			/*case TURN_RIGHT:
				ourGame.characters.at(0).turnRight();
				break;
			case TURN_LEFT:
				ourGame.characters.at(0).turnLeft();
				break;*/
			case JUMP:
				if (ourGame.characters.at(0).getYVel() == 0) {
					ourGame.characters.at(0).jump();
				}
				break;
			case KILL:
				ourGame.addDeath();
				break;

			}
			break;

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}
void generateMap(int elements) {
	//add objects to the game

	ourGame.addCharacter("Person.FBX", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//ourGame.addObject("Cube.FBX", L"red.PNG");
	ourGame.addObject("Ground.FBX", D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	//ourGame.addObject("Cube2.FBX", D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	Position loc;
	loc.x = 0; 
	loc.y = 0; 
	loc.z = 0;
	Vector traverse;
	
	//randomly generate our map
	for (int i = 0; i < elements; i++) {
		//generation 25 - 30 platforms

		//update the platform position
		if (rand()%5 == 1) {
			loc.x -= rand()%4+2;
		} else {
			loc.x += rand()%4+2;
		}

		if (rand()%4 == 1) {
			loc.y -= rand()%4+2;
		} else {
			loc.y += rand()%4+2;
		}

		if (rand()%3 == 1) {
			loc.z -= rand()%4+2;
		} else {
			loc.z += rand()%4+2;
		}

		//update traverse
		traverse.x = (float) (rand()%4);
		traverse.y = (float) (rand()%3);
		traverse.z = (float) (rand()%5);


		if (rand()%4 == 1) {
			//generate a non moving platform at location
			ourGame.addObject("Cube.FBX", D3DXCOLOR(0,0,1,(FLOAT).51));
			ourGame.elements.at(ourGame.elements.size()-1).setLocation(loc.x, loc.y, loc.z);
			//generate an enemy around the nonmoving platform
			ourGame.addEnemy("Cube2.FBX", D3DXCOLOR(1,0,0,1), traverse);
			ourGame.enemies.at(ourGame.enemies.size()-1).setLocation(loc.x+ rand()%2, loc.y-rand()%2, loc.z);
		} else {
			//generate a moving platform
			ourGame.addObstacle("Cube.FBX", D3DXCOLOR(1,.5,0,.5), traverse);
			ourGame.obstacles.at(ourGame.obstacles.size()-1).setLocation(loc.x, loc.y, loc.z);
		}

	}

	//update the platform position
	if (rand()%5 == 1) {
		loc.x -= rand()%4;
	} else {
		loc.x += rand()%4;
	}

	if (rand()%4 == 1) {
		loc.y -= rand()%4;
	} else {
		loc.y += rand()%4;
	}

	if (rand()%3 == 1) {
		loc.z -= rand()%4;
	} else {
		loc.z += rand()%4;
	}

	ourGame.addObjective("Cube.FBX", D3DXCOLOR(1,1,1,1), traverse);
	ourGame.objectives.at(0).setLocation(loc.x, loc.y, loc.z);
}