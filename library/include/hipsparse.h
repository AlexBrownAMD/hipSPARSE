/* ************************************************************************
 * Copyright 2018 Advanced Micro Devices, Inc.
 * ************************************************************************ */

//! HIP = Heterogeneous-compute Interface for Portability
//!
//! Define a extremely thin runtime layer that allows source code to be compiled
//! unmodified through either AMD HCC or NVCC. Key features tend to be in the spirit
//! and terminology of CUDA, but with a portable path to other accelerators as well.
//!
//! This is the master include file for hipSPARSE, wrapping around rocSPARSE and
//! cuSPARSE "version 2".
//

#pragma once
#ifndef HIPSPARSE_H
#define HIPSPARSE_H

#include "hipsparse-export.h"
#include "hipsparse-version.h"

#include <hip/hip_runtime_api.h>

typedef void* hipsparseHandle_t;
typedef void* hipsparseMatDescr_t;
typedef void* hipsparseHybMat_t;

/* hipSPARSE status types */
typedef enum {
    HIPSPARSE_STATUS_SUCCESS                   = 0, // Function succeeds
    HIPSPARSE_STATUS_NOT_INITIALIZED           = 1, // hipSPARSE was not initialized
    HIPSPARSE_STATUS_ALLOC_FAILED              = 2, // Resource allocation failed
    HIPSPARSE_STATUS_INVALID_VALUE             = 3, // Unsupported value was passed to the function
    HIPSPARSE_STATUS_ARCH_MISMATCH             = 4, // Device architecture not supported
    HIPSPARSE_STATUS_MAPPING_ERROR             = 5, // Access to GPU memory space failed
    HIPSPARSE_STATUS_EXECUTION_FAILED          = 6, // GPU program failed to execute
    HIPSPARSE_STATUS_INTERNAL_ERROR            = 7, // An internal hipSPARSE operation failed
    HIPSPARSE_STATUS_MATRIX_TYPE_NOT_SUPPORTED = 8, // Matrix type not supported
    HIPSPARSE_STATUS_ZERO_PIVOT                = 9  // Zero pivot was computed
} hipsparseStatus_t;

/* Types definitions */
typedef enum {
    HIPSPARSE_POINTER_MODE_HOST   = 0,
    HIPSPARSE_POINTER_MODE_DEVICE = 1
} hipsparsePointerMode_t;

typedef enum {
    HIPSPARSE_ACTION_SYMBOLIC = 0,
    HIPSPARSE_ACTION_NUMERIC  = 1
} hipsparseAction_t;

typedef enum {
    HIPSPARSE_MATRIX_TYPE_GENERAL    = 0,
    HIPSPARSE_MATRIX_TYPE_SYMMETRIC  = 1,
    HIPSPARSE_MATRIX_TYPE_HERMITIAN  = 2,
    HIPSPARSE_MATRIX_TYPE_TRIANGULAR = 3
} hipsparseMatrixType_t;

typedef enum {
    HIPSPARSE_FILL_MODE_LOWER = 0,
    HIPSPARSE_FILL_MODE_UPPER = 1
} hipsparseFillMode_t;

typedef enum {
    HIPSPARSE_DIAG_TYPE_NON_UNIT = 0,
    HIPSPARSE_DIAG_TYPE_UNIT     = 1
} hipsparseDiagType_t;

typedef enum {
    HIPSPARSE_INDEX_BASE_ZERO = 0,
    HIPSPARSE_INDEX_BASE_ONE  = 1
} hipsparseIndexBase_t;

typedef enum {
    HIPSPARSE_OPERATION_NON_TRANSPOSE       = 0,
    HIPSPARSE_OPERATION_TRANSPOSE           = 1,
    HIPSPARSE_OPERATION_CONJUGATE_TRANSPOSE = 2
} hipsparseOperation_t;

typedef enum {
    HIPSPARSE_HYB_PARTITION_AUTO = 0,
    HIPSPARSE_HYB_PARTITION_USER = 1,
    HIPSPARSE_HYB_PARTITION_MAX  = 2
} hipsparseHybPartition_t;

typedef enum {
    HIPSPARSE_SIDE_LEFT  = 0,
    HIPSPARSE_SIDE_RIGHT = 1
} hipsparseSideMode_t;

#ifdef __cplusplus
extern "C" {
#endif

/* hipSPARSE initialization and management routines */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseCreate(hipsparseHandle_t* handle);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDestroy(hipsparseHandle_t handle);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseGetVersion(hipsparseHandle_t handle, int* version);
// HIPSPARSE_EXPORT
// hipsparseStatus_t hipsparseGetProperty(libraryPropertyType type,
//                                       int *value);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSetStream(hipsparseHandle_t handle, hipStream_t streamId);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseGetStream(hipsparseHandle_t handle, hipStream_t* streamId);

/* hipSPARSE type creation, destruction, set and get routines */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSetPointerMode(hipsparseHandle_t handle,
                                          hipsparsePointerMode_t mode);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseGetPointerMode(hipsparseHandle_t handle,
                                          hipsparsePointerMode_t* mode);

HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseCreateMatDescr(hipsparseMatDescr_t* descrA);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDestroyMatDescr(hipsparseMatDescr_t descrA);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseCopyMatDescr(hipsparseMatDescr_t dest,
                                        const hipsparseMatDescr_t src);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSetMatType(hipsparseMatDescr_t descrA,
                                      hipsparseMatrixType_t type);
HIPSPARSE_EXPORT
hipsparseMatrixType_t hipsparseGetMatType(const hipsparseMatDescr_t descrA);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSetMatFillMode(hipsparseMatDescr_t descrA,
                                          hipsparseFillMode_t fillMode);
HIPSPARSE_EXPORT
hipsparseFillMode_t hipsparseGetMatFillMode(const hipsparseMatDescr_t descrA);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSetMatDiagType(hipsparseMatDescr_t descrA,
                                          hipsparseDiagType_t diagType);
HIPSPARSE_EXPORT
hipsparseDiagType_t hipsparseGetMatDiagType(const hipsparseMatDescr_t descrA);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSetMatIndexBase(hipsparseMatDescr_t descrA,
                                           hipsparseIndexBase_t base);
HIPSPARSE_EXPORT
hipsparseIndexBase_t hipsparseGetMatIndexBase(const hipsparseMatDescr_t descrA);

/* Hybrid (HYB) format */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseCreateHybMat(hipsparseHybMat_t* hybA);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDestroyHybMat(hipsparseHybMat_t hybA);

/* --- Sparse Level 1 routines --- */

/* Description: Addition of a scalar multiple of a sparse vector x
   and a dense vector y. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSaxpyi(hipsparseHandle_t handle,
                                  int nnz,
                                  const float* alpha,
                                  const float* xVal,
                                  const int* xInd,
                                  float* y,
                                  hipsparseIndexBase_t idxBase);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDaxpyi(hipsparseHandle_t handle,
                                  int nnz,
                                  const double* alpha,
                                  const double* xVal,
                                  const int* xInd,
                                  double* y,
                                  hipsparseIndexBase_t idxBase);

/* --- Sparse Level 2 routines --- */

/* Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y,
   where A is a sparse matrix in CSR storage format, x and y are dense vectors. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseScsrmv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  int m,
                                  int n,
                                  int nnz,
                                  const float* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const float* csrSortedValA,
                                  const int* csrSortedRowPtrA,
                                  const int* csrSortedColIndA,
                                  const float* x,
                                  const float* beta,
                                  float* y);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDcsrmv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  int m,
                                  int n,
                                  int nnz,
                                  const double* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const double* csrSortedValA,
                                  const int* csrSortedRowPtrA,
                                  const int* csrSortedColIndA,
                                  const double* x,
                                  const double* beta,
                                  double* y);

/* Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y,
   where A is a sparse matrix in COO storage format, x and y are dense vectors. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseScoomv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  int m,
                                  int n,
                                  int nnz,
                                  const float* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const float* cooSortedValA,
                                  const int* cooSortedRowIndA,
                                  const int* cooSortedColIndA,
                                  const float* x,
                                  const float* beta,
                                  float* y);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDcoomv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  int m,
                                  int n,
                                  int nnz,
                                  const double* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const double* cooSortedValA,
                                  const int* cooSortedRowIndA,
                                  const int* cooSortedColIndA,
                                  const double* x,
                                  const double* beta,
                                  double* y);

/* Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y,
   where A is a sparse matrix in ELL storage format, x and y are dense vectors. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseSellmv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  int m,
                                  int n,
                                  const float* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const float* ellValA,
                                  const int* ellColIndA,
                                  int ellWidth,
                                  const float* x,
                                  const float* beta,
                                  float* y);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDellmv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  int m,
                                  int n,
                                  const double* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const double* ellValA,
                                  const int* ellColIndA,
                                  int ellWidth,
                                  const double* x,
                                  const double* beta,
                                  double* y);

/* Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y,
   where A is a sparse matrix in HYB storage format, x and y are dense vectors. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseShybmv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  const float* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const hipsparseHybMat_t hybA,
                                  const float* x,
                                  const float* beta,
                                  float* y);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDhybmv(hipsparseHandle_t handle,
                                  hipsparseOperation_t transA,
                                  const double* alpha,
                                  const hipsparseMatDescr_t descrA,
                                  const hipsparseHybMat_t hybA,
                                  const double* x,
                                  const double* beta,
                                  double* y);

/* --- Sparse Format Conversion --- */

/* Description: This routine converts a sparse matrix in CSR storage format
   to a sparse matrix in COO storage format. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseXcsr2coo(hipsparseHandle_t handle,
                                    const int* csrRowPtr,
                                    int nnz,
                                    int m,
                                    int* cooRowInd,
                                    hipsparseIndexBase_t idxBase);

/* Description: This routine computes the number of ELL entries per row
   (ell_width) from a given CSR matrix. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseXcsr2ellWidth(hipsparseHandle_t handle,
                                         int m,
                                         const hipsparseMatDescr_t descrA,
                                         const int* csrRowPtrA,
                                         const hipsparseMatDescr_t descrC,
                                         int* ellWidthC);

/* Description: This routine converts a sparse matrix in CSR storage format
   to a sparse matrix in ELL storage format. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseScsr2ell(hipsparseHandle_t handle,
                                    int m,
                                    const hipsparseMatDescr_t descrA,
                                    const float* csrValA,
                                    const int* csrRowPtrA,
                                    const int* csrColIndA,
                                    const hipsparseMatDescr_t descrC,
                                    int ellWidthC,
                                    float* ellValC,
                                    int* ellColIndC);

HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDcsr2ell(hipsparseHandle_t handle,
                                    int m,
                                    const hipsparseMatDescr_t descrA,
                                    const double* csrValA,
                                    const int* csrRowPtrA,
                                    const int* csrColIndA,
                                    const hipsparseMatDescr_t descrC,
                                    int ellWidthC,
                                    double* ellValC,
                                    int* ellColIndC);

/* Description: This routine converts a sparse matrix in CSR storage format
   to a sparse matrix in HYB storage format. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseScsr2hyb(hipsparseHandle_t handle,
                                    int m,
                                    int n,
                                    const hipsparseMatDescr_t descrA,
                                    const float* csrSortedValA,
                                    const int* csrSortedRowPtrA,
                                    const int* csrSortedColIndA,
                                    hipsparseHybMat_t hybA,
                                    int userEllWidth,
                                    hipsparseHybPartition_t partitionType);
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseDcsr2hyb(hipsparseHandle_t handle,
                                    int m,
                                    int n,
                                    const hipsparseMatDescr_t descrA,
                                    const double* csrSortedValA,
                                    const int* csrSortedRowPtrA,
                                    const int* csrSortedColIndA,
                                    hipsparseHybMat_t hybA,
                                    int userEllWidth,
                                    hipsparseHybPartition_t partitionType);

/* Description: This routine converts a sparse matrix in COO storage format
   to a sparse matrix in CSR storage format. */
HIPSPARSE_EXPORT
hipsparseStatus_t hipsparseXcoo2csr(hipsparseHandle_t handle,
                                    const int* cooRowInd,
                                    int nnz,
                                    int m,
                                    int* csrRowPtr,
                                    hipsparseIndexBase_t idxBase);

#ifdef __cplusplus
}
#endif

#endif // HIPSPARSE_H