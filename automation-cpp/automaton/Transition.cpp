#include "Transition.h"
#include "State.h"

namespace dk {
    namespace brics {
        namespace automaton {

            Transition::Transition(wchar_t c, State * to) {
                min = max = c;
                this->to = to;
            }

            Transition::Transition(wchar_t min, wchar_t max, State * to) {
                if (max < min) {
                    wchar_t t = max;
                    max = min;
                    min = t;
                }
                this->min = min;
                this->max = max;
                this->to = to;
            }

            wchar_t Transition::getMin() {
                return min;
            }

            wchar_t Transition::getMax() {
                return max;
            }

            State * Transition::getDest() {
                return to;
            }

            bool Transition::equals(void * obj) {
                if (dynamic_cast<Transition *>(obj) != nullptr) {
                    Transition * t = static_cast<Transition *>(obj);
                    return t->min == min && t->max == max && t->to == to;
                } else {
                    return false;
                }
            }

            int Transition::hashCode() {
                return min * 2 + max * 3;
            }

            Transition * Transition::clone() {
                try {
                    return static_cast<Transition *>(__super::clone());
                }
                catch (const CloneNotSupportedException & e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
                    throw std::exception();
                }
            }

            void Transition::appendCharString(wchar_t c, StringBuilder * b) {
                if (c >= 0x21 && c <= 0x7e && c != L'\\' && c != L'"') {
                    b->append(c);
                } else {
                    b->append(L"\\u");
                    std::wstring s = Integer::toHexString(c);
                    if (c < 0x10) {
                        b->append(L"000")->append(s);
                    } else if (c < 0x100) {
                        b->append(L"00")->append(s);
                    } else if (c < 0x1000) {
                        b->append(L"0")->append(s);
                    } else {
                        b->append(s);
                    }
                }
            }

            std::wstring Transition::toString() {
                StringBuilder * b = new StringBuilder();
                appendCharString(min, b);
                if (min != max) {
                    b->append(L"-");
                    appendCharString(max, b);
                }
                b->append(L" -> ")->append(to->number);
                return b->toString();
            }

            void Transition::appendDot(StringBuilder * b) {
                b->append(L" -> ")->append(to->number)->append(L" [label=\"");
                appendCharString(min, b);
                if (min != max) {
                    b->append(L"-");
                    appendCharString(max, b);
                }
                b->append(L"\"]\n");
            }
        }
    }
}
