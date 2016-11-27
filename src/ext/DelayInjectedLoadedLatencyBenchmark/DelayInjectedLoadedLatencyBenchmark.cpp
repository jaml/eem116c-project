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
 * @brief Implementation file for the DelayInjectedLatencyBenchmark class.
 */


//Headers
#include <common.h>

#ifdef EXT_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK

#include <DelayInjectedLoadedLatencyBenchmark.h>
#include <delay_injected_benchmark_kernels.h>
#include <MemoryWorker.h>
#include <LatencyWorker.h>
#include <LoadWorker.h>

//Libraries
#include <iostream>
#include <random>
#include <assert.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <processthreadsapi.h>
#endif

using namespace xmem;
        
DelayInjectedLoadedLatencyBenchmark::DelayInjectedLoadedLatencyBenchmark(
        void* mem_array,
        size_t len,
        uint32_t iterations,
        uint32_t num_worker_threads,
        uint32_t mem_node,
        uint32_t cpu_node,
        chunk_size_t chunk_size,
        std::vector<PowerReader*> dram_power_readers,
        std::string name,
        uint32_t delay
    ) :
        LatencyBenchmark(
            mem_array,
            len,
            iterations,
            num_worker_threads,
            mem_node,
            cpu_node,
            SEQUENTIAL,
            READ,
            chunk_size,
            1,
            dram_power_readers,
            name
        ),
        delay_(delay)
    { 
}

void DelayInjectedLoadedLatencyBenchmark::reportBenchmarkInfo() const {
    LatencyBenchmark::reportBenchmarkInfo();
    std::cout << "Load worker kernel delay value: " << delay_ << std::endl;
}

uint32_t DelayInjectedLoadedLatencyBenchmark::getDelay() const {
    return delay_;
}

bool DelayInjectedLoadedLatencyBenchmark::runCore() {
    size_t len_per_thread = len_ / num_worker_threads_; //Carve up memory space so each worker has its own area to play in

    //Set up latency measurement kernel function pointers
    RandomFunction lat_kernel_fptr = &chasePointers;
    RandomFunction lat_kernel_dummy_fptr = &dummy_chasePointers;

    //Initialize memory regions for all threads by writing to them, causing the memory to be physically resident.
    forwSequentialWrite_Word32(mem_array_,
                               reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(mem_array_)+len_)); //static casts to silence compiler warnings

    //Build pointer indices for random-access latency thread. We assume that latency thread is the first one, so we use beginning of memory region.
    if (!build_random_pointer_permutation(mem_array_,
                                       reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(mem_array_)+len_per_thread), //static casts to silence compiler warnings
#ifndef HAS_WORD_64 //special case: 32-bit architectures
                                       CHUNK_32b)) { 
#endif
#ifdef HAS_WORD_64
                                       CHUNK_64b)) { 
#endif
        std::cerr << "ERROR: Failed to build a random pointer permutation for the latency measurement thread!" << std::endl;
        return false;
    }

    //Set up load generation kernel function pointers
    SequentialFunction load_kernel_fptr = NULL;
    SequentialFunction load_kernel_dummy_fptr = NULL; 
    if (num_worker_threads_ > 1) { //If we only have one worker thread, it is used for latency measurement only, and no load threads will be used.
        switch (chunk_size_) {
            case CHUNK_32b:
                switch (delay_) {
                    case 0:
                        load_kernel_fptr = &forwSequentialRead_Word32; //not an extended kernel
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32; //not an extended kernel
                        break;
                    case 1:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay1;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay1;
                        break;
                    case 2:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay2;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay2;
                        break;
                    case 4:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay4;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay4;
                        break;
                    case 8:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay8;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay8;
                        break;
                    case 16:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay16;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay16;
                        break;
                    case 32:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay32;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay32;
                        break;
                    case 64:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay64;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay64;
                        break;
                    case 128:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay128;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay128;
                        break;
                    case 256:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay256;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay256;
                        break;
                    case 512:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay512;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay512plus;
                        break;
                    case 1024:
                        load_kernel_fptr = &forwSequentialRead_Word32_Delay1024;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word32_Delay512plus;
                        break;
                    default:
                        std::cerr << "ERROR: Failed to find appropriate benchmark kernel." << std::endl;
                        return false;
                }
                break;
#ifdef HAS_WORD_64
            case CHUNK_64b:
                switch (delay_) {
                    case 0:
                        load_kernel_fptr = &forwSequentialRead_Word64; //not an extended kernel
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64; //not an extended kernel
                        break;
                    case 1:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay1;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay1;
                        break;
                    case 2:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay2;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay2;
                        break;
                    case 4:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay4;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay4;
                        break;
                    case 8:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay8;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay8;
                        break;
                    case 16:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay16;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay16;
                        break;
                    case 32:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay32;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay32;
                        break;
                    case 64:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay64;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay64;
                        break;
                    case 128:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay128;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay128;
                        break;
                    case 256:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay256;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay256plus;
                        break;
                    case 512:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay512;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay256plus;
                        break;
                    case 1024:
                        load_kernel_fptr = &forwSequentialRead_Word64_Delay1024;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word64_Delay256plus;
                        break;
                    default:
                        std::cerr << "ERROR: Failed to find appropriate benchmark kernel." << std::endl;
                        return false;
                }
                break;
#endif
#ifdef HAS_WORD_128
            case CHUNK_128b:
                switch (delay_) {
                    case 0:
                        load_kernel_fptr = &forwSequentialRead_Word128; //not an extended kernel
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128; //not an extended kernel
                        break;
                    case 1:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay1;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay1;
                        break;
                    case 2:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay2;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay2;
                        break;
                    case 4:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay4;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay4;
                        break;
                    case 8:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay8;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay8;
                        break;
                    case 16:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay16;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay16;
                        break;
                    case 32:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay32;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay32;
                        break;
                    case 64:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay64;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay64;
                        break;
                    case 128:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay128;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay128plus;
                        break;
                    case 256:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay256;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay128plus;
                        break;
                    case 512:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay512;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay128plus;
                        break;
                    case 1024:
                        load_kernel_fptr = &forwSequentialRead_Word128_Delay1024;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word128_Delay128plus;
                        break;
                    default:
                        std::cerr << "ERROR: Failed to find appropriate benchmark kernel." << std::endl;
                        return false;
                }
                break;
#endif
#ifdef HAS_WORD_256
            case CHUNK_256b:
                switch (delay_) {
                    case 0:
                        load_kernel_fptr = &forwSequentialRead_Word256; //not an extended kernel
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256; //not an extended kernel
                        break;
                    case 1:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay1;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay1;
                        break;
                    case 2:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay2;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay2;
                        break;
                    case 4:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay4;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay4;
                        break;
                    case 8:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay8;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay8;
                        break;
                    case 16:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay16;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay16;
                        break;
                    case 32:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay32;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay32;
                        break;
                    case 64:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay64;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay64plus;
                        break;
                    case 128:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay128;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay64plus;
                        break;
                    case 256:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay256;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay64plus;
                        break;
                    case 512:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay512;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay64plus;
                        break;
                    case 1024:
                        load_kernel_fptr = &forwSequentialRead_Word256_Delay1024;
                        load_kernel_dummy_fptr = &dummy_forwSequentialLoop_Word256_Delay64plus;
                        break;
                    default:
                        std::cerr << "ERROR: Failed to find appropriate benchmark kernel." << std::endl;
                        return false;
                }
                break;
#endif
            default:
                std::cerr << "ERROR: Chunk size must be 64-bit or 256-bit to run the delay-injected latency benchmark extension!" << std::endl;
                return false;
        }
    }

    //Set up some stuff for worker threads
    std::vector<MemoryWorker*> workers;
    std::vector<Thread*> worker_threads;
    
    //Start power measurement
    if (g_verbose)
        std::cout << "Starting power measurement threads...";
    
    if (!startPowerThreads()) {
        if (g_verbose)
            std::cout << "FAIL" << std::endl;
        std::cerr << "WARNING: Failed to start power threads." << std::endl;
    } else if (g_verbose)
        std::cout << "done" << std::endl;
    
    //Run benchmark
    if (g_verbose)
        std::cout << "Running benchmark." << std::endl << std::endl;

    //Do a bunch of iterations of the core benchmark routine
    for (uint32_t i = 0; i < iterations_; i++) {

        //Create load workers and load worker threads
        for (uint32_t t = 0; t < num_worker_threads_; t++) {
            void* threadmem_array_ = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(mem_array_) + t*len_per_thread);
            int32_t cpu_id = cpu_id_in_numa_node(cpu_node_, t);
            if (cpu_id < 0)
                std::cerr << "WARNING: Failed to find logical CPU " << t << " in NUMA node " << cpu_node_ << std::endl;
            if (t == 0) { //special case: thread 0 is always latency thread
                workers.push_back(new LatencyWorker(threadmem_array_,
                                                    len_per_thread,
                                                    lat_kernel_fptr,
                                                    lat_kernel_dummy_fptr,
                                                    cpu_id));
            } else {
                workers.push_back(new LoadWorker(threadmem_array_,
                                                 len_per_thread,
                                                 load_kernel_fptr,
                                                 load_kernel_dummy_fptr,
                                                 cpu_id));
            }
            worker_threads.push_back(new Thread(workers[t]));
        }

        //Start worker threads! gogogo
        for (uint32_t t = 0; t < num_worker_threads_; t++)
            worker_threads[t]->create_and_start();

        //Wait for all threads to complete
        for (uint32_t t = 0; t < num_worker_threads_; t++)
            if (!worker_threads[t]->join())
                std::cerr << "WARNING: A worker thread failed to complete correctly!" << std::endl;
        
        //Compute metrics for this iteration
        bool iterwarning_ = false;

        //Compute latency metric
        uint32_t lat_passes = workers[0]->getPasses();  
        tick_t lat_adjusted_ticks = workers[0]->getAdjustedTicks();
        tick_t lat_elapsed_dummy_ticks = workers[0]->getElapsedDummyTicks();
        uint32_t lat_bytes_per_pass = workers[0]->getBytesPerPass();
        uint32_t lat_accesses_per_pass = lat_bytes_per_pass / 8;
        iterwarning_ |= workers[0]->hadWarning();
        
        //Compute throughput generated by load threads
        uint32_t load_total_passes = 0;
        tick_t load_total_adjusted_ticks = 0;
        tick_t load_total_elapsed_dummy_ticks = 0;
        uint32_t load_bytes_per_pass = 0;
        double load_avg_adjusted_ticks = 0;
        for (uint32_t t = 1; t < num_worker_threads_; t++) {
            load_total_passes += workers[t]->getPasses();
            load_total_adjusted_ticks += workers[t]->getAdjustedTicks();
            load_total_elapsed_dummy_ticks += workers[t]->getElapsedDummyTicks();
            load_bytes_per_pass = workers[t]->getBytesPerPass(); //all should be the same.
            iterwarning_ |= workers[t]->hadWarning();
        }

        //Compute load metrics for this iteration
        load_avg_adjusted_ticks = static_cast<double>(load_total_adjusted_ticks) / (num_worker_threads_-1);
        if (num_worker_threads_ > 1)
            load_metric_on_iter_[i] = (((static_cast<double>(load_total_passes) * static_cast<double>(load_bytes_per_pass)) / static_cast<double>(MB)))   /  ((load_avg_adjusted_ticks * g_ns_per_tick) / 1e9);

        if (iterwarning_)
            warning_ = true;
    
        if (g_verbose) { //Report metrics for this iteration
            //Latency thread
            std::cout << "Iter " << i+1 << " had " << lat_passes << " latency measurement passes, with " << lat_accesses_per_pass << " accesses per pass:";
            if (iterwarning_) std::cout << " -- WARNING";
            std::cout << std::endl;

            std::cout << "...lat clock ticks == " << lat_adjusted_ticks << " (adjusted by -" << lat_elapsed_dummy_ticks << ")";
            if (iterwarning_) std::cout << " -- WARNING";
            std::cout << std::endl;

            std::cout << "...lat ns == " << lat_adjusted_ticks * g_ns_per_tick << " (adjusted by -" << lat_elapsed_dummy_ticks * g_ns_per_tick << ")";
            if (iterwarning_) std::cout << " -- WARNING";
            std::cout << std::endl;

            std::cout << "...lat sec == " << lat_adjusted_ticks * g_ns_per_tick / 1e9 << " (adjusted by -" << lat_elapsed_dummy_ticks * g_ns_per_tick / 1e9 << ")";
            if (iterwarning_) std::cout << " -- WARNING";
            std::cout << std::endl;

            //Load threads
            if (num_worker_threads_ > 1) {
                std::cout << "Iter " << i+1 << " had " << load_total_passes << " total load generation passes, with " << load_bytes_per_pass << " bytes per pass:";
                if (iterwarning_) std::cout << " -- WARNING";
                std::cout << std::endl;

                std::cout << "...load total clock ticks across " << num_worker_threads_-1 << " threads == " << load_total_adjusted_ticks << " (adjusted by -" << load_total_elapsed_dummy_ticks << ")";
                if (iterwarning_) std::cout << " -- WARNING";
                std::cout << std::endl;

                std::cout << "...load total ns across " << num_worker_threads_-1 << " threads == " << load_total_adjusted_ticks * g_ns_per_tick << " (adjusted by -" << load_total_elapsed_dummy_ticks * g_ns_per_tick << ")";
                if (iterwarning_) std::cout << " -- WARNING";
                std::cout << std::endl;

                std::cout << "...load total sec across " << num_worker_threads_-1 << " threads == " << load_total_adjusted_ticks * g_ns_per_tick / 1e9 << " (adjusted by -" << load_total_elapsed_dummy_ticks * g_ns_per_tick / 1e9 << ")";
                if (iterwarning_) std::cout << " -- WARNING";
                std::cout << std::endl;
            }
        }
        
        //Compute overall metrics for this iteration
        metric_on_iter_[i] = static_cast<double>(lat_adjusted_ticks * g_ns_per_tick)  /  static_cast<double>(lat_accesses_per_pass * lat_passes);
        
        //Clean up workers and threads for this iteration
        for (uint32_t t = 0; t < num_worker_threads_; t++) {
            delete worker_threads[t];
            delete workers[t];
        }
        worker_threads.clear();
        workers.clear();
    }

    //Stop power measurement
    if (g_verbose) {
        std::cout << std::endl;
        std::cout << "Stopping power measurement threads...";
    }
    
    if (!stopPowerThreads()) {
        if (g_verbose)
            std::cout << "FAIL" << std::endl;
        std::cerr << "WARNING: Failed to stop power measurement threads." << std::endl;
    } else if (g_verbose)
        std::cout << "done" << std::endl;
    
    //Run metadata
    has_run_ = true;
    
    //Get mean load metrics -- these aren't part of Benchmark class thus not covered by computeMetrics()
    computeMetrics();
    for (uint32_t i = 0; i < iterations_; i++)
        mean_load_metric_ += load_metric_on_iter_[i];
    mean_load_metric_ /= static_cast<double>(iterations_);

    return true;
}

#endif
