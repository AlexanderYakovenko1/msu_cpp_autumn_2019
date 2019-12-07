#include <iostream>
#import <chrono>
#include <thread>
#include <cstdlib>
#include "pool.h"

void slow()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
}

int waste_time()
{
    int tmp = 0;
    for (int i = 0; i < 10000000; ++i) {
        tmp = rand();
    }
    return tmp;
}

int main()
{
    ThreadPool pool(8);

    auto thr1 = pool.exec(waste_time);
    auto thr2 = pool.exec(slow);

    for (int i = 0; i < 40; ++i) {
        std::cout << pool.exec([i]() { return i; }).get() << std::endl;
    }

    int buf = thr1.get();
    thr2.get();

    std::cout << buf << std::endl;

    return 0;
}