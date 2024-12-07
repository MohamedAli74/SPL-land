#include "../include/Plan.h"
#include <iostream>
#include <string>
using namespace std;

        Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
        plan_id(planId),settlement(settlement),facilityOptions(facilityOptions),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),life_quality_score(0),economy_score(0),environment_score(0)
        {
        }

        //////////////////////////////////methods//////////////////////////////////
        int Plan::getPlanId()
        {
            return plan_id;
        }
        const int Plan::getlifeQualityScore() const{
            return life_quality_score;
        }
        const int Plan::getEconomyScore() const{
            return economy_score;
        }
        const int Plan::getEnvironmentScore() const{
            return environment_score;
        }
        void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
            delete Plan::selectionPolicy;
            Plan::selectionPolicy = selectionPolicy;
        }

        void Plan::step(){
            if(status == PlanStatus::AVALIABLE){
                while(underConstruction.size()<=int(settlement.getType())+1)
                { //for example for a village(0) the limit is 0 + 1, and the same for the city and metropolis. 
                    FacilityType newfacilitytype = selectionPolicy -> selectFacility(facilityOptions);
                    Facility* toAdd = new Facility(newfacilitytype,settlement.getName());//Kareem: why this reqiered
                    addFacility(toAdd);
            }
        }

            for(int i = underConstruction.size() - 1; i >= 0; i--){
                underConstruction[i] -> step();    
                //check if the facility becomes operational
                if(underConstruction[i] -> getStatus() == FacilityStatus::OPERATIONAL)
                {
                    Facility* tmp = underConstruction[i];
                    underConstruction.erase(underConstruction.begin()+i);
                    facilities.push_back(tmp);
                    tmp = nullptr;
                }
            }

            if(underConstruction.size()==int(settlement.getType())){
                status = PlanStatus::BUSY;
            }else{
                if(status == PlanStatus::BUSY)
                    status = PlanStatus::AVALIABLE;
            }
        }
        void Plan::printStatus(){
            cout<< to_string(int(status));
        }
        const vector<Facility*> &Plan::getFacilities() const{
            return facilities;
        }
        void Plan::addFacility(Facility* facility)
        {
            underConstruction.push_back(facility); //do we have to add something here?
            economy_score += facility->getEconomyScore();
            life_quality_score += facility -> getLifeQualityScore();
            environment_score += facility -> getEnvironmentScore();
        }
        const string Plan::toString() const{
            string s = "PlanID: " + to_string(int(plan_id)) + "\nSettlementName: " + settlement.getName() + "\nPlanStatus: " +to_string(int(status)) +"\nSelectionPolicy: " + selectionPolicy->toString() + "\nLifeQualityScore : " + to_string(life_quality_score) + "\nEconomyScore : " + to_string(economy_score) + "\nEnvironmentScore : " + to_string(environment_score) + "\n" ;
            for(Facility* f : facilities){
                s=s+f->toString();
            } 
            for(Facility* f : underConstruction){
                s=s+f->toString();
            } 
            return s;
        }

        PlanStatus Plan::getStatus() const
        {
            return status;
        }

        //////////////////////////////////rule of 5//////////////////////////////////

        Plan::~Plan()
        {
            delete selectionPolicy;
            for (Facility* facility : facilities)
            {
                delete facility;
            }
            for (Facility* facility : underConstruction)
            {
                delete facility;
            }
        }

        Plan::Plan(const Plan& other)
        :plan_id(other.plan_id),settlement(other.settlement),status(other.status),facilities(other.facilities),underConstruction(other.underConstruction),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score)
        {
            delete selectionPolicy;
            selectionPolicy=other.selectionPolicy->clone();
        }

        Plan::Plan(Plan&& other):plan_id(other.plan_id),settlement(other.settlement),status(other.status),facilities(other.facilities),underConstruction(other.underConstruction),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score),selectionPolicy(other.selectionPolicy)
        {
            other.selectionPolicy = nullptr;
        }

        SelectionPolicy* Plan::getPolicy() const{
            return selectionPolicy;
        }