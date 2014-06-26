#ifndef _DEMAND_H
#define _DEMAND_H

#include <list>
#include <tuple>


class Demand{
    public:
        std::list< std::tuple<double,double,double> > _demand_list;

        Demand(){}
        void Add(double time_end, double flow_r, double flow_c);
        double GetFlowR(double time_now);
        double GetFlowC(double time_now);
};

#endif 
