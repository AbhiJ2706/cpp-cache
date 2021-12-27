#include <bitset>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "cache.h"

using std::make_shared;
using std::cout;
using std::endl;
using std::setw;
using std::to_string;


string operator*(string y, float x) {
    string ret;
    for (int i = 0; i < x; ++i) ret += y;
    return ret;
}

std::ostream & operator<<(std::ostream & out, vector<int> x) {
    string o;
    for (int i = 0; i < x.size(); ++i) {
        o += to_string(x[i]);
        if (i != x.size() - 1) o += ", ";
    }
    out << o;
    return out;
}

std::ostream & operator<<(std::ostream & out, vector<string> x) {
    string o;
    for (int i = 0; i < x.size(); ++i) {
        o += x[i];
        if (i != x.size() - 1) o += ", ";
    }
    out << o;
    return out;
}

Cache::Cache(int ways, int num_entries, int block_size, int addr_length) 
    :   ways(ways),
        num_entries(num_entries),
        index_bits(log2(num_entries)),
        block_size(block_size),
        addr_length(addr_length)
{
    for (int i = 0; i < num_entries; ++i) {
        const int num_bits = index_bits;
        this->indices.push_back(std::bitset<64>(i).to_string().substr(64 - num_bits, 64));
    }
}

bool Cache::receiveEntry(string addr, int mem_addr, bool dirty) {
    string index = addr.substr(addr.size() - index_bits, addr.size());
    string tag = addr.substr(0, addr.size() - index_bits);
    if (index.length() != index_bits) throw std::runtime_error{"wrong index length"};
    if (tag.length() != addr_length - index_bits) throw std::runtime_error{"wrong tag length"};
    if (mem_addr > 9999) throw std::runtime_error{"memory address too large"};

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
    
    if (c.ways > 4) {
        for (auto ent: c.entries) {
            out << "--------------------------" << endl;
            out << ent.first << endl;
            for (auto item: ent.second) {
                out << item->getTag() << " ";
                for (auto b: item->getBlock()) out << b << " ";
                out << endl;
            }
        }
        return out;
    }

    const string horizontal = "-";
    const string intersect = "+";
    const string vertical = "|";
    const int index_length = c.index_bits + 5;
    const int tag_length = (c.addr_length - c.index_bits) + 5;
    const int data_length =  (c.block_size * 6);

    out << setw(index_length) << "index" << "|";
    for (int i = 0; i < c.ways; ++i) 
        out << "V|D|" << setw(tag_length) << "tag" + to_string(i) << "|" << setw(data_length) << "data" + to_string(i) << "|";
    out << endl;

    for (int i = 0; i < c.entries.size() * 2; ++i) {
        if (!(i % 2)) {
            out << horizontal * index_length;
            out << "+";
            for (int i = 0; i < c.ways; ++i) {
                out << horizontal << "+" << horizontal << "+";
                out << horizontal * tag_length;
                out << "+";
                out << horizontal * data_length;
                out << "+";
            }
        } else {
            int ind = i / 2;
            out << setw(index_length) << c.indices[ind] << "|";
            for (auto ent: c.entries[c.indices[ind]]) {
                out << ent->getValid() << "|" << ent->getDirty() << "|";
                out << setw(tag_length) << ent->getTag() << "|" << setw(data_length) << ent->getBlock() << "|";
            }
            if (c.entries[c.indices[ind]].size() < c.ways) {
                for (int j = c.entries[c.indices[ind]].size(); j < c.ways; ++j) {
                    out << " | |";
                    out << setw(tag_length) << " " << "|" << setw(data_length) << " " << "|";
                }
            }
        }
        out << endl;
    }
    return out;
}