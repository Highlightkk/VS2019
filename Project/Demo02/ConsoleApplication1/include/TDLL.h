//
// MATLAB Compiler: 8.3 (R2021b)
// Date: Fri Nov  4 20:16:41 2022
// Arguments:
// "-B""macro_default""-W""cpplib:TDLL,all,version=1.0""-T""link:lib""-d""E:\SZU
// \Code\MatLab\Arduino
// Radar\DLL\TDLL\for_testing""-v""E:\SZU\Code\MatLab\Arduino Radar\phase
// demodulation\TDLL.m"
//

#ifndef TDLL_h
#define TDLL_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_TDLL_C_API 
#define LIB_TDLL_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_TDLL_C_API 
bool MW_CALL_CONV TDLLInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_TDLL_C_API 
bool MW_CALL_CONV TDLLInitialize(void);

extern LIB_TDLL_C_API 
void MW_CALL_CONV TDLLTerminate(void);

extern LIB_TDLL_C_API 
void MW_CALL_CONV TDLLPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_TDLL_C_API 
bool MW_CALL_CONV mlxTDLL(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_TDLL
#define PUBLIC_TDLL_CPP_API __declspec(dllexport)
#else
#define PUBLIC_TDLL_CPP_API __declspec(dllimport)
#endif

#define LIB_TDLL_CPP_API PUBLIC_TDLL_CPP_API

#else

#if !defined(LIB_TDLL_CPP_API)
#if defined(LIB_TDLL_C_API)
#define LIB_TDLL_CPP_API LIB_TDLL_C_API
#else
#define LIB_TDLL_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_TDLL_CPP_API void MW_CALL_CONV TDLL(int nargout, mwArray& cVal, const mwArray& aVal, const mwArray& bFlag);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
