#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int dCount = 0;
string binCode[128];
string diction[8];

void readFromFile(string fileName);

bool myCompare( pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2){
    if (p1.second.second != p2.second.second){
        return (p1.second.second > p2.second.second);
    } 
    else{
        return (p1.second.first < p2.second.first);
    }
}

void sortByFrequency(string arr[]);

int main(){
    readFromFile("original.txt");
    sortByFrequency(binCode);

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

void sortByFrequency(string arr[]){
    unordered_map<string, pair<int,int>> map;
    for (int i=0; i<dCount; i++){
        if (map.find(arr[i]) != map.end()) {  //if element already in hash map increase count by 1
            map[arr[i]].second++;
        }
        else {                                //if element not in hash map, add element with count 1 and first apearance
            map[arr[i]] = make_pair(i,1);
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
}