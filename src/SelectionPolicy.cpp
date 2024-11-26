#include "../include/SelectionPolicy.h"
#include <iostream>
using namespace std;

class NaiveSelection: public SelectionPolicy {
    public:



        NaiveSelection():lastSelectedIndex(-1){}
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            lastSelectedIndex+=1;
            return facilitiesOptions.at(lastSelectedIndex+1);
        }
        const string toString() const override{
            return "Naive";
        }
        NaiveSelection *clone() const override{
            return new NaiveSelection(lastSelectedIndex);
        }
        ~NaiveSelection() override = default;

        //Rule Of 3:

        //copy constructor:
         NaiveSelection(int lastSelectedIndex):lastSelectedIndex(lastSelectedIndex){}

         //assignment operator:

         //destructor

    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};