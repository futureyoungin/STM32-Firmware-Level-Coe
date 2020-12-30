/*
 * def.h
 *
 *  Created on: 2020. 12. 29.
 *      Author: MCES
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_



#endif /* SRC_COMMON_DEF_H_ */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>


#if defined (__linux__)
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#elif defined (__WIN32__) || (__WIN64__)
#include <Windows.h>

#endif



#define _DEF_UART1        0
#define _DEF_UART2        1
#define _DEF_UART3        2
#define _DEF_UART4        3
