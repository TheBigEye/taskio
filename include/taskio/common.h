#ifndef TASKIO_COMMON_GUARD_HEADER
#define TASKIO_COMMON_GUARD_HEADER

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "async.h"
#include "runtime/runtime.h"
#include "task/task.h"

struct taskio_join_env {
    struct taskio_join_handle* handles;
    size_t current;
    size_t len;
};

struct taskio_sleep_env {
    struct timespec deadline;
};

struct taskio_select_env {
    bool biased;
    void* out;
    struct taskio_join_handle* handles;
    size_t len;
};

#define taskio_join(...)                                                       \
    taskio_join(sizeof((void*[]){__VA_ARGS__}) / sizeof(void*), __VA_ARGS__)

#define taskio_select(biased, out, ...)                                        \
    taskio_select(biased, out, sizeof((void*[]){__VA_ARGS__}) / sizeof(void*), \
                  __VA_ARGS__)

future_fn(void, taskio_join)(size_t len, ...);
future_fn(void, taskio_sleep)(uint64_t seconds, uint64_t nanoseconds);
future_fn(size_t, taskio_select)(bool biased, void* out, size_t len, ...);

#endif // TASKIO_COMMON_GUARD_HEADER
