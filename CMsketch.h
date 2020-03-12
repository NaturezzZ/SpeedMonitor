#define counter_t uint32_t
#define CNT_MAX 0xffffffff
#include "BOBHash32.h"
class CM{
    private:
        int d;
        int width;
        counter_t** counters;
        BOBHash32** hash_func;
    public:
        CM(int d_ = 3, int width_ = 256 * 1024):d(d_), width(width_){
            counters = new counter_t*[d];
            hash_func = new BOBHash32*[d];
            for(int i = 0; i < d; i++){
                counters[i] = new counter_t[width];
                for(int k = 0; k < width; k++){
                    counters[i][k] = 0;
                }
                hash_func[i] = new BOBHash32(i * 12 + 221);
            }
        }
        ~CM(){
            for(int i = 0; i < d; i++){
                delete counters[i];
                delete hash_func[i];
            }
            delete counters;
            delete hash_func;
        }
        void insert(const char* key, counter_t f = 1){
            for(int i = 0; i < d; i++){
                int index = hash_func[i]->run(key, KEY_LENGTH) % width;
                counters[i][index] += f;
            }
        }
        counter_t query(const char* key){
            counter_t ret = CNT_MAX;
            for(int i = 0; i < d; i++){
                int index = hash_func[i]->run(key, KEY_LENGTH) % width;
                ret = MIN(ret, counters[i][index]);
            }
            return ret;
        }
        double calc_ARE(const FREQ_RECORD& Real_Freq){
            double RE_sum = 0;
            for(FREQ_RECORD::const_iterator it = Real_Freq.begin(); it != Real_Freq.end(); it++){
                int AE = query(it->first.c_str()) - it->second;
                double RE = (double)AE/(double)(it->second);
                RE_sum += RE;
		        if(AE < 0){
		            printf("error! %d\n", AE);
		            printf("%d, %d\n", it->second, query(it->first.c_str()));
		        }
            }
            return RE_sum / (double)Real_Freq.size();
        }
};
