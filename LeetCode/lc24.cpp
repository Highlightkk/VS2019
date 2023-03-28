#include"lc24.h"

int LC24::removeElement(vector<int>& nums, int val) {
	int len = nums.size();
	int fastIndex = 0; // ��ָ��
	int lowIndex = 0; // ��ָ��
	for (fastIndex = 0; fastIndex < len; fastIndex++) {
		if (nums[fastIndex] != val) {
 			nums[lowIndex++] = nums[fastIndex]; // ��λ����Ԫ��
		}
	}
	return lowIndex;
}
/*
* �������
* 	int len = nums.size();
	for (int i = 0; i < len; i++) {
		if (nums[i] == val) {
			for (int j = i; j < len-1; j++) {
				nums[j] = nums[j + 1];
			}
			i--; // �±�i�Ժ����ֵ����ǰ�ƶ���һλ����iҲ��ǰ�ƶ�һλ
			len--; // ��ʱ����Ĵ�С-1
		}
	}
	return len;
*/