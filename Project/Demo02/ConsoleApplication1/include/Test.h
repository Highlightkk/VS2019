//
// MATLAB Compiler: 8.3 (R2021b)
// Date: Fri Nov  4 20:34:29 2022
// Arguments:
// "-B""macro_default""-W""cpplib:Test,all,version=1.0""-T""link:lib""-d""E:\SZU
// \Code\MatLab\Arduino
// Radar\DLL\Test\for_testing""-v""E:\SZU\Code\MatLab\Arduino Radar\phase
// demodulation\Test.m"
//

#ifndef Test_h
#define Test_h 1

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
#ifndef LIB_Test_C_API 
#define LIB_Test_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_Test_C_API 
bool MW_CALL_CONV TestInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_Test_C_API 
bool MW_CALL_CONV TestInitialize(void);

extern LIB_Test_C_API 
void MW_CALL_CONV TestTerminate(void);

extern LIB_Test_C_API 
void MW_CALL_CONV TestPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_Test_C_API 
bool MW_CALL_CONV mlxTest(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_Test
#define PUBLIC_Test_CPP_API __declspec(dllexport)
#else
#define PUBLIC_Test_CPP_API __declspec(dllimport)
#endif

#define LIB_Test_CPP_API PUBLIC_Test_CPP_API

#else

#if !defined(LIB_Test_CPP_API)
#if defined(LIB_Test_C_API)
#define LIB_Test_CPP_API LIB_Test_C_API
#else
#define LIB_Test_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_Test_CPP_API void MW_CALL_CONV Test();

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
