#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static void
create_pipe(int *p)
{
    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
}

static void
send(int *p)
{
    close(p[0]);
    write(p[1], "0", 1);
    close(p[1]);
}

static void
receive(int *p, char *tmp)
{
    close(p[1]);
    read(p[0], tmp, 1);
    close(p[0]);
}

int
main(int argc, char *argv[])
{
    int p1[2], p2[2];
    char tmp;

    if (argc != 1) {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    create_pipe(p1);
    create_pipe(p2);

    if (fork() == 0) {
        /* ping */
        receive(p1, &tmp);
        fprintf(1, "%d: received ping\n", getpid());
        /* pong */
        send(p2);
    } else {
        /* ping */
        send(p1);
        /* pong */
        receive(p2, &tmp);
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}