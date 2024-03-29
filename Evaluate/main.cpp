#include<bits/stdc++.h>
#include "MahjongGB/MahjongGB.h"

#define _BOTZONE_ONLINE
#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include <json/json.h>
#endif

//#define LOCAL

#define SIMPLEIO 1

using namespace std;

vector<string> request, response;

const char* tripleFlushFansName[] = {"SanSeSanTongShun", "SanSeSanBuGao", "QingLong", "HuaLong", "YiSeSanBuGao", "YiSeSanTongShun"};
const int tripleFlushFansSize[] = {7, 30, 3, 6, 36, 21};

int tilesRange[5][34];
const double baseValueQiDuiZi[8] = {0.01,0.02,0.04,0.2, 0.75, 12.5, 350,10000.0};
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

int quanFeng, menFeng;

const double pairsCoe[] = {0.1, 0.4, 1.0};
const double singleCoe[] = {0.05, 0.25, 0.5, 1.0};
const double extraCoe[] = {0.0, 0.2, 0.3, 0.35, 0.4, 0.42, 0.445, 0.455, 0.460, 0.464};
const double tilenumCoe[] = {0.4, 0.75, 1, 1.2, 1.33, 1.44, 1.55, 1.66, 1.77};
const double readyTileCoe[] = {0, 0.28, 0.5387, 0.7776, 0.9978, 1.2006, 1.3872, 1.5587, 1.7161, 1.8604, 1.9924, 2.1132, 2.2235, 2.324, 2.4155, 2.4987, 2.5742, 2.6427, 2.7046, 2.7605, 2.811, 2.8564, 2.8971, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337, 2.9337}; // 23
const double yiShanTenCoe[] = {0.0, 0.0, 0.0227, 0.0826, 0.1701, 0.2778, 0.4, 0.5325, 0.6722, 0.8163, 0.9631, 1.1111, 1.2591, 1.4062, 1.5519, 1.6955, 1.8367, 1.9753, 2.111, 2.2438, 2.3734, 2.5, 2.6234, 2.7438, 2.861, 2.9752, 3.0864, 3.1947, 3.3001, 3.4028, 3.5027, 3.6, 3.6947, 3.787, 3.8768, 3.9643, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496, 4.0496,4.0496, 4.0496, 4.0496}; // 38
const double erShanTenCoe[] = {0.0, 0.0, 0.0, 0.0004028062166426102, 0.0029296875, 0.009015777610818933, 0.019539995929167515, 0.03498542274052478, 0.05555555555555555, 0.08125876058673721, 0.11196967487972008, 0.14747382794720074, 0.1875, 0.23174359048765977, 0.27988338192419826, 0.33159344460236206, 0.38655146506386173, 0.4444444444444444, 0.504972466507767, 0.5678510541980101, 0.6328125, 0.6996064564934679, 0.768, 0.8377773254630572, 0.9087391898042786, 0.9807021903987856, 1.0534979423868314, 1.1269722013523666, 1.2009839650145773, 1.2754045779268115, 1.350116855959654, 1.4250142419624208, 1.5, 1.574986452610571, 1.6498942633681313, 1.724651765468092, 1.7991943359375, 1.8734638142922166, 1.9474079639368895, 2.0209799742654515, 2.09413800122125, 2.166844743979617, 2.239067055393586, 2.3107755838742032, 2.3819444444444446, 2.4525509167979807, 2.522575168301976, 2.592, 2.660810613792098, 2.7289943990905368, 2.7965407373691398, 2.8634408231397517}; //52
const double sanShanTenCoe[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0063, 0.0828, 0.1602, 0.2377, 0.3146, 0.3904, 0.4647, 0.5371, 0.6074, 0.6754, 0.7409, 0.8039, 0.8642, 0.9219, 0.9768, 1.0291, 1.0788, 1.1259, 1.1704, 1.2124, 1.252, 1.2892, 1.3241, 1.3568, 1.3874, 1.4159, 1.4425, 1.4671, 1.4899, 1.511, 1.5303, 1.5481, 1.5644, 1.5792, 1.5926, 1.6046, 1.6154, 1.625, 1.6334, 1.6408, 1.6471};
const double shanTenBase[] = {486.0, 40.0, 5.5, 2, 0.75, 0.0, 0.0, 0.0};
const double tripleFlushesCoe[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 0.3, 0.6, 1.2, 3};

int worstShanTenAccepted[] = {1, 6, 12, 21};

char tileNameTenhou[34][3];

struct shanTenTiles{
    int shanTen;
    bool isImproveTiles[34];

    shanTenTiles(){
        shanTen = 0;
        memset(isImproveTiles, 0, sizeof(isImproveTiles));
    }
};

const int ALL_FOUR_TILE = 34 * 34 * 34 * 34;
const int MAX_LOAD_NUM = 38422;
shanTenTiles stData[MAX_LOAD_NUM];
int idST[ALL_FOUR_TILE];

int countOnes(int x){
    if(x <= 0)return 0;
    int res = 0;
    while(x > 0){
        res += x % 2;
        x /= 2;
    }
    return res;
}


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
        deck.clear();
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

    void sort(){
        if(cid1 > cid2)swap(cid1, cid2);
        if(cid1 > cid3)swap(cid1, cid3);
        if(cid2 > cid3)swap(cid2, cid3);
    }

    void toArray(int *used){
        for(int i = 0; i < 34; ++i)used = 0;
        used[cid1]++; used[cid1 + 1]++; used[cid1 + 2]++;
        used[cid2]++; used[cid2 + 1]++; used[cid2 + 2]++;
        used[cid3]++; used[cid3 + 1]++; used[cid3 + 2]++;
    }
};

struct HandElements{
    int pairs[34], triples[34];
    int flushes[3][7];

    int totalM, totalS, totalP, totalF, totalJ;
    int singleM, singleS, singleP, singleF, singleJ;
};

struct allTripleFlushesFan{

    vector<tripleFlushes> sanSeSanTongShun;
    vector<tripleFlushes> sanSeSanBuGao;
    vector<tripleFlushes> qingLong;
    vector<tripleFlushes> huaLong;
    vector<tripleFlushes> yiSeSanBuGao;
    vector<tripleFlushes> yiSeSanTongShun;

    tripleFlushes tmpTripleFlushes;


    void initTripleFlushesFan(){
        addSanSeSanTongShun();
        addSanSeSanBuGao();
        addQingLong();
        addHuaLong();
        addYiSeSanBuGao();
        addYiSeSanTongShun();
    }

    void add(vector<tripleFlushes> &arr, int cid1, int cid2, int cid3){
        tmpTripleFlushes.setTripleFlushes(cid1, cid2, cid3);
        tmpTripleFlushes.sort();
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

    void addYiSeSanBuGao(){
        for(int type = 0; type < 3; type++){
            for(int start = 0; start < 7; start++){
                int base = start + type * 9;
                add(yiSeSanBuGao, base, base + 1, base + 2);
                if(start < 5)add(yiSeSanBuGao, base, base + 2, base + 4);
            }
        }
    }

    void addYiSeSanTongShun(){
        for(int type = 0; type < 3; type++){
            for(int start = 0; start < 7; start++){
                int base = start + type * 9;
                add(yiSeSanTongShun, base, base, base);
            }
        }
    }
};

enum limitFans{
    hunYiSe, tuiBuDao, daYuWu, xiaoYuWu, lvYiSe
};

struct limitFan{
    int limitedTiles[34];
    int fanS;

    void addHunYiSe(){

    }
};

int getID(int *a){
    return a[0] * 34 * 34 * 34 + a[1] * 34 * 34 + a[2] * 34 + a[3];
}

struct tripleFanElement{
    tripleFlushes* p;

    int tripleFlushID;

    int orgCounter, orgScore;
    int lackMain[34];
    int *remains;
    int other[34];
    int extraSuits;
    bool improveTiles[34];
    int totalImproveTiles;

    tripleFanElement(){
        p = NULL; remains = NULL;
        tripleFlushID = -1;
        orgCounter = orgScore = extraSuits = 0;
        memset(other, 0, sizeof(other));
        memset(improveTiles, 0, sizeof(improveTiles));
        memset(lackMain, 0, sizeof(lackMain));

        totalImproveTiles = 0;
    }

    void setRemains(int *arr){
        remains = arr;
    }

    void refreshTotalImproveTiles(){
        totalImproveTiles = 0;

        for(int i = 0; i < 34; ++i){
            if(lackMain[i])improveTiles[i] = true;
            if(improveTiles[i])totalImproveTiles += remains[i];
        }
    }

    int getShanTen(){
        int mainTiles = 9 - orgCounter;
        if(mainTiles >= 5)return mainTiles;
        int otherTiles = getShanTenOthers();
        refreshTotalImproveTiles();
        return mainTiles + otherTiles;
    }

    double getScore(){
        int shanTenNum = getShanTen();
        int nextTile = totalImproveTiles;



        if(shanTenNum > 5)return 0.0;
        if(shanTenNum == 5)return 0.375 * tripleFlushesCoe[orgCounter];

        if(shanTenNum < 4 && nextTile < worstShanTenAccepted[shanTenNum]){
            shanTenNum++;
            nextTile += 24;
        }

        if(nextTile > 50)nextTile = 50;

        if(shanTenNum == 4)return 0.75 * tripleFlushesCoe[orgCounter];

        double coe3 = 1.0;
        if(shanTenNum == 3)coe3 = sanShanTenCoe[nextTile];
        if(shanTenNum == 2)coe3 = erShanTenCoe[nextTile];
        if(shanTenNum == 1)coe3 = yiShanTenCoe[nextTile];
        if(shanTenNum == 0)coe3 = readyTileCoe[nextTile];

        return shanTenBase[shanTenNum] * tripleFlushesCoe[orgCounter] * coe3;
    }

    int getShanTenOthers(){
        vector<int> otherTiles;
        otherTiles.clear();
        otherTiles.clear();

        for(int i = 0; i < 34; ++i){
            for(int j = 0; j < other[i]; ++j){
                otherTiles.push_back(i);
            }
        }

        int n = otherTiles.size();

        if(n >= 5){
            //CheckHu

            bool hu = false;

            for(int pairs = 0; !hu && (pairs < (tripleFlushID ? 34 : 27)); ++pairs){
                if(other[pairs] >= 2){
                    other[pairs] -= 2;

                    if(extraSuits)hu = true;

                    else{

                        for(int start = 0; start < 34; ++start){
                            if(other[start] >= 3){hu = true; break;}
                            if(tileType[start] < 3 && tileOrd[start] < 7 && other[start] && other[start + 1] && other[start + 2]) {hu = true; break;}
                        }

                    other[pairs] += 2;
                    }
                }
            }

            if(hu)return -1;
        }

        // Find it in the table

        int best = 2;
        bool bestImproveTile[34];

        memset(bestImproveTile, 0, sizeof(bestImproveTile));

        int a[4], b[4];
        for(a[0] = 0; a[0] < n - 3; ++a[0])
            for(a[1] = a[0] + 1; a[1] < n - 2; ++a[1])
                for(a[2] = a[1] + 1; a[2] < n - 1; ++a[2])
                    for(a[3] = a[2] + 1; a[3] < n; ++a[3]){
                        b[0] = otherTiles[a[0]]; b[1] = otherTiles[a[1]]; b[2] = otherTiles[a[2]]; b[3] = otherTiles[a[3]];

                        int oid = getID(b);
                        int idx = idST[oid];
                        int cur = idx == -1 ? 2 : stData[idx].shanTen;

                        if(cur < best) {
                            best = cur;
                            memcpy(bestImproveTile, stData[idx].isImproveTiles, sizeof(bestImproveTile));
                        }

                        else if(cur == best && cur != 2){

                            for(int i = 0; i < 34; ++i){
                                bestImproveTile[i] |= stData[idx].isImproveTiles[i];
                            }
                        }
                    }

        memcpy(improveTiles, bestImproveTile, sizeof(bestImproveTile));
        return best;
    }
};

vector<limitFan> allLimitFans;

class Hands{
public:
    int tiles[34], remains[34];
    int shown[34], hidden[34];
    int tmp[34];

    static const int MAX_ROUND = 4096;
    static const int tripleFlushesFansNum = 6;

    HandElements he;
    allTripleFlushesFan tf;

    vector<tripleFanElement> tfe[6];

    vector<tripleFlushes> *tfPointers[tripleFlushesFansNum];
    vector<int> tfCounts[tripleFlushesFansNum];


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
        tfPointers[4] = &tf.yiSeSanBuGao;
        tfPointers[5] = &tf.yiSeSanTongShun;

        for(int i = 0; i < tripleFlushesFansNum; ++i){
            for(unsigned int j = 0; j < tfPointers[i] -> size(); ++j){
                tripleFanElement cur;
                tfe[i].push_back(cur);
            }
        }

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

    std::pair<int, int> getSingleMatchTiles(tripleFanElement &cur, const tripleFlushes &fan, int *lack){
        bool ok1, ok2, ok3;
        ok1 = ok2 = ok3 = false;

        int *otherTiles = cur.other;

        memcpy(otherTiles, hidden, sizeof(hidden));

        int matches = 0, m = showCards.size();

        for(int i = 0; i < m; ++i){
            if(showCards[i].first == 0)
                {
                    if(!ok1 && showCards[i].second == fan.cid1) { matches += (7 << 6); ok1 = true; }
                    else if(!ok2 && showCards[i].second == fan.cid2) { matches += (7 << 3); ok2 = true; }
                    else if(!ok3 && showCards[i].second == fan.cid3) { matches += (7 << 0); ok3 = true; }
                }
        }

        if(!ok1){
            int temRes1 = checkAndExcludeSingleFlush(otherTiles, fan.cid1, lack);
            matches += (temRes1 << 6);
        }

        if(!ok2){
            int temRes2 = checkAndExcludeSingleFlush(otherTiles, fan.cid2, lack);
            matches += (temRes2 << 3);
        }

        if(!ok3){
            int temRes3 = checkAndExcludeSingleFlush(otherTiles, fan.cid3, lack);
            matches += temRes3;
        }

        const int extraSuit = m - ok1 - ok2 - ok3;

        std::pair<int, int> res;
        res.first = extraSuit, res.second = matches;

        return res;
    }


    /**@
        fans[];  The normal
        res[]: the result contains all kinds of triple flushes fans.

        0. SanSeSanTongSHun, 7 types in total.
        1. SanSeSanBuGao, 30 types in total.
        2. QingLong, 3 types in total.
        3. Hualong, 6 types in total.
        4. YiSeSanBuGao, 36 types in total.
        5. YiSeSanTongShun, 7 types in total.
    **/

    void getMatchTiles(vector<tripleFlushes> *fans[], vector<tripleFanElement> res[]){
        int m = tripleFlushesFansNum;

        tripleFanElement cur;

        for(int i = 0; i < m; ++i){
            vector<tripleFlushes> &fan = *fans[i];

            int n = fan.size();
            for(int j = 0; j < n; ++j){
                tripleFlushes &k = fan[j];



                memcpy(cur.other, hidden, sizeof(hidden));
                memset(cur.lackMain, 0, sizeof(cur.lackMain));

                std::pair<int, int> val = getSingleMatchTiles(cur, k, cur.lackMain);

                cur.extraSuits = val.first;
                cur.orgScore = val.second;

                cur.orgCounter = countOnes(cur.orgScore);

                res[i][j] = cur;
            }
        }
    }

    void updateHE(){
        memset(he.pairs, 0, sizeof(he.pairs));
        memset(he.triples, 0, sizeof(he.triples));
        memset(he.flushes, 0, sizeof(he.flushes));

        he.singleF = he.singleJ = 0;


        for(auto it = showCards.begin(); it != showCards.end(); ++it){
            int type = it -> first, card = it -> second;

            if(type == 0){
                he.flushes[type][card % 9] = 2;
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

    double evaluatePengPengHu(){
        updateHE();

        int m = showCards.size();


        int triples, pairs, deadPairs;
        triples = pairs = deadPairs = 0;

        vector<int> pairsOrd, singlesOrd;

        pairsOrd.clear(); singlesOrd.clear();

        for(int i = 0; i < m; ++i){
            if(showCards[i].first == 0)return 0.0;
            else triples++;
        }



        for(int i = 0; i < 34; ++i){
            if(hidden[i] >= 3)triples++;
            else if(hidden[i] == 2){
                pairs++;
                pairsOrd.push_back(i);
                if(remains[i] == 0)deadPairs++;
            }

            else if(hidden[i] == 1){
                singlesOrd.push_back(i);
            }
        }

        int ps = pairsOrd.size(), ss = singlesOrd.size();

        int idx;
        if(triples == 4){// isReady
            idx = singlesOrd[0];
            return 500 * sqrt(remains[idx] / 3.00);
        }

        else if(triples == 3){
            if(pairs == 2){ // Ready, waiting for 2 tiles
                int readyTiles = 0;
                for(int j = 0; j < 2; ++j){
                    idx = pairsOrd[j];
                    readyTiles += remains[idx];
                }

                return 500 * readyTileCoe[readyTiles];
            }



            else if(pairs == 1){ //yiShanTen
                int base = 40.0;

                int v = 0;

                v += remains[pairsOrd[0]] * 4;

                for(int i = 0; i < ss; ++i){
                    int j = singlesOrd[i];
                    v += remains[j];
                }

                v = min(37, v);

                return base * yiShanTenCoe[v];
            }

            else { // No Pairs, can only draw card, 2 shan ten .


                int v = 0;
                for(int i = 0; i < ss; ++i){
                    int j = singlesOrd[i];
                    v += remains[j];
                }

                v = min(v, 51);

                return shanTenBase[2] * erShanTenCoe[v];
            }
        }

        else if(triples == 2){

            int realPairs = pairs - deadPairs;
            if(realPairs >= 2){
                int v = 0;

                for(int i = 0; i < ps; ++i){
                    int j = pairsOrd[i];
                    v += remains[j] * 4;
                }

                for(int i = 0; i < ss; ++i){
                    int j = singlesOrd[i];
                    v += remains[j];
                }

                v = min(v, 51);

                return shanTenBase[2] * erShanTenCoe[v];
            }

            else if(realPairs == 1)return 0.96;

            else return 0.2;

        }

        else if(triples == 1){ // One triples
            int realPairs = pairs - deadPairs;
            if(realPairs == 5)return 1.89;

            else if(pairs == 4)return 1.69;

            else if(pairs == 3)return 0.8;

            if(pairs <= 2)return 0.01;
        }

        else { // No triples
            if(pairs - deadPairs <= 3)return 0.0;
            else if(pairs - deadPairs <= 4)return 0.25;
            return 0.92;
        }

        return 0.0;
    }

    bool checkYiShanTen(){ // Remove x to y and get z that can be win. Here x != z.
        return false;
    }

    double calcValue(const char* fanName, int *cntsWin, int maxRound){

        int acc = 0;
        double accScore = 0.0;

        double ra = 1.0, decayRate = 0.85, accRate = 0.0;

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


    double evaluateSpecificLimitedFans(const string &fanName, int *limitTiles, int *origional ){

        return 0.0;
    }

    double evaluateQuanBuKao(){
        int round = MAX_ROUND;

        myDeck -> reset(1);

        const int maxRound = min(21, remainTiles);

        int cntsWin[maxRound];

        memcpy(tmp, hidden, sizeof(hidden));
        memset(cntsWin, 0, sizeof(cntsWin));

        if(countQuanBuKaoTiles() <= 9)return 0.0;



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

    int checkAndExcludeSingleFlush(int *arr, const int &cid, int *lack){
        int val = 0;

        if(arr[cid]){val += 4; arr[cid]--; }
        else lack[cid] += 1;
        if(arr[cid + 1]){val += 2; arr[cid + 1]--; }
        else lack[cid + 1] += 1;
        if(arr[cid + 2]){val += 1; arr[cid + 2]--; }
        else lack[cid + 2] += 1;

        return val;
    }

    int countOrgTiles(int *orgTiles, int cid1, int cid2, int cid3){
        if(cid1 == cid2 && cid2 == cid3)return orgTiles[cid1];


        return (orgTiles[cid1] > 0) + (orgTiles[cid2] > 0) + (orgTiles[cid3] > 0);
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

    void debugOutputTFE(){
        for(int i = 0; i < tripleFlushesFansNum; ++i){
            printf("%s\n", tripleFlushFansName[i]);

            for(unsigned int j = 0; j < tfe[i].size(); ++j){

                printf("%d %d %d %d\n", j, tfe[i][j].orgCounter, tfe[i][j].extraSuits, tfe[i][j].orgScore);
            }
        }
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

    /**
    This function is used to evaluate all triple flush function.

    */

    void evaluateNormal(vector<double> &values){
        int round = MAX_ROUND;

        //outputTenhouFormat();

        myDeck -> setTiles(remains);
        myDeck -> reset(1);


        const int maxRound = min(5, remainTiles);

        int cntsWin[maxRound + 1];

        int tmp[34];

        bool hasSuit[5], hasPair[5];

        getMatchTiles(tfPointers, tfe);

        //debugOutputTFE();

        memset(cntsWin, 0, sizeof(cntsWin));

        vector<double> g;
        // Evaluate all 6 triple flushes
        for(int i = 0; i < tripleFlushesFansNum; ++i){
            g.clear();
            for(int j = 0; j < tripleFlushFansSize[i]; ++j){
                tfe[i][j].setRemains(remains);
                double curValue = tfe[i][j].getScore();
                g.push_back(curValue);
            }

            sort(g.begin(), g.end(), greater<double>());

            double combinedValue = g[0] * 0.7 + g[1] * 0.2 + g[2] * 0.1;
            values.push_back(combinedValue);
            //printf("%s: %.4lf\n", tripleFlushFansName[i], combinedValue);
        }


        while(round--){
            memcpy(tmp, hidden, sizeof(hidden));

            int curRound = 0;

            evaluateWuMenQiInit(hasPair, hasSuit);

            while(!(myDeck -> isEmpty()) && curRound < maxRound){

                int cid = myDeck -> drawCard();

                if(cid == -1) break;

                hidden[cid]++;
                updateHE();

                if(evaluateWuMenQiSuits(hasPair, hasSuit, cid) == 5){
                    cntsWin[curRound] = 1;
                    break;
                }

                curRound++;
            }

            myDeck -> reset(1);

            memcpy(hidden, tmp, sizeof(tmp));
        }

        double wuMenQiValue = calcValue("WuMenQi", cntsWin, maxRound);

        values.push_back(wuMenQiValue);
    }

    double evaluateQiDuiZi(){

        if(showCards.size() > 0)return 0.00;

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
        values.push_back(evaluatePengPengHu());

        evaluateNormal(values);

        sort(values.begin(), values.end());

        int n = values.size();

        double curWeight = 1.4, decayRate = 0.33, acc = 0.0;
        double value = 0.0;
        for(int i = n - 1; i >= 0; --i){
            value += curWeight * values[i];
            acc += curWeight;
            curWeight *= decayRate;
        }

        return value / acc;
    }

    int playTile(double &maxValue){
        double bestValue = 0.0, bestChoice = -1;
        for(int i = 0; i < 34; ++i){
            if(hidden[i] > 0){



                hidden[i]--;

                double curValue = getWeightValue();

                hidden[i]++;

                #ifdef LOCAL
                printf("Play: %s\n", tileNameTenhou[i]);
                printf("WeightedValue: %.4lf\n" , curValue);
                printf("\n");
                #endif


                if(curValue > bestValue){
                    bestChoice = i;
                    bestValue = curValue;
                }
            }
        }

        maxValue = bestValue;

        return bestChoice;
    }

    int mingPai(int cid){

        int curValue = getWeightValue();

        int type = tileType[cid],  ord = tileOrd[cid];

        if(type < 3){ // canChi
            if(ord >= 2)return 0;
        }

        return curValue;
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
            int showTypeID = showCards[i].first;
            string showType = showTypeName[showTypeID];
            int cid = showCards[i].second;
            string showCardNames = showTypeID == 0 ? tileName[cid + 1] : tileName[cid];
            int cardFromID = 1;


            ans.push_back({showType, {showCardNames, cardFromID} });
        }

        return ans;
    }

    void debugOutput(){
        for(unsigned int i = 0; i < showCards.size(); ++i){
            printf("%d %d\n", showCards[i].first, showCards[i].second);
        }

        for(int i = 0; i < 34; ++i){
            for(int j = 0; j < hidden[i]; ++j){
                cout << tileName[i];
            }
        }
    }

    void outputTenhouFormat(){

        int m = showCards.size();
        for(int i = 0; i < m; ++i){
            printf("%d %d\n", showCards[i].first, showCards[i].second);
        }

        int cnt = 0;
        for(int i = 0; i < 34; ++i){
            for(int j = 0; j < hidden[i]; ++j){
                printf("%d", i % 9 + 1);
                cnt++;
            }

            if(cnt > 0 && (i % 9 == 8 || i == 33)){
                if(cnt > 0){
                    if(i == 8)printf("m");
                    if(i == 17)printf("s");
                    if(i == 26)printf("p");
                    if(i == 33)printf("z");
                    cnt = 0;
                }
            }
        }
        printf("\n");
    }
};

void loadSTData(const char *filename){
    FILE *fp = fopen(filename, "r");

    if(fp != NULL){
        char s[36];
        int id;

        for(int i = 0; i < MAX_LOAD_NUM; ++i){
            fscanf(fp, "%d", &id);
            fscanf(fp, "%d", &stData[i].shanTen);
            fscanf(fp, "%s", s);

            for(int j = 0; j < 34; ++j){
                stData[i].isImproveTiles[j] = s[j] == '1';
            }


            idST[id] = i;
        }
    }

    fclose(fp);
}

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

    memset(idST, -1, sizeof(idST));

    loadSTData("data/shanten4.dat");
}

int calcFan(Hands &myHand, int winCid, bool isSelfDraw){


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
    bool isZIMO = isSelfDraw;
    bool isJUEZHANG = false;
    bool isGANG = false;
    bool isLAST = false;

    int countFan = 0;

    try{
        auto re = MahjongFanCalculator(pack, hand, winTile, flowerCount, isZIMO, isJUEZHANG, isGANG, isLAST, menFeng, quanFeng);
        //auto re = MahjongFanCalculator({{"GANG",{"W1",1}},{"CHI",{"T2",2}}},{"W3","W3","W3","W4","W4","W4","W5"},"W5",1,0,0,0,0,0,0);

        for(auto i : re){
            countFan += i.first;
           // cout << i.first << " " << i.second << endl;
        }

        //printf("Total: %d Fans.\n", countFan);
    }

    catch(const string &error){
        //cout << error << endl;
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
        }

        else {


            //First 2 rounds
            int itmp, myPlayerID, quan;

            int lastTile = -1;

            ostringstream sout;
            istringstream sin;
            sin.str(request[0]);
            sin >> itmp >> myPlayerID >> quan;

            quanFeng = quan;
            menFeng = myPlayerID;

            sin.clear();
            sin.str(request[1]);

            for(int j = 0; j < 5; j++) sin >> itmp;
            for(int j = 0; j < 13; j++) {
                sin >> stmp;
                myHand.addTile(tileNameID[stmp]);
            }

            // Previous Rounds
            for(int i = 2; i < turnID; i++) {
                sin.clear();
                sin.str(request[i]);
                sin >> itmp;

                if(itmp == 2) { // Draw Card


                    myHand.remainTiles--;
                    sin >> stmp;
                    myHand.addTile(tileNameID[stmp]);
                    sin.clear();



                    sin.str(response[i]);
                    sin >> stmp >> stmp;

                    myHand.discardTile(tileNameID[stmp], true);
                    lastTile = tileNameID[stmp];
                }

                else if(itmp == 3){ //Other's player
                    stmp.clear();
                    string sid;
                    sin >> sid >> stmp;
                    int outCID = -1;

                    int playerID = sid[0] - '0';

                    if(stmp == "DRAW"){ // DRAW
                        lastTile = -1;
                        //Do nothing
                    }

                    else if(stmp == "PENG"){ //PENG
                        sin >> stmp;
                        outCID = tileNameID[stmp];

                        if(playerID == menFeng){
                            myHand.showCards.push_back({1, lastTile});
                            myHand.hidden[lastTile] -= 2;
                            myHand.discardTile(outCID, true);
                        }

                        else{
                            myHand.deductRemain(lastTile);
                            myHand.deductRemain(lastTile);
                        }

                        lastTile = outCID;
                    }

                    else if(stmp == "CHI"){ //CHI
                        sin >> stmp;

                        int midCID = tileNameID[stmp];

                        sin >> stmp;

                        outCID = tileNameID[stmp];

                        if(playerID == menFeng){
                            myHand.showCards.push_back({0, midCID - 1});
                            for(int k = midCID - 1; k <= midCID + 1; ++k)
                                if(k != lastTile)myHand.hidden[k]--;
                            myHand.discardTile(outCID, true);
                        }

                        else {
                            for(int k = midCID - 1; k <= midCID + 1; ++k)
                                if(k != lastTile)myHand.deductRemain(k);
                        }

                        lastTile = outCID;
                    }

                    else if(stmp == "PLAY"){ //PLAY
                        sin >> stmp;
                        outCID = tileNameID[stmp];
                        myHand.discardTile(outCID, false);

                        lastTile = outCID;
                    }
                    //printf("Round #%d: %d\n", i, lastTile);
                }
        }

        sin.clear();

        //Current Round

        sin.str(request[turnID]);
        sin >> itmp;

        if(itmp == 2) {  // Self Draw

            sout.clear();

            sin >> stmp;

            myHand.updateHE();

            #ifndef LOCAL
            int curFan = calcFan(myHand, tileNameID[stmp], true);
            #endif

            #ifdef LOCAL
            int curFan = 7;
            #endif

            if(curFan >= 8)sout << "HU";


            else{

                myHand.addTile(tileNameID[stmp]);

                double bestValue;

                myHand.updateHE();
                //myHand.outputTenhouFormat();
                int cid = myHand.playTile(bestValue);

                if(bestValue <= 0.35) {
                    for(int i = 27; i < 34; ++i){
                        if(myHand.hidden[i] == 1){
                            cid = i;
                            break;
                        }
                    }
                }

                sout << "PLAY " << tileName[cid];

                myHand.discardTile(cid, true);

                lastTile = cid;
            }
        }

        else if(itmp == 3){ //Other player discard a tile

            int outCID = -1;
            string outPlayerStr;
            int outPlayerID;

            sin >> outPlayerStr >> stmp;

            outPlayerID = outPlayerStr[0] - '0';

            //printf("%d %d\n", huCID, outCID);

            if(stmp == "PENG"){ //PENG
                sin >> stmp;

                outCID = tileNameID[stmp];

                if(lastTile != -1){
                    myHand.deductRemain(lastTile);
                    myHand.deductRemain(lastTile);
                }

                myHand.deductRemain(outCID);
            }

            else if(stmp == "CHI"){ //CHI
                sin >> stmp;

                int midCID = tileNameID[stmp];

                sin >> stmp;

                outCID = tileNameID[stmp];

                for(int k = midCID - 1; k <= midCID + 1; ++k)
                    if(k != lastTile)myHand.deductRemain(k);

                myHand.deductRemain(outCID);
            }

            else if(stmp == "PLAY"){ //PLAY

                sin >> stmp;
                outCID = tileNameID[stmp];

                myHand.deductRemain(outCID);
            }

            if(outCID != -1){
                lastTile = outCID;


                #ifndef LOCAL
                int curFan = calcFan(myHand, outCID, false);
                #endif

                #ifdef LOCAL
                int curFan = 7;
                #endif

                if(curFan >= 8)sout << "HU";
                else {

                    int &x = outCID;

                    int outType = tileType[x], outOrd = tileOrd[x];


                    double curValue = myHand.getWeightValue();


                    double bestValue = curValue;
                    int bestChoice = -1, arg1 = 0, arg2 = 0;

                    Hands tmpHand = myHand;

                    Hands bestTmpHand;

                    double tmpValue, playCID;

                    if(outType < 3 && (outPlayerID - menFeng + 4) % 4 == 3){
                        //Can chow

                        // Chow x - 2, x - 1, x

                        if(outOrd >= 2 && tmpHand.hidden[x - 2] && tmpHand.hidden[x - 1]){
                            tmpHand.showCards.push_back({0, x - 2});
                            tmpHand.hidden[x - 2]--;
                            tmpHand.hidden[x - 1]--;

                            playCID = tmpHand.playTile(tmpValue);

                            if(tmpValue > bestValue && playCID != x){
                                bestTmpHand = tmpHand;
                                bestValue = tmpValue;
                                bestChoice = 0;
                                arg1 = x - 1;
                                arg2 = playCID;
                            }

                            tmpHand.showCards.pop_back();
                            tmpHand.showCards.shrink_to_fit();
                            tmpHand.hidden[x - 2]++;
                            tmpHand.hidden[x - 1]++;


                        }

                        if(outOrd >= 1 && outOrd < 8 && tmpHand.hidden[x - 1] && tmpHand.hidden[x + 1]){
                            tmpHand.showCards.push_back({0, x - 1});
                            tmpHand.hidden[x - 1]--;
                            tmpHand.hidden[x + 1]--;

                            int playCID = tmpHand.playTile(tmpValue);

                            if(tmpValue > bestValue && playCID != x){
                                bestTmpHand = tmpHand;
                                bestValue = tmpValue;
                                bestChoice = 0;
                                arg1 = x;
                                arg2 = playCID;
                            }

                            tmpHand.showCards.pop_back();
                            tmpHand.showCards.shrink_to_fit();
                            tmpHand.hidden[x - 1]++;
                            tmpHand.hidden[x + 1]++;


                        }

                        if(outOrd < 7 && tmpHand.hidden[x + 1] && tmpHand.hidden[x + 2]){
                            tmpHand.showCards.push_back({0, x});
                            tmpHand.hidden[x + 2]--;
                            tmpHand.hidden[x + 1]--;

                            int playCID = tmpHand.playTile(tmpValue);

                            if(tmpValue > bestValue && playCID != x){
                                bestTmpHand = tmpHand;
                                bestValue = tmpValue;
                                bestChoice = 0;
                                arg1 = x + 1;
                                arg2 = playCID;
                            }

                            tmpHand.showCards.pop_back();
                            tmpHand.showCards.shrink_to_fit();
                            tmpHand.hidden[x + 2]++;
                            tmpHand.hidden[x + 1]++;


                        }
                    }

                    if(outPlayerID != menFeng && tmpHand.hidden[x] >= 2){
                        //Can Pung

                        tmpHand.hidden[x] -= 2;
                        tmpHand.showCards.push_back({1, x});

                        int playCID = tmpHand.playTile(tmpValue);

                        if(tmpValue > bestValue && playCID != x){
                            bestTmpHand = tmpHand;
                            bestValue = tmpValue;
                            bestChoice = 1;
                            arg1 = x;
                            arg2 = playCID;
                        }

                        tmpHand.showCards.pop_back();
                        tmpHand.showCards.shrink_to_fit();
                        tmpHand.hidden[x] += 2;


                    }


                    if(bestChoice != -1 && bestValue > 0.7500 && bestValue / curValue >= 7.5){

                        if(bestChoice == 0) sout << "CHI" << " " << tileName[arg1] << " " << tileName[arg2];
                        else if(bestChoice == 1) sout << "PENG" << " " << tileName[arg2];

                        myHand = bestTmpHand;
                    }

                    else{
                        sout << "PASS";
                    }
                }
            }
            else sout << "PASS";
        }

        else{
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
    //freopen("test.txt", "r", stdin);

    init();



    Hands myHand;
    CardDeck myDeck;

    myHand.setDeck(myDeck);
    myDeck.randomShuffle();

    myDeck.setTiles(myHand.remains);

    myGamePlay(myHand);

    //myHand.debugOutput();

    return 0;
}
