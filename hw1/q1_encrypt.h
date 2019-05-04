#ifndef Q1_ENCRYPT_H
#define Q1_ENCRYPT_H

#include "q1_auxFuncs.h"

//input  parameters:  plain  text  and encryption key.
//output:  the  cipher  text.
// will use the formula for R=3
// x_0(2A+A^3) % N
extern void door_encryption(char* out, char* ptext, int** pkey) {
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

#endif /*  Q1_ENCRYPT_H */