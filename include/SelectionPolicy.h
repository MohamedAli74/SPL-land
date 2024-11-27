#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection *clone() const override;
        ~NaiveSelection() override = default;
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
            return "Economy";
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
            return "Sustainability";
        }
        SustainabilitySelection *clone() const override
        {
            return new SustainabilitySelection(*this);
        }
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};