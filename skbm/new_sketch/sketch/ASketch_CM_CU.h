#ifndef ASketch_CM_CU_H //must change this MACRO
#define ASketch_CM_CU_H //must change this MACRO
#include "SketchBase.h" //DO NOT change this include
#include "factor.h"//DO NOT change this include
#include "../hash/hashfunction.h"//If you want to use DSAB-builtin hashfunction must include this
#include "../hash/cuckoo_hashing.h" //If you want to use DSAB-builtin hashtable must include this
#include "../atomSketch/myCMCUSketch.h" //If you want to use DSAB-builtin sketch must include this
#include<string>
#include<iostream>
#include<memory.h>
#include <unordered_map>
#include <cstring>
#include <algorithm> 
#include <x86intrin.h>
#include <bmiintrin.h>
#include<memory.h>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <sstream>
using namespace std;

#ifndef ACURCMP
#define ACURCMP
bool AcurCMP(std::pair<string, int> a, std::pair<string, int> b)
{
	return a.second > b.second;
}
#endif // ACURCMP


#define MAX_HASH_NUM 16
#ifndef _PARAMS_H
#define _PARAMS_H

#include <cstdint>

//#define MAX_INSERT_PACKAGE 32000000
//#define MAX_HASH_NUM 20
#define STR_LEN 8

typedef const unsigned char cuc;
typedef unsigned int(*funPtr)(const unsigned char *, unsigned int);

struct HashFunction {
	HashFunction();
	static uint32_t Str2Int(cuc *str, uint32_t hidx, uint32_t len = STR_LEN);
	static funPtr hash_function[14];
};

#define mix(a, b, c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

uint32_t BOB1(cuc *str, uint32_t len) {
	//register ub4 a,b,c,len;
	uint32_t a, b, c;
	uint32_t initval = 2;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

						 /*---------------------------------------- handle most of the key */
	while (len >= 12) {
		a += (str[0] + ((uint32_t)str[1] << 8) + ((uint32_t)str[2] << 16) + ((uint32_t)str[3] << 24));
		b += (str[4] + ((uint32_t)str[5] << 8) + ((uint32_t)str[6] << 16) + ((uint32_t)str[7] << 24));
		c += (str[8] + ((uint32_t)str[9] << 8) + ((uint32_t)str[10] << 16) + ((uint32_t)str[11] << 24));
		mix(a, b, c);
		str += 12;
		len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch (len)              /* all the case statements fall through */ {
	case 11: c += ((uint32_t)str[10] << 24);
	case 10: c += ((uint32_t)str[9] << 16);
	case 9: c += ((uint32_t)str[8] << 8);
		/* the first byte of c is reserved for the length */
	case 8: b += ((uint32_t)str[7] << 24);
	case 7: b += ((uint32_t)str[6] << 16);
	case 6: b += ((uint32_t)str[5] << 8);
	case 5: b += str[4];
	case 4: a += ((uint32_t)str[3] << 24);
	case 3: a += ((uint32_t)str[2] << 16);
	case 2: a += ((uint32_t)str[1] << 8);
	case 1: a += str[0];
		/* case 0: nothing left to add */
	}
	mix(a, b, c);
	/*-------------------------------------------- report the result */
	return c;
}

uint32_t BOB2(cuc *str, uint32_t len) {
	//register ub4 a,b,c,len;
	uint32_t a, b, c;
	uint32_t initval = 31;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

						 /*---------------------------------------- handle most of the key */
	while (len >= 12) {
		a += (str[0] + ((uint32_t)str[1] << 8) + ((uint32_t)str[2] << 16) + ((uint32_t)str[3] << 24));
		b += (str[4] + ((uint32_t)str[5] << 8) + ((uint32_t)str[6] << 16) + ((uint32_t)str[7] << 24));
		c += (str[8] + ((uint32_t)str[9] << 8) + ((uint32_t)str[10] << 16) + ((uint32_t)str[11] << 24));
		mix(a, b, c);
		str += 12;
		len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch (len)              /* all the case statements fall through */ {
	case 11: c += ((uint32_t)str[10] << 24);
	case 10: c += ((uint32_t)str[9] << 16);
	case 9: c += ((uint32_t)str[8] << 8);
		/* the first byte of c is reserved for the length */
	case 8: b += ((uint32_t)str[7] << 24);
	case 7: b += ((uint32_t)str[6] << 16);
	case 6: b += ((uint32_t)str[5] << 8);
	case 5: b += str[4];
	case 4: a += ((uint32_t)str[3] << 24);
	case 3: a += ((uint32_t)str[2] << 16);
	case 2: a += ((uint32_t)str[1] << 8);
	case 1: a += str[0];
		/* case 0: nothing left to add */
	}
	mix(a, b, c);
	/*-------------------------------------------- report the result */
	return c;
}

uint32_t
BOB3(cuc *str, uint32_t len) {
	//register ub4 a,b,c,len;
	uint32_t a, b, c;
	uint32_t initval = 73;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

						 /*---------------------------------------- handle most of the key */
	while (len >= 12) {
		a += (str[0] + ((uint32_t)str[1] << 8) + ((uint32_t)str[2] << 16) + ((uint32_t)str[3] << 24));
		b += (str[4] + ((uint32_t)str[5] << 8) + ((uint32_t)str[6] << 16) + ((uint32_t)str[7] << 24));
		c += (str[8] + ((uint32_t)str[9] << 8) + ((uint32_t)str[10] << 16) + ((uint32_t)str[11] << 24));
		mix(a, b, c);
		str += 12;
		len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch (len)              /* all the case statements fall through */ {
	case 11: c += ((uint32_t)str[10] << 24);
	case 10: c += ((uint32_t)str[9] << 16);
	case 9: c += ((uint32_t)str[8] << 8);
		/* the first byte of c is reserved for the length */
	case 8: b += ((uint32_t)str[7] << 24);
	case 7: b += ((uint32_t)str[6] << 16);
	case 6: b += ((uint32_t)str[5] << 8);
	case 5: b += str[4];
	case 4: a += ((uint32_t)str[3] << 24);
	case 3: a += ((uint32_t)str[2] << 16);
	case 2: a += ((uint32_t)str[1] << 8);
	case 1: a += str[0];
		/* case 0: nothing left to add */
	}
	mix(a, b, c);
	/*-------------------------------------------- report the result */
	return c;
}
uint32_t
BOB4(cuc *str, uint32_t len) {
	//register ub4 a,b,c,len;
	uint32_t a, b, c;
	uint32_t initval = 127;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

						 /*---------------------------------------- handle most of the key */
	while (len >= 12) {
		a += (str[0] + ((uint32_t)str[1] << 8) + ((uint32_t)str[2] << 16) + ((uint32_t)str[3] << 24));
		b += (str[4] + ((uint32_t)str[5] << 8) + ((uint32_t)str[6] << 16) + ((uint32_t)str[7] << 24));
		c += (str[8] + ((uint32_t)str[9] << 8) + ((uint32_t)str[10] << 16) + ((uint32_t)str[11] << 24));
		mix(a, b, c);
		str += 12;
		len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch (len)              /* all the case statements fall through */ {
	case 11: c += ((uint32_t)str[10] << 24);
	case 10: c += ((uint32_t)str[9] << 16);
	case 9: c += ((uint32_t)str[8] << 8);
		/* the first byte of c is reserved for the length */
	case 8: b += ((uint32_t)str[7] << 24);
	case 7: b += ((uint32_t)str[6] << 16);
	case 6: b += ((uint32_t)str[5] << 8);
	case 5: b += str[4];
	case 4: a += ((uint32_t)str[3] << 24);
	case 3: a += ((uint32_t)str[2] << 16);
	case 2: a += ((uint32_t)str[1] << 8);
	case 1: a += str[0];
		/* case 0: nothing left to add */
	}
	mix(a, b, c);
	/*-------------------------------------------- report the result */
	return c;
}

uint32_t RSHash(cuc *str, uint32_t len) {
	uint32_t b = 378551;
	uint32_t a = 63689;
	uint64_t hash = 0;
	for (uint32_t i = 0; i < len; ++i) {
		hash = hash * a + str[i];
		a = a * b;
	}
	return (uint32_t)hash;
}

uint32_t JSHash(cuc *str, uint32_t len) {
	uint64_t hash = 1315423911;
	for (uint32_t i = 0; i < len; ++i) {
		hash ^= ((hash << 5) + str[i] + (hash >> 2));
	}
	return (uint32_t)hash;
}

uint32_t PJWHash(cuc *str, uint32_t len) {
	uint64_t BitsInUnsignedInt = (uint64_t)32;
	uint64_t ThreeQuarters = (uint64_t)((BitsInUnsignedInt * 3) / 4);
	uint64_t OneEighth = (uint64_t)(BitsInUnsignedInt / 8);
	uint64_t HighBits = (uint64_t)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	uint64_t hash = 0;
	uint64_t test = 0;
	for (uint32_t i = 0; i < len; i++) {
		hash = (hash << OneEighth) + str[i];
		if ((test = hash & HighBits) != 0) {
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}
	return (uint32_t)hash;
}

uint32_t ELFHash(cuc *str, uint32_t len) {
	uint64_t hash = 0;
	uint64_t x = 0;
	for (uint32_t i = 0; i < len; ++i) {
		hash = (hash << 4) + str[i];
		if ((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}
	return (uint32_t)hash;
}

uint32_t BKDRHash(cuc *str, uint32_t len) {
	uint64_t seed = 131;
	uint64_t hash = 0;
	for (uint32_t i = 0; i < len; ++i) {
		hash = (hash * seed) + str[i];
	}
	return (uint32_t)hash;
}

uint32_t SDBMHash(cuc *str, uint32_t len) {
	uint64_t hash = 0;
	for (uint32_t i = 0; i < len; ++i) {
		hash = str[i] + (hash << 6) + (hash << 16) - hash;
	}
	return (uint32_t)hash;
}

uint32_t DJBHash(cuc *str, uint32_t len) {
	uint64_t hash = 5381;
	for (uint32_t i = 0; i < len; ++i) {
		hash = ((hash << 5) + hash) + str[i];
	}
	return (uint32_t)hash;
}

uint32_t DEKHash(cuc *str, uint32_t len) {
	uint64_t hash = len;
	for (uint32_t i = 0; i < len; ++i) {
		hash = ((hash << 5) ^ (hash << 27)) ^ str[i];
	}
	return (uint32_t)hash;
}

uint32_t APHash(cuc *str, uint32_t len) {
	uint64_t hash = 0xAAAAAAAA;
	for (uint32_t i = 0; i < len; ++i) {
		if ((i & 1) == 0) {
			hash ^= ((hash << 7) ^ str[i] * (hash >> 3));
		}
		else {
			hash ^= (~(((hash << 11) + str[i]) ^ (hash >> 5)));
		}
	}
	return (uint32_t)hash;
}

unsigned int(*HashFunction::hash_function[])(const unsigned char *, unsigned int) = {
	&BOB1, &BOB2, &BOB3, &BOB4,
	&RSHash, &JSHash,
	&PJWHash, &ELFHash,
	&BKDRHash, &SDBMHash,
	&DJBHash, &DEKHash,
	&APHash, &DJBHash
};

HashFunction::HashFunction() {
	/*
	hash_function[0] = &BOB1;
	hash_function[1] = &BOB2;
	hash_function[2] = &BOB3;
	hash_function[3] = &BOB4;
	hash_function[4] = &RSHash;
	hash_function[5] = &JSHash;
	hash_function[6] = &PJWHash;
	hash_function[7] = &ELFHash;
	hash_function[8] = &BKDRHash;
	hash_function[9] = &SDBMHash;
	hash_function[10] = &DJBHash;
	hash_function[11] = &DEKHash;
	hash_function[12] = &APHash;
	hash_function[13] = &DJBHash;
	*/
}

uint32_t HashFunction::Str2Int(cuc *str, uint32_t hidx, uint32_t len) {
	return hash_function[hidx](str, len);
}

#endif //_PARAMS_H

#ifndef _SPA_H
#define _SPA_H
using namespace std;

class SPA
{
public:
	virtual void insert(unsigned int key, int f) = 0;
};

#endif // _SPA_H
using std::min;
using std::swap;


/*----optional according to your need----*/

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


class ASketch_CM_CU: public SketchBase {
private:
	/*----optional according to your need----*/
	int tot_memory_in_bytes,filter_size,capacity;//parameter
	int w;
	int bucket_num;

	int *new_count;
	int *old_count;
	uint32_t *items;

	int cur_pos;
	int d = 3;
	myCMCUSketch<4,1000,3> * sketch;
	

	BOBHash *bobhash[MAX_HASH_NUM];
    /*----optional according to your need----*/
public:
    using SketchBase::sketch_name;//DO NOT change this declaration
    ASketch_CM_CU()
    {
        /*constructed function MUST BT non-parameter!!!*/
        sketch_name =  "ASketch_CM_CU";//please keep sketch_name the same as class name and .h file name
    }
    void parameterSet(const std::string& parameterName, double  parameterValue)
    {
        /*MUST have this function even empty function body*/

        /*----optional according to your need----*/
        if (parameterName=="tot_memory_in_bytes")
        {

			tot_memory_in_bytes = parameterValue;
            return;
        }
        if (parameterName=="filter_size")
        {
			filter_size = parameterValue;
            return;
        }
        /*----optional according to your need----*/
    }
    void init()
    {
        /*MUST have this function even empty function body*/

        /*----optional according to your need----*/
		w = tot_memory_in_bytes - filter_size * 12;
		sketch = new myCMCUSketch<4,1000, 3>(w);
		bucket_num = filter_size / 16;
		new_count = new int[filter_size];
		old_count = new int[filter_size];
		items = new uint32_t[filter_size];
		
		memset(items, 0, sizeof(items));
		memset(new_count, 0, sizeof(new_count));
		memset(old_count, 0, sizeof(old_count));
		cur_pos = 0;

		for (int i = 0; i < d; i++)
		{
			bobhash[i] = new BOBHash;
			srand(time(0));
			bobhash[i]->SetSeed(uint32_t(rand() % MAX_PRIME32));
		}
        /*----optional according to your need----*/
    }
	uint32_t * get_items()
	{
		return items;
	}

	int * get_freq()
	{
		return new_count;
	}
    void Insert(const char *str, const int & len)
    {
        /*MUST have this function DO NOT change parameter type*/
		int * key = new int;
		memcpy(key, str, 4);
        /*----optional according to your need----*/
		const __m128i item = _mm_set1_epi32(*key);

		for (int i = 0; i < bucket_num; i++)
		{
			__m128i *keys_p = (__m128i *)(items + (i << 4));

			__m128i a_comp = _mm_cmpeq_epi32(item, keys_p[0]);
			__m128i b_comp = _mm_cmpeq_epi32(item, keys_p[1]);
			__m128i c_comp = _mm_cmpeq_epi32(item, keys_p[2]);
			__m128i d_comp = _mm_cmpeq_epi32(item, keys_p[3]);

			a_comp = _mm_packs_epi32(a_comp, b_comp);
			c_comp = _mm_packs_epi32(c_comp, d_comp);
			a_comp = _mm_packs_epi32(a_comp, c_comp);

			int matched = _mm_movemask_epi8(a_comp);

			if (matched != 0)
			{
				//return 32 if input is zero;
				int matched_index = _tzcnt_u32((uint32_t)matched) + (i << 4);
				new_count[matched_index] += 1;
				return;
			}
		}
		if (cur_pos != filter_size)
		{
			items[cur_pos] = *key;
			new_count[cur_pos] = 1;
			old_count[cur_pos] = 0;
			cur_pos++;
			return;
		}
		int estimate_value, min_index, min_value, temp;
		sketch->insert(str);
		estimate_value = sketch->frequencyQuery(str, len);
		min_index = 0;
		min_value = (1 << 30);
		for (int i = 0; i < filter_size; i++)
		{
			if (items[i] != (uint32_t)(-1) && min_value > new_count[i])
			{
				min_value = new_count[i];
				min_index = i;
			}
		}
		if (estimate_value > min_value)
		{
			temp = new_count[min_index] - old_count[min_index];
			for(int i =0;i<temp;++i)
				sketch->insert(str);
			items[min_index] = *key;
			new_count[min_index] = estimate_value;
			old_count[min_index] = estimate_value;
		}
        /*----optional according to your need----*/
    }
    int frequencyQuery(const char *str, const int & len)
    {
         /*MUST have this function DO NOT change function head and parameter type */

        /*----optional according to your need----*/
		int * key = new int;
		memcpy(key, str, 4);
		const __m128i item = _mm_set1_epi32(*key);
		for (int i = 0; i < bucket_num; i++)
		{
			__m128i *keys_p = (__m128i *)(items + (i << 4));

			__m128i a_comp = _mm_cmpeq_epi32(item, keys_p[0]);
			__m128i b_comp = _mm_cmpeq_epi32(item, keys_p[1]);
			__m128i c_comp = _mm_cmpeq_epi32(item, keys_p[2]);
			__m128i d_comp = _mm_cmpeq_epi32(item, keys_p[3]);

			a_comp = _mm_packs_epi32(a_comp, b_comp);
			c_comp = _mm_packs_epi32(c_comp, d_comp);
			a_comp = _mm_packs_epi32(a_comp, c_comp);

			int matched = _mm_movemask_epi8(a_comp);

			if (matched != 0)
			{
				//return 32 if input is zero;
				int matched_index = _tzcnt_u32((uint32_t)matched) + (i << 4);
				return new_count[matched_index];
			}
		}
		return sketch->frequencyQuery(str, len);
        /*----optional according to your need----*/
    }
	std::vector<std::pair <std::string, int> >  topkQuery(const int & k)
    {
        /*MUST have this function DO NOT change function head and parameter type */

        /*----optional according to your need----*/
		std::vector<std::pair <std::string, int> > topkItem;		
		return topkItem;
        /*----optional according to your need----*/
    }
    void reset()
    {
         /*MUST have this function,reset sketch to the initial state */

        /*----optional according to your need----*/
		sketch = new myCMCUSketch<4,1000, 3>(w);
		bucket_num = filter_size / 16;
		new_count = new int[filter_size];
		old_count = new int[filter_size];
		items = new uint32_t[filter_size];

		memset(items, 0, sizeof(items));
		memset(new_count, 0, sizeof(new_count));
		memset(old_count, 0, sizeof(old_count));
		cur_pos = 0;
        /*----optional according to your need----*/
    }
    ~ASketch_CM_CU()
    {
        /*MUST have this function */

        /*----optional according to your need----*/
		
		delete[] new_count;
		delete[] old_count;
		delete[] items;
        /*----optional according to your need----*/
    }

    /*----optional You can add your function----*/
};
REGISTER(ASketch_CM_CU);
#endif//DO NOT change this file