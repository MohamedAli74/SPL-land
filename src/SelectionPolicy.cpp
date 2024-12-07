#include "../include/SelectionPolicy.h"
#include <iostream>
#include <climits>
using namespace std;
using std::max;
using std::min;


////////////////////////////NaiveSelection////////////////////////////

        NaiveSelection::NaiveSelection():lastSelectedIndex(-1){}
        
        const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
        {
            lastSelectedIndex= (lastSelectedIndex + 1) % facilitiesOptions.size() ;
            return facilitiesOptions[lastSelectedIndex];
        }
        
        const string NaiveSelection::toString() const{
            return "nve";
        }
        
        NaiveSelection *NaiveSelection::clone() const{
            return new NaiveSelection(*this);
        }

////////////////////////////BalancedSelection////////////////////////////


        BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
        :LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){}

        const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
            FacilityType *minDiff;
            int diff = INT_MAX;
            for(FacilityType f : facilitiesOptions){
                if (calculateDiff(f)<diff){
                    minDiff = &f;
                    diff = calculateDiff(f);
                }
                return *minDiff;
            }
        }

        const string BalancedSelection::toString() const{
            return "bal";
        }
        
        BalancedSelection *BalancedSelection::clone() const{
            return new BalancedSelection(*this);
        };
        
        
        int BalancedSelection::getLife() const{
            return LifeQualityScore;
        }

        int BalancedSelection::getEconomy() const{
            return EconomyScore;
        }

        int BalancedSelection::getEnviroment() const{
            return EnvironmentScore;
        }
        int BalancedSelection::calculateDiff(FacilityType& toCalculate){
            return max(max(toCalculate.getEconomyScore() + getEconomy() , toCalculate.getEnvironmentScore() + getEnviroment()) , toCalculate.getLifeQualityScore() + getLife())
                        -min(min(toCalculate.getEconomyScore() + getEconomy() , toCalculate.getEnvironmentScore() + getEnviroment() ), toCalculate.getLifeQualityScore() + getLife());
        }

////////////////////////////EconomySelection////////////////////////////

        EconomySelection::EconomySelection():lastSelectedIndex(0)
        {
        }
        const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
        {
            for(int i = lastSelectedIndex; i<=facilitiesOptions.size(); i=(i+1)%facilitiesOptions.size())
            {
                if(facilitiesOptions[i].getCategory() == FacilityCategory :: ECONOMY)
                {
                    lastSelectedIndex = (i+1)%facilitiesOptions.size();
                    return facilitiesOptions[i];
                }

            }
        }
        const string EconomySelection::toString() const
        {
            return "eco";
        }
        EconomySelection *EconomySelection::clone() const
        {
            return new EconomySelection(*this);
        }

////////////////////////////SustainabilitySelection////////////////////////////

        SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0)
        {
    
        }
        const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
        {
            for(int i = lastSelectedIndex; i <= facilitiesOptions.size(); i=(i+1)%facilitiesOptions.size())
        
            {
                if(facilitiesOptions[i].getCategory() == FacilityCategory :: ENVIRONMENT)
                {
                    lastSelectedIndex = (i+1)%facilitiesOptions.size();
                    return facilitiesOptions[i];
                }
            }
        }
        const string SustainabilitySelection::toString() const
        {
            return "env";
        }
        SustainabilitySelection *SustainabilitySelection::clone() const
        {
            return new SustainabilitySelection(*this);
        }