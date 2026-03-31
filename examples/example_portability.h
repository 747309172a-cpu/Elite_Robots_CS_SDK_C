
#ifndef ELITE_EXAMPLE_PORTABILITY_H
#define ELITE_EXAMPLE_PORTABILITY_H
// This header provides cross-platform implementations of some utility functions
//兼容linux和windows的工具函数，主要是时间相关的函数和字符串处理函数
//所有example都应该使用这些函数来保证在不同平台上的兼容性
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static int64_t example_monotonic_now_ns(void) {
    static LARGE_INTEGER frequency = {0};
    LARGE_INTEGER counter;
    int64_t whole_seconds = 0;
    int64_t remainder = 0;

    if (frequency.QuadPart == 0) {
        QueryPerformanceFrequency(&frequency);
    }

    QueryPerformanceCounter(&counter);
    whole_seconds = (int64_t)(counter.QuadPart / frequency.QuadPart);
    remainder = (int64_t)(counter.QuadPart % frequency.QuadPart);

    return whole_seconds * 1000000000LL + (remainder * 1000000000LL) / (int64_t)frequency.QuadPart;
}

static void example_sleep_us(int64_t microseconds) {
    DWORD milliseconds = 0;

    if (microseconds <= 0) {
        return;
    }

    milliseconds = (DWORD)((microseconds + 999LL) / 1000LL);
    if (milliseconds == 0) {
        milliseconds = 1;
    }
    Sleep(milliseconds);
}

static void example_sleep_seconds(uint32_t seconds) {
    if (seconds == 0U) {
        return;
    }
    Sleep(seconds * 1000U);
}

static void example_sleep_until_monotonic_ns(int64_t target_ns) {
    int64_t now_ns = 0;

    if (target_ns <= 0) {
        return;
    }

    now_ns = example_monotonic_now_ns();
    while (now_ns < target_ns) {
        int64_t remaining_ns = target_ns - now_ns;
        if (remaining_ns > 2000000LL) {
            DWORD sleep_ms = (DWORD)(remaining_ns / 1000000LL);
            if (sleep_ms > 1U) {
                Sleep(sleep_ms - 1U);
            } else {
                Sleep(1U);
            }
        } else {
            Sleep(1U);
        }
        now_ns = example_monotonic_now_ns();
    }
}

static double example_monotonic_elapsed_seconds(int64_t start_ns, int64_t end_ns) {
    return (double)(end_ns - start_ns) / 1000000000.0;
}

static char* example_strdup(const char* text) {
    char* copy = NULL;
    size_t len = 0;

    if (!text) {
        return NULL;
    }

    len = strlen(text) + 1U;
    copy = (char*)malloc(len);
    if (!copy) {
        return NULL;
    }

    memcpy(copy, text, len);
    return copy;
}

static char* example_strtok_r(char* text, const char* delimiters, char** saveptr) {
    char* token_start = NULL;
    char* cursor = NULL;

    if (!delimiters || !saveptr) {
        return NULL;
    }

    cursor = text ? text : *saveptr;
    if (!cursor) {
        return NULL;
    }

    cursor += strspn(cursor, delimiters);
    if (*cursor == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    token_start = cursor;
    cursor += strcspn(cursor, delimiters);
    if (*cursor == '\0') {
        *saveptr = NULL;
    } else {
        *cursor = '\0';
        *saveptr = cursor + 1;
    }

    return token_start;
}

#else
#include <time.h>
#include <unistd.h>

static int64_t example_monotonic_now_ns(void) {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec;
}

static void example_sleep_us(int64_t microseconds) {
    if (microseconds <= 0) {
        return;
    }
    usleep((useconds_t)microseconds);
}

static void example_sleep_seconds(uint32_t seconds) {
    sleep(seconds);
}

static void example_sleep_until_monotonic_ns(int64_t target_ns) {
    struct timespec ts;

    if (target_ns <= 0) {
        return;
    }

    ts.tv_sec = (time_t)(target_ns / 1000000000LL);
    ts.tv_nsec = (long)(target_ns % 1000000000LL);

#if defined(CLOCK_MONOTONIC)
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
#else
    nanosleep(&ts, NULL);
#endif
}

static double example_monotonic_elapsed_seconds(int64_t start_ns, int64_t end_ns) {
    return (double)(end_ns - start_ns) / 1000000000.0;
}

static char* example_strdup(const char* text) {
    char* copy = NULL;
    size_t len = 0;

    if (!text) {
        return NULL;
    }

    len = strlen(text) + 1U;
    copy = (char*)malloc(len);
    if (!copy) {
        return NULL;
    }

    memcpy(copy, text, len);
    return copy;
}

static char* example_strtok_r(char* text, const char* delimiters, char** saveptr) {
    char* token_start = NULL;
    char* cursor = NULL;

    if (!delimiters || !saveptr) {
        return NULL;
    }

    cursor = text ? text : *saveptr;
    if (!cursor) {
        return NULL;
    }

    cursor += strspn(cursor, delimiters);
    if (*cursor == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    token_start = cursor;
    cursor += strcspn(cursor, delimiters);
    if (*cursor == '\0') {
        *saveptr = NULL;
    } else {
        *cursor = '\0';
        *saveptr = cursor + 1;
    }

    return token_start;
}
#endif

#endif
