## ``param.h``
- ``param.h``中``datapath``为输入数据集的地址，``logpath``为输出log的地址

## ``GroundTruth.h``
#### burst类
- ``GroundTruth.h`` 中 ``burst``类用来记载burst的信息，包括开始时间、flowid、窗口大小、窗口数量、倍数、valid=1表示这是一个有效的burst

- 当中间windownum个窗口中的平均数量大于两侧相同大小的各一个窗口的数量时，认为这个是一个burst

#### gt类
- ``GroundTruth.h`` 中 ``gt`` 类用来无误差地记录进入流的信息

- ``void gt::insert(const uint64_t & timestamp, const uint64_t & flowid)`` 插入时间戳和flowid

- ``std::set<burst> gt::query(const uint64_t & starttime, const uint64_t & endtime, const uint64_t & flowid, const unsigned int & windowsize, const unsigned int & windownum, const unsigned int & scalesize)``

    得到[starttime, endtime)开始的所有满足要求的burst，如果不存在则返回一个valid=0的占位符，并记录log。否则返回一个burst对象的set。
    
    - starttime为开始时间戳，endtime为结束时间戳
    - flowid为选取流的id
    - windowsize为window的大小
    - windownum表示burst的宽度
    - scalesize表示burst的高度