#include<bits/stdc++.h>
#include "BurstMonitor.h"

int main(){
    bm* BurstMonitor; BurstMonitor = new bm(1);
    int cnt = 0;
    rep2(i, 0, 10000){
        if(BurstMonitor->check(10))cnt += 1;
    }
    rep2(i,0,10){
    cout << BurstMonitor->powlev(i) << endl;
    }cout << 10000.0/(float)cnt << endl;
    return 0;
}