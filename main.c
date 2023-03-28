#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#define MAX 20
FILE *fAptr;
FILE *fBptr;
FILE *fCptr_perMatrix;
FILE *fCptr_perRow;
FILE *fCptr_perElem;
int step_i = 0;
int matA[MAX][MAX]={{0}};
int matB[MAX][MAX]={{0}};
int matC[MAX][MAX]={{0}};
int matC_perRow[MAX][MAX]={{0}};
int matC_perElement[MAX][MAX]={{0}};
int rA,cA,rB,cB;
int step_a = 0;
int step_b = 0;
struct timeval stop, start;

struct element
{
int row_num,col_num;

};

//////////// Routine of thread per row /////////////////////////////////////////////

void* multi(void* i)
{
    int a = (int) i;//i denotes row number of resultant matC

    for (int j = 0; j < cB; j++)
      for (int k = 0; k < cA; k++){
        matC_perRow[a][j] += matA[a][k] * matB[k][j];
}
}

///////////// Routine of thread per element
void* multi1(void* a){

        struct element *b;
        b=(struct element *)a;
        for(int r=0;r<cA;r++)
        {
            matC_perElement[b->row_num][b->col_num] += matA[b->row_num][r]*matB[r][b->col_num];
        }
        free(b);

}


int main(int argc, char **argv[])
{
    char token;
    char tokenB;

////////////////////////////////////////////////////////////////////////// initializing pointers to files /////////////////////////////////////////////////////////////////////////////////////
if(argc == 1){ //keda ana msh md5l arguments
        /*strcat(argv[1],".txt");
        strcat(argv[2],".txt");
        strcat(argv[3],".txt");*/
        printf("NO ARG\n");
        /*if(((fCptr_perElem = fopen(strcat(C,"_per_elem"),"a")))==NULL){
            printf("Error opening file");
            exit(1);
        }*/
        if((fAptr = fopen("a","r"))==NULL){
        printf("Error opening file a no arg");
        exit(1);
    }
        if((fBptr = fopen("b","r"))==NULL){
        printf("Error opening file b no arg");
        exit(1);
    }
        if((fCptr_perMatrix= fopen("c_per_matrix","w"))==NULL){
        printf("Error opening file c no arg");
        exit(1);
    }
        if((fCptr_perRow = fopen("c_per_row","w"))==NULL){
        printf("Error opening file c no arg");
        exit(1);
    }
        if((fCptr_perElem = fopen("c_per_element","w"))==NULL){
        printf("Error opening file c no arg");
        exit(1);
    }
}
else if(argc > 1) {
    //md5l Arg
        if((fAptr = fopen(argv[1],"r"))==NULL){
        printf("Error opening file a");
        exit(1);
    }
        if((fBptr = fopen(argv[2],"r"))==NULL){
        printf("Error opening file b");
        exit(1);
    }
//////////////////////////////////////////////////////////////// Third argument which is the product file needs to be concatenated with the string "_per_matrix","_per_row" or "_per_element"
        char C_per_matrix[20];
        char C_per_row[20];
        char C_per_element[20];
        strcpy(C_per_matrix,argv[3]);
        strcpy(C_per_row,argv[3]);
        strcpy(C_per_element,argv[3]);
        strcat(C_per_matrix,"_per_matrix");
        strcat(C_per_row,"_per_row");
        strcat(C_per_element,"_per_element");
        if((fCptr_perMatrix = fopen(C_per_matrix,"w"))==NULL){
            printf("Error opening file c");
            exit(1);
        }
        if((fCptr_perRow = fopen(C_per_row,"w"))==NULL){
            printf("Error opening file c");
            exit(1);
        }
        if((fCptr_perElem = fopen(C_per_element,"w"))==NULL){
            printf("Error opening file c");
            exit(1);
        }




}
    /*strcat(argv[1],".txt");
    strcat(argv[2],".txt");
    strcat(argv[3],".txt");*/
/*    if((fAptr = fopen("mata.txt","r"))==NULL){
        printf("Error opening file");
        exit(1);
    }*/
/////////////////////////////////////////////////////////////////////////////////// Reading from files ////////////////////////////////////////////////////////////////////////////////////////
    while(1){
        fscanf(fAptr,"%c",&token);
        if(token == '='){
            break;
            }
    }
    fscanf(fAptr,"%d",&rA);
    while(1){
        fscanf(fAptr,"%c",&token);
        if(token == '='){
            break;
            }
    }
    fscanf(fAptr,"%d",&cA);
    for(size_t i=0;i<rA;i++){
        for(size_t j=0;j<cA;j++){
            fscanf(fAptr,"%d",&matA[i][j]);
        }
    }
    printf("\033[1;31mFirst Matrix Displayed:\n");
        for(size_t i=0;i<rA;i++){
        for(size_t j=0;j<cA;j++){
            printf("%d ",matA[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    while(1){
        fscanf(fBptr,"%c",&tokenB);
        if(tokenB == '='){
            break;
            }
    }
    fscanf(fBptr,"%d",&rB);
    while(1){
        fscanf(fBptr,"%c",&tokenB);
        if(tokenB == '='){
            break;
            }
    }
    fscanf(fBptr,"%d",&cB);
    for(size_t i=0;i<rB;i++){
        for(size_t j=0;j<cB;j++){
            fscanf(fBptr,"%d",&matB[i][j]);
        }
    }
    printf("\033[1;32mSecond Matrix Displayed:\n");
        for(size_t i=0;i<rB;i++){
        for(size_t j=0;j<cB;j++){
            printf("%d ",matB[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    if(cA != rB){
        printf("Cannot multiply matrices Number of Rows of 1st matrix is not equal to Number of Cols of 2nd matrix");
        return;
    }
///////////////////////////////////////////////////////////////////////////// Thread per Matrix //////////////////////////////////////////////////////////////////////////////////////////////
printf("\033[1;33mA thread per matrix\n");
fputs("Method : A Thread per matrix\n",fCptr_perMatrix);
gettimeofday(&start, NULL); //start checking time
for(int k=0;k<rA;k++){
for(int l=0;l<cB;l++){
    matC[k][l]=0;
    for(int j=0;j<cA;j++){
        matC[k][l] += (matA[k][j]*matB[j][l]);
    }
}
}

gettimeofday(&stop, NULL);
long unsigned time_perMatrix_secs = stop.tv_sec - start.tv_sec;
long unsigned time_perMatrix_micros = stop.tv_usec - start.tv_usec;


for(int k=0;k<rA;k++){
for(int l=0;l<cB;l++){
printf("%d ",matC[k][l]);
fprintf(fCptr_perMatrix,"%d ",matC[k][l]);}
printf("\n");
fprintf(fCptr_perMatrix,"\n");
}
printf("\n");
printf("Seconds taken: %d\n",time_perMatrix_secs);
printf("MicroSeconds taken: %lu\n\n",time_perMatrix_micros);
fputs("Time taken: ",fCptr_perMatrix);
fprintf(fCptr_perMatrix,"%d seconds / %d microsecs",time_perMatrix_secs,time_perMatrix_micros);
//////////////////////////////////////////////////////////////////////////// Thread per Row /////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\033[1;34mA thread per row\n");
    fputs("Method : A Thread per row\n",fCptr_perRow);
    pthread_t threads_byrow[rA];
    /*mult_per_row();*/
    gettimeofday(&start, NULL); //start checking time

    for (long i = 0; i <rA; i++) {
        pthread_create(&threads_byrow[i], NULL, multi, (void*)i);
    }
    //////////////// Wait for the main thread to create all worker threads then call pthread_join on all worker threads so your program does not work sequentially ///////////////////////////////////////
    for (long i = 0; i <rA; i++) {
        pthread_join(threads_byrow[i], NULL);
    }

    gettimeofday(&stop, NULL);

    long unsigned time_perRow_secs = stop.tv_sec - start.tv_sec;
    long unsigned time_perRow_micros = stop.tv_usec - start.tv_usec;
    for(int i = 0;i<rA;i++){
    for(int j = 0;j<cB;j++){
        printf("%d ",matC_perRow[i][j]);
        fprintf(fCptr_perRow,"%d ",matC_perRow[i][j]);
    }printf("\n");
    fprintf(fCptr_perRow,"\n");
    }printf("\n");
    printf("Seconds taken: %d\n",time_perRow_secs);
    printf("MicroSeconds taken: %lu\n\n",time_perRow_micros);
    fputs("Time taken: ",fCptr_perRow);
    fprintf(fCptr_perRow,"%d seconds / %d microsecs",time_perRow_secs,time_perRow_micros);
/////////////////////////////////////////////////////////////////////////// Thread per Element //////////////////////////////////////////////////////////////////////////////////////////////
    int thread_id = 0;

    pthread_t threads_byelem[rA][cB];

    gettimeofday(&start, NULL);
    printf("\033[1;35mA thread per element\n");
    fputs("Method : A Thread per element\n",fCptr_perElem);

    for (int i = 0; i <rA; i++) {
        for(int j=0;j<cB;j++){
            struct element *a;
            a=(struct element *)malloc(sizeof(struct element));
            a->row_num = i;
            a->col_num = j;
            pthread_create(&threads_byelem[i][j], NULL, multi1,(void*)a);
        }
}
    for(int k=0;k<rA;k++){
        for(int j=0;j<cB;j++){
        pthread_join(threads_byelem[k][j], NULL);}
    }
    gettimeofday(&stop, NULL);
    long unsigned time_perElem_secs = stop.tv_sec - start.tv_sec;
    long unsigned time_perElem_micros = stop.tv_usec - start.tv_usec;

        for(int i = 0;i<rA;i++){
            for(int j = 0;j<cB;j++){
                printf("%d ",matC_perElement[i][j]);
                fprintf(fCptr_perElem,"%d ",matC_perElement[i][j]);
        }printf("\n");
        fprintf(fCptr_perElem,"\n");
    } printf("\n");
    printf("Seconds taken: %ld\n",time_perElem_secs);
    printf("MicroSeconds taken: %lu\n",time_perElem_micros);

    fputs("Time taken: ",fCptr_perElem);
    fprintf(fCptr_perElem,"%d seconds / %d microsecs",time_perElem_secs,time_perElem_micros);
    printf("\033[1;0m");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







    return 0;
}
