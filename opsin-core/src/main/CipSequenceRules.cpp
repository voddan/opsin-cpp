#include "CipSequenceRules.h"
#include "Atom.h"
#include "CipOrderingException.h"


CipSequenceRules::CipOrderingRunTimeException::CipOrderingRunTimeException(const std::wstring &message)
        : RuntimeException(message) {
}

CipSequenceRules::CipSequenceRules(Atom *chiralAtom) : chiralAtom(chiralAtom) {
}

std::vector<Atom *> CipSequenceRules::getNeighbouringAtomsInCipOrder() throw(CipOrderingException) {
    std::vector<Atom *> neighbours = chiralAtom->getAtomNeighbours();
    try {
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(neighbours, new SortByCipOrder(chiralAtom));
        SortByCipOrder tempVar(this, chiralAtom);
        std::sort(neighbours.begin(), neighbours.end(), &tempVar);
    }
    catch (const CipOrderingRunTimeException &e) {
        throw CipOrderingException(e->what());
    }
    return neighbours;
}

std::vector<Atom *> CipSequenceRules::getNeighbouringAtomsInCipOrderIgnoringGivenNeighbour(
        Atom *neighbourToIgnore) throw(CipOrderingException) {
    std::vector<Atom *> neighbours = chiralAtom->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    if (!neighbours.remove(neighbourToIgnore)) {
        throw std::invalid_argument(std::wstring(L"OPSIN bug: Atom") + neighbourToIgnore->getID() +
                                    std::wstring(L" was not a neighbour of the given stereogenic atom"));
    }
    try {
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(neighbours, new SortByCipOrder(chiralAtom));
        SortByCipOrder tempVar(this, chiralAtom);
        std::sort(neighbours.begin(), neighbours.end(), &tempVar);
    }
    catch (const CipOrderingRunTimeException &e) {
        throw CipOrderingException(e->what());
    }
    return neighbours;
}

CipSequenceRules::CipState::CipState(std::vector<AtomWithHistory *> &nextAtoms1,
                                     std::vector<AtomWithHistory *> &nextAtoms2) : nextAtoms1(nextAtoms1),
                                                                                   nextAtoms2(nextAtoms2) {
}

CipSequenceRules::AtomWithHistory::AtomWithHistory(Atom *atom, std::vector<Atom *> &visitedAtoms,
                                                   boost::optional<int> indexOfOriginalFromRoot) : atom(atom),
                                                                                                   visitedAtoms(
                                                                                                           visitedAtoms),
                                                                                                   indexOfOriginalFromRoot(
                                                                                                           indexOfOriginalFromRoot) {
}

CipSequenceRules::SortByCipOrder::SortByCipOrder(CipSequenceRules *outerInstance, Atom *chiralAtom) : chiralAtom(
        chiralAtom), outerInstance(outerInstance) {
}

int CipSequenceRules::SortByCipOrder::compare(Atom *a, Atom *b) {
    /*
     * rule = 0 --> Rule 1a Higher atomic number precedes lower
     * rule = 1 --> Rule 1b A duplicated atom, with its predecessor node having the same label closer to the root, ranks higher than a duplicated atom, with its predecessor node having the same label farther from the root, which ranks higher than any non-duplicated atom node
     * rule = 2 --> Rule 2 Higher atomic mass number precedes lower
     */
    for (rule = 0; rule <= 2; rule++) {
        std::vector<Atom *> atomsVisted;
        atomsVisted.push_back(chiralAtom);
        AtomWithHistory *aWithHistory = new AtomWithHistory(a, atomsVisted, nullptr);
        AtomWithHistory *bWithHistory = new AtomWithHistory(b, std::vector<Atom *>(atomsVisted), nullptr);

        int compare = compareByCipRules(aWithHistory, bWithHistory);
        if (compare != 0) {
            return compare;
        }

        std::vector<AtomWithHistory *> nextAtoms1;
        nextAtoms1.push_back(aWithHistory);

        std::vector<AtomWithHistory *> nextAtoms2;
        nextAtoms2.push_back(bWithHistory);

        CipState *startingState = new CipState(nextAtoms1, nextAtoms2);
        Deque < CipState * > *cipStateQueue = new ArrayDeque<CipState *>();
        cipStateQueue->add(startingState);
        /* Go through CIP states in a breadth-first manner:
         * Neighbours of the given atom/s (if multiple atoms this is because so far the two paths leading to them have been equivalent) are evaluated for both a and b
         * Neighbours are sorted by CIP priority
         * Comparisons performed between neighbours of a and neighbours of b (will break if compare != 0)
         * Degenerate neighbours grouped together
         * CIP state formed for each list of neighbours and added to queue in order of priority
         *
         */
        while (!cipStateQueue->isEmpty()) {
            CipState *currentState = cipStateQueue->removeFirst();
            compare = compareAtNextLevel(currentState, cipStateQueue);
            if (compare != 0) {
                return compare;
            }
        }
    }
    throw CipOrderingRunTimeException(
            L"Failed to assign CIP stereochemistry, this indicates a bug in OPSIN or a limitation in OPSIN's implementation of the sequence rules");
}

int CipSequenceRules::SortByCipOrder::compareAtNextLevel(CipState *cipState, std::list<CipState *> &queue) {
    std::vector<std::vector<AtomWithHistory *>> neighbours1 = getNextLevelNeighbours(cipState->nextAtoms1);
    std::vector<std::vector<AtomWithHistory *>> neighbours2 = getNextLevelNeighbours(cipState->nextAtoms2);

    int compare = compareNeighboursByCipPriorityRules(neighbours1, neighbours2);

    if (compare != 0) {
        return compare;
    }
    std::vector<std::vector<AtomWithHistory *>> prioritisedNeighbours1 = formListsWithSamePriority(neighbours1);
    std::vector<std::vector<AtomWithHistory *>> prioritisedNeighbours2 = formListsWithSamePriority(neighbours2);

    //As earlier compare was 0, prioritisedNeighbours1.size() == prioritisedNeighbours2.size()
    for (int i = prioritisedNeighbours1.size() - 1; i >= 0; i--) {
        CipState tempVar(prioritisedNeighbours1[i], prioritisedNeighbours2[i]);
        queue.push_back(&tempVar);
    }
    return 0;
}

int CipSequenceRules::SortByCipOrder::compareNeighboursByCipPriorityRules(
        std::vector<std::vector<AtomWithHistory *>> &neighbours1,
        std::vector<std::vector<AtomWithHistory *>> &neighbours2) {
    int difference = listOfAtomListsCipComparator->compare(neighbours1, neighbours2);
    if (difference > 0) {
        return 1;
    }
    if (difference < 0) {
        return -1;
    }
    return 0;
}

std::vector<std::vector<AtomWithHistory *>>
CipSequenceRules::SortByCipOrder::getNextLevelNeighbours(std::vector<AtomWithHistory *> &nextAtoms) {
    std::vector<std::vector<AtomWithHistory *>> neighbourLists;
    for (auto nextAtom : nextAtoms) {
        neighbourLists.push_back(getNextAtomsWithAppropriateGhostAtoms(nextAtom));
    }
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(neighbourLists, atomListCipComparator);
    std::sort(neighbourLists.begin(), neighbourLists.end(), atomListCipComparator);
    return neighbourLists;
}

std::vector<std::vector<AtomWithHistory *>> CipSequenceRules::SortByCipOrder::formListsWithSamePriority(
        std::vector<std::vector<AtomWithHistory *>> &neighbourLists) {
    int intialNeighbourListCount = neighbourLists.size();
    if (intialNeighbourListCount > 1) {
        std::vector<std::vector<AtomWithHistory *>> listsToRemove;
        for (int i = 0; i < intialNeighbourListCount; i++) {
            std::vector<std::vector<AtomWithHistory *>> neighbourListsToCombine;
            std::vector<AtomWithHistory *> primaryAtomList = neighbourLists[i];
            for (int j = i + 1; j < intialNeighbourListCount; j++) {
                std::vector<AtomWithHistory *> neighbourListToCompareWith = neighbourLists[j];
                if (atomListCipComparator->compare(primaryAtomList, neighbourListToCompareWith) == 0) {
                    neighbourListsToCombine.push_back(neighbourListToCompareWith);
                    i++;
                } else {
                    break;
                }
            }
            for (auto neighbourList : neighbourListsToCombine) {
                listsToRemove.push_back(neighbourList);
                primaryAtomList.insert(primaryAtomList.end(), neighbourList.begin(), neighbourList.end());
            }
        }
        neighbourLists.removeAll(listsToRemove);
    }

    std::vector<std::vector<AtomWithHistory *>> updatedNeighbourLists;
    //lists of same priority have already been combined (see above) e.g. [H,C,C] [H,C,C] -->[H,C,C,H,C,C]
    //now sort these combined lists by CIP priority
    //then group atoms that have the same CIP priority
    for (int i = 0, lstsLen = neighbourLists.size(); i < lstsLen; i++) {
        std::vector<AtomWithHistory *> neighbourList = neighbourLists[i];
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(neighbourList, cipComparator);
        std::sort(neighbourList.begin(), neighbourList.end(), cipComparator);
        AtomWithHistory *lastAtom = nullptr;
        std::vector<AtomWithHistory *> currentAtomList;
        for (int j = 0, lstLen = neighbourList.size(); j < lstLen; j++) {
            AtomWithHistory *a = neighbourList[j];
            if (lastAtom != nullptr && compareByCipRules(lastAtom, a) != 0) {
                updatedNeighbourLists.push_back(currentAtomList);
                currentAtomList = std::vector<AtomWithHistory *>();
            }
            currentAtomList.push_back(a);
            lastAtom = a;
        }
        if (!currentAtomList.empty()) {
            updatedNeighbourLists.push_back(currentAtomList);
        }
    }
    return updatedNeighbourLists;
}

CipSequenceRules::SortByCipOrder::CipComparator::CipComparator(CipSequenceRules::SortByCipOrder *outerInstance)
        : outerInstance(outerInstance) {
}

int CipSequenceRules::SortByCipOrder::CipComparator::compare(AtomWithHistory *a, AtomWithHistory *b) {
    return outerInstance->compareByCipRules(a, b);
}

CipSequenceRules::SortByCipOrder::AtomListCipComparator::AtomListCipComparator(
        CipSequenceRules::SortByCipOrder *outerInstance) : outerInstance(outerInstance) {
}

int CipSequenceRules::SortByCipOrder::AtomListCipComparator::compare(std::vector<AtomWithHistory *> &a,
                                                                     std::vector<AtomWithHistory *> &b) {
    int aSize = a.size();
    int bSize = b.size();
    int differenceInSize = aSize - bSize;
    int maxCommonSize = aSize > bSize ? bSize : aSize;
    for (int i = 1; i <= maxCommonSize; i++) {
        int difference = outerInstance->compareByCipRules(a[aSize - i], b[bSize - i]);
        if (difference > 0) {
            return 1;
        }
        if (difference < 0) {
            return -1;
        }
    }
    if (differenceInSize > 0) {
        return 1;
    }
    if (differenceInSize < 0) {
        return -1;
    }
    return 0;
}

CipSequenceRules::SortByCipOrder::ListOfAtomListsCipComparator::ListOfAtomListsCipComparator(
        CipSequenceRules::SortByCipOrder *outerInstance) : outerInstance(outerInstance) {
}

int
CipSequenceRules::SortByCipOrder::ListOfAtomListsCipComparator::compare(std::vector<std::vector<AtomWithHistory *>> &a,
                                                                        std::vector<std::vector<AtomWithHistory *>> &b) {
    int aSize = a.size();
    int bSize = b.size();
    int differenceInSize = aSize - bSize;
    int maxCommonSize = aSize > bSize ? bSize : aSize;
    for (int i = 1; i <= maxCommonSize; i++) {
        std::vector<AtomWithHistory *> aprime = a[aSize - i];
        std::vector<AtomWithHistory *> bprime = b[bSize - i];
        int aprimeSize = aprime.size();
        int bprimeSize = bprime.size();
        int differenceInSizeprime = aprimeSize - bprimeSize;
        int maxCommonSizeprime = aprimeSize > bprimeSize ? bprimeSize : aprimeSize;
        for (int j = 1; j <= maxCommonSizeprime; j++) {
            int difference = outerInstance->compareByCipRules(aprime[aprimeSize - j], bprime[bprimeSize - j]);
            if (difference > 0) {
                return 1;
            }
            if (difference < 0) {
                return -1;
            }
        }
        if (differenceInSizeprime > 0) {
            return 1;
        }
        if (differenceInSizeprime < 0) {
            return -1;
        }
    }
    if (differenceInSize > 0) {
        return 1;
    }
    if (differenceInSize < 0) {
        return -1;
    }
    return 0;
}

std::vector<AtomWithHistory *>
CipSequenceRules::SortByCipOrder::getNextAtomsWithAppropriateGhostAtoms(AtomWithHistory *atomWithHistory) {
    Atom *atom = atomWithHistory->atom;
    std::vector<Atom *> &visitedAtoms = atomWithHistory->visitedAtoms;
    Atom *previousAtom = visitedAtoms[visitedAtoms.size() - 1];
    std::vector<Atom *> visitedAtomsIncludingCurrentAtom(visitedAtoms);
    visitedAtomsIncludingCurrentAtom.push_back(atom);

    std::vector<AtomWithHistory *> neighboursWithHistory;
    for (auto b : atom->getBonds()) {
        Atom *atomBondConnectsTo = b->getOtherAtom(atom);
        if (!atomBondConnectsTo->equals(
                chiralAtom)) { //P-91.1.4.2.4 (higher order bonds to chiral centre do not involve duplication of atoms)
            for (int j = b->getOrder(); j > 1; j--) { //add ghost atoms to represent higher order bonds
                Atom *ghost = new Atom(atomBondConnectsTo->getElement());
                if (rule > 0) {
                    int indexOfOriginalAtom = visitedAtoms.find(atomBondConnectsTo);
                    if (indexOfOriginalAtom != -1) {
                        AtomWithHistory tempVar(ghost, visitedAtomsIncludingCurrentAtom, indexOfOriginalAtom);
                        neighboursWithHistory.push_back(&tempVar);
                    } else {
                        AtomWithHistory tempVar2(ghost, visitedAtomsIncludingCurrentAtom, visitedAtoms.size() + 1);
                        neighboursWithHistory.push_back(&tempVar2);
                    }
                } else {
                    AtomWithHistory tempVar3(ghost, visitedAtomsIncludingCurrentAtom, nullptr);
                    neighboursWithHistory.push_back(&tempVar3);
                }
            }
        }
        if (!atomBondConnectsTo->equals(previousAtom)) {
            if (std::find(visitedAtoms.begin(), visitedAtoms.end(), atomBondConnectsTo) !=
                visitedAtoms.end()) { //cycle detected, add ghost atom instead
                Atom *ghost = new Atom(atomBondConnectsTo->getElement());
                if (rule > 0) {
                    AtomWithHistory tempVar4(ghost, visitedAtomsIncludingCurrentAtom,
                                             visitedAtoms.find(atomBondConnectsTo));
                    neighboursWithHistory.push_back(&tempVar4);
                } else {
                    AtomWithHistory tempVar5(ghost, visitedAtomsIncludingCurrentAtom, nullptr);
                    neighboursWithHistory.push_back(&tempVar5);
                }
            } else {
                AtomWithHistory tempVar6(atomBondConnectsTo, visitedAtomsIncludingCurrentAtom, nullptr);
                neighboursWithHistory.push_back(&tempVar6);
            }
        }
    }
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(neighboursWithHistory, cipComparator);
    std::sort(neighboursWithHistory.begin(), neighboursWithHistory.end(), cipComparator);
    return neighboursWithHistory;
}

int CipSequenceRules::SortByCipOrder::compareByCipRules(AtomWithHistory *a, AtomWithHistory *b) {
    //rule 1a
    //prefer higher atomic number
    int atomicNumber1 = a->atom->getElement()
    ::ATOMIC_NUM;
    int atomicNumber2 = b->atom->getElement()
    ::ATOMIC_NUM;
    if (atomicNumber1 > atomicNumber2) {
        return 1;
    } else if (atomicNumber1 < atomicNumber2) {
        return -1;
    }
    if (rule > 0) {
        //rule 1b
        //prefer duplicate to non-duplicate
        boost::optional<int> indexFromRoot1 = a->indexOfOriginalFromRoot;
        boost::optional<int> indexFromRoot2 = b->indexOfOriginalFromRoot;
        if (indexFromRoot1 && !indexFromRoot2) {
            return 1;
        }
        if (!indexFromRoot1 && indexFromRoot2) {
            return -1;
        }
        //prefer duplicate of node closer to root
        if (indexFromRoot1 && indexFromRoot2) {
            if (indexFromRoot1 < indexFromRoot2) {
                return 1;
            }
            if (indexFromRoot1 > indexFromRoot2) {
                return -1;
            }
        }
        if (rule > 1) {
            //rule 2
            //prefer higher atomic mass
            boost::optional<int> atomicMass1 = a->atom->getIsotope();
            boost::optional<int> atomicMass2 = b->atom->getIsotope();
            if (atomicMass1 && !atomicMass2) {
                return 1;
            } else if (!atomicMass1 && atomicMass2) {
                return -1;
            } else if (atomicMass1 && atomicMass2) {
                if (atomicMass1 > atomicMass2) {
                    return 1;
                } else if (atomicMass1 < atomicMass2) {
                    return -1;
                }
            }
        }

    }
    return 0;
}
