#include "../include/Settlement.h"
#include <iostream>
using namespace std;

        Settlement::Settlement(const string &name, SettlementType type):name(name),type(type){}
        Settlement::Settlement(const Settlement &other):name(other.name),type(other.type)
        {
        }


        
        const string &Settlement::getName() const{
            return name;
        }
        SettlementType Settlement::getType() const{
            return type;
        }
        const string Settlement::toString() const{
            return "settlement name: " + name + ", setelment Type: " + to_string(int(getType()));
        }



        
