#include<bits/stdc++.h>
//#include "CorrectBurstDetector.h"
#include "BurstMonitor.h"
using namespace std;
int main(){
    uint64_t k1, k2;
    //char key[10];
    k1 = 100, k2 = 100;
    rep2(i, 0, 1000000){
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        //mylog << k1 << endl;
    }
    bm* BurstMonitor; BurstMonitor = new bm(3);
    CorrectBurstDetector* GT; GT = new CorrectBurstDetector(25);
    rep2(i,0,400000){
        if(i%100000 == 0)cout << i << endl;
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        BurstMonitor->insert(k2, k1);
        GT->insert(k2, k1);
    }
    cout << GT->Record.size() << endl;
    for(int i = 0; i < GT->Record.size(); i++){
        mylog << GT->Record[i].flow_id << endl;
    }
    //cout << "********" << endl;
    //cout << (GT->strmap.end()--)->se.size() << endl;
    //cout << (GT->strmap.size()) << endl;
    /*
    rep2(i,0,1000){
        fin.read((char*)&k1,sizeof(uint64_t));
        fin.read((char*)&k2, sizeof(uint64_t)); 
        BurstMonitor->insert(k2, k1);
        GT->insert(k2, k1);
    }*/
    //cout << "k1," << k1 << endl;
    //vector<burst> t = GT->query(0,2880112032798295UL, 2028919099124358108UL,BURST_THRE,1.5,0);
    //cout << k1 <<','<< k2 << endl;
    //cout << t.front().valid << endl;
    //cout << t.size() << endl;
    //for(int i = 0; i < t.size(); i++){
    //    cout << i << ',' << endl;
    //    cout << t[i].starttime << ',' << t[i].endtime << endl;
    //}
    //cout << t.front().starttime << ',' << t.front().endtime << endl;
    
    return 0;
}
