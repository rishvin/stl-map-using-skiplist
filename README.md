This is stl map implementation using skiplist.


Development environment:
  gcc - 4.6.3
  ubuntu 12.04 LTS 32 bit


Setting up:
  Assuming you have gcc 4.6.3 installed, 
  1. copy file under 4.6.3/bits folder to /usr/include/c++/4.6.3/bits
  2. copy test.cpp to any location and compile using g++ test.cpp
  3. before compiling make sure your include path is set to /usr/include/c++/4.6.3/bits
  

Files:
  1. test.cpp - Contains basic test cases
  2. 4.6.3/bits/stl_skiplist.h - Contains the skiplist logic
  3. 4.6.3/bit/stl_smap  - Wrapper around the stl_skiplist.h


What's next:    
  1. Balacing of list after deletion.
