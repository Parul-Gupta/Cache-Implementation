#ifndef CACHE_HPP
#define CACHE_HPP
#include <iostream>
#include <cmath>
#define BLOCK_SIZE 32   //in bytes
#define CACHE_SIZE 16  //in KB
#define WAY 1
using namespace std;
class Block {
    int tag;
    bool dirty;
    int lru;
    bool valid;
    int v[BLOCK_SIZE/4];
    public:
        Block();
        friend class CacheSet;
        friend class Cache;
};

Block::Block(){
    dirty=false;
    lru=0;
    valid=false;
    int k=BLOCK_SIZE/4;
    for(int i=0;i<k;i++)v[i]=0;
}

class CacheSet {
    //vector of blocks, size of vector is number of ways
    Block v1[WAY];
    friend class Cache;
};

class Cache {
    //vector of CacheSet
    CacheSet v2[CACHE_SIZE*1024/(BLOCK_SIZE*WAY)];
    int hits;
    int misses;
    int wb;
    public:
            Cache();
            int gethits();
            int getmisses();
            int getwb();
            int read(int address);
            void write(int address);
};
Cache::Cache(){
    hits=0;
    misses=0;
    wb=0;
}

int Cache::gethits(){
    return hits;
}

int Cache::getmisses(){
    return misses;
}

int Cache::getwb(){
    return wb;
}
int Cache::read(int address){
    int o=log2(BLOCK_SIZE);
    int i=log2(CACHE_SIZE*1024/(BLOCK_SIZE*WAY));
    int t=32-o-i;
    unsigned int offset=address<<(31-o);
    offset=offset & 0x7FFFFFFF;
    offset=offset>>(31-o);
    unsigned int index=address <<(t-1);
    index =index & 0x7FFFFFFF;
    index =index>>(31-i);
    unsigned int tag_b=address>>1;
    tag_b=tag_b & 0x7FFFFFFF;
    tag_b=tag_b >>(31-t);
    for(int i=0;i<WAY;i++){
        if(v2[index].v1[i].valid==true && v2[index].v1[i].tag==tag_b){
            hits++;
            v2[index].v1[i].lru++;
            return v2[index].v1[i].v[offset/4];
        }
    }
    misses++;
    int j=0;
    while(v2[index].v1[j].valid==true && j<WAY)j++;
    if(j<WAY){
        v2[index].v1[j].tag=tag_b;
        v2[index].v1[j].valid=true;
        v2[index].v1[j].lru++;
        return v2[index].v1[j].v[offset/4];
    }
    j=0;
    int m=0;
    int l=v2[index].v1[0].lru;
    for(j=1;j<WAY;j++){
        if(v2[index].v1[j].lru<l){
            l=v2[index].v1[j].lru;
            m=j;
        }
    }
    if(v2[index].v1[m].dirty==true)wb++;
    v2[index].v1[m].dirty=false;
    v2[index].v1[m].tag=tag_b;
    v2[index].v1[m].valid=true;
    v2[index].v1[m].lru++;
    return v2[index].v1[m].v[offset/4];
}
void Cache::write(int address){
    int o=log2(BLOCK_SIZE);
    int i=log2(CACHE_SIZE*1024/(BLOCK_SIZE*WAY));
    int t=32-o-i;
    unsigned int offset=address<<(31-o);
    offset=offset & 0x7FFFFFFF;
    offset=offset>>(31-o);
    unsigned int index=address <<(t-1);
    index =index & 0x7FFFFFFF;
    index =index>>(31-i);
    unsigned int tag_b=address>>1;
    tag_b=tag_b & 0x7FFFFFFF;
    tag_b=tag_b >>(31-t);
     for(int i=0;i<WAY;i++){
        if(v2[index].v1[i].valid==true && v2[index].v1[i].tag==tag_b){
            hits++;
            v2[index].v1[i].lru++;
            v2[index].v1[i].dirty=true;
            return;
        }
    }
    misses++;
    int j=0;
    while(v2[index].v1[j].valid==true && j<WAY)j++;
    if(j<WAY){
        v2[index].v1[j].tag=tag_b;
        v2[index].v1[j].valid=true;
        v2[index].v1[j].lru++;
        v2[index].v1[j].dirty=true;
        return ;
    }
    j=0;
    int m=0;
    int l=v2[index].v1[0].lru;
    for(j=1;j<WAY;j++){
        if(v2[index].v1[j].lru<l){
            l=v2[index].v1[j].lru;
            m=j;
        }
    }
    if(v2[index].v1[m].dirty==true)wb++;
    v2[index].v1[m].tag=tag_b;
    v2[index].v1[m].valid=true;
    v2[index].v1[m].lru++;
    v2[index].v1[m].dirty=true;
    return ;
}
#endif
