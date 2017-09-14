#include "Ring.h"
#include "Atom.h"
#include "Bond.h"


Ring::Ring(std::vector<Bond *> &bondList) : bondList(bondList) {
    if (bondList.empty() || bondList.empty()) {
        throw std::invalid_argument("Bond list is empty");
    }

    for (auto bond : bondList) {
        Atom *atom1 = bond->getFromAtom();
        if (!std::find(atomList.begin(), atomList.end(), atom1) != atomList.end()) {
            atomList.push_back(atom1);
        }

        Atom *atom2 = bond->getToAtom();
        if (!std::find(atomList.begin(), atomList.end(), atom2) != atomList.end()) {
            atomList.push_back(atom2);
        }
    }

    if (atomList.size() != bondList.size()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("atomList and bondList different sizes. Ring(bond)");
        throw std::exception();
    }
}

std::vector<Bond *> Ring::getBondList() {
    return bondList;
}

std::vector<Atom *> Ring::getAtomList() {
    return atomList;
}

int Ring::size() {
    return atomList.size();
}

int Ring::getNumberOfFusedBonds() {
    return bondToNeighbourRings.size();
}

std::vector<Bond *> Ring::getFusedBonds() {
    return std::vector<Bond *>(bondToNeighbourRings.keySet());
}

int Ring::getBondIndex(Bond *bond) {
    return cyclicBondList.find(bond);
}

std::vector<Bond *> Ring::getCyclicBondList() {
    return cyclicBondList;
}

std::vector<Atom *> Ring::getCyclicAtomList() {
    return cyclicAtomList;
}

std::vector<Ring *> Ring::getNeighbours() {
    return std::vector<Ring *>(bondToNeighbourRings.values());
}

Ring *Ring::getNeighbourOfFusedBond(Bond *fusedBond) {
    return bondToNeighbourRings[fusedBond];
}

void Ring::addNeighbour(Bond *bond, Ring *ring) {
    if (this == ring) {
        throw std::invalid_argument("Ring can't be a neighbour of itself");
    }
    bondToNeighbourRings[bond] = ring;
}

void Ring::makeCyclicLists(Bond *stBond, Atom *stAtom) {
    if (cyclicBondList.empty()) {
        cyclicBondList = std::vector<Bond *>();
        cyclicAtomList = std::vector<Atom *>();

        Atom *atom = stAtom;
        cyclicBondList.push_back(stBond);
        cyclicAtomList.push_back(atom);

        for (int i = 0; i < size() - 1; i++) {
            for (auto bond2 : bondList) {
                if (std::find(cyclicBondList.begin(), cyclicBondList.end(), bond2) != cyclicBondList.end()) {
                    continue;
                }
                if (bond2->getFromAtom() == atom) {
                    cyclicBondList.push_back(bond2);
                    atom = bond2->getToAtom();
                    cyclicAtomList.push_back(atom);
                } else if (bond2->getToAtom() == atom) {
                    cyclicBondList.push_back(bond2);
                    atom = bond2->getFromAtom();
                    cyclicAtomList.push_back(atom);
                }
            }
        }
    }
}

std::wstring Ring::toString() {
    StringBuilder *sb = new StringBuilder();
    for (auto atom : atomList) {
        sb->append(atom->getID());
        sb->append(L" ");
    }
    return sb->toString();
}
