#include "StereoAnalyser.h"
#include "Atom.h"
#include "Bond.h"
#include "CipOrderingException.h"
#include "CipSequenceRules.h"
#include "Fragment.h"
#include "ChemEl.h"


StereoAnalyser::StereoCentre::StereoCentre(StereoAnalyser *outerInstance, Atom *stereoAtom,
                                           boost::optional<bool> isTrueStereoCentre) : stereoAtom(stereoAtom),
                                                                                       trueStereoCentre(
                                                                                               isTrueStereoCentre),
                                                                                       outerInstance(outerInstance) {
}

Atom *StereoAnalyser::StereoCentre::getStereoAtom() {
    return stereoAtom;
}

bool StereoAnalyser::StereoCentre::isTrueStereoCentre() {
    return trueStereoCentre;
}

std::vector<Atom *> StereoAnalyser::StereoCentre::getCipOrderedAtoms() throw(CipOrderingException) {
    CipSequenceRules tempVar(stereoAtom);
    std::vector<Atom *> cipOrderedAtoms = (&tempVar)->getNeighbouringAtomsInCipOrder();
    if (cipOrderedAtoms.size() ==
        3) { //lone pair is the 4th. This is represented by the atom itself and is always the lowest priority
        cipOrderedAtoms.push_back(0, stereoAtom);
    }
    return cipOrderedAtoms;
}

StereoAnalyser::StereoBond::StereoBond(StereoAnalyser *outerInstance, Bond *bond) : bond(bond),
                                                                                    outerInstance(outerInstance) {
}

Bond *StereoAnalyser::StereoBond::getBond() {
    return bond;
}

std::vector<Atom *> StereoAnalyser::StereoBond::getOrderedStereoAtoms() throw(CipOrderingException) {
    Atom *a1 = bond->getFromAtom();
    Atom *a2 = bond->getToAtom();
    CipSequenceRules tempVar(a1);
    std::vector<Atom *> cipOrdered1 = (&tempVar)->getNeighbouringAtomsInCipOrderIgnoringGivenNeighbour(a2);
    CipSequenceRules tempVar2(a2);
    std::vector<Atom *> cipOrdered2 = (&tempVar2)->getNeighbouringAtomsInCipOrderIgnoringGivenNeighbour(a1);
    std::vector<Atom *> stereoAtoms;
    stereoAtoms.push_back(cipOrdered1[cipOrdered1.size() - 1]); //highest CIP adjacent to a1
    stereoAtoms.push_back(a1);
    stereoAtoms.push_back(a2);
    stereoAtoms.push_back(cipOrdered2[cipOrdered2.size() - 1]); //highest CIP adjacent to a2
    return stereoAtoms;
}

int StereoAnalyser::AtomicNumberThenAtomicMassComparator::compare(Atom *a, Atom *b) {
    return compareAtomicNumberThenAtomicMass(a, b);
}

int StereoAnalyser::compareAtomicNumberThenAtomicMass(Atom *a, Atom *b) {
    int atomicNumber1 = a->getElement()
    ::ATOMIC_NUM;
    int atomicNumber2 = b->getElement()
    ::ATOMIC_NUM;
    if (atomicNumber1 > atomicNumber2) {
        return 1;
    } else if (atomicNumber1 < atomicNumber2) {
        return -1;
    }
    boost::optional<int> atomicMass1 = a->getIsotope();
    boost::optional<int> atomicMass2 = b->getIsotope();
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
    return 0;
}

StereoAnalyser::AtomNeighbouringColoursComparator::AtomNeighbouringColoursComparator(StereoAnalyser *outerInstance)
        : outerInstance(outerInstance) {
}

int StereoAnalyser::AtomNeighbouringColoursComparator::compare(Atom *a, Atom *b) {
    std::vector<int> colours1 = outerInstance->atomNeighbourColours[a];
    std::vector<int> colours2 = outerInstance->atomNeighbourColours[b];

    int colours1Size = colours1.size();
    int colours2Size = colours2.size();

    int maxCommonColourSize = std::min(colours1Size, colours2Size);
    for (int i = 1; i <= maxCommonColourSize; i++) {
        int difference = colours1[colours1Size - i] - colours2[colours2Size - i];
        if (difference > 0) {
            return 1;
        }
        if (difference < 0) {
            return -1;
        }
    }
    int differenceInSize = colours1Size - colours2Size;
    if (differenceInSize > 0) {
        return 1;
    }
    if (differenceInSize < 0) {
        return -1;
    }
    return 0;
}

StereoAnalyser::StereoAnalyser(Fragment *molecule) : StereoAnalyser(molecule->getAtomList(), molecule->getBondSet()) {
}

StereoAnalyser::StereoAnalyser(Collection < Atom * > *atoms, Collection < Bond * > *bonds) : atoms(atoms), bonds(bonds),
                                                                                             mappingToColour(
                                                                                                     std::unordered_map<Atom *, int>(
                                                                                                             atomsToSort->size())),
                                                                                             atomNeighbourColours(
                                                                                                     std::unordered_map<Atom *, std::vector<int>>(
                                                                                                             atomsToSort->size())) {
    std::vector<Atom *> ghostAtoms = addGhostAtoms();
    std::vector<Atom *> atomsToSort(atoms);
    atomsToSort.insert(atomsToSort.end(), ghostAtoms.begin(), ghostAtoms.end());
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(atomsToSort, atomicNumberThenAtomicMassComparator);
    std::sort(atomsToSort.begin(), atomsToSort.end(), atomicNumberThenAtomicMassComparator);
    std::vector<std::vector<Atom *>> groupsByColour = populateColoursByAtomicNumberAndMass(atomsToSort);
    bool changeFound = true;
    while (changeFound) {
        for (auto groupWithAColour : groupsByColour) {
            for (auto atom : groupWithAColour) {
                std::vector<int> neighbourColours = findColourOfNeighbours(atom);
                atomNeighbourColours[atom] = neighbourColours;
            }
        }
        std::vector<std::vector<Atom *>> updatedGroupsByColour;
        changeFound = populateColoursAndReportIfColoursWereChanged(groupsByColour, updatedGroupsByColour);
        groupsByColour = updatedGroupsByColour;
    }
    removeGhostAtoms(ghostAtoms);
}

std::vector<Atom *> StereoAnalyser::addGhostAtoms() {
    std::vector<Atom *> ghostAtoms;
    for (auto bond : bonds) {
        int bondOrder = bond->getOrder();
        for (int i = bondOrder; i > 1; i--) {
            Atom *fromAtom = bond->getFromAtom();
            Atom *toAtom = bond->getToAtom();

            Atom *ghost1 = new Atom(fromAtom->getElement());
            Bond *b1 = new Bond(ghost1, toAtom, 1);
            toAtom->addBond(b1);
            ghost1->addBond(b1);
            ghostAtoms.push_back(ghost1);

            Atom *ghost2 = new Atom(toAtom->getElement());
            Bond *b2 = new Bond(ghost2, fromAtom, 1);
            fromAtom->addBond(b2);
            ghost2->addBond(b2);
            ghostAtoms.push_back(ghost2);
        }
    }
    return ghostAtoms;
}

void StereoAnalyser::removeGhostAtoms(std::vector<Atom *> &ghostAtoms) {
    for (auto atom : ghostAtoms) {
        Bond *b = atom->getFirstBond();
        b->getOtherAtom(atom)->removeBond(b);
    }
}

std::vector<std::vector<Atom *>> StereoAnalyser::populateColoursByAtomicNumberAndMass(std::vector<Atom *> &atomList) {
    std::vector<std::vector<Atom *>> groupsByColour;
    Atom *previousAtom = nullptr;
    std::vector<Atom *> atomsOfThisColour;
    int atomsSeen = 0;
    for (auto atom : atomList) {
        if (previousAtom != nullptr && compareAtomicNumberThenAtomicMass(previousAtom, atom) != 0) {
            for (auto atomOfthisColour : atomsOfThisColour) {
                mappingToColour[atomOfthisColour] = atomsSeen;
            }
            groupsByColour.push_back(atomsOfThisColour);
            atomsOfThisColour = std::vector<Atom *>();
        }
        previousAtom = atom;
        atomsOfThisColour.push_back(atom);
        atomsSeen++;
    }
    if (!atomsOfThisColour.empty()) {
        for (auto atomOfThisColour : atomsOfThisColour) {
            mappingToColour[atomOfThisColour] = atomsSeen;
        }
        groupsByColour.push_back(atomsOfThisColour);
    }
    return groupsByColour;
}

bool StereoAnalyser::populateColoursAndReportIfColoursWereChanged(std::vector<std::vector<Atom *>> &groupsByColour,
                                                                  std::vector<std::vector<Atom *>> &updatedGroupsByColour) {
    bool changeFound = false;
    int atomsSeen = 0;
    for (auto groupWithAColour : groupsByColour) {
        Collections::sort(groupWithAColour, atomNeighbouringColoursComparator);
        Atom *previousAtom = nullptr;
        std::vector<Atom *> atomsOfThisColour;
        for (auto atom : groupWithAColour) {
            if (previousAtom != nullptr && atomNeighbouringColoursComparator->compare(previousAtom, atom) != 0) {
                for (auto atomOfThisColour : atomsOfThisColour) {
                    if (!changeFound && atomsSeen != mappingToColour[atomOfThisColour]) {
                        changeFound = true;
                    }
                    mappingToColour[atomOfThisColour] = atomsSeen;
                }
                updatedGroupsByColour.push_back(atomsOfThisColour);
                atomsOfThisColour = std::vector<Atom *>();
            }
            previousAtom = atom;
            atomsOfThisColour.push_back(atom);
            atomsSeen++;
        }
        if (!atomsOfThisColour.empty()) {
            for (auto atomOfThisColour : atomsOfThisColour) {
                if (!changeFound && atomsSeen != mappingToColour[atomOfThisColour]) {
                    changeFound = true;
                }
                mappingToColour[atomOfThisColour] = atomsSeen;
            }
            updatedGroupsByColour.push_back(atomsOfThisColour);
        }
    }
    return changeFound;
}

std::vector<int> StereoAnalyser::findColourOfNeighbours(Atom *atom) {
    std::vector<Bond *> bonds = atom->getBonds();
    int bondCount = bonds.size();
    std::vector<int> colourOfAdjacentAtoms(bondCount);
    for (int i = 0; i < bondCount; i++) {
        Bond *bond = bonds[i];
        Atom *otherAtom = bond->getOtherAtom(atom);
        colourOfAdjacentAtoms[i] = mappingToColour[otherAtom];
    }
    Arrays::sort(colourOfAdjacentAtoms); //sort such that this goes from low to high
    return colourOfAdjacentAtoms;
}

std::vector<StereoCentre *> StereoAnalyser::findStereoCentres() {
    std::vector<Atom *> potentialStereoAtoms = getPotentialStereoCentres();
    std::vector<Atom *> trueStereoCentres;
    for (auto potentialStereoAtom : potentialStereoAtoms) {
        if (isTrueStereCentre(potentialStereoAtom)) {
            trueStereoCentres.push_back(potentialStereoAtom);
        }
    }
    std::vector<StereoCentre *> stereoCentres;
    for (auto trueStereoCentreAtom : trueStereoCentres) {
        StereoCentre tempVar(this, trueStereoCentreAtom, true);
        stereoCentres.push_back(&tempVar);
    }

    potentialStereoAtoms.removeAll(trueStereoCentres);
    std::vector<Atom *> paraStereoCentres = findParaStereoCentres(potentialStereoAtoms, trueStereoCentres);
    for (auto paraStereoCentreAtom : paraStereoCentres) {
        StereoCentre tempVar2(this, paraStereoCentreAtom, false);
        stereoCentres.push_back(&tempVar2);
    }
    return stereoCentres;
}

std::vector<Atom *> StereoAnalyser::getPotentialStereoCentres() {
    std::vector<Atom *> potentialStereoAtoms;
    for (auto atom : atoms) {
        if (isPossiblyStereogenic(atom)) {
            potentialStereoAtoms.push_back(atom);
        }
    }
    return potentialStereoAtoms;
}

bool StereoAnalyser::isTrueStereCentre(Atom *potentialStereoAtom) {
    std::vector<Atom *> neighbours = potentialStereoAtom->getAtomNeighbours();
    if (neighbours.size() != 3 && neighbours.size() != 4) {
        return false;
    }
    std::vector<int> colours(4);
    for (int i = neighbours.size() - 1; i >= 0; i--) {
        colours[i] = mappingToColour[neighbours[i]];
    }

    bool foundIdenticalNeighbour = false;
    for (int i = 0; i < 4; i++) {
        int cl = colours[i];
        for (int j = i + 1; j < 4; j++) {
            if (cl == colours[j]) {
                foundIdenticalNeighbour = true;
                break;
            }
        }
    }
    return !foundIdenticalNeighbour;
}

std::vector<Atom *> StereoAnalyser::findParaStereoCentres(std::vector<Atom *> &potentialStereoAtoms,
                                                          std::vector<Atom *> &trueStereoCentres) {
    std::vector<Atom *> paraStereoCentres;
    for (auto potentialStereoAtom : potentialStereoAtoms) {
        std::vector<Atom *> neighbours = potentialStereoAtom->getAtomNeighbours();
        if (neighbours.size() == 4) {
            std::vector<int> colours(4);
            for (int i = neighbours.size() - 1; i >= 0; i--) {
                colours[i] = mappingToColour[neighbours[i]];
            }
            //find pairs of constitutionally identical substituents
            std::unordered_map<int, int> foundPairs;
            for (int i = 0; i < 4; i++) {
                int cl = colours[i];
                for (int j = i + 1; j < 4; j++) {
                    if (cl == colours[j]) {
                        foundPairs[i] = j;
                        break;
                    }
                }
            }
            int pairs = foundPairs.keySet()->size();
            if (pairs == 1 || pairs == 2) {
                bool hasTrueStereoCentreInAllBranches = true;
                for (auto entry : foundPairs) {
                    if (!branchesHaveTrueStereocentre(neighbours[entry.first], neighbours[entry.second],
                                                      potentialStereoAtom, trueStereoCentres)) {
                        hasTrueStereoCentreInAllBranches = false;
                        break;
                    }
                }
                if (hasTrueStereoCentreInAllBranches) {
                    paraStereoCentres.push_back(potentialStereoAtom);
                }
            }
        }
    }
    return paraStereoCentres;
}

bool StereoAnalyser::branchesHaveTrueStereocentre(Atom *branchAtom1, Atom *branchAtom2, Atom *potentialStereoAtom,
                                                  std::vector<Atom *> &trueStereoCentres) {
    std::vector<Atom *> atomsToVisit;
    Set < Atom * > *visitedAtoms = std::unordered_set<Atom *>();
    visitedAtoms->add(potentialStereoAtom);
    atomsToVisit.push_back(branchAtom1);
    atomsToVisit.push_back(branchAtom2);
    while (!atomsToVisit.empty()) {
        std::vector<Atom *> newAtomsToVisit;
        while (!atomsToVisit.empty()) {
            Atom *atom = atomsToVisit.erase(atomsToVisit.begin());
            if (std::find(trueStereoCentres.begin(), trueStereoCentres.end(), atom) != trueStereoCentres.end()) {
                return true;
            }
            if (std::find(atomsToVisit.begin(), atomsToVisit.end(), atom) !=
                atomsToVisit.end()) { //the two branches have converged on this atom, don't investigate neighbours of it
                do {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                    atomsToVisit.remove(atom);
                } while (std::find(atomsToVisit.begin(), atomsToVisit.end(), atom) != atomsToVisit.end());
                continue;
            } else {
                std::vector<Atom *> neighbours = atom->getAtomNeighbours();
                for (auto neighbour : neighbours) {
                    if (visitedAtoms->contains(neighbour)) {
                        continue;
                    }
                    newAtomsToVisit.push_back(neighbour);
                }
            }
            visitedAtoms->add(atom);
        }
        atomsToVisit = newAtomsToVisit;
    }
    return false;
}

bool StereoAnalyser::isPossiblyStereogenic(Atom *atom) {
    return isKnownPotentiallyStereogenic(atom) && !isAchiralDueToResonanceOrTautomerism(atom);
}

bool StereoAnalyser::isKnownPotentiallyStereogenic(Atom *atom) {
    std::vector<Atom *> neighbours = atom->getAtomNeighbours();
    ChemEl chemEl = atom->getElement();
    if (neighbours.size() == 4) {
        if (chemEl == ChemEl::B || chemEl == ChemEl::C || chemEl == ChemEl::Si || chemEl == ChemEl::Ge ||
            chemEl == ChemEl::Sn || chemEl == ChemEl::N || chemEl == ChemEl::P || chemEl == ChemEl::As ||
            chemEl == ChemEl::S || chemEl == ChemEl::Se) {
            return true;
        }
    } else if (neighbours.size() == 3) {
        if ((chemEl == ChemEl::S || chemEl == ChemEl::Se) &&
            (atom->getIncomingValency() == 4 || (atom->getCharge() == 1 && atom->getIncomingValency() == 3))) {
            //tetrahedral sulfur/selenium - 3 bonds and the lone pair
            return true;
        }
        if (chemEl == ChemEl::N && atom->getCharge() == 0 && atom->getIncomingValency() == 3 &&
            atomsContainABondBetweenThemselves(neighbours)) {
            return true;
            //nitrogen where two attached atoms are connected together
        }
    }
    return false;
}

bool StereoAnalyser::atomsContainABondBetweenThemselves(std::vector<Atom *> &atoms) {
    for (auto atom : atoms) {
        for (auto neighbour : atom->getAtomNeighbours()) {
            if (std::find(atoms.begin(), atoms.end(), neighbour) != atoms.end()) {
                return true;
            }
        }
    }
    return false;
}

bool StereoAnalyser::isAchiralDueToResonanceOrTautomerism(Atom *atom) {
    ChemEl chemEl = atom->getElement();
    if (chemEl == ChemEl::N || chemEl == ChemEl::P || chemEl == ChemEl::As || chemEl == ChemEl::S ||
        chemEl == ChemEl::Se) {
        std::vector<Atom *> neighbours = atom->getAtomNeighbours();
        Set <std::wstring> *resonanceAndTautomerismAtomicElementPlusIsotopes = std::unordered_set<std::wstring>();
        for (auto neighbour : neighbours) {
            ChemEl neighbourChemEl = neighbour->getElement();
            if ((neighbourChemEl::isChalcogen() || neighbourChemEl == ChemEl::N) &&
                isOnlyBondedToHydrogensOtherThanGivenAtom(neighbour, atom)) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                if (resonanceAndTautomerismAtomicElementPlusIsotopes->contains(
                        neighbourChemEl::toString() + atom->getIsotope())) {
                    return true;
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                resonanceAndTautomerismAtomicElementPlusIsotopes->add(neighbourChemEl::toString() + atom->getIsotope());
            }
            if (neighbourChemEl == ChemEl::H && neighbour->getBondCount() == 1) {
                //terminal H atom neighbour
                return true;
            }
        }
    }
    return false;
}

bool StereoAnalyser::isOnlyBondedToHydrogensOtherThanGivenAtom(Atom *atom, Atom *attachedNonHydrogen) {
    for (auto neighbour : atom->getAtomNeighbours()) {
        if (neighbour->equals(attachedNonHydrogen)) {
            continue;
        }
        if (neighbour->getElement() != ChemEl::H) {
            return false;
        }
    }
    return true;
}

std::vector<StereoBond *> StereoAnalyser::findStereoBonds() {
    std::vector<StereoBond *> stereoBonds;
    for (auto bond : bonds) {
        if (bond->getOrder() == 2) {
            Atom *a1 = bond->getFromAtom();
            std::vector<Atom *> neighbours1 = a1->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            neighbours1.remove(bond->getToAtom());
            if (neighbours1.size() == 2 ||
                (neighbours1.size() == 1 && a1->getElement() == ChemEl::N && a1->getIncomingValency() == 3 &&
                 a1->getCharge() == 0)) {
                if (neighbours1.size() == 2 &&
                    mappingToColour[neighbours1[0]].equals(mappingToColour[neighbours1[1]])) {
                    continue;
                }
                Atom *a2 = bond->getToAtom();
                std::vector<Atom *> neighbours2 = a2->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                neighbours2.remove(bond->getFromAtom());
                if (neighbours2.size() == 2 ||
                    (neighbours2.size() == 1 && a2->getElement() == ChemEl::N && a2->getIncomingValency() == 3 &&
                     a2->getCharge() == 0)) {
                    if (neighbours2.size() == 2 &&
                        mappingToColour[neighbours2[0]].equals(mappingToColour[neighbours2[1]])) {
                        continue;
                    }
                    StereoBond tempVar(this, bond);
                    stereoBonds.push_back(&tempVar);
                }
            }
        }
    }
    return stereoBonds;
}

boost::optional<int> StereoAnalyser::getAtomEnvironmentNumber(Atom *a) {
    return mappingToColour[a];
}
