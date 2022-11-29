#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/***********************************/
/**************** PROTOTYPES *****************/
/***********************************/
/* This function performs the Cholesky decomposition A = L * L**T and B=L1*L1**T */
int CholeskyDecomposition(int n, double **L, int nn, double **L1);
/* This function computes the Cholesky factorization of a real symmetric positive definite matrix A
using the recursive algorithm. */
int RecursiveCholeskyFactorization(int n, double **a, int a_row_offset, int a_col_offset, int nn,
                                   double **a1, int a1_row_offset, int a1_col_offset);
/* This function performs the symmetric rank k operation C := C + A*B**T+B*A**T */
int SymmetricRank2KOperation(int n, int k, double **a, int read_row_offset, int read_col_offset, int write_row_offset, int write_col_offset, int nn, int k1, double **a1, int read1_row_offset, int read1_col_offset, int write1_row_offset, int write1_col_offset);
/***********************************/
/***************** MAIN ******************/
/***********************************/
int main(int argc, char *argv[])
{
    int i, j, system_return = 0, error = 0;
    /* Read inputs */
    /* Get the order of symmetric matrix a*/
    int order = 0;
    int order1 = 0;
    FILE *infile_order = fopen("order.txt", "r");
    system_return = fscanf(infile_order, "%u", &order);
    if (system_return == -1)
    {
        printf("Failed reading file order.txt\n");
    }
    fclose(infile_order);
    printf("order = %d\n", order);
    // get the order of matrix b
    FILE *infile_order1 = fopen("order1.txt", "r");
    system_return = fscanf(infile_order1, "%u", &order1);
    if (system_return == -1)
    {
        printf("Failed reading file order1.txt\n");
    }
    fclose(infile_order1);
    printf("order1 = %d\n", order1);
    /* Now get the matrix values of a */
    double **matrix;
    printf("matrix = \n");
    FILE *infile_matrix = fopen("matrix.txt", "r");
    matrix = malloc(sizeof(double *) * order);
    for (i = 0; i < order; i++)
    {
        matrix[i] = malloc(sizeof(double) * order);
        for (j = 0; j < order; j++)
        {
            system_return = fscanf(infile_matrix, "%lf\t", &matrix[i][j]);
            if (system_return == -1)
            {
                printf("Failed reading file matrix.txt\n");
            }
            else
            {
                printf("%lf\t", matrix[i][j]);
            }
        } // end of j loop
        printf("\n");
    } // end of i loop
    fclose(infile_matrix);
    // getting the matrix value of the second matrix b
    double **matrix1;
    printf("matrix1 = \n");
    FILE *infile_matrix1 = fopen("matrix1.txt", "r");
    matrix1 = malloc(sizeof(double *) * order1);
    for (i = 0; i < order1; i++)
    {
        matrix1[i] = malloc(sizeof(double) * order1);
        for (j = 0; j < order1; j++)
        {
            system_return = fscanf(infile_matrix1, "%lf\t", &matrix1[i][j]);
            if (system_return == -1)
            {
                printf("Failed reading file matrix1.txt\n");
            }
            else
            {
                printf("%lf\t", matrix1[i][j]);
            }
        } // end of j loop
        printf("\n");
    } // end of i loop
    fclose(infile_matrix1);
    // end of calling the second matrix
    /* Call function to perform Cholesky decomposition on our real symmetric positive definite
   matrix A and B*/
    error = CholeskyDecomposition(order, matrix, order1, matrix1);
    /* Free dynamically allocated memory */
    for (i = 0; i < order; i++)
    {
        free(matrix[i]);
    } // end of i loop
    free(matrix);
    return 0;
    // end of main
    /*2. Free dynamically allocated memory */
    for (i = 0; i < order1; i++)
    {
        free(matrix1[i]);
    } // end of i loop
    free(matrix1);
    return 0;
} // end of main
/***********************************/
/**************** FUNCTIONS ****************/
/***********************************/
/* This function performs the Cholesky decomposition A = L * L**T * and B=L1*L1**T */
int CholeskyDecomposition(int n, double **L, int nn, double **L1)
{
    int i, j, error = 0;
    error = RecursiveCholeskyFactorization(n, L, 0, 0, nn, L1, 0, 0);
    /* Print L and L1 or error code message */
    if (error == 0)
    {
        /* Zero upper triangular matrix without diagonal since it wasn't updated */
        for (i = 0; i < n - 1; i++)
        {
            for (j = i + 1; j < n; j++)
            {
                L[i][j] = 0.0;
            } // end of j loop
        }     // end of i loop

        printf("\nL = \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%e\t", L[i][j]);
            } // end of j loop
            printf("\n");
        } // end of i loop

        for (i = 0; i < nn - 1; i++)
        {
            for (j = i + 1; j < nn; j++)
            {
                L1[i][j] = 0.0;
            } // end of j loop
        }     // end of i loop

        printf("\nL = \n");
        for (i = 0; i < nn; i++)
        {
            for (j = 0; j < nn; j++)
            {
                printf("%e\t", L1[i][j]);
            } // end of j loop
            printf("\n");
        } // end of i loop
    }
    else if (error == 1)
    {
        printf("ERROR: RecursiveCholeskyFactorization somehow managed to start with n == 0\n");
    }
    else if (error == 2)
    {
        printf("ERROR: SolveTriangularMatrixEquation somehow managed to start with m or n == 0\n");
    }
    else if (error == 3)
    {
        printf("ERROR: SymmetricRank2KOperation somehow managed to start with n or k == 0\n");
    }
    else
    {
        printf("ERROR: Matrix is not positive-definite!\n");
    }
    return error;
} // end of CholeskyFactorization function

/*This function computes the Cholesky factorization of a real symmetric
positive definite matrix A and B using the recursive algorithm.
The factorization has the form
 A = L * L**T , B=L1 * L1**T
where L , L1 is lower triangular.
This is the recursive version of the algorithm. It divides
the matrix into four submatrices:
 [ A11 | A12 ] where A11 is n1 by n1 and A22 is n2 by n2
 A = [ -----|----- ] with n1 = n / 2 and n2 = n - n1
 [ A21 | A22 ]
The function calls itself to factor A11,B11. Update and scale A21, B21
update A22, B22 then calls itself to factor A22,B22.
Modified from dpotrf2.
*/
int RecursiveCholeskyFactorization(int n, double **a, int a_row_offset, int a_col_offset, int nn,
                                   double **a1, int a1_row_offset, int a1_col_offset)
{
    /*
    Args:
    n,n1: The order of the matrix a. n >= 0.
    a,a1: Matrix that we are modifying in place.
    a_row_offset,a1_row_offset: Number of rows we need to offset a.
    a_col_offset, a1_col_offset: Number of cols we need to offset a.

    Returns:
    Integer error code.
    */
    int i, j, error = 0;
    /* Quick return if possible */
    if (n == 0 || nn == 0)
    {
        return 1;
    }
    if (n == 1 || nn == 1) // n == 1 case
    {
        /* Test for non-positive-definiteness */
        if (a[a_row_offset][a_col_offset] <= 0.0 || isnan(a[a_row_offset][a_col_offset]) ||
            a1[a1_row_offset][a1_col_offset] <= 0.0 || isnan(a1[a1_row_offset][a1_col_offset]))
        {
            return 99;
        }
        /* Factor */
        a[a_row_offset][a_col_offset] = sqrt(a[a_row_offset][a_col_offset]);
        a1[a1_row_offset][a1_col_offset] = sqrt(a1[a1_row_offset][a1_col_offset]);
    }
    else // use recursive code
    {
        int n1 = n / 2;
        int n2 = n - n1;
        int n3 = nn / 2;
        int n4 = nn - n3;
        /* Factor A11 and B11 */
        error = RecursiveCholeskyFactorization(n1, a, a_row_offset, a_col_offset, n3, a1, a1_row_offset,
                                               a1_col_offset);
        if (error != 0)
        {
            return error;
        }
        /* Update A22 and B22 */
        error = SymmetricRank2KOperation(n2, n1, a, n1 + a_row_offset, a_col_offset, n1 + a_row_offset,
                                         n1 + a_col_offset, n4, n3, a1, n3 + a1_row_offset, a1_col_offset, n3 + a1_row_offset, n1 + a1_col_offset);
        if (error != 0)
            if (error != 0)
            {
                return error;
            }

        /* Factor A22 and B22*/
        error = RecursiveCholeskyFactorization(n2, a, n1 + a_row_offset, n1 + a_col_offset, n4, a1, n3 + a1_row_offset, n3 + a1_col_offset);
    }
    return error;
} // end of RecursiveCholeskyFactorization function
// PERFORMING SYMMETRIC RANK 2K UPDATE OPERATION
//* Form C := C+B*A**T+A*B**T
int SymmetricRank2KOperation(int n, int k, double **a, int read_row_offset, int read_col_offset, int write_row_offset, int write_col_offset, int nn, int k1, double **a1, int read1_row_offset, int read1_col_offset, int write1_row_offset, int write1_col_offset)
{
    /*
    Args:
    n,nn: Number of rows of current submatrix of a.
    k,k1: Number of cols of current submatrix of a.
    a,a1: Matrix that we are modifying in place.
    read_row_offset, read1_row_offset: Number of rows we need to offset for reading from a.
    read_col_offset, read1_col_offset: Number of cols we need to offset for reading from a.
    write_row_offset, write1_row_offset: Number of rows we need to offset for writing to a.
    write_col_offset, write1_col_offset: Number of cols we need to offset for writing to a.

    Returns:
    Integer error code.
    */
    int i, j, l, i1, j1, l1;
    double temp, temp3;
    double dl, dl1;
    /* Quick return if possible. */
    if (n == 0 || k == 0 || nn == 0 || k1 == 0)
    {
        return 3;
    }
    /* Form C := C+B*A**T+A*B**T */
    for (j = 0; j < n; j++)
    {
        for (l = 0; l < k; l++)
        {
            if (a[j + read_row_offset][l + read_col_offset] != 0.0)
            {
                temp = -a[j + read_row_offset][l + read_col_offset];
                for (j1 = 0; j1 < nn; j1++)
                {
                    for (l1 = 0; l1 < k1; l1++)
                    {
                        if (a1[j1 + read1_row_offset][l1 + read1_col_offset] != 0.0)
                        {
                            temp3 = -a1[j1 + read1_row_offset][l1 + read1_col_offset];
                            for (i = j - 1; i < n; i++)
                            {
                                dl1 = temp3 * a[i + read_row_offset][l + read_col_offset];
                            }

                            // end of i loop
                            for (i1 = j1 - 1; i1 < nn; i1++)
                            {
                                dl = a1[i1 + read1_row_offset][l1 + read1_col_offset] * temp;
                            }
                            a[i + write_row_offset][j + write_col_offset] += dl + dl1;
                        }
                    }
                }
            } // end of l loop
        }     // end of j loop
    }
    return 0;
} // end of SymmetricRankKOperation function