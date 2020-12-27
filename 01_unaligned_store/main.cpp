//#include <atomic>
#include <iostream>
#include <thread>

extern "C" void store(char*);
extern "C" short load(char*);

alignas(64) char buf[65];

char* ptr = buf + 63;
//char* ptr = buf + 0;

static long n = 1'000'000'000L;

//std::atomic<bool> finished {false};

void f1()
{
// while (!finished)
   for (long i = 0; i < n; i++)
      store(ptr);
}

void f2()
{
   long v0x0000 = 0;
   long v0x0101 = 0;
   long v0x0100 = 0;
   long v0x0001 = 0;
   long other = 0;

   for (long i = 0; i < n; i++)
   {
      short a = load(ptr);

      if (a == 0x0000) v0x0000++;
      else if (a == 0x0101) v0x0101++;
      else if (a == 0x0100) v0x0100++;
      else if (a == 0x0001) v0x0001++;
      else other++;
   }

// finished = true;
   
   std::cout << "0x0000: " << v0x0000 << std::endl;
   std::cout << "0x0101: " << v0x0101 << std::endl;
   std::cout << "0x0100: " << v0x0100 << std::endl;
   std::cout << "0x0001: " << v0x0001 << std::endl;
   std::cout << "other: " << other << std::endl;
}


int main()
{
   std::thread t1(f1);
   std::thread t2(f2);

   t1.join();
   t2.join();
}
