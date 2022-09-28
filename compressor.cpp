#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int count = 0;
string binCode[128];
void readFromFile(string fileName);

int main(){
    readFromFile("original.txt");
    for (int i=0; i<count; i++){
        cout<<binCode[i]<<endl;
    }

}

void readFromFile(string fileName){
    string text;
    ifstream File(fileName);
    while(getline(File,text)){
        binCode[count]=text;
        count++;
        cout << text <<endl;
    }
    File.close();
}