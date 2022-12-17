#include <iostream>
#include "matrix.hpp"
using namespace std;

int main()
{
    int data[27]={255,0,0,255,0,0,255,0,0,0,255,0,0,255,0,0,255,0
    ,0,0,255,0,0,255,0,0,255};
    int data2[27]={1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1};
    matrix<int,3> mat1(3,3,data);
    matrix<int,3> mat2(3,3,data2);
    matrix<int,3> mat3(mat1,Rect(1,1,2,2));
    mat3.print_matrix();

    mat1.clone(mat3);
    cout<<(mat1==mat3)<<endl;
}

