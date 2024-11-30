#include "../include/Action.h"
#include <iostream>
using namespace std;
#include <string>
////////////////////////////BaseAction////////////////////////////

    BaseAction::BaseAction()
        {
        }

    ActionStatus BaseAction::getStatus() const
        {
            return status;
        }

        void BaseAction::complete()
        {
            this-> status = ActionStatus :: COMPLETED;
        }

        void BaseAction::error(string errorMsg)
        {
            this -> status = ActionStatus :: ERROR;
            std::cout << "Error: " << errorMsg;
        }
        
        const string &BaseAction::getErrorMsg() const
        {
            return errorMsg;
        }

////////////////////////////SimulateStip////////////////////////////
    
        SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps)
        {}

        void SimulateStep::act(Simulation &simulation) {
            for(Plan &p : simulation.getPlans()){
                p.step();
            }
        }
        
        const string SimulateStep::toString() const {
            return "step " + numOfSteps;
        }
        
        SimulateStep *SimulateStep::clone() const {
            return new SimulateStep(*this);
        }

////////////////////////////AddPlan////////////////////////////

        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
        :settlementName(settlementName),selectionPolicy(selectionPolicy),flag(true),policy(nullptr)
        {
            if(selectionPolicy == "nve"){
                policy = new NaiveSelection();
            }  
             else if(selectionPolicy == "bal"){
                policy = new BalancedSelection(0,0,0);
            } 
             else if(selectionPolicy == "eco"){
                policy = new EconomySelection();
            } 
             else if(selectionPolicy == "env"){
                policy = new SustainabilitySelection();
            }  
            else{
                flag = false;
            }
        }
        
        void AddPlan::act(Simulation &simulation) 
        {
            if (!simulation.isSettlementExists(settlementName)|!flag)
            {
                error("Cannot create this plan");
            }
            else
            {
                simulation.addPlan(simulation.getSettlement(settlementName),policy);
                complete();
            }
        }
        
        const string AddPlan::toString() const 
        {
            if (getStatus() == ActionStatus ::COMPLETED)
            {
                return "Plan added: " + settlementName+" " + selectionPolicy + " COMPLETED";
            }
            else
            {
                return "Cannot create this plan";
            }
        }
        
        AddPlan *AddPlan::clone() const 
        {
            new AddPlan(*this);
        }



////////////////////////////AddSettlement////////////////////////////

        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType)
            :settlementName(settlementName),settlementType(settlementType){}
        
        void AddSettlement::act(Simulation &simulation) {
            if(simulation.isSettlementExists(settlementName)){
                this->error("Settlement already exists");
            }else{
            simulation.addSettlement(Settlement(settlementName , settlementType));
            this->complete();
            }
        }
        
        AddSettlement *AddSettlement::clone() const {
            return new AddSettlement(*this);
        }
        
        const string AddSettlement::toString() const {
            return "settlement " + settlementName + " " + std::to_string(int(settlementType));
        }

////////////////////////////AddFacility////////////////////////////

        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
        :BaseAction(),facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(economyScore)
        {
            flag = true;
        }
        
        void AddFacility::act(Simulation &simulation) 
        {
            FacilityType newfacitype(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
            flag = simulation.addFacility(newfacitype);
            if (flag)
            {
                complete();
            }
            else
            {
                error("Facility already exists");
            }
            
        }
        
        AddFacility *AddFacility::clone() const {
            return new AddFacility(*this);
        }
        
        const string AddFacility::toString() const 
        {
            if (ActionStatus::COMPLETED == getStatus())
            {
                return "Facility added: " +facilityName+" " + to_string(int(facilityCategory)) + to_string(price) + to_string(lifeQualityScore)+ " "+to_string(economyScore)+" " + to_string(economyScore) + " Completed";
            }
            else
            {
                return "Facility already exists";
            }
        }
    
////////////////////////////PrintPlanStatus////////////////////////////


        PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}
        
        void PrintPlanStatus::act(Simulation &simulation) {
            if(simulation.PlanExists(planId)){
                std::cout << simulation.getPlan(planId).toString() ;
                complete();
            }else{
                error("Plan doesn’t exist");
            }
        }
        
        PrintPlanStatus *PrintPlanStatus::clone() const {
            return new PrintPlanStatus(*this);
        }
        
        const string PrintPlanStatus::toString() const 
        {
            if (ActionStatus::COMPLETED == getStatus())
            {
                return "planId: "+planId;
            }
            else
            {
                return "Plan doesn't exist";
            }
        }
        }

////////////////////////////ChangePolicy////////////////////////////

        ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
        :planId(planId),newPolicy(newPolicy){ s = "";       }
        
        void ChangePlanPolicy::act(Simulation &simulation) {
            if(simulation.PlanExists(planId)){
                Plan & p = simulation.getPlan(planId);
                s = p.getPolicy()->toString();
                if(s == newPolicy){
                    error("Cannot change selection policy");
                }else{
                    p.setSelectionPolicy(SelectPolicy(newPolicy,p));
                    complete();
                }
            }
        }

        ChangePlanPolicy *ChangePlanPolicy::clone() const {
            return new ChangePlanPolicy(*this);
        }

        const string ChangePlanPolicy::toString() const 
        {
            if (ActionStatus::COMPLETED == getStatus())
            {
                return "planId: "+to_string(planId) + "/n previousPolicy: "+s+ "/n newPolicy: "+ newPolicy; 
            }
            else
            {
                return "Cannot change selection policy";
            }
        }

        SelectionPolicy *ChangePlanPolicy::SelectPolicy(const string& policy, Plan& p ){
            if(policy == "nve"){
                return new NaiveSelection();
            }else if(policy == "bal"){
                return new BalancedSelection(p.getlifeQualityScore(), p.getEconomyScore(), p.getEnvironmentScore());
            }else if(policy == "eco"){
                return new EconomySelection();
            }else if(policy == "env"){
                return new SustainabilitySelection();
            }else{
                return nullptr;
            }
        }

////////////////////////////PrintActionsLog////////////////////////////


    PrintActionsLog::PrintActionsLog(){}
    void PrintActionsLog::act(Simulation &simulation) {}
    PrintActionsLog *PrintActionsLog::clone() const {}
    const string PrintActionsLog::toString() const {}


////////////////////////////Close////////////////////////////

class Close : public BaseAction {
    public:
        Close();
        void act(Simulation &simulation) ;
        Close *clone() const ;
        const string toString() const ;
    private:
};

////////////////////////////BackupSimulation////////////////////////////

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation();
        void act(Simulation &simulation) ;
        BackupSimulation *clone() const ;
        const string toString() const ;
    private:
};

////////////////////////////RestoreSimulation////////////////////////////

class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation();
        void act(Simulation &simulation) ;
        RestoreSimulation *clone() const ;
        const string toString() const ;
    private:
};
