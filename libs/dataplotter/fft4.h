#ifndef FFT4_H
#define FFT4_H

#include <cmath>
#include <QVector>

void fft4(const double* src_x, const double* src_y, double* dst_x, double* dst_y, int k);

int fft4(const QVector<double>& in, QVector<double> &out , size_t k);

#endif // FFT4_H
