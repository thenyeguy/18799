/*
 *
Cmatrix, Simple Matrix Library written in C
Copyright (C) 2010,2011,2012  Mehmet Hakan Satman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Mehmet Hakan Satman - mhsatman@yahoo.com
 * http://www.mhsatman.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

Matrix *Matrix_create(int rows, int cols){
    int i;
    int total=rows*cols;
    Matrix *m=(Matrix*)malloc(sizeof(Matrix));
    m->cols=cols;
    m->rows=rows;
    m->data=(double*)malloc(sizeof(double)*total);
    for (i=0;i<total;i++)m->data[i]=0.0;
    return(m);
}

Matrix *Matrix_create_identity(int rows){
    Matrix *m=Matrix_create(rows,rows);
    int i,j,d1=m->rows, d2=m->cols;
    double *p=m->data;
    for (i=0;i<d1;i++){
        for (j=0;j<d2;j++){
            if(i==j){
                *p=1;
            }else{
                *p=0;
            }
            p++;
        }
    }
    return(m);
}


void Matrix_dump(Matrix *m){
    int i,j,d1=m->rows, d2=m->cols;
    double *p=m->data;
    for (i=0;i<d1;i++){
        for (j=0;j<d2;j++){
            printf("%0.5f ",*p);
            p++;
        }
        printf("\n");
    }
}

Matrix *Matrix_add(Matrix *m1, Matrix *m2){
    Matrix* sum=Matrix_create(m1->rows, m1->cols);
    int total=m1->rows*m1->cols;
    int i;
    double *p1=m1->data;
    double *p2=m2->data;
    double *s1=sum->data;
    for (i=0;i<total;i++){
        *s1=*p1 + *p2;
        s1++;p1++;p2++;
    }
    return(sum);
}

Matrix *Matrix_sub(Matrix *m1, Matrix *m2){
    Matrix* sum=Matrix_create(m1->rows, m1->cols);
    int total=m1->rows*m1->cols;
    int i;
    double *p1=m1->data;
    double *p2=m2->data;
    double *s1=sum->data;
    for (i=0;i<total;i++){
        *s1=*p1 - *p2;
        s1++;p1++;p2++;
    }
    return(sum);
}

Matrix *Matrix_product_scaler(Matrix *m, double s){
    Matrix* newm=Matrix_create(m->rows, m->cols);
    int i,total=m->cols*m->rows;
    double *p=m->data;
    double *np=newm->data;
    for (i=0;i<total;i++){
        *np=*p*s;
        np++;p++;
    }
    return(newm);
}

double Matrix_get(Matrix *m, int i, int j){
    return(m->data[i*m->cols+j]);
}

void Matrix_set(Matrix *m, int i, int j, double value){
    m->data[i*m->cols+j]=value;
}

Matrix *Matrix_product(Matrix *m1, Matrix *m2){
        int i,j,y;
        Matrix *c=Matrix_create(m1->rows,m1->cols);
        for(i =0;i<m1->rows; i++){
            for(j=0;j<m2->cols; j++){
                for(y=0;y<m1->cols; y++){
                    Matrix_set(c,i,j,Matrix_get(c,i,j) + Matrix_get(m1,i,y) * Matrix_get(m2,y,j));
                }
            }

        }
        return(c);
}

Matrix *Matrix_transpose(Matrix *m){
    Matrix *newmatrix=Matrix_create(m->cols, m->rows);
    int i,j;
    for (i=0;i<m->rows;i++){
        for (j=0;j<m->cols;j++){
            Matrix_set(newmatrix,j,i,Matrix_get(m,i,j));
        }
    }
    return(newmatrix);
}

Matrix* Matrix_clone(Matrix *source){
    Matrix* newmatrix=Matrix_create(source->rows,source->cols);
    int total=source->rows * source->cols;
    int i;
    double *p=newmatrix->data;
    double *sourcedata=source->data;
    for (i=0;i<total;i++){
        *p=*sourcedata;
        p++;sourcedata++;
    }
    return(newmatrix);
}


Matrix *Matrix_echelon(Matrix* m){
    int cols=m->cols;
    int rows=m->rows;
    int i=0,j=0,k=0;
    Matrix *temp=Matrix_clone(m);
    double pivot;
    for (i=0;i<rows;i++){
    for(j = i + 1;j<rows;j++){
        pivot = -1*Matrix_get(temp,j,i) / Matrix_get(temp,i,i);
        for(k = 0; k < cols; k++){
            Matrix_set(temp,j,k, Matrix_get(temp,i,k) * pivot + Matrix_get(temp,j,k));
        }
    }
    }
    return(temp);
}

double Matrix_determinant(Matrix *m){
    Matrix *temp=Matrix_echelon(m);
    int i;
    double sum=1.0;
    for (i=0;i<m->rows;i++) {
        sum*=Matrix_get(temp,i,i);
    }
    return(sum);
}

Matrix*  Matrix_minor(Matrix *mat, int x, int y){
    Matrix *temp = Matrix_clone(mat);
    Matrix *newmat = Matrix_create(temp->rows - 1, temp->cols - 1);
    int i,j;
    int n = temp->rows;
    int m = temp->cols;
    int cnt = 0;
    double *myArr = (double*)malloc((n - 1) * (m - 1) * sizeof(double));
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(i != x && j != y){
                myArr[cnt] = Matrix_get(temp,i,j);
                cnt++;
            }
        }
    }
    cnt = 0;
    for(i = 0; i < n - 1; i++){
        for(j = 0; j < m - 1; j++){
            Matrix_set(newmat,i,j,myArr[cnt]);
            cnt++;
        }
    }

    return newmat;
}

Matrix *Matrix_inverse(Matrix *mat){
        double det=Matrix_determinant(mat);
        if(mat->rows != mat->cols || det == 0.0){
            return Matrix_create(mat->rows, mat->cols);
        }
        Matrix *temp = Matrix_clone(mat);
        int n = temp->rows;
        int m = temp->cols;
        int i,j;
        double tempdet,val;
        Matrix *Cofactor = Matrix_create(n, m);
        for(j = 0; j < m; j++){
            for(i = 0; i < n; i++){
                Matrix *Mintemp = Matrix_minor(temp,i, j);
                tempdet = Matrix_determinant(Mintemp);
                Matrix_set(Cofactor,i,j,tempdet);
            }
        }

        Cofactor = Matrix_transpose(Cofactor);
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                val=Matrix_get(Cofactor,i,j) * pow(-1.0,(i+j)) /det;
                if(isnan(val)) val=0;
                Matrix_set(Cofactor,i,j, val);
                //Cofactor.setValue(i, j, (Cofactor.getValue(i, j) * Math.pow(-1D, i + j + 2)) / det);
            }
        }

        return Cofactor;
    }


int Matrix_isIdentity(Matrix *m){
    int i,j,rows=m->rows,cols=m->cols;
    for (i=0;i<rows;i++){
        for (j=0;j<cols;j++){
            if(i==j && Matrix_get(m,i,j)!=1) return(0);
            if(i!=j && Matrix_get(m,i,j)!=0) return(0);
        }
    }
    return(1);
}



