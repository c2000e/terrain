#ifndef THREADPOOL_H
#define THREADPOOL_H

// https://nachtimwald.com/2019/04/12/thread-pool-in-c/

typedef void (*ThreadFunc)(void *arg);

typedef struct ThreadPool ThreadPool;

ThreadPool *ThreadPool_make(unsigned int thread_count);
void ThreadPool_free(ThreadPool *pool);

void ThreadPool_addWork(ThreadPool *pool, ThreadFunc func, void *arg);
void ThreadPool_wait(ThreadPool *pool);

#endif
