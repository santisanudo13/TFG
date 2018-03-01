/**
 * @file marte_pistorms_internal.h
 * @author Carlos Ayerbe González
 * @date 8 Mar 2017
 * @brief Library to add a debugger into the code.
 * @version 1.0
 *
 * This file adds a debugger to the aplication, if you add the library and
 * uncomment the constant "#define DBG" the aplication is going to run the debugger.
 */



/* Defines for MARTE_PISTORMS_INTERNAL */
#ifndef MARTE_PISTORMS_INTERNAL
#define MARTE_PISTORMS_INTERNAL
#define DEBUGER
#define DBG


/*Debug*/
#ifdef DBG
# define printf_dbg(...) printf(__VA_ARGS__)
#else
# define printf_dbg(...)
#endif

/*Debug*/
#ifdef DEBUGER
# define printf_debuger(...) printf(__VA_ARGS__)
#else
# define printf_debuger(...)
#endif




#endif /* MARTE_PISTORMS_INTERNAL */
