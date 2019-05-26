// CppFizzBuzz04a.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Stream.h"

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

Stream<int> intsFrom(int n)
{
	return Stream<int>([n]() {
			return Cell<int>(n, intsFrom(n + 1));
	});
}

int main()
{
	auto r = intsFrom(1);

	auto p1 = fmap(r, [](int i) { return FBPair(i, ""); });
}
