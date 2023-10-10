/***********************************************************************************************************************
 * @author Hackman Lo
 * @file cost_timer.h
 * @description 
 * @created_at 2023-10-10
***********************************************************************************************************************/

#ifndef OBELISK_COST_TIMER_H
#define OBELISK_COST_TIMER_H
#include <chrono>
#include <iostream>
namespace obelisk {

    class cost_timer {
    public:
        cost_timer(){
            begin = std::chrono::high_resolution_clock::now();
        }
        ~cost_timer(){
            auto cost = std::chrono::high_resolution_clock::now() - begin;
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(cost);
            std::cout << "Time Cost:" << milliseconds.count() << std::endl;
        }
    protected:
        std::chrono::high_resolution_clock::time_point begin;
    };

} // obelisk

#endif //OBELISK_COST_TIMER_H
