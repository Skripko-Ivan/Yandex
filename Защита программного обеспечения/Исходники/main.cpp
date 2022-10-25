#include "main.h"

int main() 
{
    //ввод размера матрицы
    cout << "Enter the size of the matrix: ";
    int matrix_size;
    cin >> matrix_size;

    vector<vector<double>> matrix;
    vector<vector<double>> identity;
    matrix.resize(matrix_size);
    identity.resize(matrix_size);

    //заполняем матрицы
    srand(time(NULL));
    for (int i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; j++) 
        {
            identity[i].push_back(i == j ? 1.0 : 0.0);
            matrix[i].push_back(rand() % 10);
        }
    cout << "Before inverse:\n";
    print(matrix, matrix_size);
    double time = clock();
    //нахождение обратной матрицы
    inverse(matrix, identity, matrix_size);
    time = (clock()-time) / CLOCKS_PER_SEC;
    cout << "Inversed:\n";
    print(identity, matrix_size);
    //cout << "Time of work: " << time << endl;

    return 0;
}


void print(vector<vector<double>> &matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout <<fixed<< matrix[i][j] << "\t";
        cout << endl;
    }
}

// Поставить на место row строку с наибольшим ведущим коэффициентом
void pivotize(vector<vector<double>>& matrix, vector<vector<double>>& identity, int n, int row) {
    int max_index = row;
    double max_value = fabs(matrix[row][row]);
    double current_value;
    for (int i = row + 1; i < n; i++) 
    {
        current_value = fabs(matrix[i][row]);
        if (current_value > max_value) 
        {
            max_index = i;
            max_value = current_value;
        }
    }

    //меняем строки местами
    if (row != max_index)
    {
        swap(matrix[row], matrix[max_index]);
        swap(identity[row], identity[max_index]);
    }
}

// Разделить строку матрицы на значение
void devide(vector<vector<double>>& matrix, int n, int i, double denominator) 
{
    for (int j = 0; j < n; j++)
        matrix[i][j] /= denominator;
}

// Обнуление элементов под ведущим элементом в строке x
void subtract_below(vector<vector<double>>& matrix, vector<vector<double>>& identity, int n, int x) {
    double coeff;
    for (int i = x + 1; i < n; i++) 
    {
        coeff = matrix[i][x] / matrix[x][x];
        for (int j = x; j < n; j++)
            matrix[i][j] -= coeff * matrix[x][j];
        for (int j = 0; j < n; j++)
            identity[i][j] -= coeff * identity[x][j];
    }
}

// Обнуление элементов над ведущим элементом в строке x
void subtract_above(vector<vector<double>>& matrix, vector<vector<double>>& identity, int n, int x) 
{
    double coeff;
    for (int i = x - 1; i >= 0; i--) 
    {
        //обнуляем элементы выде главной диагонали матрицы для левой части расширенной матрицы
        coeff = matrix[i][x] / matrix[x][x];
        for (int j = x; j >= 0; j--)
            matrix[i][j] -= coeff * matrix[x][j];

        //делаем тоже самое для правой матрицы расширенной матрицы
        for (int j = 0; j < n; j++)
            identity[i][j] -= coeff * identity[x][j];
    }
}

// Обращение матрицы методом Гаусса-Жордана
void inverse(vector<vector<double>>& matrix, vector<vector<double>>& identity, int n) 
{
    //с помощью элементарных преобразований над матрицами получим верхнюю треугольную матрицу
    for (int i = 0; i < n - 1; i++) 
    {
        // Обмен строк
        pivotize(matrix, identity, n, i);

        // Обнулить элементы ниже ведущего
        subtract_below(matrix, identity, n, i);
    }
    
    //с помощью элементарных преобразований получим диагональную матрицу
    for (int i = n - 1; i > 0; i--)
        // Обнулить элементы выше ведущего
        subtract_above(matrix, identity, n, i);
    
    // Разделить строки на ведущие элементы
    for (int i = 0; i < n; i++) 
    {
        devide(identity, n, i, matrix[i][i]);
        devide(matrix, n, i, matrix[i][i]);
    }
}