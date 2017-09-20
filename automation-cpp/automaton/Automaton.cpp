#include "Automaton.h"
#include "State.h"
#include "Transition.h"
#include "BasicAutomata.h"
#include "BasicOperations.h"
#include "StatePair.h"
#include "MinimizationOperations.h"
#include "SpecialOperations.h"
#include "ShuffleOperations.h"

namespace dk {
    namespace brics {
        namespace automaton {

            int Automaton::minimization = MINIMIZE_HOPCROFT;
            bool Automaton::minimize_always = false;
            bool Automaton::allow_mutation = false;
            boost::optional<bool> Automaton::is_debug = nullptr;

            Automaton::Automaton() {
                initial = new State();
                deterministic = true;
                singleton = L"";
            }

            bool Automaton::isDebug() {
                if (!is_debug) {
                    is_debug = System::getProperty(L"dk.brics.automaton.debug") != nullptr;
                }
                return is_debug;
            }

            void Automaton::setMinimization(int algorithm) {
                minimization = algorithm;
            }

            void Automaton::setMinimizeAlways(bool flag) {
                minimize_always = flag;
            }

            bool Automaton::setAllowMutate(bool flag) {
                bool b = allow_mutation;
                allow_mutation = flag;
                return b;
            }

            bool Automaton::getAllowMutate() {
                return allow_mutation;
            }

            void Automaton::checkMinimizeAlways() {
                if (minimize_always) {
                    minimize();
                }
            }

            bool Automaton::isSingleton() {
                return singleton != L"";
            }

            std::wstring Automaton::getSingleton() {
                return singleton;
            }

            void Automaton::setInitialState(State * s) {
                initial = s;
                singleton = L"";
            }

            State * Automaton::getInitialState() {
                expandSingleton();
                return initial;
            }

            bool Automaton::isDeterministic() {
                return deterministic;
            }

            void Automaton::setDeterministic(bool deterministic) {
                this->deterministic = deterministic;
            }

            void Automaton::setInfo(void * info) {
                this->info = info;
            }

            void * Automaton::getInfo() {
                return info;
            }

            std::set<State *> * Automaton::getStates() {
                expandSingleton();
                std::set<State *> * visited;
                if (isDebug()) {
                    visited = new LinkedHashSet<State *>();
                } else {
                    visited = std::unordered_set<State *>();
                }
                std::list<State *> worklist;
                worklist.push_back(initial);
                visited->add(initial);
                while (worklist.size() > 0) {
                    State * s = worklist.pop_front();
                    Collection < Transition * > *tr;
                    if (isDebug()) {
                        tr = s->getSortedTransitions(false);
                    } else {
                        tr = s->transitions;
                    }
                    for (auto t : tr) {
                        if (!visited->contains(t->to)) {
                            visited->add(t->to);
                            worklist.push_back(t->to);
                        }
                    }
                }
                return visited;
            }

            std::set<State *> * Automaton::getAcceptStates() {
                expandSingleton();
                std::unordered_set<State *> accepts;
                std::unordered_set<State *> visited;
                std::list<State *> worklist;
                worklist.push_back(initial);
                visited.insert(initial);
                while (worklist.size() > 0) {
                    State * s = worklist.pop_front();
                    if (s->accept) {
                        accepts.insert(s);
                    }
                    for (auto t : s->transitions) {
                        if (!std::find(visited.begin(), visited.end(), t->to) != visited.end()) {
                            visited.insert(t->to);
                            worklist.push_back(t->to);
                        }
                    }
                }
                return accepts;
            }

            void Automaton::setStateNumbers(std::set<State *> * states) {
                if (states->size() == std::numeric_limits<int>::max()) {
                    throw std::invalid_argument("number of states exceeded Integer.MAX_VALUE");
                }
                int number = 0;
                for (auto s : states) {
                    s->number = number++;
                }
            }

            void Automaton::totalize() {
                State * s = new State();
                Transition tempVar(std::numeric_limits<wchar_t>::min(), std::numeric_limits<wchar_t>::max(), s);
                s->transitions->add(&tempVar);
                for (auto p : getStates()) {
                    int maxi = std::numeric_limits<wchar_t>::min();
                    for (auto t : p->getSortedTransitions(false)) {
                        if (t->min > maxi) {
                            Transition tempVar2(static_cast<wchar_t>(maxi), static_cast<wchar_t>(t->min - 1), s);
                            p->transitions->add(&tempVar2);
                        }
                        if (t->max + 1 > maxi) {
                            maxi = t->max + 1;
                        }
                    }
                    if (maxi <= std::numeric_limits<wchar_t>::max()) {
                        Transition tempVar3(static_cast<wchar_t>(maxi), std::numeric_limits<wchar_t>::max(), s);
                        p->transitions->add(&tempVar3);
                    }
                }
            }

            void Automaton::restoreInvariant() {
                removeDeadTransitions();
            }

            void Automaton::reduce() {
                if (isSingleton()) {
                    return;
                }
                std::set<State *> * states = getStates();
                setStateNumbers(states);
                for (auto s : states) {
                    std::vector<Transition *> st = s->getSortedTransitions(true);
                    s->resetTransitions();
                    State * p = nullptr;
                    int min = -1, max = -1;
                    for (auto t : st) {
                        if (p == t->to) {
                            if (t->min <= max + 1) {
                                if (t->max > max) {
                                    max = t->max;
                                }
                            } else {
                                if (p != nullptr) {
                                    Transition tempVar(static_cast<wchar_t>(min), static_cast<wchar_t>(max), p);
                                    s->transitions->add(&tempVar);
                                }
                                min = t->min;
                                max = t->max;
                            }
                        } else {
                            if (p != nullptr) {
                                Transition tempVar2(static_cast<wchar_t>(min), static_cast<wchar_t>(max), p);
                                s->transitions->add(&tempVar2);
                            }
                            p = t->to;
                            min = t->min;
                            max = t->max;
                        }
                    }
                    if (p != nullptr) {
                        Transition tempVar3(static_cast<wchar_t>(min), static_cast<wchar_t>(max), p);
                        s->transitions->add(&tempVar3);
                    }
                }
                clearHashCode();
            }

            std::vector<wchar_t> Automaton::getStartPoints() {
                std::set<wchar_t> * pointset = std::unordered_set<wchar_t>();
                pointset->add(std::numeric_limits<wchar_t>::min());
                for (auto s : getStates()) {
                    for (auto t : s->transitions) {
                        pointset->add(t->min);
                        if (t->max < std::numeric_limits<wchar_t>::max()) {
                            pointset->add(static_cast<wchar_t>(t->max + 1));
                        }
                    }
                }
                std::vector<wchar_t> points(pointset->size());
                int n = 0;
                for (* : :optional < wchar_t > m : pointset)
                {
                    points[n++] = m;
                }
                Arrays::sort(points);
                return points;
            }

            std::set<State *> * Automaton::getLiveStates() {
                expandSingleton();
                return getLiveStates(getStates());
            }

            std::set<State *> * Automaton::getLiveStates(std::set<State *> * states) {
                std::unordered_map<State *, std::set<State *> *> map;
                for (auto s : states) {
                    map[s] = std::unordered_set<State *>();
                }
                for (auto s : states) {
                    for (auto t : s->transitions) {
                        map[t->to]->add(s);
                    }
                }
                std::set<State *> * live = std::unordered_set<State *>(getAcceptStates());
                std::list<State *> worklist(live);
                while (worklist.size() > 0) {
                    State * s = worklist.pop_front();
                    for (auto p : map[s]) {
                        if (!live->contains(p)) {
                            live->add(p);
                            worklist.push_back(p);
                        }
                    }
                }
                return live;
            }

            void Automaton::removeDeadTransitions() {
                clearHashCode();
                if (isSingleton()) {
                    return;
                }
                std::set<State *> * states = getStates();
                std::set<State *> * live = getLiveStates(states);
                for (auto s : states) {
                    std::set<Transition *> * st = s->transitions;
                    s->resetTransitions();
                    for (auto t : st) {
                        if (live->contains(t->to)) {
                            s->transitions->add(t);
                        }
                    }
                }
                reduce();
            }

            std::vector<std::vector<Transition *>> Automaton::getSortedTransitions(std::set<State *> * states) {
                setStateNumbers(states);
                std::vector<std::vector<Transition *>> transitions(states->size());
                for (auto s : states) {
                    transitions[s->number] = s->getSortedTransitionArray(false);
                }
                return transitions;
            }

            void Automaton::expandSingleton() {
                if (isSingleton()) {
                    State * p = new State();
                    initial = p;
                    for (int i = 0; i < singleton.length(); i++) {
                        State * q = new State();
                        q->number = i;
                        Transition tempVar(singleton[i], q);
                        p->transitions->add(&tempVar);
                        p = q;
                    }
                    p->accept = true;
                    deterministic = true;
                    singleton = L"";
                }
            }

            int Automaton::getNumberOfStates() {
                if (isSingleton()) {
                    return singleton.length() + 1;
                }
                return getStates()->size();
            }

            int Automaton::getNumberOfTransitions() {
                if (isSingleton()) {
                    return singleton.length();
                }
                int c = 0;
                for (auto s : getStates()) {
                    c += s->transitions->size();
                }
                return c;
            }

            bool Automaton::equals(void * obj) {
                if (obj == this) {
                    return true;
                }
                if (!(dynamic_cast<Automaton *>(obj) != nullptr)) {
                    return false;
                }
                Automaton * a = static_cast<Automaton *>(obj);
                if (isSingleton() && a->isSingleton()) {
                    return singleton == a->singleton;
                }
                return hashCode() == a->hashCode() && subsetOf(a) && a->subsetOf(this);
            }

            int Automaton::hashCode() {
                if (hash_code == 0) {
                    minimize();
                }
                return hash_code;
            }

            void Automaton::recomputeHashCode() {
                hash_code = getNumberOfStates() * 3 + getNumberOfTransitions() * 2;
                if (hash_code == 0) {
                    hash_code = 1;
                }
            }

            void Automaton::clearHashCode() {
                hash_code = 0;
            }

            std::wstring Automaton::toString() {
                StringBuilder * b = new StringBuilder();
                if (isSingleton()) {
                    b->append(L"singleton: ");
                    for (auto c : singleton.toCharArray()) {
                        Transition::appendCharString(c, b);
                    }
                    b->append(L"\n");
                } else {
                    std::set<State *> * states = getStates();
                    setStateNumbers(states);
                    b->append(L"initial state: ")->append(initial->number)->append(L"\n");
                    for (auto s : states) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        b->append(s->toString());
                    }
                }
                return b->toString();
            }

            std::wstring Automaton::toDot() {
                StringBuilder * b = new StringBuilder(L"digraph Automaton {\n");
                b->append(L"  rankdir = LR;\n");
                std::set<State *> * states = getStates();
                setStateNumbers(states);
                for (auto s : states) {
                    b->append(L"  ")->append(s->number);
                    if (s->accept) {
                        b->append(L" [shape=doublecircle,label=\"\"];\n");
                    } else {
                        b->append(L" [shape=circle,label=\"\"];\n");
                    }
                    if (s == initial) {
                        b->append(L"  initial [shape=plaintext,label=\"\"];\n");
                        b->append(L"  initial -> ")->append(s->number)->append(L"\n");
                    }
                    for (auto t : s->transitions) {
                        b->append(L"  ")->append(s->number);
                        t->appendDot(b);
                    }
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                return b->append(L"}\n")->toString();
            }

            Automaton * Automaton::cloneExpanded() {
                Automaton * a = clone();
                a->expandSingleton();
                return a;
            }

            Automaton * Automaton::cloneExpandedIfRequired() {
                if (allow_mutation) {
                    expandSingleton();
                    return this;
                } else {
                    return cloneExpanded();
                }
            }

            Automaton * Automaton::clone() {
                try {
                    Automaton * a = static_cast<Automaton *>(__super::clone());
                    if (!isSingleton()) {
                        std::unordered_map<State *, State *> m;
                        std::set<State *> * states = getStates();
                        for (auto s : states) {
                            m[s] = new State();
                        }
                        for (auto s : states) {
                            State * p = m[s];
                            p->accept = s->accept;
                            if (s == initial) {
                                a->initial = p;
                            }
                            for (auto t : s->transitions) {
                                Transition tempVar(t->min, t->max, m[t->to]);
                                p->transitions->add(&tempVar);
                            }
                        }
                    }
                    return a;
                }
                catch (const CloneNotSupportedException & e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
                    throw std::exception();
                }
            }

            Automaton * Automaton::cloneIfRequired() {
                if (allow_mutation) {
                    return this;
                } else {
                    return clone();
                }
            }

//			Automaton *Automaton::load(URL *url) throw(IOException, ClassCastException, ClassNotFoundException)
//			{
//				return load(url->openStream());
//			}

            Automaton *
            Automaton::load(std::istream stream) throw(IOException, ClassCastException, ClassNotFoundException) {
                ObjectInputStream * s = new ObjectInputStream(stream);
                return static_cast<Automaton *>(s->readObject());
            }

            void Automaton::store(std::ostream stream) throw(IOException) {
                ObjectOutputStream * s = new ObjectOutputStream(stream);
                s->writeObject(this);
                s->flush();
            }

            Automaton * Automaton::makeEmpty() {
                return BasicAutomata::makeEmpty();
            }

            Automaton * Automaton::makeEmptyString() {
                return BasicAutomata::makeEmptyString();
            }

            Automaton * Automaton::makeAnyString() {
                return BasicAutomata::makeAnyString();
            }

            Automaton * Automaton::makeAnyChar() {
                return BasicAutomata::makeAnyChar();
            }

            Automaton * Automaton::makeChar(wchar_t c) {
                return BasicAutomata::makeChar(c);
            }

            Automaton * Automaton::makeCharRange(wchar_t min, wchar_t max) {
                return BasicAutomata::makeCharRange(min, max);
            }

            Automaton * Automaton::makeCharSet(const std::wstring & set) {
                return BasicAutomata::makeCharSet(set);
            }

            Automaton * Automaton::makeInterval(int min, int max, int digits) throw(std::invalid_argument) {
                return BasicAutomata::makeInterval(min, max, digits);
            }

            Automaton * Automaton::makeString(const std::wstring & s) {
                return BasicAutomata::makeString(s);
            }

            Automaton * Automaton::makeStringUnion(std::vector<std::string> & strings) {
                return BasicAutomata::makeStringUnion(strings);
            }

            Automaton * Automaton::makeMaxInteger(const std::wstring & n) {
                return BasicAutomata::makeMaxInteger(n);
            }

            Automaton * Automaton::makeMinInteger(const std::wstring & n) {
                return BasicAutomata::makeMinInteger(n);
            }

            Automaton * Automaton::makeTotalDigits(int i) {
                return BasicAutomata::makeTotalDigits(i);
            }

            Automaton * Automaton::makeFractionDigits(int i) {
                return BasicAutomata::makeFractionDigits(i);
            }

            Automaton * Automaton::makeIntegerValue(const std::wstring & value) {
                return BasicAutomata::makeIntegerValue(value);
            }

            Automaton * Automaton::makeDecimalValue(const std::wstring & value) {
                return BasicAutomata::makeDecimalValue(value);
            }

            Automaton * Automaton::makeStringMatcher(const std::wstring & s) {
                return BasicAutomata::makeStringMatcher(s);
            }

            Automaton * Automaton::concatenate(Automaton * a) {
                return BasicOperations::concatenate(this, a);
            }

            Automaton * Automaton::concatenate(std::vector<Automaton *> & l) {
                return BasicOperations::concatenate(l);
            }

            Automaton * Automaton::optional() {
                return BasicOperations::optional(this);
            }

            Automaton * Automaton::repeat() {
                return BasicOperations::repeat(this);
            }

            Automaton * Automaton::repeat(int min) {
                return BasicOperations::repeat(this, min);
            }

            Automaton * Automaton::repeat(int min, int max) {
                return BasicOperations::repeat(this, min, max);
            }

            Automaton * Automaton::complement() {
                return BasicOperations::complement(this);
            }

            Automaton * Automaton::minus(Automaton * a) {
                return BasicOperations::minus(this, a);
            }

            Automaton * Automaton::intersection(Automaton * a) {
                return BasicOperations::intersection(this, a);
            }

            bool Automaton::subsetOf(Automaton * a) {
                return BasicOperations::subsetOf(this, a);
            }

            Automaton * Automaton::union_Renamed(Automaton * a) {
                return BasicOperations::union_Renamed(this, a);
            }

            Automaton * Automaton::union_Renamed(std::vector<Automaton *> * l) {
                return BasicOperations::union_Renamed(l);
            }

            void Automaton::determinize() {
                BasicOperations::determinize(this);
            }

            void Automaton::addEpsilons(std::vector<StatePair *> * pairs) {
                BasicOperations::addEpsilons(this, pairs);
            }

            bool Automaton::isEmptyString() {
                return BasicOperations::isEmptyString(this);
            }

            bool Automaton::isEmpty() {
                return BasicOperations::isEmpty(this);
            }

            bool Automaton::isTotal() {
                return BasicOperations::isTotal(this);
            }

            std::wstring Automaton::getShortestExample(bool accepted) {
                return BasicOperations::getShortestExample(this, accepted);
            }

            bool Automaton::run(const std::wstring & s) {
                return BasicOperations::run(this, s);
            }

            void Automaton::minimize() {
                MinimizationOperations::minimize(this);
            }

            Automaton * Automaton::minimize(Automaton * a) {
                a->minimize();
                return a;
            }

            Automaton * Automaton::overlap(Automaton * a) {
                return SpecialOperations::overlap(this, a);
            }

            Automaton * Automaton::singleChars() {
                return SpecialOperations::singleChars(this);
            }

            Automaton * Automaton::trim(const std::wstring & set, wchar_t c) {
                return SpecialOperations::trim(this, set, c);
            }

            Automaton * Automaton::compress(const std::wstring & set, wchar_t c) {
                return SpecialOperations::compress(this, set, c);
            }

            Automaton * Automaton::subst(std::unordered_map<wchar_t, std::set<wchar_t> *> & map) {
                return SpecialOperations::subst(this, map);
            }

            Automaton * Automaton::subst(wchar_t c, const std::wstring & s) {
                return SpecialOperations::subst(this, c, s);
            }

            Automaton * Automaton::homomorph(std::vector<wchar_t> & source, std::vector<wchar_t> & dest) {
                return SpecialOperations::homomorph(this, source, dest);
            }

            Automaton * Automaton::projectChars(std::set<wchar_t> * chars) {
                return SpecialOperations::projectChars(this, chars);
            }

            bool Automaton::isFinite() {
                return SpecialOperations::isFinite(this);
            }

            std::set<std::wstring> * Automaton::getStrings(int length) {
                return SpecialOperations::getStrings(this, length);
            }

            std::set<std::wstring> * Automaton::getFiniteStrings() {
                return SpecialOperations::getFiniteStrings(this);
            }

            std::set<std::wstring> * Automaton::getFiniteStrings(int limit) {
                return SpecialOperations::getFiniteStrings(this, limit);
            }

            std::wstring Automaton::getCommonPrefix() {
                return SpecialOperations::getCommonPrefix(this);
            }

            void Automaton::prefixClose() {
                SpecialOperations::prefixClose(this);
            }

            Automaton * Automaton::hexCases(Automaton * a) {
                return SpecialOperations::hexCases(a);
            }

            Automaton * Automaton::replaceWhitespace(Automaton * a) {
                return SpecialOperations::replaceWhitespace(a);
            }

            std::wstring Automaton::shuffleSubsetOf(std::vector<Automaton *> * ca, Automaton * a,
                                                    boost::optional<wchar_t> suspend_shuffle,
                                                    boost::optional<wchar_t> resume_shuffle) {
                return ShuffleOperations::shuffleSubsetOf(ca, a, suspend_shuffle, resume_shuffle);
            }

            Automaton * Automaton::shuffle(Automaton * a) {
                return ShuffleOperations::shuffle(this, a);
            }
        }
    }
}
