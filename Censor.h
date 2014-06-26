#ifndef _CENSOR_H
#define _CENSOR_H

#include <iostream>
#include <fstream>

class Vehicle;

class Censor{
    public:
        double _position;
        int _lane_id;
        int _recorder;

        Censor(double position, int lane_id):
            _position(position), _lane_id(lane_id), _recorder(0){}

        void Estimate(Vehicle *car);
        void Say(std::ofstream &out, double time_now);
};

#endif
