//Panait Stefan-Andrei 332CA
#include <stdio.h>
#include <stdlib.h>
typedef struct matrix_list {
	int **list_of_matrix;
	int size_column;
	int size_line;
} matrix_list;

#include "my_octave.h"

int main(void)
{
	solve();
	return 0;
}

//Functie pentru citire si functionalitate
void solve(void)
{
	int contor = 0, index, pow, searched, matrix1, matrix2, capacity = DIM;
	char caracter;
	matrix_list *matrix_list = (struct matrix_list *)malloc(DIM *
													sizeof(struct matrix_list));
	do {
		scanf(" %c", &caracter);
		switch (caracter) {
		case 'L': {
			matrix_list[contor++] = read_matrix();
			if (contor % DIM == 0)
				matrix_list = reallocation(matrix_list, &contor, &capacity);
		}
				break;
		case 'D': {
			scanf("%d", &searched);
			m_dim(matrix_list, contor, searched);
		}
				break;
		case 'P': {
			scanf("%d", &searched);
			print_matrix(matrix_list, contor, searched);
		}
				break;
		case 'C': {
			scanf("%d", &searched);
			resize(matrix_list, contor, searched);
		}
				break;
		case 'M': {
			scanf("%d%d", &matrix1, &matrix2);
			if (matrix1 < contor && matrix2 < contor &&
				matrix1 >= 0 && matrix2 >= 0) {
				if (matrix_list[matrix1].size_column ==
					matrix_list[matrix2].size_line) {
					matrix_list[contor] = multiply(matrix1, matrix2,
												   matrix_list);
					contor++;
					if (contor % DIM == 0)
						matrix_list = reallocation(matrix_list,
												   &contor, &capacity);
				} else {
					printf("Cannot perform matrix multiplication\n");
				}
			} else {
				printf("No matrix with the given index\n");
			}
		}
				break;
		case 'O':
			sort(matrix_list, contor);
			break;
		case 'R': {
			scanf("%d%d", &index, &pow);
			if (index > contor || index < 0)
				printf("No matrix with the given index\n");
			else if (matrix_list[index].size_column !=
					 matrix_list[index].size_line)
				printf("Cannot perform matrix multiplication\n");
			else if (pow < 0)
				printf("Power should be positive\n");
			else
				matrix_power(matrix_list[index].list_of_matrix,
							 pow, matrix_list[index].size_line);
		}
				break;
		case 'T':
			transpunere(matrix_list, contor);
			break;
		case 'F':
			delete_mat(&contor, matrix_list, &capacity);
			break;
		case 'Q':
			free_resources(matrix_list, &contor);
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
	} while (caracter != 'Q');
}

void resize(struct matrix_list *list, int contor, int searched)
{
	//aloc vectorii pentru linii si coloane
	int n, m;
	scanf("%d", &n);
	int *l = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		scanf("%d", &l[i]);
	scanf("%d", &m);
	int *c = (int *)malloc(m * sizeof(int));
	for (int i = 0; i < m; i++)
		scanf("%d", &c[i]);
	//verific daca exista matricea cu indexul dat
	if (searched >= contor || searched < 0) {
		// daca nu exista afisez ca nu exista si dau free la vectori
		printf("No matrix with the given index\n");
		free(l);
		free(c);
		return;
	}
	//aloc spatiu pentru noua matrice
	int **a = matrix_allocation(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			a[i][j] = list[searched].list_of_matrix[l[i]][c[j]];
	}
	for (int i = 0; i < list[searched].size_line; i++)
		free(list[searched].list_of_matrix[i]);
	free(list[searched].list_of_matrix);
	// salvez la indexul meu noua matrice
	list[searched].list_of_matrix = a;
	list[searched].size_line = n;
	list[searched].size_column = m;
	free(l);
	free(c);
}

void print_matrix(struct matrix_list *matrix_list, int contor, int searched)
{
	if (searched >= contor || searched < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	for (int i = 0; i < matrix_list[searched].size_line; i++) {
		for (int j = 0; j < matrix_list[searched].size_column; j++)
			printf("%d ", matrix_list[searched].list_of_matrix[i][j]);
		printf("\n");
	}
}

struct matrix_list multiply(int matrix1, int matrix2, struct matrix_list *list)
{
	//creez o noua structura pe care o sa o returnez dupa inmultire
	struct matrix_list aux;
	aux.list_of_matrix = matrix_allocation(list[matrix1].size_line,
										   list[matrix2].size_column);
	//fac inmultirea si setez matricea
	for (int i = 0; i < list[matrix1].size_line; i++) {
		for (int j = 0; j < list[matrix2].size_column; j++) {
			int s = 0;
			for (int k = 0; k < list[matrix1].size_column; k++) {
				s += list[matrix1].list_of_matrix[i][k] *
					list[matrix2].list_of_matrix[k][j] % MOD;
			}
			while (s < 0)
				s += MOD;
			aux.list_of_matrix[i][j] = s % MOD;
		}
	}
	aux.size_line = list[matrix1].size_line;
	aux.size_column = list[matrix2].size_column;
	return aux;
}

//functie pentru a realoca vecotrul meu de structuri
struct matrix_list *reallocation(struct matrix_list *list,
								 int *contor, int *capacity)
{
	int size = sizeof(struct matrix_list) * (*contor + DIM);
	list = (struct matrix_list *)realloc(list, size);
	*capacity += DIM;
	return list;
}

void swap_integers(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void m_dim(struct matrix_list *list, int contor, int searched)
{
	if (searched >= contor || searched < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	printf("%d %d\n", list[searched].size_line, list[searched].size_column);
}

int **matrix_allocation(int n, int m)
{
	int **tmp = (int **)malloc(n * sizeof(int *));
	if (!tmp) {
		fprintf(stderr, "Nu s-a putut aloca!\n");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		tmp[i] = (int *)malloc(m * sizeof(int));
		if (!tmp[i]) {
			fprintf(stderr, "Nu s-a putut aloca!\n");
			return NULL;
		}
	}
	return tmp;
}

struct matrix_list read_matrix(void)
{
	matrix_list aux;
	int n, m;
	scanf("%d%d", &n, &m);
	aux.list_of_matrix = matrix_allocation(n, m);
	aux.size_line = n;
	aux.size_column = m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", &aux.list_of_matrix[i][j]);
	return aux;
}

//functie pentru a calcula suma elementelor unei matrici
int sum_matrix(int **mat, int n, int m)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (mat[i][j] < 0)
				sum += (MOD + mat[i][j] % MOD) % MOD;
			else
				sum += (mat[i][j] % MOD) % MOD;
			sum %= MOD;
		}
	return sum;
}

// sortez matricile in functie de suma elementelor lor
void sort(struct matrix_list *list, int contor)
{
	int **aux, i, j;
	for (i = 0; i < contor - 1; i++) {
		for (j = i + 1; j < contor; j++) {
			if (sum_matrix(list[i].list_of_matrix,
						   list[i].size_line, list[i].size_column) >
				sum_matrix(list[j].list_of_matrix,
						   list[j].size_line, list[j].size_column)) {
				aux = list[i].list_of_matrix;
				list[i].list_of_matrix = list[j].list_of_matrix;
				list[j].list_of_matrix = aux;
				swap_integers(&list[i].size_line, &list[j].size_line);
				swap_integers(&list[i].size_column, &list[j].size_column);
			}
		}
	}
}

void transpunere(struct matrix_list *list, int contor)
{
	int x;
	scanf("%d", &x);
	//verific daca exista matricea cu indexul dat
	if (x >= contor || x < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	int n = list[x].size_column;
	int m = list[x].size_line;
	//aloc noua matrice
	int **t = matrix_allocation(n, m);
	//fac transpunerea
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			t[i][j] = list[x].list_of_matrix[j][i];
	}
	//eliberez vechea matrice
	for (int i = 0; i < m; i++)
		free(list[x].list_of_matrix[i]);
	free(list[x].list_of_matrix);
	//setez noua matrice transpusa
	list[x].list_of_matrix = t;
	list[x].size_line = n;
	list[x].size_column = m;
}

void delete_mat(int *contor, struct matrix_list *list, int *capacity)
{
	int x;
	scanf("%d", &x);
	//verific daca exista matricea cu indexul dat
	if (x >= *contor || x < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	//aloc o noua lista de matrici
	struct matrix_list *list_aux = (struct matrix_list *)
									malloc((*contor - 1) *
									sizeof(struct matrix_list));
	for (int i = 0; i < list[x].size_line; i++)
		free(list[x].list_of_matrix[i]);
	free(list[x].list_of_matrix);
	//salvez in noua lista toate matricile in afara de cea pe care o sterg
	for (int i = 0; i < x; i++) {
		list_aux[i].list_of_matrix = list[i].list_of_matrix;
		list_aux[i].size_line = list[i].size_line;
		list_aux[i].size_column = list[i].size_column;
	}
	for (int i = x; i < *contor - 1; i++) {
		list_aux[i].list_of_matrix = list[i + 1].list_of_matrix;
		list_aux[i].size_line = list[i + 1].size_line;
		list_aux[i].size_column = list[i + 1].size_column;
	}
	// pun la loc in lista mea de matrici fara cea stearsa
	for (int i = 0; i < *contor - 1; i++) {
		list[i].list_of_matrix = list_aux[i].list_of_matrix;
		list[i].size_line = list_aux[i].size_line;
		list[i].size_column = list_aux[i].size_column;
	}
	free(list_aux);
	--*contor;
	if (*capacity - *contor > DIM)
		reallocation(list, contor, capacity);
}

//inmultirea a 2 matrici facuta pentru putere(putand fii apelata recursiv)
void mul(int **a, int **b, int n)
{
	int **c = matrix_allocation(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			c[i][j] = 0;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int s = 0;
			for (int k = 0; k < n; k++)
				s += a[i][k] * b[k][j] % MOD;
			while (s < 0)
				s += MOD;
			c[i][j] = s % MOD;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			a[i][j] = c[i][j];
	}
	for (int i = 0; i < n; i++)
		free(c[i]);
	free(c);
}

void matrix_power(int **a, int pow, int n)
{
	//cazul de baza
	if (pow == 1)
		return;
	if (pow == 0) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				a[i][j] = (i == j);
		}
		return;
	}
	// b matrice auxiliara pe care o folosesc pentru ca
	// ca sa fac ridicarea la putere
	int **b;
	if (pow % 2) {
		b = matrix_allocation(n, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				b[i][j] = a[i][j];
		}
		matrix_power(a, pow - 1, n);
		mul(b, a, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				a[i][j] = b[i][j];
		}
		//eliberez b pentru ca numai imi trb
		for (int i = 0; i < n; i++)
			free(b[i]);
		free(b);
	} else {
		mul(a, a, n);
		matrix_power(a, pow / 2, n);
	}
}

void free_resources(struct matrix_list *list, int *contor)
{
	//eliberez matricile dupa care eliberez lista
	if (*contor > 0) {
		for (int k = 0; k < *contor; k++) {
			for (int i = 0; i < list[k].size_line; i++)
				free(list[k].list_of_matrix[i]);
			free(list[k].list_of_matrix);
		}
		contor = 0;
	}
	free(list);
}
