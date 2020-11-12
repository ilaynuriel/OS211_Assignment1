#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

void
tee(int fd1, int fd2)
{
    int n;

    while((n = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, n) != n) {
            printf(1, "tee: write error\n");
            exit();
        }
    }
    if(n < 0){
        printf(1, "tee: read error\n");
        exit();
    }
}

int
main(int argc, char *argv[])
{
    int fd1,fd2;
    if(argc <= 1){
        tee(0,0);
        exit();
    }
    fd2 = open(argv[2],O_CREATE|O_WRONLY);
    if((fd1 = open(argv[1], 0)) < 0){
            printf(1, "tee: cannot open %s\n", argv[1]);
            exit();
        }
    tee(fd1,fd2);
    close(fd1);
    close(fd2);

//    for(i = 1; i < argc; i+/+){
//        printf(1,"argv %s \n", argv[i]);
//        if((fd = open(argv[i], 0)) < 0){
//            printf(1, "cat: cannot open %s\n", argv[i]);
//            exit();
//        }
//        cat(fd);
//        close(fd);
//    }
    exit();
}
