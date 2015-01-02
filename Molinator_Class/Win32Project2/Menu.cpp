#include "Small_Scene_Render.h"

void menu()
{
	char Option = '0';
	//DWORD n;      //unreferenced local variable
			//SetStdHandle(STD_OUTPUT_HANDLE, GetStdHandle(STD_OUTPUT_HANDLE));
			//SetStdHandle(STD_INPUT_HANDLE, GetStdHandle(STD_INPUT_HANDLE));

	while (Option != '1') {
		if(Option == '0')
		{
			//Askii art help by http://www.network-science.de/ascii/ --> (Type: Banner 3)
			/*cout << "######## ########  ######  ########" << endl;
			cout << "   ##    ##       ##    ##    ##   " << endl;
			cout << "   ##    ##       ##          ##   " << endl;
			cout << "   ##    ######    ######     ##   " << endl;
			cout << "   ##    ##             ##    ##   " << endl;
			cout << "   ##    ##       ##    ##    ##   " << endl;
			cout << "   ##    ########  ######     ##   " << endl << endl;*/
			//Output did not work as intended **Update** a printed \ will not work, \\ will print \. **Update 2** changed style

			cout << "-----------------------------------------------------------" << endl;
			cout << "######## ##     ## ##    ##    ########  ##     ## ##    ##" << endl;
            cout << "##       ##     ## ###   ##    ##     ## ##     ## ###   ##" << endl;
            cout << "##       ##     ## ####  ##    ##     ## ##     ## ####  ##" << endl; 
            cout << "######   ##     ## ## ## ##    ########  ##     ## ## ## ##" << endl; 
            cout << "##       ##     ## ##  ####    ##   ##   ##     ## ##  ####" << endl; 
            cout << "##       ##     ## ##   ###    ##    ##  ##     ## ##   ###" << endl; 
            cout << "##        #######  ##    ##    ##     ##  #######  ##    ##" << endl;
			cout << "-----------------------------------------------------------" << endl << endl;

			cout << "             *********************************" << endl;
			cout << "             *      Welcome to Fun Run!      *" << endl;
			cout << "             *********************************" << endl;
			cout << "             * What would you like to do?    *" << endl;
			cout << "             * 1 --> Play                    *" << endl;
			cout << "             * 2 --> Instructions            *" << endl;
			cout << "             * 3 --> Quit                    *" << endl;
			cout << "             *********************************" << endl << endl;
			cout << "Selection:  ";
			//ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &Option, 1, &n, 0);
			cin >> Option;
		}

		else if(Option == '2')
		{
			cout << endl << endl;
			cout << "The object of this game is to traverse the obstacles as fast as you can!" << endl << endl;
			cout << "Orange cubes will bounce you high, red cubes will kill the character, and blue cubes are nonmoving platforms."<<endl<<" The goal is to touch the white cube." << endl;
			cout << "You control the player movement with the keyboard." << endl << endl;
			cout << "Forward / Back = W / S" << endl;
			cout << "Strafe Left / Right = A / D" << endl;
			cout << "Jump = Space Bar" << endl;
			cout << "Self-Kill (for when youre stuck) = ` " << endl << endl;
			//cout << "Fall = X" << endl;
			//cout << "Turn Left / Right = Q / E" << endl << endl;
			//cout << "Dig / Un-Dig = 1 / 2" << endl; //Was this removed?
			cout << "*** Camera Controls ***" << endl << endl;
			cout << "Zoom = + / -" << endl;
			cout << "Pan Sides = J / L" << endl;
			cout << "Pan Vertical = I / K" << endl << endl;
			cout << "Return to Main Menu ---> Enter 0" << endl; 
			
			//ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &Option, 1, &n, 0);
			cin >> Option;
		}

		else if(Option == '3')
		{
			
			cout << "Terminating Program: (Takes a few seconds do not hard close)" << endl;
			exit(Option == '3');
			system("pause");
		}

		else
		{
			cout << "You have entered an incorrect Value!" << endl;
			Option = '0';
			cin.clear();
		}


	}
}