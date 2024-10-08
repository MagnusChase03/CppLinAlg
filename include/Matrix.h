#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
    private:
        int rows;
        int cols;
        double* data; 
    public:
        Matrix(int r, int c);
        Matrix(int r, int c, double* d);
        ~Matrix();

        int getRows() {return rows;}
        int getCols() {return cols;}

        Matrix* add(Matrix* m);
        Matrix* dot(Matrix* m);
        Matrix* transpose();
        Matrix* slice(int x, int x2, int y, int y2);

        double get(int r, int c);
        void set(int r, int c, double v);
        void print();
};

#endif
