/*
 * =======================================================================================
 *
 *      Filename:  error.h
 *
 *      Description:  Central error handling macros
 *
 *      Version:   <VERSION>
 *      Released:  <DATE>
 *
 *      Author:  Jan Treibig (jt), jan.treibig@gmail.com
 *      Project:  likwid
 *
 *      Copyright (C) 2013 Jan Treibig 
 *
 *      This program is free software: you can redistribute it and/or modify it under
 *      the terms of the GNU General Public License as published by the Free Software
 *      Foundation, either version 3 of the License, or (at your option) any later
 *      version.
 *
 *      This program is distributed in the hope that it will be useful, but WITHOUT ANY
 *      WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 *      PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along with
 *      this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * =======================================================================================
 */

#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <string.h>
#include <msr.h>
#include <pci.h>

extern int perfmon_verbosity;

#define str(x) #x

#define FINALIZE      msr_finalize(); \
                      pci_finalize()

#define ERRNO_PRINT fprintf(stderr, "ERROR - [%s:%d] %s\n", __FILE__, __LINE__, strerror(errno))

#define ERROR  \
    ERRNO_PRINT; \
    FINALIZE; \
    exit(EXIT_FAILURE)

#define ERROR_PLAIN_PRINT(msg) \
   fprintf(stderr,  "ERROR - [%s:%s:%d] " str(msg) "\n", __FILE__, __func__,__LINE__);  \
   FINALIZE;


#define ERROR_PRINT(fmt, ...) \
   fprintf(stderr,  "ERROR - [%s:%s:%d] %s.\n" str(fmt) "\n", __FILE__,  __func__,__LINE__, strerror(errno), __VA_ARGS__);  \
   FINALIZE;

#define CHECK_ERROR(func, msg)  \
    if ((func) < 0) { \
        fprintf(stderr, "ERROR - [%s:%d] " str(msg) " - %s \n", __FILE__, __LINE__, strerror(errno));  \
    }

#define CHECK_AND_RETURN_ERROR(func, msg)  \
    if ((func) < 0) { \
        fprintf(stderr, "ERROR - [%s:%d] " str(msg) " - %s \n", __FILE__, __LINE__, strerror(errno));  \
        return errno; \
    }

#define EXIT_IF_ERROR(func, msg)  \
    if ((func) < 0) {  \
        fprintf(stderr,"ERROR - [%s:%d] " str(msg) " - %s \n", __FILE__, __LINE__, strerror(errno)); \
        FINALIZE; \
        exit(EXIT_FAILURE); \
    }

#define DEBUGLEV_ONLY_ERROR 0
#define DEBUGLEV_INFO 1
#define DEBUGLEV_DETAIL 2
#define DEBUGLEV_DEVELOP 3

#define VERBOSEPRINTREG(cpuid,reg,flags,msg) \
    if (perfmon_verbosity == DEBUGLEV_DEVELOP) \
    { \
        printf("DEBUG - [%s:%d] "  str(msg) " [%d] Register 0x%llX , Flags: 0x%llX \n",  \
                __func__, __LINE__,  (cpuid), LLU_CAST (reg), LLU_CAST (flags)); \
        fflush(stdout);  \
    }
    
#define VERBOSEPRINTPCIREG(cpuid,dev,reg,flags,msg) \
    if (perfmon_verbosity == DEBUGLEV_DEVELOP) \
    { \
        printf("DEBUG - [%s:%d] "  str(msg) " [%d] Device %d Register 0x%llX , Flags: 0x%llX \n",  \
                __func__, __LINE__,  (cpuid), dev, LLU_CAST (reg), LLU_CAST (flags)); \
        fflush(stdout);  \
    }


#define DEBUG_PRINT(lev, fmt, ...) \
    if ((lev >= 0) && (lev <= perfmon_verbosity)) { \
        fprintf(stdout, "DEBUG: " str(fmt) "\n", __VA_ARGS__); \
        fflush(stdout); \
    }

#define DEBUG_PLAIN_PRINT(lev, msg) \
    if ((lev >= 0) && (lev <= perfmon_verbosity)) { \
        fprintf(stdout, "DEBUG: " str(msg) "\n");  \
        fflush(stdout); \
    }
    
#define CHECK_MSR_WRITE_ERROR(func) CHECK_AND_RETURN_ERROR(func, MSR write operation failed);
#define CHECK_MSR_READ_ERROR(func) CHECK_AND_RETURN_ERROR(func, MSR read operation failed);
#define CHECK_PCI_WRITE_ERROR(func) CHECK_AND_RETURN_ERROR(func, PCI write operation failed);
#define CHECK_PCI_READ_ERROR(func) CHECK_AND_RETURN_ERROR(func, PCI read operation failed);
#define CHECK_POWER_READ_ERROR(func) CHECK_AND_RETURN_ERROR(func, Power register read operation failed);
#define CHECK_TEMP_READ_ERROR(func) CHECK_AND_RETURN_ERROR(func, Temperature register read operation failed);

#endif /*ERROR_H*/
