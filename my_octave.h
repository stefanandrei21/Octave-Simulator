//Panait Stefan-Andrei
#ifndef HEADER_H_
#define HEADER_H_
#define MOD 10007
#define DIM  35

void solve(void);

void resize(struct matrix_list *list, int contor, int searched);

void print_matrix(struct matrix_list *v, int nr_matrix, int searched);

struct matrix_list multiply(int matrix1, int matrix2, struct matrix_list *v);

struct matrix_list *reallocation(struct matrix_list *list,
								 int *contor, int *capacity);

void swap_integers(int *a, int *b);

void m_dim(struct matrix_list *list, int contor, int searched);

int **matrix_allocation(int n, int m);

struct matrix_list read_matrix(void);

int sum_matrix(int **mat, int n, int m);

void sort(struct matrix_list *list, int contor);

void transpunere(struct matrix_list *list, int contor);

void delete_mat(int *nr_matrix, struct matrix_list *list, int *contor);

void matrix_power(int **a, int pow, int n);

void mul(int **a, int **b, int n);

void free_resources(struct matrix_list *list, int *contor);
#endif

