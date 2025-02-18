#include <stdio.h>

#include <taskio.h>
#include <taskio/common.h>
#include <taskio/sync/semaphore.h>

struct wait_task_env {
    struct taskio_semaphore* sem;
    uint64_t delay;

    bool result;

    future_env(taskio_semaphore_timedwait, taskio_sleep);
};

static_future_fn(void, wait_task)(struct taskio_semaphore* sem, uint64_t delay) {
    return_future_fn(void, wait_task, sem, delay);
}

async_fn(void, wait_task) {
    struct taskio_semaphore* sem = async_env(sem);
    uint64_t delay = async_env(delay);

    async_scope() {
        printf("task: wait\n");
        await_fn_get(&async_env(result), taskio_semaphore_timedwait(sem, 1000));
    }

    async_scope() {
        if (!async_env(result)) {
            printf("task: timeout\n");
            async_return();
        }

        printf("task: sleeping for %lu seconds.\n", delay / 1000);
        await_fn(taskio_sleep(delay));
    }

    async_scope() {
        printf("task: signal\n");
        taskio_semaphore_signal(sem);
        async_return();
    }
}

struct taskio_main_env {
    int argc;
    char** args;

    future_env(taskio_join);

    struct taskio_semaphore sem;
    struct wait_task_future wait_one;
    struct wait_task_future wait_two;
};

taskio_main() {
    struct taskio_semaphore* sem = &async_env(sem);

    async_scope() {
        taskio_semaphore_init(sem, 1);

        async_env(wait_one) = wait_task(sem, 4000);
        async_env(wait_two) = wait_task(sem, 2000);
        await_fn(taskio_join(NULL, &async_env(wait_one), &async_env(wait_two)));
    }

    async_scope() {
        taskio_semaphore_drop(sem);
        async_return();
    }
}
