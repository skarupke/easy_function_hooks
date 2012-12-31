// function hooks.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#include "fcf.h"
#include <iostream>

bool calledUpdate = false;

__declspec(noinline) void update()
{
	calledUpdate = true;
}

auto oldUpdate = fcf::function(update);

bool calledNewUpdate = false;

__declspec(noinline) void newUpdate()
{
	calledNewUpdate = true;
	oldUpdate();
}

void test1()
{
	// see if the function does what I expect
	FCF_ASSERT(!calledUpdate);
	update();
	FCF_ASSERT(calledUpdate);
	calledUpdate = false;

	// test simple assignment
	FCF_ASSIGN(&update, &newUpdate);
	FCF_ASSERT(!calledUpdate && !calledNewUpdate);
	update();
	FCF_ASSERT(calledUpdate && calledNewUpdate);
	calledUpdate = calledNewUpdate = false;


	// test lambda assignment
	bool calledLambdaUpdate = false;
	std::function<void ()> lambdaUpdate = [&calledLambdaUpdate]()
	{
		calledLambdaUpdate = true;
	};
	FCF_ASSIGN(&update, lambdaUpdate);
	FCF_ASSERT(!calledUpdate && !calledNewUpdate && !calledLambdaUpdate);
	update();
	FCF_ASSERT(!calledUpdate && !calledNewUpdate && calledLambdaUpdate);
	calledLambdaUpdate = false;

	// test assigning back
	FCF_ASSIGN(&update, oldUpdate);
	FCF_ASSERT(!calledUpdate && !calledNewUpdate && !calledLambdaUpdate);
	update();
	FCF_ASSERT(calledUpdate && !calledNewUpdate && !calledLambdaUpdate);
}

__declspec(noinline) int addOne(int a)
{
	return a + 1;
}

void test2()
{
	auto oldAddOne = fcf::function(&addOne);

	// test lambda assignment
	FCF_ASSERT(addOne(1) == 2);
	FCF_ASSIGN(&addOne, [](int a) -> int
	{
		return a + 2;
	});
	FCF_ASSERT(addOne(1) == 3);
	FCF_ASSIGN(&addOne, [&oldAddOne](int a) -> int
	{
		return oldAddOne(a) + 3;
	});
	FCF_ASSERT(addOne(1) == 5);
}

struct Bar
{
	Bar(int i) : i(i) {}

	__declspec(noinline) int foo()
	{
		return i;
	}
	__declspec(noinline) virtual int blub(int j)
	{
		return i + j;
	}

	int i;
};

struct ReroutedBar
{
	int foo()
	{
		Bar * barthis = reinterpret_cast<Bar *>(this);
		return barthis->i - 1;
	}
	int blubblub(int j)
	{
		Bar * barthis = reinterpret_cast<Bar *>(this);
		return barthis->i - j;
	}
};

void test3()
{
	// check that fcf::function works for member functions
	auto oldFoo = fcf::function(&Bar::foo);
	Bar a(10);
	FCF_ASSERT(oldFoo(a) == 10);

	// check that rerouting works for member functions
	FCF_ASSERT(a.foo() == 10);
	FCF_ASSIGN(&Bar::foo, &ReroutedBar::foo);
	FCF_ASSERT(a.foo() == 9);

	// check that rerouting to lambda works for member functions
	FCF_ASSIGN(&Bar::foo, [](Bar & bar) -> int
	{
		return bar.i * 2;
	});
	FCF_ASSERT(a.foo() == 20);

	// check that setting it back works
	FCF_ASSIGN(&Bar::foo, oldFoo);
	FCF_ASSERT(a.foo() == 10);
}

struct Derived : public Bar
{
	Derived() : Bar(1000) {}
	__declspec(noinline) virtual int blub(int j) override
	{
		return i + 2 * j;
	}
	__declspec(noinline) int blubblub(int j)
	{
		return i + 3 * j;
	}
};

void test4()
{
	// check that capturing works
	Derived d;
	auto oldBlub = fcf::virtual_function(d, &Derived::blub);
	FCF_ASSERT(oldBlub(d, 10) == 1020);

	// check that rerouting works
	FCF_VIRTUAL_ASSIGN(d, &Bar::blub, &Derived::blubblub);
	FCF_ASSERT(oldBlub(d, 10) == 1020);
	FCF_ASSERT(d.blub(10) == 1030);
	Bar & b = d; static_cast<void>(b);
	FCF_ASSERT(b.blub(10) == 1030);

	// check that rerouting works for unconnected type
	FCF_VIRTUAL_ASSIGN(d, &Bar::blub, &ReroutedBar::blubblub);
	FCF_ASSERT(d.blub(10) == 990);

	// check that rerouting works for lambda functions
	FCF_VIRTUAL_ASSIGN(d, &Derived::blub, [](Derived & d, int j) -> int
	{
		return d.i / j;
	});
	FCF_ASSERT(d.blub(10) == 100);

	// check that setting back works
	FCF_VIRTUAL_ASSIGN(d, &Derived::blub, oldBlub);
	FCF_ASSERT(d.blub(10) == 1020);
}

struct Foo
{
	Foo(int i) : i(i) {}
	int plusOne() const { return i + 1; }
	int plusTwo() const { return i + 2; }
	int plusThree() const { return i + 3; }
	int i;
};

void test5()
{
	Foo foo(4);
	FCF_ASSERT(foo.plusOne() == 5);

	FCF_ASSIGN(&Foo::plusOne, &Foo::plusTwo);
	FCF_ASSERT(foo.plusOne() == 6);

	// check that rerouting back to a rerouted function works
	auto oldPlusOne = fcf::function(&Foo::plusOne);
	FCF_ASSIGN(&Foo::plusOne, &Foo::plusThree);
	FCF_ASSERT(foo.plusOne() == 7);
	FCF_ASSERT(oldPlusOne(foo) == 6);

	FCF_ASSIGN(&Foo::plusOne, oldPlusOne);
	FCF_ASSERT(foo.plusOne() == 6);
}

#include "fcf/symbol_lookup.h"

struct FakeFoo
{
	FakeFoo::FakeFoo(int i)
	{
		std::cout << "FakeFoo " << i << std::endl;
	}
};

void main()
{
	test1();
	test2();
	test3();
	test4();
	test5();

//	void * blub = FCF_LOOKUP_SYMBOL(Foo::Foo);
//	void * toreturn = nullptr;
//	{
//		_asm mov eax, Foo::Foo
//		_asm mov toreturn, eax
//	}
	//std::cout << reinterpret_cast<size_t>(toreturn) << std::endl;
	//fcf::place_jmp(FCF_LOOKUP_SYMBOL(Foo::Foo) FCF_LOOKUP_SYMBOL(FakeFoo::FakeFoo));


	std::cout << "\nPress any key to quit." << std::endl;
	std::cin.get();
}

