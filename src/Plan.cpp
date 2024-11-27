#include "../include/Plan.h"

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
            for(int i=0;i<underConstruction.size();i++){
                underConstruction[i] -> step();
                
                //check if the facility becomes operational
                if(int i = underConstruction.size() - 1; i >= 0; i--){
                    Facility* tmp = underConstruction[i];
                    underConstruction.erase(underConstruction.begin()+i);
                    facilities.push_back(tmp);
                }
            }
        }
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

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