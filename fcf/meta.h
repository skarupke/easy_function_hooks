#pragma once

#include <functional>
#include <type_traits>

#define FCF_TYPENAMELIST1 typename Arg0
#define FCF_TYPENAMELIST2 FCF_TYPENAMELIST1, typename Arg1
#define FCF_TYPENAMELIST3 FCF_TYPENAMELIST2, typename Arg2
#define FCF_TYPENAMELIST4 FCF_TYPENAMELIST3, typename Arg3
#define FCF_TYPENAMELIST5 FCF_TYPENAMELIST4, typename Arg4
#define FCF_TYPENAMELIST6 FCF_TYPENAMELIST5, typename Arg5
#define FCF_TYPENAMELIST7 FCF_TYPENAMELIST6, typename Arg6
#define FCF_TYPENAMELIST8 FCF_TYPENAMELIST7, typename Arg7

#define FCF_ARGTYPELIST1 Arg0
#define FCF_ARGTYPELIST2 FCF_ARGTYPELIST1, Arg1
#define FCF_ARGTYPELIST3 FCF_ARGTYPELIST2, Arg2
#define FCF_ARGTYPELIST4 FCF_ARGTYPELIST3, Arg3
#define FCF_ARGTYPELIST5 FCF_ARGTYPELIST4, Arg4
#define FCF_ARGTYPELIST6 FCF_ARGTYPELIST5, Arg5
#define FCF_ARGTYPELIST7 FCF_ARGTYPELIST6, Arg6
#define FCF_ARGTYPELIST8 FCF_ARGTYPELIST7, Arg7

#define FCF_ARGLIST1 Arg0 arg0
#define FCF_ARGLIST2 FCF_ARGLIST1, Arg1 arg1
#define FCF_ARGLIST3 FCF_ARGLIST2, Arg2 arg2
#define FCF_ARGLIST4 FCF_ARGLIST3, Arg3 arg3
#define FCF_ARGLIST5 FCF_ARGLIST4, Arg4 arg4
#define FCF_ARGLIST6 FCF_ARGLIST5, Arg5 arg5
#define FCF_ARGLIST7 FCF_ARGLIST6, Arg6 arg6
#define FCF_ARGLIST8 FCF_ARGLIST7, Arg7 arg7

#define FCF_RREFARGLIST1 Arg0 && arg0
#define FCF_RREFARGLIST2 FCF_RREFARGLIST1, Arg1 && arg1
#define FCF_RREFARGLIST3 FCF_RREFARGLIST2, Arg2 && arg2
#define FCF_RREFARGLIST4 FCF_RREFARGLIST3, Arg3 && arg3
#define FCF_RREFARGLIST5 FCF_RREFARGLIST4, Arg4 && arg4
#define FCF_RREFARGLIST6 FCF_RREFARGLIST5, Arg5 && arg5
#define FCF_RREFARGLIST7 FCF_RREFARGLIST6, Arg6 && arg6
#define FCF_RREFARGLIST8 FCF_RREFARGLIST7, Arg7 && arg7

#define FCF_FORWARDLIST1 std::forward<Arg0>(arg0)
#define FCF_FORWARDLIST2 FCF_FORWARDLIST1, std::forward<Arg1>(arg1)
#define FCF_FORWARDLIST3 FCF_FORWARDLIST2, std::forward<Arg2>(arg2)
#define FCF_FORWARDLIST4 FCF_FORWARDLIST3, std::forward<Arg3>(arg3)
#define FCF_FORWARDLIST5 FCF_FORWARDLIST4, std::forward<Arg4>(arg4)
#define FCF_FORWARDLIST6 FCF_FORWARDLIST5, std::forward<Arg5>(arg5)
#define FCF_FORWARDLIST7 FCF_FORWARDLIST6, std::forward<Arg6>(arg6)
#define FCF_FORWARDLIST8 FCF_FORWARDLIST7, std::forward<Arg7>(arg7)

#define FCF_TYPEDEFLIST1 typedef Arg0 arg0;
#define FCF_TYPEDEFLIST2 FCF_TYPEDEFLIST1 typedef Arg1 arg1;
#define FCF_TYPEDEFLIST3 FCF_TYPEDEFLIST2 typedef Arg2 arg2;
#define FCF_TYPEDEFLIST4 FCF_TYPEDEFLIST3 typedef Arg3 arg3;
#define FCF_TYPEDEFLIST5 FCF_TYPEDEFLIST4 typedef Arg4 arg4;
#define FCF_TYPEDEFLIST6 FCF_TYPEDEFLIST5 typedef Arg5 arg5;
#define FCF_TYPEDEFLIST7 FCF_TYPEDEFLIST6 typedef Arg6 arg6;
#define FCF_TYPEDEFLIST8 FCF_TYPEDEFLIST7 typedef Arg7 arg7;

#define FCF_TYPEDEFARGLIST1 typedef typename meta::arg_types<T>::arg0 Arg0;
#define FCF_TYPEDEFARGLIST2 FCF_TYPEDEFARGLIST1 typedef typename meta::arg_types<T>::arg1 Arg1;
#define FCF_TYPEDEFARGLIST3 FCF_TYPEDEFARGLIST2 typedef typename meta::arg_types<T>::arg2 Arg2;
#define FCF_TYPEDEFARGLIST4 FCF_TYPEDEFARGLIST3 typedef typename meta::arg_types<T>::arg3 Arg3;
#define FCF_TYPEDEFARGLIST5 FCF_TYPEDEFARGLIST4 typedef typename meta::arg_types<T>::arg4 Arg4;
#define FCF_TYPEDEFARGLIST6 FCF_TYPEDEFARGLIST5 typedef typename meta::arg_types<T>::arg5 Arg5;
#define FCF_TYPEDEFARGLIST7 FCF_TYPEDEFARGLIST6 typedef typename meta::arg_types<T>::arg6 Arg6;
#define FCF_TYPEDEFARGLIST8 FCF_TYPEDEFARGLIST7 typedef typename meta::arg_types<T>::arg7 Arg7;
#define FCF_MEMBERARGLIST1
#define FCF_MEMBERARGLIST2 Arg1 arg1
#define FCF_MEMBERARGLIST3 FCF_MEMBERARGLIST2, Arg2 arg2
#define FCF_MEMBERARGLIST4 FCF_MEMBERARGLIST3, Arg3 arg3
#define FCF_MEMBERARGLIST5 FCF_MEMBERARGLIST4, Arg4 arg4
#define FCF_MEMBERARGLIST6 FCF_MEMBERARGLIST5, Arg5 arg5
#define FCF_MEMBERARGLIST7 FCF_MEMBERARGLIST6, Arg6 arg6
#define FCF_MEMBERARGLIST8 FCF_MEMBERARGLIST7, Arg7 arg7
#define FCF_MEMBERFORWARDLIST1
#define FCF_MEMBERFORWARDLIST2 FCF_MEMBERFORWARDLIST1, std::forward<Arg1>(arg1)
#define FCF_MEMBERFORWARDLIST3 FCF_MEMBERFORWARDLIST2, std::forward<Arg2>(arg2)
#define FCF_MEMBERFORWARDLIST4 FCF_MEMBERFORWARDLIST3, std::forward<Arg3>(arg3)
#define FCF_MEMBERFORWARDLIST5 FCF_MEMBERFORWARDLIST4, std::forward<Arg4>(arg4)
#define FCF_MEMBERFORWARDLIST6 FCF_MEMBERFORWARDLIST5, std::forward<Arg5>(arg5)
#define FCF_MEMBERFORWARDLIST7 FCF_MEMBERFORWARDLIST6, std::forward<Arg6>(arg6)
#define FCF_MEMBERFORWARDLIST8 FCF_MEMBERFORWARDLIST7, std::forward<Arg7>(arg7)

namespace fcf
{

namespace meta
{
	enum CallingConvention
	{
		CDecl,
		StdCall,
		FastCall,
		ThisCall,
		X64Call
	};
	template<typename T>
	struct remove_ref_cv
	{
		typedef typename std::remove_reference<typename std::remove_cv<T>::type>::type type;
	};
	
	template<typename T>
	struct return_type;
	template<typename T>
	struct num_args;
	template<typename T>
	struct arg_types;
	template<typename T>
	struct func_type;
	template<typename T>
	struct class_type { typedef void type; };
	template<typename T, typename F>
	struct is_method_compatible { static const bool value = false; };
	template<typename T>
	struct calling_convention;

#ifdef _M_X64
#define FCF_CALL_CONVENTION
#define FCF_CALL_CONVENTION_ENUM X64Call
#include "meta_by_convention.h"
#undef FCF_CALL_CONVENTION_ENUM
#undef FCF_CALL_CONVENTION
#else
#define FCF_CALL_CONVENTION __cdecl
#define FCF_CALL_CONVENTION_ENUM CDecl
#include "meta_by_convention.h"
#undef FCF_CALL_CONVENTION_ENUM
#undef FCF_CALL_CONVENTION
#define FCF_CALL_CONVENTION __stdcall
#define FCF_CALL_CONVENTION_ENUM StdCall
#include "meta_by_convention.h"
#undef FCF_CALL_CONVENTION_ENUM
#undef FCF_CALL_CONVENTION
#define FCF_CALL_CONVENTION __fastcall
#define FCF_CALL_CONVENTION_ENUM FastCall
#include "meta_by_convention.h"
#undef FCF_CALL_CONVENTION_ENUM
#undef FCF_CALL_CONVENTION
#define FCF_CALL_CONVENTION __thiscall
#define FCF_CALL_CONVENTION_ENUM ThisCall
#include "meta_by_convention.h"
#undef FCF_CALL_CONVENTION_ENUM
#undef FCF_CALL_CONVENTION
#endif

	template<typename R>
	struct return_type<std::function<R ()>> { typedef R type; };
	template<typename R>
	struct num_args<std::function<R ()>> { static const int value = 0; };
	
#define FCF_DEFINE_META_STRUCTS(numArgs)\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct return_type<std::function<R (FCF_ARGTYPELIST ## numArgs)>> { typedef R type; };\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct arg_types<std::function<R (FCF_ARGTYPELIST ## numArgs)>> { FCF_TYPEDEFLIST ## numArgs };\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct num_args<std::function<R (FCF_ARGTYPELIST ## numArgs)>> { static const int value = numArgs; };\

	FCF_DEFINE_META_STRUCTS(1)
	FCF_DEFINE_META_STRUCTS(2)
	FCF_DEFINE_META_STRUCTS(3)
	FCF_DEFINE_META_STRUCTS(4)
	FCF_DEFINE_META_STRUCTS(5)
	FCF_DEFINE_META_STRUCTS(6)
	FCF_DEFINE_META_STRUCTS(7)
	FCF_DEFINE_META_STRUCTS(8)
#undef FCF_DEFINE_META_STRUCTS

	template<typename T>
	struct is_method { static const bool value = !std::is_same<typename class_type<T>::type, void>::value; };
}

#define FCF_RETURN_TYPE typename ::fcf::meta::return_type<T>::type
#define FCF_NUM_ARGS ::fcf::meta::num_args<T>::value
#define FCF_IS_METHOD ::fcf::meta::is_method<T>::value
#define FCF_CALL_CONVENTION ::fcf::meta::calling_convention<T>::value
#define FCF_MEMBER_IS_CONST std::is_const<typename std::remove_reference<typename meta::arg_types<T>::arg0>::type>::value
}
