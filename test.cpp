#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
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

void small_tick_more() {
    constexpr int N = 10000;
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

void large_tick_more() {
    constexpr int N = 2048;
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

void keep_allocate() {
    constexpr int N = 2048;
    std::vector<std::string*> v;
    while(true) {
        v.push_back(new std::string(10 * 4 * 1024, 'A'));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(){
    // malloc_stats_print(NULL, NULL, NULL);
    // return 0;
    // large();
    // large_tick_more();
    keep_allocate();
}