#include<bits/stdc++.h>
#include "CardDeck.h"
using namespace std;

class Hands{
public:
    int tiles[34], remains[34];
    int shown[34], hidden[34];
    vector<pair<int, int> > showCards; // 0: Chow 1: Pung 2: Kung 3: AnKung

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
            hidden[i] =
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
        for(int i = 0; i < 34; ++i){
            if(limitaions[i] == true;)
        }
    }

    double evaluateAnyChow(){
    }

    double evaluateAnyPung(){
    }

    double evaluatePairs(int cid){

    }

    double evaluateChow(int cid){

    }

    bool hasChow(int cid){
        for(auto i = showCards.begin(); i != showCards.end(); ++i){
            int type
        }
    }

    bool hasPung(int cid){
    }

    bool hasPair(int cid){

    }

    double evaluateWuMenQi(){

        int round = 1000;
        const int maxRound = 21;

        while(round--){
            CardDeck deck;


            while(!deck.isEmpty()){
                int cid = deck.drawCard();

            }
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



int main()
{
    char s[32];
    scanf("%s", s);

    Hands myHand(s);
    CardDeck myDeck;

    myHand.setDeck(myDeck);

    myDeck.randomShuffle();
    myDeck.addTile(myHand.tiles);

   // myHand.


    return 0;
}
