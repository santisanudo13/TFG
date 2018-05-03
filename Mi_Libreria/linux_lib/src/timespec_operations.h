/*----------------------------------------------------------------------------
 *-------------------------      M a R T E   O S      ------------------------
 *----------------------------------------------------------------------------
 *                                                             {MARTE_VERSION}
 *
 *                    't i m e s p e c _ o p e r a t i o n s'
 *
 *                                      H
 *
 * File 'timespec_operations.h'                                        by MAR.
 *
 * Some basic operations with the type 'timespec'.
 *
 * {MARTE_COPYRIGHT}
 *
 *---------------------------------------------------------------------------*/
/* {<MAR}
 *
 * 10-10-01: formateado y copiado a 'include/misc'.
 *
 * 20-01-03: 'div_timespec' y 'mult_timespec': a�adidas.
 *
 * 06-11-04: a�ado 'show_timespec_s'.
 *
 * {MAR>} -------------------------------------------------------------------*/

#ifndef _MARTE_MISC_TIMESPEC_OPERATIONS_H_
#define _MARTE_MISC_TIMESPEC_OPERATIONS_H_

#include <stdio.h>
#include <time.h>

static inline int smaller_timespec (const struct timespec *t1,
                                    const struct timespec *t2)
{
        return t1->tv_sec < t2->tv_sec ||
               (t1->tv_sec == t2->tv_sec && t1->tv_nsec < t2->tv_nsec);
}

static inline int smaller_or_equal_timespec (const struct timespec *t1,
                                             const struct timespec *t2)
{
        return t1->tv_sec < t2->tv_sec || (t1->tv_sec == t2->tv_sec &&
                        t1->tv_nsec <= t2->tv_nsec);
}

static inline void incr_timespec (struct timespec *t1, const struct timespec *t2)
{
        t1->tv_sec += t2->tv_sec;
        t1->tv_nsec += t2->tv_nsec;
        if (t1->tv_nsec >= 1000000000) {
                t1->tv_sec ++;
                t1->tv_nsec -= 1000000000;
        }
}

static inline void decr_timespec (struct timespec *t1, const struct timespec *t2)
{
        if (t1->tv_nsec < t2->tv_nsec) {
                t1->tv_sec -= t2->tv_sec + 1;
                t1->tv_nsec = t1->tv_nsec + 1000000000 - t2->tv_nsec;
        } else {
                t1->tv_sec -= t2->tv_sec;
                t1->tv_nsec -= t2->tv_nsec;
        }
}

static inline void  add_timespec (struct timespec *s,
                                  const struct timespec *t1,
                                  const struct timespec*t2)
{
        s->tv_sec  = t1->tv_sec  + t2->tv_sec;
        s->tv_nsec = t1->tv_nsec + t2->tv_nsec;
        if (s->tv_nsec >= 1000000000) {
                s->tv_sec ++;
                s->tv_nsec -= 1000000000;
        }
}

static inline void div_timespec (struct timespec *s,
                                 const struct timespec *dividend,
                                 const struct timespec *divisor)
{
        long long ldividend = dividend->tv_sec*1000000000 + dividend->tv_nsec;
        long long ldivisor  = divisor->tv_sec*1000000000  + divisor->tv_nsec;
        long long result;

        result = ldividend / ldivisor;

        s->tv_sec = result / 1000000000;
        s->tv_nsec = result % 1000000000;
}

static inline void mult_timespec (struct timespec *s,
                                  const struct timespec *t1,
                                  const struct timespec *t2)
{
        long long lt1 = t1->tv_sec*1000000000 + t1->tv_nsec;
        long long lt2 = t2->tv_sec*1000000000 + t2->tv_nsec;
        long long result;

        result = lt1 / lt2;

        s->tv_sec = result / 1000000000;
        s->tv_nsec = result % 1000000000;
}

typedef long long unsigned nanosecs_t;
char str_timespec_s[40];
static inline char * show_timespec_s (struct timespec *ts)
{
        nanosecs_t ns = (nanosecs_t)(ts->tv_sec) * 1000000000 + ts->tv_nsec;
        nanosecs_t s = ns / 1000000000;
        nanosecs_t ms = ns % 1000000000;
        nanosecs_t us = ns % 1000000;
        ns = ns % 1000;
        sprintf (str_timespec_s, "%3lus%3lums%3luus%3luns", (unsigned long)s,
                 (unsigned long)(ms / 1000000), (unsigned long)(us / 1000),
                 (unsigned long)ns);
        return str_timespec_s;
}

//--------------------//
// timespec_to_double //
//--------------------//

static inline double timespec_to_double(const struct timespec *time)
{
        return time->tv_nsec*0.000000001 + (double)time->tv_sec;
}

//--------------------//
// double_to_timespec //
//--------------------//

static inline void double_to_timespec(double time, struct timespec *ts)
{
        ts->tv_sec = (long) time;
        ts->tv_nsec = (long)((time - (double)ts->tv_sec) * 1000000000);
}

#endif /* _MARTE_MISC_TIMESPEC_OPERATIONS_H_ */
