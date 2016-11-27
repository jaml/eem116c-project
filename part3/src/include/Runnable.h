/* The MIT License (MIT)
 *
 * Copyright (c) 2014 Microsoft
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Author: Mark Gottscho <mgottscho@ucla.edu>
 */

/**
 * @file
 * 
 * @brief Header file for the Runnable class.
 */

#ifndef RUNNABLE_H
#define RUNNABLE_H

//Libraries
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __gnu_linux__
#include <pthread.h>
#endif

namespace xmem {

    /**
     * @brief A base class for any object that implements a thread-safe run() function for use by Thread objects.
     */
    class Runnable {
        public:
            /**
             * @brief Constructor.
             */
            Runnable();

            /**
             * @brief Destructor.
             */
            ~Runnable();

            /**
             * @brief Does some "work". Pure virtual method that any derived class must implement in a thread-safe manner.
             */
            virtual void run() = 0;

        protected:
            /** 
             * @brief Acquires the object lock to access all object state in thread-safe manner.
             * @param timeout timeout in milliseconds to acquire the lock. If 0, does not wait at all. If negative, waits indefinitely.
             * @returns true on success. If not successful, the lock was not acquired, possibly due to a timeout, or the lock might already be held.
             */
            bool acquireLock(int32_t timeout);

            /** 
             * @brief Releases the object lock to access all object state in thread-safe manner.
             * @returns true on success. If not successful, the lock is either still held or the call was illegal (e.g., releasing a lock that was never acquired).
             */
            bool releaseLock();
            
            /** A handle to the OS native Windows mutex, i.e., the locking mechanism. Outside the constructor, this should only be accessed via _acquireLock() and _releaseLock(). */
#ifdef _WIN32
            HANDLE mutex_; 
#endif

#ifdef __gnu_linux__
            /** A handle to the OS pthreads mutex, i.e., the locking mechanism. Outside the constructor, this should only be accessed via _acquireLock() and _releaseLock(). */
            pthread_mutex_t mutex_; 
#endif
    };
};

#endif
