#include <iostream>
#include <complex>
#include <math.h>
#include "include/DACM.h"
#include "include/myadd.h"
#include "include/TDLL.h"
#include "include/Test.h"

#pragma comment(lib, "mclmcrrt.lib")
#pragma comment(lib, "mclmcr.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmat.lib")

using namespace std;

int main()
{
	mclmcrInitialize();
	if (!mclInitializeApplication(NULL, 0))
	{
		std::cout << "Could not initialize the application.\n";
		return -1;
	}
	DACMInitialize();
	int nargout = 1;
	double phaseVal[1];
	double rdata[2] = { 1.0, 2.0 };
	double idata[2] = { 10.0, 20.0 };
	complex<double> phasePrev_Complex(1.3, 1.5);
	mwArray phase(1, 1, mxDOUBLE_CLASS);
	mwArray wrap_data(2, 1, mxDOUBLE_CLASS, mxCOMPLEX);
	mwArray phasePrev(1, 1, mxDOUBLE_CLASS);
	mwArray K1(1, 1, mxDOUBLE_CLASS);
	mwArray R0(1, 1, mxDOUBLE_CLASS);
	wrap_data.Real().SetData(rdata, 2);
	wrap_data.Imag().SetData(idata, 2);
	phasePrev = 2;
	K1 = 1;
	R0 = 1;
	DACM(nargout, phase, wrap_data, phasePrev, K1, R0);
	phase.GetData(phaseVal, 1);
	cout << "当前相位为：" << phaseVal[0] << endl;
	DACMTerminate();
	mclTerminateApplication();
	return 0;
}