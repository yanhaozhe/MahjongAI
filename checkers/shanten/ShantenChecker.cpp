#include<bits/stdc++.h>
#include<shanten.h>
using namespace std;

void ShantenChecker::CheckValid() {

}

ShantenChecker::ShantenChecker(int *tiles) {
    memcpy(total_hands_, tiles, sizeof(total_hands_));
}

void ShantenChecker::CheckSevenPairs(int &seven_pairs) {
    int pairs = 0;
    for(int i = 0; i < TOTAL_TYPES; ++i) {
        pairs += total_hands_[i] / 2;
    }

    seven_pairs = 6 - pairs;
}

void ShantenChecker::ShantenChecker(const int* tiles) {
    memcpy(total_hands_, tiles, sizeof(total_hands_));
}

void ShantenChecker::CheckNormalWin() {
}

bool ShantenChecker::CheckReady(vector<int> &tiles) {
    tiles.clear();
    int tmp[ALL_TYPES];
    memcpy(tmp, tiles, sizeof(tmp));
    for(int id = 0; id < ALL_TYPES; ++id) {
        tmp[id]++;
        if(CheckWin(tmp)) {
            tiles.emplace_back(id);
        }
        tmp[id]--;
    }

    printf("Total waiting tiles %zu:\n", tiles.size());
    for(auto id:tiles) {
        printf("%d\n", id);
    }

    return 0;
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
