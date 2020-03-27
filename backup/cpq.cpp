#include<bits/stdc++.h>
#include "GroundTruth.h"
#include "BM.h"
using namespace std;
int main(){
    uint64_t k1, k2;
    //char key[10];
    k1 = 100, k2 = 100;
    rep2(i, 0, 1000000){
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        //cout << k1 << endl;s
    }
    bm* BurstMonitor; BurstMonitor = new bm();
    gt* GT; GT = new gt();
    rep2(i,0,1000000){
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        BurstMonitor->insert(k1, k2);
        GT->insert(k1, k2);
    }
    rep2(i,0,1000){
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        BurstMonitor->insert(k1, k2);
        GT->insert(k1, k2);
        if(i%100 == 0){
            mylog << i << endl;
            mylog << GT->strmap.size() << endl;
            rep2(j, 0, BUCK_NUM){
                rep2(k, 0, 1){
                    uint32_t nz_monitor = BurstMonitor->get_buck(j)->speed_monitor;
                    if (nz_monitor != 0) {
                      mylog << nz_monitor << ",";
                    }
                }
                //mylog << endl;
            }
            mylog << endl;
        }
    }
    gtmap::iterator it = GT->strmap.begin();
    
    return 0;
}
