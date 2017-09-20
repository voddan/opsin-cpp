#include "StatePair.h"
#include "State.h"

namespace dk {
    namespace brics {
        namespace automaton {

            StatePair::StatePair(State * s, State * s1, State * s2) {
                this->s = s;
                this->s1 = s1;
                this->s2 = s2;
            }

            StatePair::StatePair(State * s1, State * s2) {
                this->s1 = s1;
                this->s2 = s2;
            }

            State * StatePair::getFirstState() {
                return s1;
            }

            State * StatePair::getSecondState() {
                return s2;
            }

            bool StatePair::equals(void * obj) {
                if (dynamic_cast<StatePair *>(obj) != nullptr) {
                    StatePair * p = static_cast<StatePair *>(obj);
                    return p->s1 == s1 && p->s2 == s2;
                } else {
                    return false;
                }
            }

            int StatePair::hashCode() {
                return s1->hashCode() + s2->hashCode();
            }
        }
    }
}
