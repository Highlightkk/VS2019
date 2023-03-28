#include <iostream>
#include <complex>
#include <math.h>
#include "DACM.h"
#include "TDLL.h"
#include "Test.h"
using namespace std;

#pragma comment(lib, "mclmcrrt.lib")
#pragma comment(lib, "mclmcr.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmat.lib")
int main()
{
    cout << "Hello World!\n";
    mclmcrInitialize();
    if (!mclInitializeApplication(NULL, 0))
    {
        std::cout << "Could not initialize the application.\n";
        return -1;
    }
    DACMInitialize();
    TDLLInitialize();
    TestInitialize();

    cout << "Hello World!\n";
    int nargout = 1;
    double cVal[1];
    double rdata[2] = {1.0, 2.0};
    double idata[2] = {10.0, 20.0};
    complex<double> phasePrev_Complex(1.3, 1.5);
    mwArray phase(1, 1, mxDOUBLE_CLASS);
    mwArray wrap_data(2, 1, mxDOUBLE_CLASS, mxCOMPLEX);
    mwArray phasePrev(1, 1, mxDOUBLE_CLASS);
    mwArray K1(1, 1, mxDOUBLE_CLASS);
    mwArray R0(1, 1, mxDOUBLE_CLASS);
    wrap_data.Real().SetData(rdata, 2);
    wrap_data.Imag().SetData(idata, 2);
    wrap_data.Real().GetData(idata, 2);
    wrap_data.Imag().GetData(rdata, 2);
    cout << "Hello World!\n";
    phasePrev = 0;
    K1 = 1;
    R0 = 1;
    TDLL(nargout, phase, phasePrev, R0);
    Test();
    phase.GetData(cVal, 1);
    cout << cVal[1] << endl;
    DACM(nargout, phase, wrap_data, phasePrev, K1, R0);
    cout << "Hello World!\n";
    TestTerminate();
    TDLLTerminate();
    DACMTerminate();
    mclTerminateApplication();
    return 0;
}
