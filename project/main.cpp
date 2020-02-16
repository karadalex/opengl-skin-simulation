#include <iostream>
#include <stdio.h>

#include "program1.h"
#include "program2.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "Skin Simulation project" << endl;
	cout << "by Alexios Karadimos, up1046820@upnet.gr, karadalex@gmail.com" << endl;
	cout << "Choose program: " << endl;
	cout << "1. Cloth (skin-like) and sphere" << endl;
	cout << "2. Skin stretching" << endl;

	int programChoice;
	cin >> programChoice;

	switch (programChoice)
	{
	case 1:
		program1::main();
	case 2:
		program2::main();
	default:
		break;
	}
}