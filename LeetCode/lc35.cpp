#include "lc35.h"

int LC35::searchInsert(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int middle = 0;
    while (left <= right) { // 二分法
        middle = (left + right) / 2;
        if (nums[middle] < target) {
            left = middle + 1;
        }
        else if (nums[middle] > target) {
            right = middle - 1;
        }
        else {
            return middle;
        }
    }
    if (left > right) { // 若没找到目标值，则根据左边界与右边界的值输出目标值在序列应在的位置
        return left;
    }
    else if (left < right) {
        return right;
    }
    else {
        return middle;
    }
}