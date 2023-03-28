#include"lc283.h"

void LC283::moveZeros(vector<int>& nums) {
	int len = nums.size();
	if (len > 1) { // ��len<=1������Ҫ��������в���
		int lowIndex = 0;
		int fastIndex = 0;
		int tmp = 0;
		for (; fastIndex < len; fastIndex++) { 
			if ((nums[lowIndex] == 0) && (nums[fastIndex] != 0)) { // �����ڷ���Ԫ������Ԫ�ص�ǰ��
				tmp = nums[lowIndex]; // ����λ��
				nums[lowIndex] = nums[fastIndex];
				nums[fastIndex] = tmp;
				lowIndex++; // ����λ�ú�lowIndex������λ
			}
			else if ((nums[lowIndex] != 0) && (nums[fastIndex]) != 0) { // ����ָ���ָ�����Ԫ��
				lowIndex++; // lowIndex������λ
			}
			else { // �������lowIndex��������λ
				continue;
			}
		}
	}
}