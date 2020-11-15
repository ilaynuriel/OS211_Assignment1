//
// Created by ilay on 12/11/2020.
//

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

// Command: tee 1.txt
void tee(int fd) {
    int n;
    while (1) {
        if ((n = read(0, buf, sizeof(buf))) > 0) {
            if (buf[0] == '^' && buf[1] == 'C') {
                break;
            }
            if (write(fd, buf, n) != n) {
                printf(1, "tee: write error\n");
                exit(0);
            } else
                printf(1, "%s", buf);
        }
        memset(buf, 0, sizeof(buf));
    }
}

// Command: tee 1.txt 2.txt
void tee2(int fd1, int fd2) {
    int n;

    while ((n = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, n) != n) {
            printf(1, "tee: write error\n");
            exit(0);
        }
    }
    if (n < 0) {
        printf(1, "tee: read error\n");
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int fd, fd1, fd2;

    if (argc <= 1) {
        tee(0);
        exit(0);
    }
    // Command: tee 1.txt
    if (argc == 2) {
        // If the file doesn't exist - create it
        if(open(argv[1], 0) < 0)
            fd = open(argv[1], O_CREATE | O_WRONLY);
        // If the file exist - delete it and create new one
        else {
            unlink(argv[1]);
            fd = open(argv[1], O_CREATE | O_WRONLY);
        }
        tee(fd);
        close(fd);
        exit(0);
    }
    // Command: tee 1.txt 2.txt
    if (argc == 3) {
        if(open(argv[2], 0) < 0)
            fd2 = open(argv[2], O_CREATE | O_WRONLY);
        else {
            unlink(argv[2]);
            fd2 = open(argv[2], O_CREATE | O_WRONLY);
        }
        if ((fd1 = open(argv[1], 0)) < 0) {
            printf(1, "tee: cannot open %s\n", argv[1]);
            exit(0);
        }
        tee2(fd1, fd2);
        close(fd1);
        close(fd2);
        exit(0);
    }
}
