#include <bits/stdc++.h>
using namespace std;

const int ALL_TYPES = 34;
const int ALL_NUMBER_TYPES = 27;
const int ALL_TILES = 136;


const vector<int> ALL_19_TILES = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33}; //19m19p19s1234567z

/*
00 - 08: 123456789m
09 - 17: 123456789p
18 - 26: 123456789s
27 - 34: 1234567z 东南西北白发中
*/


//牌型判断
bool isEndOrChar[ALL_TYPES]; //幺九牌判断

int CheckWin(const int* tiles) {
    int tmp[ALL_TYPES];
    bool ok = false;
    int total_tiles = 0, pairs = 0, total_19_tiles = 0;
    for(int i = 0; i < ALL_TYPES; ++i) {
        total_tiles += tiles[i];
        pairs += tiles[i] / 2;
    }

    if(total_tiles % 3 != 2) {
        return false;
    }

    if(7 == total_tiles) { // 七对特盼
        return true;
    }

    const int group_required = (total_tiles - 2) / 3; 

    for(int i = 0; i < ALL_TYPES; ++i) {
        memcpy(tmp, tiles, sizeof(tmp));
        int groups = 0;
        if(tmp[i] >= 2) {
            tmp[i] -= 2;

            bool is_continue = true;
            do {
                for(int id = ALL_NUMBER_TYPES; id < ALL_TYPES; ++id) { //检查字牌，除去将之后只能为0或3张
                    if(tmp[i] % 3 == 0) {
                        groups += tmp[i] / 3;
                    }

                    else{
                        is_continue = false;
                        break;
                    }
                }
                if(!is_continue) break;

                for(int id = 0; id < ALL_NUMBER_TYPES; ++id) {
                    if(tmp[id]) {
                        if(tmp[id] > 3) {
                            tmp[id] -= 3;
                            groups++;
                        }

                        if(id % 9 < 7 && tmp[id]) { //只有123, 234, ..., 789 的顺子， 没有8和9开头的顺子 
                            if(tmp[id + 1] >= tmp[id] && tmp[id + 2] >= tmp[id]) {
                                tmp[id + 1] -= tmp[id];
                                tmp[id + 2] -= tmp[id];
                                groups += tmp[id];
                                tmp[id] = 0;
                            }

                            else {
                                is_continue = false;
                                break;
                            }
                        }
                    }
                }
                if(!is_continue) break;
            } while(0);

            if(groups == group_required) {
                ok = true;
                break;
            }
            tmp[i] += 2;
        }
    }
    return ok;
}

void ConvertTilesString2Array(const string& tiles_string, int* tiles_array) { // 将类似1112345678999m转化成34维数组[3,1,1,1,1,1,1,1,3,0,0,...,0]
    if(tiles_array == nullptr) {
        tiles_array = (int *)malloc(sizeof(int) * ALL_TYPES);
    }
}