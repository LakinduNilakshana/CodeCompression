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
bool hit;
vector<pair<string, pair<int,int>>> comp;
string dictionDec[8];

void readFromFile(string fileName);
bool myCompare(pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2);
string decToBinary(int n, int l);
// void printStrArray(string arr[], int n);
void sortByFrequency();
string rleEncode(int occurance);
string directMatch(int ind);
void oneBitMismatch(int bitIndex, int dicIndex);
void twoBitMismatch(int bitIndex, int dicIndex);
void bitMask(int bitIndex, int dicIndex);
bool compareBest(pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2);
string findBest();
void writeCompressed(string str);
string readCompressed(string fileName);
void decodeLines(string input);

int main(){
    readFromFile("original.txt");
    vector <string> compressed;
    // string compressed[dCount];
    sortByFrequency();

    for (int i = 0; i < dCount; i++){
        comp.clear();
        currentLine = binCode[i];
        hit = false;
        if (prevLine == currentLine && equalCount<4){
            equalCount++;
            prevLine = currentLine;
            continue;   //since RLE compresses the line into 5 bits, it gives the least compression ratio
        } 
        // compressed[i] = rleEncode(equalCount);
        if (equalCount!=0)
            compressed.push_back(rleEncode(equalCount-1));
        equalCount = 0;

        for (int j=0; j<8; j++){
            if (diction[j] == currentLine){
                // compressed[i] = directMatch(j);
                compressed.push_back(directMatch(j));
                hit = true;
                prevLine = currentLine;
                break;  //since direct match compresses the line into 6 bits, it gives the second lowest compression ratio
            }
        }
        if (hit)
            continue;  //since direct match compresses the line into 6 bits, it gives the second lowest compression ratio
        for (int j=0; j<8; j++){
            oneBitMismatch(i,j);
            twoBitMismatch(i,j);
            bitMask(i,j);
        }
        
        if (hit){
            // compressed[i] = findBest();
            compressed.push_back(findBest());
        } else {
            // compressed[i] = "110" + binCode[i];
            compressed.push_back("110" + binCode[i]);
        }
        prevLine = currentLine;

    }
    string output;
    for (int z=0; z<compressed.size(); z++){
        // cout << compressed[z] << endl;
        output += compressed[z];
    }
    writeCompressed(output);
    string compressedInput = readCompressed("compressed.txt");
    decodeLines(compressedInput);
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
        // cout << v[i].first << " " << v[i].second.first << " " << v[i].second.second << endl;
    }
}

string rleEncode(int occurance){
    return "000" + decToBinary(occurance,2);
}

string directMatch(int ind){
    return "101" + decToBinary(ind,3);
}

void oneBitMismatch(int bitIndex, int dicIndex){
    bitset<32> dataLine(binCode[bitIndex]);
    bitset<32> dictLine(diction[dicIndex]);
    dictLine ^= dataLine;
    if (dictLine.count() == 1){
        for (int x=0; x<32; x++){
            if (dictLine.test(x)){
                string out = "010" + decToBinary(31-x,5) + decToBinary (dicIndex,3);
                comp.push_back(make_pair(out,make_pair(11,2)));
                // cout << bitIndex + 1 << " : " << "1 Bit : " << out << endl;
                break;
            }
        }
        hit = true;
    }
}

void twoBitMismatch(int bitIndex, int dicIndex){
    bitset<32> dataLine(binCode[bitIndex]);
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
        for (int x=firstPos+1; x<32; x++){
            if (dictLine.test(x)){
                secondPos = x;
                break;
            }
        }

        if (secondPos-firstPos == 1){
            out = "011" + decToBinary(31-secondPos,5) + decToBinary (dicIndex,3);
            comp.push_back(make_pair(out,make_pair(11,3)));
            // cout << bitIndex + 1 << " : " << "2 Bit Con : " << out << endl;
        }
        else{
            out = "100" + decToBinary(31-secondPos,5) + decToBinary(31-firstPos,5) + decToBinary (dicIndex,3);
            comp.push_back(make_pair(out,make_pair(16,4)));
            // cout << bitIndex + 1 << " : " << "2 Bit Any : " << out << endl;
        }
        hit = true;
    }
}

void bitMask(int bitIndex, int dicIndex){
    bitset<32> dataLine(binCode[bitIndex]);
    bitset<32> dictLine(diction[dicIndex]);
    bitset<4> mask;
    dictLine ^= dataLine;
    // Using bitmask technique for 1 bit mismatch is not optimum
    if(dictLine.count()<=4 && dictLine.count()>1){
        for (int x=31; x>2; x--){
            if (dictLine.test(x)){
                for (int i=0; i<4; i++){
                    mask.set(3-i,dictLine[x-i]);
                }
                if(mask.count()==dictLine.count()){
                    string out = "001" + decToBinary(31-x,5) + mask.to_string() + decToBinary(dicIndex,3);
                    comp.push_back(make_pair(out,make_pair(15,1)));
                    // cout << bitIndex + 1 << " : " << "BitMask : " << out << endl;
                    hit = true;
                }
                break;
            }
        }
    }
}

bool compareBest(pair<string, pair<int,int>> p1,pair<string, pair<int,int>> p2){
    if (p1.second.first != p2.second.first){
        return (p1.second.first < p2.second.first);
    } 
    else{
        return (p1.second.second < p2.second.second);
    }
}

string findBest(){
    sort(comp.begin(),comp.end(),compareBest);
    // cout << comp[0].first << endl;
    return comp[0].first;
}

void writeCompressed(string str){
    ofstream MyFile("cout.txt");
    int lineN = str.length()/32+1;
    for (int line=0; line < lineN-1; line++){
        // cout << str.substr(32*line,32) << endl;
        MyFile << str.substr(32*line,32) << endl;
    }
    int bitsLeft = lineN*32-str.length();
    for (int i =0; i<bitsLeft;i++){
        str+="1";
    }
    // cout << str.substr(32*(lineN-1),32)<< endl;
    MyFile << str.substr(32*(lineN-1),32)<< endl;
    MyFile << "xxxx" <<endl;
    for (int i=0; i<7; i++){
        MyFile << diction[i] << endl;
    }
    MyFile << diction[7] ;
    MyFile.close();
}

string readCompressed(string fileName){
    string inp;
    string text;
    ifstream File(fileName);
    while(getline(File,text)&& text!="xxxx"){
        inp += text;
        // cout << text <<endl;
    }
    cout << inp << endl;
    int i=0;
    while(getline(File,text)){
        dictionDec[i]= text;
        i++;
        // cout << text << endl;
    }
    File.close();
    return inp;
}

void decodeLines(string input){
    int pos = 0;
    while (pos < input.length()){
        bitset<3> ind (input.substr(pos,3));
        int indicator = (int) (ind.to_ulong());
        switch (indicator){
            case 0:
                cout << "000" << input.substr(pos+3,2) << endl;
                pos+=5;
                break;
            case 1:
                cout << "001" << input.substr(pos+3,12) << endl;
                pos+=15;
                break;
            case 2:
                cout << "010" << input.substr(pos+3,8) << endl;
                pos+=11;
                break;
            case 3:
                cout << "011" << input.substr(pos+3,8) << endl;
                pos+=11;
                break;
            case 4:
                cout << "100" << input.substr(pos+3,13) << endl;
                pos+=16;
                break;
            case 5:
                cout << "101" << input.substr(pos+3,3) << endl;
                pos+=6;
                break;
            case 6:
                cout << "110" << input.substr(pos+3,32) << endl;
                pos+=35;
                break;
            default:
                cout << "111" << "Default" << endl;
                pos+=32;
                break;
        }
    }
}

