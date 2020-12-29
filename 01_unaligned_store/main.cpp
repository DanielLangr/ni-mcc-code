#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>

extern "C" void store(void*);
extern "C" uint16_t load(void*);

alignas(64) char buf[65];

static long n = 1'000'000'000L;

void f1(void* ptr)
{
   for (long i = 0; i < n; i++)
      store(ptr);
}

void f2(void* ptr)
{
   long v[258]{};

   for (long i = 0; i < n; i++)
   {
      uint16_t a = load(ptr);
      v[a]++;
   }

   std::cout << "0x0000: " << v[0x0000] << std::endl;
   std::cout << "0x0101: " << v[0x0101] << std::endl;
   std::cout << "0x0100: " << v[0x0100] << std::endl;
   std::cout << "0x0001: " << v[0x0001] << std::endl;

   std::cout << "expected sum: " << n << std::endl;
   long sum = v[0x0000] + v[0x0101] + v[0x0100] + v[0x0001];
   std::cout << "sum: " << sum << std::endl;
}


int main(int arc, char* argv[])
{
   int offset = std::atoi(argv[1]);
   void* ptr = buf + offset;

   std::thread t1(f1, ptr);
   std::thread t2(f2, ptr);

   t1.join();
   t2.join();
}
