#include<bits/stdc++.h>
#include"BOBHash32.h"

#define datapath "/usr/share/dataset/DataSet/CAIDA/formatted00.dat"
#define logpath "/home/zhengnq/SpeedMonitor/log"
#define fi first
#define se second
#define rep(i,a,b) for(int (i)=(a);(i)<=(b);(i)++)
#define rep2(i,a,b) for(int (i)=(a);(i)<(b);(i)++)
#define END_FILE_NO 1
#define START_FILE_NO 0
#define KEY_LENGTH 8
#define ITEM_LENGTH 16
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) ((a)>0?(a):(-(a)))
struct FIVE_TUPLE{
	char key[ITEM_LENGTH];
};
typedef vector<FIVE_TUPLE> TRACE;
typedef unordered_map<string, int> FREQ_RECORD;

ifstream fin(datapath, ios::binary);
ofstream mylog(logpath);