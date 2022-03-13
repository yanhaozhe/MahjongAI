#include<bits/stdc++.h>
#include "comm/comm.h"
using namespace std;


class ShantenChecker {

public:
    ShantenChecker(const int* tiles);

    void CheckSevenPairs();
    void CheckSpecialWin(int &thirteen_orpans, int &seven_pairs);
    int CheckShanTenNum();
    bool CheckReady(vector<int>& tiles);

protected:

private:
    int total_hands_[ALL_TYPES];
    int show_hands_[ALL_TYPES];
    int hidden_hands_[ALL_TYPES];
    bool has_show_tiles_;
};