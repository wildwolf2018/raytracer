#ifndef MATRIX_H
#define MATRIX_H
#include "Vec3.h"
#include <iostream>

/*void cameraToWorld(Vec3 cameraSpace, Vec3 *result)
{
    const int m = 4;
    const int p = 4;
    const int q = 1;
    int c, d, k, sum = 0;
    float first[10][10], second[10][10], multiply[10][10];

    //first matrix
    first[0][0] = 0.4;
    first[0][1] = 0.2;
    first[0][2] = -0.8;
    first[0][3] = 0.0;
    first[1][0] = -0.4;
    first[1][1] = 0.9;
    first[1][2] = 0.0;
    first[1][3] = 0.0;
    first[2][0] = 0.8;
    first[2][1] = 0.3;
    first[2][2] = 0.4;
    first[2][3] = 0.0;
    first[3][0] = 5.4;
    first[3][1] = 3.0;
    first[3][2] = -0.1;
    first[3][3] = 1.0;

    //second matrix
    second[0][0] = cameraSpace.x;
    second[1][0] = cameraSpace.y;
    second[2][0] = cameraSpace.z;
    second[3][0] = 1;

    for (c = 0; c < m; c++)
    {
      for (d = 0; d < q; d++)
      {
        for (k = 0; k < p; k++)
        {
          sum = sum + first[c][k]*second[k][d];
        }
        multiply[c][d] = sum;
     //   std::cout << multiply[c][d] << std::endl;
        sum = 0;
      }
    }

    //result matrix
    result->x = multiply[0][0];
    result->y = multiply[1][0];
    result->z = multiply[2][0];
 //   std::cout << result->x << "  " << result->y << " " << result->z << std::endl;
}**/
#endif
