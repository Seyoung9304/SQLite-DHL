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
//void *delete_query(void* tid);

int main(){
	
	pthread_t th1, th2, th3, th4, th5, th6, th7;	
	int tid1, tid2, tid3, tid4, tid5, tid6, tid7;

	char p1[] = "thread_1";   // 1번 쓰레드 이름
	char p2[] = "thread_2";   // 2번 쓰레드 이름
	char p3[] = "thread_3";   // 3번 쓰레드 이름
	char p4[] = "thread_4";   // 4번 쓰레드 이름
	char p5[] = "thread_5";   // 5번 쓰레드 이름
	char p6[] = "thread_6";   // 6번 쓰레드 이름
	char p7[] = "thread_7";   // 7번 쓰레드 이름

	int pr[7]={1, 2, 3, 4, 5, 6, 7};


	srand(time(NULL));

	rc = sqlite3_open("dbdb.db", &db);

	if(rc!=SQLITE_OK)
		printf("error\n");

	rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF ", NULL, NULL, NULL);

	if(rc!=SQLITE_OK)
		printf("error\n");

	char *create_query = "CREATE TABLE ORDER_LINE (OL_O_ID INT DEFAULT '0' NOT NULL, OL_D_ID TINYINT DEFAULT '0' NOT NULL,OL_W_ID SMALLINT DEFAULT '0' NOT NULL,OL_NUMBER INTEGER DEFAULT '0' NOT NULL,OL_I_ID INTEGER DEFAULT NULL,OL_SUPPLY_W_ID SMALLINT DEFAULT NULL,OL_QUANTITY INTEGER DEFAULT NULL,OL_AMOUNT FLOAT DEFAULT NULL,OL_DIST_INFO VARCHAR(32) DEFAULT NULL, PRIMARY KEY (OL_O_ID), CONSTRAINT OL_FKEY_O FOREIGN KEY (OL_O_ID, OL_D_ID, OL_W_ID) REFERENCES ORDERS (O_ID, O_D_ID, O_W_ID), CONSTRAINT OL_FKEY_S FOREIGN KEY (OL_I_ID, OL_SUPPLY_W_ID) REFERENCES STOCK (S_I_ID, S_W_ID));";


	char* drop_query = "DROP TABLE ORDER_LINE;";
	
	
	printf("create query\n");
	rc=sqlite3_exec(db, create_query, NULL, NULL, NULL);
	if(rc!=SQLITE_OK)
		printf("error\n");

	tid1 = pthread_create(&th1, NULL, insert_query, (void*) p1);
	tid2 = pthread_create(&th2, NULL, insert_query, (void*) p2);
	/*
	tid3 = pthread_create(&th3, NULL, insert_query, (void*) &pr[2]);
	tid4 = pthread_create(&th4, NULL, insert_query, (void*) &pr[3]);
	tid5 = pthread_create(&th5, NULL, insert_query, (void*) &pr[4]);
	tid6 = pthread_create(&th6, NULL, insert_query, (void*) &pr[5]);
	tid7 = pthread_create(&th7, NULL, insert_query, (void*) &pr[6]);
	*/

	if(tid1!=0 || tid2!=0) // && tid3>0 && tid4>0 && tid5>0 && tid6>0 && tid7>0))
		printf("Thread Create Failure\n");
	/*
	int i;
	char strbuf[300];
	memset(strbuf, 0, 300);

	for(i=0; i<100; i++){
		sprintf(strbuf, insert_query, i, i, i, i);

		rc=sqlite3_exec(db, strbuf, NULL, NULL, NULL);
		if(rc!=SQLITE_OK)
			printf("error\n");
	}
	*/
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	/*
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	pthread_join(th5, NULL);
	pthread_join(th6, NULL);
	pthread_join(th7, NULL);
	*/
		
	printf("close\n");
	sqlite3_close(db);
}

void *insert_query(void *pr){
        char strbuf[300];
        memset(strbuf, 0, 300);
	char* pname = (char*) pr;
	int pkey;
        pkey = atoi(&(pname[7]));
	//printf("pkey: %d", *pkey);
	char *insert_query = "INSERT INTO ORDER_LINE (OL_O_ID, OL_D_ID, OL_W_ID, OL_NUMBER, OL_I_ID, OL_SUPPLY_W_ID, OL_QUANTITY, OL_AMOUNT, OL_DIST_INFO) VALUES (%d, %d, %d, %d, 481, 1, 2, 17.16, 'lqetldzgsfktkcbcpxldrwdd');";

        for(int i=0; i<100; i++){
		sprintf(strbuf, insert_query, pkey*100 + i, i, i, i);
        	rc=sqlite3_exec(db, strbuf, NULL, NULL, &errMsg);
		printf("[%s] - insert %d\n", pname, i);
		if(rc!=SQLITE_OK)
                	printf("error- %s\n", errMsg);
        }
}
