#include "CardDeck.h"
#include<bits/stdc++.h>
using namespace std;

CardDeck::CardDeck()
{
    p = 0;
    srand(time(0));
    for(int i = 0; i < MAX_CARD; ++i){
        vis[i] = false;
        deck[i] = i;
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

void CardDeck::addTile(int *tiles){
    for(int i = 0; i < 34; ++i){
        for(int j = 0; j < tiles[i]; ++j){
            mark(i);
        }
    }
}

int CardDeck::drawCard(){
    while(p < MAX_CARD && vis[p])++p;
    if(p < MAX_CARD)return p;
    return -1;
}

void CardDeck::randomShuffle(){
    random_shuffle(deck, deck + MAX_CARD);
    for(int i = 0; i < MAX_CARD; ++i){
        printf("%d ", deck[i]);
    }
    printf("\n");
}

CardDeck::~CardDeck()
{
    //dtor
}
