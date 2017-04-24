#pragma once

#include <iostream>
#include <stdio.h>
#include <Windows.h>

using namespace std;

class Intro 
{
public:
	void Play()
	{
		system("Color 04"); //Change sys color
		for (int i = 0; i < 50; i++) printf("\n"); // jump to bottom of console
		cout << "                                               _______  _____  ______  _______ " << endl;
		cout << "                                              |_______||____ ||      ||    |  |" << endl;
		cout << "                                                |   |  |     <|  --  ||       |" << endl;
		cout << "                                                |___|  |__|__||______||__|____|\n\n" << endl;
		cout << "                                                       T H E  C L O N E  \n" << endl;
		int j = 300;
		int b = 500;
		for (int i = 0; i < 50; i++) 
		{
			Sleep(j); // move faster and faster,
			j = (int)(j * 0.9); // so sleep less each time
			printf("\n"); // move Logo a line upward
			Beep(b, j);
			b = (int)(b * 1.1);
			if (i == 9)
			{
				system("Color 0B"); //Change sys color
				Sleep(1500);
			}
		}

		system("CLS");
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n";
		cout << "                                                         Developed By          " << endl;
		cout << "                                                ------------------------------ " << endl;
		cout << "                                                       Brendan Nelligan" << endl;
		cout << "                                                           Ryan Levy" << endl;
		cout << "                                                        and Mike Sarnie" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n";
		Sleep(2000);
		system("CLS");
	}
	void Death()
	{
		char block = 219;
		system("Color 04"); //Change sys color

		for(int i = 0; i < 4445; i++)
			cout << block;
		
		Sleep(10000);
		system("CLS");

		cout << "                                           _______  _____  ______  _______ " << endl;
		system("Color 02"); //Change sys color
		Sleep(100);
		system("CLS");

		cout << "                                           _______  _____  ______  _______ " << endl;
		system("Color 01"); //Change sys color
		Sleep(100);
		system("CLS");

		cout << "                                           _______  _____  ______  _______ " << endl;
		system("Color 00"); //Change sys color
		system("CLS");
	}
};