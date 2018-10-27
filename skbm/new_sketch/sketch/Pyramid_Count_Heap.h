#ifndef Pyramid_Count_Heap_H //must change this MACRO
#define Pyramid_Count_Heap_H //must change this MACRO
#include "SketchBase.h" //DO NOT change this include
#include "factor.h"//DO NOT change this include
#include "../hash/hashfunction.h"//If you want to use DSAB-builtin hashfunction must include this
#include "../hash/cuckoo_hashing.h" //If you want to use DSAB-builtin hashtable must include this
/*----optional according to your need----*/
#include<string>
#include<iostream>
#include<memory.h>
#include <algorithm>
#include <cstring>
#include <string.h>
#include <math.h>

#ifndef _PyramidPARAMS_H
#define _PyramidPARAMS_H

#define MAX_INSERT_PACKAGE 1000000

#define MAX_HASH_NUM_P 20

#define FILTER_SIZE 32

#define COUNTER_SIZE 16

#define LOW_HASH_NUM 4

typedef long long lint;
typedef unsigned int uint;
typedef unsigned long long int uint64;
#endif //_PARAMS_H
using namespace std;
/*----optional according to your need----*/

/*----builtin hashfunction----*/
/*
DSAB-builtin hashfunction type:BOBHash\
HOW TO USE:
    define: e.g. BOBHash myhash
    setseed: e.g. myhash.SetSeed(1001)
    calculate hash: e.g. myhash.Run(const char *str, const int & len)
*/
/*----builtin hashfunction----*/

/*----builtin hashTable----*/
/*
DSAB-builtin hashTable type:cuckoo_hashtable\
HOW TO USE:
define: cuckoo::CuckooHashing<key_len> ht;
!!!MUST init: ht.init(capacity)
bool insert(char * key, uint32_t val, int from_k = -1, int remained = 5)
bool query(char * key, uint32_t & val)
bool find(char * key)
bool erase(char * key)
*/
/*----builtin hashTable----*/

/*----SketchBase virtual function must be finished----*/
/*
virtual ~SketchBase(){}
virtual void Insert(const char * str, const int & len) = 0;
virtual int frequencyQuery(const char * str, const int & len) = 0;
virtual std::vector<std::pair <std::string, int> > topkQuery(const int & k) = 0;
virtual void parameterSet(const std::string& parameterName, double  parameterValue)=0;
virtual void init() = 0;
virtual void reset() = 0;//reset sketch to the initial state
*/
/*----SketchBase virtual function must be finished----*/


class Pyramid_Count_Heap: public SketchBase {
private:
	/*----optional according to your need----*/
	int word_num,d,word_size,capacity;//parameter
	uint64 *counter[60];
	bool *flag[60];
	int word_index_size, counter_index_size;
	int counter_num;
	BOBHash * bobhash[MAX_HASH_NUM_P];
	//word_num is the number of words in the first level.
	typedef pair <string, int> KV;
	typedef pair <int, string> VK;
	VK * heap;
	int heap_element_num;
	unordered_map<string, uint32_t> ht;
	void heap_adjust_down(int i) {
		while (i < heap_element_num / 2) {
			int l_child = 2 * i + 1;
			int r_child = 2 * i + 2;
			int larger_one = i;
			if (l_child < heap_element_num && heap[l_child] < heap[larger_one]) {
				larger_one = l_child;
			}
			if (r_child < heap_element_num && heap[r_child] < heap[larger_one]) {
				larger_one = r_child;
			}
			if (larger_one != i) {
				swap(heap[i], heap[larger_one]);
				swap(ht[heap[i].second], ht[heap[larger_one].second]);
				heap_adjust_down(larger_one);
			}
			else {
				break;
			}
		}
	}

	void heap_adjust_up(int i) {
		while (i > 1) {
			int parent = (i - 1) / 2;
			if (heap[parent] <= heap[i]) {
				break;
			}
			swap(heap[i], heap[parent]);
			swap(ht[heap[i].second], ht[heap[parent].second]);
			i = parent;
		}
	}
    /*----optional according to your need----*/
public:
    using SketchBase::sketch_name;//DO NOT change this declaration
    Pyramid_Count_Heap()
    {
        /*constructed function MUST BT non-parameter!!!*/
        sketch_name =  "Pyramid_Count_Heap";//please keep sketch_name the same as class name and .h file name
    }
    void parameterSet(const std::string& parameterName, double  parameterValue)
    {
        /*MUST have this function even empty function body*/

        /*----optional according to your need----*/
		if (parameterName == "capacity")
		{
			capacity = parameterValue;
			return;
		}
        if (parameterName=="word_num")
        {

			word_num = parameterValue;
            return;
        }
        if (parameterName=="d")
        {
            d = parameterValue;
            return;
        }
         if (parameterName=="word_size")
        {
			word_size = parameterValue;
            return;
        }
        /*----optional according to your need----*/
    }
    void init()
    {
        /*MUST have this function even empty function body*/

        /*----optional according to your need----*/
		heap = new VK[capacity];
		memset(heap, 0, capacity * sizeof(VK));
		for (int i = 0; i < capacity; ++i) {
			heap[i].first = 0;
		}


		heap_element_num = 0;
		//for calculating the four hash value constrained in one certain word;
		word_index_size = 18;

		counter_index_size = (int)(log(word_size) / log(2)) - 2;//4-8->16-256 counters in one word;
		counter_num = (word_num << counter_index_size);


		for (int i = 0; i < 15; i++)
		{
			counter[i] = new uint64[word_num >> i];
			flag[i] = new bool[counter_num >> i];
			memset(counter[i], 0, sizeof(uint64) * (word_num >> i));
			memset(flag[i], false, sizeof(bool) * (counter_num >> i));
		}

		for (int i = 0; i < d * 2; i++)
		{
			bobhash[i] = new BOBHash;
			bobhash[i]->SetSeed(i + 1000);
		}

        /*----optional according to your need----*/
    }
    void Insert(const char *str, const int & len)
    {
        /*MUST have this function DO NOT change parameter type*/

        /*----optional according to your need----*/
			int min_value = 1 << 30;
			int value[MAX_HASH_NUM_P], index[MAX_HASH_NUM_P];

			int flag_t = 0xFFFF;


			int word_index, offset, hash_value;

			hash_value = (bobhash[0]->Run(str, 4));
			word_index = (hash_value & ((1 << word_index_size) - 1)) % word_num;
			
			hash_value >>= word_index_size;
		
			for (int i = 0; i < 2; i++)
			{
				offset = (hash_value & 0xFFF) % (1 << counter_index_size);
				index[i] = (word_index << counter_index_size) + offset;

				hash_value >>= counter_index_size;
			}

			hash_value = (bobhash[1]->Run(str, 4));
			word_index = (hash_value & ((1 << word_index_size) - 1)) % word_num;
			hash_value >>= word_index_size;
		
			for (int i = 2; i < 4; i++)
			{
				offset = (hash_value & 0xFFF) % (1 << counter_index_size);
				index[i] = (word_index << counter_index_size) + offset;

				hash_value >>= counter_index_size;
			}
			
			for (int i = 0; i < d; i++)
			{
				word_index = (index[i] >> 4);
				offset = (index[i] & 0xF);


				if (((flag_t >> offset) & 1) == 0)
					continue;

				flag_t &= (~(1 << offset));

				
			
				value[i] = (counter[0][word_index] >> (offset << 2)) & 0xF;
				
				int	g = (bobhash[i + d]->Run(str,4)) % 2;
			
				//++
				if (g == 0)
				{
					//posi
					
					if (flag[0][index[i]] == false)
					{
						if (value[i] == 15)
						{
							counter[0][word_index] &= (~((uint64)0xF << (offset << 2)));
							carry(index[i]);
						}
						else
						{
							counter[0][word_index] += ((uint64)0x1 << (offset << 2));
						}
					}
					//nega
					else
					{
						if (value[i] == 1)
						{
							counter[0][word_index] &= (~((uint64)0xF << (offset << 2)));
							flag[0][index[i]] = false;
						}
						else
						{
							counter[0][word_index] -= ((uint64)0x1 << (offset << 2));
						}
					}
				}
				//--
				else
				{
					//posi
				
					if (flag[0][index[i]] == false)
					{
						if (value[i] == 0)
						{
							counter[0][word_index] += ((uint64)0x1 << (offset << 2));
							flag[0][index[i]] = true;
						}
						else
						{
							counter[0][word_index] -= ((uint64)0x1 << (offset << 2));
						}
					}
					else
					{
						if (value[i] == 15)
						{
							counter[0][word_index] &= (~((uint64)0xF << (offset << 2)));

							down_carry(index[i]);
						}
						else
						{
							counter[0][word_index] += ((uint64)0x1 << (offset << 2));

						}
					}
				}
			}
		
			int tmin = frequencyQuery(str, len);
			string str_key = string(str, len);
			if (ht.find(str_key) != ht.end()) {
				heap[ht[str_key]].first++;
				heap_adjust_down(ht[str_key]);
			}
			else if (heap_element_num < capacity) {
				heap[heap_element_num].second = str_key;
				heap[heap_element_num].first = tmin;
				ht[str_key] = heap_element_num++;
				heap_adjust_up(heap_element_num - 1);
			}
			else if (tmin > heap[0].first) {
				VK & kv = heap[0];
				ht.erase(kv.second);
				kv.second = str_key;
				kv.first = tmin;
				ht[str_key] = 0;
				heap_adjust_down(0);
			}
			return;
		
        /*----optional according to your need----*/
    }
    int frequencyQuery(const char *str, const int & len)
    {
         /*MUST have this function DO NOT change function head and parameter type */

        /*----optional according to your need----*/
		int temp, temp2;
		int res[MAX_HASH_NUM], value[MAX_HASH_NUM], index[MAX_HASH_NUM];
		int flag_t = 0xFFFF;
		int hash_value;

		int word_index, offset;
		hash_value = (bobhash[0]->Run(str, 4));
		word_index = (hash_value & ((1 << word_index_size) - 1)) % word_num;
		hash_value >>= word_index_size;

		for (int i = 0; i < 2; i++)
		{
			offset = (hash_value & 0xFFF) % (1 << counter_index_size);
			index[i] = (word_index << counter_index_size) + offset;

			hash_value >>= counter_index_size;
		}

		hash_value = (bobhash[1]->Run(str,4));
		word_index = (hash_value & ((1 << word_index_size) - 1)) % word_num;
		hash_value >>= word_index_size;

		for (int i = 2; i < 4; i++)
		{
			offset = (hash_value & 0xFFF) % (1 << counter_index_size);
			index[i] = (word_index << counter_index_size) + offset;

			hash_value >>= counter_index_size;
		}



		for (int i = 0; i < d; i++)
		{
			word_index = (index[i] >> 4);
			offset = (index[i] & 0xF);


			value[i] = (counter[0][word_index] >> (offset << 2)) & 0xF;

			int	g = (bobhash[i + d]->Run(str, 4)) % 2;

			if (flag[0][index[i]] == false)
				temp = value[i] + get_value(index[i]);
			else
				temp = 0 - value[i] + get_value(index[i]);

			res[i] = (g == 0 ? temp : -temp);
		}

		sort(res, res + d);
		int r;
		if (d % 2 == 0)
		{
			r = (res[d / 2] + res[d / 2 - 1]) / 2;
		}
		else
		{
			r = res[d / 2];
		}
		return r;
        /*----optional according to your need----*/
    }
	std::vector<std::pair <std::string, int> >  topkQuery(const int & k)
    {
        /*MUST have this function DO NOT change function head and parameter type */

        /*----optional according to your need----*/
		std::vector<std::pair <std::string, int> > topkItem;
		VK * a = new VK[capacity];
		for (int i = 0; i < capacity; ++i) {
			a[i] = heap[i];
		}
		sort(a, a + capacity);
		int i;
		for (i = 0; i < k && i < capacity; ++i) {
			topkItem.push_back(make_pair(a[capacity - 1 - i].second, a[capacity - 1 - i].first));

		}
		return topkItem;
        /*----optional according to your need----*/
    }
	void down_carry(int index)
	{
		int left_or_right;

		int value;
		int word_index = index >> 4;
		int offset = index & 0xF;
		int counter_index;

		for (int i = 1; i < 15; i++)
		{

			left_or_right = word_index & 1;
			word_index >>= 1;

			counter_index = (word_index << 4) + offset;

			counter[i][word_index] |= ((uint64)0x1 << (2 + left_or_right + (offset << 2)));
			value = (counter[i][word_index] >> (offset << 2)) & 0x3;

			//posi
			if (flag[i][counter_index] == false)
			{
				if (value == 0)
				{
					counter[i][word_index] += ((uint64)0x1 << (offset << 2));
					flag[i][counter_index] = true;
					return;
				}
				else
				{
					counter[i][word_index] -= ((uint64)0x1 << (offset << 2));
					return;
				}
			}
			//nega
			else
			{
				if (value == 3)
				{
					counter[i][word_index] &= (~((uint64)0x3 << (offset << 2)));
				}
				else
				{
					counter[i][word_index] += ((uint64)0x1 << (offset << 2));
					return;
				}
			}

		}
	}

	void carry(int index)
	{
		int left_or_right;

		int value;
		int word_index = index >> 4;
		int offset = index & 0xF;
		int counter_index;

		for (int i = 1; i < 15; i++)
		{

			left_or_right = word_index & 1;
			word_index >>= 1;

			counter_index = (word_index << 4) + offset;

			counter[i][word_index] |= ((uint64)0x1 << (2 + left_or_right + (offset << 2)));
			value = (counter[i][word_index] >> (offset << 2)) & 0x3;

			//posi
			if (flag[i][counter_index] == false)
			{
				if (value == 3)
				{
					counter[i][word_index] &= (~((uint64)0x3 << (offset << 2)));
				}
				else
				{
					counter[i][word_index] += ((uint64)0x1 << (offset << 2));
					return;
				}
			}
			//nega
			else
			{
				if (value == 1)
				{
					counter[i][word_index] -= ((uint64)0x1 << (offset << 2));

					flag[i][counter_index] = false;
					return;
				}
				else
				{
					counter[i][word_index] -= ((uint64)0x1 << (offset << 2));
					return;
				}
			}
		}
	}

	int get_value(int index)
	{
		int left_or_right;
		int anti_left_or_right;

		int value;
		int word_index = index >> 4;
		int offset = index & 0xF;


		int high_value = 0;

		for (int i = 1; i < 15; i++)
		{

			left_or_right = word_index & 1;
			anti_left_or_right = (left_or_right ^ 1);

			word_index >>= 1;

			value = (counter[i][word_index] >> (offset << 2)) & 0xF;

			if (((value >> (2 + left_or_right)) & 1) == 0)
				return high_value;

			int t = ((value & 3) - ((value >> (2 + anti_left_or_right)) & 1)) * (1 << (2 + 2 * i));

			high_value += (flag[i][(word_index << 4) + offset] == false) ? t : -t;
		}
	}
    void reset()
    {
         /*MUST have this function,reset sketch to the initial state */

        /*----optional according to your need----*/
        /*----optional according to your need----*/
    }
    ~Pyramid_Count_Heap()
    {
        /*MUST have this function */

        /*----optional according to your need----*/
        /*----optional according to your need----*/
    }
    /*----optional You can add your function----*/
};
REGISTER(Pyramid_Count_Heap);
#endif//DO NOT change this file