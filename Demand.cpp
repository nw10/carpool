#include "Demand.h"

void Demand::Add(double time_end, double flow_r, double flow_c){
    _demand_list.push_back(std::make_tuple(time_end, flow_r, flow_c));
}

double Demand::GetFlowC(double time_now){
    for(auto &i: _demand_list){
        if(std::get<0>(i) > time_now)
            return std::get<2>(i);
    }
    return 0;
}

double Demand::GetFlowR(double time_now){
    for(auto &i: _demand_list){
        if(std::get<0>(i) > time_now)
            return std::get<1>(i);
    }
    return 0;
}

