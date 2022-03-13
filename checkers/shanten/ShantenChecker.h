#include<bits/stdc++.h>
#include "comm/comm.h"
using namespace std;


class ShantenChecker {

public:
ShantenChecker();
ShantenChecker(int *tiles);
~ShantenChecker();

void CheckSevenPairs();
void CheckSpecialWin(int &thirteen_orpans, int &seven_pairs);
int CheckShanTenNum();
bool CheckReady();

protected:

private:
    int total_hands_[ALL_TYPES];
    int show_hands_[ALL_TYPES];
    int hidden_hands_[ALL_TYPES];
    bool has_show_tiles_;
};