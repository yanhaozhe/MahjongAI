#include<bits/stdc++.h>
using namespace std;

int tilesRange[5][34];
const double baseValueQiDuiZi[8] = {0.01,0.02,0.04,0.2,1.25,25,500,10000.0};
//0.75 0.2 0.04 0.01

const int tileType[34] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,4,4,4};
const int tileOrd[34] =  {0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,0,1,2};


char tileNameTenhou[34][3];

class CardDeck{
public:

    static const int MAX_CARD = 136, MAX_TYPE = 34;
    bool vis[MAX_CARD];
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

struct HandElements{
    int pairs[34], triples[34];
    int flushes[3][7];
};

class Hands{
public:
    int tiles[34], remains[34];
    int shown[34], hidden[34];
    static const int MAX_ROUND = 1024;

    HandElements he;

    typedef double (Hands::*evaluateFunction)();

    int remainTiles;
    vector<pair<int, int> > showCards; // 0: Chow 1: Pung 2: Kung
    CardDeck *myDeck;

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

    void addRemain(int cid){
        remains[cid]++;
    }

    void removeRemain(int cid){
        remains[cid]--;
    }

    void addTile(int cid){
        hidden[cid]++;
        removeRemain(cid);
    }

    void removeTile(int cid){
        hidden[cid]--;
        addRemain(cid);
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
        int type = tileType[cid], ord = tileOrd[cid];
        return type < 3 && ord < 7 && hidden[cid] && hidden[cid + 1] && hidden[cid + 2];
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

        double ra = 1.0, decayRate = 0.55, accRate = 0.0;

        for(int i = 0; i < maxRound; ++i){
            acc += cntsWin[i];
            accScore += acc * 1.0 / MAX_ROUND * ra;

            accRate += ra;
            ra = ra * decayRate;
           // printf("Round: %d Counts: %d Acc: %d Ratio: .%03d \n", i, cntsWin[i], acc, acc * 1000 / MAX_ROUND);
        }

        return accScore / accRate * 10000.0;
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

    double evaluateWeightValue(){
         int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));

        updateHE();

        while(round--){
            int curRound = 0;

            while(!(myDeck -> isEmpty()) && curRound < maxRound){

                curRound++;
                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;

                //addToHE(cid);
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

    double evaluateSanSeSanTongShun(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];
        int tmp[34];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));

        updateHE();

        while(round--){
            int curRound = 0;

            while(!(myDeck -> isEmpty()) && curRound < maxRound){

                curRound++;
                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;

                //addToHE(cid);
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

                    if((hasHiddenChow(j) || hasHiddenPung(j))){ok = true; break;}
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



    int evaluateWuMenQiSuits(bool *hasPair, bool *hasSuit, int cid){
        int best = 0;

        int type = tileType[cid], ord = tileOrd[cid];

        if(hidden[cid] >= 2)hasPair[type] = true;

        else if(hidden[cid] >= 3)hasSuit[type] = true;

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

    double evaluatePengPengHu(){

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

                int curSuits = evaluateWuMenQiSuits(hasPair, hasSuit, cid);

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

        printf("Time cost: %d ms. \n", (ed - st) * 1000 / CLOCKS_PER_SEC);
    }

    double getWeightValue(){
        vector<double> values;
        values.clear();

        values.push_back(evaluateQiDuiZi());

        values.push_back(evaluateQuanBuKao());
        values.push_back(evaluateWuMenQi());
        values.push_back(evaluateSanSeSanTongShun());

        sort(values.begin(), values.end());

        int n = values.size();
        double curWeight = 1.4, decayRate = 0.88, acc = 0.0;
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
                hidden[i]--;

                double curValue = getWeightValue();

                hidden[i]++;

                printf("Play: %s Value: %.4lf\n", tileNameTenhou[i], curValue);

                if(curValue > bestValue){
                    bestChoice = i;
                    bestValue = curValue;
                }
            }
        }

        return bestChoice;
    }
};

void init(){
    memset(tilesRange, 0, sizeof(tilesRange));
    memset(tileNameTenhou, 0, sizeof(tileNameTenhou));
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
    }
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

    myHand.playTile();


    return 0;
}
