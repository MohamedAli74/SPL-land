#include <iostream>
using namespace std;
#include <vector>
#include "../include/Simulation.h"
using std::string;

#include <fstream>
#include <iostream>
#include "Auxiliary.h"
#include "../include/Action.h"

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
                addSettlement(new Settlement(tokens[1], static_cast<SettlementType>(stoi(tokens[2]))));//using the stoi function we turn the token into int then cast it to the Settlement type enum
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
            string currentLine;
            while(isRunning){
                getline(cin,currentLine);
                vector<string> tokens = Auxiliary::parseArguments(currentLine);

                if (tokens[0] == "settlement") {
            BaseAction *newSettlement = new AddSettlement(tokens[1], static_cast<SettlementType>(stoi(tokens[2])));
            newSettlement->act(*this);
            addAction(newSettlement);
            } 
            else if (tokens[0] == "facility") {

                BaseAction *newFacility = new AddFacility(tokens[1], static_cast<FacilityCategory>(stoi(tokens[2])),stoi(tokens[3]),stoi(tokens[4]),stoi(tokens[5]), stoi(tokens[6]));
                newFacility->act(*this);
                addAction(newFacility);
            } 
            else if (tokens[0] == "plan") {
                BaseAction *newPlan = new AddPlan(tokens[1], tokens[2]);
                newPlan ->act(*this);
                addAction(newPlan);
            } 
            else if (tokens[0] == "step") {
                const int numOfStep = stoi(tokens[1]);
                BaseAction *newStep = new SimulateStep(numOfStep);
                newStep ->act(*this);
                addAction(newStep);
            }
            else if (tokens[0] == "planStatus") {
                BaseAction *planStatus = new PrintPlanStatus(stoi(tokens[1]));
                planStatus ->act(*this);
                addAction(planStatus);
            } 
            else if (tokens[0] == "changePolicy") {
                BaseAction *changePolicy = new ChangePlanPolicy(stoi(tokens[1]), tokens[2]);

                changePolicy ->act(*this);
                addAction(changePolicy);
            } 
            else if (tokens[0] == "log") {
                BaseAction *printActionsLog = new PrintActionsLog();
                printActionsLog ->act(*this);
                addAction(printActionsLog);
            }
            else if (tokens[0] == "close") {
                BaseAction *close = new Close();
                close ->act(*this);
                addAction(close);
            }  
            else if (tokens[0] == "backup") {
                BaseAction *backupSimulation = new BackupSimulation();
                backupSimulation ->act(*this);
                addAction(backupSimulation);
            }  
            else if (tokens[0] == "restore") {
                BaseAction *restoreSimulation = new RestoreSimulation();
                restoreSimulation ->act(*this);
                addAction(restoreSimulation);
            } 
            else {
            cout << "no comment like this" << endl;
            }
        }
    }
        
        void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
        {
            Plan newplan(planCounter,settlement,selectionPolicy,facilitiesOptions);
            planCounter = planCounter + 1;
            plans.push_back(newplan);
        }
        
        void Simulation::addAction(BaseAction *action)
        {
            actionsLog.push_back(action);
        }
        
        bool Simulation::addFacility(FacilityType facility)
        {
            bool flag = false;
            for(FacilityType facility1: facilitiesOptions)
            {
                if (facility1.getName() == facility.getName())
                {
                    flag = true;
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
<<<<<<< HEAD
=======
            bool flag = false;
>>>>>>> 6b84e7bd17ba4479b83b3d10616e29a1eb897301
            for(Settlement *settlement: settlements)
            {
                if (settlement->getName()==settlementName)
                {
<<<<<<< HEAD
                    return true;
=======
                    flag = true;
>>>>>>> 6b84e7bd17ba4479b83b3d10616e29a1eb897301
                }
            }
            return false;

        }
        bool Simulation::addSettlement(Settlement *settlement)
        {
            bool flag = isSettlementExists(settlement->getName());
            if (flag == false)
            {
                settlements.push_back(settlement);
                flag = isSettlementExists(settlement->getName());
                return true;//succeeded to add
            }
            return false;//failed to add
        }
        
        Settlement &Simulation::getSettlement(const string &settlementName)
        {
            for(Settlement *settlement: settlements)
            {
                if (settlement->getName()==settlementName)
                {
                    return *settlement;
                }
            }
        }
        Plan &Simulation::getPlan(const int planID)
        {
            for(Plan &plan: plans)
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
                for(Plan &plan : plans)
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
            for(Plan &plan : plans)
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

        SelectionPolicy* Simulation::select(const string& selectionPolicy) const{
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
        ///Rule of 5///
        Simulation::Simulation(const Simulation& other)
        :isRunning(other.isRunning),planCounter(other.planCounter),actionsLog(),plans(other.plans),settlements(other.settlements),facilitiesOptions(other.facilitiesOptions){
            for(BaseAction* a : other.actionsLog ){
                actionsLog.push_back(a->clone());
            }
        }

        Simulation::Simulation(Simulation&& other)
        :isRunning(other.isRunning),planCounter(other.planCounter),actionsLog(),plans(other.plans),settlements(other.settlements),facilitiesOptions(other.facilitiesOptions){
            for(BaseAction* a : other.actionsLog ){
                actionsLog.push_back(a);
            }
            other.actionsLog.clear();
        }

        Simulation Simulation::operator=(const Simulation& other){
            if(this != &other){
            isRunning=other.isRunning;
            planCounter=other.planCounter;

            plans=vector<Plan>();
            for(const Plan &p : other.plans){
                plans.push_back(p);
            }
            
            settlements=other.settlements;

            facilitiesOptions=vector<FacilityType>();
            for(const FacilityType &p : other.facilitiesOptions)
            {
                facilitiesOptions.push_back(p);
            }
            for(BaseAction* a : actionsLog ){
                delete a;
            }
            actionsLog.clear();

            for(BaseAction* a : other.actionsLog ){
                actionsLog.push_back(a->clone());
            }
            }
            return *this;
        }

        Simulation Simulation::operator=(Simulation&& other){
            if(this != &other){
            isRunning=other.isRunning;
            planCounter=other.planCounter;

            plans=vector<Plan>();
            for(const Plan &p : other.plans){
                plans.push_back(p);
            }

            for(Settlement* s : settlements ){
                delete s;
            }

            settlements.clear();
            
            for(Settlement* s : other.settlements ){
                settlements.push_back(s);
            }

            facilitiesOptions=vector<FacilityType>();
            for(const FacilityType &p : other.facilitiesOptions)
            {
                facilitiesOptions.push_back(p);
            } 

            for(BaseAction* a : actionsLog ){
                delete a;
            }

            actionsLog.clear();
            
            for(BaseAction* a : other.actionsLog ){
                actionsLog.push_back(a);
            }
            other.actionsLog.clear();
            }

        return *this;    
        }
        