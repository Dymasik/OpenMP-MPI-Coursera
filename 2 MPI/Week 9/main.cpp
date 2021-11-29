#include <stdio.h>
#include "mpi.h"
#include "Exercise2_dll.h"
#include <iostream>
using namespace std;

// Функция выделения памяти под вектор
double* malloc_array(int n)
{
	double* a = new double[n];
	return a;
}

// Функция освобождения памяти 
int free_array(double* a, int n)
{
	delete[] a;
	return 0;
}

int multmv_s(int n, double* A_glob, double* x)
{
	double* temp_x = malloc_array(n);
	for (int k = 0; k < 100; k++)
	{
		for (int i = 0; i < n; i++)
		{
			temp_x[i] = 0;
			for (int j = 0; j < n; j++)
			{
				//cout << Rank << " " << i << " :: " << A_glob[i * n + j] << " " << x[j] << endl;

				temp_x[i] += A_glob[i * n + j] * x[j];
			}
		}
		for (int i = 0; i < n; i++)
		{
			x[i] = temp_x[i];
		}
	}

	free_array(temp_x, n);

	// Сбор решения

	return 0;
}

int multmv(int n, double* A_glob, double* x, double* y)
{
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int n1 = n / size;

	double* matrix = malloc_array(n * n);

	MPI_Scatter(A_glob, n * n1, MPI_DOUBLE, matrix, n * n1, MPI_DOUBLE, 0, MPI_COMM_WORLD); //Scatter the Matrix
	MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double* temp_x = malloc_array(n1);
	for (int k = 0; k < 100; k++)
	{
		for (int i = 0; i < n1; i++)
		{
			y[i] = 0;
			for (int j = 0; j < n; j++)
			{
				y[i] += matrix[i * n + j] * x[j];
			}
		}
		for (int i = 0; i < n1; i++)
		{
			temp_x[i] = y[i];
		}
		MPI_Allgather(temp_x, n1, MPI_DOUBLE, x, n1, MPI_DOUBLE, MPI_COMM_WORLD);
	}


	// Сбор решения


	free_array(temp_x, n1);
	free_array(matrix, n * n1);

	return 0;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	// Иницилизация MPI
	//int rank, size;
	/*
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);*/

	// Определение размера задачи и подзадач
	/*int n = 2;
	//if (rank < n - n1 * size) n1++;

	// Выделение памяти 
	double* A_glob = malloc_array(n * n);
	double* x = malloc_array(n);
	double* y = malloc_array(n);
	double* matrix = malloc_array(n * n);

	// Заполнение матрицы А и вектора х
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A_glob[i * n + j] = (double)rand() / RAND_MAX / (n / 2);
			}
			x[i] = (double)rand() / RAND_MAX / (n / 2);
			y[i] = 0;
		}

		A_glob[0] = 1;
		A_glob[1] = 2;
		A_glob[2] = 3;
		A_glob[3] = 4;
		x[0] = 2;
		x[1] = 4;
	}


	MPI_Barrier(MPI_COMM_WORLD);*/
	/*
	MPI_Scatter(A_glob, (n * n) / 2, MPI_DOUBLE, matrix, (n * n) / 2, MPI_DOUBLE, 0, MPI_COMM_WORLD); //Scatter the Matrix
	MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);*/

	//multmv(n, A_glob, x, y);

	/*
	if (rank == 0) {
		cout << "The Program is RUN on " << size << " CPU" << endl;
		cout << " y[0] = " << y[0] << "!" << endl;
	}*/

	// Вызов функции из библиотеки, которая проводит проверку корректности реализации алгоритма и его ускорения
	submit(multmv);
	/*
	free_array(A_glob, n * n);
	free_array(x, n);
	free_array(y, n);*/
	MPI_Finalize();
	return 0;
}