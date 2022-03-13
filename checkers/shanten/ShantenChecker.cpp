#include<bits/stdc++.h>
#include "ShantenChecker.h"
using namespace std;

ShantenChecker::ShantenChecker(const int* tiles) {
    memcpy(total_hands_, tiles, sizeof(total_hands_));
}

void ShantenChecker::ShowHands() {
    for(int i = 0; i < ALL_TYPES; ++i) {
        printf("%d ", total_hands_[i]);
    }
    printf("\n");
}

bool ShantenChecker::CheckSevenPairsWin() {
    int pairs = 0;
    for(int i = 0; i < ALL_TYPES; ++i) {
        pairs += total_hands_[i] / 2;
    }

    return 7 == pairs;
}

bool ShantenChecker::CheckReady(vector<int> &tiles) {
    ShowHands(); 
    tiles.clear();
    int tmp[ALL_TYPES];
    memcpy(tmp, total_hands_, sizeof(tmp));
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

    return tiles.size() > 0;
}

bool ShantenChecker::Check1Shanten(vector<int>& tiles) {
    int tmp[ALL_TYPES];
    tiles.clear();
    memcpy(tmp, total_hands_, sizeof(tmp));

    for(int i = 0; i < ALL_TYPES; ++i) { //Swap tile #i to #j, and get #k can get a win
        memcpy(total_hands_, tmp, sizeof(tmp)); 
        if(total_hands_[i]) {
            total_hands_[i]--;
            printf("%d\n", i);
            bool ok = false;
            for(int j = 0; j < ALL_TYPES && !ok; ++j) {
                if(total_hands_[j]) {
                    total_hands_[j]--;
                    for(int k = 0; k < ALL_TYPES && !ok; ++k) {
                        if(i == k || j == k) {
                            continue;
                        }
                        total_hands_[k]++;

                        vector<int> ready_tiles;                       
                        if(CheckReady(ready_tiles)) {
                            tiles.emplace_back(i);
                            ok = true;
                        }

                        total_hands_[k]--;
                    }
                    total_hands_[j]++;
                }
            }
            total_hands_[i]++;
        }
    }

    printf("Total 1 shanten tiles %zu:\n", tiles.size());
    for(auto id:tiles) {
        printf("%d\n", id);
    }

    return tiles.size() > 0;
}

void ShantenChecker::CheckSpecialWin(bool &thirteen_orpans, bool &seven_pairs) {
    if(has_show_tiles_) {
        thirteen_orpans = seven_pairs = -2; //ERR_ARG
        return;
    }

    //seven_pairs = CheckSevenPairs();
}

int ShantenChecker::CheckShanTenNum() {
    return 0;
}

int main() {
    const int test_array[] = {3,1,1,1,1,1,1,1,3,
                              0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0};

    const int test_array2[] = {0,0,0,0,0,0,0,2,0,
                            0,0,1,1,2,3,0,0,0,
                            1,1,1,1,0,0,0,0,0,
                            1,0,0,0,0,0,0};
    
    bool is_win = CheckWin(test_array);
    vector<int> win_tiles;
    ShantenChecker checker(test_array);
    checker.ShowHands();
    checker.CheckReady(win_tiles);

    ShantenChecker checker2(test_array2);
    checker2.ShowHands();
    checker2.Check1Shanten(win_tiles);
    return 0;
}