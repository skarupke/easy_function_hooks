#pragma once

#include "meta.h"
#include <functional>

namespace fcf
{

template<typename T, typename P, P ptr>
struct unique_storer
{
	static T func;
};
template<typename T, typename P, P ptr>
T unique_storer<T, P, ptr>::func;

template<int calling_convention, typename T, T * func, typename Enable = void>
struct call_stored_function;
template<int calling_convention, typename T, T * func, typename Enable = void>
struct member_call_stored_function;

#ifdef _M_X64
#define FCF_CURRENT_CALL_CONVENTION
#define FCF_CURRENT_CALL_CONVENTION_ENUM meta::X64Call
#include "lambda_by_convention.h"
#undef FCF_CURRENT_CALL_CONVENTION_ENUM
#undef FCF_CURRENT_CALL_CONVENTION
#else
#define FCF_CURRENT_CALL_CONVENTION __cdecl
#define FCF_CURRENT_CALL_CONVENTION_ENUM meta::CDecl
#include "lambda_by_convention.h"
#undef FCF_CURRENT_CALL_CONVENTION_ENUM
#undef FCF_CURRENT_CALL_CONVENTION
#define FCF_CURRENT_CALL_CONVENTION __stdcall
#define FCF_CURRENT_CALL_CONVENTION_ENUM meta::StdCall
#include "lambda_by_convention.h"
#undef FCF_CURRENT_CALL_CONVENTION_ENUM
#undef FCF_CURRENT_CALL_CONVENTION
#define FCF_CURRENT_CALL_CONVENTION __fastcall
#define FCF_CURRENT_CALL_CONVENTION_ENUM meta::FastCall
#include "lambda_by_convention.h"
#undef FCF_CURRENT_CALL_CONVENTION_ENUM
#undef FCF_CURRENT_CALL_CONVENTION
#define FCF_CURRENT_CALL_CONVENTION __thiscall
#define FCF_CURRENT_CALL_CONVENTION_ENUM meta::ThisCall
#include "lambda_by_convention.h"
#undef FCF_CURRENT_CALL_CONVENTION_ENUM
#undef FCF_CURRENT_CALL_CONVENTION
#endif

}
