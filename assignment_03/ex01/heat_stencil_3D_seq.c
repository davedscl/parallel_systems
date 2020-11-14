#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef double value_t;

#define RESOLUTION 100

// -- Matrix utilities --

typedef value_t ***Matrix;

Matrix create3DMatrix(int N);

void releaseMatrix(Matrix m, int N);

// -- simulation code ---

int main(int argc, char **argv){
  // 'parsing' optional input parameter = problem size
  int N = 200; // rows x columns

  if (argc > 1) {
    N = atoi(argv[1]);
  }
  int T = N * 100;
  printf("Computing heat-distribution for room size N=%dx%dx%d, for T=%d timesteps\n", N, N, N, T);

  // ---------- setup ----------

  // create a buffer for storing temperature fields
  Matrix A = create3DMatrix(N);
  releaseMatrix(A, N);
  return 0;
  /*// set up initial conditions in A
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] = 273; // temperature is 0° C everywhere (273 K)
    }
  }

  // and there is a heat source in one corner
  int source_x = N / 4;
  int source_y = N / 8;
  A[source_y][source_x] = 273 + 60;

  printf("Initial:\t");
  printTemperature(A, N);
  printf("\n");

  // ---------- compute ----------

  // create a second buffer for the computation
  Matrix B = create3DMatrix(N);

  // for each time step ..
  for (int t = 0; t < T; t++) {
    // .. we propagate the temperature
    for (long long i = 0; i < N; i++) {
      for (long long j = 0; j < N; j++) {
        // center stays constant (the heat is still on)
        if (i == source_y && j == source_x) {
          B[i][j] = A[i][j];
          continue;
        }

        // get temperature at current position
        value_t tc = A[i][j];

        // get temperatures of adjacent cells
        value_t tu = (j != 0) ? A[i][j - 1] : tc;
        value_t ta = (j != N - 1) ? A[i][j + 1] : tc;
        value_t tl = (i != 0) ? A[i - 1][j] : tc;
        value_t tr = (i != N - 1) ? A[i + 1][j] : tc;

        // compute new temperature at current position
        B[i][j] = 0.25 * (tl + tr + tu + ta);
      }
    }

    // swap matrices (just pointers, not content)
    Matrix H = A;
    A = B;
    B = H;

    // show intermediate step
    if (!(t % 1000)) {
      printf("Step t=%d:\n", t);
      printTemperature(A, N);
      printf("\n");
    }
  }

  releaseMatrix(B);

  // ---------- check ----------
  printf("Final:\t\t\n");
  printTemperature(A, N);
  printf("\n");

  FILE *fp;

  fp = fopen("2D-output-seq.dat", "w");
  fprintf(fp, "%d\n", N);


  int success = 1;
  for (long long i = 0; i < N; i++) {
    for (long long j = 0; j < N; j++) {
      value_t temp = A[i][j];
      fprintf(fp, "%f\n", temp);
      if (273 <= temp && temp <= 273 + 60)
        continue;
      success = 0;
      break;
    }
  }

  fclose(fp);

  printf("Verification: %s\n", (success) ? "OK" : "FAILED");

  // ---------- cleanup ----------

  releaseMatrix(A);

  // done
  return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
*/
}

Matrix create3DMatrix(int N) {
  // create data and index Matrix
  value_t ***mat = malloc(sizeof(value_t) * N);
  for (int i = 0; i < N; i++) {
    mat[i] = malloc(sizeof(value_t*) * N);
    for (int j = 0; j < N; j++) {
      mat[i][j] = malloc(sizeof(value_t) * N);
    }
  }
  return mat;
}

void releaseMatrix(Matrix m, int N) { 
  for(int i=0; i < N; i++){
    for(int j=0; j < N; j++){
      free(m[i][j]);
    }
    free(m[i]); 
  }
  free(m);
}

