#include "../include/Matrix.h"
#include <iostream>
#include <stdexcept>
#include <thread>

/*
Constructor for a new Matrix.

Arguments:
    - r (int): The number of rows of the matrix.
    - c (int): The number of cols of the matrix.

Return:
    - std::runtime_error: An error if any occured.

Example:
    Matrix m(3, 2);
*/
Matrix::Matrix(int r, int c) {
    if (r < 0 || c < 0) {
        throw std::runtime_error("Error: Dimension error.");
    }

    rows = r;
    cols = c;
    data = new double[r * c];
}

/*
Thread function to partial clone a matrix.

Arguments:
    - a (double*): The source of values.
    - b (double*): The dest of values.
    - cols (int): The number of columns in the matrix.
    - row (int): The row to preform math on.

Return:
    - N/A

Example:
    std::thread(clone_thread, a, b, 3, 0);
*/
void clone_thread(double* a, double* b, int cols, int row) {
    for (int i = 0; i < cols; i++) {
        b[(row * cols) + i] = a[(row * cols) + i];
    }
}

/*
Constructs a matrix with given values.

Arguments:
    - r (int): The number of rows in the matrix.
    - c (int): The number of cols in the matrix.
    - d (double*): The values to set the matrix to.

Return:
    - std::runtime_error: An error if any occured.

Example:
    double values[] = {1.0, 1.0, 2.0, 2.0}:
    Matrix m(2, 2, values);
*/
Matrix::Matrix(int r, int c, double* d) {
    if (r < 0 || c < 0) {
        throw std::runtime_error("Error: Dimension error.");
    }

    rows = r;
    cols = c;
    data = new double[r * c];

    std::thread threads[r];
    for (int i = 0; i < r; i++) {
        threads[i] = std::thread(clone_thread, d, data, c, i);
    }

    for (int i = 0; i < r; i++) {
        threads[i].join();
    }
}

/*
Deconstructor for a Matrix.

Arguments:
    - N/A

Return:
    - N/A

Example:
    Matrix* m = new Matrix(3, 2);
    delete m;
*/
Matrix::~Matrix() {
    delete data;
}

/*
Thread function to calculate partial addition for a single row in the result
matrix.

Arguments:
    - a (Matrix*): The first matrix for addition.
    - b (Matrix*): The second matrix for addition.
    - c (Matrix*): The result matrix.
    - row (int): The row to preform math on.

Return:
    - N/A

Example:
    std::thread(add_thread, this, b, c, 0);
*/
void add_thread(Matrix* a, Matrix* b, Matrix* c, int row) {
    for (int i = 0; i < a->getCols(); i++) {
        c->set(row, i, a->get(row, i) + b->get(row, i));
    }
}

/*
Returns the addition of two matrixes.

Arguments:
    - m (Matrix*): The matrix to add with.

Returns:
    - Matrix*: The result matrix of the addition.

Example:
    try {
        Matrix* m = new Matrix(3, 2);
        Matrix* m2 = new Matrix(3, 2);
        Matrix* m3 = m.add(m2);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
*/
Matrix* Matrix::add(Matrix* m) {
    if (rows != m->getRows() || cols != m->getCols()) {
        throw std::runtime_error("Error: Dimension error.");
    }
    
    std::thread threads[rows];
    Matrix* result = new Matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        threads[i] = std::thread(add_thread, this, m, result, i);
    }

    for (int i = 0; i < rows; i++) {
        threads[i].join();
    }

    return result;
}

/*
Thread function to calculate partial dot product for a single row in the result
matrix.

Arguments:
    - a (Matrix*): The first matrix in dot product.
    - b (Matrix*): The second matrix in dot product.
    - c (Matrix*): The result matrix.
    - row (int): The row to preform math on.

Return:
    - N/A

Example:
    std::thread(dot_thread, this, b, c, 0);
*/
void dot_thread(Matrix* a, Matrix* b, Matrix* c, int row) {
    for (int i = 0; i < b->getCols(); i++) {
        for (int j = 0; j < b->getRows(); j++) {
            c->set(row, i, c->get(row, i) + (a->get(row, j) * b->get(j, i)));
        }
    }
}

/*
Computes the dot product of two matrixes.

Arguments:
    - m (Matrix*): The matrix to do the dot product with.

Return:
    - Matrix*: The result of the dot product.
    - std::runtime_error: An error if any occured.

Example:
    try {
        Matrix* m = new Matrix(3, 2);
        Matrix* m2 = new Matrix(2, 1);
        Matrix* m3 = m.dot(m2);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl; 
    }
*/
Matrix* Matrix::dot(Matrix* m) {
    if (cols != m->getRows()) {
        throw std::runtime_error("Error: Dimension error.");
    }

    std::thread threads[rows];
    Matrix* result = new Matrix(rows, m->getCols());
    for (int i = 0; i < rows; i++) {
        threads[i] = std::thread(dot_thread, this, m, result, i);
    }

    for (int i = 0; i < rows; i++) {
        threads[i].join();
    }

    return result;
}

/*
Does a partial transpose for multithreading.

Arguments:
    - a (Matrix*): The source matrix.
    - b (Matrix*): The result matrix.
    - row (int): The row to do partial transpose on.

Returns:
    - N/A

Example:
    std::thread(transpose_thread, a, b, 0);
*/
void transpose_thread(Matrix* a, Matrix* b, int row) {
    for (int i = 0; i < a->getCols(); i++) {
        b->set(i, row, a->get(row, i));
    }
}

/*
Returns the transpose of a matrix.

Arguments:
    - N/A

Returns:
    - N/A

Example:
    Matrix* m = new Matrix(3, 2);
    Matrix* m2 = m.transpose();
*/
Matrix* Matrix::transpose() {
    std::thread threads[rows];

    Matrix* m = new Matrix(cols, rows);
    for (int i = 0; i < rows; i++) {
        threads[i] = std::thread(transpose_thread, this, m, i);
    }
    
    for (int i = 0; i < rows; i++) {
        threads[i].join();
    }

    return m;
}

/*
Threaded function to copy a portion of the original matrix.

Arguments:
    - a (Matrix*): The source matrix.
    - b (Matrix*): The result matrix.
    - x (int): Starting x.
    - x2 (int): Ending x (non-inclusive).
    - y (int): Starting y.
    - row (int): The row to process.

Returns:
    - N/A

Example:
    std::thread(slice_thread, a, b, 0, 2, 0, 1);
*/
void slice_thread(Matrix* a, Matrix* b, int x, int x2, int y, int row) {
    for (int i = x; i < x2; i++) {
        b->set(row - y, i - x, a->get(row, i));
    } 
}

/*
Returns a new matrix with requested ranges from current matrix.

Arguments:
    - x (int): Starting x
    - x2 (int): Ending x (non-inclusive)
    - y (int): Starting y
    - y2 (int): Ending y (non-inclusive)

Returns:
    - Matrix*: The subset of the matrix with requested bounds.
    - std::runtime_error: The error if any occured.

Example:
    try {
        Matrix* m = new Matrix(4, 4);
        Matrix* m2 = m.slice(0, 2, 0, 2);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
*/
Matrix* Matrix::slice(int x, int x2, int y, int y2) {
    if (
        x < 0 || x >= cols || x2 < 0 || x2 > cols ||
        y < 0 || y >= rows || y2 < 0 || y2 > rows ||
        x2 <= x || y2 <= y
    ) {
        throw std::runtime_error("Error: Dimension error.");
    }

    std::thread threads[y2 - y];
    Matrix* result = new Matrix(y2 - y, x2 - x);
    for (int i = y; i < y2; i++) {
        threads[i - y] = std::thread(slice_thread, this, result, x, x2, y, i);
    }

    for (int i = y; i < y2; i++) {
        threads[i - y].join();
    }

    return result;
}

/*
Gets a value in the matrix.

Arguments:
    - r (int): The row to get.
    - c (int): The col to get.

Return:
    - double: The value in the matrix.
    - std::runtime_error: An error if any occured.

Example:
    Matrix m(3, 2);
    double d = m.get(0, 0);
*/
double Matrix::get(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw std::runtime_error("Error: Invalid dimension.");
    }

    return data[(r * cols) + c];
}

/*
Sets a value in the matrix.

Arguments:
    - r (int): The row to set.
    - c (int): The col to set.
    - v (double): The value to set.

Return:
    - std::runtime_error: An error if any occured.

Example:
    Matrix m(3, 2);
    m.set(0, 0, 1.0);
*/
void Matrix::set(int r, int c, double v) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw std::runtime_error("Error: Invalid dimension.");
    }

    data[(r * cols) + c] = v;
}


/*
Prints the data of the matrix.

Arguments:
    - N/A

Return:
    - N/A

Example:
    Matrix m(3, 2);
    m.print();
*/
void Matrix::print() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << data[(i * cols) + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "(" << rows << ", " << cols << ")" << std::endl;
}
