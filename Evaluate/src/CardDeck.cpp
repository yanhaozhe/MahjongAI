#include "CardDeck.h"
#include<bits/stdc++.h>
using namespace std;

CardDeck::CardDeck()
{
    deck.clear();
    p = 0;
    srand(time(0));
    for(int i = 0; i < MAX_CARD; ++i){
        vis[i] = false;
    }
}

bool CardDeck::isEmpty(){
    return p > MAX_CARD;
}

void CardDeck::reset(int type){
    p = 0;
    if(type == 1)randomShuffle();
}


void CardDeck::mark(int tid){
    int base = tid * 4;
    for(int offset = 0; offset < 4; ++offset){
        if(!vis[base + offset]){
            vis[base + offset] = true;
            break;
        }
    }
}

void CardDeck::setTiles(int *remainTiles){
    for(int i = 0; i < 34; ++i){
        for(int j = 0; j < remainTiles[i]; ++j){
            deck.push_back(i * 4 + j);
        }
    }
}

int CardDeck::drawCard(){
    int n = deck.size();
    while(p < n && vis[deck[p]])++p;
    if(p < n){++p; return deck[p] / 4;}
    return -1;
}

void CardDeck::randomShuffle(){
    random_shuffle(deck.begin(), deck.end());
}

CardDeck::~CardDeck()
{
    //dtor
}
