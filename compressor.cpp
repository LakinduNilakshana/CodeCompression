#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int dCount = 0;
string binCode[128];
string diction[8];
string prevLine, currentLine;
int equalCount = 0;

void readFromFile(string fileName);
bool myCompare(pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2);
string decToBinary(int n, int l);
// void printStrArray(string arr[], int n);
void sortByFrequency();
string rleEncode(int occurance);
string directMatch(int ind);
void oneBitMismatch(vector<pair<string, pair<int,int>>> v, int dicIndex, string currentLine, bool flag);

int main(){
    readFromFile("original.txt");
    string compressed[dCount];
    sortByFrequency();

    for (int i = 0; i < dCount; i++){
        currentLine = binCode[i];
        bool hit = false;
        if (prevLine == currentLine && equalCount<4){
            compressed[i] = rleEncode(equalCount);
            equalCount++;
            prevLine = currentLine;
            continue;   //since RLE compresses the line into 5 bits, it gives the least compression ratio
        } 
        equalCount = 0;

        for (int j=0; j<8; j++){
            if (diction[j] == currentLine){
                compressed[i] = directMatch(j);
                hit = true;
                prevLine = currentLine;
                break;  //since direct match compresses the line into 6 bits, it gives the second lowest compression ratio
            }
        }
        
        if (hit)
            continue;  //since direct match compresses the line into 6 bits, it gives the second lowest compression ratio
        vector<pair<string, pair<int,int>>> comp;

        for (int j=0; j<8; j++){
            oneBitMismatch(comp,j,currentLine,hit);
            twoBitMismatch(comp,j,currentLine,hit);
        }
        compressed[i] = "110" + binCode[i];
        prevLine = currentLine;
    }
    // printStrArray(compressed,dCount);
}

void readFromFile(string fileName){
    string text;
    ifstream File(fileName);
    while(getline(File,text)){
        binCode[dCount]=text;
        dCount++;
        // cout << text <<endl;
    }
    File.close();
}

bool myCompare(pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2){
    if (p1.second.second != p2.second.second){
        return (p1.second.second > p2.second.second);
    } 
    else{
        return (p1.second.first < p2.second.first);
    }
}

string decToBinary(int n, int l){
    string bin;
    for (int i = l-1; i >= 0; i--) {
        int k = n >> i;
        if (k & 1)
            bin += "1";
        else
            bin += "0";
    }
    return bin;
}

// void printStrArray(string arr[], int n){
//     for (int i=0; i<n; i++) {
//         cout << arr[i] << endl;
//     }
// }

void sortByFrequency(){
    unordered_map<string, pair<int,int>> map;
    for (int i=0; i<dCount; i++){
        if (map.find(binCode[i]) != map.end()) {  //if element already in hash map increase count by 1
            map[binCode[i]].second++;
        }
        else {                                //if element not in hash map, add element with count 1 and first apearance
            map[binCode[i]] = make_pair(i,1);
        }
    }
    auto it = map.begin();

    vector<pair<string, pair<int,int>>> v;
    for (it; it != map.end(); ++it)
        v.push_back(make_pair(it->first, it->second));
    sort(v.begin(), v.end(), myCompare);
    // cout << "Dictionary;" << endl;
    for (int i=0; i<8; i++){
        diction[i] = v[i].first;
        // cout << v[i].first << " ";
        // cout << v[i].second.first << " ";
        // cout << v[i].second.second << endl;
    }
}

string rleEncode(int occurance){
    return "000" + decToBinary(occurance,2);
}

string directMatch(int ind){
    return "101" + decToBinary(ind,3);
}

void oneBitMismatch(vector<pair<string, pair<int,int>>> v1, int dicIndex, string currentLine, bool flag){
    bitset<32> dataLine(currentLine);
    bitset<32> dictLine(diction[dicIndex]);
    dictLine ^= dataLine;
    if (dictLine.count() == 1){
        for (int x=0; x<32; x++){
            if (dictLine.test(x)){
                string out = "010" + decToBinary(31-x,5) + decToBinary (dicIndex,3);
                v1.push_back(make_pair(out,make_pair(out.size(),2)));
                break;
            }
        }
        flag = true;
    }
}

void twoBitMismatch(vector<pair<string, pair<int,int>>> v1, int dicIndex, string currentLine, bool flag){
    bitset<32> dataLine(currentLine);
    bitset<32> dictLine(diction[dicIndex]);
    dictLine ^= dataLine;
    if (dictLine.count() == 2){
        int firstPos, secondPos;
        string out;
        for (int x=0; x<32; x++){
            if (dictLine.test(x)){
                firstPos = x;
                break;
            }
        }
        for (int x=firstPos; x<32; x++){
            if (dictLine.test(x)){
                secondPos = x;
                break;
            }
        }
        if (secondPos-firstPos == 1){
            out = "011" + decToBinary(31-firstPos,5) + decToBinary (dicIndex,3);
            v1.push_back(make_pair(out,make_pair(out.size(),3)));
        }
        else{
            out = "100" + decToBinary(31-firstPos,5) + decToBinary(31-secondPos,5) + decToBinary (dicIndex,3);
            v1.push_back(make_pair(out,make_pair(out.size(),4)));
        }
        flag = true;
    }
}
