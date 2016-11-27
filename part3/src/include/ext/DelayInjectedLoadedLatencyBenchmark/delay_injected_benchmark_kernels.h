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
 * @brief Header file for benchmark kernel functions with integrated delays for doing the actual work we care about. :)
 */

#ifndef __DELAY_INJECTED_BENCHMARK_KERNELS_H
#define __DELAY_INJECTED_BENCHMARK_KERNELS_H

//Headers
#include <common.h>

//Libraries
#include <cstdint>
#if defined(_WIN32) && defined(ARCH_INTEL_AVX)
#include <intrin.h> //For Intel intrinsics
#endif
#if defined(__gnu_linux__) && defined(ARCH_INTEL_AVX)
#include <immintrin.h> //For Intel intrinsics
#endif

//Helper macros for inserting delays with nops.
#ifdef _WIN32
#define my_nop() __nop() //Works on both x86 family and ARM family as-is
#endif

#ifdef __gnu_linux__
#define my_nop() asm("nop") //Works on both x86 family and ARM family as-is
#endif

#define my_nop2() my_nop(); my_nop()
#define my_nop4() my_nop2(); my_nop2()
#define my_nop8() my_nop4(); my_nop4()
#define my_nop16() my_nop8(); my_nop8()
#define my_nop32() my_nop16(); my_nop16()
#define my_nop64() my_nop32(); my_nop32()
#define my_nop128() my_nop64(); my_nop64()
#define my_nop256() my_nop128(); my_nop128()
#define my_nop512() my_nop256(); my_nop256()
#define my_nop1024() my_nop512(); my_nop512()

namespace xmem {
    
    /***********************************************************************
     ***********************************************************************
     ******************* THROUGHPUT-RELATED BENCHMARK KERNELS **************
     ***********************************************************************
     ***********************************************************************/
    
    /* -------------------- DUMMY BENCHMARK ROUTINES -------------------------- */
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay2(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay16(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay64(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay128(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay256(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 32 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word32_Delay512plus(void* start_address, void* end_address);
    
#ifdef HAS_WORD_64
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay2(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay16(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay64(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay128(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 64 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word64_Delay256plus(void* start_address, void* end_address);
#endif

#ifdef HAS_WORD_128
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay2(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay16(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay64(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 128 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word128_Delay128plus(void* start_address, void* end_address);
#endif

#ifdef HAS_WORD_256 
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay2(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay16(void* start_address, void* end_address);

    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Used for measuring the time spent doing everything in delay-injected forward sequential Word 256 loops except for the memory access and delays themselves.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t dummy_forwSequentialLoop_Word256_Delay64plus(void* start_address, void* end_address);
#endif

    /* --------------------- CORE BENCHMARK ROUTINES --------------------------- */

    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 1 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 2 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay2(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 4 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 8 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 16 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay16(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 32 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 64 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay64(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 128 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay128(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 256 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay256(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 512 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay512(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 32-bit chunks. 1024 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word32_Delay1024(void* start_address, void* end_address);

#ifdef HAS_WORD_64
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 1 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 2 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay2(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 4 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 8 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 16 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay16(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 32 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 64 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay64(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 128 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay128(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 256 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay256(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 512 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay512(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 1024 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word64_Delay1024(void* start_address, void* end_address);
#endif

#ifdef HAS_WORD_128
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 1 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 2 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay2(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 4 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 8 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 16 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay16(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 32 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 64 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay64(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 128 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay128(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 256 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay256(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 512 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay512(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 128-bit chunks. 1024 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word128_Delay1024(void* start_address, void* end_address);
#endif

#ifdef HAS_WORD_256
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 1 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay1(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 2 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay2(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 4 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay4(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 8 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay8(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 16 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay16(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 32 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay32(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 64 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay64(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 128 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay128(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 256 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay256(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 512 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay512(void* start_address, void* end_address);
    
    /**
     * @brief Walks over the allocated memory forward sequentially, reading in 64-bit chunks. 1024 delays (nops) are inserted between memory instructions.
     * @param start_address The beginning of the memory region of interest.
     * @param end_address The end of the memory region of interest.
     * @returns Undefined.
     */
    int32_t forwSequentialRead_Word256_Delay1024(void* start_address, void* end_address);
#endif
};

#endif
