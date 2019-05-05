#ifndef Q1_AUXFUNCS_H
#define Q1_AUXFUNCS_H

#define M 2
#define R 3
#define N 26	//not sure what this value should be
#define DEBUG 0

/* This function calculates 2 times pkey matrix + pkey matrix raised to the power of 3 
it returns the result of the matrix pointer to allocated space that needs to be freed later on.*/
extern int** alloc_round_A_to_3 (int** pkey){
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
extern int** alloc_text_to_matrix(char* ptext,int transpose) {
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

extern void free_matrix_to_text(char* output, int** mat, int m, int n) {
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

extern int mod_inverse(int a, int m) {
    a = a % m; 
    for (int x=1; x<m; x++) 
       if ((a*x) % m == 1) 
          return x; 
} 

extern void print_mat(int** mat, int m, int n, char* des) {
	int i,j;
	printf("%s\n",des);
	for(i=0;i<m;i++) {
		for(j=0;j<n;j++)
			printf("%3d(%c)", mat[i][j],mat[i][j]+'a');
		printf("\n");
	}
}

#endif /* Q1_AUXFUNCS_H */
