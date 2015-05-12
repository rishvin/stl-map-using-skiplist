#include <iostream>
#include <memory.h>

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
using namespace std;

//
//#define ORG_MAP
//


#ifdef ORG_MAP
	#include <map>
	#define MAP map<int, int>
#else
	#include <bits/stl_smap.h>
	#define MAP smap<int, int>
#endif

#define SIZE 10000
#define LOW 1000

#define ITR MAP::iterator
#define RITR MAP::reverse_iterator

#define START               \
    clock_t start, end;     \
	start = clock();

#define ELAPSED     \
    end = clock();

int g_random[SIZE];

void gen_random()
{
    int d = 1;
    for(int i = 0; i < SIZE; ++i)
    {
        g_random[i] = LOW + i + d++;
    }
}

clock_t test_insert(MAP &mp)
{
    START
    int x;
    for(int i = 0; i < SIZE; ++i)
	{
		x = g_random[i];
		mp.insert(make_pair(x, x));
	}
	assert(mp.size() == SIZE);
	return ELAPSED
}

clock_t test_walk(MAP &mp)
{
    START
    ITR itr = mp.begin();
    int prev = itr->first;
    for(++itr; itr != mp.end(); ++itr)
    {
        assert(prev < itr->first);
    }
    return ELAPSED
}

clock_t test_rev_walk(MAP &mp)
{
    START
    RITR itr = mp.rbegin();
    int prev = itr->first;
    for(++itr; itr != mp.rend(); ++itr)
    {
        assert(prev > itr->first);
    }
    return ELAPSED
}


clock_t test_search_by_key(MAP &mp)
{
    START
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        itr = mp.find(g_random[i]);
        assert(itr != mp.end());
    }
    return ELAPSED
}

clock_t test_assign(MAP &mp, MAP &mp2)
{
    START
    mp2 = mp;
    return ELAPSED
}

clock_t test_lower_bound(MAP &mp)
{
    START
    ITR itr, itr2;
    for(int i = 0; i < SIZE; ++i)
    {
        itr = mp.lower_bound(LOW + i);
        itr2 = mp.find(LOW + i);
        if(itr2 != mp.end())
            assert(itr->first == (LOW + i));
        else if(itr != mp.end())
            assert(itr->first > (LOW + i));
    }
    return ELAPSED
}

clock_t test_upper_bound(MAP &mp)
{
    START
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        itr = mp.upper_bound(LOW + i);
        if(itr != mp.end())
            assert(itr->first > (LOW + i));
    }
    return ELAPSED
}


clock_t test_erase_by_value(MAP &mp)
{
    START
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        assert(mp.erase(g_random[i]) == 1);
        itr = mp.find(g_random[i]);
        assert(itr == mp.end());
    }
    assert(mp.size() == 0);
    assert(mp.empty() == true);
    return ELAPSED
}

clock_t test_erase_by_hint(MAP &mp)
{
    START
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        itr = mp.find(g_random[i]);
        assert(itr != mp.end());
        mp.erase(itr);
        itr = mp.find(g_random[i]);
        assert(itr == mp.end());
    }
    assert(mp.size() == 0);
    assert(mp.empty() == true);
    return ELAPSED
}

clock_t test_clear(MAP &mp)
{
    START
    mp.clear();
    assert(mp.empty() == true);
    assert(mp.size() == 0);
    return ELAPSED
}

clock_t test_range_insert(MAP &mp)
{
    START
    MAP tmp;
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        itr = mp.find(i + 4 * SIZE);
        assert(itr == mp.end());
        tmp.insert(make_pair((i + 4 * SIZE), (i + 4 * SIZE)));
    }
    mp.insert(tmp.begin(), tmp.end());
    assert(mp.size() == SIZE + SIZE);

    itr = mp.find(0 + 4 * SIZE);

    mp.erase(itr, mp.end());
    assert(mp.size() == SIZE);
    return ELAPSED
}

clock_t test_operator_insert(MAP &mp)
{
    START
    MAP tmp;
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        itr = mp.find(i + 4 * SIZE);
        assert(mp[i + 4 * SIZE] == 0);
        mp[i + 4 * SIZE] = i + 4 * SIZE;
        itr = mp.find(i + 4 * SIZE);
        assert(mp[i + 4 * SIZE] != 0);
    }

    assert(mp.size() == SIZE + SIZE);
    itr = mp.find(0 + 4 * SIZE);

    mp.erase(itr, mp.end());
    assert(mp.size() == SIZE);
    return ELAPSED
}

clock_t test_at(MAP &mp)
{
    START
    MAP tmp;
    ITR itr;
    for(int i = 0; i < SIZE; ++i)
    {
        try
        {
            mp.at(g_random[i]);
        }
        catch(...)
        {
            assert(false);
        }
    }
    return ELAPSED
}

int walk_and_get_count(MAP &mp)
{
    ITR itr = mp.begin();
    if(itr == mp.end())
        return 0;

    int count = 1;
    int prev = itr->first;
    for(++itr; itr != mp.end(); ++itr)
    {
        count++;
        assert(prev < itr->first);
    }
    return count;
}

clock_t test_swap(MAP &mp)
{
    START
    MAP tmp;
    for(int i = 0; i < SIZE / 2; ++i)
    {
        tmp.insert(make_pair(g_random[i], g_random[i]));
    }
    assert(tmp.size() == SIZE / 2);

    tmp.swap(mp);

    assert(mp.size() == SIZE / 2);
    assert(tmp.size() == SIZE);

    assert(walk_and_get_count(mp) == mp.size());
    assert(walk_and_get_count(tmp) == tmp.size());

    mp.swap(tmp);

    assert(mp.size() == SIZE);
    assert(tmp.size() == SIZE / 2);

    assert(walk_and_get_count(mp) == mp.size());
    assert(walk_and_get_count(tmp) == tmp.size());

    return ELAPSED
}

void result(const char *test_case, clock_t (*fn)(MAP&mp), MAP &mp)
{

    clock_t t = fn(mp);
    cout << endl << "["<< test_case << "]";
    cout << endl << "Tick elapsed = " << t;
    cout << endl << "[DONE]" << endl;
}

int main()
{
	srand(time(NULL));
    gen_random();
    cout << endl << "#### TESTING STL SKIPLIST MAP ####" << endl;

    clock_t t;

    MAP f;

    result("INSERT", test_insert, f);

    result("WALK", test_walk, f);

    result("REVERSE WALK", test_rev_walk, f);

    result("SWAP", test_swap, f);

    result("SEARCH BY KEY", test_search_by_key, f);

    result("ERASE BY KEY", test_erase_by_value, f);
    test_insert(f);

    result("ERASE BY HINT", test_erase_by_hint, f);
    test_insert(f);

    result("LOWER BOUND", test_lower_bound, f);

    result("UPPER BOUND", test_upper_bound, f);

    result("RANGE INSERT", test_range_insert, f);

    result("OPERATOR [] INSERT", test_operator_insert, f);

    result("AT", test_at, f);

    result("CLEAR", test_clear, f);
    test_insert(f);


    MAP f2;
    f2 = f;

    result("ASSIGNED WALK", test_walk, f2);

    result("ASSIGNED REVERSE WALK", test_rev_walk, f2);

    result("SWAP", test_swap, f2);

    result("ASSIGNED SEARCH BY KEY", test_search_by_key, f2);

    result("ASSIGNED ERASE BY KEY", test_erase_by_value, f2);
    test_insert(f2);

    result("ASSIGNED ERASE BY HINT", test_erase_by_hint, f2);
    test_insert(f2);

    result("ASSIGNED LOWER BOUND", test_lower_bound, f2);

    result("ASSIGNED UPPER BOUND", test_upper_bound, f2);

    result("RANGE INSERT", test_range_insert, f2);

    result("OPERATOR [] INSERT", test_operator_insert, f2);

    result("AT", test_at, f2);

    result("CLEAR", test_clear, f2);
    test_insert(f2);



    MAP f3(f2);

    result("COPY CTOR WALK", test_walk, f3);

    result("COPY CTOR REVERSE WALK", test_rev_walk, f3);

    result("SWAP", test_swap, f3);

    result("COPY CTOR SEARCH BY KEY", test_search_by_key, f3);

    result("COPY CTOR ERASE BY KEY", test_erase_by_value, f3);
    test_insert(f3);

    result("COPY CTOR ERASE BY HINT", test_erase_by_hint, f3);
    test_insert(f3);

    result("COPY CTOR LOWER BOUND", test_lower_bound, f3);

    result("COPY CTOR UPPER BOUND", test_upper_bound, f3);

    result("COPY CTOR RANGE INSERT", test_range_insert, f3);

    result("OPERATOR [] INSERT", test_operator_insert, f3);

    result("AT", test_at, f3);

    result("COPY CTOR CLEAR", test_clear, f3);
    test_insert(f3);

    cout << endl << "#### TESTING STL SKIPLIST MAP DONE####" << endl;
    cout << endl;
	return 0;
}
