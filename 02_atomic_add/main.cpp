#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

extern "C" void inc(void*);
extern "C" void atomic_inc(void*);

const uint64_t n = 100'000'000;
uint64_t res = 0;

void f()
{
   for (long i = 0; i < n; i++)
//    inc(&res);
      atomic_inc(&res);
}

int main(int argc, char* argv[])
{
   int n_threads = std::atoi(argv[1]);
   std::cout << "Number of threads: " << n_threads << std::endl;

   std::vector<std::thread> threads;
   threads.reserve(n_threads);
   for (int i = 0; i < n_threads; i++)
      threads.emplace_back(f);

   for (auto & t : threads) 
      t.join();

   std::cout << "Expected value: " << n * n_threads << std::endl;
   std::cout << "Obtained value: " << res << std::endl;
}
