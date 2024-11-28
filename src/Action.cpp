#include "../include/Action.h"
#include <iostream>
using namespace std;

class BaseAction{
    public:
        BaseAction()
        {
        }
        ActionStatus getStatus() const
        {
            return status;
        }
        virtual void act(Simulation& simulation)=0;
        virtual const string toString() const=0;
        virtual BaseAction* clone() const = 0;
        virtual ~BaseAction() = default;

    protected:
        void complete()
        {
            this-> status = ActionStatus :: COMPLETED;
        }
        void error(string errorMsg)
        {
            this -> status = ActionStatus :: ERROR;
            std::cout << "Error: " << errorMsg;
        }
        const string &getErrorMsg() const
        {
            return errorMsg;
        }

    private:
        string errorMsg;
        ActionStatus status;
};


class SimulateStep : public BaseAction {

    public:
        SimulateStep(const int numOfSteps):numOfSteps(numOfSteps)
        {}
        void act(Simulation &simulation) override{
            for(Plan &p :simulation.getPlans()){
                p.step();
            }
        }
        const string toString() const override{
            return "step " + numOfSteps;
        }
        SimulateStep *clone() const override{
            return new SimulateStep(*this);
        }
    private:
        const int numOfSteps;
};

class AddPlan : public BaseAction 
{
    public:
        AddPlan(const string &settlementName, const string &selectionPolicy)
        :settlementName(settlementName),selectionPolicy(selectionPolicy){
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
        void act(Simulation &simulation) override
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
        const string toString() const override
        {
            std::string s = "plan" + settlementName + selectionPolicy;
        }
        AddPlan *clone() const override
        {
            new AddPlan(*this);
        }

    private:
        const string settlementName;
        const string selectionPolicy;
        SelectionPolicy* policy;
        bool flag = true;
};



class AddSettlement : public BaseAction {
    public:
        AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){

        }
        void act(Simulation &simulation) override{
            if(simulation.isSettlementExists(settlementName)){
                this->error("Settlement already exists");
            }else{
            simulation.addSettlement(Settlement(settlementName , settlementType));
            this->complete();
            }
        }
        AddSettlement *clone() const override{
            return new AddSettlement(*this);
        }
        const string toString() const override{
            return "settlement " + settlementName + " " + std::to_string(int(settlementType));
        }
    private:
        const string settlementName;
        const SettlementType settlementType;
};



class AddFacility : public BaseAction {
    public:
        AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
        :facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
        void act(Simulation &simulation) override{
            vector<FacilityType>& facilityList=simulation.getOptions();
            for(FacilityType& t : facilityList){
                if(t.getName() == facilityName){
                    error("Facility already exists");
                }
            }
            if((this->getStatus() != ActionStatus::ERROR)){
                facilityList.push_back(FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore));
                complete();
            }
        }
        AddFacility *clone() const override{
            return new AddFacility(*this);
        }
        const string toString() const override{
            return "facility " + facilityName + " " + to_string(int(facilityCategory)) +" " + to_string(price) +" " + to_string(lifeQualityScore) +" " + to_string(economyScore) +" " + to_string(environmentScore);
        }
    private:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;

};

class PrintPlanStatus: public BaseAction {
    public:
        PrintPlanStatus(int planId):planId(planId){}
        void act(Simulation &simulation) override{
            if(simulation.PlanExists(planId)){
                std::cout << simulation.getPlan(planId).toString() ;
                complete();
            }else{
                error("Plan doesn’t exist");
            }
        }
        PrintPlanStatus *clone() const override{
            return new PrintPlanStatus(*this);
        }
        const string toString() const override{
            return "PlanStatus " + to_string(planId);
        }
    private:
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
    public:
        ChangePlanPolicy(const int planId, const string &newPolicy)
        :planId(planId),newPolicy(newPolicy){}

        void act(Simulation &simulation) override{
            if(simulation.PlanExists(planId)){
                Plan & p = simulation.getPlan(planId);
                if(p.getPolicy()->toString() == newPolicy){
                    error("Cannot change selection policy");
                }else{
                    p.setSelectionPolicy(choosePolicy(newPolicy,p));
                    complete();
                }
            }else{
                error("Cannot change selection policy");
            }
        }

        ChangePlanPolicy *clone() const override{
            return new ChangePlanPolicy(*this);
        }

        const string toString() const override{
            return "changePolicy " + to_string(planId) + newPolicy ; 
        }
    private:
        const int planId;
        const string newPolicy;

        SelectionPolicy* choosePolicy(const string &policyName,Plan& plan){
            if(policyName == "nve"){
                return new NaiveSelection();
            }else if(policyName == "bal"){
                    return new BalancedSelection(plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());
            }else if(policyName == "eco"){
                return new EconomySelection();
            }else if(policyName == "env"){
                return new SustainabilitySelection();
            }
        }

};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(Simulation &simulation) override;
        PrintActionsLog *clone() const override;
        const string toString() const override;
    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(Simulation &simulation) override;
        Close *clone() const override;
        const string toString() const override;
    private:
};

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation();
        void act(Simulation &simulation) override;
        BackupSimulation *clone() const override;
        const string toString() const override;
    private:
};


class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation();
        void act(Simulation &simulation) override;
        RestoreSimulation *clone() const override;
        const string toString() const override;
    private:
};