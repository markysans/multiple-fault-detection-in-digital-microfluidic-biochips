#include <bits/stdc++.h>
using namespace std;

class cmplx{
    int a;
    float b;
    public:
    cmplx(int x, float y){
        a= x;
        b=y;
    }

    void add(int a, float b){
        a = a+a;
        b=b+b;
    }
    void display(){
        cout << a << " "<< b;
    }
};

int main(){

    cmplx c(5, 2.4);
    c.add(2, 4.0);
    c.display();
    return 0;
}