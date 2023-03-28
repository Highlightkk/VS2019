/*
	Matlab÷–unwrap(X,[],1)
*/


#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>

using namespace std;

#define pi 3.1415926535

void  unwrap(const int n, const float data_in[], float* data_out)
{
	int cen = 0;
	data_out[0] = data_in[0];
	if (((data_in[1]>data_in[0])&&((data_in[1]-data_in[0])<pi))||((data_in[1]-data_in[0])<-pi))
	{
		cen = 1;//+
	}
	else
	{
		cen = 2;//-
	}
	

	for (int i = 1; i < n; i++)
	{
		if (abs(data_in[i] - data_out[i - 1]) < pi)
		{
			data_out[i] = data_in[i];
			printf("%f ", data_out[i]);
			cout << endl;
		}
		else
		{
			if (cen == 1)//+
			{
				data_out[i] = data_in[i] + 2 * pi;
				loop1:
				if (abs(data_out[i]-data_out[i-1]) > pi)
				{
					data_out[i] = data_out[i] + 2 * pi;
					goto loop1;
				}
				printf("%f ", data_out[i]);
				cout << endl;
			}
			else if (cen == 2)//-
			{
				data_out[i] = data_in[i] - 2 * pi;
				loop2:
				if (abs(data_out[i] - data_out[i - 1]) > pi)
				{
					data_out[i] = data_out[i] - 2 * pi;
					goto loop2;
				}
				printf("%f ", data_out[i]);
				cout << endl;
			}
			
		}
	}
}

int main()
{
	float suf[13] = { 3.04646285203787,
		3.28431134182441,
		4.54824774677619,
		6.56197170009954,
		3.63558291469138,
		2.21415579196004,
		4.85612503661644,
		4.47323391813426,
		5.13167152847201,
		2.49199441305071,
		1.37075116256323,
		2.29828529404011,
		0.0915022788565189 };
	float sue[13] = { 0 };
	
	unwrap(13, suf, sue);


	while (1);
}
