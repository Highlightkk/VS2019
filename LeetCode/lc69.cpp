#include "lc69.h"
/**
* @func          mySqrt
* 常规的二分法，从x/2开始不能使用while循环
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
		if ((long long)mid * mid <= x) { // long long 避免溢出
			ans = mid;
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return ans;
}
