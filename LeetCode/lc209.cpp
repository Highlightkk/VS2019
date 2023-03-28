#include <iostream>
#include "lc209.h"

int LC209::minSubArrayLen(int target, vector <int>& nums) {
	int i = 0;
	int j = 0;
	int subLen = 0;
	int ans = INT32_MAX;
	int sums = 0;
	for (; j < nums.size(); j++){
		sums += nums[j];
		while (sums >= target) { 
		// 使用while循环代码更简洁，若当前窗口的元素之和少于target，则增加窗口长度 
			subLen = j - i + 1; // 窗口长度
			ans = subLen < ans ? subLen : ans; // 条件表达式
			sums -= nums[i++]; // 窗口值大于或等于target时，需减少窗口长度
		}
	}
	return ans == INT32_MAX ? 0 : ans;
}
