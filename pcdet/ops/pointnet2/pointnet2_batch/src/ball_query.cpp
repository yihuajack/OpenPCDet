/*
batch version of ball query, modified from the original implementation of official PointNet++ codes.
Written by Shaoshuai Shi
All Rights Reserved 2018.
*/


#include <torch/serialize/tensor.h>
#include <vector>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include "ball_query_gpu.h"

#if TORCH_MAJOR_VERSION == 1 && TORCH_MINOR_VERSION <= 10
#include <THC/THC.h>
extern THCState *state;
#endif

#define CHECK_CUDA(x) do { \
	  if (!x.type().is_cuda()) { \
		      fprintf(stderr, "%s must be CUDA tensor at %s:%d\n", #x, __FILE__, __LINE__); \
		      exit(-1); \
		    } \
} while (0)
#define CHECK_CONTIGUOUS(x) do { \
	  if (!x.is_contiguous()) { \
		      fprintf(stderr, "%s must be contiguous tensor at %s:%d\n", #x, __FILE__, __LINE__); \
		      exit(-1); \
		    } \
} while (0)
#define CHECK_INPUT(x) CHECK_CUDA(x);CHECK_CONTIGUOUS(x)


int ball_query_wrapper_fast(int b, int n, int m, float radius, int nsample, 
    at::Tensor new_xyz_tensor, at::Tensor xyz_tensor, at::Tensor idx_tensor) {
    CHECK_INPUT(new_xyz_tensor);
    CHECK_INPUT(xyz_tensor);
    const float *new_xyz = new_xyz_tensor.data<float>();
    const float *xyz = xyz_tensor.data<float>();
    int *idx = idx_tensor.data<int>();
    
    ball_query_kernel_launcher_fast(b, n, m, radius, nsample, new_xyz, xyz, idx);
    return 1;
}
