// CppFizzBuzz03b.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <functional>

using namespace std;

class FBPair
{
public:
	int val;
	string acc;

	FBPair(int v, string a) : val(v), acc(a) {}
	string toString()
	{
		if (acc.length() == 0) return to_string(val);
		return acc;
	}
};

std::function<FBPair(FBPair)> genFB(int div, string str)
{
	return [div, str](FBPair p) {
		if (p.val % div != 0) return p;
		return FBPair(p.val, p.acc + str);
	};
}

auto do3 = genFB(3, "fizz");
auto do5 = genFB(5, "buzz");
auto do7 = genFB(7, "bang");
auto do11 = genFB(11, "boom");

string doFB(int val)
{

	FBPair p(val, "");
	p = do3(p);
	p = do5(p);
	p = do7(p);
	p = do11(p);

	return p.toString();
}

int main()
{
	for (int ii = 0; ii < 100; ii++)
	{
		cout << doFB(ii) << endl;
	}

	return 0;
}