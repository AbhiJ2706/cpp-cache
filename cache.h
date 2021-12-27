#ifndef _CACHE_
#define _CACHE_

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "entry.h"

using std::map;
using std::string;
using std::vector;
using std::shared_ptr;

class Cache {
    friend std::ostream & operator<<(std::ostream & out, Cache c);

    map<string, vector< shared_ptr<CacheEntry> > > entries;
    vector<string> indices;
    int ways;
    int num_entries;
    int index_bits;
    int block_size;
    int addr_length;
    public:
        Cache(int ways=2, int num_entries=8, int block_size=1, int addr_length=8);
        bool receiveEntry(string addr, int mem_addr, bool dirty=false);
        void update();
        
};

std::ostream & operator<<(std::ostream & out, Cache c);

#endif