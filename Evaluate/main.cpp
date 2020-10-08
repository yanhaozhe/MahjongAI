#include<bits/stdc++.h>
using namespace std;

class Hands{
public:
    int tiles[34], remains[34];
    int m, s, p, f, j, z;
    int pairs[6], triples[6], flushes[6]; //

    static const int HU_VALUE = 65535.0;

    Hands(char *str){
        memset(tiles, 0, sizeof(tiles));

        int n = strlen(str);
        int i = 0, j, offset;
        while(i < n){
            int j = i;
            while(j < n && isdigit(str[j]))++j;

            offset = 0;
            char type = str[j];
            if(type == 'm')offset = 0;
            else if(type == 'p')offset = 9;
            else if(type == 's')offset = 18;
            else if(type == 'z')offset = 27;

            for(int k = i; k < j; ++k){
                int idx = str[k] - '1' + offset;
                tiles[idx]++;
            }

            i = j + 1;
        }

        for(int i = 0; i < 34; ++i){
            remains[i] = 4 - hands[i];
        }
    }

    void output(){
        for(int i = 0; i < 34; ++i){
            if(i == 8 || i == 17 || i == 26 || i == 33)printf("%d\n", tiles[i]);
            else printf("%d ", tiles[i]);
        }
    }

    void debug(){
    }

    double evaluate(){
       /* int m, s, p, f, j;
        m = s = p = f = 0;
       // pairs = triples = flushes = 0;

        for(int i = 0; i < 9; ++i)m += tiles[i];
        for(int i = 9; i < 18; ++i) s += tiles[i];
        for(int i = 18; i < 27; ++i) p += tiles[i];

        for(int i = 27; i < 31; ++i)f += tiles[i];
        for(int i = 31; i < 34; ++i) j += tiles[i];*/


    }

    double wuMenQi(){
        double base = 125.0;
        double coeffiencient = 1.0;

        // 先判断风牌和箭牌

        int pairs[6], triples[6], flushs[6], singles[6];
        double valuePairs

        for(int i = 27; i < 31; ++i){
            if(tiles[i] == 3)fTriple += 1;
            else if(tiles[i] == 2)fPairs += 1;
        }

        for(int i = 31; i < 34; ++i){
            if(tiles[i] == 3)jTriple += 1;
            else if(tiles[i] == 2)jPairs += 1;
        }



        for(int i = 0; i < 9; ++)

        return base * coeffiencient;
    }

    double qiDuiZi(){

        for(int i = 0; i < 34; ++i){

        }

        if(pairs == 7)return HU_VALUE;
        else if(pairs == 6)return 96.0;
        else if(paris == 5)return 48.0;
        else if(pairs == 4)return 12.0;
        else return 0.1;
    }

    double PengPengHu(){

        double base = 112.0;

        int triples, pairs;
        triples = pairs = 0;

        for(int i = 0; i < 34; ++i){
            if(tiles[i] >= 3)triples++;
            else if(tiles[i] == 2)pairs += 1;
            else if(tiles[i] == 4)pairs += 2;
        }

        if(triples == 4) return base;
    }

    double SanSeSanBuGao(){

    }



    double SanSeSanTongShun(){
        double base = 117;
        for(int i = 0; i < 7; ++i){

        }
    }

    double QingLong(){
        for(int i = 0; i < 9; ++i){

        }
    }
};

int main()
{
    char s[32];
    scanf("%s", s);

    Hands myHand(s);

    myHand.


    return 0;
}
