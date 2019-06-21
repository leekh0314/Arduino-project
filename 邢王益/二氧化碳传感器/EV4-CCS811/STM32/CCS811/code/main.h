/*
 * main.h:
 * Copyright (c) 2014-2017 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
#ifndef __MAIN_H_
#define __MAIN_H_

#ifndef __cplusplus /* In C++, 'bool', 'true' and 'false' and keywords */
    #define bool _Bool
    #define true 1
    #define TRUE true
    #define false 0
    #define FALSE false
#else
    #ifdef __GNUC__
    /* GNU C++ supports direct inclusion of stdbool.h to provide C99
        compatibility by defining _Bool */
        #define _Bool bool
    #endif
#endif

#endif /* __MAIN_H_ */


