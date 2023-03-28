//
// MATLAB Compiler: 8.3 (R2021b)
// Date: Thu Nov  3 23:10:03 2022
// Arguments:
// "-B""macro_default""-W""cpplib:DACM,all,version=1.0""-T""link:lib""-d""E:\SZU
// \Code\MatLab\Arduino
// Radar\DLL\DACM\for_testing""-v""E:\SZU\Code\MatLab\Arduino Radar\phase
// demodulation\DACM.m"
//

#ifndef DACM_h
#define DACM_h 1

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
#ifndef LIB_DACM_C_API 
#define LIB_DACM_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_DACM_C_API 
bool MW_CALL_CONV DACMInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_DACM_C_API 
bool MW_CALL_CONV DACMInitialize(void);

extern LIB_DACM_C_API 
void MW_CALL_CONV DACMTerminate(void);

extern LIB_DACM_C_API 
void MW_CALL_CONV DACMPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_DACM_C_API 
bool MW_CALL_CONV mlxDACM(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_DACM
#define PUBLIC_DACM_CPP_API __declspec(dllexport)
#else
#define PUBLIC_DACM_CPP_API __declspec(dllimport)
#endif

#define LIB_DACM_CPP_API PUBLIC_DACM_CPP_API

#else

#if !defined(LIB_DACM_CPP_API)
#if defined(LIB_DACM_C_API)
#define LIB_DACM_CPP_API LIB_DACM_C_API
#else
#define LIB_DACM_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_DACM_CPP_API void MW_CALL_CONV DACM(int nargout, mwArray& phase, const mwArray& wrap_data, const mwArray& phasePrev, const mwArray& K1, const mwArray& R0);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
