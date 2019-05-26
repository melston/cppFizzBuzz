// CppFizzBuzz04a.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include "range/v3/all.hpp"
//#include "range/v3/view/all.hpp"

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

auto print  = [](const string& s) { cout << s << endl; };
auto toPair = [] (int i) { return FBPair(i, ""); };
auto x3  = [] (FBPair p) { return do3(p); };
auto x5  = [] (FBPair p) { return do5(p); };
auto x7  = [] (FBPair p) { return do7(p); };
auto x11 = [] (FBPair p) { return do11(p); };
auto toStr = [] (FBPair p) { return p.toString(); };

int main()
{
  auto s = view::ints(1, unreachable)
    | view::transform(toPair)
    | view::transform(x3)
    | view::transform(x5)
    | view::transform(x7)
    | view::transform(x11)
    | view::transform(toStr)
    | view::take(100);
  ranges::for_each(s, print);

  return 0;
}

