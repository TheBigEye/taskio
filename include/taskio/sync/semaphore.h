#ifndef TASKIO_SYNC_SEMAPHORE_GUARD_HEADER
#define TASKIO_SYNC_SEMAPHORE_GUARD_HEADER

#include <stdatomic.h>
#include <stdint.h>
#include <threads.h>

#include <taskio/alloc.h>
#include <taskio/async.h>
#include <taskio/common.h>

struct taskio_semaphore_node {
    atomic_size_t counter;
    struct taskio_waker waker;

    struct taskio_semaphore_node* back;
    struct taskio_semaphore_node* next;
};

struct taskio_semaphore {
    atomic_size_t counter;

    struct taskio_allocator allocator;

    mtx_t wake_guard;
    struct taskio_semaphore_node* wake_queue_head;
    struct taskio_semaphore_node* wake_queue_tail;
};

struct taskio_semaphore_wait_env {
    struct taskio_semaphore* semaphore;
    struct taskio_semaphore_node* node;
};

void taskio_semaphore_init(struct taskio_semaphore* semaphore, size_t permits);
void taskio_semaphore_init_with_alloc(struct taskio_semaphore* semaphore, size_t permits,
                                      struct taskio_allocator* allocator);
void taskio_semaphore_drop(struct taskio_semaphore* semaphore);

size_t taskio_semaphore_getvalue(struct taskio_semaphore* semaphore);

future_fn(void, taskio_semaphore_wait)(struct taskio_semaphore* semaphore);
void taskio_semaphore_signal(struct taskio_semaphore* semaphore);

#endif // TASKIO_SYNC_SEMAPHORE_GUARD_HEADER
