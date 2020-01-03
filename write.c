#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 69420
#define LAST_LINE 4

int main(){
  printf("trying to get in\n");
  struct sembuf sb;
  int * size;
  char data[256];
  int semd, op, shmd, fd, r, w, shmt, c;
  semd = semget(KEY, 1, 0);
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  sb.sem_num = 0;
  sb.sem_flg = 0;
  sb.sem_op = -1;
  op = semop(semd, &sb, 1);
  if (op == -1){
    printf("error op %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  shmd = shmget(KEY, LAST_LINE, 0);
  if (shmd == -1){
    printf("error shmd %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  size = shmat(shmd, 0, 0);
  fd = open("semaphone.txt", O_RDWR);
  if (fd == -1){
    printf("error fd %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  char buffer[256];
  printf("%d\n", * size);
  lseek(fd, -*size, SEEK_END);
  r = read(fd, buffer, *size);
  if (r == -1){
    printf("error fd %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  buffer[*size] = 0;
  printf("Last addition: %s\n", buffer);
  printf("Your addition: ");
  fgets(data, 256, stdin);
  data[strlen(data)] = 0;
  * size = strlen(data);
  w = write(fd, data, strlen(data));
  if (w == -1){
    printf("error w %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  shmt = shmdt(size);
  if (shmt == -1){
    printf("error shmt %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  c = close(fd);
  if (c == -1){
    printf("error c %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  sb.sem_op = 1;
  op = semop(semd, &sb, 1);
  if (op == -1){
    printf("error op %d: %s\n", errno, strerror(errno));
    exit(1);
  }
}
