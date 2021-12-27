#ifndef _ENTRY_
#define _ENTRY_

#include <string>
#include <vector>

using std::vector;
using std::string;

class CacheEntry {
    vector<int> block;
    string tag;
    int time;
    bool valid;
    bool dirty;
    void calcBlock(int addr, int blocksize);
    public:
        CacheEntry(string tag, int addr, bool dirty, int blocksize);
        void increment();
        string getTag() const {return this->tag;};
        int getTime() const {return this->time;};
        vector<int> getBlock() const {return this->block;};
        bool hit(string tag, int addr);
};

#endif