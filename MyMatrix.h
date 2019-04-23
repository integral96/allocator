#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

template <typename MT, typename Alloc = std::allocator<MT> >
class MyMatrix
{
    private:
        MT *arr = nullptr;
        MT *tmp = nullptr;
        Alloc _alloc;
        unsigned N, M;
        void Clean()
            {
                if(arr) 
                {
                    for(size_t i = 0; i<N;i++)
                    {
                        for(size_t j = 0; j<M;j++)
                        {
                            _alloc.destroy(&arr[M*i+j]);
                        }
                    }
                _alloc.deallocate(arr, N*M);
                arr = nullptr;
                }
                //std::cout << "\nDTOR";
            }
    public:
        MyMatrix(unsigned n, unsigned m) : N(n), M(m)
        {
            if(N <= 0 || M <= 0) throw std::out_of_range("Matrix constructor has 0 size");
            arr =_alloc.allocate(N*M);
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    _alloc.construct(&arr[M*i+j], 0);
                    if(j==i) _alloc.construct(&arr[M*i+j], 1);
                }
            }
            //std::cout << "\nCTOR_DEF";
        }
        ///////////////////////////////////////////////////////////////////////////
        MyMatrix(const MyMatrix &x):N(x.N), M(x.M)
        {
            if((N<=0 || M<=0)) throw std::out_of_range("Matrix constructor has 0 size");
            arr =_alloc.allocate(N*M);
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    _alloc.construct(&arr[M*i+j], x.arr[M*i+j]);
                }
            }
            //std::cout << "\nCTOR_COPY";
        }
        /////============================
        const MyMatrix &operator=(const MyMatrix &mx) {
            if(this==&mx) return *this;
            for(size_t i = 0; i<N;++i){
                for(size_t j = 0; j<M;++j){
                    arr[M*i+j] = mx.arr[M*i+j];
                }
            }
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////
        MyMatrix(MyMatrix &&x): arr(std::move(x.arr)), N(x.N), M(x.M)
        {
            x.arr = nullptr;
            //std::cout << "\nCTOR_MOVE";
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
        const MyMatrix &operator+(const MyMatrix &mx)
        {
            for(size_t i = 0; i<N;++i)
            {
                for(size_t j = 0; j<M;++j)
                {
                    arr[M*i+j] += mx.arr[M*i+j];
                }
            }
            return *this;
        }
        /////////////////////////////////////////////////////////////////////////////
        const MyMatrix &operator+(MT a)
        {
            for(size_t i = 0; i<N;++i)
            {
                for(size_t j = 0; j<M;++j)
                {
                    arr[M*i+j] += a;
                }
            }
            return *this;
        }
        /////////////////////////////////////////////////////////////////////////////////
        const MyMatrix &operator-(const MyMatrix &mx)
        {
            for(size_t i = 0; i<N;++i)
            {
                for(size_t j = 0; j<M;++j)
                {
                    arr[M*i+j] -= mx.arr[M*i+j];
                }
            }
            return *this;
        }
        /////////////////////////////////////////////////////////////////////////////////
        const MyMatrix &operator*(const MyMatrix &mx)
        {
            if (M != N) throw std::out_of_range("Matrix subscript out of bounds");
            tmp =_alloc.allocate(N*M);
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    arr[M*i+j] = mx.arr[M*i+j];
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    for (size_t k = 0; k<N; k++)
                        tmp[M*i+j] += mx.arr[M*i+k] * arr[M*k+j];
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    { 
                        mx.arr[M*i+j] = tmp[M*i+j];
                        tmp[M*i+j] = 0;
                    }
            _alloc.deallocate(tmp, N*M);
            tmp = nullptr;
            return *this;
        }
        
        ////////////////////////////////////////////////////////////////////////////
        const MyMatrix &operator*(MT b) 
        {
            for (size_t i = 0; i<N; i++)
                for (size_t j = 0; j<M; j++)
                    arr[M*i+j] = this->arr[M*i+j];
            for(size_t i = 0; i<N;i++)
                for(size_t j = 0; j<M;j++)
                    arr[M*i+j]*=b;
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////////
        const MyMatrix &operator/(MT b) {
            for(size_t i = 0; i<N;i++){
                for(size_t j = 0; j<M;j++){
                    arr[M*i+j]/=b;
                }
            }
            return *this;
        }
        ///////////////////////////////////////////////////////////////////////////
        
        const MyMatrix &operator^ (size_t P) { 
            if (M != N) throw std::out_of_range("Matrix subscript out of bounds");
            tmp =_alloc.allocate(N*M);
                size_t m, n, k, p = 1;
                for(size_t i = 0; i<N;i++)
                    {
                    for(size_t j = 0; j<M;j++)
                        {
                            _alloc.construct(&tmp[M*i+j], 0);
                            if(j==i && P == 0) _alloc.construct(&tmp[M*i+j], 1);
                        }
                    }
            if(P == 1) 
                {
                for (m = 0; m<N; m++)
                    for (n = 0; n<M; n++)
                        _alloc.construct(&tmp[M*m+n], arr[M*m+n]);
                }
            else if(P > 1)
                {
                while (p++ < P) 
                    {
                    for (m = 0; m<N; m++)
                        for (n = 0; n<M; n++)
                            for (k = 0; k<N; k++)
                                tmp[M*m+n] += this->arr[M*m+k]* arr[M*k+n];
                    for (m = 0; m<N; m++)
                        for (n = 0; n<M; n++) 
                        { 
                            arr[M*m+n] = tmp[M*m+n];
                            tmp[M*m+n] = 0;
                        }
                    }
                }
            for(size_t i = 0; i<N;i++)
            {
                for(size_t j = 0; j<M;j++)
                {
                    _alloc.destroy(&tmp[M*i+j]);
                }
            }
            _alloc.deallocate(tmp, N*M);
            tmp = nullptr;
            return *this;
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

