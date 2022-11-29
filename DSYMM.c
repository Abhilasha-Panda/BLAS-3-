/*Multiplication of symmetric matrix with another matrix*/
#include<stdio.h>
void main(){
    int i,j,k,a[100][100],b[100][100],a1[100][100],prod[100][100];
    int m,n,m1,n1,c;
    printf("Enter the rows and columns of first and second matrix\n");
    scanf("%d",&m);
    scanf("%d",&n);
    scanf("%d",&m1);
    scanf("%d",&n1);
    //checking whether the first matrix is square or not and to check the conditions of multiplication of matrix.
    if((n==m1)&&(m==n))
    {
        //elements of first matrix is entered.
        printf("The elements of first matrix are\n");
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                scanf("%d",&a[i][j]);
            }
        }
        for(i=0;i<m;i++)
        {
        c=0;
            for(j=0;j<n;j++)
            {
                //condition to check whether it is symmetric or not.
                if(a[i][j]==a[j][i]&&i!=j)
                  c=1;
                else 
                break;
            }
            
        }
        //if the symmteric condition is satisfied then the second matrix elements are entered.
if(c==1)   {   
         printf("The elements of second matrix are\n");
                  for(i=0;i<m1;i++)
               {

            for(j=0;j<n1;j++)
            {
                scanf("%d",&b[i][j]);
            }
        }         
         //Multiplication of symmetric matrix with another matrix.             
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
        //final matrix after multiplication is printed. 
        if(c==1){
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

}
else 
   printf("Not symmetric");
    }
    else
    printf("it is not square matrix so symmetric of the same is not possible");
 
}
