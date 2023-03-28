#include "lc1419.h"
/*
作者：Simon
链接：https ://leetcode.cn/problems/minimum-number-of-frogs-croaking/solutions/209642/pan-duan-dang-qian-you-duo-shao-ge-zi-mu-c-by-simo/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
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
        re = max(re, c); // 遇到k前要判断有多少个c同时存在
        if (croakOfFrogs[i] == 'k') { // 遇到k就需要规约一个croak
            k++;
            if (c >= r && r >= o && o >= a && a >= k) {
                c--;
                r--;
                o--;
                a--;
                k--;
            }
        }
        if (!(c>=r && r>=o && o>=a && a>=k)) { //必须保持任意时刻（c>=r>=o>=a>=k）,才是正确的；否则就是错误的
            flag = false;
            break;
        }
    }
    if (c != 0 || r != 0 || o != 0 || a != 0 || k != 0) flag = false;//如果最后有剩的字母，也是错误的
    if (flag == true) return re;
    else return -1;
}