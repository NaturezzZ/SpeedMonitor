#include<bits/stdc++.h>
#include<map>
#include "Burst.h"
#include "param.h"
#define window_size (10*unittime)
class CorrectBurstDetector
{
	public:
	uint32_t tmp[10000000];
	map<uint64_t, int> F;
	uint32_t w, m;
	uint32_t* counter[2];
	uint64_t* timestamp;
	uint64_t* id;
	uint64_t last_timestamp;
	bool flag;

	vector<Burst> Record;
	CorrectBurstDetector(int _m)
	{
		memset(tmp, 0, sizeof(tmp));
		F.clear();
		w = 0;
		m = _m;
		flag = 0;
		last_timestamp = 0;
		Record.clear();
		counter[0] = new uint32_t[1000000];
		counter[1] = new uint32_t[1000000];
		id = new uint64_t[1000000];
		timestamp = new uint64_t[1000000];
		for(int i = 0; i < 1000000; i++)
		{
			counter[0][i] = 0;
			counter[1][i] = 0;
			timestamp[i] = -1;
			id[i] = 0;
		}
	}
	void update(uint64_t time)
	{
		for(int i = 0; i < w; i++)
		{
			//if(id[i] == 113254552UL) cout << time << ' ' << counter[flag][i] << endl;
			if(counter[flag][i] <= counter[flag ^ 1][i] / 2 && timestamp[i] != -1 && time - timestamp[i] <= 10)
			{
				//output burst
				Record.push_back(Burst(timestamp[i], time, id[i]));
				//gtlog << "id: " << id[i] << endl;
				//gtlog << timestamp[i]*window_size << ", " << time*window_size << endl;
				//if(id[i] == 113254552UL) cout << "Burst!\n" << endl;
				timestamp[i] = -1;
			}
			if(counter[flag][i] < m)
				timestamp[i] = -1; 
			if(counter[flag][i] >= 2 * counter[flag ^ 1][i] && counter[flag][i] >= m)
				timestamp[i] = time;
		}
		flag ^= 1;
		for(int i = 0; i < w; i++)
			counter[flag][i] = 0;
	}
	void insert(uint64_t flow_id, uint64_t flow_time)
	{
		uint64_t timestamp = flow_time / window_size;
		if(last_timestamp < timestamp)
		{
			for(int i = last_timestamp; i < timestamp; i++)
				update(i);
			last_timestamp = timestamp;
		}
		if(F.find(flow_id) == F.end())
		{
			id[w] = flow_id;
			F[flow_id] = w++;
		}
		counter[flag][F[flow_id]]++;
		if(flow_id == 113254552UL){
			tmp[timestamp] += 1;
		}
	}
};
