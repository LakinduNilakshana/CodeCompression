#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int dCount = 0;
string binCode[128];
string diction[8];
string compressed[dCount];
string prevLine, currentLine;
int equalCount = 0;

void readFromFile(string fileName);
bool myCompare(pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2);
string decToBinary(int n, int l);
// void printStrArray(string arr[], int n);
void sortByFrequency();
string rleEncode(int occurance);
string directMatch(int ind);

int main(){
    readFromFile("original.txt");
    sortByFrequency();
    for (int i = 0; i < dCount; i++){
        currentLine = binCode[i];
        if (prevLine == currentLine && equalCount<5){
            compressed[i] = rleEncode(equalCount);
            equalCount++;
            prevLine = currentLine;
            cout << i+1 << " ";
            cout << compressed[i] << endl;
            continue;
        } 
        equalCount = 0;
        bool hit = false;
        for (int j=0; j<8; j++){
            if (diction[j] == currentLine){
                compressed[i] = directMatch(j);
                hit = true;
                break;
            }
        }
        if (!hit)
            compressed[i] = "110" + binCode[i];
        prevLine = currentLine;
        cout << i+1 << " ";
        cout << compressed[i] << endl;
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
    cout << "Dictionary;" << endl;
    for (int i=0; i<8; i++){
        diction[i] = v[i].first;
        cout << v[i].first << " ";
        cout << v[i].second.first << " ";
        cout << v[i].second.second << endl;
        
    }
    cout << "Compressed : " << endl;
    // printStrArray(diction,8);
}

string rleEncode(int occurance){
    return "000" + decToBinary(occurance,2);
}

string directMatch(int ind){
    return "101" + decToBinary(ind,3);
}