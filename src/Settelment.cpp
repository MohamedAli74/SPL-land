#include "../include/Settlement.h"
#include <iostream>
using namespace std;

class Settlement {
    public:
        Settlement(const string &name, SettlementType type):name(name),type(type){}
        Settlement(const Settlement& other);
        const string &getName() const{
            return name;
        }
        SettlementType getType() const{
            return type;
        }
        const string toString() const{
            return "settlement name: " + name + ", setelment Type: " + to_string(int(getType()));
        }

        //we have to complete rule of 5!!

    private:
        const string name;
        SettlementType type;
};