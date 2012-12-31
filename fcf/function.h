#pragma once

#include "meta.h"
#include "os.h"
#include <cstdlib>

namespace fcf
{
struct FunctionBase
{
	FunctionBase(void * func)
	{
		memcpy(start_bytes, func, sizeof(start_bytes));
		makeUnsafe(func, sizeof(start_bytes));
	}
	FunctionBase(const FunctionBase & other)
	{
		memcpy(start_bytes, other.start_bytes, sizeof(start_bytes));
	}

	FunctionBase & operator=(void * other)
	{
		memcpy(start_bytes, other, sizeof(start_bytes));
		makeUnsafe(other, sizeof(start_bytes));
		return *this;
	}
	FunctionBase & operator=(const FunctionBase & other)
	{
		memcpy(start_bytes, other.start_bytes, sizeof(start_bytes));
		return *this;
	}

	inline const unsigned char * get_start_bytes() const { return start_bytes; }

private:
	unsigned char start_bytes[JMP_BYTE_COUNT];

protected:
	inline void prepare(void * func) const
	{
		memcpy(func, start_bytes, sizeof(start_bytes));
		notifyCodeChange(func, sizeof(start_bytes));
	}

	struct Copier
	{
		Copier(void * func)
			: func(func)
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

#define FCF_BEFORE_CALL Copier copier(func); prepare(func)

#define FCF_FUNCTION_BOILERPLATE\
	T func;\
public:\
	Function(T func)\
		: FunctionBase(func), func(func)\
	{\
	}\
	Function(const Function & other)\
		: FunctionBase(other), func(other.func)\
	{\
	}\
	Function & operator=(T other)\
	{\
		FunctionBase::operator=(other);\
		func = other;\
		return *this;\
	}\
	Function & operator=(const Function & other)\
	{\
		FunctionBase::operator=(other);\
		func = other.func;\
		return *this;\
	}\
	inline T get() const { return func; }

#define FCF_MEMBER_BEFORE_CALL Copier copier(ptr); prepare(ptr);

#define FCF_MEMBER_FUNCTION_BOILERPLATE\
	T func;\
	void * ptr;\
	typedef typename meta::class_type<T>::type class_type;\
public:\
	Function(T func)\
		: FunctionBase(getMemberFunctionAddress(func)), func(func), ptr(getMemberFunctionAddress(func))\
	{\
	}\
	Function(const Function & other)\
		: FunctionBase(other), func(other.func), ptr(other.ptr)\
	{\
	}\
	Function & operator=(T other)\
	{\
		ptr = getMemberFunctionAddress(other);\
		FunctionBase::operator=(ptr);\
		func = other;\
		return *this;\
	}\
	Function & operator=(const Function & other)\
	{\
		FunctionBase::operator=(other);\
		func = other.func;\
		ptr = other.ptr;\
		return *this;\
	}\
	inline T get() const { return func; }

#define FCF_VIRTUAL_FUNCTION_BOILERPLATE\
	typedef typename meta::class_type<T>::type class_type;\
	T func;\
	void * ptr;\
	typedef typename meta::class_type<T>::type class_type;\
public:\
	VirtualFunction(class_type & object, T func)\
		: FunctionBase(getVirtualFunctionAddress(&object, getVTableIndex<T>(func))), func(func), ptr(getVirtualFunctionAddress(&object, getVTableIndex<T>(func)))\
	{\
	}\
	VirtualFunction(const VirtualFunction & other)\
		: FunctionBase(other), func(other.func), ptr(other.ptr)\
	{\
	}\
	VirtualFunction & operator=(const VirtualFunction & other)\
	{\
		FunctionBase::operator=(other);\
		func = other.func;\
		ptr = other.ptr;\
		return *this;\
	}\
	inline T get() const { return func; }

template<typename T, typename Enable = void>
class Function;
template<typename T, typename Enable = void>
class VirtualFunction;

template<typename T>
class Function<T, typename std::enable_if<!FCF_IS_METHOD && FCF_NUM_ARGS == 0>::type>
	: public FunctionBase
{
	FCF_FUNCTION_BOILERPLATE

	FCF_RETURN_TYPE operator()() const
	{
		FCF_BEFORE_CALL;
		return func();
	}
};
#define FCF_IS_CONST_METHOD std::is_const<typename std::remove_reference<typename meta::class_type<T>::type>::type>::value
template<typename T>
class Function<T, typename std::enable_if<FCF_IS_METHOD && !FCF_IS_CONST_METHOD && FCF_NUM_ARGS == 0>::type>
	: public FunctionBase
{
	FCF_MEMBER_FUNCTION_BOILERPLATE

	FCF_RETURN_TYPE operator()(class_type & object) const
	{
		FCF_MEMBER_BEFORE_CALL;
		return (object.*func)();
	}
};
template<typename T>
class Function<T, typename std::enable_if<FCF_IS_METHOD && FCF_IS_CONST_METHOD && FCF_NUM_ARGS == 0>::type>
	: public FunctionBase
{
	FCF_MEMBER_FUNCTION_BOILERPLATE

	FCF_RETURN_TYPE operator()(const class_type & object) const
	{
		FCF_MEMBER_BEFORE_CALL;
		return (object.*func)();
	}
};
template<typename T>
class VirtualFunction<T, typename std::enable_if<FCF_IS_METHOD && !FCF_IS_CONST_METHOD && FCF_NUM_ARGS == 0>::type>
	: public FunctionBase
{
	FCF_VIRTUAL_FUNCTION_BOILERPLATE

	FCF_RETURN_TYPE operator()(class_type & object) const
	{
		FCF_MEMBER_BEFORE_CALL;
		return (object.*func)();
	}
};
template<typename T>
class VirtualFunction<T, typename std::enable_if<FCF_IS_METHOD && FCF_IS_CONST_METHOD && FCF_NUM_ARGS == 0>::type>
	: public FunctionBase
{
	FCF_VIRTUAL_FUNCTION_BOILERPLATE

	FCF_RETURN_TYPE operator()(const class_type & object) const
	{
		FCF_MEMBER_BEFORE_CALL;
		return (object.*func)();
	}
};

#define FCF_DEFINE_FUNCTION_CLASSES(numArgs)\
template<typename T>\
class Function<T, typename std::enable_if<!FCF_IS_METHOD && FCF_NUM_ARGS == numArgs>::type>\
	: public FunctionBase\
{\
	FCF_FUNCTION_BOILERPLATE\
	\
	template<FCF_TYPENAMELIST ## numArgs>\
	FCF_RETURN_TYPE operator()(FCF_RREFARGLIST ## numArgs) const\
	{\
		FCF_BEFORE_CALL;\
		return func(FCF_FORWARDLIST ## numArgs);\
	}\
};\
template<typename T>\
class Function<T, typename std::enable_if<FCF_IS_METHOD && !FCF_IS_CONST_METHOD && FCF_NUM_ARGS == numArgs>::type>\
	: public FunctionBase\
{\
	FCF_MEMBER_FUNCTION_BOILERPLATE\
	\
	template<FCF_TYPENAMELIST ## numArgs>\
	FCF_RETURN_TYPE operator()(class_type & object, FCF_RREFARGLIST ## numArgs) const\
	{\
		FCF_MEMBER_BEFORE_CALL;\
		return (object.*func)(FCF_FORWARDLIST ## numArgs);\
	}\
};\
template<typename T>\
class Function<T, typename std::enable_if<FCF_IS_METHOD && FCF_IS_CONST_METHOD && FCF_NUM_ARGS == numArgs>::type>\
	: public FunctionBase\
{\
	FCF_MEMBER_FUNCTION_BOILERPLATE\
	\
	template<FCF_TYPENAMELIST ## numArgs>\
	FCF_RETURN_TYPE operator()(const class_type & object, FCF_RREFARGLIST ## numArgs) const\
	{\
		FCF_MEMBER_BEFORE_CALL;\
		return (object.*func)(FCF_FORWARDLIST ## numArgs);\
	}\
};\
template<typename T>\
class VirtualFunction<T, typename std::enable_if<FCF_IS_METHOD && !FCF_IS_CONST_METHOD && FCF_NUM_ARGS == numArgs>::type>\
	: public FunctionBase\
{\
	FCF_VIRTUAL_FUNCTION_BOILERPLATE\
	\
	template<FCF_TYPENAMELIST ## numArgs>\
	FCF_RETURN_TYPE operator()(class_type & object, FCF_RREFARGLIST ## numArgs) const\
	{\
		FCF_MEMBER_BEFORE_CALL;\
		return (object.*func)(FCF_FORWARDLIST ## numArgs);\
	}\
};\
template<typename T>\
class VirtualFunction<T, typename std::enable_if<FCF_IS_METHOD && FCF_IS_CONST_METHOD && FCF_NUM_ARGS == numArgs>::type>\
	: public FunctionBase\
{\
	FCF_VIRTUAL_FUNCTION_BOILERPLATE\
	\
	template<FCF_TYPENAMELIST ## numArgs>\
	FCF_RETURN_TYPE operator()(const class_type & object, FCF_RREFARGLIST ## numArgs) const\
	{\
		FCF_MEMBER_BEFORE_CALL;\
		return (object.*func)(FCF_FORWARDLIST ## numArgs);\
	}\
};

FCF_DEFINE_FUNCTION_CLASSES(1)
FCF_DEFINE_FUNCTION_CLASSES(2)
FCF_DEFINE_FUNCTION_CLASSES(3)
FCF_DEFINE_FUNCTION_CLASSES(4)
FCF_DEFINE_FUNCTION_CLASSES(5)
FCF_DEFINE_FUNCTION_CLASSES(6)
FCF_DEFINE_FUNCTION_CLASSES(7)
FCF_DEFINE_FUNCTION_CLASSES(8)

}

