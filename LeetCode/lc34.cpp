#include"lc34.h"

vector<int> LC34::searchRange(vector<int>& nums, int target) {
	int border[2];
	border[0] = getLeftBorder(nums, target) + 1; // �������target���ұ߽�
	border[1] = getRightBorder(nums, target) - 1; // ��ĿҪ�����target��λ��
	if (border[0] == -1 || border[1] == -1) { // target�����鷶Χ֮��
		return { -1, -1 }; 
	}
	else if (border[1] >= border[0]) { // target�����鷶Χ���Ҵ���
		return { border[0], border[1] }; 
	}
	else {  // target�����鷶Χ���Ҳ�����
		return { -1, -1 };
	}
}
int LC34::getRightBorder(vector<int>& nums, int target) {
	int left = 0;
	int right = nums.size()-1;
	int middle = 0;
	int rightBorder = -1;
	while (left <= right) {
		middle = (left + right) / 2;
		if (nums[middle] > target) {
			right = middle - 1;
		}
		else { // ��֤target���������
			left = middle + 1;
			rightBorder = left;
		}
	}
	return rightBorder;
}
int LC34::getLeftBorder(vector<int>& nums, int target) {
	int left = 0;
	int right = nums.size() - 1;
	int middle = 0;
	int leftBorder = -1;
	while (left <= right) {
		middle = (left + right) / 2;
		if (nums[middle] < target) {
			left = middle + 1;
		}
		else { // ��֤target���������
			right = middle - 1;
			leftBorder = right;
		}
	}
	return leftBorder;
}
