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
#define LAST_LINE 4
#define STORY_SIZE 500

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int main(int argc, char *argv[]){
  if (argc == 2){
    int semd, shmd, fd, v, r, rm;
    if (strcmp(argv[1], "-c") == 0){
      semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      if (semd == -1){
        printf("error semd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      union semun us;
      us.val = 1;
      semctl(semd, 0, SETVAL, us);
      printf("semaphore created\n");
      shmd = shmget(KEY, LAST_LINE, IPC_CREAT | IPC_EXCL | 0644);
      if (shmd == -1){
        printf("error shmd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      printf("shared memory created\n");
      fd = open("semaphone.txt", O_CREAT, 0644);
      if (fd == -1){
        printf("error fd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      printf("file created\n");
      close(fd);
    }
    if (strcmp(argv[1], "-v") == 0){
      char buffer[STORY_SIZE];
      buffer[0] = 0;
      printf("The story thus far\n");
      fd = open("semaphone.txt", O_RDONLY);
      if (fd == -1){
        printf("error fd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      r = read(fd, buffer, STORY_SIZE);
      if (r == -1){
        printf("error r %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      buffer[r] = 0;
      printf("%s\n", buffer);
      close(fd);
    }
    if (strcmp(argv[1], "-r") == 0){
      printf("trying to get in\n");
      char buffer[STORY_SIZE];
      buffer[0] = 0;
      printf("The story thus far\n");
      fd = open("semaphone.txt", O_RDONLY);
      if (fd == -1){
        printf("error fd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      r = read(fd, buffer, STORY_SIZE);
      if (r == -1){
        printf("error r %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      buffer[r] = 0;
      printf("%s\n", buffer);
      close(fd);
      rm = remove("semaphone.txt");
      if (rm == -1){
         printf("error rm %d: %s\n", errno, strerror(errno));
         exit(1);
      }
      printf("file removed\n");
      shmd = shmget(KEY, LAST_LINE, 0);
      if (shmd == -1){
        printf("error shmd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      rm = shmctl(shmd, IPC_RMID, 0);
      if (rm == -1){
        printf("error rm %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      printf("shared memory removed\n");
      semd = semget(KEY, 1, 0);
      if (semd == -1){
        printf("error semd %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      rm = semctl(semd, IPC_RMID, 0);
      if (rm == -1){
         printf("error rm %d: %s\n", errno, strerror(errno));
         exit(1);
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
