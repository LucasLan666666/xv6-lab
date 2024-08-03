#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFSIZ 512

int
isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int
main(int argc, char *argv[])
{
    int i, n;
    char buf[BUFSIZ];
    char *p, *q;
    int len = 0;
    char *args[MAXARG];

    if (argc == 1) {
        fprintf(2, "Usage: xargs [command]\n");
        exit(1);
    }

    /* pad previous arguments */
    for (i = 0; i < argc - 1 && i < MAXARG - 1; i++)
        args[i] = argv[i + 1];

    /* read input */
    while ((n = read(0, buf + len, BUFSIZ - len - 1)) > 0)
        len += n;
    buf[len] = '\0';

    /* input -> arguments */
    for (q = p = buf; *p != '\0'; p++) {
        if (isspace(*p)) {
            *p = '\0';
            if (q != p) {
                args[i++] = q;
                if (i >= MAXARG - 1)
                    break;
            }
            q = p + 1;
        }
    }

    if (q != p && i < MAXARG - 1)
        args[i++] = q;

    args[i] = 0;

    exec(argv[1], args);
    fprintf(2, "exec failed\n");
    exit(1);
}
