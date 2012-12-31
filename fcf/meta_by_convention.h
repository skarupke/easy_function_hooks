

	template<typename R>
	struct return_type<R (FCF_CALL_CONVENTION *)()> { typedef R type; };
	template<typename R, typename C>
	struct return_type<R (FCF_CALL_CONVENTION C::*)()> { typedef R type; };
	template<typename R, typename C>
	struct return_type<R (FCF_CALL_CONVENTION C::*)() const> { typedef R type; };
	
	template<typename R>
	struct num_args<R (FCF_CALL_CONVENTION *)()> { static const int value = 0; };
	template<typename R, typename C>
	struct num_args<R (FCF_CALL_CONVENTION C::*)()> { static const int value = 0; };
	template<typename R, typename C>
	struct num_args<R (FCF_CALL_CONVENTION C::*)() const> { static const int value = 0; };

	template<typename R>
	struct func_type<R (FCF_CALL_CONVENTION *)()> { typedef std::function<R ()> type; };
	template<typename R, typename C>
	struct func_type<R (FCF_CALL_CONVENTION C::*)()> { typedef std::function<R (C &)> type; };
	template<typename R, typename C>
	struct func_type<R (FCF_CALL_CONVENTION C::*)() const> { typedef std::function<R (const C &)> type; };
	
	template<typename R, typename C>
	struct class_type<R (FCF_CALL_CONVENTION C::*)()> { typedef C type; };
	template<typename R, typename C>
	struct class_type<R (FCF_CALL_CONVENTION C::*)() const> { typedef C type; };

	template<typename R, typename C1, typename C2>
	struct is_method_compatible<R (FCF_CALL_CONVENTION C1::*)(), R (FCF_CALL_CONVENTION C2::*)()> { static const bool value = true; };
	template<typename R, typename C1, typename C2>
	struct is_method_compatible<R (FCF_CALL_CONVENTION C1::*)() const, R (FCF_CALL_CONVENTION C2::*)() const> { static const bool value = true; };
	
	template<typename R>
	struct calling_convention<R (FCF_CALL_CONVENTION *)()> { static const CallingConvention value = FCF_CALL_CONVENTION_ENUM; };
	template<typename R, typename C>
	struct calling_convention<R (FCF_CALL_CONVENTION C::*)()> { static const CallingConvention value = FCF_CALL_CONVENTION_ENUM; };
	template<typename R, typename C>
	struct calling_convention<R (FCF_CALL_CONVENTION C::*)() const> { static const CallingConvention value = FCF_CALL_CONVENTION_ENUM; };

#define FCF_DEFINE_META_STRUCTS(numArgs)\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct return_type<R (FCF_CALL_CONVENTION *)(FCF_ARGTYPELIST ## numArgs)> { typedef R type; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct return_type<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs)> { typedef R type; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct return_type<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs) const> { typedef R type; };\
	\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct num_args<R (FCF_CALL_CONVENTION *)(FCF_ARGTYPELIST ## numArgs)> { static const int value = numArgs; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct num_args<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs)> { static const int value = numArgs; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct num_args<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs) const> { static const int value = numArgs; };\
	\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct arg_types<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs)> { FCF_TYPEDEFLIST ## numArgs };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct arg_types<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs) const> { FCF_TYPEDEFLIST ## numArgs };\
	\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct func_type<R (FCF_CALL_CONVENTION *)(FCF_ARGTYPELIST ## numArgs)> { typedef std::function<R (FCF_ARGTYPELIST ## numArgs)> type; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct func_type<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs)> { typedef std::function<R (C &, FCF_ARGTYPELIST ## numArgs)> type; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct func_type<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs) const> { typedef std::function<R (const C &, FCF_ARGTYPELIST ## numArgs)> type; };\
	\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct class_type<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs)> { typedef C type; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct class_type<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs) const> { typedef C type; };\
	\
	template<typename R, typename C1, typename C2, FCF_TYPENAMELIST ## numArgs>\
	struct is_method_compatible<R (FCF_CALL_CONVENTION C1::*)(FCF_ARGTYPELIST ## numArgs), R (FCF_CALL_CONVENTION C2::*)(FCF_ARGTYPELIST ## numArgs)> { static const bool value = true; };\
	template<typename R, typename C1, typename C2, FCF_TYPENAMELIST ## numArgs>\
	struct is_method_compatible<R (FCF_CALL_CONVENTION C1::*)(FCF_ARGTYPELIST ## numArgs) const, R (FCF_CALL_CONVENTION C2::*)(FCF_ARGTYPELIST ## numArgs) const> { static const bool value = true; };\
	\
	template<typename R, FCF_TYPENAMELIST ## numArgs>\
	struct calling_convention<R (FCF_CALL_CONVENTION *)(FCF_ARGTYPELIST ## numArgs)> { static const CallingConvention value = FCF_CALL_CONVENTION_ENUM; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct calling_convention<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs)> { static const CallingConvention value = FCF_CALL_CONVENTION_ENUM; };\
	template<typename R, typename C, FCF_TYPENAMELIST ## numArgs>\
	struct calling_convention<R (FCF_CALL_CONVENTION C::*)(FCF_ARGTYPELIST ## numArgs) const> { static const CallingConvention value = FCF_CALL_CONVENTION_ENUM; };

	FCF_DEFINE_META_STRUCTS(1)
	FCF_DEFINE_META_STRUCTS(2)
	FCF_DEFINE_META_STRUCTS(3)
	FCF_DEFINE_META_STRUCTS(4)
	FCF_DEFINE_META_STRUCTS(5)
	FCF_DEFINE_META_STRUCTS(6)
	FCF_DEFINE_META_STRUCTS(7)
	FCF_DEFINE_META_STRUCTS(8)
#undef FCF_DEFINE_META_STRUCTS

