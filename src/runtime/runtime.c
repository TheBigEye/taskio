#include <stdlib.h>

#include "runtime_ext.h"

struct runtime_node {
    struct taskio_runtime* runtime;
    struct runtime_node* next;
};

static struct runtime_node* runtime_stack;

void taskio_context_push_runtime(struct taskio_runtime* runtime) {
    struct runtime_node* node = malloc(sizeof(struct runtime_node));
    node->runtime = runtime;
    node->next = runtime_stack;

    runtime_stack = node;
}

void taskio_context_pop_runtime() {
    struct runtime_node* node = runtime_stack;
    runtime_stack = node->next;

    free(node);
}

struct taskio_join_handle taskio_runtime_spawn(struct taskio_task* task) {
    struct taskio_runtime* runtime = runtime_stack->runtime;
    return runtime->spawn(runtime->inner, task);
}

struct taskio_join_handle
taskio_runtime_spawn_blocking(struct taskio_task* task) {
    struct taskio_runtime* runtime = runtime_stack->runtime;
    return runtime->spawn_blocking(runtime->inner, task);
}
