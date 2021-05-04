/*
 * Updated to C++, zedwood.com 2012
 * Based on Olivier Gay's version
 * See Modified BSD License below: 
 *
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Issue date:  04/30/2005
 * http://www.ouah.org/ogay/sha2/
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
//
// Reference: http://www.zedwood.com/article/cpp-sha512-function
//
#pragma once
#include <string>

class SHA512
{
  protected:
    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    const static uint64 sha512_k[];
    static const unsigned int SHA384_512_BLOCK_SIZE = (1024 / 8);

  public:
    void init();
    void update(const unsigned char* message, unsigned int len);
    void final(unsigned char* digest);
    static const unsigned int DIGEST_SIZE = (512 / 8);

  protected:
    void transform(const unsigned char* message, unsigned int block_nb);
    unsigned int m_tot_len;
    unsigned int m_len;
    unsigned char m_block[2 * SHA384_512_BLOCK_SIZE];
    uint64 m_h[8];
};

std::string sha512(std::string input);

#define SHA2_SHFR(x, n) (x >> n)
#define SHA2_ROTR(x, n) ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n) ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z) ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA512_F1(x) (SHA2_ROTR(x, 28) ^ SHA2_ROTR(x, 34) ^ SHA2_ROTR(x, 39))
#define SHA512_F2(x) (SHA2_ROTR(x, 14) ^ SHA2_ROTR(x, 18) ^ SHA2_ROTR(x, 41))
#define SHA512_F3(x) (SHA2_ROTR(x, 1) ^ SHA2_ROTR(x, 8) ^ SHA2_SHFR(x, 7))
#define SHA512_F4(x) (SHA2_ROTR(x, 19) ^ SHA2_ROTR(x, 61) ^ SHA2_SHFR(x, 6))
#define SHA2_UNPACK32(x, str)              \
    {                                      \
        *((str) + 3) = (uint8)((x));       \
        *((str) + 2) = (uint8)((x) >> 8);  \
        *((str) + 1) = (uint8)((x) >> 16); \
        *((str) + 0) = (uint8)((x) >> 24); \
    }
#define SHA2_UNPACK64(x, str)              \
    {                                      \
        *((str) + 7) = (uint8)((x));       \
        *((str) + 6) = (uint8)((x) >> 8);  \
        *((str) + 5) = (uint8)((x) >> 16); \
        *((str) + 4) = (uint8)((x) >> 24); \
        *((str) + 3) = (uint8)((x) >> 32); \
        *((str) + 2) = (uint8)((x) >> 40); \
        *((str) + 1) = (uint8)((x) >> 48); \
        *((str) + 0) = (uint8)((x) >> 56); \
    }
#define SHA2_PACK64(str, x)                                                                                                                                                                                                                                                    \
    {                                                                                                                                                                                                                                                                          \
        *(x) = ((uint64) * ((str) + 7)) | ((uint64) * ((str) + 6) << 8) | ((uint64) * ((str) + 5) << 16) | ((uint64) * ((str) + 4) << 24) | ((uint64) * ((str) + 3) << 32) | ((uint64) * ((str) + 2) << 40) | ((uint64) * ((str) + 1) << 48) | ((uint64) * ((str) + 0) << 56); \
    }
