#include"lc844.h"

/**
* @func          backspaceCompare
* �ַ����ȽϺ���(�˸����)
* @param        s:�ַ���1 
* @param        t:�ַ���2
* @return       true: ���ַ������, ���򷵻�false
* @type          [bool]
*/
bool LC844::backspaceCompare(string s, string t) {
	int len1 = backspaceDelete(s);
	int len2 = backspaceDelete(t);
	if (len1 != len2) { // �����ַ�������ȣ��򷵻�false
		return false;
	}
	for (int i = 0; i < len1; i++) {
		if (s[i] != t[i]) { // �������ַ�����ȣ��򷵻�false
			return false;
		}
	}
	return true;
}
/**
* @func          backspaceDelete
* �ַ����˸����� 
* @param        str
* @return       �����˸����ַ�������    
* @type          [int]
*/
int LC844::backspaceDelete(string& str) {
	int strlen = str.length();
	if ((strlen == 0) || ( (strlen == 1) && (str[0]=='#') ) ) { // ��lenΪ0��str=''�����ô���
		return 0;
	}
	int lowIndex = 0;
	int fastIndex = 0;
	string tmpStr = str;
	for (; fastIndex < strlen; fastIndex++) {
		if (tmpStr[fastIndex] != '#') { // ˫ָ�����
			tmpStr[lowIndex++] = tmpStr[fastIndex];
		}
		else {
			if (lowIndex == 0) { // �Կ��ı�ֻ����һ���˸����
				continue;
			}
			lowIndex--; // �˸����
		}
	}
	str = "";
	for (int i = 0; i < lowIndex; i++) {
		str += tmpStr[i]; // ���˸����ַ������и�ֵ
	}
	return lowIndex;
} 
