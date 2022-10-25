//#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <curand_kernel.h>//выделение памяти
#include <cuda.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <time.h>

using namespace std;
//__global__ - вызывается CPU, но выполняется GPU\
__device__ - выполняется на GPU\
__host__ - выполняется на CPU

//переменная, в которой будет храниться результат функции max_in_column()\
может быть вызвана только из GPU
__device__ int max_element_index;

//поиск максимального элемента в столбце, исполняется на GPU
__global__ void max_in_column(double* matrix, int matrix_size, int x)
{
	int max_index = x;
	double max_value = fabs(matrix[x * matrix_size + x]);
	double current_value;
	for (int i = x + 1; i < matrix_size; i++)
	{
		//спускается на строку каждый раз
		current_value = fabs(matrix[i * matrix_size + x]);//fabs - взятие модуля
		if (current_value > max_value)
		{
			max_index = i;
			max_value = current_value;
		}
	}
	max_element_index = max_index;//сохраняем максимальный индекс столбца
}

__global__ void swap_lines(double* matrix, double* identity, int matrix_size, int row) {
	//если выбранная строка и так на своем месте, то ничего менять не будем
	if (row == max_element_index)
		return;
	//размер блока(количество потоков в блоке)*номер блока+номер потока
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= matrix_size) return;//проверяем, что не выходим за пределы массива
	double temp;
	temp = matrix[row * matrix_size + idx];//сохраняем элемент строки
	//ставим на место этого элемента соответствующий ему элемент другой строки
	matrix[row * matrix_size + idx] = matrix[max_element_index * matrix_size + idx];
	//ставим сохраненный элемент на соответствующее ему место в другой строке
	matrix[max_element_index * matrix_size + idx] = temp;

	//делаем аналогичную операцию, но с правой матрицей
	temp = identity[row * matrix_size + idx];
	identity[row * matrix_size + idx] = identity[max_element_index * matrix_size + idx];
	identity[max_element_index * matrix_size + idx] = temp;
}

__global__ void make_zero_below(double* matrix, double* identity, int matrix_size, int x)
{
	//узнаем номер потока
	int idx = threadIdx.x;
	int i, j;
	
	//если это строка, под которой надо вычесть, то ничего не вычитаем, саму из себя строку вычитать нельзя
	if (idx * matrix_size == x * matrix_size) return;
	
	//находим коэффициент
	double 	coeff = matrix[idx * matrix_size + x] / matrix[x * matrix_size + x];

	//проходим строку x с элемента matrix[x][x] до конца строки и из каждого элемента вычитаем\
	соответствующий элемент строки x, умноженный на coeff
	for (i = x;i < matrix_size;i++)
		matrix[idx * matrix_size + i] -= coeff* matrix[x * matrix_size + i];
	
	//делаем тоже самое для правой части расширенной матрицы
	for (;i < 2*matrix_size;i++)
		identity[idx * matrix_size + i] -= coeff * identity[x * matrix_size + i];


}

void print_both(double* matrix, double* identity_matrix, int matrix_size);
void print(double* matrix, int n);

__host__ int main()
{
	//ввод размера матрицы
	cout << "Enter the size of the matrix: ";
	int matrix_size;
	cin>> matrix_size;

	//заполнение матрицы
	//выделение памяти для данных на CPU
	double* matrix = new double[matrix_size * matrix_size];
	assert(matrix != 0);
	double* identity_matrix = new double[matrix_size * matrix_size];
	assert(identity_matrix != 0);

	//заполняем таблицу случайными числами
	srand(time(NULL));
	for (int i = 0; i < matrix_size * matrix_size; i++)
		matrix[i] = 1 + rand() % 10;

	//заполняем единичную матрицу
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++)
			identity_matrix[i * matrix_size + j] = (i == j ? 1.0 : 0.0);

	cout<< "Before:"<< endl;
	print_both(matrix, identity_matrix, matrix_size);

	//Выделение памяти для данных на GPU
	double* dev_matrix;
	double* dev_identity;
	cudaMalloc((void**)&dev_matrix, sizeof(double) * matrix_size * matrix_size);
	cudaMalloc((void**)&dev_identity, sizeof(double) * matrix_size * matrix_size);

	//копируем с host на device
	cudaMemcpy(dev_matrix, matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_identity, identity_matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyHostToDevice);

	//инициализируем переменные для замера времени счета на GPU
	float recording;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	//на данном этапе все готово и можно приступать к преобразованиям\
	с помощью элементарных преобразований обнулим матрицу под главной диагональю
	//int i = 0;
	for (int i = 0; i < matrix_size; i++)
	{
	// Обменять строку со строкой с наибольшим ведущим элементом и обнулить
	// элементы под ним
	
	//ищем максимальный элемент столбца
	max_in_column<< <1, 1 >> > (dev_matrix, matrix_size, i);
	
	//меняем i-ую строку со строкой с максимальным элементом в i-ом столбце местами
	swap_lines << <1, matrix_size>> >(dev_matrix, dev_identity, matrix_size, i);

	//обнуляем элементы под i-ым в i-ом столбце
	make_zero_below << < 1, matrix_size-i >> > (dev_matrix, dev_identity, matrix_size, i);
	
	cudaMemcpy(matrix, dev_matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);
	cudaMemcpy(identity_matrix, dev_identity, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);

	cout << "After:" << endl;
	print_both(matrix, identity_matrix, matrix_size);
	//что-то
	/*nullify_below << <BLOCKS_1D, THREADS_1D >> > (dev_matrix, n, i);*/
	}
	cudaThreadSynchronize();//синхронизация потоков

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&recording, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	//копируем результат из памяти GPU на CPU
	cudaMemcpy(matrix, dev_matrix, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);
	cudaMemcpy(identity_matrix, dev_identity, sizeof(double) * matrix_size * matrix_size, cudaMemcpyDeviceToHost);

	cout << "After:" << endl;
	print_both(matrix, identity_matrix, matrix_size);

	//вывод обратной матрицы
	cout << "Invers matrix:" << endl;
	print(identity_matrix, matrix_size);

	cout << "Time of work: " << fixed << recording << endl;

	//освобождаем память device
	cudaFree(dev_matrix);
	cudaFree(dev_identity);

	//освобождаем память host
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
