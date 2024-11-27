#include "Facility.h"
#include <iostream>
class FacilityType {
    public:
        FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):name(name),category(category),price(price),lifeQuality_score(lifeQuality_score),economy_score(economy_score),environment_score(environment_score)
        {
        }
        FacilityType(FacilityType &other):name(other.name),category(other.category),price(other.price),lifeQuality_score(other.lifeQuality_score),economy_score(other.economy_score),environment_score(other.environment_score)
        {
        }

        const string &getName() const
        {
            return name;
        }
        int getCost() const
        {
            return price;
        }
        int getLifeQualityScore() const
        {
            return lifeQuality_score;
        }
        int getEnvironmentScore() const
        {
            return environment_score;
        }
        int getEconomyScore() const
        {
            return economy_score;
        }
        FacilityCategory getCategory() const
        {
            return category;
        }

    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
};



class Facility: public FacilityType {

    public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score),settlementName(settlementName)
        {  
        }
        
        Facility(FacilityType &type, const string &settlementName):FacilityType(type),settlementName(settlementName)
        {
        }
        Facility(Facility &other):FacilityType(other.name,other.category,other.price,other.lifeQuality_score,other.economy_score,other.environment_score),settlementName(other.settlementName)
        {

        }
        
        const string &getSettlementName() const
        {
            return settlementName;
        }
        
        const int getTimeLeft() const
        {
            return timeLeft;
        }
        FacilityStatus step()
        {
            timeLeft = timeLeft - 1; 
            if(timeLeft == 0)
            {
                status = FacilityStatus::OPERATIONAL;
            }
            return status;
        }
        
        void setStatus(FacilityStatus status)
        {
            this-> status = status;
        }
        const FacilityStatus& getStatus() const
        {
            return status;
        }
        const string toString() const
        {
            return "name = "+name +" category = "+ std::to_string(int(category))+" price = "+ std::to_string(int(price)) +" lifeQuality_score = "+ std::to_string(int(lifeQuality_score)) +" economy_score = "+ std::to_string(int(economy_score))+" environment_score = "+ std::to_string(int(environment_score)) + " settlementName = "+ settlementName + " status = "+ std::to_string(int(status)) + " timeLeft = "+ std::to_string(timeLeft);
        };
        //rule of 5 needed

    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};