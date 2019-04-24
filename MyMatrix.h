#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

template <typename MT, typename Alloc = std::allocator<MT> >
class MyMatrix
{
    private:
        MT *arr ;
        MT *tmp ;
        Alloc alloc;
        unsigned N, M;
        void Clean()
            {
                if(arr) 
                {
                    for(size_t i = 0; i<N;i++)
                    {
                        for(size_t j = 0; j<M;j++)
                        {
                            alloc.destroy(&arr[M*i+j]);
                        }
                    }
                alloc.deallocate(arr, N*M);
                arr = nullptr;
                }
            }
    public:
        MyMatrix(unsigned n, unsigned m) : N(n), M(m)
        {
            if(N <= 0 || M <= 0) throw std::out_of_range("Matrix constructor has 0 size");
            arr =alloc.allocate(N*M);
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    alloc.construct(&arr[M*i+j], 0);
                    if(j==i) alloc.construct(&arr[M*i+j], 1);
                }
            }
        }
        ///////////////////////////////////////////////////////////////////////////
        MyMatrix(const MyMatrix &x):N(x.N), M(x.M)
        {
            if((N<=0 || M<=0)) throw std::out_of_range("Matrix constructor has 0 size");
            arr =alloc.allocate(N*M);
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    alloc.construct(&arr[M*i+j], x.arr[M*i+j]);
                }
            }
        }
        /////============================
        MyMatrix operator=(const MyMatrix &mx) const 
        {
            MyMatrix mresult(*this);
            for(size_t i = 0; i<N;++i){
                for(size_t j = 0; j<M;++j){
                    mresult.arr[M*i+j] = mx.arr[M*i+j];
                }
            }
            return mresult;
        }
        ////////////////////////////////////////////////////////////////////////
        MyMatrix(MyMatrix &&x): arr(std::move(x.arr)), N(x.N), M(x.M)
        {
            x.arr = nullptr;
            x.N = 0;
            x.M = 0;
        }
        MyMatrix & operator=(MyMatrix &&x)
        {
            Clean();
            N = x.N;
            M = x.M;
            arr = x.arr;
            x.arr = nullptr;
            x.N = 0;
            x.M = 0;
            return *this;
        }
        //////////////////////////////////////////////////////////////////
        MyMatrix CreateMatrix(const std::vector<std::vector<MT> > &x)
        {
            N = x.size();
            M = x.size();
            if((N<=0 || M<=0)) throw std::out_of_range("Matrix constructor has 0 size");
            MyMatrix matrix(N, M);
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    matrix(i, j)=x[i][j];
                }
            }
            return matrix;
        }
        //////////////////////////////////////////////////////////////////////
        virtual ~MyMatrix() 
        {
            Clean();
            N=M=0;
        }
        MT& operator() (unsigned n, unsigned m)
        {
            if (n >= N || m >= M) throw std::out_of_range("Matrix subscript out of bounds");
            return arr[M*n + m];
        }
        ///////////////////////////////////////////////////////////////////////////
        MyMatrix operator+(const MyMatrix &mx) const 
        {
            MyMatrix mresult(*this);
            for(size_t i = 0; i<N;++i)
            {
                for(size_t j = 0; j<M;++j)
                {
                    mresult.arr[M*i+j] += mx.arr[M*i+j];
                }
            }
            return mresult;
        }
        /////////////////////////////////////////////////////////////////////////////
        MyMatrix operator+(MT a) const 
        {
            MyMatrix mresult(*this);
            for(size_t i = 0; i<N;++i)
            {
                for(size_t j = 0; j<M;++j)
                {
                    mresult.arr[M*i+j] += a;
                }
            }
            return mresult;
        }
        /////////////////////////////////////////////////////////////////////////////////
        MyMatrix operator-(const MyMatrix &mx) const
        {
            MyMatrix mresult(*this);
            for(size_t i = 0; i<N;++i)
            {
                for(size_t j = 0; j<M;++j)
                {
                    mresult.arr[M*i+j] -= mx.arr[M*i+j];
                }
            }
            return mresult;
        }
        /////////////////////////////////////////////////////////////////////////////////
        MyMatrix operator*(const MyMatrix &mx)
        {
            if (M != N) throw std::out_of_range("Matrix subscript out of bounds");
            MyMatrix mresult(*this);
            tmp =alloc.allocate(N*M);
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    mresult.arr[M*i+j] = mx.arr[M*i+j];
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    for (size_t k = 0; k<N; k++)
                        tmp[M*i+j] += mx.arr[M*i+k] * mresult.arr[M*k+j];
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    { 
                        mresult.arr[M*i+j] = tmp[M*i+j];
                        tmp[M*i+j] = 0;
                    }
            alloc.deallocate(tmp, N*M);
            tmp = nullptr;
            return mresult;
        }
        ////////////////////////////////////////////////////////////////////////////
        MyMatrix operator*(const MT& b) const
        {
            MyMatrix mresult(*this);
            for(size_t i = 0; i<N;i++)
                for(size_t j = 0; j<M;j++)
                    mresult.arr[M*i+j]*=b;
            return mresult;
        }
        ////////////////////////////////////////////////////////////////////////////
        MyMatrix operator/(const MT& b) const 
        {
            MyMatrix mresult(*this);
            for(size_t i = 0; i<N;i++){
                for(size_t j = 0; j<M;j++){
                    mresult.arr[M*i+j]/=b;
                }
            }
            return mresult;
        }
        ///////////////////////////////////////////////////////////////////////////
        
        MyMatrix operator^ (size_t P)
        { 
            if (M != N) throw std::out_of_range("Matrix subscript out of bounds");
            MyMatrix mresult(*this);
            tmp =alloc.allocate(N*M);
                size_t m, n, k, p = 1;
                for(size_t i = 0; i<N;i++)
                    {
                    for(size_t j = 0; j<M;j++)
                        {
                            alloc.construct(&tmp[M*i+j], 0);
                            if(j==i && P == 0) alloc.construct(&tmp[M*i+j], 1);
                        }
                    }
            if(P == 1) 
                {
                for (m = 0; m<N; m++)
                    for (n = 0; n<M; n++)
                        alloc.construct(&tmp[M*m+n], mresult.arr[M*m+n]);
                }
            else if(P > 1)
                {
                while (p++ < P) 
                    {
                    for (m = 0; m<N; m++)
                        for (n = 0; n<M; n++)
                            for (k = 0; k<N; k++)
                                tmp[M*m+n] += mresult.arr[M*m+k]* mresult.arr[M*k+n];
                    for (m = 0; m<N; m++)
                        for (n = 0; n<M; n++) 
                        { 
                            mresult.arr[M*m+n] = tmp[M*m+n];
                            tmp[M*m+n] = 0;
                        }
                    }
                }
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    alloc.destroy(&tmp[M*i+j]);
                }
            }
            alloc.deallocate(tmp, N*M);
            tmp = nullptr;
            return mresult;
        }
        ///////////////////////////////////////////////////////////////////////////
        size_t SizeM()
        {
            return N;
        }
        void Show()
        {
            for(size_t i = 0; i<N;++i){
                for(size_t j = 0; j<M;++j){
                    std::cout << std::setw(3)<<std::setprecision(4)<<arr[M*i + j]<<" ";
                    if(j==M-1) std::cout<<std::endl;
                }
            }
        }
    protected:


};

