#include <iostream>


void __stdcall foo(int i)
{
	std::cout << "foo " << i << std::endl;
}

int main(int /*argc*/, char * /*argv*/[])
{
	foo(1);
	return 0;
}

