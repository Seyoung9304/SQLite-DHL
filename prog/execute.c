#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sqlite3.h"
#include <malloc.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <pthread.h>
#include <stdlib.h>

#define INPUTRANGE 100000000LL 

#ifdef SEVERAL
  struct timespec se1,se2;
  long long * txn_latencies = NULL;
  int nTxn = 0;
#endif

long long *input=NULL;
long long *output=NULL;
//int *output;
int cnt=0;
int prealloc = 100;
int walsize = 0;
int period=100;
int util = 0;
int num_cycle=0;
int page_size = 4;
long long total_write_time=0;
long long total_fsync_time=0;
int write_cnt=0;
int fsync_cnt=0;
long long avg_write_time = 0;
long long avg_fsync_time = 0;
int MAX_SH_SIZE = 0;
int correctness = 0;
int store_type = 0;
int Env = 0;
int op_type = 0;
int mycur = 1;
int warmed = -1;
  int warm_ = 0;

struct timespec t3,t4;

char db_name[100] = "j.db";
char  execute_name[100] = "execution_queries";

extern char* optarg;
  //Variable initialization
  char buf[1000];
  char *zErr = NULL;
  int rc = 0;
  char *sql = NULL;
  sqlite3 *db = NULL;
  int cret = 0;
  int nData = 1000;
  int numOp = 1;
  int seed = 0;
  int nThread = 0;
  int output_type = 0;
//  int num_repeat = 1;
  int journal_mode = 0;
  int record_size = 80;
  int select_opt = 0;
  int rm_opt = 0;
  char * record = NULL;
  char * update_record = NULL;

struct t_arg{
  //Start from and End to
  int st;
  int ed;
};


void *insert(void *arg){
  sqlite3 * db_ = NULL;
  sqlite3_open(db_name, &db_);
  char buff_[1000];
  int rc_= 0;
  struct t_arg* my_arg = (struct t_arg*) arg;
  //st <= ... < ed
  int st = my_arg->st;
  int ed = my_arg->ed;
  int N = ed - st;
  //Run insertion
  int num_txn = N / numOp;
  int remainder = N - (num_txn * numOp);
  int k,j;
  char *sql_ = "insert into episodes values ('%lld', '%s')";
  char *sql_select = "SELECT * FROM episodes WHERE id = %lld";
  char * begin = "BEGIN";
  int iter = st;
  //printf("st : %d ed : %d\n",st,ed);
  for(j = 0; j < num_txn+1; j++){
    //sqlite3_mutex_enter(sqlite3_db_mutex(db));
    if(j%2 == 0)
      begin = "BEGIN IMMEDIATE";
    else
      begin = "BEGIN";
    rc_ = SQLITE_BUSY;
    while(rc_ == SQLITE_BUSY){
    rc_ = sqlite3_exec(db, begin, NULL, NULL, &zErr);
    //printf("st : %d : SQLITE_BEGIN\n", st);
    if(rc_ != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }
    }

    for(k = 0; (j < num_txn && k < numOp) || (j == (num_txn) && k < remainder); k++){
      //printf("input[%d] : %lld\n",iter, input[iter]);
      if(j%2 == 0){
        sprintf(buff_, sql_, input[iter],record);
      } else {
        sprintf(buff_, sql_select, input[iter+nData]);
      }
      rc_ = SQLITE_BUSY;
      while(rc_ == SQLITE_BUSY){
      rc_ = sqlite3_exec(db, buff_, NULL, NULL, &zErr);
      if(rc_ != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }
      }
      iter++;
    }
    rc_ = SQLITE_BUSY;
    while(rc_ == SQLITE_BUSY){
    rc_ = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
    //printf("st : %d : SQLITE_COMMIT\n", st);
    if(rc_ != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        //printf("key : %ld j: %d\n",key, j);
        sqlite3_free(zErr);
      }
    }
    }
    //sqlite3_mutex_leave(sqlite3_db_mutex(db));
  }
}
void *delete(void *arg){
  char buff_[1000];
  int rc_= 0;
  struct t_arg* my_arg = (struct t_arg*) arg;
  //st <= ... < ed
  int st = my_arg->st;
  int ed = my_arg->ed;
  int N = ed - st;
  //Run insertion
  int num_txn = N / numOp;
  int remainder = N - (num_txn * numOp);
  int k,j;
  char *sql_ = "DELETE FROM episodes WHERE id = %lld;";
  int iter = st;
  //printf("st : %d ed : %d\n",st,ed);
  for(j = 0; j < num_txn+1; j++){
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    rc_ = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
    //printf("st : %d : SQLITE_BEGIN\n", st);
    if(rc_ == SQLITE_BUSY)
      printf("SQLITE_BUSY\n");
    if(rc_ != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }

    for(k = 0; (j < num_txn && k < numOp) || (j == (num_txn) && k < remainder); k++){
      //printf("input[%d] : %lld\n",iter, input[iter]);
      sprintf(buff_, sql_, input[iter]);
      rc_ = sqlite3_exec(db, buff_, NULL, NULL, &zErr);
      if(rc_ != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }
      iter++;
    }
    rc_ = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
    //printf("st : %d : SQLITE_COMMIT\n", st);
    if(rc_ == SQLITE_BUSY)
      printf("SQLITE_BUSY\n");
    if(rc_ != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        //printf("key : %ld j: %d\n",key, j);
        sqlite3_free(zErr);
      }
    }
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
  }
}
void *update(void *arg){
  char buff_[1000];
  int rc_= 0;
  struct t_arg* my_arg = (struct t_arg*) arg;
  //st <= ... < ed
  int st = my_arg->st;
  int ed = my_arg->ed;
  int N = ed - st;
  //Run insertion
  int num_txn = N / numOp;
  int remainder = N - (num_txn * numOp);
  int k,j;
  char *sql_ = "UPDATE episodes SET name = '%s' WHERE id = %lld;";
  int iter = st;
  //printf("st : %d ed : %d\n",st,ed);
  for(j = 0; j < num_txn+1; j++){
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    rc_ = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
    //printf("st : %d : SQLITE_BEGIN\n", st);
    if(rc_ != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }

    for(k = 0; (j < num_txn && k < numOp) || (j == (num_txn) && k < remainder); k++){
      //printf("input[%d] : %lld\n",iter, input[iter]);
      sprintf(buff_, sql_, update_record, input[iter]);
      rc_ = sqlite3_exec(db, buff_, NULL, NULL, &zErr);
      if(rc_ != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }
      iter++;
    }
    rc_ = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
    //printf("st : %d : SQLITE_COMMIT\n", st);
    if(rc_ != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        //printf("key : %ld j: %d\n",key, j);
        sqlite3_free(zErr);
      }
    }
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
  }
}
//For selection
int compare(const void * a, const void * b){
  if ( *(long long*)a <  *(long long*)b ) return -1;
  if ( *(long long*)a == *(long long*)b ) return 0;
  if ( *(long long*)a >  *(long long*)b ) return 1;
}
static int callback_default(void *data, int argc, char **argv, char **azColName){
  int i;
  fprintf(stderr, "%s: ", (const char*)data);
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
int callback_cnt = 0;
static int callback(void *data, int argc, char **argv, char **azColName){
  static int i = 0;
  if(op_type == 2){
    if(atoll(argv[0]) == output[i]){ // correct record
      //printf("%d key: %lld value: %s\n", i, atoll(argv[0]),argv[1]);
    } else {
      printf("%d key: %lld value: %s (incorrect! expected record key is %lld.)\n", i, atoll(argv[0]), argv[1], output[i]);
      correctness = 1;
    }
    i++;
    callback_cnt++;
    return 0;
  }
  if(atoll(argv[0]) == output[i]){ // correct record
    //printf("%d key: %lld\n", i, atoll(argv[0]));
  } else {
    printf("%d key: %lld (incorrect! expected record key is %lld.)\n", i, atoll(argv[0]), output[i]);
    correctness = 1;
  }
  i++;
  callback_cnt++;
  return 0;
}

int main(int argc, char **argv){

  if(argc == 1){ // go to manual
    goto help;
  }
  //get options from input commend
  while((cret=getopt(argc,argv,"n:o:b:p:j:R:P:t:T:S:h:x:w:e:d:u:z:c:"))!=EOF){
    switch(cret){
      case 'n':
        nData = atoi(optarg);
        break;
      case 'o':
        numOp = atoi(optarg);

        break;
      case 'p':
        output_type = atoi(optarg);

        break;
      case 'j':
        journal_mode = atoi(optarg);

        break;
      case 'R':
        record_size = atoi(optarg);

        break;
      case 'P':
        page_size = atoi(optarg);

        break;
      case 't':
        op_type = atoi(optarg);

        break;
      case 'w':
        warm_ = atoi(optarg);

        break;
      case 'T':
        nThread = atoi(optarg);

        break;
      case 'S':
        select_opt = atoi(optarg);

        break;
      case 'x':
        rm_opt = atoi(optarg);

        break;
      case 'b':
        warmed = atoi(optarg);
        break;
      case 'e':
	memset(execute_name,0,100);
        strcpy(execute_name, optarg);
	break;
      case 'z':
        period = atoi(optarg);

        break;
      case 'c':
        prealloc = atoi(optarg);

        break;
      case 'd':
	memset(db_name,0,100);
        strcpy(db_name, optarg);

        break;
      case 'u':
        util = atoi(optarg);
        break;
      case 'h':
      help:
        printf("-n val : val number of records(default = 1000)\n");
        printf("-o val : val operations / 1 Txn(default = 1)\n");
        printf("-p val : set the output format.\n"); 
        printf("         0 : print all the detail.(default)\n");
        printf("         1 : print total execution time only.\n");
        printf("         2 : print (fsync time, write time, computation time) only.\n");
        printf("         3 : print Txn / sec only.\n");
        printf("         4 : print throughput of each type of txns(TPCC only).\n");
        printf("         5 : print (fsync time, write time, computation time) of each type of txns(TPCC only).\n");
        printf("         6 : print file size only.\n");
//        printf("-r val : repeat same work val times.(default = 1)\n");
        printf("-u val : set whether get disk utilization info or not.(default = 0(false))\n"); 
        printf("-j val : set journal mode.\n");
        printf("         0 : OFF(default)\n");
        printf("         1 : WAL\n");
        printf("         2 : MEMORY\n");
        printf("         3 : PERSIST\n");
        printf("         4 : waldio\n");
        printf("         5 : SHADOW\n"); // TODO:insert LARGEST before task.
        printf("-R val : set record size to val byte. multiple of 8 is recommended.(default = 80)\n");
        printf("-P val : set page size to val * 1024(1k) byte.(default = 4)\n"); // TODO: in sqlite file, use this value for page size, not SQLITE_DEFAULT_PAGE_SIZE.
        printf("-t val : set operation type.\n");
        printf("         0 : insert(default)\n");
        printf("         1 : delete\n"); 
        printf("         2 : update\n");
        printf("         3 : Execute given sql queries\n");
        printf("         2 : select *\n");
        printf("-e str : give name of file containing queries.(default = \"execution_queries\")\n");
        printf("-z val : set close period ( 1 per val )\n");
        printf("-c val : set wal pre-allocation size ( default: 100 pages )\n");
        printf("-d str : give name of db file.(default = \"j.db\")\n");
        printf("-w val : N : warm-up N number of records.(default = (n))\n");
        printf("         0 : skip warm-up(only possible with insert mode)\n"); 
        printf("-S val : setting select and correctness check\n");
        printf("         0 : turn off select option(default)\n");
        printf("         1 : turn on select option\n");
        printf("         2 : turn on select option with re-open db file.(j.db only)\n");
        printf("-x val : setting whether removing j.db file or not after execution\n");
        printf("         0 : remove j.db file(default)\n");
        printf("         1 : do not remove j.db file\n");
        printf("-T val : setting number of threads\n");
        printf("         0 : single threaded(default)\n");
        printf("         n : multi threaded\n");
        printf("-b val : setting berrier(Distinguish Warmup and execution)\n");
        printf("         0 : no berrier\n");
        printf("         n : use barrier(wait until get any input and wait 1 sec and resume after warmup)\n");
        printf("             the number means TxnID which will be barriered.\n");
        printf("-h val : help\n");
        return 0;
    }
  }
  if(warm_ == 0 && op_type > 0){
    warm_ = nData * 21 / 20;
  }
  //Page size computing
  page_size *= 1024;

  //seed setting.
  struct timespec seedt;
  clock_gettime(CLOCK_MONOTONIC, &seedt);
  //srand(seedt.tv_nsec);
  srand(0);
  //input setting : random value
  
  if(warmed != -1)
    warmed = warmed / numOp;
  char str;
  input = (long long *)malloc(sizeof(long long)*(nData + warm_ + 1));
  output = (long long *)malloc(sizeof(long long)*(nData + warm_ + 1));
  int *hash = malloc(sizeof(int)*INPUTRANGE);
  long long j;
  int i;
  for( i = 0; i < INPUTRANGE; i++){
    hash[i] = 0;
  }
  long long key;
  for(j = 0; j<nData + warm_; j++){
    while(1){
      key = rand()%INPUTRANGE;
      if(hash[key] == 1) continue;
      hash[key] = 1;
      input[j] = key;
      output[j] = key;
      break;
    }
  }
  //For correction check.
  long long * for_free;
  int total = 0;
  if(op_type == 0){
	  total = warm_ + nData;
    qsort(output,total,sizeof(long long),compare);
    for_free = output;
  } else if(op_type == 1){
	  total = warm_ - nData;
    qsort(&output[nData],warm_ - nData,sizeof(long long),compare);
    for_free = output;
    output = &output[nData];
  } else if(op_type == 2){
	  total = warm_;
    qsort(output,warm_,sizeof(long long),compare);
    for_free = output;
  }
  output[total++] = INPUTRANGE;
  int* iterarr = (int*)malloc(sizeof(int)*nData);
  int x;
  for(x = 0; x < nData; x++){
    iterarr[x] = x;
  }
  for(x = 0; x < nData; x++){
    int rv = rand() % nData;
    if(x != rv){
      int tmp = iterarr[x];
      iterarr[x] = iterarr[rv];
      iterarr[rv] = tmp;
    }
  }
  //Setting record regarding record size.
  record = (char *)malloc(record_size+1);
  memset(record,'a',record_size);
  memset(record+record_size,'\0',1);
//  printf("record : %s\n", record);

  //Open db file. store_type
  if(store_type == 0){
    rc = sqlite3_open(db_name, &db);

  }
  rc = 0;
  if(rc){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }
  //setting journal mode
  if(journal_mode == 0){
    rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF; ", NULL, NULL, &zErr);
  }
  else if(journal_mode == 1){
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL; ", NULL, NULL, &zErr);
//    rc = sqlite3_exec(db, "PRAGMA wal_autocheckpoint=1000;", NULL, NULL, &zErr);
//    rc = sqlite3_exec(db, "PRAGMA PRAGMA schema.journal_size_limit = 4194304;", NULL, NULL, &zErr);
//    rc = sqlite3_exec(db, "PRAGMA wal_autocheckpoint=100;", NULL, NULL, &zErr);
//    rc = sqlite3_exec(db, "PRAGMA PRAGMA schema.journal_size_limit = 409600;", NULL, NULL, &zErr);
  }
  else if(journal_mode == 2){
    rc = sqlite3_exec(db, "PRAGMA journal_mode=MEMORY; ", NULL, NULL, &zErr);
  }
  else if(journal_mode == 3){
    rc = sqlite3_exec(db, "PRAGMA journal_mode=PERSIST; ", NULL, NULL, &zErr);
  }
  else if(journal_mode == 4){
    //if(op_type == 0){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=waldio; ", NULL, NULL, &zErr);
    //}
    //else {
    //  rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF ", NULL, NULL, &zErr);
    //}
  }
  else if(journal_mode == 5){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=shadow ", NULL, NULL, &zErr);
  }
  else if(rc != SQLITE_OK){
    if(zErr != NULL){
      fprintf(stderr, "SQL err: %s\n", zErr);
      sqlite3_free(zErr);
    }
  }
  else{
    printf("ERROR(journal_mode) : Please read help carefully.\n");
    return 0;
  }

  //setting page size.
  sql = "PRAGMA page_size=%d;";
  sprintf(buf, sql, page_size);
  rc=sqlite3_exec(db,buf, NULL, NULL, &zErr);
  if(rc != SQLITE_OK){
  	if(zErr != NULL){
  		fprintf(stderr, "SQL err: %s\n", zErr);
  		sqlite3_free(zErr);
  	}
  }
  //Thread

  if(nThread>0){
    sql = "PRAGMA threads=%d;";
    sprintf(buf, sql, nThread);
    rc=sqlite3_exec(db,buf, NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s\n", zErr);
        sqlite3_free(zErr);
      }
    }
  }
  
  //2018/07/07 tpcc is appended.
  //This just executes generated queries.
  if(op_type == 3){
    FILE *fp;
    //char buff[1024];
    char* buff;
    size_t size = 0;
    size_t nread;
      //TODO: 30000 can be limitation if the number is over 30000.
      char **queries = (char**)malloc(sizeof(char*)*100000); // # of lines in execution_queries.. 
      long long int *q_times = (long long int*) malloc(sizeof(long long int) * 30000);
      struct timespec oh1, oh2;
      long long oh_time = 0;
    //EXECUTING
    write_cnt = 0;
    fp = fopen(execute_name, "r");
    int current_oper = 0;
    struct timespec start, end;
    
    total_write_time = 0;
    total_fsync_time = 0;
    write_cnt = 0;
    fsync_cnt = 0;
    int commit_cnt = 0;
    size =0;
    int num_queries=0;
    char* buf = (char*) malloc(sizeof(char) * 250000);
    while((nread = getline(&buf,&size,fp)) != -1){
	    //if(strstr(buf,"SELECT") != NULL) continue;
	    if(nread > 5000 && strstr(buf, "CREATE") == NULL) continue;
	    if(nread > 3 \
			    //&& ((num_queries == 0) ? 1 : strncmp(buf,queries[num_queries-1],nread)) != 0
			    ){
	queries[num_queries] = (char *) malloc(sizeof(char) * (nread + 1));
	strncpy(queries[num_queries],buf,nread);
	queries[num_queries][nread] = '\0';
	num_queries++;
	    }
    }
    //printf("num_queries: %d\n", num_queries);
    fclose(fp);
      size =0;
      int isbegin = 0;
      int txn_cnt = 0;
      int closed = 0;
      int willbeclosed = 0;
      int closecnt = 0;
      int query_cnt = 0;
    clock_gettime(CLOCK_MONOTONIC, &oh1);
    for(i=0; i<num_queries; i++){
//     printf("%s",queries[i]);
//      fflush(stdout);
      //continue;
      //EXECUTE QUERIES
      if(isbegin == 1){
	      //if it is commit, set end time and print time(end - start)
	      //otherwise, do not set time
	      if(i > 0){
		      if(i % period == (period / 2) ){
			      willbeclosed = 1;
		      }
	      }
      } else {
	      if(strstr(queries[i], "SELECT") != NULL){
		      continue;
	      }
	      if(i > 0){
		      if(i % period == (period / 2)){
			      willbeclosed = 1;
		      }
	      }
	      //Not start txn with BEGIN
	      //1, set start time
	      //2, check whether it is BEGIN or not
	      //2-1, if ture, do not set end time at the end of this query
	      if(strstr(queries[i], "BEGIN") != NULL){
		 isbegin = 1;
	      } 
	      clock_gettime(CLOCK_MONOTONIC, &start);
      }
      if(closed){
	      rc = sqlite3_open(db_name, &db);
	      if(journal_mode == 0 ){
		      rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF ", NULL, NULL, &zErr);
	      }
	      else if(journal_mode == 1){
		      rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL ", NULL, NULL, &zErr);
		      //      rc = sqlite3_exec(db, "PRAGMA wal_autocheckpoint=1000;", NULL, NULL, &zErr);
		      //      rc = sqlite3_exec(db, "PRAGMA PRAGMA schema.journal_size_limit = 4194304;", NULL, NULL, &zErr);
		      //      rc = sqlite3_exec(db, "PRAGMA wal_autocheckpoint=100;", NULL, NULL, &zErr);
		      //      rc = sqlite3_exec(db, "PRAGMA PRAGMA schema.journal_size_limit = 409600;", NULL, NULL, &zErr);
	      }
	      else if(journal_mode == 2){
		      rc = sqlite3_exec(db, "PRAGMA journal_mode=MEMORY ", NULL, NULL, &zErr);
	      }
	      else if(journal_mode == 3){
		      rc = sqlite3_exec(db, "PRAGMA journal_mode=PERSIST ", NULL, NULL, &zErr);
	      }
	      else if(journal_mode == 4){
		      rc = sqlite3_exec(db, "PRAGMA journal_mode=waldio ", NULL, NULL, &zErr);
	      }
	      else if(journal_mode == 5){
		      rc = sqlite3_exec(db, "PRAGMA journal_mode=shadow ", NULL, NULL, &zErr);
	      }
	      else if(rc != SQLITE_OK){
		      if(zErr != NULL){
			      fprintf(stderr, "SQL err: %s\n", zErr);
			      sqlite3_free(zErr);
		      }
	      }
	      else{
		      printf("ERROR(journal_mode) : Please read help carefully.\n");
		      return 0;
	      }

	      //setting page size.
	      sql = "PRAGMA page_size=%d;";
	      sprintf(buf, sql, page_size);
	      rc=sqlite3_exec(db,buf, NULL, NULL, &zErr);
	      if(rc != SQLITE_OK){
		      if(zErr != NULL){
			      fprintf(stderr, "SQL err: %s\n", zErr);
			      sqlite3_free(zErr);
		      }
	      }

	      closed = 0;
      }
      rc=sqlite3_exec(db,queries[i], NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s\n", zErr);
          sqlite3_free(zErr);
        }
	printf("line: %d\n %s", i,queries[i]);
	exit(0);
      }
      query_cnt++;
      clock_gettime(CLOCK_MONOTONIC, &end);
      if(isbegin == 1){
	      //if it is commit, set end time and print time(end - start)
	      if(strstr(queries[i], "COMMIT") != NULL){
		      long long int localtime = 1000000000LL*(end.tv_sec - start.tv_sec);
		      localtime += end.tv_nsec - start.tv_nsec;
		      struct timespec ckst, cked;
		      if(willbeclosed){
			      closecnt++;
			      //printf("closecnt: %d\n",++closecnt);
		      clock_gettime(CLOCK_MONOTONIC, &ckst);
			      closed = 1;
			      sqlite3_close(db);
		      clock_gettime(CLOCK_MONOTONIC, &cked);
			      system("rm -f j.db-wal");
			      unlink("j.db-wal");
		      	localtime += 1000000000LL * (cked.tv_sec - ckst.tv_sec);
			localtime += cked.tv_nsec - ckst.tv_nsec;
			willbeclosed = 0;
		      }
		      isbegin = 0;
		      q_times[txn_cnt++] = localtime;
		      //printf("%lld 1\n",localtime);
//		      printf("%lld 1 %s",localtime, queries[i]);
//		      fflush(stdout);
		      //txn_cnt++;
	      }
	      //otherwise, do not set time
      } else {
	      long long int localtime = 1000000000LL*(end.tv_sec - start.tv_sec);
	      localtime += end.tv_nsec - start.tv_nsec;
		      struct timespec ckst, cked;
	      if(willbeclosed){
			      closecnt++;
			      //printf("closecnt: %d\n",++closecnt);
		      clock_gettime(CLOCK_MONOTONIC, &ckst);
		      closed = 1;
		      sqlite3_close(db);
		      clock_gettime(CLOCK_MONOTONIC, &cked);
		      system("rm -f j.db-wal");
		      unlink("j.db-wal");
		      	localtime += 1000000000LL * (cked.tv_sec - ckst.tv_sec);
			localtime += cked.tv_nsec - ckst.tv_nsec;
			willbeclosed = 0;
	      }
	      //Not start txn with BEGIN
	      //1, set start time
	      //2, check whether it is BEGIN or not
	      //2-2, otherwise, set end time and print time(end - start)
	      q_times[txn_cnt++] = localtime;
	      //printf("%lld 1\n",localtime);
//	      printf("%lld 1 %s",localtime, queries[i]);
//	      fflush(stdout);
	      //txn_cnt++;
      }
    }
    //TIME CHECK
    clock_gettime(CLOCK_MONOTONIC, &oh2);
    int osh = 0;
    for(; osh < txn_cnt; osh++){
	printf("%lld 1\n",q_times[osh]);
    }
    oh_time += 1000000000LL*(oh2.tv_sec - oh1.tv_sec);
    oh_time += (oh2.tv_nsec - oh1.tv_nsec);
    printf("%lld %d %d (cnt)%d (close)%d (query)%d\n",oh_time, write_cnt, fsync_cnt, txn_cnt, closecnt, query_cnt);
    system("rm -f j.db*");
    unlink("j.db*");
    return 0;
  }
  //creating new table 'episodes'
  sql = "create table episodes(id integer primary key, name text)";
  rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
  if(rc != SQLITE_OK){
  	if(zErr != NULL){
  		fprintf(stderr, "SQL err: %s\n", zErr);
  		sqlite3_free(zErr);
  	}
  }

  sql = "insert into episodes values ('%lld', '%s')";
  //if(journal_mode == 5){
      //sprintf(buf, sql, INPUTRANGE,record);
      //input[nData] = INPUTRANGE;
      //rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
  //}
long long insert_time;
long long insert_write_time ;
long long insert_fsync_time ;
int insert_write_cnt ;
int insert_fsync_cnt ;
long long insert_cal_time ;
long long insert_wpt ;
long long insert_fpt ;
long long insert_cpt ;

long long delete_time;
long long delete_write_time ;
long long delete_fsync_time ;
int delete_write_cnt ;
int delete_fsync_cnt ;
long long delete_cal_time ;
long long delete_wpt ;
long long delete_fpt ;
long long delete_cpt ;

long long update_time;
long long update_write_time ;
long long update_fsync_time ;
int update_write_cnt ;
int update_fsync_cnt ;
long long update_cal_time ;
long long update_wpt ;
long long update_fpt ;
long long update_cpt ;

long long select_time;
long long select_write_time ;
long long select_fsync_time ;
int select_write_cnt ;
int select_fsync_cnt ;
long long select_cal_time ;
long long select_wpt ;
long long select_fpt ;
long long select_cpt ;
long long close_time;
  if(nThread == 0){
  //Run insertion
  int num_txn = nData / numOp;
  int remainder = nData - (num_txn * numOp);
  int k;
  int iter = 0;
  struct timespec time_st, time_ed;
    rc = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }
    sprintf(buf, sql, INPUTRANGE,record);
    rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }
    iter++;
    rc = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        //printf("key : %ld j: %d\n",key, j);
        sqlite3_free(zErr);
      }
    }
    //Warm-up
    iter = 0;
    for(j = 0; j < warm_; j++){
      rc = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }


      sprintf(buf, sql, input[iter],record);
      rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }
      iter++;
      rc = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          //printf("key : %ld j: %d\n",key, j);
          sqlite3_free(zErr);
        }
      }
    }
    


  if(warm_ > 0 && op_type < 3){
  if(journal_mode != 7){
    sqlite3_close(db);
    system("rm -f j.db-wal");
    unlink("j.db-wal");
    system("rm -f j.db-shm");
    unlink("j.db-shm");
    system("rm -f j.db-journal");
    unlink("j.db-journal");
    if(store_type == 0){
      rc = sqlite3_open(db_name, &db);
    }
    else if( store_type == 3 || store_type == 2){
      if(Env == 0){ // Nexus 5
        rc = sqlite3_open("/dev/block/mmcblk0p30", &db);
      }
      else if(Env == 1){ // r930 optane
        rc = sqlite3_open("/dev/nvme0n1p3", &db);
      }
      else {
        printf("ERROR(Environment) : Please read help carefully.\n");
        return 0;
      }
    }
    else if(store_type == 1){
      rc = sqlite3_open(":memory:", &db);
    }
    else{
      printf("ERROR(store_type) : Please read help carefully.\n");
      return 0;
    }
    if(rc){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 0;
    }
    //setting journal mode
    if(journal_mode == 0 ){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF ", NULL, NULL, &zErr);
    }
    else if(journal_mode == 1){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL ", NULL, NULL, &zErr);
//      rc = sqlite3_exec(db, "PRAGMA wal_autocheckpoint=1000;", NULL, NULL, &zErr);
//      rc = sqlite3_exec(db, "PRAGMA PRAGMA schema.journal_size_limit = 4194304;", NULL, NULL, &zErr);
//      rc = sqlite3_exec(db, "PRAGMA wal_autocheckpoint=100;", NULL, NULL, &zErr);
//      rc = sqlite3_exec(db, "PRAGMA PRAGMA schema.journal_size_limit = 409600;", NULL, NULL, &zErr);
    }
    else if(journal_mode == 2){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=MEMORY ", NULL, NULL, &zErr);
    }
    else if(journal_mode == 3){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=PERSIST ", NULL, NULL, &zErr);
    }
    else if(journal_mode == 4){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=waldio ", NULL, NULL, &zErr);
    }
  else if(journal_mode == 5){
      rc = sqlite3_exec(db, "PRAGMA journal_mode=shadow ", NULL, NULL, &zErr);
  }
    else if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s\n", zErr);
        sqlite3_free(zErr);
      }
    }
    else{
      printf("ERROR(journal_mode) : Please read help carefully.\n");
      return 0;
    }

    //setting page size.
    sql = "PRAGMA page_size=%d;";
    sprintf(buf, sql, page_size);
    rc=sqlite3_exec(db,buf, NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s\n", zErr);
        sqlite3_free(zErr);
      }
    }
  }
  }
    total_write_time = 0;
    total_fsync_time = 0;
    write_cnt = 0;
    fsync_cnt = 0;
    walsize = 0;

  if(op_type == 0){
  iter = warm_;
#ifdef SEVERAL
  nTxn = num_txn;
  txn_latencies = (long long *)malloc(sizeof(long long ) * num_txn);
#endif
  sql = "insert into episodes values ('%lld', '%s')";
//  sync();
//  system("echo 3 > /proc/sys/vm/drop_caches");
  //system("sh my_setup.sh");
  //printf("insertion BEGIN\n");
  clock_gettime(CLOCK_MONOTONIC, &time_st);
  for(j = 0; j < num_txn+1; j++){
    if(j==warmed && warmed != -1){
      if(journal_mode == 1 || journal_mode == 4){
        sqlite3_wal_checkpoint(db, NULL);
        truncate("j.db-wal",0);
        system("rm j.db-wal");
        system("rm j.db-shm");
        system("rm j.db-journal");
	sync();

	walsize = 0;
      }
      output_type = -1;
      printf("MEET BARRIER.\n");
      printf("warmed : %d\n", warmed);
      printf("iter : %d\n", iter);
      printf("~%d txn is done\n", (int)j-1);
      printf("%d remains.\n", ((remainder==0)?num_txn : num_txn+1) - (int)j);
      str = getchar();
      sleep(1);
    }
#ifdef SEVERAL
  clock_gettime(CLOCK_MONOTONIC, &se1);
#endif
    rc = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }

    for(k = 0; (j < num_txn && k < numOp) || (j == (num_txn) && k < remainder); k++){

      sprintf(buf, sql, input[iter],record);
      //printf("%lld\n", input[iter]);
      rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }
      iter++;
    }
    rc = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        //printf("key : %ld j: %d\n",key, j);
        sqlite3_free(zErr);
      }
    }
    //printf("commit-----------------------------------------------\n");
#ifdef SEVERAL
  clock_gettime(CLOCK_MONOTONIC, &se2);
  if(j < num_txn){
  txn_latencies[j] = 1000000000LLU*(se2.tv_sec - se1.tv_sec);
  txn_latencies[j] += (se2.tv_nsec - se1.tv_nsec);
  //printf("time: %lld\n", txn_latencies[j]);
  }
#endif
  }
    if(journal_mode == 1){
      sqlite3_wal_checkpoint(db, NULL);
    }
  clock_gettime(CLOCK_MONOTONIC, &time_ed);
  //printf("insertion END\n");
}
    
  //Time computing
  insert_time = 1000000000LLU*(time_ed.tv_sec - time_st.tv_sec);
  insert_time += (time_ed.tv_nsec - time_st.tv_nsec);
  insert_write_time = total_write_time;
  insert_fsync_time = total_fsync_time;
  insert_write_cnt = write_cnt;
  insert_fsync_cnt = fsync_cnt;
  insert_cal_time = insert_time - total_write_time - total_fsync_time;
  insert_wpt = insert_write_time * numOp / nData;
  insert_fpt = insert_fsync_time * numOp / nData;
  insert_cpt = insert_cal_time   * numOp / nData;
  total_write_time = 0;
  total_fsync_time = 0;
  write_cnt = 0;
  fsync_cnt = 0;

  num_txn = nData / numOp;
  remainder = nData - (num_txn * numOp);
  //run delection
  if(op_type == 1){
    k;
    iter = 0;
#ifdef SEVERAL
  nTxn = num_txn;
  txn_latencies = (long long *)malloc(sizeof(long long ) * num_txn);
#endif
    sql = "DELETE FROM episodes WHERE id = %lld;";
    //printf("deletion BEGIN\n");
//      sync();
//      system("echo 3 > /proc/sys/vm/drop_caches");
      //system("sh my_setup.sh");
    clock_gettime(CLOCK_MONOTONIC, &time_st);
    for(j = 0; j < num_txn+1; j++){
    if(j==warmed && warmed != -1){
      if(journal_mode == 1 || journal_mode == 4){
        sqlite3_wal_checkpoint(db, NULL);
        system("rm j.db-wal");
        system("rm j.db-shm");
        system("rm j.db-journal");
      }
//      sync();
//      system("echo 3 > /proc/sys/vm/drop_caches");
      output_type = -1;
      printf("MEET BARRIER.\n");
      printf("warmed : %d\n", warmed);
      printf("iter : %d\n", iter);
      printf("~%d txn is done\n", (int)j-1);
      printf("%d remains.\n", ((remainder==0)?num_txn : num_txn+1) - (int)j);
      str = getchar();
      sleep(1);
    }
#ifdef SEVERAL
  clock_gettime(CLOCK_MONOTONIC, &se1);
#endif
      rc = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }

      for(k = 0; (j < num_txn && k < numOp) || (j == num_txn && k < remainder); k++){

//	printf("%d / %lld\n", iter, input[iter]);
        sprintf(buf, sql, input[iter]);
//	printf("%s\n", buf);
        rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
        if(rc != SQLITE_OK){
          if(zErr != NULL){
            fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
            sqlite3_free(zErr);
	    return 0;
          }
        }
        iter++;
      }

      rc = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          //printf("key : %ld j: %d\n",key, j);
          sqlite3_free(zErr);
        }
      }
#ifdef SEVERAL
  clock_gettime(CLOCK_MONOTONIC, &se2);
  if(j < num_txn){
  txn_latencies[j] = 1000000000LLU*(se2.tv_sec - se1.tv_sec);
  txn_latencies[j] += (se2.tv_nsec - se1.tv_nsec);
  //printf("time: %lld\n", txn_latencies[j]);
  }
#endif
    }
    if(journal_mode == 1){
      sqlite3_wal_checkpoint(db, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &time_ed);
    //printf("iter : %d\n",iter);
  //   printf("deletion END\n");
    //    if(correctness == 0){ // Correct result
    //      printf("record insertion completes without error.\n");
    //    } else {
    //      printf("there are some incorrect point! please check the result.\n");
    //    }
  }
  //Time computing
  delete_time = 1000000000LLU*(time_ed.tv_sec - time_st.tv_sec);
  delete_time += (time_ed.tv_nsec - time_st.tv_nsec);
  delete_write_time = total_write_time;
  delete_fsync_time = total_fsync_time;
  delete_write_cnt = write_cnt;
  delete_fsync_cnt = fsync_cnt;
  delete_cal_time = delete_time - total_write_time - total_fsync_time;
  delete_wpt = delete_write_time * numOp / nData;
  delete_fpt = delete_fsync_time * numOp / nData;
  delete_cpt = delete_cal_time   * numOp / nData;
  total_write_time = 0;
  total_fsync_time = 0;
  write_cnt = 0;
  fsync_cnt = 0;

  //run update
  if(op_type == 2){
    update_record = (char *)malloc(record_size+1);
    memset(update_record,'b',record_size);
    memset(update_record+record_size,'\0',1);
    //    printf("update_record : %s\n", update_record);
    sql = "UPDATE episodes SET name = '%s' WHERE id = %lld;";
    iter = 0;
#ifdef SEVERAL
    nTxn = num_txn;
    txn_latencies = (long long*)malloc(sizeof(long long ) * num_txn);
#endif
    //printf("update BEGIN\n");
//      sync();
//      system("echo 3 > /proc/sys/vm/drop_caches");
      //system("sh my_setup.sh");
    clock_gettime(CLOCK_MONOTONIC, &time_st);
    for(j = 0; j < num_txn+1; j++){
    if(j==warmed && warmed != -1){
      if(journal_mode == 1 || journal_mode == 4){
        sqlite3_wal_checkpoint(db, NULL);
        system("rm j.db-wal");
        system("rm j.db-shm");
        system("rm j.db-journal");
      }
//      sync();
//      system("echo 3 > /proc/sys/vm/drop_caches");
      output_type = -1;
      printf("MEET BARRIER.\n");
      printf("warmed : %d\n", warmed);
      printf("iter : %d\n", iter);
      printf("~%d txn is done\n", (int)j-1);
      printf("%d remains.\n", ((remainder==0)?num_txn : num_txn+1) - (int)j);
      str = getchar();
      sleep(1);
    }
#ifdef SEVERAL
      clock_gettime(CLOCK_MONOTONIC, &se1);
#endif
      rc = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }

      for(k = 0; (j < num_txn && k < numOp) || (j == num_txn && k < remainder); k++){
        sprintf(buf, sql, update_record,input[iterarr[iter]]);
        rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
        if(rc != SQLITE_OK){
          if(zErr != NULL){
            fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
            sqlite3_free(zErr);
          }
        }
        iter++;
      }

      rc = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          //printf("key : %ld j: %d\n",key, j);
          sqlite3_free(zErr);
        }
      }
#ifdef SEVERAL
      if(j == num_txn - 1 && journal_mode == 1){
        sqlite3_wal_checkpoint(db, NULL);
      }
      clock_gettime(CLOCK_MONOTONIC, &se2);
      if(j < num_txn){
        txn_latencies[j] = 1000000000LLU*(se2.tv_sec - se1.tv_sec);
        txn_latencies[j] += (se2.tv_nsec - se1.tv_nsec);
      }
#endif
    }
    if(journal_mode == 1){
      sqlite3_wal_checkpoint(db, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &time_ed);
    //printf("update END\n");
    //    if(correctness == 0){ // Correct result
    //      printf("record insertion completes without error.\n");
    //    } else {
    //      printf("there are some incorrect point! please check the result.\n");
    //    }
    free(update_record);
  }
  //Time computing
  update_time = 1000000000LLU*(time_ed.tv_sec - time_st.tv_sec);
  update_time += (time_ed.tv_nsec - time_st.tv_nsec);
  update_write_time = total_write_time;
  update_fsync_time = total_fsync_time;
  update_write_cnt = write_cnt;
  update_fsync_cnt = fsync_cnt;
  update_cal_time = update_time - total_write_time - total_fsync_time;
  update_wpt = update_write_time * numOp / nData;
  update_fpt = update_fsync_time * numOp / nData;
  update_cpt = update_cal_time   * numOp / nData;
  total_write_time = 0;
  total_fsync_time = 0;
  write_cnt = 0;
  fsync_cnt = 0;

  if(op_type == 4){
  iter = 0;
  sql = "SELECT * FROM episodes;";
  clock_gettime(CLOCK_MONOTONIC, &time_st);
  for(j = 0; j < num_txn+1; j++){
    rc = sqlite3_exec(db, "BEGIN", NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }

    for(k = 0; (j < num_txn && k < numOp) || (j == (num_txn) && k < remainder); k++){

      sprintf(buf, sql);
      //printf("%lld\n", input[iter]);
      rc = sqlite3_exec(db, buf, NULL, NULL, &zErr);
      if(rc != SQLITE_OK){
        if(zErr != NULL){
          fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
          sqlite3_free(zErr);
        }
      }
      iter++;
    }
    rc = sqlite3_exec(db, "COMMIT", NULL, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        //printf("key : %ld j: %d\n",key, j);
        sqlite3_free(zErr);
      }
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &time_ed);
}
    
  //Time computing
  select_time = 1000000000LLU*(time_ed.tv_sec - time_st.tv_sec);
  select_time += (time_ed.tv_nsec - time_st.tv_nsec);
  select_write_time = total_write_time;
  select_fsync_time = total_fsync_time;
  select_write_cnt = write_cnt;
  select_fsync_cnt = fsync_cnt;
  select_cal_time = select_time - total_write_time - total_fsync_time;
  select_wpt = select_write_time * numOp / nData;
  select_fpt = select_fsync_time * numOp / nData;
  select_cpt = select_cal_time   * numOp / nData;
  total_write_time = 0;
  total_fsync_time = 0;
  write_cnt = 0;
  fsync_cnt = 0;
  //run selection
  if(op_type < 3){
  if(select_opt >= 1){
    if(select_opt == 2 && (store_type == 0 || store_type == 2)){
      sqlite3_close(db);
      sqlite3_open(db_name, &db);
    }
    printf("selection BEGIN\n");
    clock_gettime(CLOCK_MONOTONIC, &time_st);
//    rc = sqlite3_exec(db, "SELECT * FROM episodes;", callback, NULL, &zErr);
    rc = sqlite3_exec(db, "SELECT * FROM episodes;", callback, NULL, &zErr);
    if(rc != SQLITE_OK){
      if(zErr != NULL){
        fprintf(stderr, "SQL err: %s [%d]\n", zErr, __LINE__);
        sqlite3_free(zErr);
      }
    }
    clock_gettime(CLOCK_MONOTONIC, &time_ed);
    printf("selection END\n");
    if(warm_ + nData + 1 != callback_cnt && op_type == 0){
      printf("warm-up + nData %d : %d callback_cnt INSERTION\n", warm_ + nData + 1,callback_cnt);
      printf("ERROR : Some records are omitted.\n");
      correctness = 1;
    } else if(warm_ - nData + 1 != callback_cnt && op_type == 1){
      printf("warm-up - nData  %d : %d callback_cnt DELETION\n", warm_ - nData + 1,callback_cnt);
      printf("ERROR : Some records are omitted.\n");
      correctness = 1;
    } else if(warm_ + 1 != callback_cnt && op_type == 2){
      printf("warm-up  %d : %d callback_cnt UPDATE\n", warm_ + 1,callback_cnt);
      printf("ERROR : Some records are omitted.\n");
      correctness = 1;
    } else if(correctness == 0){ // Correct result
      printf("CORRECT : record insertion completes without error.\n");
    } else {
      printf("ERROR : there are some incorrect point! please check the result.\n");
    }
  }
  //Time computing
  select_time = 1000000000LLU*(time_ed.tv_sec - time_st.tv_sec);
  select_time += (time_ed.tv_nsec - time_st.tv_nsec);
  } else {
	  correctness = 0;
  }

  //close db
  clock_gettime(CLOCK_MONOTONIC, &time_st);
  sqlite3_close(db);
  clock_gettime(CLOCK_MONOTONIC, &time_ed);
  close_time = 1000000000LLU*(time_ed.tv_sec - time_st.tv_sec);
  close_time += (time_ed.tv_nsec - time_st.tv_nsec);
  
  }
#ifdef SEVERAL
  int a;
  for(a = 0; a < nTxn; a++){
  printf("%f %d\n",(float)txn_latencies[a]/1000000.0, 1);
}
#else
//Output format printing

if(output_type == 6){
  struct stat stat_;
  int status = stat(db_name, &stat_);
  if(status == 0){
    int my_size = stat_.st_size;
    printf("%d\n", my_size);
    printf("%f%\n", (20480.0/(float)my_size) * 100.0);
    goto end;
  }
}

if(output_type == 0){ // print all details

  //print setting details
  printf("Setting\n");
  printf("Number of records   : %d\n", nData);
  printf("Operations / Txn    : %d\n", numOp);
  printf("number of CPU cylce : %d\n", num_cycle);
  printf("journal mode        : %d\n", journal_mode);
  printf("                      0 : OFF\n");
  printf("                      1 : WAL\n");
  printf("                      2 : MEMORY\n");
  printf("                      3 : PERSIST\n");
  printf("                      4 : waldio\n");
  printf("                      5 : OFF(ls-mvbt)\n");
  printf("storage             : %d\n",store_type);
  printf("                      0 : store in j.db file.\n");
  printf("                      1 : store in memory.\n");
  printf("                      2 : store in j.db file with direct_IO. (only for DHL)\n");
  printf("                      3 : store in raw partition for ADIO. (only for DHL)\n");
  printf("envirnment          : %d\n", Env);
  printf("                      0 : Android, Nexus5 (default)\n");
  printf("                      1 : Linux, r930\n");
  printf("record size         : %d bytes\n", record_size);
  printf("page size           : %d bytes\n", page_size);
  printf("operation type      : %d\n", op_type);
  printf("                      0 : insert\n");
  printf("                      1 : delete\n"); // TODO  : callback test
  printf("                      2 : update\n"); // TODO  : callback test
  printf("thread number set   : %d\n", nThread);
  printf("                      0 : single threaded\n");
  printf("                      n : multi-threaded\n");
  printf("Correctness check   : %d\n", select_opt);
  printf("                      0 : no correctness check\n");
  printf("                      1 : select and correctness check\n"); // TODO  : callback test
  printf("                      2 : select and correctness check with re-open db file.(j.db only)\n");
  if(correctness == 1){ // uncorrect result is not needed.
    goto end;
  }
    //print insert details


  if(op_type == 0){
    printf("INSERTION\n");
    printf("execution     Time : %20lld ns\n" , insert_time);
    printf("db close      Time : %20lld ns\n\n" , close_time);
    printf("insert write  Time : %20lld ns\n"   , insert_write_time);
    printf("insert fsync  Time : %20lld ns\n\n" , insert_fsync_time);
    printf("insert I/O    Time : %20lld ns\n\n" , insert_fsync_time + insert_write_time);
    printf("insert write  cnt  : %d times\n"    , insert_write_cnt);
    printf("insert fsync  cnt  : %d times\n\n"  , insert_fsync_cnt);
    printf("write Time  / Txn  : %20lld ns\n"   , insert_wpt);
    printf("fsync Time  / Txn  : %20lld ns\n"   , insert_fpt);
    printf("comp. Time  / Txn  : %20lld ns\n\n" , insert_cpt);
  }
    if(op_type == 1){
      printf("DELETION\n");
      printf("execution     Time : %20lld ns\n\n" , delete_time);
      printf("delete write  Time : %20lld ns\n"   , delete_write_time);
      printf("delete fsync  Time : %20lld ns\n\n" , delete_fsync_time);
      printf("delete I/O    Time : %20lld ns\n\n" , delete_fsync_time + delete_write_time);
      printf("delete write  cnt  : %d times\n"    , delete_write_cnt);
      printf("delete fsync  cnt  : %d times\n\n"  , delete_fsync_cnt);
      printf("write Time  / Txn  : %20lld ns\n"   , delete_wpt);
      printf("fsync Time  / Txn  : %20lld ns\n"   , delete_fpt);
      printf("comp. Time  / Txn  : %20lld ns\n\n" , delete_cpt);
    }
    if(op_type == 2){
      printf("UPDATE\n");
      printf("execution     Time : %20lld ns\n\n" , update_time);
      printf("update write  Time : %20lld ns\n"   , update_write_time);
      printf("update fsync  Time : %20lld ns\n\n" , update_fsync_time);
      printf("update I/O    Time : %20lld ns\n\n" , update_fsync_time + update_write_time);
      printf("update write  cnt  : %d times\n"    , update_write_cnt);
      printf("update fsync  cnt  : %d times\n\n"  , update_fsync_cnt);
      printf("write Time  / Txn  : %20lld ns\n"   , update_wpt);
      printf("fsync Time  / Txn  : %20lld ns\n"   , update_fpt);
      printf("comp. Time  / Txn  : %20lld ns\n\n" , update_cpt);
    }
    if(op_type == 4){
      printf("SELECT\n");
      printf("execution     Time : %20lld ns\n\n" , select_time);
      printf("select write  Time : %20lld ns\n"   , select_write_time);
      printf("select fsync  Time : %20lld ns\n\n" , select_fsync_time);
      printf("select I/O    Time : %20lld ns\n\n" , select_fsync_time + select_write_time);
      printf("select write  cnt  : %d times\n"    , select_write_cnt);
      printf("select fsync  cnt  : %d times\n\n"  , select_fsync_cnt);
      printf("write Time  / Txn  : %20lld ns\n"   , select_wpt);
      printf("fsync Time  / Txn  : %20lld ns\n"   , select_fpt);
      printf("comp. Time  / Txn  : %20lld ns\n\n" , select_cpt);
    }
  }
  else if(output_type == 1){ // print total execution time only
    if(correctness == 1){ // uncorrect result is not needed.
      return 0;
    }
    if(op_type == 0){
      printf("%lld\n", insert_time);
    }
    else if(op_type == 1){
      printf("%lld\n", delete_time);
    }
    else if(op_type == 2){
      printf("%lld\n", update_time);
    }
    else if(op_type == 4){
      printf("%lld\n", select_time);
    }
  }
  else if(output_type == 2){ // print (fsync time, write time, compute time)
    if(correctness == 1){ // uncorrect result is not needed.
      return 0;
    }
    if(op_type == 0){
      printf("%lld %lld %lld \n", insert_fpt,insert_wpt,insert_cpt); 
    }
    if(op_type == 1){
      printf("%lld %lld %lld \n", delete_fpt,delete_wpt,delete_cpt); 
    }
    if(op_type == 2){
      printf("%lld %lld %lld \n", update_fpt,update_wpt,update_cpt); 
    }
  }
  else if(output_type == 3){ // print Txn / s
    if(correctness == 1){ // uncorrect result is not needed.
      return 0;
    }
    if(op_type == 0){
      printf("%lld\n", (1000000000LL * (long long)nData) / (long long)numOp /  (insert_time));
    }
    else if(op_type == 1){
      printf("%lld\n", (1000000000LL * (long long)nData) / (long long)numOp /  (delete_time));
    }
    else if(op_type == 2){
      printf("%lld\n", (1000000000LL * (long long)nData) / (long long)numOp /  (update_time));
    }
    else if(op_type == 4){
      printf("%lld\n", (1000000000LL * (long long)nData) / (long long)numOp /  (select_time));
    }
  }
#endif

  //freeing
  end:
//  free(input);
//  free(for_free);
//  free(hash);
//  free(record);
  if(store_type == 0 || store_type == 2){
    if(rm_opt == 0){
    system("rm j.db*");
    unlink("j.db*");
    }
  }
  return 0;
}
