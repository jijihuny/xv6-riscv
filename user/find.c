#include <kernel/types.h>
#include <kernel/stat.h>
#include <kernel/fs.h>
#include <kernel/fcntl.h>
#include <user/user.h>


void
find(char *path, char *name) 
{
    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            fprintf(2, "find: %s is not directory", path);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) { break; }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof de) == sizeof de) {
                if(de.inum == 0) { continue; }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0) {
                    fprintf(2, "find: cannot stat %s\n", buf);
                    continue;
                }

                switch(st.type) {
                    case T_DEVICE:
                    case T_FILE:
                        if(strcmp(de.name, name) == 0) {
                            fprintf(1, "%s\n", buf);
                        }
                        break;
                    case T_DIR:
                        if(strcmp(de.name, ".") && strcmp(de.name, "..")) {
                            find(buf, name);
                        }
                        break;
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "find [path] [name]\n");
        exit(1);
    } else {
        find(argv[1], argv[2]);
    }
    exit(0);
}