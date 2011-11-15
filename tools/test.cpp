#include  <stdio.h>
#include  <string.h>
#include  <malloc.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>

#include "parallel.h"
#define sizeOfMatrix 16
#define sizeOfBlock 2
#define BLOCK_NUM 128
double A[sizeOfMatrix][sizeOfMatrix];
double B[sizeOfMatrix][sizeOfMatrix], C[sizeOfMatrix][sizeOfMatrix], D[sizeOfMatrix][sizeOfMatrix];
struct matrix {
    double M[sizeOfBlock][sizeOfBlock];
    int n;
    }

;
void initMatrix (matrix * x , int a) {
    x->n = a;
    memset (x->M , 0 , sizeof (x->M));
    }

matrix getSubMatrix (int a , int b , int c) {
    matrix tmp;
    int i , j;
    initMatrix (& tmp , c);
    for( i = 0  ; i < c  ; i ++) for( j = 0  ; j < c  ; j ++)
    {
    	tmp.M[i][j]= A[i + a][j + b];
    }
    return tmp;
    }

void Init () {
    int i , j;
    srand (time (NULL));
    for( i = 0  ; i < sizeOfMatrix  ; i ++) for( j = 0  ; j < sizeOfMatrix  ; j ++) A[i][j]= rand () & 15;
    }

void update (int a , int b , matrix x) {
    int i , j;
    for( i = 0  ; i < x.n  ; i ++) for( j = 0  ; j < x.n  ; j ++)
    {
    	A[a + i][b + j]= x.M[i][j];
    }
    }

matrix getInverseL (matrix X) {
    int i , j , k;
    matrix a;
    initMatrix (& a , sizeOfBlock);
    for( i = 0  ; i < a.n  ; i ++) a.M[i][i]= 1;
    for( i = 0  ; i < a.n  ; i ++) for( j = 0  ; j < i  ; j ++) for( k = j  ; k < i  ; k ++) a.M[i][j]-= X.M[i][k]* a.M[k][j];
    return a;
    }

matrix getInverseU (matrix X) {
    matrix a;
    int i , j , k;
    initMatrix (& a , sizeOfBlock);
    for( i = 0  ; i < a.n  ; i ++) a.M[i][i]= 1 / X.M[i][i];
    for( i = a.n - 1  ; i >= 0  ; i --) for( j = a.n - 1  ; j > i  ; j --) for( k = i + 1  ; k <= j  ; k ++) {
        a.M[i][j]-= X.M[i][k]* a.M[k][j];
        }

    for( i = 0  ; i < a.n  ; i ++) for( j = i + 1  ; j < a.n  ; j ++) a.M[i][j]*= a.M[i][i];
    return a;
    }

matrix MatrixMul (matrix a , matrix b) {
    int n = a.n;
    matrix ans;
    int i , j , k;
    initMatrix (& ans , n);
    for( i = 0  ; i < n  ; i ++) for( j = 0  ; j < n  ; j ++) for( k = 0  ; k < n  ; k ++) ans.M[i][j]= (ans.M[i][j]+ (a.M[i][k]* b.M[k][j]));
    return ans;
    }

matrix MatrixSub (matrix a , matrix b) {
    int i , j;
    for( i = 0  ; i < a.n  ; i ++) for( j = 0  ; j < a.n  ; j ++) a.M[i][j]-= b.M[i][j];
    return a;
    }

matrix LUDecompose (matrix a) {
    int m = a.n , i , j , k;
    for( i = 0  ; i < m  ; i ++) {
        for( j = i + 1  ; j < m  ; j ++) a.M[j][i]/= a.M[i][i];
        for( j = i + 1  ; j < m  ; j ++) for( k = i + 1  ; k < m  ; k ++) a.M[j][k]-= a.M[j][i]* a.M[i][k];
        }

    return a;
    }

void func1 (int i , int j , matrix _L , matrix _U) {
    update (i , j , MatrixMul (_L , getSubMatrix (i , j , sizeOfBlock)));
    update (j , i , MatrixMul (getSubMatrix (j , i , sizeOfBlock) , _U));
    }

void func2 (int i , int j , int k) {
    update (j , k , MatrixSub (getSubMatrix (j , k , sizeOfBlock) , MatrixMul (getSubMatrix (j , i , sizeOfBlock) , getSubMatrix (i , k , sizeOfBlock))));
    }

int main (int argc , char * argv [ ]) {
    INITIALIZE_ENV(5)
 int i , j , k;
    int numOfBlock = sizeOfMatrix / sizeOfBlock;
    Init ();
    memcpy (D , A , sizeof (A));
    for( i = 0  ; i < sizeOfMatrix  ; i += sizeOfBlock) {
        matrix LU = LUDecompose (getSubMatrix (i , i , sizeOfBlock));
        matrix _L = getInverseL (LU);
        matrix _U = getInverseU (LU);
        int id = 0;
        update (i , i , LU);
        Parallel<void (int, int, matrix, matrix) , func1 > f1[BLOCK_NUM];
        for( j = i + sizeOfBlock  ; j < sizeOfMatrix  ; j += sizeOfBlock) {
            f1[id ++](i , j , _L , _U);
            }

        id = 0;
        Parallel<void (int, int, int) , func2 > f2[BLOCK_NUM];
        for( j = i + sizeOfBlock  ; j < sizeOfMatrix  ; j += sizeOfBlock) for( k = i + sizeOfBlock  ; k < sizeOfMatrix  ; k += sizeOfBlock) f2[id ++].waiting( all( f1) )( i , j , k);
        }

    return 0;
    }
