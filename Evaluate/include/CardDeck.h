#ifndef CARDDECK_H
#define CARDDECK_H


class CardDeck
{
    public:
        static const int MAX_CARD = 136;
        bool vis[MAX_CARD];
        int deck[MAX_CARD], p;

        CardDeck();

        bool isEmpty();
        int drawCard();
        void reset(int type);
        void addTile(int *tiles);
        void mark(int tid);
        void randomShuffle();

        virtual ~CardDeck();


    protected:

    private:
};

#endif // CARDDECK_H
