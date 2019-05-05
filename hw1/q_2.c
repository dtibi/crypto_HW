#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "q1_encrypt.h"
#include "q1_decrypt.h"
#include "q1_auxFuncs.h"

char* key;
char* plain_text = "daniel";

void main() {
	int** text_mat = alloc_text_to_matrix(plain_text,1);
	int** key_mat;
	int** calced_key;
	char key_string[256], text_string[256];
	char i,j,k,w;
	char encrypted_text[N], decrypted_text[N];
	int counter=0;
	key = malloc(sizeof(char)*N);
	strcpy(key,"road");
	key_mat = alloc_text_to_matrix(key,0);
	door_encryption(encrypted_text,plain_text,key_mat);
	printf("text = %s\nkey = %s\nencrypted text = %s\n", plain_text,key,encrypted_text);
	clock_t start, end;
	double cpu_time_used;

	start = clock();
	
	for(i='a';i<N+'a';i++) {
		for (j='a';j<N+'a';j++) {
			for (k='a';k<N+'a';k++) {
				for (w='a';w<N+'a';w++) {
					counter++;
					key[0] = i;
					key[1] = j;
					key[2] = k;
					key[3] = w;
					key_mat = alloc_text_to_matrix(key,0);
					door_decryption(decrypted_text,encrypted_text,key_mat);
					if(strcmp(decrypted_text,plain_text)==0) {
						print_mat(key_mat,M,M,"found key:");
						end = clock();
						cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
						printf("CPU time used: %lf seconds\nnumber of iterations: %d\n", cpu_time_used,counter);
						return;
					}
				}
			}
		}
	}
	return;
}
