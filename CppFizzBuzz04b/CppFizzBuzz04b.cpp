// CppFizzBuzz04a.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

#ifdef WIN32
# include <range/v3/all.hpp>
#else
#include "range/v3/all.hpp"
#endif

using namespace std;
using namespace ranges;

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

auto print = [](const string & s) { cout << s << endl; };
auto toPair = [](int i) { return FBPair(i, ""); };
auto toStr = [](FBPair p) { return p.toString(); };

int main()
{
#ifdef WIN32
	auto s = view::ints(1)
#else
	auto s = view::ints(1, unreachable)
#endif
		| view::transform(toPair)
		| view::transform(do3)
		| view::transform(do5)
		| view::transform(do7)
		| view::transform(do11)
		| view::transform(toStr)
		| view::take(100);
	ranges::for_each(s, print);

	return 0;
}

