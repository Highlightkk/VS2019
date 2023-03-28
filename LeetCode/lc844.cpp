#include"lc844.h"

/**
* @func          backspaceCompare
* 字符串比较函数(退格处理后)
* @param        s:字符串1 
* @param        t:字符串2
* @return       true: 两字符串相等, 否则返回false
* @type          [bool]
*/
bool LC844::backspaceCompare(string s, string t) {
	int len1 = backspaceDelete(s);
	int len2 = backspaceDelete(t);
	if (len1 != len2) { // 若两字符串不相等，则返回false
		return false;
	}
	for (int i = 0; i < len1; i++) {
		if (s[i] != t[i]) { // 若存在字符不相等，则返回false
			return false;
		}
	}
	return true;
}
/**
* @func          backspaceDelete
* 字符串退格处理函数 
* @param        str
* @return       返回退格后的字符串长度    
* @type          [int]
*/
int LC844::backspaceDelete(string& str) {
	int strlen = str.length();
	if ((strlen == 0) || ( (strlen == 1) && (str[0]=='#') ) ) { // 若len为0或str=''，则不用处理
		return 0;
	}
	int lowIndex = 0;
	int fastIndex = 0;
	string tmpStr = str;
	for (; fastIndex < strlen; fastIndex++) {
		if (tmpStr[fastIndex] != '#') { // 双指针遍历
			tmpStr[lowIndex++] = tmpStr[fastIndex];
		}
		else {
			if (lowIndex == 0) { // 对空文本只进行一次退格操作
				continue;
			}
			lowIndex--; // 退格操作
		}
	}
	str = "";
	for (int i = 0; i < lowIndex; i++) {
		str += tmpStr[i]; // 将退格后的字符串进行赋值
	}
	return lowIndex;
} 
