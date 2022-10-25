//#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <curand_kernel.h>//��������� ������
#include <cuda.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <time.h>

using namespace std;
//__global__ - ���������� CPU, �� ����������� GPU\
__device__ - ����������� �� GPU\
__host__ - ����������� �� CPU

//����������, � ������� ����� ��������� ��������� ������� max_in_column()\
����� ���� ������� ������ �� GPU
__device__ int max_element_index;

//����� ������������� �������� � �������, ����������� �� GPU
__global__ void max_in_column(double* matrix, int matrix_size, int x)
{
	int max_index = x;
	double max_value = fabs(matrix[x * matrix_size + x]);
	double current_value;
	for (int i = x + 1; i < matrix_size; i++)
	{
		//���������� �� ������ ������ ���
		current_value = fabs(matrix[i * matrix_size + x]);//fabs - ������ ������
		if (current_value > max_value)
		{
			max_index = i;
			max_value = current_value;
		}
	}
	max_element_index = max_index;//��������� ������������ ������ �������
}

__global__ void swap_lines(double* matrix, double* identity, int matrix_size, int row) {
	//���� ��������� ������ � ��� �� ����� �����, �� ������ ������ �� �����
	if (row == max_element_index)
		return;
	//������ �����(���������� ������� � �����)*����� �����+����� ������
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= matrix_size) return;//���������, ��� �� ������� �� ������� �������
	double temp;
	temp = matrix[row * matrix_size + idx];//��������� ������� ������
	//������ �� ����� ����� �������� ��������������� ��� ������� ������ ������
	matrix[row * matrix_size + idx] = matrix[max_element_index * matrix_size + idx];
	//������ ����������� ������� �� ��������������� ��� ����� � ������ ������
	matrix[max_element_index * matrix_size + idx] = temp;

	//������ ����������� ��������, �� � ������ ��������
	temp = identity[row * matrix_size + idx];
	identity[row * matrix_size + idx] = identity[max_element_index * matrix_size + idx];
	identity[max_element_index * matrix_size + idx] = temp;
}

__global__ void make_zero_below(double* matrix, double* identity, int matrix_size, int x)
{
	//������ ����� ������
	int idx = threadIdx.x;
	int i, j;
	
	//���� ��� ������, ��� ������� ���� �������, �� ������ �� ��������, ���� �� ���� ������ �������� ������
	if (idx * matrix_size == x * matrix_size) return;
	
	//������� �����������
	double 	coeff = matrix[idx * matrix_size + x] / matrix[x * matrix_size + x];

	//�������� ������ x � �������� matrix[x][x] �� ����� ������ � �� ������� �������� ��������\
	��������������� ������� ������ x, ���������� �� coeff
	for (i = x;i < matrix_size;i++)
		matrix[idx * matrix_size + i] -= coeff* matrix[x * matrix_size + i];
	
	//������ ���� ����� ��� ������ ����� ����������� �������
	for (;i < 2*matrix_size;i++)
		identity[idx * matrix_size + i] -= coeff * identity[x * matrix_size + i];


}

void print_both(double* matrix, double* identity_matrix, int matrix_size);
void print(double* matrix, int n);

__host__ int main()
{
	//���� ������� �������
	cout << "Enter the size of the matrix: ";
	int matrix_size;
	cin>> matrix_size;

	//���������� �������
	//��������� ������ ��� ������ �� CPU
	double* matrix = new double[matrix_size * matrix_size];
	assert(matrix != 0);
	double* identity_matrix = new double[matrix_size * matrix_size];
	assert(identity_matrix != 0);

	//��������� ������� ���������� �������
	srand(time(NULL));
	for (int i = 0; i < matrix_size * matrix_size; i++)
		matrix[i] = 1 + rand() % 10;

	//��������� ��������� �������
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++)
			identity_matrix[i * matrix_size + j] = (i == j ? 1.0 : 0.0);

	cout<< "Before:"<< endl;
	print_both(matrix, identity_matrix, matrix_size);

	//��������� ������ ��� ������ �� GPU
	double* dev_matrix;
	double* dev_identity;
	cudaMalloc((void**)&dev_matrix, sizeof(double) * matrix_size * matrix_size);
	cudaMalloc((void**)&dev_identity, sizeof(double) * matrix_size * matrix_size);

	//�������� � host �� device
	cudaMemcpy(dev_matrix, matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_identity, identity_matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyHostToDevice);

	//�������������� ���������� ��� ������ ������� ����� �� GPU
	float recording;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	//�� ������ ����� ��� ������ � ����� ���������� � ���������������\
	� ������� ������������ �������������� ������� ������� ��� ������� ����������
	//int i = 0;
	for (int i = 0; i < matrix_size; i++)
	{
	// �������� ������ �� ������� � ���������� ������� ��������� � ��������
	// �������� ��� ���
	
	//���� ������������ ������� �������
	max_in_column<< <1, 1 >> > (dev_matrix, matrix_size, i);
	
	//������ i-�� ������ �� ������� � ������������ ��������� � i-�� ������� �������
	swap_lines << <1, matrix_size>> >(dev_matrix, dev_identity, matrix_size, i);

	//�������� �������� ��� i-�� � i-�� �������
	make_zero_below << < 1, matrix_size-i >> > (dev_matrix, dev_identity, matrix_size, i);
	
	cudaMemcpy(matrix, dev_matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);
	cudaMemcpy(identity_matrix, dev_identity, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);

	cout << "After:" << endl;
	print_both(matrix, identity_matrix, matrix_size);
	//���-��
	/*nullify_below << <BLOCKS_1D, THREADS_1D >> > (dev_matrix, n, i);*/
	}
	cudaThreadSynchronize();//������������� �������

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&recording, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	//�������� ��������� �� ������ GPU �� CPU
	cudaMemcpy(matrix, dev_matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);
	cudaMemcpy(identity_matrix, dev_identity, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);

	cout << "After:" << endl;
	print_both(matrix, identity_matrix, matrix_size);

	//����� �������� �������
	cout << "Invers matrix:" << endl;
	print(identity_matrix, matrix_size);

	cout << "Time of work: " << fixed << recording << endl;

	//����������� ������ device
	cudaFree(dev_matrix);
	cudaFree(dev_identity);

	//����������� ������ host
	delete[] matrix;
	delete[] identity_matrix;
	return 0;
}

__host__ void print_both(double* matrix, double* identity_matrix, int matrix_size)
{
	for (int i = 0; i < matrix_size; i++)
	{
		int flag = 1;
		for (int j = 0; j < 2 * matrix_size; j++)
			if (j < matrix_size)
				cout <<fixed << matrix[i * matrix_size + j] << "\t";
			else
			{
				if (flag)
				{
					cout << "|\t";
					flag--;
				}
				cout << fixed << identity_matrix[i * matrix_size + j - matrix_size] << "\t";
			}
		cout << endl;
	}
	cout << endl;
}

__host__ void print(double* matrix, int n)
{
	cout << scientific;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << fixed << matrix[i * n + j] << "\t";
		cout << endl;
	}
}
