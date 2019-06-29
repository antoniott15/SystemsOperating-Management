#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#define NUM_THREADS 8

struct CData
{
  int lines3, columns3, columns, **A, **B, **C, divATHR, DivOt;
};

void *Cmatrix(void *data);

int main(int argc, char *argv[])
{
  srand(time(NULL));
  int i, j, r, lines, temp, lines2, lines3, columns, columns2, columns3, **A, **B, **C;
  long compTime;
  struct timeval start;
  struct timeval finish;
  double Time;
  do
  {
    printf("Type the MatrixA lines:\t");
    scanf("%d", &lines);
    printf("Type the MatrixA columns:\t");
    scanf("%d", &columns);
    printf("Type the MatrixB lines:\t");
    scanf("%d", &lines2);
    printf("Type the MatrixB columns:\t");
    scanf("%d", &columns2);
    if (columns != lines2)
      printf("No es posible vuelva a digitar los numeros para MATRIXB intenlalo nuevamente \n");
  } while (columns != lines2);

  A = (int **)malloc(lines * sizeof(int *));
  for (i = 0; i < lines; ++i)
    A[i] = (int *)malloc(columns * sizeof(int));

  B = (int **)malloc(lines2 * sizeof(int *));
  for (i = 0; i < lines2; ++i)
    B[i] = (int *)malloc(columns2 * sizeof(int));

  lines3 = lines;
  columns3 = columns2;
  C = (int **)malloc(lines3 * sizeof(int *));
  for (i = 0; i < lines3; ++i)
    C[i] = (int *)malloc(columns3 * sizeof(int));

  for (i = 0; i < lines; ++i)
  {
    for (j = 0; j < columns; ++j)
    {
      r = rand() % 6;
      A[i][j] = r;
    }
  }

  for (i = 0; i < lines2; ++i)
  {
    for (j = 0; j < columns2; ++j)
    {
      r = rand() % 6;
      B[i][j] = r;
    }
  }
  int DivOfThr, CantThr, PtrThr = 0, FinThr, num[NUM_THREADS];
  pthread_t workers[NUM_THREADS];
  gettimeofday(&start, 0);

  if (columns2 > lines)
  {
    CantThr = columns2;
    int TempNumberThreads = NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
      DivOfThr = ((CantThr) + (TempNumberThreads - 1)) / TempNumberThreads;
      num[i] = DivOfThr;
      CantThr = CantThr - DivOfThr;

      if (TempNumberThreads > 1)
      {
        TempNumberThreads--;
      }
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
      if (num[i])
      {
        FinThr = PtrThr + num[i];
        struct CData *dataMatrixC = (struct CData *)malloc(sizeof(struct CData));
        dataMatrixC->A = A;
        dataMatrixC->B = B;
        dataMatrixC->C = C;
        dataMatrixC->lines3 = 0;
        dataMatrixC->columns3 = lines;
        dataMatrixC->columns = columns;
        dataMatrixC->divATHR = PtrThr;
        dataMatrixC->DivOt = FinThr;
        pthread_create(&workers[i], NULL, Cmatrix, (void *)dataMatrixC);
        PtrThr = PtrThr + num[i];
      }
    }
  }
  else
  {
    CantThr = lines;
    int TempNumberThreads = NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
      DivOfThr = ((CantThr) + (TempNumberThreads - 1)) / TempNumberThreads;
      num[i] = DivOfThr;
      CantThr = CantThr - DivOfThr;

      if (TempNumberThreads > 1)
      {
        TempNumberThreads--;
      }
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
      if (num[i])
      {
        FinThr = PtrThr + num[i];
        struct CData *dataMatrixC = (struct CData *)malloc(sizeof(struct CData));
        dataMatrixC->A = A;
        dataMatrixC->B = B;
        dataMatrixC->C = C;
        dataMatrixC->lines3 = PtrThr;
        dataMatrixC->columns3 = FinThr;
        dataMatrixC->columns = columns;
        dataMatrixC->divATHR = 0;
        dataMatrixC->DivOt = columns2;
        pthread_create(&workers[i], NULL, Cmatrix, (void *)dataMatrixC);
        PtrThr = PtrThr + num[i];
      }
    }
  }
  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(workers[i], NULL);
  }

  gettimeofday(&finish, 0);

  compTime = (finish.tv_sec - start.tv_sec) * 1000000;
  compTime = compTime + (finish.tv_usec - start.tv_usec);
  Time = compTime / 1000000.0;

  printf("Time execution: %f", Time);
  return 0;
}

void *Cmatrix(void *data)
{
  struct CData *SetData = (struct CData *)data;

  int i, j;
  for (i = SetData->lines3; i < SetData->columns3; i++)
  {
    for (int k = SetData->divATHR; k < SetData->DivOt; k++)
    {
      SetData->C[i][k] = 0;
      for (j = 0; j < SetData->columns; j++)
      {
        SetData->C[i][k] += SetData->A[i][j] * SetData->B[j][k];
      }
    }
  }
  pthread_exit(NULL);
}