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

template<class P1, class P2, class R>
std::function<R(P1)> operator>>(std::function<P2(P1)> f1, std::function<R(P2)> f2)
{
	return [f1, f2](P1 p) {
		return f2(f1(p));
	};
}

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

function<FBPair(int)>    toPair = [](int i) { return FBPair(i, ""); };
function<string(FBPair)> toStr = [](FBPair p) { return p.toString(); };
function<void(string)>   print = [](const string & s) { cout << s << endl; };

int main()
{
	function<string(int)> composed = (toPair >> do3 >> do5 >> do7 >> do11 >> toStr);

#ifdef WIN32
	auto s = view::ints(1)
#else
	auto s = view::ints(1, unreachable)
#endif
		| view::transform(composed)
		| view::take(100);
	ranges::for_each(s, print);

	return 0;
}

