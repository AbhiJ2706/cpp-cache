#include <bitset>
#include <cmath>
#include <iostream>

#include "cache.h"

using std::make_shared;
using std::cout;
using std::endl;


string operator*(string y, float x) {
    string ret;
    for (int i = 0; i < x; ++i) ret += y;
    return ret;
}

Cache::Cache(int ways, int num_entries, int block_size) 
    :   ways(ways),
        num_entries(num_entries),
        index_bits(log2(num_entries)),
        block_size(block_size)
{
    for (int i = 0; i < num_entries; ++i) {
        const int num_bits = index_bits;
        this->indices.push_back(std::bitset<64>(i).to_string().substr(61, 64));
    }
}

bool Cache::receiveEntry(string addr, int mem_addr, bool dirty) {
    string index = addr.substr(addr.size() - index_bits, addr.size());
    string tag = addr.substr(0, addr.size() - index_bits);
    for (auto ent: this->entries[index]) {
        if (ent->hit(tag, mem_addr)) return true;
    }
    if (this->entries[index].size() < this->ways) {
        shared_ptr<CacheEntry> x = make_shared<CacheEntry>(tag, mem_addr, dirty, this->block_size);
        this->entries[index].push_back(x);
        return false;
    }
    shared_ptr<CacheEntry> max_time_index = this->entries[index][0];
    int max_time = 0;
    for (int i = 0; i < this->entries[index].size(); ++i) {
        if (this->entries[index][i]->getTime() > max_time_index->getTime()) {
            max_time_index = this->entries[index][i];
            max_time = i;
        }
    }
    this->entries[index][max_time] = make_shared<CacheEntry>(tag, mem_addr, dirty, this->block_size);
    return false;
}

void Cache::update() {
    for (auto ent: this->entries) {
        for (auto item: ent.second) {
            item->increment();
        }
    }
}

std::ostream & operator<<(std::ostream & out, Cache c) {
    for (auto ent: c.entries) {
        out << "--------------------------" << endl;
        out << ent.first << endl;
        for (auto item: ent.second) {
            out << item->getTag() << " ";
            out << item->getTime() << " ";
            for (auto b: item->getBlock()) out << b << " ";
            out << endl;
        }
    }
    return out;
}