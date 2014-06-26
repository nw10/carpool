#include "Censor.h"
#include "Vehicle.h"

void Censor::Estimate(Vehicle *car){
    if(car->_position_old < _position && car->_position_new >= _position)
        _recorder++;
}

void Censor::Say(std::ofstream &out, double time_now){
    out << _lane_id << " " << _position << " "
        << time_now << " " << _recorder << std::endl;
}
