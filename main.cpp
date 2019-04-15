#include "lib.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include "logging_allocator.h"
#include "MyMatrix.h"
using namespace std;

template<typename T>
using Map = map<T, T, less<T> >;

template<typename T>
using MapA = map<T, T, less<T>, logging_allocator<pair<const T, T>>>;

int factorial(int i)
{
    if(i<0){cout<< "factorial error!!"; exit(0);}
    else if(i==0) return 1;
    else return i*factorial(i-1);
}
int main()
{
    cout << "map default\n";
    Map<int> m;
    for(size_t i=0; i<10; ++i) m.insert(make_pair(i, factorial(i)));
    for(const auto & [key, value] : m) 
        cout << "(key = " << key <<": value = "<<value<<")\n";
    cout << "===================="<<endl;
    // //////////////////////////////Allocator map///////////////////////////////////
    cout << "map alloc 48\n";
    Map<int> m0;
    //pair<const int, int>* p0;
    for(size_t i=0; i<10; ++i) 
    {
        auto p0 = m0.get_allocator().allocate(12);
        m0.get_allocator().construct(&p0[i], make_pair(i, factorial(i)));
        cout << "(key = " << p0[i].first <<": value = " << p0[i].second<<")\n";
        m0.get_allocator().destroy(&p0[i]);
        m0.get_allocator().deallocate(p0, 12);
    }
    cout << "===================="<<endl;
    // //////////////////////////////Logg_Allo//////////////////////////
    cout << "map alloc 480\n";
    Map<int> m1;
    auto p = m1.get_allocator().allocate(120);
    for(size_t i=0; i<10; ++i) 
    {
        m1.get_allocator().construct(&p[i], make_pair(i, factorial(i)));
    }
    for(size_t i=0; i<10; i++) 
    cout << "(key = " << p[i].first <<": value = " << p[i].second<<")\n";
    for(size_t i=0; i<10; ++i) 
    {
        m1.get_allocator().destroy(&p[i]);
    }
    m1.get_allocator().deallocate(p, 10);
    cout << "===================="<<endl;
    // //////////////////////////////Logg_Allocator map///////////////////////////////////////
    MapA<int> m2;
    
    for(size_t i=0; i<10; ++i) 
    {
        m2.insert(make_pair(i, factorial(i)));
        cout << endl;
        //cout << "(key = " << i <<": value = " << m2.at(i)<<")\n";
    }
    for(const auto & [key, value] : m2) 
        cout << "(key = " << key <<": value = "<<value<<")\n";
    cout << "===================="<<endl;
    /////////////////////////////////////////////////////////////
    cout << "my container matrix alloc\n";
    vector<vector<int> > matrix{
        {0,2,4},
        {7,9,3},
        {1,8,5}
        };
    auto p1 = allocator<int>{};
    MyMatrix<int> E(5), B(3), C(3);
    MyMatrix<int> A(matrix), D(matrix);
    cout <<"====================\n E ="<<endl;
    E.Show();
    cout <<"====================\n A ="<<endl;
    A.Show();
    B = A * A;
    cout <<"====================\n B ="<<endl;
    B.Show();
    C = D^2;
    cout <<"====================\n A^2 ="<<endl;
    C.Show();
    return 0;
}
