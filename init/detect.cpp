#include<bits/stdc++.h>
#include "param.h"
using namespace std;
int main(){
    uint64_t k1, k2;
    fin.read((char*)&k1,sizeof(uint64_t));
    fin.read((char*)&k2, sizeof(uint64_t)); 
    uint64_t offset = 0;
    uint64_t lk1 = 0;
    uint64_t lk = 0;
    rep2(i, 0, 10000000){
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        k1 += offset;
        if(k1 < lk1){
            k1 += (lk1-offset);
            offset = lk1;
            //cout << offset << endl;
        }
        lk1 = k1;
        if(k1 < lk)cout << i << ','<< k1 <<',' << lk << ',' << offset<< endl;
        lk = k1;
    }
    return 0;
}