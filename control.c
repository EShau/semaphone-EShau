#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 69420
#define SEG_SIZE 200

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int main(int argc, char *argv[]){
  if (argc == 2){
    int semd;
    int shmd;
    int fd;
    int v;
    int r;
    if (strcmp(argv[1], "-c\n")){
      semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      if (semd == -1){
        printf("error %d: %s\n", errno, strerror(errno));
        //exit(1);
      }
      union semun us;
      us.val = 1;
      semctl(semd, 0, SETVAL, us);
      printf("semaphore created\n");
      shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0644);
      if (shmd == -1){
        printf("error %d: %s\n", errno, strerror(errno));
        //exit(1);
      }
      printf("shared memory created\n");
      fd = open("semaphone.txt", O_CREAT, 0644);
      if (fd == -1){
        printf("error %d: %s\n", errno, strerror(errno));
        //exit(1);
      }
      printf("file created\n");
      close(fd);
    }
    if (strcmp(argv[1], "-r\n")){
      // r = remove("semaphone.txt");
      // if (r == -1){
      //   printf("error %d: %s\n", errno, strerror(errno));
      //   exit(1);
      // }
      // printf("file removed\n");
      r = semctl(shmd, IPC_RMID, 0);
      if (r == -1){
         printf("error %d: %s\n", errno, strerror(errno));
         //exit(1);
      }
      printf("shared memory removed\n");
      r = shmctl(semd, IPC_RMID, 0);
      if (r == -1){
        printf("error %d: %s\n", errno, strerror(errno));
        //exit(1);
      }
      printf("semaphore removed\n");
    }
  }
  else{
    if (argc > 2){
      printf("Too many arguments!\n");
    }
    else{
      printf("Too few arguments!\n");
    }
  }
}
