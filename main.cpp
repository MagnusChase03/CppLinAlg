#include "include/Matrix.h"

int main() {
    Matrix* m = new Matrix(3, 2);
    Matrix* m2 = new Matrix(2, 1);

    m->set(0, 0, 1.0);
    m->set(1, 0, 1.0);
    m->set(2, 0, 1.0);
    m2->set(0, 0, 2.0);

    Matrix* m3 = m->dot(m2);
    m3->print();

    Matrix* m4 = m3->transpose();
    m4->print();

    Matrix* m5 = new Matrix(1, 3);
    m5->set(0, 0, 1.0);
    m5->set(0, 1, 1.0);
    m5->set(0, 2, 1.0);

    Matrix* m6 = m5->add(m4);
    m6->print();

    return 0;
}
