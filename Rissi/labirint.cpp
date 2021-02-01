#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <iostream>

using namespace std;

int solve(vector<vector<char>>& arr, int x, int y){
    bool foundExit = false;
    int ret = 0;

    int rows = arr.size();
    int cols = arr[0].size();

    int dirX[4] = {1, 0, -1, 0};
    int dirY[4] = {0, 1, 0, -1};

    //priprema polja za spremanje udaljenosti
    vector<vector<int>> distances;
    distances.reserve(rows);
    for(int i = 0; i < rows; i++){
        vector<int> distRow;
        distRow.reserve(cols);
        for(int j = 0; j < cols; j++){
            distRow.push_back(-1);
        }
        distances.push_back(distRow);
    }

    int currX, currY;
    queue<pair<int, int>> bfsQ;
    set<char> keys;
    bfsQ.push(pair<int, int>(x, y));


    while(!foundExit){
        distances[bfsQ.front().first][bfsQ.front().second] = 0; //pocetna tocka
        bool foundKey = false;

        do{
            currX = bfsQ.front().first;
            currY = bfsQ.front().second;    //prvi koji ceka u redu
            bfsQ.pop();

            char currChar = arr[currX][currY];
            if(currX == 0 || currX == rows - 1 || currY == 0 || currY == cols - 1){ //provjera rubnog uvjeta (izlaza)
                foundExit = true;
                while(!bfsQ.empty())
                    bfsQ.pop();
                bfsQ.push(pair<int, int>(currX, currY));
                break;
            } else if (currChar >= 'a' && arr[currX][currY] <= 'z' && keys.find(currChar) == keys.end()){   //provjera jesmo li stali na kljuc
                    keys.insert(currChar);
                    foundKey = true;
                    while(!bfsQ.empty())
                        bfsQ.pop();
                    bfsQ.push(pair<int, int>(currX, currY));
                    break;
                }

            for(int i = 0; i < 4; i++){     //potrebno proci sva 4 smjera (desno, dolje, lijevo, gore), osigurano pomocu dirX i dirY
                int nextX = currX + dirX[i];
                int nextY = currY + dirY[i];
                char nextChar = arr[nextX][nextY];


                if(nextChar == '#'){    //ako je susjedni znak zid, ne poduzimaj nista
                    continue;
                } else if (nextChar == ' ' || 
                        (nextChar >= 'A' && nextChar <= 'Z' && keys.find(nextChar - 'A' + 'a') != keys.end()) ||
                        (nextChar >= 'a' && nextChar <= 'z')){      //ako je susjedni znak praznina, malo ili veliko slovo abecede
                    
                    if(distances[nextX][nextY] == -1 || distances[nextX][nextY] > distances[currX][currY] + 1){ //ako susjed nije posjecen ili je posjecen duzim putem
                        distances[nextX][nextY] = distances[currX][currY] + 1;      //udaljenost susjeda od pocetne tocke je za jedan veca od udaljenosti trenutnog od pocetne tocke
                        bfsQ.push(pair<int, int>(nextX, nextY));        //dodavanje susjeda u red
                    }
                }
            }

            if(foundKey || foundExit){
                break;
            }
            
        } while(!foundKey && !foundExit);
        
        ret += distances[bfsQ.front().first][bfsQ.front().second];  //uvecavanje povratne vrijednosti za broj koraka od pocetne tocke do kljuca/izlaza

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                distances[i][j] = -1;           //ciscenje polja za spremanje udaljenosti
            }
        }
    }


    return ret;
}

vector<string> split(const string& str, const string& delimiter){
    vector<string> tokens;
    size_t previous = 0, position;
    int len = str.length();
    do{
        position = str.find(delimiter, previous);
        if (position == string::npos)
            position = str.length();
        string token = str.substr(previous, position - previous);
        if(!token.empty())
            tokens.push_back(token);
        previous = position + delimiter.length();
    } while (position < len && previous < len);
    return tokens;
}

int main(){
    string inputPath = "data/input5.txt";
    string outputPath = "data/output5.txt";

    ifstream input(inputPath);
    string line;
    vector<vector<char>> arr;
    int x, y;
    int lineSize = -1;
    if(input){
        getline(input, line);                           //prva linija ulaza predstavlja pocetne koordinate
        vector<string> firstRow = split(line, " ");
        x = stoi(firstRow[0]);
        y = stoi(firstRow[1]);
        while(getline(input, line)){                    //sve ostale linije predstavljaju labirint
            if(lineSize == -1)
                lineSize = line.size();
            arr.emplace_back(line.begin(), line.end());
            if(lineSize != line.size()){
                cerr << "Malformed input." << endl;
                exit(1);
            }
        }
    } else {
        cerr << "File does not exist." << endl;
        exit(2);
    }
    input.close();

    ofstream output(outputPath);
    
    string res = to_string(solve(arr, x, y));
    output.write(res.c_str(), res.size());
    output.close();
    return 0;
}