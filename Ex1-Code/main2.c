#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

//writes text from buffer(buff) to file that has file descriptor fd.
void doWrite(int fd, const char *buff, int len){
size_t  idx;
ssize_t wcnt;
idx = 0;
do{
wcnt = write(fd, buff + idx, len - idx);
if(wcnt == -1){
perror("write");

exit(1);
}
idx += wcnt;
} while(idx < len);
}

//writes text from infile to file that has file descriptor fd.
void write_file(int fd, const char *infile){
int fd1;
fd1 = open(infile, O_RDONLY);
if(fd1 == -1){
perror(infile);
exit(1);
}
char buff[1024];
ssize_t rcnt;

rcnt = read(fd1, buff, sizeof(buff)-1);
if(rcnt == 0){
doWrite(fd, buff, strlen(buff));
}

if(rcnt == -1){
perror("read");
exit(1);
}
buff[rcnt] = '\0';
doWrite(fd, buff, strlen(buff));
close(fd1);

}

int main(int argc, char *argv[]){
int fd3;
int mode;

if(argc == 3){
int oflags;
oflags = O_CREAT | O_WRONLY | O_TRUNC;
mode = S_IRUSR | S_IWUSR;
char outfile[1024];
strcat(outfile, argv[0]);
strcat(outfile, ".out");


fd3 = open(outfile, oflags, mode);
write_file(fd3, argv[1]);
write_file(fd3, argv[2]);
close(fd3);
}
else if(argc == 4){
int oflags;
oflags = O_CREAT | O_WRONLY | O_TRUNC;
mode = S_IRUSR | S_IWUSR;

fd3 = open(argv[3], oflags, mode);
write_file(fd3, argv[1]);
write_file(fd3, argv[2]);

close(fd3);
}
else{
printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
}
return 0;
}

