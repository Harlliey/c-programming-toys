#include <stdio.h>
#include <utmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "utmplib.h"

#define SHOWHOST

void show_info(struct utmp *record);
void show_time(long timeval);

int main()
{
    struct utmp *prec = NULL;
    int utmpfd = -1;

    if ((utmpfd = utmp_open(UTMP_FILE)) == -1)
    {
        perror(UTMP_FILE);
        exit(1);
    }

    while ((prec = utmp_next()) != NULLUT)
    {
        show_info(prec);
    }

    utmp_close();
    return 0;
}

void show_info(struct utmp *record)
{
    if (record->ut_type != USER_PROCESS) return;

    printf("%-8.8s", record->ut_user);
    printf(" ");
    printf("%-8.8s", record->ut_line);
    printf(" ");
    show_time(record->ut_tv.tv_sec);
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", record->ut_host);
#endif
    printf("\n");
}

void show_time(long timeval)
{
    char *p = ctime(&timeval);
    printf("%12.12s", (p + 4));
}