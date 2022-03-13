#pragma once
#include<bits/stdc++.h>
#include "comm/comm.h"
using namespace std;


class ShantenChecker {

public:
    ShantenChecker(const int* tiles);

    bool CheckSevenPairsWin();
    void CheckSpecialWin(bool &thirteen_orpans, bool &seven_pairs);
    int CheckShanTenNum();
    bool CheckReady(vector<int>& tiles);
    bool Check1Shanten(vector<int>& tiles);
    bool Check2Shanten();
    void ShowHands();

protected:

private:
    int total_hands_[ALL_TYPES];
    int show_hands_[ALL_TYPES];
    int hidden_hands_[ALL_TYPES];
    bool has_show_tiles_;
};