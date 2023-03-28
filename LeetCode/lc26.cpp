#include"lc26.h"

int LC26::removeDuplicates(vector<int>& nums) {
	int lowIndex = 0;
	int fastIndex = 0;
	for (fastIndex = 0; fastIndex < nums.size(); fastIndex++) {
        int len = nums.size(); // 若为空数组直接返回结果
        if (len == 0) {
            return 0;
        }
        int lowIndex = 0;
        for (int fastIndex = 0; fastIndex < len; fastIndex++) {
            if (nums[lowIndex] == nums[fastIndex]) {
                continue;
            }
            else { // 存在重复值时，找到第一个非重复值的位置并填充至相应位置
                nums[++lowIndex] = nums[fastIndex];
            }
        }
        return lowIndex + 1;
	}
	return lowIndex+1;
}