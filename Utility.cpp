//  Created by Wei Ni on 4/6/14.
//  Copyright (c) 2014 Wei Ni. All rights reserved.


#include<ctime>
#include<cstdlib>
#include "Utility.h"

double max(double a, double b){
    return a>b ? a : b;
}

double min(double a, double b){
    return a<b ? a : b;
}

double rand01(){
    return (double)rand()/RAND_MAX;
}

