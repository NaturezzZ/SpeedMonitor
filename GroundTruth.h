#include "param.h"
#include<bits/stdc++.h>
typedef map<uint64_t, multiset<uint64_t>> gtmap;
typedef multiset<uint64_t> flowset;
class burst{
public:
    uint64_t starttime;
    uint64_t flowid;
    unsigned int winsize;
    unsigned int winnum;
    unsigned int scale;
    bool valid;
    burst(const uint64_t & start, const uint64_t & flow, const unsigned int & windowsize, 
        const unsigned int & windownum, const unsigned int & scalesize): starttime(start), flowid(flow),
        winsize(windowsize), scale(scalesize), winnum(windownum), valid(1){ }
    burst(const int & sta):starttime(0), flowid(0), winsize(0), scale(0), winnum(0), valid(0){ }
};
class gt{
private:
    gtmap strmap;
public:
    gt(){
        strmap.clear();
    }
    ~gt(){}
    int insert(const uint64_t & timestamp, const uint64_t & flowid){
        gtmap::iterator gtit = strmap.find(flowid);
        if(gtit == strmap.end()){
            gtit = strmap.insert(make_pair(flowid, flowset())).fi;
        }
        gtit->se.insert(timestamp);
    }
    void notfound(const uint64_t & starttime, const uint64_t & endtime, const uint64_t & flowid,
        const unsigned int & windowsize, const unsigned int & windownum, const unsigned int & scale){
        mylog << "[Warning] query not found:" << endl;
        mylog << "flowid = " << flowid << endl;
        mylog << "start time = " << starttime << ", end time = " << endtime << endl;
        mylog << "window size = " << windowsize << ", window number = " << windownum << ", scale = " << scale << endl;
        mylog << endl;
    }
    set<burst> query(const uint64_t & starttime, const uint64_t & endtime, const uint64_t & flowid,
        const unsigned int & windowsize, const unsigned int & windownum, const unsigned int & scalesize){
        /*
        [starttime, endtime)
        */
        gtmap::iterator gtit = strmap.find(flowid);
        if(gtit == strmap.end()){
            notfound(starttime, endtime, flowid, windowsize, windownum, scalesize);
            return set<burst>{burst(0)};
        }
        flowset::iterator flit = gtit->se.begin(); //flit point to beginning of the multiset
        flowset::iterator flend = gtit->se.end();
        while(*flit < starttime){
            if(flit == flend){
                notfound(starttime, endtime, flowid, windowsize, windownum, scalesize);
                return set<burst>{burst(0)};
            }
            flit++;
        }
        set<burst> result;
        while(*flit < endtime){
            uint64_t timestamp[4];
            timestamp[0] = *flit;
            timestamp[1] = timestamp[0]+(uint64_t)windowsize;
            timestamp[2] = timestamp[1]+(uint64_t)windownum *(uint64_t)windowsize;
            timestamp[3] = timestamp[2] + windowsize;
            flowset::iterator it = flit;
            uint64_t cnt[3];
            rep2(i, 0, 3){
                cnt[i] = 0;
                while(it != flend && *it < timestamp[i+1]){
                    cnt[i] += 1;
                    it ++;
                }
            }
            double ave[3];
            rep2(i, 0, 3){ ave[i] = (double)cnt[i]; }
            ave[1] /= (double) windownum;
            if(ave[1]>(double)scalesize * ave[0] && ave[1]>(double)scalesize * ave[2]){
                result.insert(burst(starttime, flowid, windowsize, windownum, scalesize ) );
                flit = it;
            }
        }
    }
};