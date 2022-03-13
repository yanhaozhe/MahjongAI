#include "ShantenChecker.h"
using namespace std;

int main() {
    const int test_array[] = {1,1,1,1,0,1,1,1,1,
                              0,0,0,1,1,1,0,0,2,
                              0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0};

    bool is_win = CheckWin(test_array);
    vector<int> win_tiles;
    ShantenChecker checker(test_array);
    checker.CheckReady(win_tiles);
    printf("is_win %d\n", is_win);
    return 0;
}