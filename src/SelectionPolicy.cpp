#include "../include/SelectionPolicy.h"
#include <iostream>
#include <climits>
using namespace std;
using std::max;
using std::min;


class NaiveSelection: public SelectionPolicy {
    public:

        NaiveSelection():lastSelectedIndex(-1){}
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            lastSelectedIndex= (lastSelectedIndex + 1) % facilitiesOptions.size() ;
            return facilitiesOptions[lastSelectedIndex];
        }
        const string toString() const override{
            return "nve";
        }
        NaiveSelection *clone() const override{
            return new NaiveSelection(*this);
        }
        ~NaiveSelection() override = default;

        //Rule Of 3: deafult

    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
        :LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){}

        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override{
            FacilityType *minDiff;
            int diff = INT_MAX;
            for(FacilityType f : facilitiesOptions){
                if (calculateDiff(f)<=diff){
                    minDiff = &f;
                    diff = calculateDiff(f);
                }
                return *minDiff;
            }
        }

        const string toString() const override{
            return "bal";
        }
        
        BalancedSelection *clone() const override{
            return new BalancedSelection(*this);
        };
        
        ~BalancedSelection() override = default;
        
        int getLife(){
            return LifeQualityScore;
        }

        int getEconomy(){
            return EconomyScore;
        }

        int getEnviroment(){
            return EnvironmentScore;
        }

    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
    
        int calculateDiff(FacilityType& toCalculate){
            return max(toCalculate.getEconomyScore() + getEconomy() , toCalculate.getEnvironmentScore() + getEnviroment() , toCalculate.getLifeQualityScore() + getLife())
                        -min(toCalculate.getEconomyScore() + getEconomy() , toCalculate.getEnvironmentScore() + getEnviroment() , toCalculate.getLifeQualityScore() + getLife());
        }
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection():lastSelectedIndex(0)
        {
        }
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            for(int i = lastSelectedIndex; i<=facilitiesOptions.size(); i=(i+1)%facilitiesOptions.size())
            {
                if(facilitiesOptions[i].getCategory() == FacilityCategory :: ECONOMY)
                {
                    lastSelectedIndex = i;
                    return facilitiesOptions[i];
                }

            }
        }
        const string toString() const override
        {
            return "eco";
        }
        EconomySelection *clone() const override
        {
            return new EconomySelection(*this);
        }
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy 
{
    public:
        SustainabilitySelection():lastSelectedIndex(0)
        {
    
        }
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            for(int i = lastSelectedIndex; i <= facilitiesOptions.size(); i=(i+1)%facilitiesOptions.size())
        
            {
                if(facilitiesOptions[i].getCategory() == FacilityCategory :: ENVIRONMENT)
                {
                    lastSelectedIndex = i;
                    return facilitiesOptions[i];
                }

            }
        
        }
        const string toString() const override
        {
            return "env";
        }
        SustainabilitySelection *clone() const override
        {
            return new SustainabilitySelection(*this);
        }
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};