#include<bits/stdc++.h>
#include "Mahjong-GB-CPP/MahjongGB/MahjongGB.h"

#define _BOTZONE_ONLINE
#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include <json/json.h>
#endif

#define SIMPLEIO 0

using namespace std;

vector<string> request, response;



const char* tripleFlushFansName[] = {"SanSeSanTongShun", "SanSeSanBuGao", "QingLong", "HuaLong"};
int tilesRange[5][34];
const double baseValueQiDuiZi[8] = {0.01,0.02,0.04,0.2,1.25,25,500,10000.0};
//0.75 0.2 0.04 0.01

const int tileType[34] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,4,4,4};
const int tileOrd[34] =  {0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,0,1,2};

const string tileName[34] = {"W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8", "W9",
                             "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9",
							 "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9",
							 "F1", "F2", "F3", "F4",
							 "J1", "J2", "J3"
							};

unordered_map<string, int> tileNameID;

const string showTypeName[3] = {"CHI","PENG","GANG"};




char tileNameTenhou[34][3];

class CardDeck{
public:

    static const int MAX_CARD = 136, MAX_TYPE = 34;
    bool vis[MAX_CARD];

    int tmp[34];

    int p;
    std::vector<int> deck;

    CardDeck()
    {
        deck.clear();
        p = 0;
        srand(time(0));
        for(int i = 0; i < MAX_CARD; ++i){
            vis[i] = false;
        }
    }

    bool isEmpty(){
        return p > MAX_CARD;
    }

    void reset(int type){
        p = 0;
        if(type == 1)randomShuffle();
    }


    void mark(int tid){
        int base = tid * 4;
        for(int offset = 0; offset < 4; ++offset){
            if(!vis[base + offset]){
                vis[base + offset] = true;
                break;
            }
        }
    }

    void setTiles(int *remainTiles){
        for(int i = 0; i < 34; ++i){
            for(int j = 0; j < remainTiles[i]; ++j){
                deck.push_back(i * 4 + j);
            }
        }
    }

    int drawCard(){
        int n = deck.size();
        while(p < n && vis[deck[p]])++p;
        if(p < n){++p; return deck[p] / 4;}
        return -1;
    }

    void randomShuffle(){
        random_shuffle(deck.begin(), deck.end());
    }

    ~CardDeck()
    {
        //dtor
    }
};

struct tripleFlushes{
    int cid1;
    int cid2;
    int cid3;

    tripleFlushes(){
        cid1 = cid2 = cid3 = 0;
    }

    void setTripleFlushes(int cid1, int cid2, int cid3){
        this -> cid1 = cid1;
        this -> cid2 = cid2;
        this -> cid3 = cid3;
    }
};

struct HandElements{
    int pairs[34], triples[34];
    int flushes[3][7];
};

struct allTripleFlushesFan{

    vector<tripleFlushes> sanSeSanTongShun;
    vector<tripleFlushes> sanSeSanBuGao;
    vector<tripleFlushes> qingLong;
    vector<tripleFlushes> huaLong;

    tripleFlushes tmpTripleFlushes;


    void initTripleFlushesFan(){
        addSanSeSanTongShun();
        addSanSeSanBuGao();
        addQingLong();
        addHuaLong();
    }

    void add(vector<tripleFlushes> &arr, int cid1, int cid2, int cid3){
        tmpTripleFlushes.setTripleFlushes(cid1, cid2, cid3);
        arr.push_back(tmpTripleFlushes);
    }

    void addSanSeSanTongShun(){
        for(int i = 0; i < 7; ++i){
            add(sanSeSanTongShun, i, i + 9, i + 18);
        }
    }

    void addSanSeSanBuGao(){
        for(int i = 0; i < 5; ++i){
            int j = i + 1, k = i + 2;

            add(sanSeSanBuGao, i, j + 9, k + 18);
            add(sanSeSanBuGao, i, j + 18, k + 9);
            add(sanSeSanBuGao, i + 9, j, k + 18);
            add(sanSeSanBuGao, i + 9, j + 18, k);
            add(sanSeSanBuGao, i + 18, j, k + 9);
            add(sanSeSanBuGao, i + 18, j + 9, k);
        }
    }

    void addQingLong(){
        for(int i = 0; i < 27; i += 9){
            add(qingLong, i, i + 3, i + 6);
        }
    }

    void addHuaLong(){
        int i = 0, j = 3, k = 6;
        add(huaLong, i, j + 9, k + 18);
        add(huaLong, i, j + 18, k + 9);
        add(huaLong, i + 9, j, k + 18);
        add(huaLong, i + 9, j + 18, k);
        add(huaLong, i + 18, j, k + 9);
        add(huaLong, i + 18, j + 9, k);
    }
};

class Hands{
public:
    int tiles[34], remains[34];
    int shown[34], hidden[34];
    int tmp[34];

    static const int MAX_ROUND = 500;

    HandElements he;
    allTripleFlushesFan tf;

    vector<tripleFlushes> *tfPointers[4];


    typedef double (Hands::*evaluateFunction)();

    int remainTiles;
    vector<pair<int, int> > showCards; // 0: Chow 1: Pung 2: Kung
    CardDeck *myDeck;

    Hands(){
        memset(tiles, 0, sizeof(tiles));

        myDeck = NULL;



        remainTiles = 21;

        showCards.clear();

        tf.initTripleFlushesFan();

        tfPointers[0] = &tf.sanSeSanTongShun;
        tfPointers[1] = &tf.sanSeSanBuGao;
        tfPointers[2] = &tf.qingLong;
        tfPointers[3] = &tf.huaLong;

        for(int i = 0; i < 34; ++i){
            hidden[i] = 0;
            remains[i] = 4;
        }
    }

    Hands(char *str){

        Hands();

        int n = strlen(str);
        int i = 0, j, offset;

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

    void addToHE(int cid){
        if(hidden[cid] % 2 == 1)he.pairs[cid]++;
        else if(hidden[cid] == 2)he.triples[cid]++;
        else{
            int type = cid / 9, ord = cid % 9;
            if(type < 3){
                int &i = cid;
                if(ord - 2 >= 0 && !he.flushes[i - 2] && hidden[i - 2] && hidden[i - 1])he.flushes[type][ord - 2] = 1;
                if(ord - 1 >= 0 && ord + 1 < 9 && !he.flushes[i - 1] && hidden[i - 1] && hidden[i + 1])he.flushes[type][ord - 1] = 1;
                if(ord + 2 < 0 && !he.flushes[i] && hidden[i + 1] && hidden[i + 2])he.flushes[type][ord] = 1;

            }
        }
    }

    void setDeck(CardDeck &newDeck){
        myDeck = &newDeck;
    }

    void deductRemain(int cid){
        remains[cid]--;
    }

    void addTile(int cid){
        hidden[cid]++;
        deductRemain(cid);
    }

    void discardTile(int cid, bool isSelf){
        if(isSelf)
            hidden[cid]--;

        else
            deductRemain(cid);
    }

    void output(){
        for(int i = 0; i < 34; ++i){
            if(i == 8 || i == 17 || i == 26 || i == 33)printf("%d\n", tiles[i]);
            else printf("%d ", tiles[i]);
        }
    }

    bool hasAnyPair(int *limit){
        for(int i = 0; i < 34; ++i){
            if(limit[i] && hasHiddenPair(i))return true;
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

    bool hasHiddenPair(int cid){
        return cid >= 0 && cid < 34 && hidden[cid] >= 2;
    }

    bool hasHiddenChow(int cid){
        int type = tileType[cid], ord = tileOrd[cid];
        return type < 3 && ord < 7 && hidden[cid] && hidden[cid + 1] && hidden[cid + 2];
    }

    bool hasHiddenPung(int cid){
        return hidden[cid] >= 3;
    }

    double calcValue(const char* fanName, int *cntsWin, int maxRound){

        int acc = 0;
        double accScore = 0.0;

        double ra = 1.0, decayRate = 0.4, accRate = 0.0;

        for(int i = 0; i < maxRound; ++i){
            acc += cntsWin[i];
            accScore += acc * 1.0 / MAX_ROUND * ra;

            accRate += ra;
            ra = ra * decayRate;
           // printf("Round: %d Counts: %d Acc: %d Ratio: .%03d \n", i, cntsWin[i], acc, acc * 1000 / MAX_ROUND);
        }

        double res = accScore / accRate * 10000.0;


        return res;
    }

    double evaluateQuanBuKao(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];

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

        return calcValue("QuanBuKao", cntsWin, maxRound);
    }

    void excludeSingleFlush(int cid){
        hidden[cid]--;
        hidden[cid + 1]--;
        hidden[cid + 2]--;
    }

    int countOrgTiles(int *orgTiles, int cid1, int cid2, int cid3){
        if(cid1 == cid2 && cid2 == cid3)return orgTiles[cid1];
        return (orgTiles[cid1] > 0) + (orgTiles[cid2] > 0) + (orgTiles[cid3] > 0);
    }

    int checkAndExcludeThreeFlushes(int *orgTiles, int cid1, int cid2, int cid3){
        if(cid1 > cid2)swap(cid1, cid2);
        if(cid1 > cid3)swap(cid1, cid3);
        if(cid2 > cid3)swap(cid2, cid3);

        memcpy(tmp, hidden, sizeof(tmp));

        bool ok1, ok2, ok3;
        ok1 = ok2 = ok3 = false;

        int res = 0;

        int usedOrgTiles = 0;

        for(auto it = showCards.begin(); it != showCards.end(); ++it){
            //if()
            if(it -> first == 0){
                if(!ok1 && it -> second == cid1){ok1 = true; usedOrgTiles += 3; res++;}
                if(!ok2 && it -> second == cid2){ok2 = true; usedOrgTiles += 3; res++;}
                if(!ok3 && it -> second == cid3){ok3 = true; usedOrgTiles += 3; res++;}
            }
        }

        if(!ok1){
            if(hasHiddenChow(cid1)){
                ok1 = true;
                usedOrgTiles += countOrgTiles(orgTiles, cid1, cid1 + 1, cid1 + 2);
                excludeSingleFlush(cid1);
            }
        }

         if(!ok2){
            if(hasHiddenChow(cid2)){
                ok2 = true;
                usedOrgTiles += countOrgTiles(orgTiles, cid2, cid2 + 1, cid2 + 2);
                excludeSingleFlush(cid2);
            }
        }

         if(!ok3){
            if(hasHiddenChow(cid3)){
                ok3 = true;
                usedOrgTiles += countOrgTiles(orgTiles, cid3, cid3 + 1, cid3 + 2);
                excludeSingleFlush(cid3);
            }
        }

        if(ok1 && ok2 && ok3 && usedOrgTiles >= 6)
            return res;
        memcpy(hidden, tmp, sizeof(tmp));
        return -1;
    }


    bool checkOnePairWithOneSuits(int *orgTiles, int usedSuits){
        int extraSuit = showCards.size() - usedSuits;

        for(int i = 0; i < 34; ++i){
            if(hidden[i] >= 2 && orgTiles[i]){
                hidden[i] -= 2;

                if(extraSuit)return true;

                int sub = min(orgTiles[i], 2);
                orgTiles[i] -= sub;

                for(int j = 0; j < 34; ++j){
                    if((hasHiddenChow(j) && countOrgTiles(orgTiles, j, j + 1, j + 2) >= 2)|| (hasHiddenPung(j) && countOrgTiles(orgTiles, j, j, j) >= 2))
                    {
                        orgTiles[i] += sub;
                        return true;
                    }
                }

                orgTiles[i] += sub;

                hidden[i] += 2;
            }
        }

        return false;
    }

    bool winTripleFlushesFans(int *orgTiles, vector<tripleFlushes> &arr){

        memcpy(tmp, hidden, sizeof(tmp));

        int m = arr.size();

        for(int i = 0; i < m; ++i){
            memcpy(hidden, tmp, sizeof(tmp));

            tripleFlushes* cur = &arr[i];

            int val = checkAndExcludeThreeFlushes(orgTiles, cur -> cid1, cur -> cid2, cur -> cid3);

            if(val != -1){
                if(checkOnePairWithOneSuits(orgTiles, val))return true;
            }
        }

        return false;
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



    int evaluateWuMenQiSuits(bool *hasPair, bool *hasSuit, int cid){
        int best = 0;

        int type = tileType[cid], ord = tileOrd[cid];

        if(hidden[cid] >= 2)hasPair[type] = true;

        if(hidden[cid] >= 3)hasSuit[type] = true;

        else if(hidden[cid] == 1 && type < 3){
            if(ord >= 2 && hasHiddenChow(cid - 2))hasSuit[type] = true;
            if(ord >= 1 && ord < 8 && hasHiddenChow(cid - 1))hasSuit[type] = true;
            if(ord < 7 && hasHiddenChow(cid))hasSuit[type] = true;
        }

        int cur;
        for(int i = 0; i < 5; ++i){
            cur = 0;
            for(int j = 0; j < 5; ++j){
                if(i == j)cur += hasPair[j];
                else cur += hasSuit[j];
            }

            if(cur > best)best = cur;
        }
        return best;
    }

    void evaluateWuMenQiInit(bool *hasPair, bool *hasSuit){
        for(int i = 0; i < 5; ++i){
            hasPair[i] = hasSuit[i] = false;
        }

        for(auto it = showCards.begin(); it != showCards.end(); ++it){
            int type = tileType[it -> second];
            hasSuit[type] = true;
        }

        for(int i = 0; i < 34; ++i){
            int type = tileType[i];
            if(hasHiddenChow(i) || hasHiddenPung(i))hasSuit[type] = true;
            if(hasHiddenPair(i))hasPair[type] = true;
        }
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

            evaluateWuMenQiInit(hasPair, hasSuit);

            while(!(myDeck -> isEmpty()) && curRound < maxRound){
                curRound++;
                int cid = myDeck -> drawCard();

               // cid = 27;

                if(cid == -1) break;

                hidden[cid]++;

                addToHE(cid);

                int curSuits = evaluateWuMenQiSuits(hasPair, hasSuit, cid);

                if(curSuits == 5){
                    cntsWin[curRound]++;
                    break;
                }
            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        double wuMenQiValue = calcValue("WuMenQi", cntsWin, maxRound);

        printf("WuMenQi: %.4lf\n", wuMenQiValue);

        return wuMenQiValue;
    }

    void evaluateNormal(vector<double> &values){
        int round = MAX_ROUND;
        myDeck -> reset(1);
        const int maxRound = min(21, remainTiles);

        int cntsWin[4][maxRound];
        bool isWin[4];

        int tmp[34], tmp2[34];


        memset(cntsWin, 0, sizeof(cntsWin));


        while(round--){
            memcpy(tmp, hidden, sizeof(hidden));
            int curRound = 0;

            memset(isWin, 0, sizeof(isWin));

            while(!(myDeck -> isEmpty()) && curRound < maxRound){

                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;
                updateHE();

                for(int i = 0; i < 4; ++i){
                    memcpy(tmp2, hidden,sizeof(tmp2));
                    if(!isWin[i] && winTripleFlushesFans(tmp, *tfPointers[i])){
                        isWin[i] = true; cntsWin[i][curRound]++;
                    }

                    memcpy(hidden, tmp2, sizeof(tmp2));
                }

                curRound++;

            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        for(int i = 0; i < 4; ++i){
            double curValue = calcValue(tripleFlushFansName[i], cntsWin[i], maxRound);
            values.push_back(curValue);

            //printf("%s Value: %.4lf\n", tripleFlushFansName[i], curValue);
        }

        memcpy(hidden, tmp, sizeof(tmp));


       // return calcValue(cntsWin, maxRound);*/
    }

    double evaluateQiDuiZi(){
        /*int round = MAX_ROUND;

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

        return calcValue(cntsWin, maxRound);*/

        updateHE();

        int pairs = 0;
        for(int i = 0; i < 34; ++i)
            pairs += hidden[i] / 2;

        if(pairs <= 2)return baseValueQiDuiZi[pairs];

        if(pairs >= 7)pairs = 7;

        double base = baseValueQiDuiZi[pairs], adjust = 1.0;

        for(int i = 0; i < 34; ++i){
            if(hidden[i] % 2 == 1)adjust *= (remains[i] + 1) / 4.0;
        }

        adjust = pow(adjust, 1.0 / (8 - pairs));

        return base * adjust;

    }

    void testEvaluateFunction(const char* funcName, evaluateFunction func){

        time_t st, ed;

        printf("Starting evalutaing %s...\n", funcName);

        st = clock();

        printf("%.4lf\n", (this->*func)());

        ed = clock();

        printf("Time cost: %d ms. \n", (int)((ed - st) * 1000 / CLOCKS_PER_SEC));
    }

    double getWeightValue(){
        vector<double> values;
        values.clear();

        values.push_back(evaluateQiDuiZi());
        values.push_back(evaluateQuanBuKao());
        values.push_back(evaluateWuMenQi());

        //values.push_back(evaluateSanSeSanTongShun());

        evaluateNormal(values);

        sort(values.begin(), values.end());

        int n = values.size();

        double curWeight = 1.4, decayRate = 0.4, acc = 0.0;
        double value = 0.0;
        for(int i = n - 1; i >= 0; --i){
            value += curWeight * values[i];
            acc += curWeight;
            curWeight *= decayRate;
        }

        return value / acc;
    }

    int playTile(){
        double bestValue = 0.0, bestChoice = -1;
        for(int i = 0; i < 34; ++i){
            if(hidden[i] > 0){

                printf("Play: %s\n", tileNameTenhou[i]);

                hidden[i]--;

                double curValue = getWeightValue();

                hidden[i]++;

                printf("WeightedValue: %.4lf\n" , curValue);
                printf("\n");

                if(curValue > bestValue){
                    bestChoice = i;
                    bestValue = curValue;
                }
            }
        }

        return bestChoice;
    }

    int mingPai(int cid){

        int curValue = getWeightValue();

        int type = tileType[cid],  ord = tileOrd[cid];

        if(type < 3){ // canChi
            if(ord >= 2)return 0;
        }
    }

    vector<string> hand2stdHand(){
        vector<string> ans;
        ans.clear();
        for(int i = 0; i < 34; ++i){
            for(int j = 0; j < hidden[i]; ++j){
                ans.push_back(tileName[i]);
            }
        }

        return ans;
    }

    vector<pair<string, pair<string, int> > > hand2stdPack(){
        vector<pair<string, pair<string, int> > > ans;
        ans.clear();

        int m = showCards.size();

        for(int i = 0; i < m; ++i){
            string showType = showTypeName[showCards[i].first];
            int cid = showCards[i].second;
            string showCardNames = tileName[cid];
            int cardFromID = 1;


            ans.push_back({showType, {showCardNames, cardFromID} });
        }

        return ans;
    }

};

void init(){
    memset(tilesRange, 0, sizeof(tilesRange));
    memset(tileNameTenhou, 0, sizeof(tileNameTenhou));

    MahjongInit();

    for(int i = 0; i < 9; ++i)tilesRange[0][i] = true;
    for(int i = 9; i < 18; ++i)tilesRange[1][i] = true;
    for(int i = 18; i < 27; ++i)tilesRange[2][i] = true;
    for(int i = 27; i < 31; ++i)tilesRange[3][i] = true;
    for(int i = 31; i < 34; ++i)tilesRange[4][i] = true;

    for(int i = 0; i < 34; ++i){
        int type = i / 9, ord = i % 9;
        tileNameTenhou[i][0] = ord + '1';
        if(type == 0)tileNameTenhou[i][1] = 'm';
        if(type == 1)tileNameTenhou[i][1] = 's';
        if(type == 2)tileNameTenhou[i][1] = 'p';
        if(type == 3)tileNameTenhou[i][1] = 'z';

        tileNameID[tileName[i]] = i;
    }
}

int calcFan(Hands &myHand, int winCid){


    /* format:
     MahjongFanCalculator(
    vector<pair<string, pair<string, int> > > pack,
    vector<string> hand,
    string winTile,
    int flowerCount,
    bool isZIMO,
    bool isJUEZHANG,
    bool isGANG,
    bool isLAST,
    int menFeng,
    int quanFeng)
    */

    vector<pair<string, pair<string, int> > > pack = myHand.hand2stdPack();
    vector<string> hand = myHand.hand2stdHand();
    string winTile = tileName[winCid];
    int flowerCount = 0;
    bool isZIMO = false;
    bool isJUEZHANG = false;
    bool isGANG = false;
    bool isLAST = false;
    int menFeng = 0;
    int quanFeng = 1;
    int countFan = 0;

    try{
        auto re = MahjongFanCalculator(pack, hand, winTile, flowerCount, isZIMO, isJUEZHANG, isGANG, isLAST, menFeng, quanFeng);
        //auto re = MahjongFanCalculator({{"GANG",{"W1",1}},{"CHI",{"T2",2}}},{"W3","W3","W3","W4","W4","W4","W5"},"W5",1,0,0,0,0,0,0);

        for(auto i : re){
            countFan += i.first;
            cout << i.first << " " << i.second << endl;
        }

        printf("Total: %d Fans.\n", countFan);
    }

    catch(const string &error){
        cout << error << endl;
        countFan = -1;
    }

    return countFan;
}

void myGamePlay(Hands &myHand){
    int turnID;
    string stmp;

    #if SIMPLEIO
        cin >> turnID;
        turnID--;

        getline(cin, stmp);
        for(int i = 0; i < turnID; i++) {
            getline(cin, stmp);
            request.push_back(stmp);
            getline(cin, stmp);
            response.push_back(stmp);
        }
        getline(cin, stmp);
        request.push_back(stmp);
    #else
        Json::Value inputJSON;
        cin >> inputJSON;
        turnID = inputJSON["responses"].size();
        for(int i = 0; i < turnID; i++) {
            request.push_back(inputJSON["requests"][i].asString());
            response.push_back(inputJSON["responses"][i].asString());
        }
        request.push_back(inputJSON["requests"][turnID].asString());

    #endif


        if(turnID < 2) {
            response.push_back("PASS");
        } else {

            int itmp, myPlayerID, quan;

            int lastTile = -1;

            ostringstream sout;
            istringstream sin;
            sin.str(request[0]);
            sin >> itmp >> myPlayerID >> quan;

            sin.clear();
            sin.str(request[1]);

            for(int j = 0; j < 5; j++) sin >> itmp;
            for(int j = 0; j < 13; j++) {
                sin >> stmp;
                myHand.addTile(tileNameID[stmp]);
            }

            for(int i = 2; i < turnID; i++) { // Exe
                sin.clear();
                sin.str(request[i]);
                sin >> itmp;

                if(itmp == 2) { // Draw Card
                    sin >> stmp;

                    myHand.addTile(tileNameID[stmp]);

                    sin.clear();
                    sin.str(response[i]);
                    sin >> stmp >> stmp;
                    hand.erase(find(hand.begin(), hand.end(), stmp));

                    removetile(stmp);
                }

                else if(itmp == 3){ // itmp == 3
                    stmp.clear();
                    sin >> stmp >> stmp;
                    int outCID = -1;
                    if(stmp[0] == 'P' && stmp[1] == 'E'){ //PENG
                        sin >> stmp;
                        outCID = tileToInt(stmp);
                        if(lastTile != -1)remain[lastTile] -= 2;
                    }

                    if(stmp[0] == 'C'){ //CHI
                        sin >> stmp;
                        int midCID = tileToInt(stmp);
                        sin >> stmp;
                        outCID = tileToInt(stmp);

                        for(int k = midCID - 1; k <= midCID + 1; ++k)
                            if(k != lastTile)remain[k] -= 1;
                    }

                    if(stmp[0] == 'P' && stmp[1] == 'L'){ //PLAY
                        sin >> stmp;
                        outCID = tileToInt(stmp);
                        remain[outCID] -= 1;
                    }

                    if(outCID != -1)lastTile = outCID;
                    //printf("Round #%d: %d\n", i, lastTile);
                }
            }

            sin.clear();

            sin.str(request[turnID]);
            sin >> itmp;

            if(itmp == 2) {

                sout.clear();

                //Here we only consider seven pairs

                sin >> stmp;

                addtile(stmp);

                if(pairs == 7){sout << "HU";}

                else{
                    int cid = playTile();
                    string cardName = intTotile(cid);

                    sout << "PLAY " << cardName;

                    tiles[cid]--;
                }

                random_shuffle(hand.begin(), hand.end());
                sout << "PLAY " << *hand.rbegin();
                hand.pop_back();

            } else if(itmp == 3){ // itmp == 3
                int huCID = -1, outCID = -1;
                bool isWin = false;
                if(pairs == 6){

                    for(int i = 0; i < TOTAL_TILES; ++i){
                        if(tiles[i] % 2 == 1){huCID = i; break;}
                    }
                }

                sin >> stmp >> stmp;

                //printf("%d %d\n", huCID, outCID);

                if(stmp[0] == 'P' && stmp[1] == 'E'){ //PENG
                    sin >> stmp;
                    outCID = tileToInt(stmp);
                    if(outCID == huCID)
                        {sout << "HU"; isWin = true;}

                    if(lastTile != -1)remain[lastTile] -= 2;
                }

                if(stmp[0] == 'C'){ //CHI
                    sin >> stmp;

                    int midCID = tileToInt(stmp);

                    sin >> stmp;

                    outCID = tileToInt(stmp);
                    if(outCID == huCID)
                        {sout << "HU"; isWin = true;}

                    for(int k = midCID - 1; k <= midCID + 1; ++k)
                        if(k != lastTile)remain[k] -= 1;

                    //printf("CHI: midcid = %d outcid = %d\n", midCID, outCID);
                }

                if(stmp[0] == 'P' && stmp[1] == 'L'){ //PLAY
                    sin >> stmp;
                    outCID = tileToInt(stmp);
                    if(outCID == huCID)
                        {sout << "HU"; isWin = true;}

                    remain[outCID] -= 1;
                }

                if(outCID != -1)lastTile = outCID;

                //outRemains();

                if(!isWin)sout << "PASS";


            } else{
                sout << "PASS";
            }

            response.push_back(sout.str());
        }

    #if SIMPLEIO
        cout << response[turnID] << endl;
    #else
        Json::Value outputJSON;
        outputJSON["response"] = response[turnID];
        cout << outputJSON << endl;
    #endif
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

   // myHand.playTile();

    calcFan(myHand, 10);

    //play{

    return 0;
}
