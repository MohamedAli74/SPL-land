#include "../include/Action.h"
#include <iostream>
using namespace std;
#include <string>
extern Simulation* backup;

////////////////////////////BaseAction////////////////////////////

    BaseAction::BaseAction():errorMsg(),status(){}

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
            std::cout << "Error:" << errorMsg << endl;
        }
        
        const string &BaseAction::getErrorMsg() const
        {
            return errorMsg; 
        }

////////////////////////////SimulateStip////////////////////////////
    
        SimulateStep::SimulateStep(const int numOfSteps):BaseAction(),numOfSteps(numOfSteps)
        {}

        void SimulateStep::act(Simulation &simulation) {
            for (int i = 0; i < numOfSteps; i++){
                for(Plan &p : simulation.getPlans()){
                    p.step();
                }
            }
        }
        
        const string SimulateStep::toString() const { 
            return "step " +  to_string(numOfSteps) +" COMPLETED";
        }
        
        SimulateStep *SimulateStep::clone() const {
            return new SimulateStep(*this);
        }

////////////////////////////AddPlan////////////////////////////

        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
        :BaseAction(),settlementName(settlementName),selectionPolicy(selectionPolicy),flag(true)
        {
            flag = (selectionPolicy == "nve")||(selectionPolicy == "bal")||(selectionPolicy == "eco")||(selectionPolicy == "env");
        }
        
        void AddPlan::act(Simulation &simulation) 
        {
            if (!simulation.isSettlementExists(settlementName)|| !flag)
            {
                error("Cannot create this plan");
            }else{
                simulation.addPlan(simulation.getSettlement(settlementName),simulation.select(selectionPolicy));
                complete();
            }
        }
        
        const string AddPlan::toString() const 
        {
            if (getStatus() == ActionStatus::COMPLETED)
            {
                return "Plan " + settlementName+" " + selectionPolicy + " COMPLETED";
            }
            else
            {
                return "Plan " + settlementName+" " + selectionPolicy + " ERROR";
            }
        }
        
        AddPlan *AddPlan::clone() const 
        {
            return new AddPlan(*this);
        }

        AddPlan::AddPlan(const AddPlan& other)
        :BaseAction(),settlementName(other.settlementName),selectionPolicy(other.selectionPolicy),flag(other.flag){}

////////////////////////////AddSettlement////////////////////////////

        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType)
            :BaseAction(),settlementName(settlementName),settlementType(settlementType){}
        
        void AddSettlement::act(Simulation &simulation) 
        {
            if(simulation.isSettlementExists(settlementName))
            {
                this->error("Settlement already exists");
            }
            else
            {
            simulation.addSettlement(new Settlement(settlementName , settlementType));
            this->complete();
            }
        }
        
        AddSettlement *AddSettlement::clone() const {
            return new AddSettlement(*this);
        }
        
        const string AddSettlement::toString() const 
        {
            if (ActionStatus::COMPLETED == getStatus())
            {
                return "settlement " + settlementName + " " + std::to_string(int(settlementType)) + " COMPLETED";
            }
            else
            {
                return "settlement " + settlementName + " " + std::to_string(int(settlementType)) + " ERROR";
            }
        }

////////////////////////////AddFacility////////////////////////////

        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
        :BaseAction(),facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(economyScore),flag(true)
        {
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
                return "Facility " +facilityName+" " + to_string(int(facilityCategory))+" " + to_string(price)+" " + to_string(lifeQualityScore)+ " "+to_string(economyScore)+" " + to_string(economyScore) + " COMPLETED";
            }
            else
            {
                return "Facility " +facilityName+" " + to_string(int(facilityCategory))+" " + to_string(price)+" " + to_string(lifeQualityScore)+ " "+to_string(economyScore)+" " + to_string(economyScore) + " ERROR";

            }
        }
    
////////////////////////////PrintPlanStatus////////////////////////////


        PrintPlanStatus::PrintPlanStatus(int planId):BaseAction(),planId(planId){}
        
        void PrintPlanStatus::act(Simulation &simulation) 
        {
            if(simulation.getCounter() > planId)
            {
                std::cout << simulation.getPlan(planId).toString();
                complete();
            }else
            {
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
                return "planStatus "+to_string(planId)+" COMPLETED";
            }
            else
            {
                return "planStatus "+to_string(planId)+" ERROR";
            }
        }
        

////////////////////////////ChangePolicy////////////////////////////

        ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
        :BaseAction(),planId(planId),newPolicy(newPolicy),s()
        {  
        }
        
        void ChangePlanPolicy::act(Simulation &simulation) 
        {
            if(simulation.PlanExists(planId)){
                Plan & p = simulation.getPlan(planId);
                s = p.getPolicy()->toString();
                if(s == newPolicy){
                    error("Cannot change selection policy");
                }else
                {
                    SelectionPolicy *policy = SelectPolicy(newPolicy,p);
                    p.setSelectionPolicy(policy);
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
                return "planId "+to_string(planId) + "\npreviousPolicy: "+s+ "\nnewPolicy: "+ newPolicy + " COMPLETED"; 
            }
            else
            {
                return "planId "+to_string(planId) + "\npreviousPolicy: "+s+ "\nnewPolicy: "+ newPolicy + " ERROR"; 
            }
        }

        SelectionPolicy *ChangePlanPolicy::SelectPolicy(const string& policy, Plan& p )
        {
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


    PrintActionsLog::PrintActionsLog():BaseAction(){}
    void PrintActionsLog::act(Simulation &simulation) 
    {
        for(BaseAction *action: simulation.getLogActions())
        {
            cout<<action->toString()<<endl;
        }
        complete();
    }
    PrintActionsLog *PrintActionsLog::clone() const 
    {
        return new PrintActionsLog();
    }
    const string PrintActionsLog::toString() const 
    {
        return "Log COMMPLETED";
    }


////////////////////////////Close////////////////////////////

Close::Close():BaseAction(){}
        
        void Close::act(Simulation &simulation){
            for(Plan &p : simulation.getPlans()){
                cout<< p.toString() << endl;
            }
            simulation.close();
            complete();
        }
        
        Close *Close::clone() const {
            return new Close(*this);
        }
        
        const string Close::toString() const {
            return "Close COMPLETED";
        }


////////////////////////////BackupSimulation////////////////////////////

        BackupSimulation::BackupSimulation():BaseAction(){}
        void BackupSimulation::act(Simulation &simulation)
        {
            if (backup == nullptr)
            {
                backup = new Simulation(simulation); 
                complete();
            }
            else
            {
                backup = &simulation;
                complete();
            }
                
        }
        BackupSimulation *BackupSimulation::clone() const 
        {
           return new BackupSimulation(*this); 
        }
        const string BackupSimulation::toString() const
        {
            return "backup COMPLETED";
        }

////////////////////////////RestoreSimulation////////////////////////////

        RestoreSimulation::RestoreSimulation():BaseAction(){}

        void RestoreSimulation::act(Simulation &simulation)
        {
            if (backup != nullptr)
            {
                simulation = *backup;
                complete();
            }
            else
            {
                error("No backup available");
            }
            
        }
        
        RestoreSimulation *RestoreSimulation::clone() const
        {
            return new RestoreSimulation();
        }
        
        const string RestoreSimulation::toString() const
        {
            return "restore "+to_string(int(getStatus()));
        }
