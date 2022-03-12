#include "../comm/comm.h"

int main() {

    const int[] test_array = {1,1,1,1,0,1,1,1,1,
                              0,0,0,1,1,1,0,0,2,
                              0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0};

    bool is_win = CheckWin(test_array);
    printf("is_win %d\n", is_win);
    return 0;
}