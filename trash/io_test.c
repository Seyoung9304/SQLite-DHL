#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#define PGSIZE 10240

int main(int argc, char* argv[]){
  int PGSIZE = atoi(argv[1]);
  int nOper = atoi(argv[2]);
  int Gap = atoi(argv[3]);
  int fd1 = open("oh.txt",O_RDWR | O_CREAT, 00666);
  int i = 0;
struct timespec t1, t2;
  char buf[PGSIZE+1];
  for(i = 0; i < PGSIZE; i++){
    buf[i] = '0';
  }
  buf[PGSIZE] = '\0';
  ftruncate(fd1,0);
  sync();
  system("echo 3 > /proc/sys/vm/drop_caches");
  system("sh my_setup.sh");
  clock_gettime(CLOCK_MONOTONIC, &t1);
  int cnt = 1;
  int j;
  for(i = 0; i < 100; i++){
    for(j = 0; j < nOper; j++){
      pwrite(fd1,buf,PGSIZE, (cnt-1)*PGSIZE);
      cnt+= Gap;
    }
    fsync(fd1);
  }
  clock_gettime(CLOCK_MONOTONIC, &t2);
  long long int time = (t2.tv_sec - t1.tv_sec) * 1000000000LL + (t2.tv_nsec - t1.tv_nsec);
  printf("page size : %d\n", PGSIZE);
  printf("# of writes per txn : %d\n", nOper);
  printf("Gap(1 means seq) : %d\n", Gap);
  printf("%lld\n", time);

  return 0;
}
