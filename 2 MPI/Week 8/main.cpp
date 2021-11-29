#include <stdio.h>
#include <mpi.h>
#include "Exercise1_dll.h"
#include <iostream>
using namespace std;

// Функция выделения памяти под вектор
double* malloc_array(int n)
{
	double* a = new double[n];
	return a;
}

// Функция освобождения памяти 
int free_array(double*a, int n)
{
	delete[] a;
	return 0;
}
double scalmult(int n, double* a, double* b)
{
	// Определения числа доступных процессов и размера подзадач

	// Вычисление скалярного произведения
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += a[i] * b[i];
	}
	sum = sqrt(sum);
	return sum;
}

int main(int argc, char **argv)
{

	// Иницилизация MPI
	int rank, size;
	double res = 0;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Определение размера задачи и подзадач
	int n = 10;
	int n1 = n / size;
	if (rank < n - n1 * size) n1++;

	// Выделение памяти 
	double *a = malloc_array(n1);
	double *b = malloc_array(n1);

	// Заполнение векторов
	for (int i = 0; i < n1; i++)
	{
		a[i] = (double) rand() / RAND_MAX;
		b[i] = (double) rand() / RAND_MAX;
	}

	double sc = scalmult(n, a, b);

	MPI_Reduce(&sc, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	if (rank == 0) {
		/*res = sc;
		for (int i = 1; i < size; i++) {
			MPI_Recv(&sc, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			res += sc;
		}*/
		cout << "The Program is RUN on " << size << " CPU" << endl;
	}
	/*else {
		MPI_Send(&sc, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}*/

	// Вызов функции из библиотеки, которая проводит проверку корректности реализации алгоритма и его ускорения
	submit(scalmult);

	free_array(a, n1);
	free_array(b, n1);
	MPI_Finalize();
	return 0;
}

