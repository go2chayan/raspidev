/*
 * rpihw.c
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <byteswap.h>

#include "rpihw.h"


#define LINE_WIDTH_MAX                           80
#define HW_VER_STRING                            "Revision"

#define PERIPH_BASE_RPI                          0x20000000
#define PERIPH_BASE_RPI2                         0x3f000000
#define PERIPH_BASE_RPI4                         0xfe000000

#define VIDEOCORE_BASE_RPI                       0x40000000
#define VIDEOCORE_BASE_RPI2                      0xc0000000

#define RPI_MANUFACTURER_MASK                    (0xf << 16)//0xf0000
#define RPI_WARRANTY_MASK                        (0xf << 24)//0xf000000    0xff =1111 1111 
#define RPI_PROCESSOR_MASK                       (0xf << 12)//0xf000

static const rpi_hw_t rpi_hw_info[] = {
    {
        .hwver = 0x003000,
        .type = RPI_HWVER_TYPE_PI4,
        .periph_base = PERIPH_BASE_RPI4,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Raspberry Pi Processor - BCM2711"
    },
    {
        .hwver = 0x002000,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Raspberry Pi Processor - BCM2837"
    },
    {
        .hwver = 0x001000,
        .type = RPI_HWVER_TYPE_PI2,
        .periph_base = PERIPH_BASE_RPI2,
        .videocore_base = VIDEOCORE_BASE_RPI2,
        .desc = "Raspberry Pi Processor - BCM2836"
    },
    {
        .hwver = 0x000000,
        .type = RPI_HWVER_TYPE_PI1,
        .periph_base = PERIPH_BASE_RPI,
        .videocore_base = VIDEOCORE_BASE_RPI,
        .desc = "Raspberry Pi Processor - BCM2835"
    },
    

};


const rpi_hw_t *rpi_hw_detect(void)
{
    const rpi_hw_t *result = NULL;
    uint32_t rev;
    unsigned i;

#ifdef __aarch64__
    // On ARM64, read revision from /proc/device-tree as it is not shown in
    // /proc/cpuinfo
    FILE *f = fopen("/proc/device-tree/system/linux,revision", "r");
    if (!f)
    {
        return NULL;
    }
    size_t read = fread(&rev, 1, sizeof(uint32_t), f);
    if (read != sizeof(uint32_t))
        goto done;
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        rev = bswap_32(rev);  // linux,revision appears to be in big endian
    #endif

    for (i = 0; i < (sizeof(rpi_hw_info) / sizeof(rpi_hw_info[0])); i++)
    {
        uint32_t hwver = rpi_hw_info[i].hwver;
        if (rev == hwver)
        {
            result = &rpi_hw_info[i];

            goto done;
        }
    }
#else
    FILE *f = fopen("/proc/cpuinfo", "r");
    char line[LINE_WIDTH_MAX];

    if (!f)
    {
        return NULL;
    }

    while (fgets(line, LINE_WIDTH_MAX - 1, f))
    {
        if (strstr(line, HW_VER_STRING))
        {
            char *substr;

            substr = strstr(line, ": ");
            if (!substr)
            {
                continue;
            }

            errno = 0;
            rev = strtoul(&substr[1], NULL, 16);  // Base 16
            if (errno)
            {
                continue;
            }

            for (i = 0; i < (sizeof(rpi_hw_info) / sizeof(rpi_hw_info[0])); i++)
            {
                uint32_t hwver = rpi_hw_info[i].hwver;

                // Take out warranty and manufacturer bits
                hwver &= RPI_PROCESSOR_MASK;
                rev &= RPI_PROCESSOR_MASK;

                if (rev == hwver)
                {
                    result = &rpi_hw_info[i];

                    goto done;
                }
            }
        }
    }
#endif
done:
    fclose(f);

    return result;
}

