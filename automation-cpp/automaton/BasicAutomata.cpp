#include "BasicAutomata.h"
#include "Automaton.h"
#include "State.h"
#include "Transition.h"
#include "StatePair.h"
#include "StringUnionOperations.h"
#include "RegExp.h"
#include "Datatypes.h"

namespace dk {
    namespace brics {
        namespace automaton {

            BasicAutomata::BasicAutomata() {
            }

            Automaton * BasicAutomata::makeEmpty() {
                Automaton * a = new Automaton();
                a->initial = new State();
                a->deterministic = true;
                return a;
            }

            Automaton * BasicAutomata::makeEmptyString() {
                Automaton * a = new Automaton();
                a->singleton = L"";
                a->deterministic = true;
                return a;
            }

            Automaton * BasicAutomata::makeAnyString() {
                Automaton * a = new Automaton();
                State * s = new State();
                a->initial = s;
                s->accept = true;
                Transition tempVar(std::numeric_limits<wchar_t>::min(), std::numeric_limits<wchar_t>::max(), s);
                s->transitions->add(&tempVar);
                a->deterministic = true;
                return a;
            }

            Automaton * BasicAutomata::makeAnyChar() {
                return makeCharRange(std::numeric_limits<wchar_t>::min(), std::numeric_limits<wchar_t>::max());
            }

            Automaton * BasicAutomata::makeChar(wchar_t c) {
                Automaton * a = new Automaton();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                a->singleton = Character::toString(c);
                a->deterministic = true;
                return a;
            }

            Automaton * BasicAutomata::makeCharRange(wchar_t min, wchar_t max) {
                if (min == max) {
                    return makeChar(min);
                }
                Automaton * a = new Automaton();
                State * s1 = new State();
                State * s2 = new State();
                a->initial = s1;
                s2->accept = true;
                if (min <= max) {
                    Transition tempVar(min, max, s2);
                    s1->transitions->add(&tempVar);
                }
                a->deterministic = true;
                return a;
            }

            Automaton * BasicAutomata::makeCharSet(const std::wstring & set) {
                if (set.length() == 1) {
                    return makeChar(set[0]);
                }
                Automaton * a = new Automaton();
                State * s1 = new State();
                State * s2 = new State();
                a->initial = s1;
                s2->accept = true;
                for (int i = 0; i < set.length(); i++) {
                    Transition tempVar(set[i], s2);
                    s1->transitions->add(&tempVar);
                }
                a->deterministic = true;
                a->reduce();
                return a;
            }

            State * BasicAutomata::anyOfRightLength(const std::wstring & x, int n) {
                State * s = new State();
                if (x.length() == n) {
                    s->setAccept(true);
                } else {
                    Transition tempVar(L'0', L'9', anyOfRightLength(x, n + 1));
                    s->addTransition(&tempVar);
                }
                return s;
            }

            State * BasicAutomata::atLeast(const std::wstring & x, int n, Collection<State *> * initials, bool zeros) {
                State * s = new State();
                if (x.length() == n) {
                    s->setAccept(true);
                } else {
                    if (zeros) {
                        initials->add(s);
                    }
                    wchar_t c = x[n];
                    Transition tempVar(c, atLeast(x, n + 1, initials, zeros && c == L'0'));
                    s->addTransition(&tempVar);
                    if (c < L'9') {
                        Transition tempVar2(static_cast<wchar_t>(c + 1), L'9', anyOfRightLength(x, n + 1));
                        s->addTransition(&tempVar2);
                    }
                }
                return s;
            }

            State * BasicAutomata::atMost(const std::wstring & x, int n) {
                State * s = new State();
                if (x.length() == n) {
                    s->setAccept(true);
                } else {
                    wchar_t c = x[n];
                    Transition tempVar(c, atMost(x, static_cast<wchar_t>(n) + 1));
                    s->addTransition(&tempVar);
                    if (c > L'0') {
                        Transition tempVar2(L'0', static_cast<wchar_t>(c - 1), anyOfRightLength(x, n + 1));
                        s->addTransition(&tempVar2);
                    }
                }
                return s;
            }

            State * BasicAutomata::between(const std::wstring & x, const std::wstring & y, int n,
                                           Collection<State *> * initials, bool zeros) {
                State * s = new State();
                if (x.length() == n) {
                    s->setAccept(true);
                } else {
                    if (zeros) {
                        initials->add(s);
                    }
                    wchar_t cx = x[n];
                    wchar_t cy = y[n];
                    if (cx == cy) {
                        Transition tempVar(cx, between(x, y, n + 1, initials, zeros && cx == L'0'));
                        s->addTransition(&tempVar);
                    } else { // cx<cy
                        Transition tempVar2(cx, atLeast(x, n + 1, initials, zeros && cx == L'0'));
                        s->addTransition(&tempVar2);
                        Transition tempVar3(cy, atMost(y, n + 1));
                        s->addTransition(&tempVar3);
                        if (cx + 1 < cy) {
                            Transition tempVar4(static_cast<wchar_t>(cx + 1), static_cast<wchar_t>(cy - 1),
                                                anyOfRightLength(x, n + 1));
                            s->addTransition(&tempVar4);
                        }
                    }
                }
                return s;
            }

            Automaton * BasicAutomata::makeInterval(int min, int max, int digits) throw(std::invalid_argument) {
                Automaton * a = new Automaton();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                std::wstring x = Integer::toString(min);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                std::wstring y = Integer::toString(max);
                if (min > max || (digits > 0 && y.length() > digits)) {
//JAVA TO C++ CONVERTER TODO TASK: This exception's constructor requires an argument:
//ORIGINAL LINE: throw new IllegalArgumentException();
                    throw std::invalid_argument();
                }
                int d;
                if (digits > 0) {
                    d = digits;
                } else {
                    d = y.length();
                }
                StringBuilder * bx = new StringBuilder();
                for (int i = x.length(); i < d; i++) {
                    bx->append(L'0');
                }
                bx->append(x);
                x = bx->toString();
                StringBuilder * by = new StringBuilder();
                for (int i = y.length(); i < d; i++) {
                    by->append(L'0');
                }
                by->append(y);
                y = by->toString();
                Collection < State * > *initials = std::vector<State *>();
                a->initial = between(x, y, 0, initials, digits <= 0);
                if (digits <= 0) {
                    std::vector<StatePair *> pairs;
                    for (auto p : initials) {
                        if (a->initial != p) {
                            StatePair tempVar(a->initial, p);
                            pairs.push_back(&tempVar);
                        }
                    }
                    a->addEpsilons(pairs);
                    Transition tempVar2(L'0', a->initial);
                    a->initial->addTransition(&tempVar2);
                    a->deterministic = false;
                } else {
                    a->deterministic = true;
                }
                a->checkMinimizeAlways();
                return a;
            }

            Automaton * BasicAutomata::makeString(const std::wstring & s) {
                Automaton * a = new Automaton();
                a->singleton = s;
                a->deterministic = true;
                return a;
            }

            Automaton * BasicAutomata::makeStringUnion(std::vector<std::string> & strings) {
                if (strings->length == 0) {
                    return makeEmpty();
                }
                Arrays::sort(strings, StringUnionOperations::LEXICOGRAPHIC_ORDER);
                Automaton * a = new Automaton();
                a->setInitialState(StringUnionOperations::build(strings));
                a->setDeterministic(true);
                a->reduce();
                a->recomputeHashCode();
                return a;
            }

            Automaton * BasicAutomata::makeMaxInteger(const std::wstring & n) {
                int i = 0;
                while (i < n.length() && n[i] == L'0') {
                    i++;
                }
                StringBuilder * b = new StringBuilder();
                b->append(L"0*(0|");
                if (i < n.length()) {
                    b->append(L"[0-9]{1,")->append(n.length() - i - 1)->append(L"}|");
                }
                maxInteger(n.substr(i), 0, b);
                b->append(L")");
                RegExp tempVar(b->toString());
                return Automaton::minimize((&tempVar)->toAutomaton());
            }

            void BasicAutomata::maxInteger(const std::wstring & n, int i, StringBuilder * b) {
                b->append(L'(');
                if (i < n.length()) {
                    wchar_t c = n[i];
                    if (c != L'0') {
                        b->append(L"[0-")->append(static_cast<wchar_t>(c - 1))->append(L"][0-9]{")->append(
                                n.length() - i - 1)->append(L"}|");
                    }
                    b->append(c);
                    maxInteger(n, i + 1, b);
                }
                b->append(L')');
            }

            Automaton * BasicAutomata::makeMinInteger(const std::wstring & n) {
                int i = 0;
                while (i + 1 < n.length() && n[i] == L'0') {
                    i++;
                }
                StringBuilder * b = new StringBuilder();
                b->append(L"0*");
                minInteger(n.substr(i), 0, b);
                b->append(L"[0-9]*");
                RegExp tempVar(b->toString());
                return Automaton::minimize((&tempVar)->toAutomaton());
            }

            void BasicAutomata::minInteger(const std::wstring & n, int i, StringBuilder * b) {
                b->append(L'(');
                if (i < n.length()) {
                    wchar_t c = n[i];
                    if (c != L'9') {
                        b->append(L"[")->append(static_cast<wchar_t>(c + 1))->append(L"-9][0-9]{")->append(
                                n.length() - i - 1)->append(L"}|");
                    }
                    b->append(c);
                    minInteger(n, i + 1, b);
                }
                b->append(L')');
            }

            Automaton * BasicAutomata::makeTotalDigits(int i) {
                RegExp tempVar(
                        std::wstring(L"[ \t\n\r]*[-+]?0*([0-9]{0,") + i + std::wstring(L"}|((([0-9]\\.*){0,") + i +
                        std::wstring(L"})&@\\.@)0*)[ \t\n\r]*"));
                return Automaton::minimize((&tempVar)->toAutomaton());
            }

            Automaton * BasicAutomata::makeFractionDigits(int i) {
                RegExp tempVar(
                        std::wstring(L"[ \t\n\r]*[-+]?[0-9]+(\\.[0-9]{0,") + i + std::wstring(L"}0*)?[ \t\n\r]*"));
                return Automaton::minimize((&tempVar)->toAutomaton());
            }

            Automaton * BasicAutomata::makeIntegerValue(const std::wstring & value) {
                bool minus = false;
                int i = 0;
                while (i < value.length()) {
                    wchar_t c = value[i];
                    if (c == L'-') {
                        minus = true;
                    }
                    if (c >= L'1' && c <= L'9') {
                        break;
                    }
                    i++;
                }
                StringBuilder * b = new StringBuilder();
                b->append(value.substr(i));
                if (b->length() == 0) {
                    b->append(L"0");
                }
                Automaton * s;
                if (minus) {
                    s = Automaton::makeChar(L'-');
                } else {
                    s = Automaton::makeChar(L'+')->optional();
                }
                Automaton * ws = Datatypes::getWhitespaceAutomaton();
                return Automaton::minimize(ws->concatenate(
                        s->concatenate(Automaton::makeChar(L'0')->repeat())->concatenate(
                                Automaton::makeString(b->toString())))->concatenate(ws));
            }

            Automaton * BasicAutomata::makeDecimalValue(const std::wstring & value) {
                bool minus = false;
                int i = 0;
                while (i < value.length()) {
                    wchar_t c = value[i];
                    if (c == L'-') {
                        minus = true;
                    }
                    if ((c >= L'1' && c <= L'9') || c == L'.') {
                        break;
                    }
                    i++;
                }
                StringBuilder * b1 = new StringBuilder();
                StringBuilder * b2 = new StringBuilder();
                int p = (int) value.find(L'.', i);
                if (p == -1) {
                    b1->append(value.substr(i));
                } else {
                    b1->append(value.substr(i, p - i));
                    i = value.length() - 1;
                    while (i > p) {
                        wchar_t c = value[i];
                        if (c >= L'1' && c <= L'9') {
                            break;
                        }
                        i--;
                    }
                    b2->append(value.substr(p + 1, (i + 1) - (p + 1)));
                }
                if (b1->length() == 0) {
                    b1->append(L"0");
                }
                Automaton * s;
                if (minus) {
                    s = Automaton::makeChar(L'-');
                } else {
                    s = Automaton::makeChar(L'+')->optional();
                }
                Automaton * d;
                if (b2->length() == 0) {
                    d = Automaton::makeChar(L'.')->concatenate(Automaton::makeChar(L'0')->repeat(1))->optional();
                } else {
                    d = Automaton::makeChar(L'.')->concatenate(Automaton::makeString(b2->toString()))->concatenate(
                            Automaton::makeChar(L'0')->repeat());
                }
                Automaton * ws = Datatypes::getWhitespaceAutomaton();
                return Automaton::minimize(ws->concatenate(
                        s->concatenate(Automaton::makeChar(L'0')->repeat())->concatenate(
                                Automaton::makeString(b1->toString()))->concatenate(d))->concatenate(ws));
            }

            Automaton * BasicAutomata::makeStringMatcher(const std::wstring & s) {
                Automaton * a = new Automaton();
                std::vector<State *> states(s.length() + 1);
                states[0] = a->initial;
                for (int i = 0; i < s.length(); i++) {
                    states[i + 1] = new State();
                }
                State * f = states[s.length()];
                f->accept = true;
                Transition tempVar(std::numeric_limits<wchar_t>::min(), std::numeric_limits<wchar_t>::max(), f);
                f->transitions->add(&tempVar);
                for (int i = 0; i < s.length(); i++) {
                    Set<wchar_t> * done = std::unordered_set<wchar_t>();
                    wchar_t c = s[i];
                    Transition tempVar2(c, states[i + 1]);
                    states[i]->transitions->add(&tempVar2);
                    done->add(c);
                    for (int j = i; j >= 1; j--) {
                        wchar_t d = s[j - 1];
                        if (!done->contains(d) && s.substr(0, j - 1)->equals(s.substr(i - j + 1, i - (i - j + 1)))) {
                            Transition tempVar3(d, states[j]);
                            states[i]->transitions->add(&tempVar3);
                            done->add(d);
                        }
                    }
                    std::vector<wchar_t> da(done->size());
                    int h = 0;
                    for (auto w : done) {
                        da[h++] = w;
                    }
                    Arrays::sort(da);
                    int from = std::numeric_limits<wchar_t>::min();
                    int k = 0;
                    while (from <= std::numeric_limits<wchar_t>::max()) {
                        while (k < da.size() && da[k] == from) {
                            k++;
                            from++;
                        }
                        if (from <= std::numeric_limits<wchar_t>::max()) {
                            int to = std::numeric_limits<wchar_t>::max();
                            if (k < da.size()) {
                                to = da[k] - 1;
                                k++;
                            }
                            Transition tempVar4(static_cast<wchar_t>(from), static_cast<wchar_t>(to), states[0]);
                            states[i]->transitions->add(&tempVar4);
                            from = to + 2;
                        }
                    }
                }
                a->deterministic = true;
                return a;
            }
        }
    }
}
