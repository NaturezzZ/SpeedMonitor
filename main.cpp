#include<bits/stdc++.h>
//#include "CorrectBurstDetector.h"
#include "BurstMonitor.h"
using namespace std;
pair<float,float> test(){
    ifstream fin(datapath, std::ios::binary);
    
    uint64_t k1, k2;
    k1 = 100, k2 = 100;
    bm* BurstMonitor; BurstMonitor = new bm(1);
    CorrectBurstDetector* GT; GT = new CorrectBurstDetector(50);
    uint64_t lk1 = 0;
    uint64_t offset = 0;
    rep2(i,0,10000000){
        if(i%1000000 == 0)cout << i/100000<< '%' << endl;
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        k1 += offset;
        if(k1 < lk1){
            k1 += (lk1-offset);
            offset = lk1;
        }
        lk1 = k1;
        BurstMonitor->insert(k2, k1);
        GT->insert(k2, k1);
    }
    cout << GT->Record.size() << endl;
    int cnt = 0;
    rep2(i, 0, BurstMonitor->Record.size()){
        uint64_t id = BurstMonitor->Record[i].flow_id;
        rep2(j, 0, GT->Record.size()){
            if(GT->Record[j].flow_id == id){
                cnt += 1;
                break;
            }
        }
    }
    int cnt1 = 0;
    rep2(i, 0, GT->Record.size()){
        uint64_t id = GT->Record[i].flow_id;
        rep2(j, 0, BurstMonitor->Record.size()){
            if(BurstMonitor->Record[j].flow_id == id){
                cnt1 += 1;
                break;
            }
        }
    }
    cout << "flowsize " << GT->w << endl;
    cout << "bmsize " << BurstMonitor->Record.size() << endl;
    cout << "gtsize " << GT->Record.size() << endl;
    cout << "precision " << (float)cnt/(float)(BurstMonitor->Record.size()) << endl;
    cout << "recall " << (float)cnt1/(float)(GT->Record.size()) << endl;
    
    fin.close();
    return make_pair((float)cnt/(float)(BurstMonitor->Record.size()), (float)cnt1/(float)(GT->Record.size()));
}
int main(){

    for(uint32_t i = 0; i < 100; i++){
        BUCK_NUM = 1000*(i+1);
        pair<float,float> r = test();
        mylog << BUCK_NUM << ',' << r.fi << ',' << r.se << endl;
    }
    return 0;
}
