#include "lc35.h"

int LC35::searchInsert(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int middle = 0;
    while (left <= right) { // ���ַ�
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
    if (left > right) { // ��û�ҵ�Ŀ��ֵ���������߽����ұ߽��ֵ���Ŀ��ֵ������Ӧ�ڵ�λ��
        return left;
    }
    else if (left < right) {
        return right;
    }
    else {
        return middle;
    }
}