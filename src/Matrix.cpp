/*
TODO:
    - Add add function
*/

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
    - N/A

Example:
    Matrix m(3, 2);
*/
Matrix::Matrix(int r, int c) {
    rows = r;
    cols = c;
    data = new double[r * c];
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
        cerr << e.what() << endl; 
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
