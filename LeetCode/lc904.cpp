#include "lc904.h"

int LC904::totalFruit(vector<int>& fruits) {
	int i, j, k;
	int fruitsNum;
	int ans = INT32_MIN;

	for (i = 0, j = 0, k = -1, fruitsNum = 0; j < fruits.size(); j++) { // 采摘第一个水果的类型至第一个篮子里
		if (fruits[i] == fruits[j]) { // 遇到连续的水果类型，采摘至相同的篮子里
			i = j;
			fruitsNum++; // 计数
		}
		else { 
			if ((k == -1) || (fruits[k] == fruits[j])) { // 遇到另一种类型的水果, 采摘至另一个篮子里
				k = j;
				fruitsNum++; // 计数
			}
			else { // 遇到第三种类型的水果，停止采摘与计数，移动采摘窗口
				i = (i < k ? i : k) + 1; // 需要返回至原窗口的第一种类型水果首次出现的位置
				j = i - 1;
				k = -1;
				fruitsNum = 0;
			}
		}
		ans = ans > fruitsNum ? ans : fruitsNum; // 记录采摘水果长度
	}
	return ans == INT32_MIN ? 0 : ans;
}
