#include"cache.hpp"
#include<iostream>
#include<fstream>
using namespace std;
int main(){
//main will read input file one by one, and will increment cacheHit, cacheMiss, and WriteBack counters based on scenarios.
Cache c;
ifstream file;
file.open("input.txt");
char op;
int addr=0;
while(file>>op>>hex>>addr){
        cout<<op<<addr<<"\n";
        if(op=='R')int x=c.read(addr);
        else c.write(addr);
}
cout<<"hits = "<<c.gethits()<<"\t"<<"misses = "<<c.getmisses()<<"\t"<<"wb = "<<c.getwb()<<endl;
return 0;
}
