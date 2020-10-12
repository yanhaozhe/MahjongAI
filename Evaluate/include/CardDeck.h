#ifndef CARDDECK_H
#define CARDDECK_H
#include <vector>

class CardDeck
{
    public:
        static const int MAX_CARD = 136, MAX_TYPE = 34;
        bool vis[MAX_CARD];
        int p;
        std::vector<int> deck;

        CardDeck();

        bool isEmpty();
        int drawCard();
        void reset(int type);
        void setTiles(int *remainTiles);
        void mark(int tid);
        void randomShuffle();

        virtual ~CardDeck();


    protected:

    private:
};

#endif // CARDDECK_H
