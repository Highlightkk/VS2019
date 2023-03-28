#include "lc904.h"

int LC904::totalFruit(vector<int>& fruits) {
	int i, j, k;
	int fruitsNum;
	int ans = INT32_MIN;

	for (i = 0, j = 0, k = -1, fruitsNum = 0; j < fruits.size(); j++) { // ��ժ��һ��ˮ������������һ��������
		if (fruits[i] == fruits[j]) { // ����������ˮ�����ͣ���ժ����ͬ��������
			i = j;
			fruitsNum++; // ����
		}
		else { 
			if ((k == -1) || (fruits[k] == fruits[j])) { // ������һ�����͵�ˮ��, ��ժ����һ��������
				k = j;
				fruitsNum++; // ����
			}
			else { // �������������͵�ˮ����ֹͣ��ժ��������ƶ���ժ����
				i = (i < k ? i : k) + 1; // ��Ҫ������ԭ���ڵĵ�һ������ˮ���״γ��ֵ�λ��
				j = i - 1;
				k = -1;
				fruitsNum = 0;
			}
		}
		ans = ans > fruitsNum ? ans : fruitsNum; // ��¼��ժˮ������
	}
	return ans == INT32_MIN ? 0 : ans;
}
