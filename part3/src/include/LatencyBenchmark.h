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
 * @brief Header file for the LatencyBenchmark class.
 */

#ifndef LATENCY_BENCHMARK_H
#define LATENCY_BENCHMARK_H

//Headers
#include <Benchmark.h>
#include <common.h>

//Libraries
#include <cstdint>
#include <string>

namespace xmem {

    /**
     * @brief A type of benchmark that measures memory latency via random pointer chasing. Loading may be provided with separate threads which access memory as quickly as possible using given access patterns.
     */
    class LatencyBenchmark : public Benchmark {
    public:

        /**
         * @brief Constructor. Parameters are passed directly to the Benchmark constructor. See Benchmark class documentation for parameter semantics.
         */
        LatencyBenchmark(
            void* mem_array,
            size_t len,
            uint32_t iterations,
            uint32_t num_worker_threads,
            uint32_t mem_node,
            uint32_t cpu_node,
            pattern_mode_t pattern_mode,
            rw_mode_t rw_mode,
            chunk_size_t chunk_size,
            int32_t stride_size,
            uint8_t mlp,
            std::vector<PowerReader*> dram_power_readers,
            std::string name
        );

        /**
         * @brief Destructor.
         */
        virtual ~LatencyBenchmark() {}

        /**
         * @brief Get the average load throughput in MB/sec that was imposed on the latency measurement during the given iteration.
         * @brief iter The iteration of interest.
         * @returns The average throughput in MB/sec.
         */
        double getLoadMetricOnIter(uint32_t iter) const;

        /**
         * @brief Get the overall arithmetic mean load throughput in MB/sec that was imposed on the latency measurement.
         * @returns The mean throughput in MB/sec.
         */
        double getMeanLoadMetric() const;

        /**
         * @brief Reports benchmark configuration details to the console.
         */
        virtual void reportBenchmarkInfo() const;

        /**
         * @brief Reports results to the console.
         */
        virtual void reportResults() const;

    protected:
        virtual bool runCore();

        std::vector<double> load_metric_on_iter_; /**< Load metrics for each iteration of the benchmark. This is in MB/s. */
        double mean_load_metric_; /**< The average load throughput in MB/sec that was imposed on the latency measurement. */
    };
};

#endif
