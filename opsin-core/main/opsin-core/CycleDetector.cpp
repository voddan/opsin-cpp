#include "CycleDetector.h"
#include "Fragment.h"
#include "Atom.h"
#include "Bond.h"


void CycleDetector::assignWhetherAtomsAreInCycles(Fragment *frag) {
    std::vector<Atom *> atomList = frag->getAtomList();
    for (auto atom : atomList) {
        atom->setAtomIsInACycle(false);
        atom->setProperty(Atom::VISITED, nullptr);
    }
    for (auto a : atomList) { //as OPSIN does not disallow disconnected sections within a single "fragment" (e.g. in suffixes) for vigorousness this for loop is required
        if (a->getProperty(Atom::VISITED) == nullptr) { //true for only the first atom in a fully connected molecule
            traverseRings(a, nullptr, 0);
        }
    }
}

int CycleDetector::traverseRings(Atom *currentAtom, Atom *previousAtom, int depth) {
    boost::optional<int> previouslyAssignedDepth = currentAtom->getProperty(Atom::VISITED);
    if (previouslyAssignedDepth) {
        return previouslyAssignedDepth;
    }
    currentAtom->setProperty(Atom::VISITED, depth);
    std::vector<Atom *> equivalentAtoms;
    equivalentAtoms.push_back(currentAtom);

    std::vector<Atom *> neighbours;
    for (;;) {
        //Non-recursively process atoms in a chain
        //add the atoms in the chain to equivalentAtoms as either all or none of them are in a ring
        neighbours = currentAtom->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
        neighbours.remove(previousAtom);
        if (neighbours.size() != 1) {
            break;
        }
        Atom *nextAtom = neighbours[0];
        if (nextAtom->getProperty(Atom::VISITED) != nullptr) {
            //chain reached a previously visited atom, must be a ring
            break;
        }
        previousAtom = currentAtom;
        currentAtom = nextAtom;
        equivalentAtoms.push_back(currentAtom);
        currentAtom->setProperty(Atom::VISITED, ++depth);
    }

    int result = depth + 1;
    for (auto neighbour : neighbours) {
        int temp = traverseRings(neighbour, currentAtom, depth + 1);
        result = std::min(result, temp);
    }
    if (result < depth) {
        for (auto a : equivalentAtoms) {
            a->setAtomIsInACycle(true);
        }
    } else if (result == depth) {
        currentAtom->setAtomIsInACycle(true);
    }
    return result;
}

CycleDetector::PathSearchState::PathSearchState(Atom *currentAtom, std::vector<Atom *> &orderAtomsVisited)
        : currentAtom(currentAtom), orderAtomsVisited(orderAtomsVisited) {
}

Atom *CycleDetector::PathSearchState::getCurrentAtom() {
    return currentAtom;
}

std::vector<Atom *> CycleDetector::PathSearchState::getOrderAtomsVisited() {
    return orderAtomsVisited;
}

std::vector<std::vector<Atom *>>
CycleDetector::getPathBetweenAtomsUsingBonds(Atom *a1, Atom *a2, std::set<Bond *> *peripheryBonds) {
    std::vector<std::vector<Atom *>> paths;
    Deque < PathSearchState * > *stateStack = new ArrayDeque<PathSearchState *>();
    PathSearchState tempVar(a1, new std::vector<Atom *>());
    stateStack->add(&tempVar);
    while (stateStack->size() > 0) {
        PathSearchState *state = stateStack->removeLast(); //depth first traversal
        std::vector<Atom *> orderAtomsVisited = state->getOrderAtomsVisited();
        Atom *nextAtom = state->getCurrentAtom();
        orderAtomsVisited.push_back(nextAtom);
        Set < Bond * > *neighbourBonds = new LinkedHashSet<Bond *>(nextAtom->getBonds());
        neighbourBonds->retainAll(peripheryBonds);
        for (auto neighbourBond : neighbourBonds) {
            Atom *neighbour = neighbourBond->getOtherAtom(nextAtom);
            if (std::find(orderAtomsVisited.begin(), orderAtomsVisited.end(), neighbour) !=
                orderAtomsVisited.end()) { //atom already visited by this path
                continue;
            }
            if (neighbour == a2) { //target atom found
                paths.push_back(std::vector<Atom *>(orderAtomsVisited.subList(1, orderAtomsVisited.size())));
            } else { //add atom to stack, its neighbours will be recursively investigated shortly
                PathSearchState tempVar2(neighbour, new std::vector<Atom *>(orderAtomsVisited));
                stateStack->add(&tempVar2);
            }
        }
    }
    return paths;
}
