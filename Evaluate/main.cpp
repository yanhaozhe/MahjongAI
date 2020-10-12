#include<bits/stdc++.h>
#include "CardDeck.h"
using namespace std;

int tilesRange[5][34];

//0.75 0.2 0.04 0.01


struct HandElements{
    int pairs[34], triples[34];
    int flushes[3][7];
};

class Hands{
public:
    int tiles[34], remains[34];
    int shown[34], hidden[34];
    static const int MAX_ROUND = 8192;

    HandElements he;

    typedef double (Hands::*evaluateFunction)();

    int remainTiles;
    vector<pair<int, int> > showCards; // 0: Chow 1: Pung 2: Kung
    CardDeck *myDeck;

    static const int HU_VALUE = 65535.0;

    Hands(char *str){
        memset(tiles, 0, sizeof(tiles));

        myDeck = NULL;

        int n = strlen(str);
        int i = 0, j, offset;

        remainTiles = 21;

        showCards.clear();

        while(i < n){
            j = i;
            while(j < n && isdigit(str[j]))++j;

            offset = 0;
            char type = str[j];
            if(type == 'm')offset = 0;
            else if(type == 's')offset = 9;
            else if(type == 'p')offset = 18;
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

    void updateHE(){
        memset(he.pairs, 0, sizeof(he.pairs));
        memset(he.triples, 0, sizeof(he.triples));
        memset(he.flushes, 0, sizeof(he.flushes));

        for(auto it = showCards.begin(); it != showCards.end(); ++it){
            int type = it -> first, card = it -> second;

            if(type == 0){
                he.flushes[card / 3][card % 9] = 2;
            }

            else{
                he.triples[card]++;
            }
        }

        for(int i = 0; i < 34; ++i){
            he.pairs[i] += hidden[i] / 2;
            he.triples[i] += hidden[i] / 3;


            int type = i / 9, ord = i % 9;
            if(type < 3 && ord < 7){
                if(he.flushes[type][ord])continue;
                if(hidden[i] && hidden[i + 1] && hidden[i + 2])he.flushes[type][ord] = 1;
            }
        }
    }

    void setDeck(CardDeck &newDeck){
        myDeck = &newDeck;
    }

    void output(){
        for(int i = 0; i < 34; ++i){
            if(i == 8 || i == 17 || i == 26 || i == 33)printf("%d\n", tiles[i]);
            else printf("%d ", tiles[i]);
        }
    }

    bool hasAnyPair(int *limit){
        for(int i = 0; i < 34; ++i){
            if(limit[i] && hasSinglePair(i))return true;
        }
        return false;
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
        return false;
    }

    bool hasSinglePair(int cid){
        return cid >= 0 && cid < 34 && hidden[cid] >= 2;
    }

    bool hasHiddenChow(int cid){
        return hidden[cid] && hidden[cid + 1] && hidden[cid + 2];
    }

    bool hasHiddenPung(int cid){
        return hidden[cid] >= 3;
    }

    double evaluateSpecial(){
        evaluateQuanBuKao();
        evaluateQiDuiZi();
    }

    double calcValue(int *cntsWin, int maxRound){

        int acc = 0;
        double accScore = 0.0;

        for(int i = 0; i < maxRound; ++i){
            acc += cntsWin[i];
            double ra = acc * 1.0 / MAX_ROUND;

            if(i == 2)accScore += ra / 2.0;
            if(i == 5)accScore += ra / 4.0;
            if(i == 8)accScore += ra / 8.0;
            if(i == 11)accScore += ra / 16.0;
            if(i == 14)accScore += ra / 32.0;
            if(i == 17)accScore += ra / 64.0;
            if(i == 20)accScore += ra / 64.0;
           // printf("Round: %d Counts: %d Acc: %d Ratio: .%03d \n", i, cntsWin[i], acc, acc * 1000 / MAX_ROUND);
        }

        if(maxRound < 2)return acc * 1.0 / MAX_ROUND;
        if(maxRound < 5)return accScore * 2.0 / 1.0;
        if(maxRound < 8)return accScore * 4.0 / 3.0;
        if(maxRound < 11)return accScore * 8.0 / 7.0;
        if(maxRound < 14)return accScore * 16.0 / 15.0;
        if(maxRound < 17)return accScore * 32.0 / 31.0;
        if(maxRound < 20)return accScore * 65.0 / 64.0;

        return accScore * 100;
    }

    double evaluateQuanBuKao(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));



        while(round--){
            int curRound = 0;

            while(!(myDeck -> isEmpty()) && curRound < maxRound){

                curRound++;
                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;

                int quanBuKaoTiles = countQuanBuKaoTiles();

                if(quanBuKaoTiles >= 14){
                    cntsWin[curRound]++;
                    break;
                }
            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        return calcValue(cntsWin, maxRound);
    }

    double evaluateSanSeSanTongShun(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));

        while(round--){
            int curRound = 0;

            while(!(myDeck -> isEmpty()) && curRound < maxRound){

                curRound++;
                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;

                updateHE();

                if(winSanSeSanTongShun()){
                    cntsWin[curRound]++;
                    break;
                }

            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        return calcValue(cntsWin, maxRound);
    }

    int hasSanSeSanTongShun(){
        for(int start = 0; start < 7; ++start){
            if(he.flushes[0][start] && he.flushes[1][start] && he.flushes[2][start]){
                int val = start * 1000 + he.flushes[0][start] * 100 + he.flushes[1][start] * 10 + he.flushes[2][start];
                return val;
            }
        }

        return -1;
    }

    bool winSanSeSanTongShun(){
        int checkVal = hasSanSeSanTongShun();
        if(checkVal == -1)return false;


        int startId = checkVal / 1000, mv = checkVal / 100 % 10, sv = checkVal / 10 % 10, pv = checkVal % 10;

        int tmp[34];
        memcpy(tmp, hidden, sizeof(tmp));

        int extraShowSuits = showCards.size();

        if(mv == 1){hidden[startId]--; hidden[startId + 1]--; hidden[startId + 2]--;}
        else extraShowSuits--;

        startId += 9;
        if(sv == 1){hidden[startId]--; hidden[startId + 1]--; hidden[startId + 2]--;}
        else extraShowSuits--;

        startId += 9;
        if(pv == 1){hidden[startId]--; hidden[startId + 1]--; hidden[startId + 2]--;}
        else extraShowSuits--;

        bool ok = false;
        for(int i = 0; i < 34; ++i){
            if(hidden[i] >= 2){
                hidden[i] -= 2;
                if(extraShowSuits > 0){ok = true; break;}
                for(int j = 0; j < 34; ++j){
                    if(hasHiddenChow(j) || hasHiddenPung(j)){ok = true; break;}
                }
                hidden[i] += 2;
            }

            if(ok)break;
        }

        memcpy(hidden, tmp, sizeof(tmp));

        return ok;
    }

    int countQuanBuKaoTiles(){

        int countQuanBuKaoNums[3][3];

        memset(countQuanBuKaoNums, 0, sizeof(countQuanBuKaoNums));

        for(int i = 0; i < 27; ++i){
            int type = i / 9, ord = i % 3;
            if(hidden[i])countQuanBuKaoNums[type][ord]++;
        }

        int v = 0;
        for(int i = 27; i < 34; ++i){
            if(hidden[i] > 0)v++;
        }

        int u = 0;

        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(i != j){
                    int k = 3 - i - j;
                    if(i != k && j != k){
                        int cur = countQuanBuKaoNums[0][i] + countQuanBuKaoNums[1][j] + countQuanBuKaoNums[2][k];
                        if(cur > u) u = cur;
                    }
                }
            }
        }
        return u + v;
    }



    int evaluateWuMenQiSuits(bool *hasPair, bool *hasSuit){
        int best = 0;
        for(int i = 0; i < 5; ++i){
            int cur = 0;
            for(int j = 0; j < 5; ++j){
                if(i == j)
                    if(hasPair[j])cur++;
                    else{
                        hasPair[j] = hasAnyPair(tilesRange[j]);
                        cur += hasPair[j];
                    }

                else{
                    if(hasSuit[j])cur++;
                    else{
                        hasSuit[j] = hasAnySuit(tilesRange[j]);
                        cur += hasSuit[j];
                    }
                }
            }

            if(cur > best)best = cur;
            if(cur == 5)break;
        }

        return best;
    }

    double evaluateWuMenQi(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));


        bool hasPair[5], hasSuit[5];

        while(round--){
            int curRound = 0;

            for(int i = 0; i < 5; ++i)hasPair[i] = hasSuit[i] = false;

            while(!(myDeck -> isEmpty()) && curRound < maxRound){
                curRound++;
                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;

                int curSuits = evaluateWuMenQiSuits(hasPair, hasSuit);

                if(curSuits == 5){
                    cntsWin[curRound]++;
                    break;
                }
            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        return calcValue(cntsWin, maxRound);
    }

    double evaluateQiDuiZi(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));

        while(round--){
            int curRound = 0;

            while(!(myDeck -> isEmpty()) && curRound < maxRound){
                curRound++;
                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;

                int worst = -1;

                for(int j = 0; j < 34; ++j){
                    if(hidden[j] % 2 == 1){
                        if(worst == -1 || remains[j] < remains[worst])worst = j;
                    }
                }

                hidden[worst]--;

                int pairs = 0;

                for(int j = 0; j < 34; ++j){
                    pairs += hidden[j] / 2;
                }

                if(pairs >= 7){
                    cntsWin[curRound]++;
                    break;
                }
            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        return calcValue(cntsWin, maxRound);
    }

    void testEvaluateFunction(const char* funcName, evaluateFunction func){

        time_t st, ed;

        printf("Starting evalutaing %s...\n", funcName);

        st = clock();

        printf("%.4lf\n", (this->*func)());

        ed = clock();

        printf("Time cost: %d ms. \n", (ed - st) * 1000 / CLOCKS_PER_SEC);
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
    freopen("test.txt", "r", stdin);
    char s[256];
    scanf("%s", s);

    init();

    Hands myHand(s);
    CardDeck myDeck;

    myHand.setDeck(myDeck);

    myDeck.randomShuffle();
    myDeck.setTiles(myHand.remains);


    myHand.testEvaluateFunction("WuMenQi", &Hands::evaluateWuMenQi);

    myHand.testEvaluateFunction("QuanBuKao", &Hands::evaluateQuanBuKao);

    myHand.testEvaluateFunction("QiDuiZi", &Hands::evaluateQiDuiZi);

    myHand.testEvaluateFunction("SanSeSanTongShun", &Hands::evaluateSanSeSanTongShun);


   // myHand.


    return 0;
}
