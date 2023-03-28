#include"lc26.h"

int LC26::removeDuplicates(vector<int>& nums) {
	int lowIndex = 0;
	int fastIndex = 0;
	for (fastIndex = 0; fastIndex < nums.size(); fastIndex++) {
        int len = nums.size(); // ��Ϊ������ֱ�ӷ��ؽ��
        if (len == 0) {
            return 0;
        }
        int lowIndex = 0;
        for (int fastIndex = 0; fastIndex < len; fastIndex++) {
            if (nums[lowIndex] == nums[fastIndex]) {
                continue;
            }
            else { // �����ظ�ֵʱ���ҵ���һ�����ظ�ֵ��λ�ò��������Ӧλ��
                nums[++lowIndex] = nums[fastIndex];
            }
        }
        return lowIndex + 1;
	}
	return lowIndex+1;
}