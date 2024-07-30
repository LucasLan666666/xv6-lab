#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// static char*
// fmtname(char *path)
// {
//     char *p;
//     /* Find first character after last slash. */
//     for (p = path + strlen(path); p >= path && *p != '/'; p--)
//         ;
//     p++;
//     return p;
// }

static void
find(char *path, char *patt)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            break;

        case T_DIR:
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, sizeof(de.name));
                p[sizeof(de.name)] = 0;
                if (stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                if (strcmp(de.name, patt) == 0)
                    printf("%s\n", buf);
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
                    find(buf, patt);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "Usage: find [path] [pattern]\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
