// CppFizzBuzz02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using namespace std;

string addStr(string acc, int value, int div, string divStr)
{
	if (value % div == 0) return acc + divStr;
	else return acc;
}

string doFB(int val)
{
	string acc = "";

	acc = addStr(acc, val, 3, "fizz");
	acc = addStr(acc, val, 5, "buzz");
	acc = addStr(acc, val, 7, "bang");
	acc = addStr(acc, val, 11, "boom");

	if (acc.length() == 0) acc = to_string(val);
	return acc;
}

int main()
{
	for (int ii = 0; ii < 100; ii++)
	{
		cout << doFB(ii) << endl;
	}

	return 0;
}
