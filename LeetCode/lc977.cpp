#include "lc977.h"

vector<int> LC977::sortedSquares(vector<int>& nums) {
	int i = 0;
	int j = nums.size()-1;
	int k = j;
	vector<int> sorted_nums(j+1, 0);
	for (; i <= j; ) {
		if ((nums[i] * nums[i]) > (nums[j] * nums[j])) { 
			// 若存在负数且平方后比原数组大，则将其置于数组最右
			sorted_nums[k--] = nums[i] * nums[i];
			i++;
		}
		else {
			// 若无需重新排序，则复制数组
			sorted_nums[k--] = nums[j] * nums[j];
			j--;
		}
	}
	return sorted_nums;
}
