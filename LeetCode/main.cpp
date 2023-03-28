#include<iostream>
#include<string>
#include<vector>
//#include"lc24.h"
//#include"lc26.h"
//#include"lc34.h"
//#include"lc35.h"
//#include"lc209.h"
//#include"lc844.h"
#include"lc904.h"
//#include"lc977.h"
//#include"lc1419.h"

using namespace std;

int main() {
	LC904 L904;
	vector<int> fruits = { 1,0,1,4,1,4,1,2,3 };
	cout << "Case1:" << L904.totalFruit(fruits) << endl;
	return 0;
}
/*
* 	LC24 L24;
	vector<int> v1 = { 3, 2, 2, 3 };
	int val = 3;
	cout << "Case1:" << L24.removeElement(v1, val) << endl;
*/
/*
* 	LC26 L26;
	vector nums = { 1, 1, 2 };
	cout << "Case1:" << L26.removeDuplicates(nums) << endl;
*/
/*
* 	LC34 L34;
	int a1[] = {1};
	int target = 1;
	vector<int> v1(a1, a1 + 1);
	L34.searchRange(v1, target); 
*/
/*	LC35 L35;
	int a1[] = { 1, 3, 5, 6 };
	int target[] = { 5, 2, 7 };
	vector<int> v1(a1, a1+4);
	cout << "Case1:" << L35.searchInsert(v1, target[1]) << endl;*/
/*
	int target = 11;
	vector<int> nums = {1,2,3,4,5};
	LC209 L209;
	cout << "Case1:" << L209.minSubArrayLen(target, nums) << endl;
*/
/*
	LC844 L844;
	string s = "a##c";
	string t = "#a#c";
	cout << "Case1:" << L844.backspaceCompare(s, t) << endl;
*/
/*
* 	vector<int> nums = { -4,-1,0,3,10 };
	LC977 L977;
	L977.sortedSquares(nums);
*/ 
/*
	LC1419 L1419;
	string str1 = "croakcroak" , str2 = "crcoakroak", str3 = "croakcrook";
	cout << "Case1:" << S1.minNumberOfFrogs(str1) << endl;
	cout << "Case2:" << S1.minNumberOfFrogs(str2) << endl;
	cout << "Case3:" << S1.minNumberOfFrogs(str3) << endl;*/
