#include <stdio.h>
#include <string.h>
#include "sqlite3_dhl.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

long long *input=NULL;
long long *output=NULL;
//int *output;
//int cnt=0;
//int prealloc = 100;
//int walsize = 0;
//int period=100;
int util = 0;
//int num_cycle=0;
//long long total_write_time=0;
//long long total_fsync_time=0;
//int write_cnt=0;
//int fsync_cnt=0;
//long long avg_write_time = 0;
//long long avg_fsync_time = 0;
//int MAX_SH_SIZE = 0;
//int correctness = 0;
//int store_type = 0;
//int Env = 0;
//int op_type = 0;
//int mycur = 1;
//int warmed = -1;
//  int warm_ = 0;
int store_type=0;
int page_size=4096;
int fsync_cnt=0;
int write_cnt=0;
int num_cycle=0;
long total_write_time=0;
long total_fsync_time=0;
int nData=1000;
int record_size=80;
sqlite3 *db;
int rc;
char *errMsg = NULL;
void *insert_query(void* tid);
int errFlag=0;
//void error_check(int rc, char* errMsg);



int main(int argc, char* argv[]){
	
	int i;
	pthread_t th[32];
	//1, th2, th3, th4, th5, th6, th7;	
	int tid[32];
	//1, tid2, tid3, tid4, tid5, tid6, tid7;
	
	if(argc<2) {
		printf("Usage: %s num_threads\n", argv[0]);
		exit(1);
	}
	int nthreads = atoi(argv[1]);

	char p[32][16]; // = "thread_1";   // 1번 쓰레드 이름

	for(i=0;i<nthreads;i++){
		sprintf(p[i], "thread_%d\0", i);
	}	

	srand(time(NULL));

	rc = sqlite3_open("dbdb.db", &db);

	if(rc!=SQLITE_OK)
		printf("error\n");

	rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF ", NULL, NULL, NULL);

	if(rc!=SQLITE_OK)
		printf("error\n");

	char *create_query = "CREATE TABLE ORDER_LINE (OL_O_ID INT DEFAULT '0' NOT NULL, OL_D_ID TINYINT DEFAULT '0' NOT NULL,OL_W_ID SMALLINT DEFAULT '0' NOT NULL,OL_NUMBER INTEGER DEFAULT '0' NOT NULL,OL_I_ID INTEGER DEFAULT NULL,OL_SUPPLY_W_ID SMALLINT DEFAULT NULL,OL_QUANTITY INTEGER DEFAULT NULL,OL_AMOUNT FLOAT DEFAULT NULL,OL_DIST_INFO VARCHAR(32) DEFAULT NULL, PRIMARY KEY (OL_O_ID), CONSTRAINT OL_FKEY_O FOREIGN KEY (OL_O_ID, OL_D_ID, OL_W_ID) REFERENCES ORDERS (O_ID, O_D_ID, O_W_ID), CONSTRAINT OL_FKEY_S FOREIGN KEY (OL_I_ID, OL_SUPPLY_W_ID) REFERENCES STOCK (S_I_ID, S_W_ID));";

	//char* drop_query = "DROP TABLE ORDER_LINE;";
	
	printf("create query\n");
	rc=sqlite3_exec(db, create_query, NULL, NULL, &errMsg);
	if(rc!=SQLITE_OK)
		printf("error - %s\n", errMsg);
	
	for(i=0;i<nthreads;i++){
	    tid[i] = pthread_create(&th[i], NULL, insert_query, (void*) p[i]);
	    if(tid[i]!=0)
		printf("Thread %d Create Failure\n", i);
	}


	for(i=0;i<nthreads;i++){
	    pthread_join(th[i], NULL);
	}
		
	
	printf("Close\n");
	sqlite3_close(db);
	
	if(errFlag)
		printf("Error has occured at least once\n");
}

void *insert_query(void *pr){
        char strbuf[300];
        memset(strbuf, 0, 300);
	char* pname = (char*) pr;
	int pkey;
        pkey = atoi(&(pname[7]));
	char *insert_query = "INSERT INTO ORDER_LINE (OL_O_ID, OL_D_ID, OL_W_ID, OL_NUMBER, OL_I_ID, OL_SUPPLY_W_ID, OL_QUANTITY, OL_AMOUNT, OL_DIST_INFO) VALUES (%d, %d, %d, %d, 481, 1, 2, 17.16, 'lqetldzgsfktkcbcpxldrwdd');";

        for(int i=0; i<100; i++){
		sprintf(strbuf, insert_query, pkey*100 + i, i, i, i);
        	rc=sqlite3_exec(db, strbuf, NULL, NULL, &errMsg);
		printf("[%s] - insert %d\n", pname, i);
		if(rc!=SQLITE_OK){
                	printf("error- %s\n", errMsg);
			errFlag=1;
		}
	}
}

/*
void error_check(int rc, char* errMsg){

	if(rc!=SQLITE_OK){
		printf("ERROR - %s\n", errMsg);
		errFlag=1;
	}

}*/
