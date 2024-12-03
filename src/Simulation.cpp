#include <iostream>
using namespace std;
#include <vector>
#include "Simulation.h"
using std::string;

#include <fstream>
#include <iostream>
#include "Auxiliary.h"

        Simulation::Simulation(const string &configFilePath):isRunning(false),planCounter(0){

            string currentLine;
            ifstream configFile(configFilePath);
            while(getline(configFile, currentLine)){
                if (currentLine.empty() || currentLine[0] == '#')//if we face an empty line or a comment we have to skip
                {
                    continue;//skips the current iteration
                }
                vector<string> tokens = Auxiliary::parseArguments(currentLine);
            if (tokens[0] == "settlement") {
                addSettlement(Settlement(tokens[1], static_cast<SettlementType>(stoi(tokens[2]))));//using the stoi function we turn the token into int then cast it to the Settlement type enum
            } 
            else if (tokens[0] == "facility") {
                addFacility(FacilityType(tokens[1], static_cast<FacilityCategory>(stoi(tokens[2])),
                    stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6])));
            } 
            else if (tokens[0] == "plan") {
                addPlan(getSettlement(tokens[1]), select(tokens[2]));
            }
        }
        configFile.close();
    }

        
        void Simulation::start()
        {
            open();
            cout<< "The simulation has started";
        }
        
        void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
        {
            Plan newplan = Plan(planCounter,settlement,selectionPolicy,facilitiesOptions);
            planCounter = planCounter + 1;
            plans.push_back(newplan);
        }
        
        void Simulation::addAction(BaseAction *action)
        {
            actionsLog.push_back(action);
        }
        
        bool Simulation::addSettlement(Settlement settlement)
        {
            bool flag = isSettlementExists(settlement.getName());
            if (!flag)
            {
                settlements.push_back(settlement);
                return false;//failed to add
            }
            return true;//succeeded to add
        }
        
        bool Simulation::addFacility(FacilityType facility)
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
        
        bool Simulation::isSettlementExists(const string &settlementName)
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
        Settlement &Simulation::getSettlement(const string &settlementName)
        {
            for(Settlement settelment: settlements)
            {
                if (settelment.getName()==settlementName)
                {
                    return settelment;
                }
            }
        }
        Plan &Simulation::getPlan(const int planID)
        {
            for(Plan plan: plans)
            {
                if (plan.getPlanId() == planID)
                {
                     return plan;
                }
            }
        }
        
        void Simulation::step()
        {
            if(isRunning)
            {
                for(Plan plan : plans)
                {
                    plan.step();
                }
            }
        }
        
        void Simulation::close()
        {
            isRunning = false;
        }

        Simulation::~Simulation()
        {
           for(BaseAction* action : actionsLog)
            {
                delete action;                                
            }
        }

        void Simulation::open()
        {
            isRunning = true;
        }

        vector<Plan> &Simulation::getPlans() 
        {
            return plans;
        }
        
        vector<FacilityType> &Simulation::getOptions() 
        {
            return facilitiesOptions;
        }
        
        bool Simulation::PlanExists(const int planID) 
        {
            for(Plan plan : plans)
            {
                if (plan.getPlanId() == planID)
                {
                    return true;
                }
            }
            return false;
        }
        vector<BaseAction*> &Simulation::getLogActions() 
        {
            return actionsLog;
        }

        SelectionPolicy* Simulation::select(const string& selectionPolicy){
            if(selectionPolicy == "nve"){
                return new NaiveSelection();
            }  
             else if(selectionPolicy == "bal"){
                return new BalancedSelection(0,0,0);
            } 
             else if(selectionPolicy == "eco"){
                return new EconomySelection();
            } 
             else if(selectionPolicy == "env"){
                return new SustainabilitySelection();
            }
        }