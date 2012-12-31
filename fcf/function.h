#pragma once

#include "meta.h"
#include "os.h"
#include <cstdlib>

namespace fcf
{
template<typename T>
struct FunctionBase
{
	FunctionBase(T func)
		: func(func)
	{
		memcpy(start_bytes, func, sizeof(start_bytes));
		makeUnsafe(func, sizeof(start_bytes));
	}
	FunctionBase(const FunctionBase & other)
		: func(other.func)
	{
		memcpy(start_bytes, other.start_bytes, sizeof(start_bytes));
	}

	FunctionBase & operator=(void * other)
	{
		func = other;
		memcpy(start_bytes, other, sizeof(start_bytes));
		makeUnsafe(other, sizeof(start_bytes));
		return *this;
	}
	FunctionBase & operator=(const FunctionBase & other)
	{
		func = other.func;
		memcpy(start_bytes, other.start_bytes, sizeof(start_bytes));
		return *this;
	}

	inline const unsigned char * get_start_bytes() const { return start_bytes; }

private:
	unsigned char start_bytes[JMP_BYTE_COUNT];

protected:
	T func;
	inline void prepare() const
	{
		memcpy(func, start_bytes, sizeof(start_bytes));
		notifyCodeChange(func, sizeof(start_bytes));
	}

	struct Copier
	{
		Copier(const FunctionBase & parent)
			: func(parent.func)
		{
			memcpy(copied_bytes, func, sizeof(copied_bytes));
		}
		~Copier()
		{
			memcpy(func, copied_bytes, sizeof(copied_bytes));
			notifyCodeChange(func, sizeof(copied_bytes));
		}
	private:
		void * func;
		unsigned char copied_bytes[JMP_BYTE_COUNT];

		// intentionally left unimplemented
		Copier(const Copier &);
		Copier & operator=(const Copier &);
	};
};

template<typename>
class Function;
template<typename>
class VirtualFunction;

template<typename Result, typename... Args>
class Function<Result (*)(Args...)>
	: public FunctionBase<Result (*)(Args...)>
{
public:
	Function(Result (*func)(Args...))
		: FunctionBase(func)
	{
	}
	Function(const Function & other)
		: FunctionBase(other)
	{
	}
	Function & operator=(Result (*other)(Args...))
	{
		FunctionBase::operator=(other);
		return *this;
	}
	Function & operator=(const Function & other)
	{
		FunctionBase::operator=(other);
		return *this;
	}
	inline Result (*get() const)(Args...)
	{
		return func;
	}

	Result operator()(Args... args) const
	{
		Copier copier(*this);
		prepare();
		return func(std::forward<Args>(args)...);
	}
};
template<typename Result, typename Class, typename... Args>
class Function<Result (Class::*)(Args...)>
	: public FunctionBase<void *>
{
	Result (Class::*func)(Args...);
public:
	Function(Result (Class::*func)(Args...))
		: FunctionBase(getMemberFunctionAddress(func)), func(func)
	{
	}
	Function(const Function & other)
		: FunctionBase(other), func(other.func)
	{
	}
	Function & operator=(Result (Class::*other)(Args...))
	{
		FunctionBase::operator=(getMemberFunctionAddress(other));
		func = other;
		return *this;
	}
	Function & operator=(const Function & other)
	{
		FunctionBase::operator=(other);
		func = other.func;
		return *this;
	}
	inline Result (Class::*get() const)(Args...)
	{
		return func;
	}

	Result operator()(Class & object, Args... args) const
	{
		Copier copier(*this);
		prepare();
		return (object.*func)(std::forward<Args>(args)...);
	}
};
template<typename Result, typename Class, typename... Args>
class Function<Result (Class::*)(Args...) const>
	: public FunctionBase<void *>
{
	Result (Class::*func)(Args...) const;
public:
	Function(Result (Class::*func)(Args...) const)
		: FunctionBase(getMemberFunctionAddress(func)), func(func)
	{
	}
	Function(const Function & other)
		: FunctionBase(other), func(other.func)
	{
	}
	Function & operator=(Result (Class::*other)(Args...) const)
	{
		FunctionBase::operator=(getMemberFunctionAddress(other));
		func = other;
		return *this;
	}
	Function & operator=(const Function & other)
	{
		FunctionBase::operator=(other);
		func = other.func;
		return *this;
	}
	inline Result (Class::*get() const)(Args...) const
	{
		return func;
	}

	Result operator()(const Class & object, Args... args) const
	{
		Copier copier(*this);
		prepare();
		return (object.*func)(std::forward<Args>(args)...);
	}
};
template<typename Result, typename Class, typename... Args>
class VirtualFunction<Result (Class::*)(Args...)>
	: public FunctionBase<void *>
{
	Result (Class::*func)(Args...);
public:
	VirtualFunction(Class & object, Result (Class::*func)(Args...))
		: FunctionBase(getVirtualFunctionAddress(&object, getVTableIndex<Result (Class::*)(Args...)>(func))), func(func)
	{
	}
	VirtualFunction(const VirtualFunction & other)
		: FunctionBase(other), func(other.func)
	{
	}
	VirtualFunction & operator=(const VirtualFunction & other)
	{
		FunctionBase::operator=(other);
		func = other.func;
		return *this;
	}
	inline Result (Class::*get() const)(Args...)
	{
		return func;
	}

	Result operator()(Class & object, Args... args) const
	{
		Copier copier(*this);
		prepare();
		return (object.*func)(std::forward<Args>(args)...);
	}
};
template<typename Result, typename Class, typename... Args>
class VirtualFunction<Result (Class::*)(Args...) const>
	: public FunctionBase<void *>
{
	Result (Class::*func)(Args...) const;
public:
	VirtualFunction(Class & object, Result (Class::*func)(Args...) const)
		: FunctionBase(getVirtualFunctionAddress(&object, getVTableIndex<Result (Class::*)(Args...) const>(func))), func(func)
	{
	}
	VirtualFunction(const VirtualFunction & other)
		: FunctionBase(other), func(other.func)
	{
	}
	VirtualFunction & operator=(const VirtualFunction & other)
	{
		FunctionBase::operator=(other);
		func = other.func;
		return *this;
	}
	inline Result (Class::*get() const)(Args...) const
	{
		return func;
	}

	Result operator()(const Class & object, Args... args) const
	{
		Copier copier(*this);
		prepare();
		return (object.*func)(std::forward<Args>(args)...);
	}
};

}

