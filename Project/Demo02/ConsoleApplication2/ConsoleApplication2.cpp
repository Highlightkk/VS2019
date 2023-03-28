#include <iostream>
#include "myadd.h"

#pragma comment(lib, "mclmcrrt.lib")
#pragma comment(lib, "mclmcr.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmat.lib")

using namespace std;
int main()
{
	bool isOk = 0;
	if (!mclInitializeApplication(NULL, 0))
	{
		std::cout << "Could not initialize the application.\n";
		return -1;
	}
	std::cout << "isOk = " << isOk << std::endl;// isOK = 0
	isOk = myaddInitialize();
	std::cout << "isOk = " << isOk << std::endl; // isOK = 1

	mwArray a(1, 1, mxDOUBLE_CLASS);
	mwArray b(1, 1, mxDOUBLE_CLASS);
	a(1, 1) = 1.8;
	b(1, 1) = 2.9;
	mwArray z(1, 1, mxDOUBLE_CLASS);

	myadd(1, z, a, b);

	std::cout << a << "+" << b << "=" << z << std::endl;

	myaddTerminate();
	mclTerminateApplication();

	system("pause");
	return 0;
}