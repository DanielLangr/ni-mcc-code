#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

extern "C" void spin_lock_naive(void*, void*);
extern "C" void spin_lock(void*, void*);

const uint64_t n = 10'000'000;

bool lock = false;

void f(int thread_num)
{
   thread_local uint64_t cnt = 0;

   auto begin = std::chrono::high_resolution_clock::now();

   for (long i = 0; i < n; i++)
//    spin_lock_naive(&lock, &cnt);
      spin_lock(&lock, &cnt);

   auto end = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
   double avg = (double)duration.count() / (double)cnt;
   std::cout << "Thread " << thread_num << " avrage time: " << avg << " [ns]" << std::endl;
}

int main(int argc, char* argv[])
{
   int n_threads = std::atoi(argv[1]);
   std::cout << "Number of threads: " << n_threads << std::endl;

   std::vector<std::thread> threads;
   threads.reserve(n_threads);
   for (int i = 0; i < n_threads; i++)
      threads.emplace_back(f, i);

   for (auto & t : threads) 
      t.join();
}
