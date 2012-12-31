
template<typename T, T * func>
struct call_stored_function<FCF_CURRENT_CALL_CONVENTION_ENUM, T, func, typename std::enable_if<FCF_NUM_ARGS == 0>::type>
{
	static FCF_RETURN_TYPE FCF_CURRENT_CALL_CONVENTION call()
	{
		return (*func)();
	}
};

#define FCF_DEFINE_CALL_STORED_FUNCTION(numArgs)\
template<typename T, T * func>\
struct call_stored_function<FCF_CURRENT_CALL_CONVENTION_ENUM, T, func, typename std::enable_if<FCF_NUM_ARGS == numArgs>::type>\
{\
	FCF_TYPEDEFARGLIST ## numArgs\
	static FCF_RETURN_TYPE FCF_CURRENT_CALL_CONVENTION call(FCF_ARGLIST ## numArgs)\
	{\
		return (*func)(FCF_FORWARDLIST ## numArgs);\
	}\
};

#define FCF_DEFINE_MEMBER_CALL_STORED_FUNCTION(numArgs)\
template<typename T, T * func>\
struct member_call_stored_function<FCF_CURRENT_CALL_CONVENTION_ENUM, T, func, typename std::enable_if<FCF_NUM_ARGS == numArgs && !FCF_MEMBER_IS_CONST>::type>\
{\
	FCF_TYPEDEFARGLIST ## numArgs\
	FCF_RETURN_TYPE FCF_CURRENT_CALL_CONVENTION call(FCF_MEMBERARGLIST ## numArgs)\
	{\
		return (*func)(reinterpret_cast<Arg0>(*this) FCF_MEMBERFORWARDLIST ## numArgs);\
	}\
};\
template<typename T, T * func>\
struct member_call_stored_function<FCF_CURRENT_CALL_CONVENTION_ENUM, T, func, typename std::enable_if<FCF_NUM_ARGS == numArgs && FCF_MEMBER_IS_CONST>::type>\
{\
	FCF_TYPEDEFARGLIST ## numArgs\
	typedef FCF_RETURN_TYPE ReturnType;\
	ReturnType FCF_CURRENT_CALL_CONVENTION call(FCF_MEMBERARGLIST ## numArgs) const\
	{\
		return (*func)(reinterpret_cast<Arg0>(*this) FCF_MEMBERFORWARDLIST ## numArgs);\
	}\
};

#define FCF_DEFINE_LAMBDA_FUNCTIONS(i) FCF_DEFINE_CALL_STORED_FUNCTION(i) FCF_DEFINE_MEMBER_CALL_STORED_FUNCTION(i)
FCF_DEFINE_LAMBDA_FUNCTIONS(1)
FCF_DEFINE_LAMBDA_FUNCTIONS(2)
FCF_DEFINE_LAMBDA_FUNCTIONS(3)
FCF_DEFINE_LAMBDA_FUNCTIONS(4)
FCF_DEFINE_LAMBDA_FUNCTIONS(5)
FCF_DEFINE_LAMBDA_FUNCTIONS(6)
FCF_DEFINE_LAMBDA_FUNCTIONS(7)
FCF_DEFINE_LAMBDA_FUNCTIONS(8)
