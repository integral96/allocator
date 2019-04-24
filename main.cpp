#include "lib.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include "logging_allocator.h"
#include "MyMatrix.h"
using namespace std;

int factorial(int i)
{
    if(i<0) throw out_of_range("factorial error!");
    else if(i==0) return 1;
    else return i*factorial(i-1);
}
int main()
{
    cout << "map default\n";
    map<int, int, less<int> > m;
    for(size_t i=0; i<10; ++i) m.insert(make_pair(i, factorial(i)));
    for(const auto & [key, value] : m) 
        cout << "(key = " << key <<": value = "<<value<<")\n";
    cout << "===================="<<endl;
    ///////////////////////////////Allocator map///////////////////////////////////
    cout << "map alloc 48\n";
    map<int, int, less<int> > m0;
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
    map<int, int, less<int> > m1;
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
    cout << "map with template logging_allocator 480\n";
    map<int, int, less<int>, logging_allocator<pair<const int, int>>> m2;
    
        for(size_t i=0; i<10; ++i) 
        {
            m2.insert(make_pair(i, factorial(i)));
        }
        for(const auto & [key, value] : m2) 
            cout << "(key = " << key <<": value = "<<value<<")\n";
    cout << "===================="<<endl;
    ////////////////////////////////////////////////////////////////
    cout << "my container matrix alloc\n";
    vector<vector<int> > matrix{
        {0,2,4},
        {7,9,3},
        {1,8,5}
        };
    vector<vector<int> > matrix1{
        {2,5,4},
        {6,7,2},
        {1,0,3}
        };
    MyMatrix<int> E(6,6), E1(3,3), E2(3,3);
    MyMatrix<int> K1 = E1.CreateMatrix(matrix);
    MyMatrix<int> A1 = E1.CreateMatrix(matrix);
    MyMatrix<int> A2 = E1.CreateMatrix(matrix);
    MyMatrix<int> A3 = E1.CreateMatrix(matrix1);
    MyMatrix<int> B1 = E1.CreateMatrix(matrix);
    MyMatrix<int> D1 = E1.CreateMatrix(matrix);
    MyMatrix<int> C1(A1);
    
    E(2, 4) = 153;
    
    cout <<"====================\n E ="<<endl;
    E.Show();
    cout <<"====================\n A1 ="<<endl;
    A1.Show();
    A1 = A1 + A2;
    cout <<"====================\n A1 + A2 ="<<endl;
    A1.Show();
    A1 = A2 * 3;
    cout <<"====================\n A1 * 3 ="<<endl;
    A1.Show();
    A3 = A3 - A2;
    cout <<"====================\n A3 - A2 ="<<endl;
    A3.Show();
    B1 = B1 * B1;
    cout <<"====================\n B1 * B1 ="<<endl;
    B1.Show();
    D1 = D1^2;
    cout <<"====================\n B1^2 ="<<endl;
    D1.Show();
    cout <<"====================\n C1 ="<<endl;
    C1.Show();
    cout <<"====================\n K1 ="<<endl;
    K1.Show();
    cout <<"====================\n B1^2(i, 2) ="<<endl;
    for(size_t i=0; i<3; ++i)  cout <<D1(i, 2) << "\n";
    return 0;
}
