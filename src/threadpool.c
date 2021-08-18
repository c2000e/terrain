#include "threadpool.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct ThreadWork ThreadWork;

struct ThreadWork {
    ThreadFunc func;
    void *arg;
    ThreadWork *next;
};

static ThreadWork *ThreadWork_make(ThreadFunc func, void *arg)
{
    ThreadWork *work;

    work = malloc(sizeof *work);
    work->func = func;
    work->arg = arg;
    work->next = NULL;

    return work;
}

static void ThreadWork_free(ThreadWork *work)
{
    if (work)
    {
        free(work);
    }
}

struct ThreadPool {
    ThreadWork *work_first;
    ThreadWork *work_last;
    pthread_mutex_t work_mutex;
    pthread_cond_t work_cond;
    pthread_cond_t active_cond;
    unsigned int thread_count;
    unsigned int active_count;
    bool stop;
};

void ThreadPool_addWork(ThreadPool *pool, ThreadFunc func, void *arg)
{
    ThreadWork *work;
    work = ThreadWork_make(func, arg);

    pthread_mutex_lock(&pool->work_mutex);
    if (pool->work_first == NULL)
    {
        pool->work_first = work;
        pool->work_last = pool->work_first;
    }
    else
    {
        pool->work_last->next = work;
        pool->work_last = work;
    }

    pthread_cond_broadcast(&pool->work_cond);
    pthread_mutex_unlock(&pool->work_mutex);
}

static ThreadWork *ThreadPool_getWork(ThreadPool *pool)
{
    ThreadWork *work;

    work = pool->work_first;
    if (work == NULL)
    {
        return NULL;
    }

    if (work->next == NULL)
    {
        pool->work_first = NULL;
        pool->work_last = NULL;
    }
    else
    {
        pool->work_first = work->next;
    }

    return work;
}

static void *ThreadPool_worker(void *arg)
{
    ThreadPool *pool = arg;
    ThreadWork *work;

    while (true)
    {
        pthread_mutex_lock(&pool->work_mutex);

        while (pool->work_first == NULL && !pool->stop)
        {
            pthread_cond_wait(&pool->work_cond, &pool->work_mutex);
        }

        if (pool->stop)
        {
            break;
        }

        work = ThreadPool_getWork(pool);
        pool->active_count += 1;

        pthread_mutex_unlock(&pool->work_mutex);

        if (work != NULL)
        {
            work->func(work->arg);
            ThreadWork_free(work);
        }

        pthread_mutex_lock(&pool->work_mutex);

        pool->active_count -= 1;
        if (!pool->stop && pool->active_count == 0 && pool->work_first == NULL)
        {
            pthread_cond_signal(&pool->active_cond);
        }

        pthread_mutex_unlock(&pool->work_mutex);
    }

    pool->thread_count -= 1;
    pthread_cond_signal(&pool->active_cond);
    pthread_mutex_unlock(&pool->work_mutex);

    return NULL;
}

ThreadPool *ThreadPool_make(unsigned int thread_count)
{
    ThreadPool *pool;
    pool = malloc(sizeof *pool);
    pool->thread_count = thread_count;

    pthread_mutex_init(&pool->work_mutex, NULL);
    pthread_cond_init(&pool->work_cond, NULL);
    pthread_cond_init(&pool->active_cond, NULL);

    pool->work_first = NULL;
    pool->work_last = NULL;
    
    pthread_t thread;
    for (unsigned int i = 0; i < thread_count; i++)
    {
        pthread_create(&thread, NULL, ThreadPool_worker, pool);
        pthread_detach(thread);
    }

    return pool;
}

void ThreadPool_wait(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->work_mutex);
    while (true)
    {
        if ((!pool->stop && pool->active_count != 0)
                || (pool->stop && pool->thread_count != 0))
        {
            pthread_cond_wait(&pool->active_cond, &pool->work_mutex);
        }
        else
        {
            break;
        }
    }
    pthread_mutex_unlock(&pool->work_mutex);
}

void ThreadPool_free(ThreadPool *pool)
{
    ThreadWork *work1;
    ThreadWork *work2;

    if (pool == NULL)
    {
        return;
    }

    pthread_mutex_lock(&pool->work_mutex);
    work1 = pool->work_first;
    while (work1 != NULL)
    {
        work2 = work1->next;
        ThreadWork_free(work1);
        work1 = work2;
    }

    pool->stop = true;
    pthread_cond_broadcast(&pool->work_cond);
    pthread_mutex_unlock(&pool->work_mutex);

    ThreadPool_wait(pool);

    pthread_mutex_destroy(&pool->work_mutex);
    pthread_cond_destroy(&pool->work_cond);
    pthread_cond_destroy(&pool->active_cond);
}
