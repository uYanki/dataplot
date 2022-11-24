#include "fft4.h"

// 基4FFT https://whycan.com/t_2804.html

void fft4(const double* src_x, const double* src_y, double* dst_x, double* dst_y, int k) {
    int     i, n;
    double  temp, temp_x, temp_y;
    double* buffer;
    if (0 == k) {
        *dst_x = *src_x;
        *dst_y = *src_y;
        return;
    }
    n      = 1 << (k - 1);
    buffer = (double*)malloc(4 * n * sizeof(double));
    for (i = 0; i < n; i++) {
        buffer[i]         = src_x[i * 2];
        buffer[i + n]     = src_y[i * 2];
        buffer[i + 2 * n] = src_x[i * 2 + 1];
        buffer[i + 3 * n] = src_y[i * 2 + 1];
    }
    fft4((const double*)buffer + 2 * n, (const double*)buffer + 3 * n, dst_x, dst_y, k - 1);
    fft4((const double*)buffer, (const double*)buffer + n, (double*)buffer + 2 * n, (double*)buffer + 3 * n, k - 1);
    for (i = 0; i < n; i++) {
        temp          = i * M_PI / n;
        temp_x        = cos(temp);
        temp_y        = sin(temp);
        buffer[i]     = dst_y[i] * temp_y + dst_x[i] * temp_x;
        buffer[i + n] = dst_y[i] * temp_x - dst_x[i] * temp_y;
        dst_x[i]      = buffer[i + 2 * n] + buffer[i];
        dst_y[i]      = buffer[i + 3 * n] + buffer[i + n];
        dst_x[i + n]  = buffer[i + 2 * n] - buffer[i];
        dst_y[i + n]  = buffer[i + 3 * n] - buffer[i + n];
    }
    free(buffer);
}

#include <QDebug>

int fft4(const QVector<double> &in , QVector<double> &out , size_t k)
{
    int len = 1<<k;
    int size = len * sizeof(double);

    double *src_x = new double[len];
    double *src_y = new double[len];
    double *dst_x = new double[len];
    double *dst_y = new double[len];

    if( in.count() > len ){
        int i = 0;
        for(i=0;i<len;++i){
            src_x[i] = in.at(i) ;
            src_y[i] = 0;
            dst_x[i] = 0;
            dst_y[i] = 0;
        }
    }else{
        int i = 0;

        for(i=0;i<in.count();++i){
            src_x[i] = in.data()[i] ;
        }

        for(i=in.count();i<len;++i){
            src_x[i] = 0 ;
        }

        for(i=0;i<len;++i){
            src_y[i] = 0;
            dst_x[i] = 0;
            dst_y[i] = 0;
        }
    }


    fft4(src_x,src_y,dst_x,dst_y,k);

    out.resize(len/=2);
//    qDebug() << k;
    for(int i = 0;i<len;++i) out.data()[i] = sqrt(dst_x[i]*dst_x[i]+dst_y[i]*dst_y[i]);

    delete[] src_x;
    delete[] src_y;
    delete[] dst_x;
    delete[] dst_y;

    return len; // size of out vector

}
