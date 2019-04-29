#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define M 2
#define R 3
#define N 26	//not sure what this value should be

char* key = "road";
char* plain_text = "I am Daniel";

//input  parameters:  plain  text  and encryption key.
//output:  the  cipher  text.
// will use the formula for R=3
// x_0(2A+A^3) % N
char* door_encryption(char* ptext, int key[M][M]) {
	
	
}

void round_x(int** output, int** ptext_mat, int** key_mat, int round) {
	int c,d,k,sum;
	int n = (strlen(plain_text)+1)/2;
	if (round==1) {
		for (c = 0; c < n; c++) {
			for (d = 0; d < M; d++) {
				for (k = 0; k < M; k++) {
					sum = sum + ptext_mat[c][k]*key_mat[k][d];
				}
				output[c][d] = sum % N;
				sum = 0;
			}
		}
		return;
	} else {

	}
}


//converts text to matrix to work with to encrypt
//this function allocates memory and needs to free it later on.
int** alloc_text_to_matrix(char* ptext,int transpose) {
	int i,j,n,rows,columns;
	int **text_mat;
	n = (strlen(ptext)+1)/2;
	if(transpose) {text_mat = (int**) malloc(sizeof(int*)*n);rows=M;columns=n;}
	else {text_mat = (int**) malloc(sizeof(int*)*M);rows=n;columns=M;}
	for (i=0;i<rows;i++) 
		text_mat[i] = (int*) malloc(sizeof(int)*columns);
	for(i=0;i<M;i++) {
		for(j=0;j<n;j++) {
			if(i*n+j < strlen(ptext)) {
				if(transpose) text_mat[i][j] = ptext[i*n+j] - ' ';
				else text_mat[j][i] = ptext[i*n+j] - ' ';
			} else {
				if(transpose) text_mat[i][j] = 0;
				else text_mat[j][i] = 0;
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
			result[k++] = mat[i][j] + ' ';
		}
	}
	result[k] = 0;
	strcpy(output,result);
	for(i=0;i<m;i++) free(mat[i]);
	free(mat);
}
	

void print_mat(int** mat, int m, int n, char* des) {
	int i,j;
	printf("%s\n",des);
	for(i=0;i<m;i++) {
		for(j=0;j<n;j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
}

void main() {
	int ** text_mat = alloc_text_to_matrix(plain_text,1);
	int ** key_mat = alloc_text_to_matrix(key,0);
	char key_string[256], text_string[256];
	int i,n =(strlen(plain_text)+1)/2;
	int** encrypted_text;
	
	encrypted_text = (int**) malloc(sizeof(int*)*n);
	for(i=0;i<n;i++) encrypted_text[i] = (int*) malloc(sizeof(int)*M);
	
	printf("----- Plain text is: %s  with length of: %d-----\n",plain_text, (int)strlen(plain_text));
	print_mat(text_mat, M ,(strlen(plain_text)+1)/2, "Plain Text Matrix:");
	printf("----- Key is: %s  with length of: %d-----\n",key, (int)strlen(key));
	print_mat(key_mat,M,M,"key matrix:");
	
	free_matrix_to_text(key_string,key_mat,M,M);
	printf("key: %s\n",key_string);
	free_matrix_to_text(text_string,text_mat,M,n);
	printf("text: %s \n",text_string);
	
	round_x(encrypted_text,text_mat,key_mat,1);
	
	print_mat(encrypted_text,M,n,"encrypted text matrix:");
	
	return;
}


