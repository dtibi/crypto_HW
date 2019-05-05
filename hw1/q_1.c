#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "q1_encrypt.h"
#include "q1_decrypt.h"
#include "q1_auxFuncs.h"

char* key = "zzpk";
char* plain_text = "daniel";

void main() {
	int** text_mat = alloc_text_to_matrix(plain_text,1);
	int** key_mat = alloc_text_to_matrix(key,0);
	int** calced_key;
	char key_string[256], text_string[256];
	int i,j,n =(strlen(plain_text)+1)/2;
	char encrypted_text[N], decrypted_text[N];
	int ptext_len = strlen(plain_text);
	int key_text_len = strlen(key);
	
	printf("----- Plain text is: %s  with length of: %d-----\n",plain_text, ptext_len);
	printf("----- Key is: %s  with length of: %d-----\n",key, key_text_len);
	printf("encrypting plain text in progress...\n");
	if (DEBUG) {
		print_mat(text_mat, (strlen(plain_text)+1)/2,M, "Plain Text Matrix:");
		print_mat(key_mat,M,M,"key matrix:");
	}
	
	door_encryption(encrypted_text,plain_text,key_mat);
	printf("encrypted text: %s\n" , encrypted_text);
	
	printf("decryption in progress...\n");
	door_decryption(decrypted_text,encrypted_text,key_mat);
	
	printf("decrypted text from: %s to %s\n",encrypted_text,decrypted_text);
	
	free_matrix_to_text(key_string,key_mat,M,M);
	//key_mat is null at this point
	
	free_matrix_to_text(text_string,text_mat,n,M);
	//text_mat is null at this point
	
	printf("done\n");
	return;
}
