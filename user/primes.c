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
sieve(int *p)
{
    int p_child[2];
    int base, n;

    close(p[1]);

    if (read(p[0], &base, sizeof(base)) == 0)
        return;

    fprintf(1, "prime %d\n", base);

    create_pipe(p_child);

    if (fork() == 0) {
        close(p[0]);
        sieve(p_child);
    } else {
        while (read(p[0], &n, sizeof(n)) != 0) {
            if (n % base != 0)
                write(p_child[1], &n, sizeof(n));
        }
        close(p_child[1]);
        close(p[0]);
        wait(0);
    }

}

int
main(int argc, char *argv[])
{
    int i;
    int p[2];

    if (argc > 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    create_pipe(p);

    if (fork() == 0) {
        sieve(p);
    } else {
        close(p[0]);
        for (i = 2; i <= 35; i++)
            write(p[1], &i, sizeof(i));
        close(p[1]);
        wait(0);
    }

    exit(0);
}