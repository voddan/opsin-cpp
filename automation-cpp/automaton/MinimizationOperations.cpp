#include <utils/RectangularVectors.h>
#include "MinimizationOperations.h"
#include "Automaton.h"
#include "Transition.h"
#include "State.h"
#include "BasicOperations.h"
#include "SpecialOperations.h"

namespace dk {
    namespace brics {
        namespace automaton {

            MinimizationOperations::MinimizationOperations() {
            }

            void MinimizationOperations::minimize(Automaton * a) {
                if (!a->isSingleton()) {
                    switch (Automaton::minimization) {
                        case Automaton::MINIMIZE_HUFFMAN:
                            minimizeHuffman(a);
                            break;
                        case Automaton::MINIMIZE_BRZOZOWSKI:
                            minimizeBrzozowski(a);
                            break;
                        case Automaton::MINIMIZE_VALMARI:
                            minimizeValmari(a);
                            break;
                        default:
                            minimizeHopcroft(a);
                    }
                }
                a->recomputeHashCode();
            }

            bool MinimizationOperations::statesAgree(std::vector<std::vector<Transition *>> & transitions,
                                                     std::vector<std::vector<bool>> & mark, int n1, int n2) {
                std::vector<Transition *> t1 = transitions[n1];
                std::vector<Transition *> t2 = transitions[n2];
                for (int k1 = 0, k2 = 0; k1 < t1.size() && k2 < t2.size();) {
                    if (t1[k1]->max < t2[k2]->min) {
                        k1++;
                    } else if (t2[k2]->max < t1[k1]->min) {
                        k2++;
                    } else {
                        int m1 = t1[k1]->to.number;
                        int m2 = t2[k2]->to.number;
                        if (m1 > m2) {
                            int t = m1;
                            m1 = m2;
                            m2 = t;
                        }
                        if (mark[m1][m2]) {
                            return false;
                        }
                        if (t1[k1]->max < t2[k2]->max) {
                            k1++;
                        } else {
                            k2++;
                        }
                    }
                }
                return true;
            }

            void MinimizationOperations::addTriggers(std::vector<std::vector<Transition *>> & transitions,
                                                     std::vector<std::vector<std::unordered_set<IntPair *>>> & triggers,
                                                     int n1, int n2) {
                std::vector<Transition *> t1 = transitions[n1];
                std::vector<Transition *> t2 = transitions[n2];
                for (int k1 = 0, k2 = 0; k1 < t1.size() && k2 < t2.size();) {
                    if (t1[k1]->max < t2[k2]->min) {
                        k1++;
                    } else if (t2[k2]->max < t1[k1]->min) {
                        k2++;
                    } else {
                        if (t1[k1]->to != t2[k2]->to) {
                            int m1 = t1[k1]->to.number;
                            int m2 = t2[k2]->to.number;
                            if (m1 > m2) {
                                int t = m1;
                                m1 = m2;
                                m2 = t;
                            }
                            if (triggers[m1][m2] == nullptr) {
                                triggers[m1][m2] = std::unordered_set<IntPair *>();
                            }
                            IntPair tempVar(n1, n2);
                            triggers[m1][m2].insert(&tempVar);
                        }
                        if (t1[k1]->max < t2[k2]->max) {
                            k1++;
                        } else {
                            k2++;
                        }
                    }
                }
            }

            void MinimizationOperations::markPair(std::vector<std::vector<bool>> & mark,
                                                  std::vector<std::vector<std::unordered_set<IntPair *>>> & triggers,
                                                  int n1, int n2) {
                mark[n1][n2] = true;
                if (triggers[n1][n2] != nullptr) {
                    for (IntPair * p : triggers[n1][n2]) {
                        int m1 = p->n1;
                        int m2 = p->n2;
                        if (m1 > m2) {
                            int t = m1;
                            m1 = m2;
                            m2 = t;
                        }
                        if (!mark[m1][m2]) {
                            markPair(mark, triggers, m1, m2);
                        }
                    }
                }
            }

            template<typename T>
            void MinimizationOperations::initialize(std::vector<T> & list, int size) {
                for (int i = 0; i < size; i++) {
                    list.push_back(nullptr);
                }
            }

            void MinimizationOperations::minimizeHuffman(Automaton * a) {
                a->determinize();
                a->totalize();
                Set < State * > *ss = a->getStates();
                std::vector<std::vector<Transition *>> transitions(ss->size());
                std::vector<State *> states = ss->toArray(std::vector<State *>(ss->size()));
//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: boolean[][] mark = new boolean[states.length][states.length];
                std::vector<std::vector<bool>> mark = RectangularVectors::ReturnRectangularVector(states.size(),
                                                                                                  states.size());
                std::vector<std::vector<std::unordered_set<IntPair *>>> triggers;
                for (int n1 = 0; n1 < states.size(); n1++) {
                    std::vector<std::unordered_set<IntPair *>> v;
                    initialize(v, states.size());
                    triggers.push_back(v);
                }
                // initialize marks based on acceptance status and find transition arrays
                for (int n1 = 0; n1 < states.size(); n1++) {
                    states[n1]->number = n1;
                    transitions[n1] = states[n1]->getSortedTransitionArray(false);
                    for (int n2 = n1 + 1; n2 < states.size(); n2++) {
                        if (states[n1]->accept != states[n2]->accept) {
                            mark[n1][n2] = true;
                        }
                    }
                }
                // for all pairs, see if states agree
                for (int n1 = 0; n1 < states.size(); n1++) {
                    for (int n2 = n1 + 1; n2 < states.size(); n2++) {
                        if (!mark[n1][n2]) {
                            if (statesAgree(transitions, mark, n1, n2)) {
                                addTriggers(transitions, triggers, n1, n2);
                            } else {
                                markPair(mark, triggers, n1, n2);
                            }
                        }
                    }
                }
                // assign equivalence class numbers to states
                int numclasses = 0;
                for (int n = 0; n < states.size(); n++) {
                    states[n]->number = -1;
                }
                for (int n1 = 0; n1 < states.size(); n1++) {
                    if (states[n1]->number == -1) {
                        states[n1]->number = numclasses;
                        for (int n2 = n1 + 1; n2 < states.size(); n2++) {
                            if (!mark[n1][n2]) {
                                states[n2]->number = numclasses;
                            }
                        }
                        numclasses++;
                    }
                }
                // make a new state for each equivalence class
                std::vector<State *> newstates(numclasses);
                for (int n = 0; n < numclasses; n++) {
                    newstates[n] = new State();
                }
                // select a class representative for each class and find the new initial
                // state
                for (int n = 0; n < states.size(); n++) {
                    newstates[states[n]->number]->number = n;
                    if (states[n] == a->initial) {
                        a->initial = newstates[states[n]->number];
                    }
                }
                // build transitions and set acceptance
                for (int n = 0; n < numclasses; n++) {
                    State * s = newstates[n];
                    s->accept = states[s->number]->accept;
                    for (auto t : states[s->number]->transitions) {
                        Transition tempVar(t->min, t->max, newstates[t->to->number]);
                        s->transitions->add(&tempVar);
                    }
                }
                a->removeDeadTransitions();
            }

            void MinimizationOperations::minimizeBrzozowski(Automaton * a) {
                if (a->isSingleton()) {
                    return;
                }
                BasicOperations::determinize(a, SpecialOperations::reverse(a));
                BasicOperations::determinize(a, SpecialOperations::reverse(a));
            }

            void MinimizationOperations::minimizeHopcroft(Automaton * a) {
                a->determinize();
                Set < Transition * > *tr = a->initial->getTransitions();
                if (tr->size() == 1) {
                    Transition * t = tr->begin()->next();
                    if (t->to == a->initial && t->min == std::numeric_limits<wchar_t>::min() &&
                        t->max == std::numeric_limits<wchar_t>::max()) {
                        return;
                    }
                }
                a->totalize();
                // make arrays for numbered states and effective alphabet
                Set < State * > *ss = a->getStates();
                std::vector<State *> states(ss->size());
                int number = 0;
                for (auto q : ss) {
                    states[number] = q;
                    q->number = number++;
                }
                std::vector<wchar_t> sigma = a->getStartPoints();
                // initialize data structures
                std::vector<std::vector<std::list<State *>>> reverse;
                for (int q = 0; q < states.size(); q++) {
                    std::vector<std::list<State *>> v;
                    initialize(v, sigma.size());
                    reverse.push_back(v);
                }
//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: boolean[][] reverse_nonempty = new boolean[states.length][sigma.length];
                std::vector<std::vector<bool>> reverse_nonempty = RectangularVectors::ReturnRectangularVector(
                        states.size(), sigma.size());
                std::vector<std::list<State *>> partition;
                initialize(partition, states.size());
                std::vector<int> block(states.size());
//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: StateList[][] active = new StateList[states.length][sigma.length];
                std::vector<std::vector<StateList *>> active = RectangularVectors::ReturnRectangularVector(
                        states.size(), sigma.size());
//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: StateListNode[][] active2 = new StateListNode[states.length][sigma.length];
                std::vector<std::vector<StateListNode *>> active2 = RectangularVectors::ReturnRectangularVector(
                        states.size(), sigma.size());
                std::list<IntPair *> pending;
//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: boolean[][] pending2 = new boolean[sigma.length][states.length];
                std::vector<std::vector<bool>> pending2 = RectangularVectors::ReturnRectangularVector(sigma.size(),
                                                                                                      states.size());
                std::vector<State *> split;
                std::vector<bool> split2(states.size());
                std::vector<int> refine;
                std::vector<bool> refine2(states.size());
                std::vector<std::vector<State *>> splitblock;
                initialize(splitblock, states.size());
                for (int q = 0; q < states.size(); q++) {
                    splitblock[q] = std::vector<State *>();
                    partition[q] = std::list<State *>();
                    for (int x = 0; x < sigma.size(); x++) {
                        reverse[q][x] = std::list<State *>();
                        active[q][x] = new StateList();
                    }
                }
                // find initial partition and reverse edges
                for (int q = 0; q < states.size(); q++) {
                    State * qq = states[q];
                    int j;
                    if (qq->accept) {
                        j = 0;
                    } else {
                        j = 1;
                    }
                    partition[j].push_back(qq);
                    block[qq->number] = j;
                    for (int x = 0; x < sigma.size(); x++) {
                        wchar_t y = sigma[x];
                        State * p = qq->step(y);
                        reverse[p->number][x].push_back(qq);
                        reverse_nonempty[p->number][x] = true;
                    }
                }
                // initialize active sets
                for (int j = 0; j <= 1; j++) {
                    for (int x = 0; x < sigma.size(); x++) {
                        for (auto qq : partition[j]) {
                            if (reverse_nonempty[qq->number][x]) {
                                active2[qq->number][x] = active[j][x]->add(qq);
                            }
                        }
                    }
                }
                // initialize pending
                for (int x = 0; x < sigma.size(); x++) {
                    int a0 = active[0][x]->size;
                    int a1 = active[1][x]->size;
                    int j;
                    if (a0 <= a1) {
                        j = 0;
                    } else {
                        j = 1;
                    }
                    IntPair tempVar(j, x);
                    pending.push_back(&tempVar);
                    pending2[x][j] = true;
                }
                // process pending until fixed point
                int k = 2;
                while (!pending.empty()) {
                    IntPair * ip = pending.pop_front();
                    int p = ip->n1;
                    int x = ip->n2;
                    pending2[x][p] = false;
                    // find states that need to be split off their blocks
                    for (StateListNode * m = active[p][x]->first; m != nullptr; m = m->next) {
                        for (State * s : reverse[m->q->number][x]) {
                            if (!split2[s->number]) {
                                split2[s->number] = true;
                                split.push_back(s);
                                int j = block[s->number];
                                splitblock[j].push_back(s);
                                if (!refine2[j]) {
                                    refine2[j] = true;
                                    refine.push_back(j);
                                }
                            }
                        }
                    }
                    // refine blocks
                    for (auto j : refine) {
                        if (splitblock[j].size() < partition[j].size()) {
                            std::list<State *> b1 = partition[j];
                            std::list<State *> b2 = partition[k];
                            for (auto s : splitblock[j]) {
                                b1.remove(s);
                                b2.push_back(s);
                                block[s->number] = k;
                                for (int c = 0; c < sigma.size(); c++) {
                                    StateListNode * sn = active2[s->number][c];
                                    if (sn != nullptr && sn->sl == active[j][c]) {
                                        sn->remove();
                                        active2[s->number][c] = active[k][c]->add(s);
                                    }
                                }
                            }
                            // update pending
                            for (int c = 0; c < sigma.size(); c++) {
                                int aj = active[j][c]->size;
                                int ak = active[k][c]->size;
                                if (!pending2[c][j] && 0 < aj && aj <= ak) {
                                    pending2[c][j] = true;
                                    IntPair tempVar2(j, c);
                                    pending.push_back(&tempVar2);
                                } else {
                                    pending2[c][k] = true;
                                    IntPair tempVar3(k, c);
                                    pending.push_back(&tempVar3);
                                }
                            }
                            k++;
                        }
                        for (auto s : splitblock[j]) {
                            split2[s->number] = false;
                        }
                        refine2[j] = false;
                        splitblock[j].clear();
                    }
                    split.clear();
                    refine.clear();
                }
                // make a new state for each equivalence class, set initial state
                std::vector<State *> newstates(k);
                for (int n = 0; n < newstates.size(); n++) {
                    State * s = new State();
                    newstates[n] = s;
                    for (auto q : partition[n]) {
                        if (q == a->initial) {
                            a->initial = s;
                        }
                        s->accept = q->accept;
                        s->number = q->number; // select representative
                        q->number = n;
                    }
                }
                // build transitions and set acceptance
                for (int n = 0; n < newstates.size(); n++) {
                    State * s = newstates[n];
                    s->accept = states[s->number]->accept;
                    for (auto t : states[s->number]->transitions) {
                        Transition tempVar4(t->min, t->max, newstates[t->to->number]);
                        s->transitions->add(&tempVar4);
                    }
                }
                a->removeDeadTransitions();
            }

            void MinimizationOperations::minimizeValmari(Automaton * automaton) {
                automaton->determinize();
                Set < State * > *states = automaton->getStates();
                splitTransitions(states);
                int stateCount = states->size();
                int transitionCount = automaton->getNumberOfTransitions();
                Set < State * > *acceptStates = automaton->getAcceptStates();
                Partition * blocks = new Partition(stateCount);
                Partition * cords = new Partition(transitionCount);
                std::vector<IntPair *> labels(transitionCount);
                std::vector<int> tails(transitionCount);
                std::vector<int> heads(transitionCount);
                // split transitions in 'heads', 'labels', and 'tails'
                Automaton::setStateNumbers(states);
                int number = 0;
                for (State * s : automaton->getStates()) {
                    for (auto t : s->getTransitions()) {
                        tails[number] = s->number;
                        labels[number] = new IntPair(t->min, t->max);
                        heads[number] = t->getDest()->number;
                        number++;
                    }
                }
                // make initial block partition
                for (auto s : acceptStates) {
                    blocks->mark(s->number);
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                blocks->split();
                // make initial transition partition
                if (transitionCount > 0) {
                    LabelComparator tempVar(labels);
                    Arrays::sort(cords->elements, &tempVar);
                    cords->setCount = cords->markedElementCount[0] = 0;
                    IntPair * a = labels[cords->elements[0]];
                    for (int i = 0; i < transitionCount; ++i) {
                        int t = cords->elements[i];
                        if (labels[t]->n1 != a->n1 || labels[t]->n2 != a->n2) {
                            a = labels[t];
                            cords->past[cords->setCount++] = i;
                            cords->first[cords->setCount] = i;
                            cords->markedElementCount[cords->setCount] = 0;
                        }
                        cords->setNo[t] = cords->setCount;
                        cords->locations[t] = i;
                    }
                    cords->past[cords->setCount++] = transitionCount;
                }
                // split blocks and cords
                std::vector<int> A(transitionCount);
                std::vector<int> F(stateCount + 1);
                makeAdjacent(A, F, heads, stateCount, transitionCount);
                for (int c = 0; c < cords->setCount; ++c) {
                    for (int i = cords->first[c]; i < cords->past[c]; ++i) {
                        blocks->mark(tails[cords->elements[i]]);
                    }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                    blocks->split();
                    for (int b = 1; b < blocks->setCount; ++b) {
                        for (int i = blocks->first[b]; i < blocks->past[b]; ++i) {
                            for (int j = F[blocks->elements[i]]; j < F[blocks->elements[i] + 1]; ++j) {
                                cords->mark(A[j]);
                            }
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                        cords->split();
                    }
                }
                // build states and acceptance states
                std::vector<State *> newStates(blocks->setCount);
                for (int bl = 0; bl < blocks->setCount; ++bl) {
                    newStates[bl] = new State();
                    if (blocks->first[bl] < acceptStates->size()) {
                        newStates[bl]->accept = true;
                    }
                }
                // build transitions
                for (int t = 0; t < transitionCount; ++t) {
                    if (blocks->locations[tails[t]] == blocks->first[blocks->setNo[tails[t]]]) {
                        State * tail = newStates[blocks->setNo[tails[t]]];
                        State * head = newStates[blocks->setNo[heads[t]]];
                        Transition tempVar2(static_cast<wchar_t>(labels[t]->n1), static_cast<wchar_t>(labels[t]->n2),
                                            head);
                        tail->addTransition(&tempVar2);
                    }
                }
                automaton->setInitialState(newStates[blocks->setNo[automaton->getInitialState().number]]);
                automaton->reduce();
            }

            void MinimizationOperations::makeAdjacent(std::vector<int> & A, std::vector<int> & F, std::vector<int> & K,
                                                      int nn, int mm) {
                for (int q = 0; q <= nn; ++q) {
                    F[q] = 0;
                }
                for (int t = 0; t < mm; ++t) {
                    ++F[K[t]];
                }
                for (int q = 0; q < nn; ++q) {
                    F[q + 1] += F[q];
                }
                for (int t = mm; t-- > 0;) {
                    A[--F[K[t]]] = t;
                }
            }

            void MinimizationOperations::splitTransitions(std::set<State *> * states) {
                std::set<wchar_t> pointSet;
                for (auto s : states) {
                    for (auto t : s->getTransitions()) {
                        pointSet.insert(t->min);
                        pointSet.insert(t->max);
                    }
                }
                for (auto s : states) {
                    Set < Transition * > *transitions = s->getTransitions();
                    s->resetTransitions();
                    for (auto t : transitions) {
                        if (t->min == t->max) {
                            s->addTransition(t);
                            continue;
                        }
                        SortedSet<wchar_t> * headSet = pointSet.headSet(t->max, true);
                        SortedSet<wchar_t> * tailSet = pointSet.tailSet(t->min, false);
                        SortedSet<wchar_t> * intersection = std::set<wchar_t>(headSet);
                        intersection->retainAll(tailSet);
                        wchar_t start = t->min;
                        for (* : :optional < wchar_t > c : intersection)
                        {
                            Transition tempVar(start, t->to);
                            s->addTransition(&tempVar);
                            Transition tempVar2(c, t->to);
                            s->addTransition(&tempVar2);
                            if (c - start > 1) {
                                Transition tempVar3(static_cast<wchar_t>(start + 1), static_cast<wchar_t>(c - 1),
                                                    t->to);
                                s->addTransition(&tempVar3);
                            }
                            start = c;
                        }
                    }
                }
            }

            MinimizationOperations::IntPair::IntPair(int n1, int n2) {
                this->n1 = n1;
                this->n2 = n2;
            }

            StateListNode * MinimizationOperations::StateList::add(State * q) {
                return new StateListNode(q, this);
            }

            MinimizationOperations::StateListNode::StateListNode(State * q, StateList * sl) {
                this->q = q;
                this->sl = sl;
                if (sl->size++ == 0) {
                    sl->first = sl->last = this;
                } else {
                    sl->last->next = this;
                    prev = sl->last;
                    sl->last = this;
                }
            }

            void MinimizationOperations::StateListNode::remove() {
                sl->size--;
                if (sl->first == this) {
                    sl->first = next;
                } else {
                    prev->next = next;
                }
                if (sl->last == this) {
                    sl->last = prev;
                } else {
                    next->prev = prev;
                }
            }

            MinimizationOperations::Partition::Partition(int size) {
                setCount = (size == 0) ? 0 : 1;
                elements = std::vector<boost::optional<int> *>(size);
                locations = std::vector<int>(size);
                setNo = std::vector<int>(size);
                first = std::vector<int>(size);
                past = std::vector<int>(size);
                markedElementCount = std::vector<int>(size);
                touchedSets = std::vector<int>(size);
                for (int i = 0; i < size; ++i) {
                    elements[i] = locations[i] = i;
                    setNo[i] = 0;
                }
                if (setCount != 0) {
                    first[0] = 0;
                    past[0] = size;
                }
            }

            void MinimizationOperations::Partition::mark(int e) {
                int s = setNo[e];
                int i = locations[e];
                int j = first[s] + markedElementCount[s];
                elements[i] = elements[j];
                locations[elements[i]] = i;
                elements[j] = e;
                locations[e] = j;
                if (markedElementCount[s]++ == 0) {
                    touchedSets[touchedSetCount++] = s;
                }
            }

            void MinimizationOperations::Partition::split() {
                while (touchedSetCount > 0) {
                    int s = touchedSets[--touchedSetCount];
                    int j = first[s] + markedElementCount[s];
                    if (j == past[s]) {
                        markedElementCount[s] = 0;
                        continue;
                    }
                    // choose the smaller of the marked and unmarked part, and make it a new set
                    if (markedElementCount[s] <= past[s] - j) {
                        first[setCount] = first[s];
                        past[setCount] = first[s] = j;
                    } else {
                        past[setCount] = past[s];
                        first[setCount] = past[s] = j;
                    }
                    for (int i = first[setCount]; i < past[setCount]; ++i) {
                        setNo[elements[i]] = setCount;
                    }
                    markedElementCount[s] = markedElementCount[setCount++] = 0;
                }
            }

            MinimizationOperations::LabelComparator::LabelComparator(std::vector<IntPair *> & labels) {
                this->labels = labels;
            }

            int MinimizationOperations::LabelComparator::compare(boost::optional<int> i, boost::optional<int> j) {
                IntPair * p1 = labels[i];
                IntPair * p2 = labels[j];
                if (p1->n1 < p2->n1) {
                    return -1;
                }
                if (p1->n1 > p2->n1) {
                    return 1;
                }
                if (p1->n2 < p2->n2) {
                    return -1;
                }
                if (p1->n2 > p2->n2) {
                    return 1;
                }
                return 0;
            }
        }
    }
}
