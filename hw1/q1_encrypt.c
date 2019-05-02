#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define M 2
#define R 3
#define N 26	//not sure what this value should be

char* key = "road";
char* plain_text = "hadarr";


int** alloc_round_A_to_3 (int** pkey){
	int i,j,k,sum=0;
	int A_pow_2[M][M];
	int** out = (int**) malloc(sizeof(int*)*M);
	for(i=0;i<M;i++) out[i] = (int*) malloc(sizeof(int)*M);
	
	//calculating A^2 (pkey^2)
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			for (k = 0; k < M; k++) {
				sum = sum + pkey[i][k]*pkey[k][j];
			}
			A_pow_2[i][j] = sum;
			sum = 0;
		}
	}
	//calculating A^3
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			for (k = 0; k < M; k++) {
				sum = sum + A_pow_2[i][k]*pkey[k][j];
			}
			out[i][j] = sum % N;
			sum = 0;
		}
	}
	
	for(i=0;i<M;i++)
		for(j=0;j<M;j++)
			out[i][j] = (out[i][j] + 2*pkey[i][j])%N;
	//out holds 2*pkey+pkey^2 matrix ie. out=2A+A^2
	return out;
}



//converts text to matrix to work with to encrypt
//this function allocates memory and needs to free it later on.
int** alloc_text_to_matrix(char* ptext,int transpose) {
	int i,j,n,d,rows,columns;
	int **text_mat;
	n = (strlen(ptext)+1)/2;
	d = 0;
	if(transpose) {text_mat = (int**) malloc(sizeof(int*)*n);rows=n;columns=M;}
	else {text_mat = (int**) malloc(sizeof(int*)*M);rows=M;columns=n;}
	for (i=0;i<rows;i++) 
		text_mat[i] = (int*) malloc(sizeof(int)*columns);
	for(i=0;i<rows;i++) {
		for(j=0;j<columns;j++) {
			if(d < strlen(ptext)) {
				text_mat[i][j] = ptext[d++] - 'a';
			} else {
				text_mat[i][j] = 0;
			}
		}
	}
	return text_mat;
}

void free_matrix_to_text(char* output, int** mat, int m, int n) {
	int i,j,k=0;
	char result[256];
	for(i=0;i<m;i++) {
		for(j=0;j<n;j++) {
			result[k++] = mat[i][j] + 'a';
		}
	}
	result[k] = 0;
	strcpy(output,result);
	for(i=0;i<m;i++) free(mat[i]);
	free(mat);
	return;
}

//input  parameters:  plain  text  and encryption key.
//output:  the  cipher  text.
// will use the formula for R=3
// x_0(2A+A^3) % N
char* alloc_door_encryption(char* ptext, int** key) {
	int i,j,k,sum=0;
	int n = (strlen(ptext)+1)/2;
	int** ptext_mat = alloc_text_to_matrix(ptext,1);
	int** out_mat = alloc_text_to_matrix(ptext,1); //using this func just to allocate space for a matrix same size as ptext
	char* out = malloc(sizeof(char)*N);
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < M; j++) {
			for (k = 0; k < M; k++) {
				sum = sum + ptext_mat[i][k]*key[k][j];
			}
			out_mat[i][j] = sum % N;
			sum = 0;
		}
	}
	free_matrix_to_text(out,ptext_mat,n,M);
	free_matrix_to_text(out,out_mat,n,M);
	return out;
}

void print_mat(int** mat, int m, int n, char* des) {
	int i,j;
	printf("%s\n",des);
	for(i=0;i<m;i++) {
		for(j=0;j<n;j++)
			printf("%3d(%c)", mat[i][j],mat[i][j]+'a');
		printf("\n");
	}
}

void main() {
	int** text_mat = alloc_text_to_matrix(plain_text,1);
	int** key_mat = alloc_text_to_matrix(key,0);
	int** calced_key;
	char key_string[256], text_string[256];
	int i,j,n =(strlen(plain_text)+1)/2;
	char* encrypted_text;
	
	printf("----- Plain text is: %s  with length of: %d-----\n",plain_text, (int)strlen(plain_text));
	print_mat(text_mat, (strlen(plain_text)+1)/2,M, "Plain Text Matrix:");
	printf("----- Key is: %s  with length of: %d-----\n",key, (int)strlen(key));
	print_mat(key_mat,M,M,"key matrix:");

	calced_key = alloc_round_A_to_3(key_mat);
	encrypted_text = alloc_door_encryption(plain_text,calced_key);
	printf("encrypted text: %s\n" , encrypted_text);
	free(encrypted_text);
	
	
	free_matrix_to_text(key_string,calced_key,M,M);
	//key_mat is null at this point
	printf("key (2A+A^3): %s\n",key_string);
	
	free_matrix_to_text(key_string,key_mat,M,M);
	//key_mat is null at this point
	printf("Original key: %s\n",key_string);
	
	free_matrix_to_text(text_string,text_mat,n,M);
	//text_mat is null at this point
	printf("text: %s \n",text_string);
	
	
	return;
}


