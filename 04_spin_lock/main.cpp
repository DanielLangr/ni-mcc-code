#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <time.h>

class Spinlock_naive
{
   std::atomic<uint8_t> l_ {0};

public:
   void lock()
   {
      while ( l_.exchange(1, std::memory_order_acquire) ) ;
   }

   void unlock()
   {
      l_.store(0, std::memory_order_release);
   }
};

class Spinlock
{
   std::atomic<uint8_t> l_ {0};

public:
   void lock()
   {
      while (true)
      {
         while ( l_.load(std::memory_order_relaxed) ) ;

         if ( l_.exchange(1, std::memory_order_acquire) == 0) 
            break;
      }
   }

   void unlock()
   {
      l_.store(0, std::memory_order_release);
   }
};

void print_clock_res()
{
   timespec ts;
   int res = clock_getres(CLOCK_MONOTONIC, &ts);

   if (res != 0)
      throw std::runtime_error("clock_getres error returned");

   std::cout << "clock resolution: " << ts.tv_sec << " [s], " << ts.tv_nsec << " [ns]" << std::endl;
}

static const long n = 1'000'000;

long cnt = 0;
//Spinlock_naive sln;
Spinlock sln;

void f(int thread_num)
{
   long temp = 0;

   while (temp < n)
   {
      sln.lock();

      temp = ++cnt;

      sln.unlock();
   }
}

int main(int argc, char* argv[])
{
   int n_threads = std::atoi(argv[1]);
   std::cout << "Number of threads: " << n_threads << std::endl;

   print_clock_res();

   std::vector<std::thread> threads;
   threads.reserve(n_threads);

   auto begin = std::chrono::high_resolution_clock::now();

   for (int i = 0; i < n_threads; i++)
      threads.emplace_back(f, i);

   for (auto & t : threads) 
      t.join();

   auto end = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
   double avg = (double)duration.count() / (double)cnt;
   std::cout << "Time per iteration: " << avg << " [ns]" << std::endl;
}
