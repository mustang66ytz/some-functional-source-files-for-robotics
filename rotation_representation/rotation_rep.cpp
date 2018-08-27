#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#define PI 3.14159265

template <typename T>
// A function to convert rotation matrix to euler angle and vice versa
vector<vector<T> > matrixMultiply(vector<vector<T> > matrix1, vector<vector<T> > matrix2){
    vector<vector<T> > product;
    int N = matrix1.size();
    for(int i = 0; i<N; i++){
        vector<T> row;
        for(int j=0; j<N; j++){
            T temp = 0;
            for(int k=0; k<N; k++){
                temp += matrix1[i][k]*matrix2[k][j];
            }
            row.push_back(temp);
        }
        product.push_back(row);
    }
    return product;
}

template <typename T>
bool convertRotationMatrix2EulerAngle(vector<vector<T> >& matrix, vector<T>& angles){
    int row = 3;
    int col = 3;
    // if the rotation matrix is empty. convert angles to rotation matrix:
    if(matrix.size() == 0){    
        // get the rotation matrices for the three rotation axes first:
        vector<vector<T> > Rz0{{cos(angles[0]*PI/180), -sin(angles[0]*PI/180), 0},
                                {sin(angles[0]*PI/180), cos(angles[0]*PI/180), 0},
                                {0, 0, 1}};
        vector<vector<T> > Ry{{cos(angles[1]*PI/180), 0, sin(angles[1]*PI/180)},
                                {0, 1, 0},
                                {-sin(angles[1]*PI/180), 0, cos(angles[1]*PI/180)}};
        vector<vector<T> > Rz1{{cos(angles[2]*PI/180), -sin(angles[2]*PI/180), 0},
                                {sin(angles[2]*PI/180), cos(angles[2]*PI/180), 0},
                                {0, 0, 1}};
        matrix = matrixMultiply(matrixMultiply(Rz0, Ry), Rz1);
        return true;
    }
    // if the angles is empty, conver matrix to angles:
    else if(angles.size() == 0){
        
        T theta_z0, theta_y, theta_z1;
        // case 1: theta y is in (0, PI):
        if(matrix[2][2]>-1 && matrix[2][2]<1){
            theta_z0 = atan2(matrix[1][2], matrix[0][2])*180/PI;
            theta_y = acos(matrix[2][2])*180/PI;
            theta_z1 = atan2(matrix[2][1], -matrix[2][0])*180/PI;
        }
        // case 2: theta y is zero:
        else if(matrix[2][2] == 1){
            theta_z0 = atan2(matrix[1][0], matrix[1][1])*180/PI;
            theta_y = 0;
            theta_z1 = 0;
            cout<<"multiple solutions: theta_z0 + theta_z1 = "<<theta_z0<<":"<<endl;
        }
        // case 3: theta y is pi:
        else if(matrix[2][2] == -1){
            theta_z0 = 0;
            theta_y = PI;
            theta_z1 = atan2(matrix[1][0], matrix[1][1])*180/PI;
            cout<<"multiple solutions: theta_z1 - theta_z0 = "<<theta_z1<<":"<<endl;
        }
        angles.push_back(theta_z0);
        angles.push_back(theta_y);
        angles.push_back(theta_z1);
        return true;
    }
    
}  

// test case
int main(){
    // test euler to matrix:
    // define the euler angles (z-y-z):
    vector<double> euler{45.0, 0.0, 45.0};
    vector<vector<double>> matrix;
    // find the rotation matrix
    bool success = convertRotationMatrix2EulerAngle(matrix, euler);
    cout<<"The rotation matrix is:"<<endl;
    for(int i=0; i<matrix.size(); i++){
        for(int j=0; j<matrix[i].size(); j++){
            cout<<matrix[i][j]<<' ';
        }
        cout<<endl;
    }

    // test matrix to euler:
    // define the rotation matrix:
    vector<double> euler1;
    vector<vector<double>> matrix1{{-0.146447, -0.853553, 0.5},{0.853553, 0.146447, 0.5 },{-0.5, 0.5, 0.707107}};
    // find the rotation matrix
    success = convertRotationMatrix2EulerAngle(matrix, euler1);
    cout<<"The euler angle zyz is:"<<endl;
    for(int i=0; i<euler1.size(); i++){
        cout<<euler1[i]<<endl;
    }

    return 0;
}