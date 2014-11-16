#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

void main(void)
{
	int key;

	cout << "Press any key to see its code" << endl;

	while (true)
	{
		key = getch();
		
		if (key != 224)
			cout << key << " - " << char(key) << endl;
	}
}