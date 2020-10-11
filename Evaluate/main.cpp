#include<bits/stdc++.h>
#include "CardDeck.h"
using namespace std;

int tilesRange[5][34];

class Hands{
public:
    int tiles[34], remains[34];
    int shown[34], hidden[34];
    vector<pair<int, int> > showCards; // 0: Chow 1: Pung 2: Kung
    CardDeck *deck;

    static const int HU_VALUE = 65535.0;

    Hands(char *str){
        memset(tiles, 0, sizeof(tiles));

        deck = NULL;

        int n = strlen(str);
        int i = 0, j, offset;

        showCards.clear();

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
            remains[i] = 4 - tiles[i];
            shown[i] = 0;
            hidden[i] = tiles[i];
        }
    }

    void setDeck(CardDeck &newDeck){
        deck = &newDeck;
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

    double evaluateAnyPairs(bool *limitaions){
    }

    double evaluateAnyChow(){
    }

    double evaluateAnyPung(){
    }

    double evaluatePairs(int cid){

    }

    double evaluateChow(int cid){

    }

    bool hasAnyPair(int *limit){
        for(int i = 0; i < 34; ++i){
            if(limit[i] && hasSinglePair(i))return true;
        }
    }

    bool hasAnySuit(int *limit){
        return hasAnyChow(limit) || hasAnyPung(limit);
    }

    bool hasAnyChow(int *limit){
        for(int i = 0; i < 27; ++i){
            if(limit[i] && i % 9 < 7 && hasSingleChow(i))return true;
        }

        return false;
    }

    bool hasAnyPung(int *limit){
        for(int i = 0; i < 34; ++i){
            if(limit[i] && hasSinglePung(i))return true;
        }

        return false;
    }

    bool hasSingleChow(int cid){ // This function is used to check if a flush cid, cid + 1 and cid + 2 exists.

        if(cid >= 0 && cid < 27 && cid % 9 < 7){

            for(auto i = showCards.begin(); i != showCards.end(); ++i){
                if(i -> first == 0 && i -> second == cid)return true;
            }

            return hidden[cid] && hidden[cid + 1] && hidden[cid + 2];
        }

        return false;
    }

    bool hasSinglePung(int cid){
        if(cid >= 0 && cid < 34){
            for(auto i = showCards.begin(); i != showCards.end(); ++i){
                if((i -> first == 1 || i -> second == 2)&& i -> second == cid)return true;
            }

            return hidden[cid] >= 3;
        }
    }

    bool hasSinglePair(int cid){
        return cid >= 0 && cid < 34 && hidden[cid] >= 2;
    }

    double evaluateSpecial(){
        evaluateQuanBuKao();
    }

    double evaluateQuanBuKao(){

    }

    int getQuanBuKaoTiles(){
        //147m258s369p
        return 14;
    }

    int evaluateWuMenQiSuits(){
        int best = 0;
        for(int i = 0; i < 5; ++i){
            int cur = 0;
            for(int j = 0; j < 5; ++j){
                if(i == j)
                    cur += hasAnyPair(tilesRange[i]);
                else
                    cur += hasAnySuit(tilesRange[i]);
            }

            if(cur > best)best = cur;
            if(cur == 5)break;
        }

        return best;
    }

    double evaluateWuMenQi(){


        int round = 1000;
        const int maxRound = 21;

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));

        while(round--){
            int curRound = 0;

            while(!(deck -> isEmpty()) && curRound < maxRound){
                curRound++;
                int cid = deck -> drawCard();
                hidden[cid]++;

                int curSuits = evaluateWuMenQiSuits();

                if(curSuits == 5){
                    cntsWin[curRound]++;
                    break;
                }
            }
            memcpy(hidden, tmp, sizeof(tmp));
        }

        for(int i = 0; i < maxRound; ++i){
            printf("Round: %d Counts: %d Ratio: .%03d \n", i, cntsWin[i], cntsWin[i] * 1000 / round);
        }
    }

    double wuMenQi(){

        //m刻 m对



       /* double base = 125.0;
        double coeffiencient = 1.0;

        int pairs[6], triples[6], flushs[6], singles[6];
        double valuePairs;

        for(int i = 27; i < 31; ++i){
            if(tiles[i] == 3)fTriple += 1;
            else if(tiles[i] == 2)fPairs += 1;
        }

        for(int i = 31; i < 34; ++i){
            if(tiles[i] == 3)jTriple += 1;
            else if(tiles[i] == 2)jPairs += 1;
        }



        for(int i = 0; i < 9; ++)

        return base * coeffiencient;*/
    }

    double qiDuiZi(){

       /* for(int i = 0; i < 34; ++i){

        }

        if(pairs == 7)return HU_VALUE;
        else if(pairs == 6)return 96.0;
        else if(paris == 5)return 48.0;
        else if(pairs == 4)return 12.0;
        else return 0.1;*/
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

void init(){
    memset(tilesRange, 0, sizeof(tilesRange));
    for(int i = 0; i < 9; ++i)tilesRange[0][i] = true;
    for(int i = 9; i < 18; ++i)tilesRange[1][i] = true;
    for(int i = 18; i < 27; ++i)tilesRange[2][i] = true;
    for(int i = 27; i < 31; ++i)tilesRange[3][i] = true;
    for(int i = 31; i < 34; ++i)tilesRange[4][i] = true;
}


int main()
{
    char s[32];
    scanf("%s", s);

    init();

    Hands myHand(s);
    CardDeck myDeck;

    myHand.setDeck(myDeck);

    myDeck.randomShuffle();
    myDeck.addTile(myHand.tiles);

    time_t st, ed;

    st = clock();

    myHand.evaluateWuMenQi();


    ed = clock();

    printf("Time cost: %d ms. \n", (ed - st) * 1000 / CLOCKS_PER_SEC);

   // myHand.


    return 0;
}
