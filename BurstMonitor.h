#include "BOBHash32.h"
#define START_SPEED 128
#define LEV_NUM 1024
#define BUCK_NUM 1000
#define HASH_NUM 4
#define BURST_THRE 2048
class buck{
public:
    uint32_t speed_monitor;
    uint32_t *time_record;
    buck(){
        speed_monitor = START_SPEED;
        time_record = new uint32_t[LEV_NUM];
        memset(time_record, 0, LEV_NUM * sizeof(uint32_t));
    }
    ~buck(){
        if(time_record) delete []time_record;
    }
};
class bm{
private:
    buck ** bucket;
    BOBHash32 ** bmhash;
public:
    
    bm(){
        bucket = new buck*[BUCK_NUM];
        rep2(i, 0, BUCK_NUM){
            bucket[i] = new buck();
        }
        srand((unsigned int)time(0));
        rep2(i, 0, HASH_NUM){
            bmhash[i] = new BOBHash32(rand());
            srand(rand());
        }
    }
    
    uint32_t getk(uint32_t x){
        return x/128;
    }

    uint32_t pow2(int x){
        if (x == 0)return 1;
        return 2*pow2(x-1);
    }

    bool check(uint32_t k){
        uint32_t r = rand() % UINT32_MAX;
        if (r < UINT32_MAX / pow2(k)) return 1;
        else return 0;
        srand(rand());
    }

    void insert(const char * flow){
        rep2(i, 0, HASH_NUM){
            unsigned int pos = bmhash[i]->run(flow, 8*sizeof(char)) % BUCK_NUM;
            uint32_t k = getk(bucket[pos]->speed_monitor);
            if(check(k)) bucket[pos]->speed_monitor += 1;
        }
    }

    void claimBurst(){

    }

    void detectBurst(uint32_t lastTime, uint32_t currTime){
        if(currTime - lastTime < BURST_THRE) claimBurst();
    }

    void decay(uint32_t pos, uint32_t currTime){
        uint32_t & sp_mo = bucket[pos]->speed_monitor;
        uint32_t k1 = getk(sp_mo);
        sp_mo -= 64;
        uint32_t k2 = getk(sp_mo);
        uint32_t & lt = bucket[pos]->time_record[k2];
        if(k2 < k1) detectBurst(lt, currTime);
        lt = currTime;
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