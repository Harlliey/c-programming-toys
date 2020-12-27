//
// Created by Harrywei on 2020/12/27.
//

#ifndef C_PROGRAMMING_TOYS_UTMPLIB_H
#define C_PROGRAMMING_TOYS_UTMPLIB_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp*) NULL)
#define UTSIZE (sizeof(struct utmp))

int utmp_open(char *filename);
struct utmp *utmp_next();
int utmp_reload();
void utmp_close();

#endif //C_PROGRAMMING_TOYS_UTMPLIB_H
