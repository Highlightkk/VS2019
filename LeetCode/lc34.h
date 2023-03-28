/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        lc34.h
*  @author      pedria
*  @date        2022-11-11
*  @brief       https://leetcode.cn/problems/find-first-and-last-position-of-element-in-sorted-array/
*  @namespace   std 
*  @note        
*  @version     -v  2022-11-11
*/
#include<iostream>
#include<vector>
using namespace std;

class LC34 {
public:
    vector<int> searchRange(vector<int>& nums, int target);
private:
    int getRightBorder(vector<int>& nums, int target);
    int getLeftBorder(vector<int>& nums, int target);
};