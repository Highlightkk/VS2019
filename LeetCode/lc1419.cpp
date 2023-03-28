#include "lc1419.h"
/*
���ߣ�Simon
���ӣ�https ://leetcode.cn/problems/minimum-number-of-frogs-croaking/solutions/209642/pan-duan-dang-qian-you-duo-shao-ge-zi-mu-c-by-simo/
��Դ�����ۣ�LeetCode��
����Ȩ���������С���ҵת������ϵ���߻����Ȩ������ҵת����ע��������
*/
int LC1419::minNumberOfFrogs(string croakOfFrogs) {
    int c = 0;
    int r = 0;
    int o = 0;
    int a = 0;
    int k = 0;
    int re = 0;
    bool flag = true;
    for (int i = 0; i < croakOfFrogs.size(); i++) {
        if (croakOfFrogs[i] == 'c') c++;
        if (croakOfFrogs[i] == 'r') r++;
        if (croakOfFrogs[i] == 'o') o++;
        if (croakOfFrogs[i] == 'a') a++;
        re = max(re, c); // ����kǰҪ�ж��ж��ٸ�cͬʱ����
        if (croakOfFrogs[i] == 'k') { // ����k����Ҫ��Լһ��croak
            k++;
            if (c >= r && r >= o && o >= a && a >= k) {
                c--;
                r--;
                o--;
                a--;
                k--;
            }
        }
        if (!(c>=r && r>=o && o>=a && a>=k)) { //���뱣������ʱ�̣�c>=r>=o>=a>=k��,������ȷ�ģ�������Ǵ����
            flag = false;
            break;
        }
    }
    if (c != 0 || r != 0 || o != 0 || a != 0 || k != 0) flag = false;//��������ʣ����ĸ��Ҳ�Ǵ����
    if (flag == true) return re;
    else return -1;
}