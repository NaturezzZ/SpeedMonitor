#include<bits/stdc++.h>
//#include "CorrectBurstDetector.h"
#include "BurstMonitor.h"
using namespace std;
pair<float,float> test(){
    ifstream fin(datapath, std::ios::binary);
    
    uint64_t k1, k2;
    k1 = 100, k2 = 100;
    bm* BurstMonitor; BurstMonitor = new bm(2);
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
                if(abs((double)BurstMonitor->Record[i].start_window - (double)GT->Record[j].start_window*window_size) < 2*(double)BURST_THRE ){
                    cnt += 1;
                    break;
                }
            }
        }
    }
    int cnt1 = 0;
    rep2(i, 0, GT->Record.size()){
        uint64_t id = GT->Record[i].flow_id;
        rep2(j, 0, BurstMonitor->Record.size()){
            if(BurstMonitor->Record[j].flow_id == id){
                if(abs((double)BurstMonitor->Record[j].start_window - (double)GT->Record[i].start_window*window_size) < 2*(double)BURST_THRE ){
                    cnt1 += 1;
                    break;
                }
            }
        }
    }

    rep2(i, 0, BurstMonitor->Record.size()){
        uint64_t flowtmp = BurstMonitor->Record[i].flow_id;
        if(flowtmp == 113254552UL){
            cout << BurstMonitor->Record[i].start_window << ',' << BurstMonitor->Record[i].end_window << endl;
        }
    }

    cout << "flowsize " << GT->w << endl;
    cout << "bm size, " << BurstMonitor->Record.size() << " gt size, " << (GT->Record.size()) << endl;
    fin.close();

    //gtlog << "***********" << endl;
    rep2(i,0,1000){
    //    gtlog << window_size*i << ',' << GT->tmp[i] << endl;
    }

    //gtlog << "*********" << endl;
    rep2(i,0, GT->Record.size()){
        //gtlog << GT->Record[i].flow_id << endl;
    }
    return make_pair((float)cnt/(float)(BurstMonitor->Record.size()), (float)cnt1/(float)(GT->Record.size()));
}
int main(int argc, char* argv[]){
    char logpath[100] = {0};
    strcat(logpath, "/home/zhengnq/SpeedMonitor/further/burst/");
    strcat(logpath, argv[1]);
    strcat(logpath, argv[2]);
    ofstream mylog(logpath, std::ios::out|std::ios::trunc);
    for(uint32_t i = 0; i < 10; i++){
        BUCK_NUM = 300*(10*atoi(argv[2])+i+1);
        
        pair<float,float> r = test();
        //cout << BUCK_NUM << ',' << r.fi << ',' << r.se << endl;
        cout << BUCK_NUM << endl;
        mylog << BUCK_NUM << ',' << r.fi << ',' << r.se << endl;
    }
    return 0;
}
