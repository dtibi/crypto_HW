 #ifndef Q1_DECRYPT_H
 #define Q1_DECRYPT_H

#include "q1_auxFuncs.h"

extern void door_decryption(char* output, char* cipher, int** pkey) {
	int determinant,inv_det;
	int i,j,k,n,sum=0;
	int adj[M][M];
	int** cipher_text_mat = alloc_text_to_matrix(cipher,1);
	int** decrypted_text_mat = alloc_text_to_matrix(cipher,1);
	float** inv_pkey;
	int** key_mat = alloc_round_A_to_3(pkey);
	n = (strlen(cipher)+1)/2;
	inv_pkey = malloc(sizeof(float*)*M);
	for(i=0;i<M;i++)
		inv_pkey[i] = malloc(sizeof(float)*M);
	for(i=0;i<M;i++)
		for(j=0;j<M;j++)
			adj[i][j] = key_mat[i][j];
	//finding determinant of 2x2 matrix
	determinant = (key_mat[0][0] * key_mat[1][1] - key_mat[0][1] * key_mat[1][0]) % N;
	inv_det = mod_inverse(determinant,N);
	
	if (DEBUG) {
		print_mat(key_mat,M,M,"2*pkey+pkey^3:");
		printf("determinant: %d\n", determinant);
	}
	if(determinant == 0) {
		if(DEBUG)printf("determinant equal to zero therefore no A^-1 exists!");
		strcpy(output, "NULL");
		return;
	}
	adj[0][0] = key_mat[1][1];
	adj[1][1] = key_mat[0][0];
	adj[0][1] = -key_mat[0][1];
	adj[1][0] = -key_mat[1][0];
	
	if(DEBUG){
		print_mat((int**)adj,M,M,"adjoint matrix");
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
	free_matrix_to_text(output,cipher_text_mat,n,M);
	if(DEBUG)print_mat(decrypted_text_mat, n, M, "dycripted text matrix");
	free_matrix_to_text(output,decrypted_text_mat,n,M);
 
   return;
}

#endif /* Q1_DECRYPT_H */
