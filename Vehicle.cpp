//  Created by Wei Ni on 4/6/14.
//  Copyright (c) 2014 Wei Ni. All rights reserved.


#include <cmath>
#include "Vehicle.h"
#include "Lane.h"
#include "Utility.h"
#include "Macro.h"

// test done
void Vehicle::CarFollow(){
    Lane *current = _current_lane;
    Vehicle *pre = current->PreWithId(this);
    double pre_velocity = pre==NULL? FFSPEED : pre->_velocity_old;
    double pre_position = pre==NULL? _position_old + 100 : pre->_position_old;
    double delta_x_U = min(FFSPEED * DELTA_T, _velocity_old * DELTA_T + A_U * DELTA_T * DELTA_T);
    double delta_x_C = pre_position - _position_old - S_JAM;
    double delta_x_L = max(0, _velocity_old * DELTA_T + A_L * DELTA_T * DELTA_T);
    double d_n_minus_1 = max(0, -pre_velocity * pre_velocity / 2.0 / A_L
            - pre_velocity * DELTA_T / 2.0);
    double delta_x_S = max(0, A_L * DELTA_T * DELTA_T / 2.0 +
            DELTA_T * sqrt(-2.0 * A_L * ((pre_position - _position_old) + d_n_minus_1)));
    _position_new = _position_old + max(delta_x_L, min(delta_x_U, min(delta_x_S, delta_x_C)));
    _velocity_new = (_position_new - _position_old) / DELTA_T;
}

// test done
void Vehicle::Cooperate(){
    double delta_x_L = max(0, _velocity_old * DELTA_T + A_L * DELTA_T * DELTA_T);
    _position_new = _position_old + delta_x_L;
    _velocity_new = (_position_new - _position_old) / DELTA_T;
    if(_wait_time != 0) _wait_time += DELTA_T;
    _cooperate = false; 
}

// test done 
void Vehicle::ReduceMe(Lane *target){
    double delta_x_L = max(0, _velocity_old * DELTA_T + A_L * DELTA_T * DELTA_T);
    double adj_speed = target->SpeedForward(this, 100);
    adj_speed = 0;
    _position_new = _position_old + max(delta_x_L, (adj_speed + _velocity_old) / 2.0 * DELTA_T);
    _velocity_new = (_position_new - _position_old) / DELTA_T;
    if(_wait_time != 0) _wait_time += DELTA_T;
}

// test done
double Vehicle::PMLaneChange(Lane* target){
    Lane * current = _current_lane;
    if(!_will_change) return -1;
    else if(_position_old < current->_left_tran_start || _position_old > current->_left_tran_end) 
        return -1;
    else{
        double x_1 = current->AccumulationForward(this, 100) - target->AccumulationForward(this, 100);
        double x_2 = (current->_left_tran_end - current->_left_tran_start) / (current->_left_tran_end - _position_old);
        double x_3 = _lane == 0? 1 : _lane;//TODO
        double utility = -BETA_0 - BETA_1 * x_1 - BETA_2 * x_2 - BETA_3 * x_3;
        return 1.0 / (1.0 + exp(utility));
    }
}

// test done 
double Vehicle::POLaneChange(Lane* target){
    Lane *current = _current_lane;
    if(_will_change && _position_old > current->_left_tran_start &&
            _position_old < current->_left_tran_end) return 0;
    if(current->_lane_id == target->_lane_id) return 0;
    double current_speed = current->SpeedForward(this, 100);
    double target_speed = target->SpeedForward(this, 100);
    return (target_speed - current_speed) / FFSPEED * DELTA_T / PHI;
}

// test done
Lane * Vehicle::TargetLane(bool is_mlc){
    Lane * target;
    if(is_mlc){
        if(_current_lane->_left_adj != NULL)
            target = _current_lane->_left_adj;
        else target = _current_lane->_right_adj;
    }
    else{
        double middle, left, right;

        if(_current_lane->_hov) middle = INFINITE;
        else middle = _current_lane->SpeedForward(this, 100);

        if(_current_lane->_left_adj == NULL) left = -1;
        else if(_current_lane->_left_adj->_hov) left = -1;
        else left = _current_lane->_left_adj->SpeedForward(this, 100);

        if(_current_lane->_right_adj == NULL) right = -1;
        else if(_current_lane->_right_adj->_hov) right = -1;
        else right = _current_lane->_right_adj->SpeedForward(this, 100);

        if(right > left) target = right > middle ? _current_lane->_right_adj : _current_lane;
        else target = left > middle ? _current_lane->_left_adj : _current_lane;
    }
    return target == NULL? _current_lane : target;
}


// test done
void Vehicle::LaneChange(Lane *target){
    Lane * current = _current_lane;
    double target_speed = target->SpeedForward(this, 100);
    if(_wait_time > PSI && _velocity_old >= target_speed) ReduceMe(target);
    else{
        Vehicle *pre_adj = target->PreWithPosition(this);
        Vehicle *after_adj = target->AfterWithPosition(this);

        double pre_adj_position_old = pre_adj==NULL? _position_old + 200 : pre_adj->_position_old;
        double after_adj_position_old = after_adj==NULL? _position_old - 200 : after_adj->_position_old;
        double pre_adj_velocity_new = pre_adj==NULL? FFSPEED : pre_adj->_velocity_new;
        double after_adj_velocity_old = after_adj==NULL? FFSPEED : after_adj->_velocity_old;

        double x_n_minus_1_adj = pre_adj_position_old;
        double s_n_adj = x_n_minus_1_adj - _position_old;
        double x_n_add_1_adj = after_adj_position_old;
        double s_n_add_1_adj = _position_old - x_n_add_1_adj;

        double v_n_minus_1_adj_next = pre_adj_velocity_new;
        double d_n_minus_1_adj_next = max(0, -pow(v_n_minus_1_adj_next,2)/2/A_L  - v_n_minus_1_adj_next*DELTA_T/2);

        double delta_x_U = min(FFSPEED * DELTA_T, _velocity_old * DELTA_T + A_U * DELTA_T * DELTA_T);
        double delta_x_L = max(0, _velocity_old * DELTA_T + A_L * DELTA_T * DELTA_T);

        double temp = -2*A_L*((s_n_adj - S_JAM + v_n_minus_1_adj_next*DELTA_T + d_n_minus_1_adj_next));
        double delta_x_S = max(0, A_L*pow(DELTA_T,2)/2 + DELTA_T*sqrt(temp>0? temp : 0));

        double v_u_n_add_1_adj = max(0, DELTA_T*A_U + after_adj_velocity_old);
        double d_n = max(0, -pow(_velocity_old,2)/2/A_L - _velocity_old*DELTA_T/2);
        double s_min_n_add_1_adj = S_JAM - d_n + v_u_n_add_1_adj*DELTA_T/2 - pow(v_u_n_add_1_adj,2)/2/A_L -A_L*pow(DELTA_T,2)/8;

        if(_wait_time > PSI && (s_n_add_1_adj < S_JAM || s_n_add_1_adj < s_min_n_add_1_adj)){
            after_adj->_cooperate = true;
            v_u_n_add_1_adj = max(0, DELTA_T*A_L + after_adj_velocity_old);
            d_n = max(0, -pow(_velocity_old,2)/2/A_L - _velocity_old*DELTA_T/2);
            s_min_n_add_1_adj = S_JAM - d_n + v_u_n_add_1_adj*DELTA_T/2 - pow(v_u_n_add_1_adj,2)/2/A_L -A_L*pow(DELTA_T,2)/8;
        } 

        if(s_n_adj > S_JAM && s_n_add_1_adj > S_JAM && s_n_add_1_adj > s_min_n_add_1_adj){
            _position_new = _position_old + max(delta_x_L, min(delta_x_S, delta_x_U));
            _velocity_new = (_position_new -_position_old) / DELTA_T;
            _wait_time = 0;
            if((current->_hov && !target->_hov) || (!current->_hov && target->_hov))    
                _will_change = false;
            _send_me_to = target;
        }
        else{
            _wait_time += DELTA_T;
            CarFollow();
        }
    }
}


