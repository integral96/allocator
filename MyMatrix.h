#ifndef MYMATRIX_H
#define MYMATRIX_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

template <typename MT, typename ...Args>
class MyMatrix
{
    private:
        MT **arr;
        MT **tmp;
       // int num_matx=0;
       // static const int NLIM = 80;
        int N;
    public:
        //static int sizeN;
        MyMatrix(int n) : N(n)
        {
            if((N<0)){cout<< "size error!!"; exit(0);}
            arr = new MT* [N];
            for(int i = 0; i<N;i++){ arr[i] = new MT [N]; }
            for(int i = 0; i<N;i++){
                for(int j = 0; j<N;j++)
                    {
                        arr[i][j] = 0;
                        if(j==i) arr[i][j] = 1;
                    }
                }
            //num_matx++;
            //cout <<__PRETTY_FUNCTION__<<"==="<< num_matx <<endl;
        }
        ///////////////////////////////////////////////////////////////
        MyMatrix(const vector<vector<MT> > &x):N(x.size())
        {
            //srand(time(0));
            if((N<0)){cout<< "size error!!"; exit(0);}
            arr = new MT* [N];
            for(int i = 0; i<N;i++){ arr[i] = new MT [N]; }
            for(int i = 0; i<N;i++){
                for(int j = 0; j<N;j++){
                        arr[i][j] = x[i][j];//.arr[i][j]; //+(rand()%109-37)/10;
                    }
                }
            //num_matx++;
            //cout <<__PRETTY_FUNCTION__<<"==="<< num_matx <<endl;
        }
        //////////////////////////////////////////////////////////////////////
        virtual ~MyMatrix() {
            //num_matx--;
            //cout<<"DELETE ARR: "<<num_matx<<endl;
            for(int i = 0; i<N; i++) delete [] arr[i];
            delete [] arr;
            //cout <<__PRETTY_FUNCTION__<<"==="<< num_matx <<endl;
        }
        ///////////////////////////////////////////////////////////////////////////
        MyMatrix &operator+(const MyMatrix &mx){
            for(int i = 0; i<N;++i){
                for(int j = 0; j<N;++j){
                    arr[i][j] += mx.arr[i][j];
                }
            }
            return *this;
        }
        /////////////////////////////////////////////////////////////////////////////
        MyMatrix &operator+(int a){
            for(int i = 0; i<N;++i){
                for(int j = 0; j<N;++j){
                    arr[i][j] += a;
                }
            }
            return *this;
        }
        /////////////////////////////////////////////////////////////////////////////////
        MyMatrix &operator-(const MyMatrix &mx){
            for(int i = 0; i<N;++i){
                for(int j = 0; j<N;++j){
                    arr[i][j] -= mx.arr[i][j];
                }
            }
            return *this;
        }
        /////////////////////////////////////////////////////////////////////////////////
        MyMatrix &operator*(const MyMatrix &mx){
            tmp = new MT* [N];
            for(int i = 0; i<N;i++)
            tmp[i] = new MT [N];
            for (int i = 0; i<N; i++)
                for (int j = 0; j<N; j++)
                    arr[i][j] = mx.arr[i][j];
            for (int i = 0; i<N; i++)
                for (int j = 0; j<N; j++)
                    for (int k = 0; k<N; k++)
                        tmp[i][j] += mx.arr[i][k] * arr[k][j];
            for (int i = 0; i<N; i++)
                for (int j = 0; j<N; j++)
                    { 
                        mx.arr[i][j] = tmp[i][j];
                        tmp[i][j] = 0;
                    }
            for(int i = 0; i<N; i++) delete [] tmp[i];
            delete [] tmp;
            return *this;
        }
        ///////////////////////////////////////////////////////////////////////////
        MyMatrix &operator=(const MyMatrix &mx) {
            if(this==&mx) return *this;
            for(int i = 0; i<N;++i){
                for(int j = 0; j<N;++j){
                    arr[i][j] = mx.arr[i][j];
                }
            }
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////////
        MyMatrix &operator*(double b) {
            for (int i = 0; i<N; i++)
                for (int j = 0; j<N; j++)
                    arr[i][j] = this->arr[i][j];
            for(int i = 0; i<N;i++){
                for(int j = 0; j<N;j++){
                    arr[i][j]*=b;
                }
            }
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////////
        MyMatrix &operator/(double b) {
            for(int i = 0; i<N;i++){
                for(int j = 0; j<N;j++){
                    arr[i][j]/=b;
                }
            }
            return *this;
        }
        ///////////////////////////////////////////////////////////////////////////
        
        MyMatrix &operator^ (int M) { 
        if((M<0)){cout<< "size error!!"; exit(0);}
            int m, n, k, p = 1;
            tmp = new MT* [N];
            for(m = 0; m<N;m++)
                tmp[m] = new MT [N];
                for(int i = 0; i<N;i++){
                    for(int j = 0; j<N;j++)
                        {
                            tmp[i][j] = 0;
                            if(j==i && M==0) tmp[i][j] = 1;
                        }
                }
        if(M==1) 
            {
                for (m = 0; m<N; m++)
                    for (n = 0; n<N; n++)
                        tmp[m][n] = arr[m][n];
            }
        else if(M>1)
            {
            while (p++ < M) {
                for (m = 0; m<N; m++)
                    for (n = 0; n<N; n++)
                        for (k = 0; k<N; k++)
                            tmp[m][n] += this->arr[m][k]* arr[k][n];
                for (m = 0; m<N; m++)
            for (n = 0; n<N; n++) { 
                arr[m][n] = tmp[m][n];
                tmp[m][n] = 0;
                }
                }
            }
            for(int i = 0; i<N; i++) delete [] tmp[i];
            delete [] tmp;
            return *this;
        }
        ///////////////////////////////////////////////////////////////////////////
        void Show()
        {
            for(int i = 0; i<N;++i){
                for(int j = 0; j<N;++j){
                    cout << setw(3)<<setprecision(4)<<arr[i][j]<<" ";
                    if(j==N-1) cout<<endl;
                }
            }
        }
    protected:


};

#endif // MYMATRIX_H
