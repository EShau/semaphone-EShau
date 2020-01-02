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
#define LAST_LINE 200

int main(){
  printf("trying to get in\n");
  int semd;
  struct sembuf sb;
  int op;
  int shmd;
  char * data;
  int fd;
  int w;
  int shmt;
  int c;
  semd = semget(KEY, 1, 0);
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  sb.sem_num = 0;
  sb.sem_flg = 0;
  sb.sem_op = -1;
  op = semop(semd, &sb, 1);
  if (op == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  shmd = shmget(KEY, LAST_LINE, 0);
  if (shmd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  sb.sem_num = 0;
  sb.sem_flg = 0;
  sb.sem_op = -1;
  op = semop(semd, &sb, 1);
  data = shmat(shmd, 0, 0);
  fd = open("semaphone.txt", O_WRONLY | O_APPEND);
  if (fd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  printf("Last addition: %s\n", data);
  printf("Your addition: ");
  fgets(data, LAST_LINE, stdin);
  w = write(fd, data, strlen(data));
  if (w == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  shmt = shmdt(data);
  if (shmt == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  c = close(fd);
  if (c == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
  sb.sem_op = 1;
  op = semop(semd, &sb, 1);
  if (op == -1){
    printf("error %d: %s\n", errno, strerror(errno));
    //exit(1);
  }
}
