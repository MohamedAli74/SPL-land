#include "../include/Plan.h"
#include <iostream>
#include <string>
using namespace std;

        Plan::Plan(const int planId, const Settlement *settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
        plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),underConstruction(),facilityOptions(facilityOptions),life_quality_score(0),economy_score(0),environment_score(0)
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

        void Plan::step()
        {
            if(status == PlanStatus::AVALIABLE)
            {
            while(int(underConstruction.size()) <= int(settlement->getType()))
            { 
                //for example for a village(0) the limit is 0 + 1, and the same for the city and metropolis.
            FacilityType newfacitype = selectionPolicy -> selectFacility(facilityOptions);
            Facility* toAdd = new Facility(newfacitype,settlement->getName());
            addFacility(toAdd);
            }
            }

            for(int i = underConstruction.size() - 1; i >= 0; i--)
            {
                underConstruction[i] -> step();    
                //check if the facility becomes operational
                if(underConstruction[i] -> getStatus() == FacilityStatus::OPERATIONAL)//47
                {
                    Facility* tmp = underConstruction[i];
                    economy_score += tmp->getEconomyScore();
                    life_quality_score += tmp -> getLifeQualityScore();
                    environment_score += tmp -> getEnvironmentScore();
                    underConstruction.erase(underConstruction.begin()+i);
                    facilities.push_back(tmp);
                    tmp = nullptr;
                }
            }

            if(int(underConstruction.size())==int(settlement->getType())+1)
            {
                status = PlanStatus::BUSY;
            }
            else
            {
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
        }
        const string Plan::toString() const
        {
            string s;
            if (status == PlanStatus::AVALIABLE )
            {
                s = "PlanID: " + to_string(int(plan_id)) + "\nSettlementName: " + settlement->getName() + "\nPlanStatus: " +"AVAILABE" +"\nSelectionPolicy: " + selectionPolicy->toString() + "\nLifeQualityScore : " + to_string(life_quality_score) + "\nEconomyScore : " + to_string(economy_score) + "\nEnvironmentScore : " + to_string(environment_score) + "\n" ;
            }
            else
            {
                s = "PlanID: " + to_string(int(plan_id)) + "\nSettlementName: " + settlement->getName() + "\nPlanStatus: " +"BUSY" +"\nSelectionPolicy: " + selectionPolicy->toString() + "\nLifeQualityScore : " + to_string(life_quality_score) + "\nEconomyScore : " + to_string(economy_score) + "\nEnvironmentScore : " + to_string(environment_score) + "\n" ;
            }
            
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

        const Settlement* Plan::getSettlement(){
            return settlement;
        }

        void Plan::setSettlement(Settlement *set){
            settlement = set;
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
        :plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(other.selectionPolicy),status(other.status),facilities(),underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score)
        {
            selectionPolicy=other.selectionPolicy->clone();
            for (Facility* facility : other.underConstruction)
            {
                underConstruction.push_back(new Facility(*facility));
            }

            for (Facility* facility : other.facilities)
            {
                facilities.push_back(new Facility(*facility));
            }
        }


        Plan::Plan(Plan&& other):
        plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(nullptr),status(other.status),facilities(),underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score)
        {
            selectionPolicy = other.selectionPolicy;
            other.selectionPolicy = nullptr;
            for (Facility* facility : other.underConstruction)
            {
                underConstruction.push_back(facility);
            }

            for (Facility* facility : other.facilities)
            {
                facilities.push_back(facility);
            }
            other.underConstruction.clear();
            other.facilities.clear();
        }

        SelectionPolicy* Plan::getPolicy() const{
            return selectionPolicy;
        }
