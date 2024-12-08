#include "../include/SelectionPolicy.h"
#include <iostream>
#include <climits>
using namespace std;
using std::max;
using std::min;


////////////////////////////NaiveSelection////////////////////////////

        NaiveSelection::NaiveSelection():lastSelectedIndex(0){}
        
        const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
        {
            const FacilityType &temp = facilitiesOptions[lastSelectedIndex];
            lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
            return temp;
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
            int tmp;
            int diff = INT_MAX;
            int index = 1;
            for(int i=0 ; i<int(facilitiesOptions.size()) ;i++){
                tmp=calculateDiff(facilitiesOptions[i],this);
                if(tmp<diff){
                    index = i;
                    diff = tmp;
                }               
            }
            LifeQualityScore += facilitiesOptions[index].getLifeQualityScore();
            EconomyScore += facilitiesOptions[index].getEconomyScore();
            EnvironmentScore += facilitiesOptions[index].getEnvironmentScore(); 
            return facilitiesOptions[index];
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
        int BalancedSelection::calculateDiff(const FacilityType& toCalculate,const BalancedSelection* policy){

            int updatedLife = policy->getLife() + toCalculate.getLifeQualityScore();
            int updatedEconomy = policy->getEconomy() + toCalculate.getEconomyScore();
            int updatedEnvironment = policy->getEnviroment() + toCalculate.getEnvironmentScore();

            int output = max(updatedEconomy , max(updatedLife , updatedEnvironment))
             - min(updatedEconomy , min(updatedLife , updatedEnvironment));
            return output;
        }

////////////////////////////EconomySelection////////////////////////////

        EconomySelection::EconomySelection():lastSelectedIndex(0)
        {
        }
        const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
        {
            int answerIndex;
            bool found=false;
            for(int i = lastSelectedIndex; i<=int(facilitiesOptions.size()) && !found ; i=(i+1)%facilitiesOptions.size())
            {
                if(facilitiesOptions[i].getCategory() == FacilityCategory :: ECONOMY)
                {
                    lastSelectedIndex = (i+1)%facilitiesOptions.size();
                    answerIndex = i;
                    found = true;
                }

            }
            return facilitiesOptions[answerIndex];
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
        const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
            int answerIndex;
            bool found=false;
            for(int i = lastSelectedIndex; i <= int(facilitiesOptions.size()) && !found ; i=(i+1)%facilitiesOptions.size()){
                
                if(facilitiesOptions[i].getCategory() == FacilityCategory :: ENVIRONMENT)
                {
                    lastSelectedIndex = (i+1)%facilitiesOptions.size();
                    answerIndex =i;
                }
            }
            return facilitiesOptions[answerIndex];
        }
        
        const string SustainabilitySelection::toString() const
        {
            return "env";
        }
        SustainabilitySelection *SustainabilitySelection::clone() const
        {
            return new SustainabilitySelection(*this);
        }