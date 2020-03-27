#include "param.h"
#include<bits/stdc++.h>
typedef map<uint64_t, multiset<uint64_t>> gtmap;
typedef multiset<uint64_t> flowset;
class burst{
public:
    uint64_t starttime;
    uint64_t flowid;
    uint64_t endtime;
    bool valid;
    burst(const uint64_t & start, const uint64_t & flow, const uint64_t & end): starttime(start), flowid(flow),
        endtime(end), valid(1){ }
    burst(const int & sta):starttime(0), flowid(0), endtime(0), valid(0){ }
};
class gt{
//private:
public:
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
    vector<burst> query(const uint64_t & starttime, const uint64_t & endtime, const uint64_t & flowid,
        const uint64_t & thres, float scale, uint32_t cut){
        /*
        [starttime, endtime)
        */
        gtmap::iterator gtit = strmap.find(flowid);
        uint64_t hop = thres/10;
        if(gtit == strmap.end()){
            //notfound(starttime, endtime, flowid, windowsize, windownum, scalesize);
            return vector<burst>{burst(0)};
        }
        flowset::iterator flit = gtit->se.begin(); //flit point to beginning of the multiset
        flowset::iterator flend = gtit->se.end();
        while(*flit < starttime){
            if(flit == flend){
                //notfound(starttime, endtime, flowid, windowsize, windownum, scalesize);
                return vector<burst>{burst(0)};
            }
            flit++;
        }
        bool flag = 0;
        vector<burst> result;
        while(flit != flend && *flit < endtime){
            flowset::iterator tmpit = flit;
            uint64_t StartTime = *flit;
            uint32_t cnt1 = 0;
            uint32_t cnt2 = 0;
            float ratio1, ratio2;
            while(*tmpit < *flit+ hop){
                cnt1 += 1;
                tmpit++;
            }
            while(*tmpit < *flit + 2*hop){
                cnt2 += 1;
                tmpit++;
            }
            ratio1 = (float)cnt2/(float)(cnt1+1);
            if(cnt2 < cut) ratio1 = 0;
            cnt1=0, cnt2=0;

            if(ratio1 < scale) {
                flit = gtit->se.find(*flit+hop);
                continue;
            }
            bool flag1 = 0;
            while(flit != flend && *flit < thres+StartTime && flag1 == 0){
                flit = tmpit;
                cnt1 = 0, cnt2 = 0;
                while(*tmpit < *flit+ hop){
                    cnt1 += 1;
                    tmpit++;
                }
                while(*tmpit < *flit + 2*hop){
                    cnt2 += 1;
                    tmpit++;
                }
                ratio2 = (float)cnt1/(float)(cnt2+1);
                if(cnt1 < cut)ratio2 = 0;
                if(ratio2 > scale){
                    result.push_back(burst(StartTime, flowid, *tmpit));
                    flag1 = 1, flag = 1;
                }
            }
            flit = tmpit;
        }
        if(flag == 0){
            return vector<burst>{burst(0)};
        }
        return result;
    }
};