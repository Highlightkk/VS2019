#include "lc69.h"
/**
* @func          mySqrt
* ����Ķ��ַ�����x/2��ʼ����ʹ��whileѭ��
* @param        x
* @return		ans 
* @type          [int]
*/
int LC69::mySqrt(int x) {
	int right = x;
	int left = 0;
	int ans = -1;
	int mid = 0;
	while (left <= right) {
		mid  = (left + right) / 2;
		if ((long long)mid * mid <= x) { // long long �������
			ans = mid;
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return ans;
}
