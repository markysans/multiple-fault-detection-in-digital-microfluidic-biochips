#include <bits/stdc++.h>
using namespace std;

struct node{
    int x, y, id, fault, visited, present;
};

// @type of fault represented by the fault value
// 0 means no fault
// 1 open fault
// 2 sort fault
// @visited represent us if the electrode is visited and brought to our notice( 1: yes, 0: no )
// @present represent us if any droplet is present at that electrode( 0: no, 1: completely present, 2: partially present )


// @given an id x co-ordinate can be calculated as ((id-1)/columns + 1 )
// @given an id y co-ordinate can be calculated as ((id-1)%columns + 1)

int getFaultLocation(vector<int> path, int clock){
    if(clock == 0)
        return path[1];
    if(clock == 1)
        return path[2];
    if(clock == 3)
        return path[3];
    clock = clock/3 + 2;
    return path[clock];
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
            if(curr != 1){
                q.push(curr-1);
            }

        }

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

void markVisited(node** nodelist, int rows, int columns, vector<int> &path){
    // @path vector is passed by reference
    int size = path.size();
    for(int i=0; i<size; i++){
        int id = path[i];
        
        int x = (id-1)/columns + 1;
        int y = (id-1)%columns + 1;
        nodelist[x-1][y-1].visited = 1;
    }
}

int DRDTest(node** nodelist, int rows, int columns, int row){
    
    vector<int> path;
    bool flag = 1;
    node current;

    // @droplet moving down
    for(int k = 0; k <= row; k++){
        if(flag == 0)
            break;
        current = nodelist[k][0];

        // @checking for open fault
        if(current.fault == 1){
            flag = 0;
            if(current.fault == 1){
                flag = 0;
                if(k > 0)
                    nodelist[k-1][0].present = 1;
            }
        }

        // @checking for sort fault
        if(current.fault == 2 && nodelist[k+1][0].fault == 2 && k < (row-1)){
            flag = 0;
            nodelist[k][0].present = 2;
            nodelist[k+1][0].present = 2;
        }
        path.push_back(current.id);

    }
    // @check for sort fault during direction change
    if(nodelist[row][0].fault == 2 && nodelist[row][1].fault == 2){
        flag = 0;
        nodelist[row][0].present = 2;
        nodelist[row][1].present = 2;
    }

    // @droplet moving right
    for(int i=1; i<columns; i++){
        if(flag == 0)
            break;
        current = nodelist[row][i];

        // @checking for open fault
        if(current.fault == 1){
            flag = 0;
            nodelist[row][i-1].present = 1;
        }

        // @checking for sort fault
        if(current.fault == 2 && nodelist[row][i+1].fault == 2 && i<(columns-2)){
            flag = 0;
            nodelist[row][i].present = 2;
            nodelist[row][i+1].present = 2;
        }


        path.push_back(current.id);
    }

    // @check for sort fault during direction change
    if(nodelist[row][columns-1].fault == 2 && nodelist[row+1][columns-1].fault == 2){
        flag = 0;
        nodelist[row][columns-1].present = 2;
        nodelist[row+1][columns-1].present = 2;
    }

    // @droplet moving down
    for(int j=row+1; j<rows; j++){
        if(flag == 0)
            break;
        current = nodelist[j][columns-1];
        // @checking for open fault
        if(current.fault == 1){
            flag = 0;
            nodelist[j-1][columns-1].present = 1;
        }

        // @checking for sort fault
        if(current.fault == 2 && nodelist[j+1][columns-1].fault == 2 && j<(rows-1)){
            flag = 0;
            nodelist[j][columns-1].present = 2;
            nodelist[j+1][columns-1].present = 2;
        }

        path.push_back(current.id);
    }

    if(flag){
        markVisited(nodelist, rows, columns, path);
        path.clear();
        return 1;
    }
    return 0;
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


    // @nodearray is kept to accesc the nodes by their id
    // node* nodearray = new node[rows*columns];
    // for(int i=0; i<rows; i++){
    //     for(int j=0; j<columns; j++){
    //         int index = i*columns + j;
    //         nodearray[index].x = nodelist[i][j].x;
    //         nodearray[index].y = nodelist[i][j].y;
    //         nodearray[index].id = nodelist[i][j].id;
    //         nodearray[index].fault = nodelist[i][j].fault;
    //         nodearray[index].visited = nodelist[i][j].visited;
    //         nodearray[index].present = nodelist[i][j].present;
    //     }
    // }


    // @check [0][0] electrode is not faulty
    
    // @peripheral test
    int t1, t2;
    t1 = DRDTest(nodelist, rows, columns, 0);
    t2 = DRDTest(nodelist, rows, columns, rows-1);

    if(t1 == 0){
        vector<int> path = getPath(nodelist, rows, columns, 0);
        int clock = detectOpenFault(nodelist, rows, columns, path, 0);
        cout << getFaultLocation(path, clock) << endl;
    }
    if(t2 == 0){
        vector<int> path = getPath(nodelist, rows, columns, rows-1);
        int clock = detectOpenFault(nodelist, rows, columns, path, rows-1);
        cout << getFaultLocation(path, clock) << endl;
    }

    

    cout << rows << " " << columns << endl;


    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++)
            cout << nodelist[i][j].fault << " ";
        cout << endl;
    }

    cout << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++)
            cout << nodelist[i][j].visited << " ";
        cout << endl;
    }  

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

// @algorithm can not detect sort fault at the boundary