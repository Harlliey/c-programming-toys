//
// Created by Harrywei on 2020/12/27.
//
#include "utmplib.h"

static struct utmp utmpbuf[NRECS];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open(char *filename)
{
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = num_recs = 0;
    return fd_utmp;
}

struct utmp *utmp_next()
{
    if (fd_utmp == -1)
    {
        return NULLUT;
    }

    if (cur_rec == num_recs && utmp_reload() == 0)
    {
        return NULLUT;
    }

    struct utmp *prec = &utmpbuf[cur_rec];
    cur_rec ++;
    return prec;
}

int utmp_reload()
{
    int bytes_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
    num_recs = bytes_read / UTSIZE;
    cur_rec = 0;
    return num_recs;
}

void utmp_close()
{
    if (fd_utmp != -1)
    {
        close(fd_utmp);
    }
}