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
 * @brief Implementation file for the Configurator class and some helper data structures.
 */

//Headers
#include <Configurator.h>
#include <common.h>
#include <optionparser.h>
#include <MyArg.h>
#include <common.h>

//Libraries
#include <cstdint>
#include <iostream>
#include <string>

using namespace xmem;

Configurator::Configurator(
    ) :
    configured_(false),
    run_extensions_(false),
#ifdef EXT_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK
    run_ext_delay_injected_loaded_latency_benchmark_(false),
#endif
#ifdef EXT_STREAM_BENCHMARK
    run_ext_stream_benchmark_(false),
#endif
    run_latency_(true),
    run_throughput_(true),
    working_set_size_per_thread_(DEFAULT_WORKING_SET_SIZE_PER_THREAD),
    num_worker_threads_(DEFAULT_NUM_WORKER_THREADS),
#ifdef HAS_WORD_64
    use_chunk_32b_(false),
    use_chunk_64b_(true),
#else
    use_chunk_32b_(true),
#endif
#ifdef HAS_WORD_128
    use_chunk_128b_(false),
#endif
#ifdef HAS_WORD_256
    use_chunk_256b_(false),
#endif
#ifdef HAS_WORD_512
    use_chunk_512b(false),
#endif
#ifdef HAS_NUMA
    numa_enabled_(true),
#else
    numa_enabled_(false),
#endif
    cpu_numa_node_affinities_(),
    memory_numa_node_affinities_(),
    iterations_(1),
    use_random_access_pattern_(false),
    use_sequential_access_pattern_(true),
    starting_test_index_(1),
    filename_(),
    use_output_file_(false),
    verbose_(false),
    use_large_pages_(false),
    use_reads_(true),
    use_writes_(true),
    use_stride_p1_(true),
    use_stride_n1_(false),
    use_stride_p2_(false),
    use_stride_n2_(false),
    use_stride_p4_(false),
    use_stride_n4_(false),
    use_stride_p8_(false),
    use_stride_n8_(false),
    use_stride_p16_(false),
    use_stride_n16_(false),
    use_mlp_1_(true),
    use_mlp_2_(false),
    use_mlp_4_(false),
    use_mlp_6_(false),
    use_mlp_8_(false),
    use_mlp_16_(false),
    use_mlp_32_(false),
    mlp_(1)
    {
}

int32_t Configurator::configureFromInput(int argc, char* argv[]) {
    if (configured_) { //If this object was already configured, cannot override from user inputs. This is to prevent an invalid state.
        std::cerr << "WARNING: Something bad happened when configuring X-Mem. This is probably not your fault." << std::endl;
        return -2;
    }

    //Throw out first argument which is usually the program name.
    argc -= (argc > 0);
    argv += (argc > 0);

    //Set up optionparser
    Stats stats(usage, argc, argv);
    Option* options = new Option[stats.options_max];
    Option* buffer = new Option[stats.buffer_max];
    Parser parse(usage, argc, argv, options, buffer); //Parse input

    //Check for parser error
    if (parse.error()) {
        std::cerr << "ERROR: Argument parsing failed. You could try running \"xmem --help\" for usage information." << std::endl;
        goto error;
    }

    //X-Mem doesn't have any non-option arguments, so we will presume the user wants a help message.
    if (parse.nonOptionsCount() > 0) {
        std::cerr << "ERROR: X-Mem does not support any non-option arguments." << std::endl;
        goto error;
    }

    //Check for any unknown options
    for (Option* unknown_opt = options[UNKNOWN]; unknown_opt != NULL; unknown_opt = unknown_opt->next()) {
        std::cerr << "ERROR: Unknown option: " << std::string(unknown_opt->name, unknown_opt->namelen) << std::endl;
        goto error;
    }

    //Verbosity
    if (options[VERBOSE]) {
        verbose_ = true; //What the user configuration is.
        g_verbose = true; //What rest of X-Mem actually uses.
    }

    //Check runtime modes
    if (options[MEAS_LATENCY] || options[MEAS_THROUGHPUT] || options[EXTENSION]) { //User explicitly picked at least one mode, so override default selection
        run_latency_ = false;
        run_throughput_ = false;
        run_extensions_ = false;
    }

    if (options[MEAS_LATENCY])
        run_latency_ = true;

    if (options[MEAS_THROUGHPUT])
        run_throughput_ = true;

    //Check extensions
    if (options[EXTENSION]) {
        if (NUM_EXTENSIONS <= 0) { //no compiled-in extensions, this must fail.
            std::cerr << "ERROR: No X-Mem extensions were included at build time." << std::endl;
            goto error;
        }

        run_extensions_ = true;

        //Init... override default values
#ifdef EXT_DELAY_INJECTED_LATENCY_BENCHMARK
        run_ext_delay_injected_loaded_latency_benchmark_ = false;
#endif
#ifdef EXT_STREAM_BENCHMARK
        run_ext_stream_benchmark_ = false;
#endif

        Option* curr = options[EXTENSION];
        while (curr) { //EXTENSION may occur more than once, this is perfectly OK.
            char* endptr = NULL;
            uint32_t ext_num = static_cast<uint32_t>(strtoul(curr->arg, &endptr, 10));
            switch (ext_num) {
#ifdef EXT_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK
                case EXT_NUM_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK:
                    run_ext_delay_injected_loaded_latency_benchmark_ = true;
                    break;
#endif
#ifdef EXT_STREAM_BENCHMARK
                case EXT_NUM_STREAM_BENCHMARK:
                    run_ext_stream_benchmark_ = true;
                    break;
#endif
                default:
                    //If no extensions are enabled, then we should not have reached this point anyway.
                    std::cerr << "ERROR: Invalid extension number " << ext_num << ". Allowed values: " << std::endl
#ifdef EXT_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK
                    << "---> Delay-injected latency benchmark: " << EXT_NUM_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK
#endif
#ifdef EXT_STREAM_BENCHMARK
                    << "---> STREAM-like benchmark: " << EXT_NUM_STREAM_BENCHMARK
#endif
                    << std::endl;
                    goto error;
            }
            curr = curr->next();
        }
    }

    //Check working set size
    if (options[WORKING_SET_SIZE_PER_THREAD]) { //Override default value with user-specified value
        if (!check_single_option_occurrence(&options[WORKING_SET_SIZE_PER_THREAD]))
            goto error;

        char* endptr = NULL;
        size_t working_set_size_KB = strtoul(options[WORKING_SET_SIZE_PER_THREAD].arg, &endptr, 10);
        if ((working_set_size_KB % 4) != 0) {
            std::cerr << "ERROR: Working set size must be specified in KB and be a multiple of 4 KB." << std::endl;
            goto error;
        }

        working_set_size_per_thread_ = working_set_size_KB * KB; //convert to bytes
    }

    //Check NUMA selection
#ifndef HAS_NUMA
    numa_enabled_ = false;
    cpu_numa_node_affinities_.push_back(0);
    memory_numa_node_affinities_.push_back(0);
#endif

    if (options[NUMA_DISABLE]) {
#ifndef HAS_NUMA
        std::cerr << "WARNING: NUMA is not supported on this build, so the NUMA-disable option has no effect." << std::endl;
#else
        numa_enabled_ = false;
        cpu_numa_node_affinities_.push_back(0);
        memory_numa_node_affinities_.push_back(0);
#endif
    }

    if (options[CPU_NUMA_NODE_AFFINITY]) {
        if (!numa_enabled_)
            std::cerr << "WARNING: NUMA is disabled, so you cannot specify CPU NUMA node affinity directly. Overriding to only use node 0 for CPU affinity." << std::endl;
        else {
            Option* curr = options[CPU_NUMA_NODE_AFFINITY];
            while (curr) { //CPU_NUMA_NODE_AFFINITY may occur more than once, this is perfectly OK.
                char* endptr = NULL;
                uint32_t cpu_numa_node_affinity = static_cast<uint32_t>(strtoul(curr->arg, &endptr, 10));
                if (cpu_numa_node_affinity >= g_num_numa_nodes) {
                    std::cerr << "ERROR: CPU NUMA node affinity of " << cpu_numa_node_affinity << " is not supported. There are only " << g_num_numa_nodes << " nodes in this system." << std::endl;
                    goto error;
                }

                bool found = false;
                for (auto it = cpu_numa_node_affinities_.cbegin(); it != cpu_numa_node_affinities_.cend(); it++) {
                    if (*it == cpu_numa_node_affinity)
                        found = true;
                }

                if (!found)
                    cpu_numa_node_affinities_.push_back(cpu_numa_node_affinity);

                curr = curr->next();
            }

            cpu_numa_node_affinities_.sort();
        }
    }
    else if (numa_enabled_) { //Default: use all CPU NUMA nodes
        for (uint32_t i = 0; i < g_num_numa_nodes; i++)
            cpu_numa_node_affinities_.push_back(i);
    }

    if (options[MEMORY_NUMA_NODE_AFFINITY]) {
        if (!numa_enabled_)
            std::cerr << "WARNING: NUMA is disabled, so you cannot specify memory NUMA node affinity directly. Overriding to only use node 0 for memory affinity." << std::endl;
        else {
            Option* curr = options[MEMORY_NUMA_NODE_AFFINITY];
            while (curr) { //MEMORY_NUMA_NODE_AFFINITY may occur more than once, this is perfectly OK.
                char* endptr = NULL;
                uint32_t memory_numa_node_affinity = static_cast<uint32_t>(strtoul(curr->arg, &endptr, 10));
                if (memory_numa_node_affinity >= g_num_numa_nodes) {
                    std::cerr << "ERROR: memory NUMA node affinity of " << memory_numa_node_affinity << " is not supported. There are only " << g_num_numa_nodes << " nodes in this system." << std::endl;
                    goto error;
                }

                bool found = false;
                for (auto it = memory_numa_node_affinities_.cbegin(); it != memory_numa_node_affinities_.cend(); it++) {
                    if (*it == memory_numa_node_affinity)
                        found = true;
                }

                if (!found)
                    memory_numa_node_affinities_.push_back(memory_numa_node_affinity);

                curr = curr->next();
            }

            memory_numa_node_affinities_.sort();
        }
    }
    else if (numa_enabled_) { //Default: use all memory NUMA nodes
        for (uint32_t i = 0; i < g_num_numa_nodes; i++)
            memory_numa_node_affinities_.push_back(i);
    }

    //Check if large pages should be used for allocation of memory under test.
    if (options[USE_LARGE_PAGES]) {
#if defined(__gnu_linux__) && defined(ARCH_INTEL)
        if (numa_enabled_) { //For now, large pages are not --simultaneously-- supported alongside NUMA. This is due to lack of NUMA support in hugetlbfs on GNU/Linux.
            std::cerr << "ERROR: On GNU/Linux version of X-Mem for Intel architectures, large pages are not simultaneously supported alongside NUMA due to reasons outside our control. If you want large pages, then force UMA using the \"-u\" option explicitly." << std::endl;
            goto error;
        }
#endif
#ifndef HAS_LARGE_PAGES
        std::cerr << "WARNING: Huge pages are not supported on this build. Regular-sized pages will be used." << std::endl;
#else
        use_large_pages_ = true;
#endif
    }

    //Check number of worker threads
    if (options[NUM_WORKER_THREADS]) { //Override default value
        if (!check_single_option_occurrence(&options[NUM_WORKER_THREADS]))
            goto error;

        char* endptr = NULL;
        num_worker_threads_ = static_cast<uint32_t>(strtoul(options[NUM_WORKER_THREADS].arg, &endptr, 10));
        if (num_worker_threads_ > g_num_logical_cpus) {
            std::cerr << "ERROR: Number of worker threads may not exceed the number of logical CPUs (" << g_num_logical_cpus << ")" << std::endl;
            goto error;
        }
    }

    //Check chunk sizes
    if (options[CHUNK_SIZE]) {
        //Init... override default values
        use_chunk_32b_ = false;
#ifdef HAS_WORD_64
        use_chunk_64b_ = false;
#endif
#ifdef HAS_WORD_128
        use_chunk_128b_ = false;
#endif
#ifdef HAS_WORD_256
        use_chunk_256b_ = false;
#endif
#ifdef HAS_WORD_512
        use_chunk_512b = false;
#endif

        Option* curr = options[CHUNK_SIZE];
        while (curr) { //CHUNK_SIZE may occur more than once, this is perfectly OK.
            char* endptr = NULL;
            uint32_t chunk_size = static_cast<uint32_t>(strtoul(curr->arg, &endptr, 10));
            switch (chunk_size) {
                case 32:
                    use_chunk_32b_ = true;
                    break;
#ifdef HAS_WORD_64
                case 64:
                    use_chunk_64b_ = true;
                    break;
#endif
#ifdef HAS_WORD_128
                case 128:
                    use_chunk_128b_ = true;
                    break;
#endif
#ifdef HAS_WORD_256
                case 256:
                    use_chunk_256b_ = true;
                    break;
#endif
#ifdef HAS_WORD_512
                case 512:
                    use_chunk_512b = true;
                    break;
#endif
                default:
                    std::cerr << "ERROR: Invalid chunk size " << chunk_size << ". Chunk sizes can be 32 "
#ifdef HAS_WORD_64
                    << "64 "
#endif
#ifdef HAS_WORD_128_
                    << "128 "
#endif
#ifdef HAS_WORD_256
                    << "256 "
#endif
#ifdef HAS_WORD_512
                    << "512 "
#endif
                    << "bits on this system." << std::endl;
                    goto error;
            }
            curr = curr->next();
        }
    }

    //Check iterations
    if (options[ITERATIONS]) { //Override default value
        if (!check_single_option_occurrence(&options[ITERATIONS]))
            goto error;

        char *endptr = NULL;
        iterations_ = static_cast<uint32_t>(strtoul(options[ITERATIONS].arg, &endptr, 10));
    }

    //Check throughput/loaded latency benchmark access patterns
    if (options[RANDOM_ACCESS_PATTERN] || options[SEQUENTIAL_ACCESS_PATTERN]) { //override defaults
        use_random_access_pattern_ = false;
        use_sequential_access_pattern_ = false;
    }

    if (options[RANDOM_ACCESS_PATTERN])
        use_random_access_pattern_ = true;

    if (options[SEQUENTIAL_ACCESS_PATTERN])
        use_sequential_access_pattern_ = true;

    //Check starting test index
    if (options[BASE_TEST_INDEX]) { //override defaults
        if (!check_single_option_occurrence(&options[BASE_TEST_INDEX]))
            goto error;

        char *endptr = NULL;
        starting_test_index_ = static_cast<uint32_t>(strtoul(options[BASE_TEST_INDEX].arg, &endptr, 10)); //What the user specified
    }
    g_starting_test_index = starting_test_index_; //What rest of X-Mem uses
    g_test_index = g_starting_test_index; //What rest of X-Mem uses. The current test index.

    //Check filename
    if (options[OUTPUT_FILE]) { //override defaults
        if (!check_single_option_occurrence(&options[OUTPUT_FILE]))
            goto error;

        filename_ = options[OUTPUT_FILE].arg;
        use_output_file_ = true;
    }

    //Check if reads and/or writes should be used in throughput and loaded latency benchmarks
    if (options[USE_READS] || options[USE_WRITES]) { //override defaults
        use_reads_ = false;
        use_writes_ = false;
    }

    if (options[USE_READS])
        use_reads_ = true;

    if (options[USE_WRITES])
        use_writes_ = true;

    //Check stride sizes
    if (options[STRIDE_SIZE]) { //override defaults
        use_stride_p1_ = false;
        use_stride_n1_ = false;
        use_stride_p2_ = false;
        use_stride_n2_ = false;
        use_stride_p4_ = false;
        use_stride_n4_ = false;
        use_stride_p8_ = false;
        use_stride_n8_ = false;
        use_stride_p16_ = false;
        use_stride_n16_ = false;

        Option* curr = options[STRIDE_SIZE];
        while (curr) { //STRIDE_SIZE may occur more than once, this is perfectly OK.
            char* endptr = NULL;
            int32_t stride_size = static_cast<int32_t>(strtoul(curr->arg, &endptr, 10));
            switch (stride_size) {
                case 1:
                    use_stride_p1_ = true;
                    break;
                case -1:
                    use_stride_n1_ = true;
                    break;
                case 2:
                    use_stride_p2_ = true;
                    break;
                case -2:
                    use_stride_n2_ = true;
                    break;
                case 4:
                    use_stride_p4_ = true;
                    break;
                case -4:
                    use_stride_n4_ = true;
                    break;
                case 8:
                    use_stride_p8_ = true;
                    break;
                case -8:
                    use_stride_n8_ = true;
                    break;
                case 16:
                    use_stride_p16_ = true;
                    break;
                case -16:
                    use_stride_n16_ = true;
                    break;

                default:
                    std::cerr << "ERROR: Invalid stride size " << stride_size << ". Stride sizes can be 1, -1, 2, -2, 4, -4, 8, -8, 16, or -16." << std::endl;
                    goto error;
            }
            curr = curr->next();
        }
    }

    // Check MLP selection
    if (options[MLP]) { // override default of 1
        // Followed the same option parsing style as for STRIDE_SIZE.
        char* endptr = NULL;
        int32_t mlp = static_cast<uint8_t>(strtoul(options[MLP].arg, &endptr, 10));

        switch(mlp) {
            case 1:
                use_mlp_1_ = true;
                mlp_ = 1;
                break;
            case 2:
                use_mlp_2_ = true;
                mlp_ = 2;
                break;
            case 4:
                use_mlp_4_ = true;
                mlp_ = 4;
                break;
            case 6:
                use_mlp_6_ = true;
                mlp_ = 6;
                break;
            case 8:
                use_mlp_8_ = true;
                mlp_ = 8;
                break;
            case 16:
                use_mlp_16_ = true;
                mlp_ = 16;
                break;
            case 32:
                use_mlp_32_ = true;
                mlp_ = 32;
                break;

            default:
                std::cerr << "ERROR: Invalid MLP " << mlp << ". MLP values can be 1, 2, 4, 6, 8, 16 or 32." << std::endl;
                goto error;
        }
    }

    //Make sure at least one mode is available
    if (!run_latency_ && !run_throughput_ && !run_extensions_) {
        std::cerr << "ERROR: At least one benchmark type must be selected." << std::endl;
        goto error;
    }

    //Make sure at least one access pattern is selected
    if (!use_random_access_pattern_ && !use_sequential_access_pattern_) {
        std::cerr << "ERROR: No access pattern was specified!" << std::endl;
        goto error;
    }

    //Make sure at least one read/write pattern is selected
    if (!use_reads_ && !use_writes_) {
        std::cerr << "ERROR: Throughput benchmark was selected, but no read/write pattern was specified!" << std::endl;
        goto error;
    }

    //If the user picked "all" option, override anything else they put in that is relevant.
    if (options[ALL]) {
        run_latency_ = true;
        run_throughput_ = true;
        run_extensions_ = true;
#ifdef EXT_DELAY_INJECTED_LOADED_LATENCY_BENCHMARK
        run_ext_delay_injected_loaded_latency_benchmark_ = true;
#endif
#ifdef EXT_STREAM_BENCHMARK
        run_ext_stream_benchmark_ = true;
#endif
        use_chunk_32b_ = true;
#ifdef HAS_WORD_64
        use_chunk_64b_ = true;
#endif
#ifdef HAS_WORD_128
        use_chunk_128b_ = true;
#endif
#ifdef HAS_WORD_256
        use_chunk_256b_ = true;
#endif
#ifdef HAS_WORD_512
        use_chunk_512b = true;
#endif
        use_random_access_pattern_ = true;
        use_sequential_access_pattern_ = true;
        use_reads_ = true;
        use_writes_ = true;
        use_stride_p1_ = true;
        use_stride_n1_ = true;
        use_stride_p2_ = true;
        use_stride_n2_ = true;
        use_stride_p4_ = true;
        use_stride_n4_ = true;
        use_stride_p8_ = true;
        use_stride_n8_ = true;
        use_stride_p16_ = true;
        use_stride_n16_ = true;

        use_mlp_1_ = true; // default MLP 1
        use_mlp_2_ = false;
        use_mlp_4_ = false;
        use_mlp_6_ = false;
        use_mlp_8_ = false;
        use_mlp_16_ = false;
        use_mlp_32_ = false;
    }

#ifdef HAS_WORD_64
    //Notify that 32-bit chunks are not used on random throughput benchmarks on 64-bit machines
    if (use_random_access_pattern_ && use_chunk_32b_)
        std::cerr << "NOTE: Random-access load kernels used in throughput and loaded latency benchmarks do not support 32-bit chunk sizes on 64-bit machines. These particular combinations will be omitted." << std::endl;
#endif

    //Check for help or bad options
    if (options[HELP] || options[UNKNOWN] != NULL)
        goto errorWithUsage;

    //Report final runtime configuration based on user inputs
    std::cout << std::endl;
    if (verbose_) {
        std::cout << "Verbose output enabled!" << std::endl;

        std::cout << "Benchmarking modes:" << std::endl;
        if (run_throughput_)
            std::cout << "---> Throughput" << std::endl;
        if (run_latency_) {
            std::cout << "---> ";
            if (num_worker_threads_ > 1)
                std::cout << "Loaded ";
            else
                std::cout << "Unloaded ";
            std::cout << "latency" << std::endl;
        }
        if (run_extensions_)
            std::cout << "---> Extensions" << std::endl;
        std::cout << std::endl;

        std::cout << "Benchmark settings:" << std::endl;
        std::cout << "---> Random access:                   ";
        if (use_random_access_pattern_)
            std::cout << "yes";
        else
            std::cout << "no";
        std::cout << std::endl;
        std::cout << "---> Sequential access:               ";
        if (use_sequential_access_pattern_)
            std::cout << "yes";
        else
            std::cout << "no";
        std::cout << std::endl;
        std::cout << "---> Use memory reads:                ";
        if (use_reads_)
            std::cout << "yes";
        else
            std::cout << "no";
        std::cout << std::endl;
        std::cout << "---> Use memory writes:               ";
        if (use_writes_)
            std::cout << "yes";
        else
            std::cout << "no";
        std::cout << std::endl;
        std::cout << "---> Chunk sizes:                     ";
        if (use_chunk_32b_)
            std::cout << "32 ";
#ifdef HAS_WORD_64
        if (use_chunk_64b_)
            std::cout << "64 ";
#endif
#ifdef HAS_WORD_128
        if (use_chunk_128b_)
            std::cout << "128 ";
#endif
#ifdef HAS_WORD_256
        if (use_chunk_256b_)
            std::cout << "256 ";
#endif
#ifdef HAS_WORD_512
        if (use_chunk_512b)
            std::cout << "512 ";
#endif
        std::cout << std::endl;
        std::cout << "---> Stride sizes:                    ";
        if (use_stride_p1_)
            std::cout << "1 ";
        if (use_stride_n1_)
            std::cout << "-1 ";
        if (use_stride_p2_)
            std::cout << "2 ";
        if (use_stride_n2_)
            std::cout << "-2 ";
        if (use_stride_p4_)
            std::cout << "4 ";
        if (use_stride_n4_)
            std::cout << "-4 ";
        if (use_stride_p8_)
            std::cout << "8 ";
        if (use_stride_n8_)
            std::cout << "-8 ";
        if (use_stride_p16_)
            std::cout << "16 ";
        if (use_stride_n16_)
            std::cout << "-16 ";
        std::cout << std::endl;
        std::cout << "---> Number of worker threads:        ";
        std::cout << num_worker_threads_ << std::endl;
        std::cout << "---> NUMA enabled:                    ";
#ifdef HAS_NUMA
        if (numa_enabled_)
            std::cout << "yes" << std::endl;
        else
            std::cout << "no" << std::endl;
        std::cout << "------> CPU NUMA node affinities:     ";
        for (auto it = cpu_numa_node_affinities_.cbegin(); it != cpu_numa_node_affinities_.cend(); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
        std::cout << "------> Memory NUMA node affinities:  ";
        for (auto it = memory_numa_node_affinities_.cbegin(); it != memory_numa_node_affinities_.cend(); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
#else
        std::cout << "not supported" << std::endl;
#endif
        std::cout << "---> Large pages:                     ";
#ifdef HAS_LARGE_PAGES
        if (use_large_pages_)
            std::cout << "yes" << std::endl;
        else
            std::cout << "no" << std::endl;
#else
        std::cout << "not supported" << std::endl;
#endif
        std::cout << "---> Iterations:                      ";
        std::cout << iterations_ << std::endl;
        std::cout << "---> Starting test index:             ";
        std::cout << starting_test_index_ << std::endl;
        std::cout << std::endl;
    }

        std::cout << "Working set per thread:               ";
    if (use_large_pages_) {
        size_t num_large_pages = 0;
        if (working_set_size_per_thread_ <= g_large_page_size) //sub one large page, round up to one
            num_large_pages = 1;
        else if (working_set_size_per_thread_ % g_large_page_size == 0) //multiple of large page
            num_large_pages = working_set_size_per_thread_ / g_large_page_size;
        else //larger than one large page but not a multiple of large page
            num_large_pages = working_set_size_per_thread_ / g_large_page_size + 1;
        std::cout << working_set_size_per_thread_ << " B == " << working_set_size_per_thread_ / KB  << " KB == " << working_set_size_per_thread_ / MB << " MB (fits in " << num_large_pages << " large pages)" << std::endl;
    } else {
        std::cout << working_set_size_per_thread_ << " B == " << working_set_size_per_thread_ / KB  << " KB == " << working_set_size_per_thread_ / MB << " MB (" << working_set_size_per_thread_/(g_page_size) << " pages)" << std::endl;
    }

    //Free up options memory
    delete[] options;
    delete[] buffer;

    return 0;

    errorWithUsage:
        printUsage(std::cerr, usage); //Display help message

    error:

        //Free up options memory
        delete[] options;
        delete[] buffer;

        return -1;
}

/* //TODOJ: delete?
uint8_t Configurator::getMlp() const {
    if (config_.useMlp1())
        return 1;
    if (config_.useMlp2())
        return 2;
    if (config_.useMlp4())
        return 4;
    if (config_.useMlp6())
        return 6;
    if (config_.useMlp8())
        return 8;
    if (config_.useMlp16())
        return 16;
    if (config_.useMlp32())
        return 32;
}*/

bool Configurator::check_single_option_occurrence(Option* opt) const {
    if (opt->count() > 1) {
        std::cerr << "ERROR: " << opt->name << " option can only be specified once." << std::endl;
        return false;
    }
    return true;
}
