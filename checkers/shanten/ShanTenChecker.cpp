#include<bits/stdc++.h>
#include<shanten.h>
using namespace std;

void ShantenChecker::CheckValid() {

}

void ShantenChecker::CheckSevenPairs(int &seven_pairs) {
    int pairs = 0;
    for(int i = 0; i < TOTAL_TYPES; ++i) {
        pairs += total_hands_[i] / 2;
    }

    seven_pairs = 6 - pairs;
}

void ShantenChecker::CheckNormalWin() {
}

bool ShantenChecker::CheckReady(vector<int> &tiles) {
    tiles.clear();
    int tmp[ALL_TYPES];
    memcpy(tmp, )
    for(int i = 0; i < n)
}

void ShantenChecker::CheckSpecialWin(int &thirteen_orpans, int &seven_pairs) {
    if(has_show_tiles) {
        thirteenOrpans = sevenPairs = -2; //ERR_ARG
        return;
    }

    checkSevenPairs(sevenPairs);
}

int ShantenChecker::CheckShanTenNum() {
}
