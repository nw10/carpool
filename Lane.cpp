//  Created by Wei Ni on 4/6/14.
//  Copyright (c) 2014 Wei Ni. All rights reserved.


#include <cmath>
#include <iostream>
#include "Lane.h"
#include "Vehicle.h"
#include "Censor.h"
#include "Demand.h"
#include "Utility.h"
#include "Macro.h"


// test done
void Lane::SetAdjLane(Lane * left_adj, Lane * right_adj){
    _left_adj = left_adj;
    _right_adj = right_adj;
}


// test done
Lane::~Lane(){
    for(auto &car: _car_list)
        delete car;
    for(auto &censor: _censor_list)
        delete censor;
}


// test done
Vehicle * Lane::PreWithId(Vehicle *current){
    Vehicle *pre = NULL;
    if(!_car_list.empty() && (*_car_list.begin())->_id != current->_id){
        auto iter = _car_list.begin();
        while(iter != _car_list.end()){
            if((*iter)->_id == current->_id) break;
            iter++;
        }
        pre = *(--iter);
    }
    return pre;
}


// test done
Vehicle * Lane::PreWithPosition(Vehicle *current){
    Vehicle *pre = NULL;
    if(!_car_list.empty() && (*_car_list.begin())->_position_old >= current-> _position_old){
        auto iter = _car_list.begin();
        while(iter != _car_list.end()){
            if((*iter)->_position_old < current->_position_old) break;
            iter++;
        }
        pre = *(--iter);
    }
    return pre;
}


// test done
Vehicle * Lane::AfterWithPosition(Vehicle *current){
    Vehicle *after = NULL;
    if(!_car_list.empty() && (*_car_list.rbegin())->_position_old < current->_position_old){
        auto iter = _car_list.begin();
        while(iter != _car_list.end()){
            if((*iter)->_position_old < current->_position_old) break;
            iter++;
        }
        after = *iter;
    }
    return after;
}


// test 
double Lane::SpeedForward(Vehicle *current, double length){
    double speed = FFSPEED;
    double temp = 0;
    int counter = 0;
    if(!_car_list.empty() && (*_car_list.begin())->_position_old >= (current->_position_old - 50)){
        auto iter = _car_list.begin();
        while(iter != _car_list.end() && (*iter)->_position_old >= (current->_position_old - 50)){
            if((*iter)->_position_old < current->_position_old + length){
                counter++;
                temp += (*iter)->_velocity_old;
            }
            iter++;
        }
        speed = temp / counter;
    }
    return speed;
}


// test done
double Lane::AccumulationForward(Vehicle *current, double length){
    int accumulation = 0;
    int counter = 0;
    if(!_car_list.empty() && (*_car_list.begin())->_position_old > current->_position_old){
        auto iter = _car_list.begin();
        while(iter != _car_list.end() && (*iter)->_position_old > current->_position_old){
            if((*iter)->_position_old < current->_position_old + length){
                counter++;
            }
            iter++;
        }
        accumulation = counter;
    }
    return (double)accumulation;
}


// test done
void Lane::InitialVehicle(int &id_recorder){
    Vehicle *first = new Vehicle(0, FFSPEED, 0, _lane_id, ++id_recorder, false, false, this);
    _car_list.push_back(first);
}


// test done
void Lane::AddDemand(double time_end, double flow_r, double flow_c){
    _demand.Add(time_end, flow_r, flow_c);
}


// test done, treat empty list
void Lane::Generate(double time_now, int &id_recorder){
    double lambda_r = _demand.GetFlowR(time_now);
    double lambda_c = _demand.GetFlowC(time_now);
    double flow = lambda_c + lambda_r;
    double dist, vel, temp;
    Vehicle * last = *_car_list.rbegin();
    if(last != *_car_list.end() && last->_position_old > S_JAM && time_now-_last_enter_time > 1.0/(flow/3600)){
        dist = max(0,min(last->_position_old-S_JAM,
                    (time_now-_last_enter_time-1.0/(flow/3600))*FFSPEED));
        temp = -2*A_L*(last->_position_old-dist-S_JAM +
                max(0,pow(-last->_velocity_old,2)/2/A_L-last->_velocity_old*DELTA_T/2));
        vel = max(0,min(FFSPEED, max(-A_L*DELTA_T, -A_L*DELTA_T/2.0+sqrt(temp>0?temp:0))));
        bool will_change = rand01() < lambda_c/flow? true : false;
        Vehicle * enter = new Vehicle(dist, vel, 0, _lane_id, ++id_recorder, false, will_change, this);
        _car_list.push_back(enter);
        _last_enter_time = time_now - dist / FFSPEED;
    }
}


// test done
void Lane::Update(){
    for(auto &i: _car_list){
        for(auto &censor: _censor_list) censor->Estimate(i);
        i->_position_old = i->_position_new;
        i->_velocity_old = i->_velocity_new;
    }
}


// test done
void Lane::KickOutRangeVehicle(){
    auto iter = _car_list.begin();
    while(!_car_list.empty() && (*iter)->_position_old > _end_point &&
            iter != _car_list.end()){
        delete *iter;
        iter = _car_list.erase(iter);
    }
}


// test done
void Lane::Say(double time_now){
    for(auto &i: _car_list){
        std::cout << time_now << " "
            << i->_lane << " "
            << i->_id << " "
            << i->_position_old << " "
            << i->_velocity_old << std::endl;
    }
}


// test done
void Lane::CensorSay(double time_now, std::ofstream &out){
    for(auto &i: _censor_list) i->Say(out, time_now);
}


// test done
void Lane::SendVehicleTo(double time_now, std::ofstream& lane_change_file){
    auto iter_vehicle = _car_list.begin();
    while(iter_vehicle != _car_list.end()){
        if((*iter_vehicle)->_send_me_to != NULL){
            Lane * target = (*iter_vehicle)->_send_me_to;
            auto iter = target->_car_list.begin();
            while(iter != target->_car_list.end()){
                if((*iter)->_position_new < (*iter_vehicle)->_position_new) break;
                iter++;
            }
            target->_car_list.insert(iter, *iter_vehicle);
            
            //write to lane_change_file
            lane_change_file << time_now << " "
                << (*iter_vehicle)->_id << " "
                << (*iter_vehicle)->_position_old << " "
                << (*iter_vehicle)->_lane << " "
                << target->_lane_id << std::endl;
            
            (*iter_vehicle)->_lane = target->_lane_id;
            (*iter_vehicle)->_current_lane = target;
            (*iter_vehicle)->_send_me_to = NULL;
            iter_vehicle = _car_list.erase(iter_vehicle);
        }
        else iter_vehicle++;
    }
}


