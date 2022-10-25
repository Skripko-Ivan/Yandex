#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>
#include <algorithm>    // std::swap
#include <vector>       // std::vector

using namespace std;

void print(vector<vector<double>> &, int );
void pivotize(vector<vector<double>> &, vector<vector<double>> &, int , int );
void devide(vector<vector<double>> &, int , int , double );
void subtract_below(vector<vector<double>> &, vector<vector<double>> &, int , int );
void subtract_above(vector<vector<double>> &, vector<vector<double>> &, int , int );
void inverse(vector<vector<double>> &, vector<vector<double>> &, int );

#endif