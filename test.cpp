#include <bits/stdc++.h>
using namespace std;

struct node{
    int x, y, id, fault, visited, present;
};

int detectFaultLocation(int timecycle){
    // if(timecycle == 0)
}

vector<int> getPath(node** nodelist, int rows, int columns, int row){
    vector<int> path;
    for(int i = 0; i <= row; i++)
        path.push_back(nodelist[i][0].id);
    for(int i=1; i<columns; i++)
        path.push_back(nodelist[row][i].id);
    for(int i=row+1; i<rows; i++)
        path.push_back(nodelist[i][columns-1].id);
    
    return path;
}

void moveDroplet(node** nodelist, int rows, int columns, int srcid, int destid){
    int srcx = (srcid-1)/columns;
    int srcy = ((srcid-1)%columns);

    int destx = (destid-1)/columns;
    int desty = ((destid-1)%columns);

    if(nodelist[srcx][srcy].present == 1){
        nodelist[srcx][srcy].present = 0;
        nodelist[destx][desty].present = 1;
    }
}

int detectOpenFault(node** nodelist, int rows, int columns, vector<int> path, int row){

    int l = path.size();
    
    queue<int> q;
    int index = 1;
    q.push(index);
    int timecycle = 0;

    int cnt = 0;
    while(index < l-1 || !q.empty()){
        if(nodelist[0][0].present == 1){
            nodelist[0][0].present = 0;
            return timecycle;
        }
        int size = q.size();

        // @this block will be executed in 1 timecycle
        while(size--){
            int curr = q.front();
            q.pop();
            int srcid = path[curr];
            int destid = path[curr-1];
            moveDroplet(nodelist, rows, columns, srcid, destid);
            cout << curr << " ";
            if(curr != 1){
                q.push(curr-1);
            }

        }
        cout << endl;

        timecycle++;
        cnt++;
        if(cnt == 2 || q.empty()){
            index++;
            if(index < l-1)
                q.push(index);
            cnt = 0;
        }
    }
    if(nodelist[0][0].present == 1){
            nodelist[0][0].present = 0;
            return timecycle;
    }

    return -1;
}

int main(){
    
    string filename;
    cout << "Enter the name of the file : ";
    cin >> filename;
    fstream fp;
    fp.open(filename, ios::in);
    if(!fp){
        cout << "Can't open the file" << endl;
        return 0;
    }
    int rows, columns;
    fp >> rows >> columns;


    // @nodelist is used to access the the nodes by their row and column index
    node** nodelist = new node*[rows];
    for(int i=0; i<rows; i++)
        nodelist[i] = new node[columns];
    
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++)
            fp >> nodelist[i][j].x >> nodelist[i][j].y >> nodelist[i][j].id >> nodelist[i][j].fault >> nodelist[i][j].visited >> nodelist[i][j].present;
    }
    
    cout << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++)
            cout << nodelist[i][j].present << " ";
        cout << endl;
    }

    vector<int> path = getPath(nodelist, rows, columns, rows-1);

    for(int i=0; i<path.size(); i++)
        cout << path[i] << " ";
    cout << detectOpenFault(nodelist, rows, columns, path, rows-1) << endl; 
  

    cout << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++)
            cout << nodelist[i][j].present << " ";
        cout << endl;
    }  


    for(int i=0; i<rows; i++)
        delete[] nodelist[i];
    delete[] nodelist;
    fp.close();
    return 0;
}