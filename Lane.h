//  Created by Wei Ni on 4/6/14.
//  Copyright (c) 2014 Wei Ni. All rights reserved.


#ifndef LANE_H
#define LANE_H

#include <list>
#include "Demand.h"

class Vehicle;
class Censor;
class Demand;

class Lane{
    public:
        double _start_point;
        double _end_point;
        double _left_tran_start;
        double _left_tran_end;
        double _right_tran_start;
        double _right_tran_end;
        double _last_enter_time;
        int _lane_id;
        Lane * _left_adj;
        Lane * _right_adj;
        bool _hov;
        std::list<Vehicle *> _car_list;
        std::list<Censor *> _censor_list;
        Demand _demand;
        

        Lane(int lane_id, double start_point, double end_point,
                double left_tran_start, double left_tran_end,
                double right_tran_start, double right_tran_end, bool hov):
            _lane_id(lane_id), _start_point(start_point), _end_point(end_point),
            _left_tran_start(left_tran_start), _left_tran_end(left_tran_end),
            _right_tran_start(right_tran_start), _right_tran_end(right_tran_end), _hov(hov),
            _left_adj(NULL), _right_adj(NULL){}

        void SetAdjLane(Lane * left_adj, Lane * right_adj);

        ~Lane();

        Vehicle * PreWithId(Vehicle *current);

        Vehicle * PreWithPosition(Vehicle *current);

        Vehicle * AfterWithPosition(Vehicle *current);

        double SpeedForward(Vehicle *current, double length);

        double AccumulationForward(Vehicle *current, double length);

        void InitialVehicle(int &id_recorder);

        void AddDemand(double time_end, double flow_r, double flow_c);

        void Generate(double time_now, int &id_recorder);

        void Update();

        void SendVehicleTo(double time_now, std::ofstream& lane_change_file);

        void KickOutRangeVehicle();

        void Say(double time_now);

        void CensorSay(double time_now, std::ofstream &out);
};

#endif
