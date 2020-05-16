#include "BOBHash32.h"
#include "CorrectBurstDetector.h"
#define START_SPEED 40
#define LEV_NUM 1024
uint32_t BUCK_NUM = (1000);
#define HASH_NUM 1
#define BURST_THRE (100*unittime)
#define DECAY 1
#define stair 8
#define DECAY_CYCLE (unittime/4)
#define LEV 1.1
#define BURST_LEV 5
#define ULMAX (0xffffffffffffffffUL)
const int k = 40;
class buck{
public:
    uint32_t speed_monitor;
    uint32_t last_sm;
    uint64_t *time_record;
    uint64_t flow_record;
    buck(){
        speed_monitor = 0;
        last_sm = 0;
        time_record = new uint64_t[LEV_NUM];
        memset(time_record, 0, LEV_NUM * sizeof(uint64_t));
        flow_record = ULMAX;
    }
    ~buck(){
        if(time_record) delete []time_record;
    }
};
class bm{
private:
    buck ** bucket;
    BOBHash32 ** bmhash;
    uint64_t * decaytime;
public:
    uint64_t ts;
    uint32_t m;
    static uint32_t seed;
    vector<Burst> Record;
    uint32_t number;
    buck * get_buck (int i) {
      return bucket[i];
    }
    
    bm(uint32_t _m){
        m = _m;
        ts = 0;
        seed = 0;
        number = 0;
        bucket = new buck*[BUCK_NUM];
        decaytime = new uint64_t[BUCK_NUM];
        rep2(i, 0, BUCK_NUM){
            bucket[i] = new buck();
            decaytime[i] = 0;
        }

        srand((unsigned int)time(0));
        bmhash = new BOBHash32*[HASH_NUM];
        rep2(i, 0, HASH_NUM){
            bmhash[i] = new BOBHash32(i);
        }

    }
    
    bool prob(double x){
        uint32_t r = rand() % RAND_MAX;
        if ((double)r < ((double)RAND_MAX * x)) return 1;
        else return 0;
    }

    uint32_t getk(uint32_t x){ // get speed level (in exponential levels)
        return x/stair;
    }

    double powlev(int x){
        if (x == 0)return 1.0;
        return LEV*powlev(x-1);
    }

    bool check(uint32_t k){
        uint32_t r = rand() % RAND_MAX;
        if ((double)r < ((double)RAND_MAX / powlev(k))) return 1;
        else return 0;
    }

    void insert(const uint64_t flow, const uint64_t & currTime){

        rep2(i, 0, HASH_NUM){

            unsigned int pos = bmhash[i]->run((char*)&flow, 8) % BUCK_NUM;
            bucket[pos]->flow_record = flow;

            bucket[pos]->speed_monitor += 1;
            uint32_t tmp = (currTime-decaytime[pos])/DECAY_CYCLE;
            bool f = 0;
            rep2(j, 0, tmp){
                decaytime[pos] = decaytime[pos] + DECAY_CYCLE;
                //if(bucket[pos]->last_sm == 0)continue;
                decay(pos, currTime, flow, f);
            }
            if(flow == 113254552UL){
                if(number > 10000) return;
                if(currTime > number*(1*unittime)){
                    number += 1;
                }
                gtlog << currTime << ','<< bucket[pos]->last_sm+bucket[pos]->speed_monitor << endl;
            } 
        }
    }

    void claimBurst(uint64_t & flowid, uint64_t & starttime, uint64_t & endtime){
        //if(flowid == 4458840894235853209UL){

        //}
        //Record.push_back(Burst(starttime,endtime,flowid));
        //mylog << "flow id = " << flowid << ':' << endl;
        //mylog << starttime << ',' << endtime << endl;
    }

    bool detectBurst(uint64_t lastTime, uint64_t currTime, uint32_t lev, uint32_t pos, uint64_t flow){
        
        if(currTime - lastTime > BURST_THRE) return 0;
        int cnt = 0;
        rep2(i, (int)lev, (int)LEV_NUM){
            uint64_t tmp = bucket[pos]->time_record[i];
            if( tmp >= lastTime && tmp <= currTime) cnt += 1;
            else break;
        }
        if (lev >= m && cnt > BURST_LEV) {claimBurst(flow, lastTime, currTime); return 1;}
        return 0;
    }

    void decay(uint32_t pos, uint64_t currTime, uint64_t flow, bool &deflag){
        
        bucket[pos]->flow_record = flow;

        double xhat = (double)bucket[pos]->speed_monitor;
        double y0p = (double)bucket[pos]->last_sm;
        double y1 = ((double)(k-1)/(double)k) * y0p + xhat;
        double dx = 2.0*(y1-y0p)/(double) (k+1);
        bucket[pos]->speed_monitor = 0;
        
        if(y0p < 0.5*(double)(k*(k-1)) * dx){
            double y1p = y0p + xhat;
            double temp = y1p - (double)(int)y1p;
            if(prob(1-temp)) bucket[pos]->last_sm = (int)y1p;
            else bucket[pos]->last_sm = (int)y1p + 1;
        }
        else{
            double y1p = y0p + 2.0*(double)k/(double)(k+1) * (double)(y1-y0p);
            double temp = y1p - (double)(int)y1p;
            if(prob(1-temp)) bucket[pos]->last_sm = (int)y1p;
            else bucket[pos]->last_sm = (int)y1p + 1;
        }
        
    }

    ~bm(){
        rep2(i, 0, BUCK_NUM){
            if(bucket[i]) delete bucket[i];
        }
        rep2(i, 0, HASH_NUM){
            if(bmhash[i]) delete bmhash[i];
        }
        delete bucket;
        delete bmhash;
    }

};
uint32_t bm::seed = 0;