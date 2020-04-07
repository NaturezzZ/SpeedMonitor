#include "BOBHash32.h"
#include "CorrectBurstDetector.h"
#define START_SPEED 8
#define LEV_NUM 1024
uint32_t BUCK_NUM = (1000);
#define HASH_NUM 1
#define BURST_THRE 100*unittime
#define DECAY 1
#define stair 8
#define DECAY_CYCLE (unittime/4)
#define LEV 1.1
#define BURST_LEV 5
#define ULMAX (0xffffffffffffffffUL)
class buck{
public:
    uint32_t speed_monitor;
    uint64_t *time_record;
    uint64_t flow_record;
    buck(){
        speed_monitor = START_SPEED;
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
    uint32_t ts;
    uint32_t m;
    vector<Burst> Record;
    buck * get_buck (int i) {
      return bucket[i];
    }
    
    bm(uint32_t _m){
        m = _m;
        ts = 0;
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
            //cout << i << endl;
            srand(rand());
        }

    }
    
    uint32_t getk(uint32_t x){ // get speed level (in exponential levels)
        return x/stair;
    }

    double powlev(int x){
        if (x == 0)return 1.0;
        return LEV*powlev(x-1);
    }

    bool check(uint32_t k){
        uint32_t r = rand() % UINT32_MAX;
        if ((double)r < (double)UINT32_MAX / powlev(k)) return 1;
        else return 0;
        srand(rand());
    }

    void insert(const uint64_t flow, const uint64_t & currTime){
        //mylog << flow << endl;
        //cout << 1 << endl;

        rep2(i, 0, HASH_NUM){

            if(ts < currTime/(BURST_THRE/2)){
                ts = currTime/(BURST_THRE/2);
                rep2(j,0,BUCK_NUM){
                    uint32_t k = getk(bucket[j]->speed_monitor);
                    if(check(k)) bucket[j]->speed_monitor += 1;
                    uint64_t prevTime = bucket[j]->time_record[k];
                    uint32_t tmp = (currTime-decaytime[j])/DECAY_CYCLE;
                    bucket[j]->time_record[k] = currTime;
                    if(prevTime == 0)continue;
                    //cout << '*'<< tmp << ',' << currTime<<',' << prevTime<< endl;
                    //if (tmp >= 10000) continue;
                    bool f = 1;
                    uint64_t tmpflow = bucket[j]->flow_record;
                    rep2(k, 0, tmp){
                        //cout << tmp << endl;
                        decaytime[j] = currTime;
                        if(bucket[j]->speed_monitor == 0)break;
                        decay(j, currTime, tmpflow, f);
                    }
                }
            }
            //cout << 1 << endl;
            unsigned int pos = bmhash[i]->run((char*)&flow, 8) % BUCK_NUM;
            //cout << pos<< endl;
            bucket[pos]->flow_record = flow;

            uint32_t k = getk(bucket[pos]->speed_monitor);
            if(check(k)) bucket[pos]->speed_monitor += 1;
            uint64_t prevTime = bucket[pos]->time_record[k];
            uint32_t tmp = (currTime-decaytime[pos])/DECAY_CYCLE;
            bucket[pos]->time_record[k] = currTime;
            if(prevTime == 0)continue;
            //cout << '*'<< tmp << ',' << currTime<<',' << prevTime<< endl;
            //if (tmp >= 10000) continue;
            bool f = 1;
            rep2(j, 0, tmp){
                //cout << tmp << endl;
                decaytime[pos] = currTime;
                if(bucket[pos]->speed_monitor == 0)break;
                decay(pos, currTime, flow, f);
            }
            if(flow == 424178577768553675UL){
                //if(tmp != 0)cout << tmp << endl;
                //mylog << currTime << ','<< bucket[pos]->speed_monitor << endl;
            } 
        }
        
    }

    void claimBurst(uint64_t & flowid, uint64_t & starttime, uint64_t & endtime){
        //if(flowid == 4458840894235853209UL){

        //}
        Record.push_back(Burst(starttime,endtime,flowid));
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
        uint32_t & sp_mo = bucket[pos]->speed_monitor;
        uint32_t k1 = getk(sp_mo);
        if (sp_mo < DECAY)sp_mo = 0;
        else sp_mo -= DECAY;
        uint32_t k2 = getk(sp_mo);
        uint64_t  lt = bucket[pos]->time_record[k2];
        if(k2 < k1 && deflag) {
            bool tmp = detectBurst(lt, currTime, k2, pos, flow);
            if(tmp) deflag = 0;
        }
        bucket[pos]->time_record[k2] = currTime;
        decaytime[pos] = currTime;
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