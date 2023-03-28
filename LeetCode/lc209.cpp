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
		// ʹ��whileѭ���������࣬����ǰ���ڵ�Ԫ��֮������target�������Ӵ��ڳ��� 
			subLen = j - i + 1; // ���ڳ���
			ans = subLen < ans ? subLen : ans; // �������ʽ
			sums -= nums[i++]; // ����ֵ���ڻ����targetʱ������ٴ��ڳ���
		}
	}
	return ans == INT32_MAX ? 0 : ans;
}
