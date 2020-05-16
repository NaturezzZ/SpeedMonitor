#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
int main(int argc, char* argv[]){
    int upper = atoi(argv[2]);
    char resultpath[100] = {0};
    strcat(resultpath, "/home/zhengnq/SpeedMonitor/further/burst/");
    strcat(resultpath, argv[1]);
    ofstream myout(resultpath, std::ios::out|std::ios::trunc);

    char originpath[100] = {0};
    strcat(originpath, "/home/zhengnq/SpeedMonitor/further/burst/");
    strcat(originpath, argv[1]);

    
    for(int i = 0; i < upper; i++){
        char buf[100] = {0};
        strcat(buf, originpath);
        strcat(buf, to_string(i).c_str());
        ifstream myin(buf);
        while( !myin.eof() ) //文件未结束，循环
        {
            char str[1024];
            myin.getline(str,sizeof(str),'\n'); //读一行
            cout << str << endl; //输出到屏幕
            myout<< str <<endl; //输出到文件
        }
        char tmp[100] = {0};
        strcat(tmp, "rm ");
        strcat(tmp, buf);
        system(tmp);  
    }
    myout.close();

}