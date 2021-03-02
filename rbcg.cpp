#include <bits/stdc++.h>
using namespace std;

int main(){
    string filename;
    cout << "Enter the name of the file : ";
    cin >> filename;
    int rows, columns;
    cout << "Enter the no of rows and columns : ";
    cin >> rows >> columns;

    fstream fp;
    fp.open(filename, ios::out);
    if(!fp){
        cout << "Can't open the file" << endl;
        return 0;
    }
    int k=1;
    fp << rows << " " << columns << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            fp << i+1 << " " << j+1 << " " << k  << " " << 0 << " " << (bool)false << " " << (bool)false << endl;
            k++;
        }
    }

    fp.close();
    return 0;
}