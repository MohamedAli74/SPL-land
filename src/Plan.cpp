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
            for(int i = underConstruction.size() - 1; i >= 0; i--){
                underConstruction[i] -> step();    
                //check if the facility becomes operational
                if(underConstruction[i] -> getStatus() == FacilityStatus::OPERATIONAL){
                    Facility* tmp = underConstruction[i];
                    underConstruction.erase(underConstruction.begin()+i);
                    facilities.push_back(tmp);
                }
            }

            selectionPolicy -> selectFacility(facilityOptions);
        }
        void printStatus();
        const vector<Facility*> &getFacilities() const{
            return facilities;
        }
        void addFacility(Facility* facility){
            underConstruction.push_back(facility);
        }
        const string toString() const{
            String s = "PlanID: " + to_string(int(plan_id)) + "\n, SettlementName: " + settlement.getName() + "\n PlanStatus: " +to_string(int(status)) +"/n SelectionPolicy:" + to_string(int(selectionPolicy)) "\n : " + ;
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