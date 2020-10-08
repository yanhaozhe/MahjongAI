class CardDeck{
public:

    static const int MAX_CARD = 136;

    bool vis[MAX_CARD];
    int deck[MAX_CARD];

    CardDeck(){
        srand(time(NULL));
        for(int i = 0; i < MAX_CARD; ++i){
            vis[i] = false;
            deck[i] = i;
        }
    }

    void randomShuffle(){
    }

    //The card number from 0 to 135, the card no is defined by card // 4.


