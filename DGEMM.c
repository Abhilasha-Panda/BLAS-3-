/*Multiplication of one matrix with the other*/
#include<stdio.h>
void main(){
    int i,j,k,a[100][100],b[100][100],prod[100][100];
    int m,n,m1,n1;
    printf("Enter the rows and columns of first and second matrix\n");
    scanf("%d",&m);
    scanf("%d",&n);
    scanf("%d",&m1);
    scanf("%d",&n1);
    //To check whether the entered row and column is satisfying the condition of matrix multiplication.
    if(n==m1)
    {
        // here the first and the second matrix elements are entered respectively.
        printf("The elements of first matrix are\n");
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                scanf("%d",&a[i][j]);
            }
        }
         printf("The elements of second matrix are\n");
        for(i=0;i<m1;i++)
        {
            for(j=0;j<n1;j++)
            {
                scanf("%d",&b[i][j]);
            }
        }
        //multiplication of the matrix is performed.
         printf("\n");
        for(i=0;i<m;i++)
        {
            for(j=0;j<n1;j++)
            {
               for(k=0;k<n;k++) 
               {
                prod[i][j]=prod[i][j]+a[i][k]*b[k][j];
               }
            }
        }
        //final matrix is displayed after multiplication
         printf("Multiplied matrix is\n");
        for(i=0;i<m;i++)
        {
            for(j=0;j<n1;j++)
            {
                printf("%d",prod[i][j]);
                printf(" ");
            }
            printf("\n");
        }
    }
    else
        printf("Matrix multiplication is not possible");


}