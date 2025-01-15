#include "stdint.h"
#include "stdio.h"

#include "pthread.h"

#include <cassert>

/*
This is an exercise from Intro OS Udacity course, using plain C.
*/

int32_t globalSharedVariable = 0;

pthread_mutex_t mutex;
pthread_cond_t condVar;

const int32_t numReaderThreads = 5;
const int32_t numWriterThreads = 5;

int32_t readerCount = 0;

int32_t globalNumOfWrites = 0;

struct ReaderThread
{
    int32_t threadID_;
    int32_t readCounter_;
    pthread_t thread_;

    void operator()()
    {
        while (globalNumOfWrites < 6)
        {
            // Grab mutex, Wait for condition, if false, release lock and sleep, wait for notify

            pthread_mutex_lock(&mutex);

            // Wait for condition (the shared condition must be met), needs to be in loop to avoid spurrious wakeups
            while (this->readCounter_ > globalNumOfWrites)
            {
                pthread_cond_wait(&condVar, &mutex);
            }

            // Add some random time

            assert(this->readCounter_ >= globalNumOfWrites);

            this->readCounter_ += 1;

            printf("ReaderThread with threadID: %d read the following value: %d", this->threadID_, globalSharedVariable);

            // Notify other threads that the condition has changed
            pthread_cond_signal(&condVar);

            // Release mutex
            pthread_mutex_unlock(&mutex);
        }
    }
};

struct WriterThread
{
    int32_t threadID_;
    int32_t writeCounter_;
    pthread_t thread_;

    void operator()()
    {
        while (globalNumOfWrites < 6)
        {
            // Grab mutex, Wait for condition, if false, release lock and sleep, wait for notify

            pthread_mutex_lock(&mutex);

            // Wait for condition (the shared condition must be met), needs to be in loop to avoid spurrious wakeups
            while (readerCount < numReaderThreads)
            {
                pthread_cond_wait(&condVar, &mutex);
            }

            // Add some random time

            assert(this->writeCounter_ == globalNumOfWrites);

            this->writeCounter_ += 1;

            const int32_t newValue = globalSharedVariable + 1;
            printf("WriterThread with threadID: %d UPDATED the following value FROM: %d TO: %d", this->threadID_, globalSharedVariable, newValue);

            globalSharedVariable = newValue;

            ++globalNumOfWrites;

            // Notify other threads that the condition has changed
            pthread_cond_signal(&condVar);

            // Release mutex
            pthread_mutex_unlock(&mutex);
        }
    }
};

int main()
{
    // Initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condVar, NULL);

    ReaderThread readerThreads[numReaderThreads];
    WriterThread writerThreads[numWriterThreads];

    // Create reader threads
    for (int32_t i = 0; i < numReaderThreads; ++i) {
        readerThreads[i].threadID_ = i;
        readerThreads[i].readCounter_ = 0;

        if (pthread_create(&readerThreads[i].thread_, NULL, [](void* arg) -> void* {
                ((ReaderThread*)arg)->operator()();
                return nullptr;
            },
            &readerThreads[i])) {
            fprintf(stderr, "Error creating reader thread %d\n", i);
            return 1;
        }
    }

    // Create writer threads
    for (int32_t i = 0; i < numWriterThreads; ++i) {
        writerThreads[i].threadID_ = i;
        writerThreads[i].writeCounter_ = 0;

        if (pthread_create(&writerThreads[i].thread_, NULL, [](void* arg) -> void* {
                ((WriterThread*)arg)->operator()();
                return nullptr;
            },
            &writerThreads[i])) {
            fprintf(stderr, "Error creating writer thread %d\n", i);
            return 1;
        }
    }

    // Wait for all reader threads to complete
    for (int32_t i = 0; i < numReaderThreads; ++i) {
        pthread_join(readerThreads[i].thread_, NULL);
    }

    // Wait for all writer threads to complete
    for (int32_t i = 0; i < numWriterThreads; ++i) {
        pthread_join(writerThreads[i].thread_, NULL);
    }

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condVar);

    return 0;
}