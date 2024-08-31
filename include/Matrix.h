#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
    private:
        int rows;
        int cols;
        double* data; 
    public:
        Matrix(int r, int c);
        ~Matrix();

        int getRows() {return rows;}
        int getCols() {return cols;}

        Matrix* dot(Matrix* m);
        Matrix* transpose();

        double get(int r, int c);
        void set(int r, int c, double v);
        void print();
};

#endif
