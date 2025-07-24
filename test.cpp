#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <cstring>
#include <jemalloc/jemalloc.h>

long get_page_num() {
    long rss = 0L;
    FILE* fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL) {
        return -1;
    }
    if (fscanf(fp, "%*s%ld", &rss) != 1) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return rss;
}

long get_rss() {
    FILE* fp = fopen("/proc/self/status", "r");
    if (fp == NULL) {
        return -1;
    }
    
    char line[128];
    long rss = -1;
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line + 6, "%ld", &rss);
            break;
        }
    }
    
    fclose(fp);
    return rss; // 单位为 KB
}

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

void lll() {
    size_t allocated, retained, mapped;
    static size_t p_allocated, p_retained, p_mapped, p_rss;
    size_t sz = sizeof(allocated);
    size_t epoch;
    mallctl("epoch", NULL, NULL, &epoch, sizeof(epoch));
    mallctl("stats.allocated", &allocated, &sz, NULL, 0);
    mallctl("stats.retained", &retained, &sz, NULL, 0);
    mallctl("stats.mapped", &mapped, &sz, NULL, 0);
    size_t rss = get_rss();
    printf("rss %lu allocated %lu retained %lu mapped %lu, (%lu, %lu, %lu, %lu)\n", 
        rss, allocated / 1024, retained / 1024, mapped / 1024,
        rss-p_rss, (allocated-p_allocated) / 1024, (retained-p_retained) / 1024, (mapped-p_mapped) / 1024
    );
    fflush(stderr);
    fflush(stdout);
    p_allocated = allocated;
    p_retained = retained;
    p_mapped = mapped;
    p_rss = rss;
}

void keep_allocate() {
    constexpr int N = 2048;
    std::vector<std::string*> v;
    lll();
    while(true) {
        v.push_back(new std::string(512 * 1024, 'A'));
        lll();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(){
    // malloc_stats_print(NULL, NULL, NULL);
    // return 0;
    // large();
    // large_tick_more();
    keep_allocate();
}