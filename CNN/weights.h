#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_CONV1D_KERNEL_0 {-41, -1, -9, -57, -10, 9, -36, 28, 16, 68, 7, -36, -36, 12, 6, -43, 18, -59, -26, -16, 27, -17, 67, 18, -5, 61, -30, -48, 67, 51, 20, 10, 65, -6, 52, 64, 7, 12, -92, 74, -51, -99, 62, 39, 8, -68, -65, -32, -55, 41, -55, -43, -20, 0, 48, -6, 20, 37, 82, 8, -14, 47, -43, -52, -56, -25, 0, -27, 26, 46, 16, 88, 45, 78, -16, 84, 17, -62, 23, 82, 36, -3, -12, -32, -29, -4, -28, 29, 17, 3, -39, 13, 5, -12, -30, -41, -1, -50, -67, 67, 85, 61, 75, -16, -24, 23, 84, 63, -62, 8, -2, -66, -80, -48, -56, -84, -18, 79, -2, -48, 28, -46, -16, 91, 58, 50, -8, 69, -77, -86, 47, -75, -47, -16, -10, 74, -57, 22, 53, -31, -18, 58, -66, 7, -37, 53, 69, 22, -34, 30, -56, -52, 27, 36, -74, 33, -48, -8, 20, -30, -12, 7, -83, -21, 72, -72, 27, 70, -56, -69, -2, -50, 60, -74, -38, 51, -9, -59, 10, 44, 26, -44, 60, -34, 68, -21, -48, 60, 71, 35, 59, 56, 93, 26, 19, 12, 41, 25, -19, 52, -39, 56, 54, -60, -45, -26, -10, 32, -56, 80, 38, -44, 57, -27, -8, -15, -22, 3, -38, -17, -48, -59, 63, -62, -87, 15, -74, 14, 48, -40, 19, 57, -76, 19, 72, 41, -58, 75, -54, 8, 54, -3, -69, 21, -18, -20, -22, 16, -86, 75, 5, 3, -25, -5, 11, 86, 72, 1, 49, 88, -19, 36, 48, -65, -45, -39, -80, 35, -23, 11}

#define TENSOR_CONV1D_KERNEL_0_DEC_BITS {8}

#define TENSOR_CONV1D_BIAS_0 {97, 57, 79, 103, 36, 15, 39, 80, 46, -4, -2, 38, 63, 57, 30, 23, 20, 55, 11, 24, 53, 18, 15, -3, 44, 109, 40, 18, 30, 3}

#define TENSOR_CONV1D_BIAS_0_DEC_BITS {9}

#define CONV1D_BIAS_LSHIFT {3}

#define CONV1D_OUTPUT_RSHIFT {8}

#define TENSOR_CONV1D_1_KERNEL_0 {-56, -23, 16, 14, 89, 35, 14, -20, 46, 49, 3, 1, -18, 44, -16, -23, -25, 63, 7, -40, 12, 31, -10, -11, -37, 16, 36, 71, 21, 46, 29, 42, -66, -48, 52, -67, 46, -28, 29, -46, 34, -30, -22, 37, -29, -33, 35, -7, 46, -28, -36, 63, 38, 64, 48, 67, 62, 52, 40, 2, -11, 32, -46, 56, 10, 8, 46, 16, 39, -16, 24, 12, 44, 48, -6, 62, 41, 62, -26, -44, 75, 16, 24, 62, -49, 11, 22, 50, 43, -7, 21, 42, -2, 35, -37, 33, 68, -56, 5, -18, -10, 25, 68, -29, 63, -4, -17, 32, 7, 10, -21, 38, -6, -41, -9, 10, -52, -31, 18, 27, 59, -38, 59, 6, -46, 75, 38, -33, -17, -40, 10, 38, 54, -38, 8, 19, -22, 10, -18, 49, -35, 21, -36, -12, 19, -25, -27, -6, 44, -53, 69, 17, 76, 27, -30, 41, 8, 24, 77, 21, 73, 51, 25, -29, -7, 14, 42, -8, 45, 80, 59, -54, -36, -53, 37, -28, -33, -9, 64, -22, 29, -13, -51, -47, 11, 0, 48, 12, 52, -9, -31, -1, -47, -33, 63, 18, -43, 42, 56, 15, -50, -5, 52, -38, 46, -50, 10, 43, -21, 23, 44, 30, -29, -32, 34, -2, -4, -38, -60, 36, 56, 12, 57, -46, 55, 23, -18, 2, -41, 40, -51, -31, 2, -14, -15, 16, 25, 13, -20, -13, -31, 20, -30, 25, -57, 45, -27, 30, -41, 16, 2, 0, -9, -23, 27, 31, -15, 23, 34, -41, -63, 34, -44, 26, -9, -61, 21, 6, -81, -3, -1, -16, 20, -3, -3, 35, 9, 20, 32, 30, 52, 36, 46, 8, 61, -34, -43, 32, 40, 26, -38, 70, -7, -31, -27, -44, -2, 19, 40, 1, -16, -41, -41, 51, 62, -26, 30, 12, 19, -14, 66, 69, 61, 7, 19, -52, -25, 53, -27, 4, 22, -18, -35, 38, 39, -64, 22, -24, -35, 33, 34, -10, 47, 8, 20, 66, 19, -2, 46, -30, 24, 38, 67, 19, -15, 16, -39, -28, 0, 59, -29, -33, 44, 5, 0, -29, 17, -42, -7, -29, -61, -43, 30, 21, 54, -16, 25, -56, 75, 41, 16, 44, 34, 47, 6, 4, 29, -70, 8, 23, -47, 65, 19, -8, 2, -15, 20, -8, 5, 71, -27, 55, 39, -55, 64, -52, 79, 30, 70, -48, 33, 47, 19, -11, 10, 43, -9, 53, -40, -4, -60, -29, 35, -1, 47, -19, 0, 55, 47, -24, 40, -23, 25, 18, 2, 6, -28, 28, 13, -1, 39, -16, -62, 79, -38, 0, -6, -30, -35, 40, -5, -40, 34, 7, 56, 88, 13, 76, 4, -33, -18, -31, 31, -15, 56, -28, 13, 17, -33, -40, 25, 61, -15, -16, -26, 36, -29, -40, -3, -24, -17, 60, 38, 46, 62, 43, 4, 7, -52, -10, 1, -45, -18, 38, 41, -60, 0, -23, 59, 4, -26, -15, 44, -10, -71, -11, -57, -19, -30, 19, -4, 9, -65, -13, 32, 31, 62, 0, 39, 33, -55, 0, -10, 11, 45, 5, 20, 41, 65, 4, 17, 31, -40, 38, -15, 85, 22, 45, -17, 29, 14, 0, -23, -18, 3, 92, 63, -22, 44, 65, 54, -20, 57, -5, 19, 15, 31, 33, 30, -50, 57, 8, 52, 18, 51, -16, 59, 67, 70, 0, -5, 72, 33, 58, -25, 47, 59, 40, 60, 8, -5, -55, -14, 4, -6, -32, -24, -26, 29, -30, 5, 46, -27, 2, 13, 5, 35, 56, 18, -21, 65, 37, 3, 61, -54, -12, 26, -53, -10, -28, 17, -52, 12, 65, -32, 22, 28, 58, 54, 30, 25, 55, -4, 20, 45, -10, -12, 70, -32, -18, -20, -21, -30, 68, -53, -8, 10, -27, 48, -40, -33, 79, 35, -37, 75, 24, -53, -32, -5, 61, 66, -40, 35, -14, 12, 23, 14, -36, -25, 51, -26, -17, -15, 15, -8, -7, 56, 55, -35, 21, 23, 42, -61, -68, 38, 43, -66, -43, -23, 50, -10, 1, 7, -39, 82, 30, -36, -61, -5, 55, 4, 9, 8, -54, 88, -4, 56, 42, -26, -4, -42, -4, -68, -53, 33, 64, -28, -47, -30, 41, 43, -5, -21, 23, 17, 23, 56, -3, -2, -1, -9, -62, 25, -3, 36, 61, -21, -12, 19, -14, 45, 26, 27, 68, 41, 19, -29, 50, 58, 51, -28, -50, -21, 35, 59, 20, -23, -12, 42, -19, 2, -29, -9, 14, -4, 24, 53, -6, 12, 6, 61, 48, 3, -17, 14, -6, -2, 46, -49, -47, -41, -25, -45, -68, -15, -52, -12, 7, 19, 35, 64, 18, 11, -30, 52, -60, 11, 58, -49, 14, 31, 66, 38, 56, -45, -36, 13, 1, 31, 36, 55, -7, -30, -12, 64, 8, -35, 31, 1, 15, 64, -18, 72, 52, -38, -36, -8, -12, -13, 21, -27, 20, -43, 32, 49, 9, 8, -4, -32, -15, 9, -54, 16, -10, -55, -10, 25, 61, 10, -37, -39, -75, -35, 14, 55, 17, 60, 79, -37, 27, -35, -10, -6, 27, -10, 50, 5, -43, -4, -33, -34, -52, 63, -35, 21, -42, 47, 30, -12, -6, 30, -25, 41, 70, -22, 20, 40, -18, 13, 34, -24, 5, -14, -47, 66, 27, 9, 8, 20, 47, 11, -38, -35, 34, 57, 27, 28, -37, 22, 45, -37, 19, 37, 24, 9, 10, 23, 1, 44, -17, 64, 24, -7, 66, 64, 21, 2, -12, 3, -45, -10, 19, 50, 53, -33, 25, 63, 22, -24, 45, 5, 23, -14, 27, 12, 7, 27, -5, 53, -30, 53, 33, 64, 45, 48, -37, -31, 62, -30, 44, -12, 63, 6, 29, 3, 58, 45, -39, -19, 26, 68, 12, 47, 57, -11, 81, -32, 39, 56, 21, 11, -27, -48, 63, -23, 28, 2, 18, -15, 36, -26, 30, 41, 64, 1, 20, -5, -15, -16, -30, -10, 21, -46, 73, -30, 97, 51, 25, -41, -28, 41, 8, 24, -8, -29, 74, 68, -18, 43, -25, 40, -16, 40, -30, 12, 11, 1, 2, 10, 56, 35, -19, 45, -56, -28, -32, -58, -31, -37, -71, 58, -43, 21, -6, -54, -9, 27, 61, -14, -35, -26, -40, 67, 14, -2, -72, 27, 32, 75, 67, 63, -60, -32, -44, -43, 38, -88, 31, -8, -37, -12, 3, -19, 36, -45, -12, 53, 15, 58, -23, 5, -50, 37, 23, 5, -39, 50, 34, 65, 14, -18, -50, 8, 6, -16, -22, 13, -46, -14, -21, -38, 33, 14, -29, -60, -29, 7, 71, -15, -15, -22, 18, 69, 13, -46, 47, -17, 77, 9, -65, -6, 24, 31, 5, 1, 93, -29, -74, -30, -37, 57, -5, 49, 37, 19, -8, 23, 14, 19, 37, 48, -7, 68, -52, 0, -17, -8, 45, -46, 26, -51, -9, 26, 15, 50, 47, -47, 26, 12, -14, 53, 13, 32, 33, -52, 36, -4, 39, -23, -36, 44, 7, 74, -62, 47, -25, 3, 72, -40, -36, -19, 48, 1, -12, 8, 48, 22, 9, -45, 39, -56, -6, 53, 26, 26, 42, -14, 23, -4, -15, -4, 48, -46, 62, -2, 59, 28, -15, 1, -5, -33, 90, -27, -11, -57, 58, -2, 46, 90, 46, -43, -30, -7, 13, 1, -57, 60, 46, 38, 13, -60, 61, -44, 53, -57, 11, -11, 10, -25, 19, 43, 30, -11, 12, 37, 12, 74, 62, 32, -45, -34, 27, -15, -38, 30, -3, 10, -7, 53, 16, -25, 76, -34, -22, -17, 41, -22, -22, 43, 24, 4, -19, 45, 57, -29, 58, 9, -20, 13, 25, -37, -29, -31, 6, -35, 24, -80, 18, 13, -27, 50, -59, -24, -55, -1, -16, 22, -48, -13, 25, 50, 3, -12, 3, 42, 15, 48, 23, -50, 34, 44, 9, -46, 33, 79, 58, -43, -37, 18, -13, 53, -36, 28, 19, 25, -23, 28, -67, -56, 40, 0, 18, 25, 27, 80, -48, -38, 20, 17, 54, 60, 23, -13, -23, 84, 52, 30, 24, 12, 44, 32, 43, 18, 2, 78, -23, 28, -33, -34, -4, -16, 39, 13, -12, 25, -35, -26, 71, -52, 74}

#define TENSOR_CONV1D_1_KERNEL_0_DEC_BITS {8}

#define TENSOR_CONV1D_1_BIAS_0 {-30, 69, -25, -38, 27, 30, 24, 5, 10, 11, 42, 21, 83, -18, -7}

#define TENSOR_CONV1D_1_BIAS_0_DEC_BITS {9}

#define CONV1D_1_BIAS_LSHIFT {3}

#define CONV1D_1_OUTPUT_RSHIFT {9}

#define TENSOR_DENSE_KERNEL_0 {-38, 22, -30, 45, 9, -35, -16, 56, -105, 48, 21, -51, 57, 36, -6, -12, 3, 8, -21, 25, -81, -43, 8, -37, -27, -93, 87, -19, 31, -13, -54, -27, 76, 70, -22, -47, 18, -54, 67, 13, 28, -27, -79, 54, -65, -2, -47, 43, -36, -39, 94, -1, -106, -35, -6, 48, -22, -34, 41, -6, -10, -70, 6, -15, 28, -2, -21, -2, -51, -40, 22, 44, 34, 15, 62, -32, 32, -5, -46, -44, 10, -79, -22, 49, 38, -19, 7, 38, -38, -4, -28, -12, -54, -25, 3, 55, 71, 44, -12, 51, 65, -56, -35, 35, -51, 9, -51, 8, 29, -47, -41, 77, -93, 25, -42, -60, 1, -30, 52, -17, 15, 8, 46, -23, 6, 40, -42, -44, 8, 61, 25, 28, -4, -17, -66, -23, 70, -45, -31, 32, -44, 1, -59, 70, 61, -68, 42, 31, 60, 16, 42, -105, 27, 60, 28, -38, -16, 32, -21, -87, -38, -39, 51, -37, 45, 35, 14, -35, -7, -67, -20, -39, -15, -84, -45, 1, -58, 33, 33, 16, -16, 19, -77, 35, 12, -9, 18, -71, -43, -20, -20, 4, -53, -68, -49, -59, -67, 25, -72, 30, 19, 38, -54, -1, -67, 55, 51, 75, -16, -33, 38, -38, -48, -55, 86, 20, -70, 36, -39, -41, 14, -53, 70, 53, -6, -7, -84, -66, -1, -51, -17, 96, -27, -50, -86, 3, 1, -15, 56, 22, -6, -14, 1, 22, -112, -25, -33, -24, -16, -12, 3, -66, 48, -48, 15, -52, -3, -67, -88, 35, -8, -23, -76, 22, 0, 35, -10, -46, 14, -64, -19, 8, -48, 42, -53, 12, -38, 6, -73, -52, 8, -96, -54, 27, -39, -65, -9, -42, -1, -22, 24, -47, -84, 25, 6, -52, 1, 25, -11, -22, -52, -3, -44, -11, -28, -4, -2, -30, -23, -1, 69, -43, 27, -10, 72, -41, -70, -15, -18, -74, -51, -35, 41, -13, 33, 63, -11, -51, -59, -73, -41, -76, -44, -5, 58, -37, 12, -35, -47, 4, -30, 21, 82, -91, -16, 58, -35, -38, -48, -62, 70, -20, 3, 10, -37, 34, -59, 25, 33, 52, 28, -55, 32, 75, -50, 13, -16, 16, 9, 28, 36, 57, 55, -25, 11, -74, -72, -35, 52, 50, -14, -64, -70, -45, 55, -39, 31, 13, -60, -69, 38, 41, -47, 25, -52, -21, -30, 10, -45, 50, -49, 26, -23, 15, -51, 22, -30, 6, 24, 49, -35, -48, 69, -53, -43, 41, 9, 16, -50, 24, -88, 60, 62, -65, -86, -12, -23, 28, 26, -63, 23, -35, 57, -72, 2, -34, -37, -42, 41, 33, 6, 8, -65, 9, 3, -20, 64, -7, -7, 28, -76, -63, 51, 11, -32, -24, -92, -11, -12, 10, -37, -3, 11, 33, -70, -56, 21, -44, 24, 38, 53, 67, -38, -35, 40, 45, 23, -1, -30, 37, -22, -27, -28, -41, -73, -24, 5, 13, 81, -68, 51, 22, 28, 92, -67, -32, 43, 50, 44, -31, 32, -60, -13, 10, 36, -55, -41, 0, -32, 54, 6, 24, 36, -76, -10, 30, -70, -30, -21, -11, -62, 64, -53, 28, 59, -29, -39, 31, 46, 45, 45, -65, -69, 39, -66, 38, -28, -47, 51, -54, -28, 6, -92, 29, 31, 27, 39, 40, 34, -36, -55, 30, 11, -69, 28, 1, -35, -72, -5, 13, 19, -65, 68, -19, -5, 9, -39, -49, 9, -59, -94, -62, -42, -59, 35, 13, 4, -46, -64, -28, 10, -74, -58, 33, -37, -2, -12, -12, -9, -117, -71, -32, -66, -26, -5, -42, -42, -3, -50, -73, -83, 31, 46, -22, 41, 77, -35, 37, -50, -74, 42, -43, -40, -52, -91, -25, 1, 62, -72, 62, -6, 26, 35, -1, -117, 10, 62, -86, -9, 8, -64, -15, 52, -37, -25, 71, -13, -54, 5, -56, -48, -90, -34, -85, -19, -44, -39, 22, 15, -72, -34, 26, -41, 29, -54, 18, -90, 23, -26, 18, 13, -58, 22, 38, 53, -15, 9, 1, -80, -78, 16, -23, -20, -33, -55, -23, 1, -33, 61, 15, 0, 18, -12, -84, 62, 82, 13, 11, 6, -41, -8, -1, -9, -26, 24, -30, 30, -1, -45, 38, -63, 7, 32, -73, -55, -21, 18, -27, -56, -46, 24, -40, 27, 10, -43, -54, 36, -52, -4, 8, 12, 44, -54, 57, -18, -46, -20, -90, -72, 30, 14, -42, -3, -69, 47, 52, 23, -52, 41, -17, 15, -78, 38, 86, -30, -5, 4, -4, -36, -96, 49, -88, -90, -23, 5, -28, 40, 33, -11, 38, -19, 52, 47, 64, 36, 63, -55, -83, -10, -8, -66, -45, -84, -71, -25, -5, -59, -24, 29, 2, 58, 55, -50, -78, 23, -12, -35, -18, -60, 24, 54, 38, 81, -28, 9, 1, -25, 2, -9, -22, -32, -65, -66, -5, 48, -17, -23, 31, 68, -73, -2, 10, 8, 70, -60, 81, -68, -13, -36, -68, -3, -60, -43, 11, -100, -87, -65, -44, -8, 44, -64, -38, 74, -32, -3, -20, 40, 66, -18, -50, 86, -11, 13, 38, -2, -37, -17, -16, 34, 8, 76, -4, -92, 12, -6, -42, 40, -47, 22, -88, -11, 65, -19, 58, 43, 5, 79, 26, -65, 25, -74, -27, 39, -41, 78, -63, -14, -9, -69, 44, 38, -39, -27, -22, -1, 2, -11, 92, -51, -38, -102, 60, 92, 3, -23, 49, 66, -32, -42, 55, 8, 79, -17, 29, 46, -40, 9, -37, -20, -33, -37, 1, 0, 5, -46, -48, 5, 7, 29, 20, 30, 8, -4, 13, -62, 41, -4, 96, 26, -73, -102, -38, -19, -95, -81, -82, -43, -94, 36, 42, 3, -71, 42, -32, 8, -80, 74, -52, -17, -74, -20, 40, -34, 38, -6, 62, 27, -17, 9, -64, -21, 52, -78, -23, -10, -5, 16, -62, 76, -29, 47, 107, 73, -56, 77}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {8}

#define TENSOR_DENSE_BIAS_0 {18, -14, 14, -24, -9, -49, -42, -49, 29, -20, -38, 42, 93}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {9}

#define DENSE_BIAS_LSHIFT {2}

#define DENSE_OUTPUT_RSHIFT {9}


/* output q format for each layer */
#define INPUT_1_OUTPUT_DEC 4
#define INPUT_1_OUTPUT_OFFSET 0
#define CONV1D_OUTPUT_DEC 4
#define CONV1D_OUTPUT_OFFSET 0
#define RE_LU_OUTPUT_DEC 4
#define RE_LU_OUTPUT_OFFSET 0
#define CONV1D_1_OUTPUT_DEC 3
#define CONV1D_1_OUTPUT_OFFSET 0
#define RE_LU_1_OUTPUT_DEC 3
#define RE_LU_1_OUTPUT_OFFSET 0
#define MAX_POOLING1D_OUTPUT_DEC 3
#define MAX_POOLING1D_OUTPUT_OFFSET 0
#define FLATTEN_OUTPUT_DEC 3
#define FLATTEN_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 2
#define DENSE_OUTPUT_OFFSET 0
#define DROPOUT_OUTPUT_DEC 2
#define DROPOUT_OUTPUT_OFFSET 0
#define SOFTMAX_OUTPUT_DEC 7
#define SOFTMAX_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[450] = {0};

const nnom_shape_data_t tensor_input_1_dim[] = {150, 3};
const nnom_qformat_param_t tensor_input_1_dec[] = {4};
const nnom_qformat_param_t tensor_input_1_offset[] = {0};
const nnom_tensor_t tensor_input_1 = {
    .p_data = (void*)nnom_input_data,
    .dim = (nnom_shape_data_t*)tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};

const nnom_io_config_t input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t*)&tensor_input_1
};
const int8_t tensor_conv1d_kernel_0_data[] = TENSOR_CONV1D_KERNEL_0;

const nnom_shape_data_t tensor_conv1d_kernel_0_dim[] = {3, 3, 30};
const nnom_qformat_param_t tensor_conv1d_kernel_0_dec[] = TENSOR_CONV1D_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_kernel_0 = {
    .p_data = (void*)tensor_conv1d_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};
const int8_t tensor_conv1d_bias_0_data[] = TENSOR_CONV1D_BIAS_0;

const nnom_shape_data_t tensor_conv1d_bias_0_dim[] = {30};
const nnom_qformat_param_t tensor_conv1d_bias_0_dec[] = TENSOR_CONV1D_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_bias_0 = {
    .p_data = (void*)tensor_conv1d_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv1d_output_shift[] = CONV1D_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv1d_bias_shift[] = CONV1D_BIAS_LSHIFT;
const nnom_conv2d_config_t conv1d_config = {
    .super = {.name = "conv1d"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv1d_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv1d_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv1d_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv1d_bias_shift, 
    .filter_size = 30,
    .kernel_size = {3},
    .stride_size = {3},
    .padding_size = {0, 0},
    .dilation_size = {1},
    .padding_type = PADDING_VALID
};
const int8_t tensor_conv1d_1_kernel_0_data[] = TENSOR_CONV1D_1_KERNEL_0;

const nnom_shape_data_t tensor_conv1d_1_kernel_0_dim[] = {3, 30, 15};
const nnom_qformat_param_t tensor_conv1d_1_kernel_0_dec[] = TENSOR_CONV1D_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_1_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_1_kernel_0 = {
    .p_data = (void*)tensor_conv1d_1_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};
const int8_t tensor_conv1d_1_bias_0_data[] = TENSOR_CONV1D_1_BIAS_0;

const nnom_shape_data_t tensor_conv1d_1_bias_0_dim[] = {15};
const nnom_qformat_param_t tensor_conv1d_1_bias_0_dec[] = TENSOR_CONV1D_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_1_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_1_bias_0 = {
    .p_data = (void*)tensor_conv1d_1_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv1d_1_output_shift[] = CONV1D_1_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv1d_1_bias_shift[] = CONV1D_1_BIAS_LSHIFT;
const nnom_conv2d_config_t conv1d_1_config = {
    .super = {.name = "conv1d_1"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv1d_1_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv1d_1_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv1d_1_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv1d_1_bias_shift, 
    .filter_size = 15,
    .kernel_size = {3},
    .stride_size = {3},
    .padding_size = {0, 0},
    .dilation_size = {1},
    .padding_type = PADDING_VALID
};

const nnom_pool_config_t max_pooling1d_config = {
    .super = {.name = "max_pooling1d"},
    .padding_type = PADDING_VALID,
    .output_shift = 0,
    .kernel_size = {3},
    .stride_size = {3},
    .num_dim = 1
};

const nnom_flatten_config_t flatten_config = {
    .super = {.name = "flatten"}
};
const int8_t tensor_dense_kernel_0_data[] = TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {75, 13};
const nnom_qformat_param_t tensor_dense_kernel_0_dec[] = TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_kernel_0 = {
    .p_data = (void*)tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_bias_0_data[] = TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t tensor_dense_bias_0_dim[] = {13};
const nnom_qformat_param_t tensor_dense_bias_0_dec[] = TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_bias_0 = {
    .p_data = (void*)tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_output_shift[] = DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_bias_shift[] = DENSE_BIAS_LSHIFT;
const nnom_dense_config_t dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_bias_shift
};

const nnom_softmax_config_t softmax_config = {
    .super = {.name = "softmax"}
};
static int8_t nnom_output_data[13] = {0};

const nnom_shape_data_t tensor_output0_dim[] = {13};
const nnom_qformat_param_t tensor_output0_dec[] = {SOFTMAX_OUTPUT_DEC};
const nnom_qformat_param_t tensor_output0_offset[] = {0};
const nnom_tensor_t tensor_output0 = {
    .p_data = (void*)nnom_output_data,
    .dim = (nnom_shape_data_t*)tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t*)&tensor_output0
};
/* model version */
#define NNOM_MODEL_VERSION (10000*0 + 100*4 + 3)

/* nnom model */
static nnom_model_t* nnom_model_create(void)
{
	static nnom_model_t model;
	nnom_layer_t* layer[10];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&input_1_config);
	layer[1] = model.hook(conv2d_s(&conv1d_config), layer[0]);
	layer[2] = model.active(act_relu(), layer[1]);
	layer[3] = model.hook(conv2d_s(&conv1d_1_config), layer[2]);
	layer[4] = model.active(act_relu(), layer[3]);
	layer[5] = model.hook(maxpool_s(&max_pooling1d_config), layer[4]);
	layer[6] = model.hook(flatten_s(&flatten_config), layer[5]);
	layer[7] = model.hook(dense_s(&dense_config), layer[6]);
	layer[8] = model.hook(softmax_s(&softmax_config), layer[7]);
	layer[9] = model.hook(output_s(&output0_config), layer[8]);
	model_compile(&model, layer[0], layer[9]);
	return &model;
}
