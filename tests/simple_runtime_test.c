#define TASKIO_RUNTIME SIMPLE

#include <stdio.h>
#include <unistd.h>

#include <taskio.h>
#include <taskio/common.h>

async_fn(int, foo, {
    int a = 123;

    await_fn(taskio_sleep, 1, 0);
    printf("foo: slept 1 second. %d\n", a);

    a++;

    await_fn(taskio_sleep, 3, 0);
    printf("foo: slept 3 seconds. %d\n", a);

    async_return(a);
});

async_fn(void, bar, {
    await_fn(taskio_sleep, 3, 0);
    printf("bar: slept 3 seconds.\n");
});

taskio_main({
    taskio_task_spawn_fn(foo);
    taskio_task_spawn_fn(bar);
});
