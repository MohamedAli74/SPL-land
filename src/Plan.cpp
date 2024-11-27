#include "../include/Plan.h"
#include <iostream>
#include <string>
using namespace std;

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
        plan_id(planId),settlement(settlement),facilityOptions(facilityOptions),selectionPolicy(selectionPolicy)
        {}

        //////////////////////////////////methods//////////////////////////////////
        const int getlifeQualityScore() const{
            return life_quality_score;
        }
        const int getEconomyScore() const{
            return economy_score;
        }
        const int getEnvironmentScore() const{
            return environment_score;
        }
        void setSelectionPolicy(SelectionPolicy *selectionPolicy){
            delete Plan::selectionPolicy;
            Plan::selectionPolicy = selectionPolicy;
        }

        void step(){

            while(underConstruction.size()<=int(settlement.getType())){
                Facility* toAdd = new Facility(selectionPolicy -> selectFacility(facilityOptions),settlement.getName());
                addFacility(toAdd);
            }

            for(int i = underConstruction.size() - 1; i >= 0; i--){
                underConstruction[i] -> step();    
                //check if the facility becomes operational
                if(underConstruction[i] -> getStatus() == FacilityStatus::OPERATIONAL){
                    Facility* tmp = underConstruction[i];
                    underConstruction.erase(underConstruction.begin()+i);
                    facilities.push_back(tmp);
                }
            }

            if(underConstruction.size()==int(settlement.getType())){
                status = PlanStatus::BUSY;
            }else{
                if(status == PlanStatus::BUSY)
                    status = PlanStatus::AVALIABLE;
            }
        }
        void printStatus(){
            cout<< to_string(int(status));
        }
        const vector<Facility*> &getFacilities() const{
            return facilities;
        }
        void addFacility(Facility* facility){
            underConstruction.push_back(facility);
        }
        const string toString() const{
            string s = "PlanID: " + to_string(int(plan_id)) + "\n, SettlementName: " + settlement.getName() + "\n PlanStatus: " +to_string(int(status)) +"/n SelectionPolicy:" + to_string(int(selectionPolicy)) + "\n LifeQualityScore : " + to_string(life_quality_score) + "\n EconomyScore : " + to_string(economy_score) + "\n EnvironmentScore : " + to_string(environment_score) + "\n" ;
            for(Facility* f : facilities){
                s=s+f->toString();
            } 
            for(Facility* f : underConstruction){
                s=s+f->toString();
            } 

            return s;
        }

        //////////////////////////////////rule of 5//////////////////////////////////

        ~Plan(){
            delete selectionPolicy;
        }

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};