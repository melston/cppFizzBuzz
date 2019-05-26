// CppFizzBuzz01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

int main()
{
	for (int ii = 0; ii < 100; ii++)
	{
		if (ii % 15 == 0)     cout << "fizzbuzz" << endl;
		else if (ii % 5 == 0) cout << "buzz" << endl;
		else if (ii % 3 == 0) cout << "fizz" << endl;
		else                  cout << ii << endl;
	}
}

