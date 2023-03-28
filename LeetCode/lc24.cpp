#include"lc24.h"

int LC24::removeElement(vector<int>& nums, int val) {
	int len = nums.size();
	int fastIndex = 0; // 快指针
	int lowIndex = 0; // 慢指针
	for (fastIndex = 0; fastIndex < len; fastIndex++) {
		if (nums[fastIndex] != val) {
 			nums[lowIndex++] = nums[fastIndex]; // 移位数组元素
		}
	}
	return lowIndex;
}
/*
* 暴力求解
* 	int len = nums.size();
	for (int i = 0; i < len; i++) {
		if (nums[i] == val) {
			for (int j = i; j < len-1; j++) {
				nums[j] = nums[j + 1];
			}
			i--; // 下标i以后的数值均向前移动了一位，故i也向前移动一位
			len--; // 此时数组的大小-1
		}
	}
	return len;
*/