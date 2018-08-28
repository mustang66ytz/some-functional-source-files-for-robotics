#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#define PI 3.14159265

template <typename T>
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

// A function to convert rotation matrix to euler angle and vice versa
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

// A function to convert rotation matrix to quaternion and vice versa
template <typename T>
bool convertRotationMatrix2Quaternion(vector<vector<T> >& matrix, vector<T>& quaternion){
    // matrix to quaternion:
    if(quaternion.size()==0){
        T s, w, x, y, z;
        T trace = matrix[0][0]+matrix[1][1]+matrix[2][2];
        if(trace>0){
            s = 0.5/sqrt(trace+1.0);
            w = 1/(4*s);
            x = (matrix[2][1]-matrix[1][2])*s;
            y = (matrix[0][2]-matrix[2][0])*s;
            z = (matrix[1][0]-matrix[0][1])*s;
        }
        else{
            if(matrix[0][0]>matrix[1][1] && matrix[0][0]>matrix[2][2]){
                s = 2.0 * sqrt( 1.0 + matrix[0][0] - matrix[1][1] - matrix[2][2]);
                w = (matrix[2][1] - matrix[1][2] ) / s;
                x = 0.25 * s;
                y = (matrix[0][1] + matrix[1][0] ) / s;
                z = (matrix[0][2] + matrix[2][0] ) / s;
            }
            else if (matrix[1][1] > matrix[2][2]) {
                s = 2.0 * sqrt( 1.0 + matrix[1][1] - matrix[0][0] - matrix[2][2]);
                w = (matrix[0][2] - matrix[2][0] ) / s;
                x = (matrix[0][1] + matrix[1][0] ) / s;
                y = 0.25 * s;
                z = (matrix[1][2] + matrix[2][1] ) / s;
                } 
            else {
                s = 2.0 * sqrt( 1.0 + matrix[2][2] - matrix[0][0] - matrix[1][1] );
                w = (matrix[1][0] - matrix[0][1] ) / s;
                x = (matrix[0][2] + matrix[2][0] ) / s;
                y = (matrix[1][2] + matrix[2][1] ) / s;
                z = 0.25 * s;
            }
        }
        quaternion.push_back(w);
        quaternion.push_back(x);
        quaternion.push_back(y);
        quaternion.push_back(z);
        return true;
    }
    // quaternion to matrix: 
    else if(matrix.size()==0){
        T w = quaternion[0];
        T x = quaternion[1];
        T y = quaternion[2];
        T z = quaternion[3];
        matrix = {{1-2*y*y-2*z*z, 2*x*y-2*z*w, 2*x*z+2*y*w}, 
                    {2*x*y+2*z*w, 1-2*x*x-2*z*z, 2*y*z-2*x*w}, 
                    {2*x*z-2*y*w, 2*y*z+2*x*w, 1-2*x*x-2*y*y}};
        return true;
    }
}


// A function to convert euler angles to quaternion and vice versa
template <typename T>
bool convertEuler2Quaternion(vector<T>& euler, vector<T>& quaternion){
    // euler to quaternion:
    if(quaternion.size() == 0){
        vector<vector<T>> matrix;
        bool success = convertRotationMatrix2EulerAngle(matrix, euler);
        success = convertRotationMatrix2Quaternion(matrix, quaternion);
    }
    // quaternion to euler:
    else if(euler.size() == 0){
        vector<vector<T>> matrix;
        bool success = convertRotationMatrix2Quaternion(matrix, quaternion);
        success = convertRotationMatrix2EulerAngle(matrix, euler);
    }
    return true;
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

    // test matrix to quaternion:
    vector<double> quaternion;
    success = convertRotationMatrix2Quaternion(matrix, quaternion);
    cout<<"The quaternion is:"<<endl;
    for(int i=0; i<quaternion.size(); i++){
        cout<<quaternion[i]<<endl;
    }

    // test quaternion to matrix:
    vector<vector<double>> matrix2;
    success = convertRotationMatrix2Quaternion(matrix2, quaternion);
    cout<<"The rotation matrix is:"<<endl;
    for(int i=0; i<matrix2.size(); i++){
        for(int j=0; j<matrix2[i].size(); j++){
            cout<<matrix2[i][j]<<' ';
        }
        cout<<endl;
    }
 
    // test euler to quaternion:
    vector<double> quaternion1;
    success = convertEuler2Quaternion(euler, quaternion1);
    cout<<"The quaternion is:"<<endl;
    for(int i=0; i<quaternion1.size(); i++){
        cout<<quaternion1[i]<<endl;
    }

    // test quaternion to euler:
    vector<double> euler2;
    success = convertEuler2Quaternion(euler2, quaternion1);
    cout<<"The euler angle is:"<<endl;
    for(int i=0; i<euler2.size(); i++){
        cout<<euler2[i]<<endl;
    }
    
    
    return 0;
}