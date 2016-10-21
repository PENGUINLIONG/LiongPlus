#include <iostream>
#include <tuple>
#include "Include/StaticInterface/StaticInterface.hpp"

using namespace std;
using namespace LiongPlus::StaticInterface;

struct my_struct
{
	void do_something() { return; }
	double do_sth_else(int a, float b) { return a * b; }
};

struct doer
{
	dm<doer, void> do_something;
	dm<doer, double, int, float> do_sth_else;
};

int main()
{
	my_struct st;

	doer it = make_interface<doer>(st, &my_struct::do_something, &my_struct::do_sth_else);
	
	it.do_something();
	cout << it.do_sth_else(1, (float)2.1);

	int i;
	cin >> i;
}