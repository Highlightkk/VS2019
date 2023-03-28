#include"lc283.h"

void LC283::moveZeros(vector<int>& nums) {
	int len = nums.size();
	if (len > 1) { // 若len<=1，则不需要对数组进行操作
		int lowIndex = 0;
		int fastIndex = 0;
		int tmp = 0;
		for (; fastIndex < len; fastIndex++) { 
			if ((nums[lowIndex] == 0) && (nums[fastIndex] != 0)) { // 当存在非零元素在零元素的前面
				tmp = nums[lowIndex]; // 交换位置
				nums[lowIndex] = nums[fastIndex];
				nums[fastIndex] = tmp;
				lowIndex++; // 交换位置后lowIndex往后移位
			}
			else if ((nums[lowIndex] != 0) && (nums[fastIndex]) != 0) { // 若两指针均指向非零元素
				lowIndex++; // lowIndex往后移位
			}
			else { // 其它情况lowIndex不进行移位
				continue;
			}
		}
	}
}