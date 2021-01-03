#include <stdio.h>
#include <string.h>
#include "sqlite3_dhl.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

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



int main(){
	sqlite3 *db;
	int rc = sqlite3_open("dbdb.db", &db);

	if(rc!=SQLITE_OK)
		printf("error\n");

	rc = sqlite3_exec(db, "PRAGMA journal_mode=OFF ", NULL, NULL, NULL);

	if(rc!=SQLITE_OK)
		printf("error\n");

	char *create_query = "CREATE TABLE ORDER_LINE (OL_O_ID INTEGER DEFAULT '0' NOT NULL, OL_D_ID TINYINT DEFAULT '0' NOT NULL,OL_W_ID SMALLINT DEFAULT '0' NOT NULL,OL_NUMBER INTEGER DEFAULT '0' NOT NULL,OL_I_ID INTEGER DEFAULT NULL,OL_SUPPLY_W_ID SMALLINT DEFAULT NULL,OL_QUANTITY INTEGER DEFAULT NULL,OL_AMOUNT FLOAT DEFAULT NULL,OL_DIST_INFO VARCHAR(32) DEFAULT NULL, PRIMARY KEY (OL_O_ID), CONSTRAINT OL_FKEY_O FOREIGN KEY (OL_O_ID, OL_D_ID, OL_W_ID) REFERENCES ORDERS (O_ID, O_D_ID, O_W_ID), CONSTRAINT OL_FKEY_S FOREIGN KEY (OL_I_ID, OL_SUPPLY_W_ID) REFERENCES STOCK (S_I_ID, S_W_ID));";

	char *insert_query = "INSERT INTO ORDER_LINE (OL_O_ID, OL_D_ID, OL_W_ID, OL_NUMBER, OL_I_ID, OL_SUPPLY_W_ID, OL_QUANTITY, OL_AMOUNT, OL_DIST_INFO) VALUES (%d, %d, %d, %d, 481, 1, 2, 17.16, 'lqetldzgsfktkcbcpxldrwdd');";

//	char *insert_query2 = "INSERT INTO ORDER_LINE (OL_O_ID, OL_D_ID, OL_W_ID, OL_NUMBER, OL_I_ID, OL_SUPPLY_W_ID, OL_QUANTITY, OL_AMOUNT, OL_DIST_INFO) VALUES (17, 11, 2, 2, 482, 2,  2, 17.16, 'lqetldzgsfktkcbcpxldrwdd');";

	printf("create query\n");
	rc=sqlite3_exec(db, create_query, NULL, NULL, NULL);
	if(rc!=SQLITE_OK)
		printf("error\n");

	printf("insert query\n");
	int i;
	char strbuf[300];
	memset(strbuf, 0, 300);

	for(i=0; i<100; i++){
		sprintf(strbuf, insert_query, i, i, i, i);

		rc=sqlite3_exec(db, strbuf, NULL, NULL, NULL);
		if(rc!=SQLITE_OK)
			printf("error\n");
	}

	printf("close\n");
	sqlite3_close(db);
}
