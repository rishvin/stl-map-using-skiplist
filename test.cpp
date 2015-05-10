#include <iostream>
#include <memory.h>

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
using namespace std;

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

#define ITR MAP::iterator
#define RITR MAP::reverse_iterator
void print_msg(const char *msg)
{
	cout << endl << "==================== " << msg << " ====================";
}

void print_end()
{
    cout << endl << "======================================================" << endl;
}

void dump(MAP &mp, const char *msg)
{
	print_msg(msg);
	cout << endl << "SIZE = " << mp.size();
	cout << endl << boolalpha << "EMPTY = " << mp.empty() << endl;
	int prev = -1;
	for(ITR itr = mp.begin(); itr != mp.end(); ++itr)
	{
        assert(prev < itr->first);
        prev = itr->first;
        cout << "(" << itr->first << ", " << itr->second << ")";
	}
	print_end();
}

int main()
{

    MAP f;

	clock_t start, end;
	double cpu_time_used;

	srand(time(NULL));

	start = clock();

	for(int i = 0; i < SIZE; ++i)
	{
		int x = i;//rand() % 100000;
		f.insert(make_pair(x + 1, x + 1));
	}

	dump(f, "DUMPING");
	/* =============================================== */



	f[6] = 7;
	dump(f, "DUMPING AFTER APPLYING [49] = 50 OPERATOR");
	f[49] = 50;
	dump(f, "DUMPING AFTER APPLYING [] OPERATOR");
	/* =============================================== */



	f.erase(7);
	f.erase(5);
	f.erase(90);

	ITR itr = f.find(SIZE + 2);
	//assert(itr == f.end());

	itr = f.find(4);
	//assert(itr != f.end());
	f.erase(itr);

	dump(f, "DUMPING AFTER ERASE");
	/* =============================================== */



	print_msg("DUMPING BOUNDS");
	int bound_value = 89;
    itr = f.lower_bound(bound_value);
	if(itr != f.end())
		cout << endl << "LOWER BOUND OF " << bound_value << " = " << itr->first;
	else cout << endl << "LOWER BOUND NOT FOUND FOR "  << bound_value;

	itr = f.upper_bound(bound_value);
	if(itr != f.end())
		cout << endl << "UPPER BOUND OF " << bound_value << " = " << itr->first;
	else  cout << endl << "UPPER BOUND NOT FOUND FOR " << bound_value;
	print_end();
	/* =============================================== */


	itr = f.find(10);
	f.insert(itr, make_pair(40, 15));

    dump(f, "After inserting hint = 10, 40");

	itr = f.find(56);
	f.insert(itr, make_pair(199, 45));

    dump(f, "After inserting hint = 56, 199");

	itr = f.find(4);
	f.insert(itr, make_pair(500, 34));

	dump(f, "After inserting hint = 4, 500");

	itr = f.find(199);
	f.insert(itr, make_pair(5, 5));

	itr = f.find(900);
	f.insert(itr, make_pair(13, 13));

	itr = f.find(6);
	f.insert(itr, make_pair(15, 15));

	dump(f, "DUMPING AFTER HINT INSERTION");
	/* =============================================== */


	MAP f1;
	f1.insert(f.begin(), f.end());

	dump(f1, "DUMPING AFTER RANGE INSERT");
	/* =============================================== */


	f1 = f;

	dump(f1, "DUMPING AFTER ASSIGNMENT");
	/* =============================================== */



	MAP f2(f);

	dump(f2, "DUMPING WITH COPY CTOR");
	/* =============================================== */

	f2.clear();
	dump(f2, "DUMPING AFTER CLEAR");

	f2[10] = 12;
	dump(f2, "DUMPING AFTER INSERT");
    /* =============================================== */

    f.swap(f2);
    dump(f, "DUMPING f AFTER SWAP WITH f2");
    dump(f2, "DUMPING f2 AFTER SWAP WITH f");
    /* =============================================== */


    itr = f.find(20);
    ITR itr2 = f.find(90);
    f.erase(itr, itr2);
    dump(f, "DUMPING f AFTER RANGE ERASE");


	end = clock();
	cout << endl << endl << "-----------------";
	cout << endl << "TICKS ELAPSED = " << (end - start) << endl;

	return 0;
}
