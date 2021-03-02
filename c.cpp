#include <bits/stdc++.h>
using namespace std;


int rec(int n){
    return rec(n-1)*rec(n-2);
}
int main(){

    cout << rec(6);
    return 0;
}