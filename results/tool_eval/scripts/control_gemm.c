/* Array-only float smoke test for the StreamHLS C/C++ integration.
 * This is not a ForgeBench design and is never counted in its result tables.
 */
void test_gemm(float A[32][32], float B[32][32], float C[32][32]) {
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      float acc = 0.0f;
      for (int k = 0; k < 32; ++k)
        acc += A[i][k] * B[k][j];
      C[i][j] = acc;
    }
  }
}
