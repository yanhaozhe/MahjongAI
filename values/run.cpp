#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#define _BOTZONE_ONLINE

#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include <json/json.h>
#endif

#define SIMPLEIO 0
//由玩家自己定义，0表示JSON交互，1表示简单交互。

using namespace std;

vector<string> request, response;
vector<string> hand;

const int TOTAL_TILES = 34;

const int values[TOTAL_TILES] = {14,11,9,8,7,8,9,11,14,14,11,9,8,7,8,9,11,14,14,11,9,8,7,8,9,11,14,15,15,15,15,15,15,15};

const string names[TOTAL_TILES] = {"W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8", "W9",
							 "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9",
							 "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9",
							 "F1", "F2", "F3", "F4",
							 "J1", "J2", "J3"
							};

int tiles[TOTAL_TILES]; // Use 34-dimensional vector to describe the tiles holds.
int remain[TOTAL_TILES];
int pairs = 0;

int tileToInt(string str){

	char ch = str[0];
	int ord = str[1] - '1';
	int res = 0;

	switch(ch){
		case 'W': {res = ord; break;}
		case 'B': {res = ord + 9; break;}
		case 'T': {res = ord + 18; break;}
		case 'F': {res = ord + 27; break;}
		case 'J': {res = ord + 31; break;}
		default: {res = -1;}
	}

	return res;
}

string intTotile(int cid){
	if(cid >= 0 && cid < TOTAL_TILES)return names[cid];
	return "";
}

void debug_outRemains(){
	printf("Current Remaining tile: \n");
	for(int i = 0; i < TOTAL_TILES; ++i){
		printf("%d ", remain[i]);
	}

	printf("\n");
}

void myUpdate(int &pairs){
	pairs = 0;
	for(int i = 0; i < 34; ++i){
		pairs += tiles[i] / 2;
	}
}

void addtile(string str){
	int cid = tileToInt(str);
	++tiles[cid]; --remain[cid];

	myUpdate(pairs);
}


void removetile(string str){
	int cid = tileToInt(str);
	--tiles[cid];
}

int playTile(){

	int worstTile, worstPoint;
	worstTile = worstPoint = -1;

	for(int i = 0; i < TOTAL_TILES; ++i){
		if(tiles[i] % 2 == 1){
			int cur = values[i] * remain[i];
			if(worstPoint == -1 || cur < worstPoint){
				worstTile = i; worstPoint = cur;
			}
		}
	}

	return worstTile;
}

int main(){
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

		for(int j = 0; j < TOTAL_TILES; ++j){
			tiles[j] = 0; remain[j] = 4;
		}

        sin.str(request[1]);

        for(int j = 0; j < 5; j++) sin >> itmp;
        for(int j = 0; j < 13; j++) {
            sin >> stmp;

            hand.push_back(stmp);

			addtile(stmp);
        }

        for(int i = 2; i < turnID; i++) { // Exe
            sin.clear();
            sin.str(request[i]);
            sin >> itmp;
            if(itmp == 2) {
                sin >> stmp;
                hand.push_back(stmp);

				addtile(stmp);

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

		//outRemains();

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

            /*random_shuffle(hand.begin(), hand.end());
            sout << "PLAY " << *hand.rbegin();
            hand.pop_back();*/

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
    return 0;
}