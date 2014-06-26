//  Created by Wei Ni on 4/6/14.
//  Copyright (c) 2014 Wei Ni. All rights reserved.

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "Utility.h"
#include "Macro.h"
#include "Lane.h"
#include "Vehicle.h"
#include "Censor.h"

int main()
{
    // random seed of time
    srand(time(NULL));

    // initialize the lane
    Lane *lanes[6];
    lanes[0] = new Lane(0, 0, 2500, 640, 2040, 0, 0, true);
    lanes[1] = new Lane(1, 0, 2500, 640, 2040, 0, 0, false);
    lanes[2] = new Lane(2, 0, 2500, 610, 2040, 0, 0, false);
    lanes[3] = new Lane(3, 0, 2500, 580, 1980, 0, 0, false);
    lanes[4] = new Lane(4, 0, 2500, 550, 1950, 0, 0, false);
    lanes[5] = new Lane(5, 0, 2500, 520, 1920, 0, 0, false);

    // set the adjacent lane for each lane
    lanes[0]->SetAdjLane(NULL, lanes[1]);
    lanes[1]->SetAdjLane(lanes[0], lanes[2]);
    lanes[2]->SetAdjLane(lanes[1], lanes[3]);
    lanes[3]->SetAdjLane(lanes[2], lanes[4]);
    lanes[4]->SetAdjLane(lanes[3], lanes[5]);
    lanes[5]->SetAdjLane(lanes[4], NULL);

    // demand 
    std::ifstream demand_file ("demand.txt");
    double time_end[6] = {0,0,0,0,0,0};
    int index;
    double time_cell, FR, FC;
    while(demand_file){
      demand_file >> index >> time_cell >> FR >> FC;
      time_end[index] += time_cell;
      lanes[index]->AddDemand(time_end[index], FR, FC);
    }

    // censor 
    std::ofstream censor_file ("censor_rec.txt");
    for(int i=0; i < 6; i++){
        lanes[i]->_censor_list.push_back(new Censor(200,i));
        lanes[i]->_censor_list.push_back(new Censor(2300,i));
    }

    //lane change recorder
    std::ofstream lane_change_file("lane_change_rec.txt");

    // set the time recorder and vehicle id recorder
    double time_now = 0;
    int id_recorder = 0;

    // initialize the first car in each lane
    for(auto &i: lanes) i->InitialVehicle(id_recorder);

    // start iteration
    std::list<Vehicle *>::iterator iter[6];
    for(; time_now < time_end[0]; time_now += DELTA_T){
        for(int i = 0; i < 6; i++) iter[i] = lanes[i]->_car_list.begin();
        while(1){
            int max_index = 0;
            double max_position = -INFINITE; 
            for(int i = 0; i < 6; i++){
                if(iter[i] != lanes[i]->_car_list.end()){
                    if(max_position < (*(iter[i]))->_position_old){
                        max_position = (*(iter[i]))->_position_old;
                        max_index = i;
                    }
                }
            } if(max_position == -INFINITE)  break;

            Vehicle *car = *(iter[max_index]);
            if(car->_cooperate) car->Cooperate();
            else{
                if(car->_wait_time > 0 && car->PMLaneChange(car->TargetLane(true)) > 0)
                    car->LaneChange(car->TargetLane(true));
                else{
                    if(rand01() < car->PMLaneChange(car->TargetLane(true)))
                        car->LaneChange(car->TargetLane(true));
                    else if(rand01() < car->POLaneChange(car->TargetLane(false)))
                        car->LaneChange(car->TargetLane(false));
                    else car->CarFollow();
                }
            }
            iter[max_index]++;
        }

        for(auto &i: lanes){
            i->Update();
            i->SendVehicleTo(time_now, lane_change_file);
            i->KickOutRangeVehicle();
            i->Generate(time_now, id_recorder);
            i->Say(time_now);
            i->CensorSay(time_now, censor_file);
        }
    }

    // manage memory
    demand_file.close();
    censor_file.close();
    lane_change_file.close();
    for(auto &i: lanes) delete i;
    return 0;
}

