#include "lc367.h"

bool LC367::isPerfectSquare(int num) {
	int left = 0;
	int right = num;
	int ans = -1;
	while (left <= right) {
		int mid = (left + right) / 2;
		if ((long long)mid * mid <= num) {
			ans = mid;
			left = mid + 1;
		}
		else if ((long long)mid * mid > num) {
			right = mid - 1;
		}
	}
	if (ans * ans == num) {
		return true;
	}
	else {
		return false;
	}
}
