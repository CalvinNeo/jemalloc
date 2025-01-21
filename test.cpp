#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <jemalloc/jemalloc.h>

void small() {
    constexpr int N = 500;
    std::string * buf[N];
    for (int i = 0; i < N; i++) {
        buf[i] = new std::string(512, 'A');
    }
    for (int i = 0; i < N; i++) {
        delete buf[i];
    }
    printf("%d\n", 100);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    int * a = new int(10);
    printf("%d\n", 111);
    printf("%d\n", 222);
    std::this_thread::sleep_for(std::chrono::seconds(60));
}

void large() {
    constexpr int N = 10;
    std::string * buf[N];
    for (int i = 0; i < N; i++) {
        buf[i] = new std::string(10 * 4 * 1024, 'A');
    }
    for (int i = 0; i < N; i++) {
        delete buf[i];
    }
    printf("%d\n", 100);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    int * a = new int(10);
    printf("%d\n", 111);
    printf("%d\n", 222);
    std::this_thread::sleep_for(std::chrono::seconds(60));
}


int main(){
    // malloc_stats_print(NULL, NULL, NULL);
    // return 0;
    large();
}