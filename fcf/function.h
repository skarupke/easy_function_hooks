#pragma once

#include "meta.h"
#include "os.h"
#include "lambda.h"

namespace fcf
{
template<typename T>
struct FunctionBase
{
	explicit FunctionBase(T func, void * ptr)
		: func(func), ptr(ptr), global_lambda(nullptr)
	{
		copy_jump_bytes(start_bytes, ptr);
		makeUnsafe(ptr, sizeof(start_bytes));
		for (auto & stored : getUniqueStorers<T>())
		{
			if (stored.first != func) continue;
			global_lambda = stored.second;
			stored_lambda = *global_lambda;
			break;
		}
	}

	inline T get() const
	{
		return func;
	}

	template<T lhs>
	void restoreState() const
	{
		// this assert is here because you can only assign a function to a fcf::Function that
		// has information about itself. so only this is valid:
		// fcf::Function old = foo;
		// fcf::assign(foo) = bar;
		// fcf::assign(foo) = old;
		FCF_ASSERT(lhs == func);
		copy_jump_bytes(ptr, start_bytes);
		if (global_lambda) *global_lambda = stored_lambda;
	}

private:
	unsigned char start_bytes[JMP_BYTE_COUNT];
	void * ptr;
	typename meta::func_type<T>::type stored_lambda;
	typename meta::func_type<T>::type * global_lambda;
	
protected:
	T func;
	inline void prepare() const
	{
		copy_jump_bytes(ptr, start_bytes);
		notifyCodeChange(ptr, sizeof(start_bytes));
	}

	struct Copier
	{
		Copier(const FunctionBase & parent)
			: parent(parent)
		{
			copy_jump_bytes(copied_bytes, parent.ptr);
			if (parent.global_lambda)
			{
				copied_lambda = std::move(*parent.global_lambda);
				if (parent.stored_lambda) *parent.global_lambda = parent.stored_lambda;
			}
		}
		~Copier()
		{
			if (parent.global_lambda)
			{
				*parent.global_lambda = std::move(copied_lambda);
			}
			copy_jump_bytes(parent.ptr, copied_bytes);
			notifyCodeChange(parent.ptr, sizeof(copied_bytes));
		}
	private:
		const FunctionBase & parent;
		typename meta::func_type<T>::type copied_lambda;
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
	explicit Function(Result (*func)(Args...))
		: FunctionBase(func, func)
	{
	}
	Function(const Function & other)
		: FunctionBase(other)
	{
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
	: public FunctionBase<Result (Class::*)(Args...)>
{
public:
	explicit Function(Result (Class::*func)(Args...))
		: FunctionBase(func, getMemberFunctionAddress(func))
	{
	}
	Function(const Function & other)
		: FunctionBase(other), func(other.func)
	{
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
	: public FunctionBase<Result (Class::*)(Args...) const>
{
public:
	explicit Function(Result (Class::*func)(Args...) const)
		: FunctionBase(func, getMemberFunctionAddress(func))
	{
	}
	Function(const Function & other)
		: FunctionBase(other), func(other.func)
	{
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
	: public FunctionBase<Result (Class::*)(Args...)>
{
public:
	VirtualFunction(Class & object, Result (Class::*func)(Args...))
		: FunctionBase(func, getVirtualFunctionAddress(&object, getVTableIndex<Result (Class::*)(Args...)>(func)))
	{
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
	: public FunctionBase<Result (Class::*)(Args...)>
{
public:
	VirtualFunction(Class & object, Result (Class::*func)(Args...) const)
		: FunctionBase(func, getVirtualFunctionAddress(&object, getVTableIndex<Result (Class::*)(Args...) const>(func)))
	{
	}

	Result operator()(const Class & object, Args... args) const
	{
		Copier copier(*this);
		prepare();
		return (object.*func)(std::forward<Args>(args)...);
	}
};

}

