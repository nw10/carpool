//  Created by Wei Ni on 4/6/14.
//  Copyright (c) 2014 Wei Ni. All rights reserved.


#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>

class Lane;

class Vehicle{
    public:
        double _position_old; 
        double _velocity_old; 
        double _position_new; 
        double _velocity_new; 
        double _wait_time;
        int _lane; 
        int _id; 
        bool _hov;
        bool _cooperate; 
        bool _will_change;
        Lane *_send_me_to;
        Lane *_current_lane;

        Vehicle(double position, double velocity, double wait_time,
                int lane, int id, bool hov, bool will_change, Lane * current_lane):
                _position_old(position),_velocity_old(velocity),
                _wait_time(wait_time), _lane(lane),_id(id), _hov(hov),
                _cooperate(false),_will_change(will_change), 
                _send_me_to(NULL), _current_lane(current_lane){}

        void CarFollow();

        void ReduceMe(Lane *target);

        void Cooperate();

        double PMLaneChange(Lane *target);

        double POLaneChange(Lane *target);

        Lane * TargetLane(bool is_mlc);

        void LaneChange(Lane *target);
};

#endif
