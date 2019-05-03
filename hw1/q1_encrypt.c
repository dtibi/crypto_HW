#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define M 2
#define R 3
#define N 26	//not sure what this value should be
#define DEBUG 1
char* key = "road";
char* plain_text = "daniel";

int** alloc_round_A_to_3 (int** pkey);
int** alloc_text_to_matrix(char* ptext,int transpose);
void free_matrix_to_text(char* output, int** mat, int m, int n);
void door_decryption(char* output, char* cipher, int** pkey);
void door_encryption(char* out, char* ptext, int** pkey);
void print_mat(int** mat, int m, int n, char* des);
int mod_inverse(int a , int m);


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

/* This function calculates 2 times pkey matrix + pkey matrix raised to the power of 3 
it returns the result of the matrix pointer to allocated space that needs to be freed later on.*/
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


/*converts text to matrix to work with to encrypt
this function allocates memory and needs to free it later on.*/
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

int mod_inverse(int a, int m) {
    a = a % m; 
    for (int x=1; x<m; x++) 
       if ((a*x) % m == 1) 
          return x; 
} 

void door_decryption(char* output, char* cipher, int** pkey) {
	int determinant,inv_det;
	int i,j,k,n,sum=0;
	int** adj = alloc_text_to_matrix(key,0);
	int** cipher_text_mat = alloc_text_to_matrix(cipher,1);
	int** decrypted_text_mat = alloc_text_to_matrix(cipher,1);
	float** inv_pkey;
	int** key_mat = alloc_round_A_to_3(pkey);
	n = (strlen(cipher)+1)/2;
	inv_pkey = malloc(sizeof(float*)*M);
	for(i=0;i<M;i++)
		inv_pkey[i] = malloc(sizeof(float)*M);
	
	//finding determinant of 2x2 matrix
	determinant = (key_mat[0][0] * key_mat[1][1] - key_mat[0][1] * key_mat[1][0]) % N;
	inv_det = mod_inverse(determinant,N);
	
	if (DEBUG) {
		print_mat(key_mat,M,M,"2*pkey+pkey^3:");
		printf("determinant: %f\n", determinant);
	}
	if(determinant == 0) {
		printf("determinant equal to zero therefore no A^-1 exists!");
		exit(1);
	}
	adj[0][0] = key_mat[1][1];
	adj[1][1] = key_mat[0][0];
	adj[0][1] = -key_mat[0][1];
	adj[1][0] = -key_mat[1][0];
	
	if(DEBUG){
		print_mat(adj,M,M,"adjoint matrix");
		printf("Inverse of matrix is: \n");
	}
	for(i = 0; i < M; i++){
		for(j = 0; j < M; j++) {
			inv_pkey[i][j] = (adj[i][j]*inv_det) % N;
			if(DEBUG)
				printf("%.2f\t",inv_pkey[i][j]);
		}
		if(DEBUG)printf("\n");
	}
 
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < M; j++) {
			for (k = 0; k < M; k++) {
				sum = sum + cipher_text_mat[i][k]*inv_pkey[k][j];
			}
			decrypted_text_mat[i][j] = sum % N;
			sum = 0;
		}
	}
	
	for(i=0;i<M;i++)free(key_mat[i]);
	free(key_mat);
	free_matrix_to_text(output,adj,M,M);
	free_matrix_to_text(output,cipher_text_mat,n,M);
	if(DEBUG)print_mat(decrypted_text_mat, n, M, "dycripted text matrix");
	free_matrix_to_text(output,decrypted_text_mat,n,M);
 
   return;
}


//input  parameters:  plain  text  and encryption key.
//output:  the  cipher  text.
// will use the formula for R=3
// x_0(2A+A^3) % N
void door_encryption(char* out, char* ptext, int** pkey) {
	int i,j,k,sum=0;
	int n = (strlen(ptext)+1)/2;
	int** ptext_mat = alloc_text_to_matrix(ptext,1);
	int** out_mat = alloc_text_to_matrix(ptext,1); //using this func just to allocate space for a matrix same size as ptext
	int** calced_key = alloc_round_A_to_3(pkey);
	for (i = 0; i < n; i++) {
		for (j = 0; j < M; j++) {
			for (k = 0; k < M; k++) {
				sum = sum + ptext_mat[i][k]*calced_key[k][j];
			}
			out_mat[i][j] = sum % N;
			sum = 0;
		}
	}
	for(i=0;i<M;i++)free(calced_key[i]);
	free(calced_key);
	free_matrix_to_text(out,ptext_mat,n,M);
	free_matrix_to_text(out,out_mat,n,M);
	return;
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



