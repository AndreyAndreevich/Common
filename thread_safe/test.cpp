//
// Created by Andrey on 2019-01-16.
//

#include "thread_safe_queue.h"
#include <iostream>

int main() {
    ts::ThreadPool<std::function<void()>> threadPool{4};
    for (size_t count = 0; count < 1000; count++) {
        threadPool.add_task([count] {std::cout << count << std::endl;})
    }
    threadPool.start();

    return 0;
}