//
// Created by Harrywei on 2020/12/27.
//
#include <stdio.h>
#include <utmp.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST

void show_info(struct utmp *record);
void show_time(long timeval);

int main()
{
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        show_info(&current_record);
    }

    close(utmpfd);
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
