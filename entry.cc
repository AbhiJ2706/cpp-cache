#include "entry.h"


CacheEntry::CacheEntry(string tag, int addr, bool dirty, int blocksize) 
    :   tag(tag),
        valid(true),
        dirty(dirty),
        time(0)
{
    this->calcBlock(addr, blocksize);
}

void CacheEntry::calcBlock(int addr, int blocksize) {
    while (addr % (8 * blocksize) != 0) {
        --addr;
    }
    for (int i = 0; i < blocksize; ++i) {
        this->block.push_back(addr + 8 * i);
    }
}

void CacheEntry::increment() {
    ++this->time;
}

bool CacheEntry::hit(string tag, int addr) {
    if (this->tag == tag)
        for (int i = 0; i < this->block.size(); ++i) {
            if (this->block[i] == addr) {
                this->time = 0;
                return true;
            }
        }
    return false;
}
