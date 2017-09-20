#include "BasicOperations.h"
#include "Automaton.h"
#include "BasicAutomata.h"
#include "State.h"
#include "Transition.h"
#include "StatePair.h"

namespace dk {
    namespace brics {
        namespace automaton {

            BasicOperations::BasicOperations() {
            }

            Automaton * BasicOperations::concatenate(Automaton * a1, Automaton * a2) {
                if (a1->isSingleton() && a2->isSingleton()) {
                    return BasicAutomata::makeString(a1->singleton + a2->singleton);
                }
                if (isEmpty(a1) || isEmpty(a2)) {
                    return BasicAutomata::makeEmpty();
                }
                bool deterministic = a1->isSingleton() && a2->isDeterministic();
                if (a1 == a2) {
                    a1 = a1->cloneExpanded();
                    a2 = a2->cloneExpanded();
                } else {
                    a1 = a1->cloneExpandedIfRequired();
                    a2 = a2->cloneExpandedIfRequired();
                }
                for (auto s : a1->getAcceptStates()) {
                    s->accept = false;
                    s->addEpsilon(a2->initial);
                }
                a1->deterministic = deterministic;
                a1->clearHashCode();
                a1->checkMinimizeAlways();
                return a1;
            }

            Automaton * BasicOperations::concatenate(std::vector<Automaton *> & l) {
                if (l.empty()) {
                    return BasicAutomata::makeEmptyString();
                }
                bool all_singleton = true;
                for (auto a : l) {
                    if (!a->isSingleton()) {
                        all_singleton = false;
                        break;
                    }
                }
                if (all_singleton) {
                    StringBuilder * b = new StringBuilder();
                    for (auto a : l) {
                        b->append(a->singleton);
                    }
                    return BasicAutomata::makeString(b->toString());
                } else {
                    for (auto a : l) {
                        if (a->isEmpty()) {
                            return BasicAutomata::makeEmpty();
                        }
                    }
                    Set<int> * ids = std::unordered_set<int>();
                    for (auto a : l) {
                        ids->add(System::identityHashCode(a));
                    }
                    bool has_aliases = ids->size() != l.size();
                    Automaton * b = l[0];
                    if (has_aliases) {
                        b = b->cloneExpanded();
                    } else {
                        b = b->cloneExpandedIfRequired();
                    }
                    Set < State * > *ac = b->getAcceptStates();
                    bool first = true;
                    for (auto a : l) {
                        if (first) {
                            first = false;
                        } else {
                            if (a->isEmptyString()) {
                                continue;
                            }
                            Automaton * aa = a;
                            if (has_aliases) {
                                aa = aa->cloneExpanded();
                            } else {
                                aa = aa->cloneExpandedIfRequired();
                            }
                            Set < State * > *ns = aa->getAcceptStates();
                            for (auto s : ac) {
                                s->accept = false;
                                s->addEpsilon(aa->initial);
                                if (s->accept) {
                                    ns->add(s);
                                }
                            }
                            ac = ns;
                        }
                    }
                    b->deterministic = false;
                    b->clearHashCode();
                    b->checkMinimizeAlways();
                    return b;
                }
            }

            Automaton * BasicOperations::optional(Automaton * a) {
                a = a->cloneExpandedIfRequired();
                State * s = new State();
                s->addEpsilon(a->initial);
                s->accept = true;
                a->initial = s;
                a->deterministic = false;
                a->clearHashCode();
                a->checkMinimizeAlways();
                return a;
            }

            Automaton * BasicOperations::repeat(Automaton * a) {
                a = a->cloneExpanded();
                State * s = new State();
                s->accept = true;
                s->addEpsilon(a->initial);
                for (auto p : a->getAcceptStates()) {
                    p->addEpsilon(s);
                }
                a->initial = s;
                a->deterministic = false;
                a->clearHashCode();
                a->checkMinimizeAlways();
                return a;
            }

            Automaton * BasicOperations::repeat(Automaton * a, int min) {
                if (min == 0) {
                    return repeat(a);
                }
                std::vector<Automaton *> as;
                while (min-- > 0) {
                    as.push_back(a);
                }
                as.push_back(repeat(a));
                return concatenate(as);
            }

            Automaton * BasicOperations::repeat(Automaton * a, int min, int max) {
                if (min > max) {
                    return BasicAutomata::makeEmpty();
                }
                max -= min;
                a->expandSingleton();
                Automaton * b;
                if (min == 0) {
                    b = BasicAutomata::makeEmptyString();
                } else if (min == 1) {
                    b = a->clone();
                } else {
                    std::vector<Automaton *> as;
                    while (min-- > 0) {
                        as.push_back(a);
                    }
                    b = concatenate(as);
                }
                if (max > 0) {
                    Automaton * d = a->clone();
                    while (--max > 0) {
                        Automaton * c = a->clone();
                        for (auto p : c->getAcceptStates()) {
                            p->addEpsilon(d->initial);
                        }
                        d = c;
                    }
                    for (auto p : b->getAcceptStates()) {
                        p->addEpsilon(d->initial);
                    }
                    b->deterministic = false;
                    b->clearHashCode();
                    b->checkMinimizeAlways();
                }
                return b;
            }

            Automaton * BasicOperations::complement(Automaton * a) {
                a = a->cloneExpandedIfRequired();
                a->determinize();
                a->totalize();
                for (auto p : a->getStates()) {
                    p->accept = !p->accept;
                }
                a->removeDeadTransitions();
                return a;
            }

            Automaton * BasicOperations::minus(Automaton * a1, Automaton * a2) {
                if (a1->isEmpty() || a1 == a2) {
                    return BasicAutomata::makeEmpty();
                }
                if (a2->isEmpty()) {
                    return a1->cloneIfRequired();
                }
                if (a1->isSingleton()) {
                    if (a2->run(a1->singleton)) {
                        return BasicAutomata::makeEmpty();
                    } else {
                        return a1->cloneIfRequired();
                    }
                }
                return intersection(a1, a2->complement());
            }

            Automaton * BasicOperations::intersection(Automaton * a1, Automaton * a2) {
                if (a1->isSingleton()) {
                    if (a2->run(a1->singleton)) {
                        return a1->cloneIfRequired();
                    } else {
                        return BasicAutomata::makeEmpty();
                    }
                }
                if (a2->isSingleton()) {
                    if (a1->run(a2->singleton)) {
                        return a2->cloneIfRequired();
                    } else {
                        return BasicAutomata::makeEmpty();
                    }
                }
                if (a1 == a2) {
                    return a1->cloneIfRequired();
                }
                std::vector<std::vector<Transition *>> transitions1 = Automaton::getSortedTransitions(a1->getStates());
                std::vector<std::vector<Transition *>> transitions2 = Automaton::getSortedTransitions(a2->getStates());
                Automaton * c = new Automaton();
                std::list<StatePair *> worklist;
                std::unordered_map<StatePair *, StatePair *> newstates;
                StatePair * p = new StatePair(c->initial, a1->initial, a2->initial);
                worklist.push_back(p);
                newstates[p] = p;
                while (worklist.size() > 0) {
                    p = worklist.pop_front();
                    p->s->accept = p->s1->accept && p->s2->accept;
                    std::vector<Transition *> t1 = transitions1[p->s1->number];
                    std::vector<Transition *> t2 = transitions2[p->s2->number];
                    for (int n1 = 0, b2 = 0; n1 < t1.size(); n1++) {
                        while (b2 < t2.size() && t2[b2]->max < t1[n1]->min) {
                            b2++;
                        }
                        for (int n2 = b2; n2 < t2.size() && t1[n1]->max >= t2[n2]->min; n2++) {
                            if (t2[n2]->max >= t1[n1]->min) {
                                StatePair * q = new StatePair(t1[n1]->to, t2[n2]->to);
                                StatePair * r = newstates[q];
                                if (r == nullptr) {
                                    q->s = new State();
                                    worklist.push_back(q);
                                    newstates[q] = q;
                                    r = q;
                                }
                                wchar_t min = t1[n1]->min > t2[n2]->min ? t1[n1]->min : t2[n2]->min;
                                wchar_t max = t1[n1]->max < t2[n2]->max ? t1[n1]->max : t2[n2]->max;
                                Transition tempVar(min, max, r->s);
                                p->s->transitions->add(&tempVar);
                            }
                        }
                    }
                }
                c->deterministic = a1->deterministic && a2->deterministic;
                c->removeDeadTransitions();
                c->checkMinimizeAlways();
                return c;
            }

            bool BasicOperations::subsetOf(Automaton * a1, Automaton * a2) {
                if (a1 == a2) {
                    return true;
                }
                if (a1->isSingleton()) {
                    if (a2->isSingleton()) {
                        return a1->singleton == a2->singleton;
                    }
                    return a2->run(a1->singleton);
                }
                a2->determinize();
                std::vector<std::vector<Transition *>> transitions1 = Automaton::getSortedTransitions(a1->getStates());
                std::vector<std::vector<Transition *>> transitions2 = Automaton::getSortedTransitions(a2->getStates());
                std::list<StatePair *> worklist;
                std::unordered_set<StatePair *> visited;
                StatePair * p = new StatePair(a1->initial, a2->initial);
                worklist.push_back(p);
                visited.insert(p);
                while (worklist.size() > 0) {
                    p = worklist.pop_front();
                    if (p->s1->accept && !p->s2->accept) {
                        return false;
                    }
                    std::vector<Transition *> t1 = transitions1[p->s1->number];
                    std::vector<Transition *> t2 = transitions2[p->s2->number];
                    for (int n1 = 0, b2 = 0; n1 < t1.size(); n1++) {
                        while (b2 < t2.size() && t2[b2]->max < t1[n1]->min) {
                            b2++;
                        }
                        int min1 = t1[n1]->min, max1 = t1[n1]->max;
                        for (int n2 = b2; n2 < t2.size() && t1[n1]->max >= t2[n2]->min; n2++) {
                            if (t2[n2]->min > min1) {
                                return false;
                            }
                            if (t2[n2]->max < std::numeric_limits<wchar_t>::max()) {
                                min1 = t2[n2]->max + 1;
                            } else {
                                min1 = std::numeric_limits<wchar_t>::max();
                                max1 = std::numeric_limits<wchar_t>::min();
                            }
                            StatePair * q = new StatePair(t1[n1]->to, t2[n2]->to);
                            if (!std::find(visited.begin(), visited.end(), q) != visited.end()) {
                                worklist.push_back(q);
                                visited.insert(q);
                            }
                        }
                        if (min1 <= max1) {
                            return false;
                        }
                    }
                }
                return true;
            }

            Automaton * BasicOperations::union_Renamed(Automaton * a1, Automaton * a2) {
                if ((a1->isSingleton() && a2->isSingleton() && a1->singleton == a2->singleton) || a1 == a2) {
                    return a1->cloneIfRequired();
                }
                a1 = a1->cloneExpandedIfRequired();
                a2 = a2->cloneExpandedIfRequired();
                State * s = new State();
                s->addEpsilon(a1->initial);
                s->addEpsilon(a2->initial);
                a1->initial = s;
                a1->deterministic = false;
                a1->clearHashCode();
                a1->checkMinimizeAlways();
                return a1;
            }

            Automaton * BasicOperations::union_Renamed(std::vector<Automaton *> * l) {
                Set<int> * ids = std::unordered_set<int>();
                for (auto a : l) {
                    ids->add(System::identityHashCode(a));
                }
                bool has_aliases = ids->size() != l->size();
                State * s = new State();
                for (auto b : l) {
                    if (b->isEmpty()) {
                        continue;
                    }
                    Automaton * bb = b;
                    if (has_aliases) {
                        bb = bb->cloneExpanded();
                    } else {
                        bb = bb->cloneExpandedIfRequired();
                    }
                    s->addEpsilon(bb->initial);
                }
                Automaton * a = new Automaton();
                a->initial = s;
                a->deterministic = false;
                a->clearHashCode();
                a->checkMinimizeAlways();
                return a;
            }

            void BasicOperations::determinize(Automaton * a) {
                if (a->deterministic || a->isSingleton()) {
                    return;
                }
                Set < State * > *initialset = std::unordered_set<State *>();
                initialset->add(a->initial);
                determinize(a, initialset);
            }

            void BasicOperations::determinize(Automaton * a, std::set<State *> * initialset) {
                std::vector<wchar_t> points = a->getStartPoints();
                // subset construction
                std::list<Set < State * >*> worklist;
                std::unordered_map<Set < State * >*, State*> newstate;
                worklist.push_back(initialset);
                a->initial = new State();
                newstate[initialset] = a->initial;
                while (worklist.size() > 0) {
                    Set < State * > *s = worklist.pop_front();
                    State * r = newstate[s];
                    for (auto q : s) {
                        if (q->accept) {
                            r->accept = true;
                            break;
                        }
                    }
                    for (int n = 0; n < points.size(); n++) {
                        Set < State * > *p = std::unordered_set<State *>();
                        for (auto q : s) {
                            for (auto t : q->transitions) {
                                if (t->min <= points[n] && points[n] <= t->max) {
                                    p->add(t->to);
                                }
                            }
                        }
                        if (!p->isEmpty()) {
                            State * q = newstate[p];
                            if (q == nullptr) {
                                worklist.push_back(p);
                                q = new State();
                                newstate[p] = q;
                            }
                            wchar_t min = points[n];
                            wchar_t max;
                            if (n + 1 < points.size()) {
                                max = static_cast<wchar_t>(points[n + 1] - 1);
                            } else {
                                max = std::numeric_limits<wchar_t>::max();
                            }
                            Transition tempVar(min, max, q);
                            r->transitions->add(&tempVar);
                        }
                    }
                }
                a->deterministic = true;
                a->removeDeadTransitions();
            }

            void BasicOperations::addEpsilons(Automaton * a, std::vector<StatePair *> * pairs) {
                a->expandSingleton();
                std::unordered_map<State *, std::unordered_set<State *>> forward;
                std::unordered_map<State *, std::unordered_set<State *>> back;
                for (auto p : pairs) {
                    std::unordered_set<State *> to = forward[p->s1];
                    if (to == nullptr) {
                        to = std::unordered_set<State *>();
                        forward[p->s1] = to;
                    }
                    to.insert(p->s2);
                    std::unordered_set<State *> from = back[p->s2];
                    if (from == nullptr) {
                        from = std::unordered_set<State *>();
                        back[p->s2] = from;
                    }
                    from.insert(p->s1);
                }
                // calculate epsilon closure
                std::list<StatePair *> worklist(pairs);
                std::unordered_set<StatePair *> workset(pairs);
                while (!worklist.empty()) {
                    StatePair * p = worklist.pop_front();
                    workset.remove(p);
                    std::unordered_set<State *> to = forward[p->s2];
                    std::unordered_set<State *> from = back[p->s1];
                    if (to != nullptr) {
                        for (auto s : to) {
                            StatePair * pp = new StatePair(p->s1, s);
                            if (!pairs->contains(pp)) {
                                pairs->add(pp);
                                forward[p->s1].insert(s);
                                back[s].insert(p->s1);
                                worklist.push_back(pp);
                                workset.insert(pp);
                                if (from != nullptr) {
                                    for (auto q : from) {
                                        StatePair * qq = new StatePair(q, p->s1);
                                        if (!std::find(workset.begin(), workset.end(), qq) != workset.end()) {
                                            worklist.push_back(qq);
                                            workset.insert(qq);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // add transitions
                for (auto p : pairs) {
                    p->s1->addEpsilon(p->s2);
                }
                a->deterministic = false;
                a->clearHashCode();
                a->checkMinimizeAlways();
            }

            bool BasicOperations::isEmptyString(Automaton * a) {
                if (a->isSingleton()) {
                    return a->singleton.length() == 0;
                } else {
                    return a->initial->accept && a->initial->transitions->isEmpty();
                }
            }

            bool BasicOperations::isEmpty(Automaton * a) {
                if (a->isSingleton()) {
                    return false;
                }
                return !a->initial->accept && a->initial->transitions->isEmpty();
            }

            bool BasicOperations::isTotal(Automaton * a) {
                if (a->isSingleton()) {
                    return false;
                }
                if (a->initial->accept && a->initial->transitions->size() == 1) {
                    Transition * t = a->initial->transitions->begin()->next();
                    return t->to == a->initial && t->min == std::numeric_limits<wchar_t>::min() &&
                           t->max == std::numeric_limits<wchar_t>::max();
                }
                return false;
            }

            std::wstring BasicOperations::getShortestExample(Automaton * a, bool accepted) {
                if (a->isSingleton()) {
                    if (accepted) {
                        return a->singleton;
                    } else if (a->singleton.length() > 0) {
                        return L"";
                    } else {
                        return L"\u0000";
                    }

                }
                return getShortestExample(a->getInitialState(), accepted);
            }

            std::wstring BasicOperations::getShortestExample(State * s, bool accepted) {
                std::unordered_map<State *, std::wstring> path;
                std::list<State *> queue;
                path[s] = L"";
                queue.push_back(s);
                std::wstring best = L"";
                while (!queue.empty()) {
                    State * q = queue.pop_front();
                    std::wstring p = path[q];
                    if (q->accept == accepted) {
                        if (best == L"" || p.length() < best.length() ||
                            (p.length() == best.length() && p.compare(best) < 0)) {
                            best = p;
                        }
                    } else {
                        for (auto t : q->getTransitions()) {
                            std::wstring tp = path[t->to];
                            std::wstring np = p + StringHelper::toString(t->min);
                            if (tp == L"" || (tp.length() == np.length() && np.compare(tp) < 0)) {
                                if (tp == L"") {
                                    queue.push_back(t->to);
                                }
                                path[t->to] = np;
                            }
                        }
                    }
                }
                return best;
            }

            bool BasicOperations::run(Automaton * a, const std::wstring & s) {
                if (a->isSingleton()) {
                    return s == a->singleton;
                }
                if (a->deterministic) {
                    State * p = a->initial;
                    for (int i = 0; i < s.length(); i++) {
                        State * q = p->step(s[i]);
                        if (q == nullptr) {
                            return false;
                        }
                        p = q;
                    }
                    return p->accept;
                } else {
                    Set < State * > *states = a->getStates();
                    Automaton::setStateNumbers(states);
                    std::list<State *> pp;
                    std::list<State *> pp_other;
                    BitSet * bb = new BitSet(states->size());
                    BitSet * bb_other = new BitSet(states->size());
                    pp.push_back(a->initial);
                    std::vector<State *> dest;
                    bool accept = a->initial->accept;
                    for (int i = 0; i < s.length(); i++) {
                        wchar_t c = s[i];
                        accept = false;
                        pp_other.clear();
                        bb_other->clear();
                        for (auto p : pp) {
                            dest.clear();
                            p->step(c, dest);
                            for (auto q : dest) {
                                if (q->accept) {
                                    accept = true;
                                }
                                if (!bb_other->get(q->number)) {
                                    bb_other->set(q->number);
                                    pp_other.push_back(q);
                                }
                            }
                        }
                        std::list<State *> & tp = pp;
                        pp = pp_other;
                        pp_other = tp;
                        BitSet * tb = bb;
                        bb = bb_other;
                        bb_other = tb;
                    }
                    return accept;
                }
            }
        }
    }
}
