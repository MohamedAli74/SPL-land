#include <iostream>
using namespace std;
#include <vector>
#include "Simulation.h"
using std::string;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start()
        {
            cout<< "The simulation has started";
        }
        
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
        {
            Plan newplan = Plan(planCounter,settlement,selectionPolicy,facilitiesOptions);
            planCounter = planCounter + 1;
            plans.push_back(newplan);
        }
        
        void addAction(BaseAction *action)
        {
            actionsLog.push_back(action);
        }
        
        bool addSettlement(Settlement settlement)
        {
            bool flag = isSettlementExists(settlement.getName());
            if (!flag)
            {
                settlements.push_back(settlement);
                return false;//failed to add
            }
            return true;//succeeded to add
        }
        
        bool addFacility(FacilityType facility)
        {
            bool flag = true;
            for(FacilityType facility1: facilitiesOptions)
            {
                if (facility1.getName() == facility.getName())
                {
                    flag = false;
                }
            }
            if (!flag)
            {
                facilitiesOptions.push_back(facility);
                return true;//succeeded to add
            }
            return false;//failed to add
            
        }
        
        bool isSettlementExists(const string &settlementName)
        {
            bool flag = true;
            for(Settlement settelment: settlements)
            {
                if (settelment.getName()==settlementName)
                {
                    flag = false;
                }
            }
            return flag;

        }
        Settlement &getSettlement(const string &settlementName)
        {
            for(Settlement settelment: settlements)
            {
                if (settelment.getName()==settlementName)
                {
                    return settelment;
                }
            }
        }
        Plan &getPlan(const int planID)
        {
            for(Plan plan: plans)
            {
                if (plan.getPlanId() == planID)
                {
                     return plan;
                }
            }
        }
        
        void step()
        {
            if(isRunning)
            {
                for(Plan plan : plans)
                {
                    plan.step();
                }
            }
        }
        
        void close()
        {
            isRunning = false;
        }

        //~Simulation()
        //{
           // for(BaseAction* action : actionsLog)
            //{
            //    delete(action);
              //  delete backup;
            //}

       // }

        void open()
        {
            isRunning = true;
        }

        vector<Plan> &getPlans() 
        {
            return plans;
        }
        
        vector<FacilityType> &getOptions() 
        {
            return facilitiesOptions;
        }
        bool PlanExists(const int planID) 
        {
            
        }
        vector<BaseAction*> &getLogActions() 
        {
            return actionsLog;
        }

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement> settlements;
        vector<FacilityType> facilitiesOptions;
};