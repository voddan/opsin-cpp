#include <RectangularVectors.h>
#include "FusedRingNumberer.h"
#include "Ring.h"
#include "Atom.h"
#include "Fragment.h"
#include "StructureBuildingException.h"
#include "SSSRFinder.h"
#include "FragmentTools.h"
#include "Bond.h"

//#include "RectangularVectors.h"


#include "RectangularVectors.h"


using org::apache::log4j::Logger;

RingConnectivityTable *FusedRingNumberer::RingConnectivityTable::copy() {
    RingConnectivityTable *copy = new RingConnectivityTable();
    copy->ringShapes.insert(copy->ringShapes.end(), ringShapes.begin(), ringShapes.end());
    copy->neighbouringRings.insert(copy->neighbouringRings.end(), neighbouringRings.begin(), neighbouringRings.end());
    copy->directionFromRingToNeighbouringRing.insert(copy->directionFromRingToNeighbouringRing.end(),
                                                     directionFromRingToNeighbouringRing.begin(),
                                                     directionFromRingToNeighbouringRing.end());
    copy->usedRings.insert(copy->usedRings.end(), usedRings.begin(), usedRings.end());
    return copy;
}

FusedRingNumberer::RingShape::RingShape(Ring *ring, FusionRingShape shape) : ring(ring), shape(shape) {
}

Ring *FusedRingNumberer::RingShape::getRing() {
    return ring;
}

FusionRingShape FusedRingNumberer::RingShape::getShape() {
    return shape;
}

FusedRingNumberer::Chain::Chain(int length, int startingX, int y) : length(length), startingX(startingX), y(y) {
}

int FusedRingNumberer::Chain::getLength() {
    return length;
}

int FusedRingNumberer::Chain::getStartingX() {
    return startingX;
}

int FusedRingNumberer::Chain::getY() {
    return y;
}

int FusedRingNumberer::SortAtomSequences::compare(std::vector<Atom *> *sequenceA, std::vector<Atom *> *sequenceB) {
    if (sequenceA.size() != sequenceB.size()) {
        //Error in fused ring building. Identified ring sequences not the same lengths!
        return 0;
    }

    int i = 0;
    int j = 0;
    //Give low numbers for the heteroatoms as a set.
    while (i < sequenceA.size()) {
        Atom *atomA = sequenceA[i];
        bool isAaHeteroatom = atomA->getElement() != ChemEl::C;


        //bridgehead carbon do not increment numbering
        if (!isAaHeteroatom && atomA->getBondCount() >= 3) {
            i++;
            continue;
        }

        Atom *atomB = sequenceB[j];
        bool isBaHeteroatom = atomB->getElement() != ChemEl::C;
        if (!isBaHeteroatom && atomB->getBondCount() >= 3) {
            j++;
            continue;
        }

        if (isAaHeteroatom && !isBaHeteroatom) {
            return -1;
        }
        if (isBaHeteroatom && !isAaHeteroatom) {
            return 1;
        }
        i++;
        j++;
    }

    i = 0;
    j = 0;
    //Give low numbers for heteroatoms when considered in the order: O, S, Se, Te, N, P, As, Sb, Bi, Si, Ge, Sn, Pb, B, Hg
    while (i < sequenceA.size()) {
        Atom *atomA = sequenceA[i];

        //bridgehead carbon do not increment numbering
        if (atomA->getElement() == ChemEl::C && atomA->getBondCount() >= 3) {
            i++;
            continue;
        }

        Atom *atomB = sequenceB[j];
        if (atomB->getElement() == ChemEl::C && atomB->getBondCount() >= 3) {
            j++;
            continue;
        }

        boost::optional<int> heteroAtomPriorityA = heteroAtomValues[atomA->getElement()];
        int atomAElementValue = heteroAtomPriorityA ? heteroAtomPriorityA : 0;

        boost::optional<int> heteroAtomPriorityB = heteroAtomValues[atomB->getElement()];
        int atomBElementValue = heteroAtomPriorityB ? heteroAtomPriorityB : 0;

        if (atomAElementValue > atomBElementValue) {
            return -1;
        }
        if (atomAElementValue < atomBElementValue) {
            return 1;
        }
        i++;
        j++;
    }

    //Give low numbers to fusion carbon atoms.
    for (i = 0; i < sequenceA.size(); i++) {
        Atom *atomA = sequenceA[i];
        Atom *atomB = sequenceB[i];
        if (atomA->getBondCount() >= 3 && atomA->getElement() == ChemEl::C) {
            if (!(atomB->getBondCount() >= 3 && atomB->getElement() == ChemEl::C)) {
                return -1;
            }
        }
        if (atomB->getBondCount() >= 3 && atomB->getElement() == ChemEl::C) {
            if (!(atomA->getBondCount() >= 3 && atomA->getElement() == ChemEl::C)) {
                return 1;
            }
        }
    }
    //Note that any sequences still unsorted at this step will have fusion carbon atoms in the same places
    //which means you can go through both sequences without constantly looking for fusion carbons i.e. the variable j is no longer needed

    //Give low numbers to fusion rather than non-fusion atoms of the same heteroelement.
    for (i = 0; i < sequenceA.size(); i++) {
        Atom *atomA = sequenceA[i];
        Atom *atomB = sequenceB[i];
        if (atomA->getBondCount() >= 3) {
            if (!(atomB->getBondCount() >= 3)) {
                return -1;
            }
        }
        if (atomB->getBondCount() >= 3) {
            if (!(atomA->getBondCount() >= 3)) {
                return 1;
            }
        }
    }
    //TODO consider heteroatoms FR5.4d
    return 0;
}

const std::unordered_map<ChemEl, int> FusedRingNumberer::heteroAtomValues = new java::util::EnumMap<ChemEl, int>(
        ChemEl::
typeid);

FusedRingNumberer::StaticConstructor::StaticConstructor() {
    //unknown heteroatoms or carbon are given a value of 0
    heteroAtomValues[ChemEl::Hg] = 2;
    heteroAtomValues[ChemEl::Tl] = 3;
    heteroAtomValues[ChemEl::In] = 4;
    heteroAtomValues[ChemEl::Ga] = 5;
    heteroAtomValues[ChemEl::Al] = 6;
    heteroAtomValues[ChemEl::B] = 7;
    heteroAtomValues[ChemEl::Pb] = 8;
    heteroAtomValues[ChemEl::Sn] = 9;
    heteroAtomValues[ChemEl::Ge] = 10;
    heteroAtomValues[ChemEl::Si] = 11;
    heteroAtomValues[ChemEl::Bi] = 12;
    heteroAtomValues[ChemEl::Sb] = 13;
    heteroAtomValues[ChemEl::As] = 14;
    heteroAtomValues[ChemEl::P] = 15;
    heteroAtomValues[ChemEl::N] = 16;
    heteroAtomValues[ChemEl::Te] = 17;
    heteroAtomValues[ChemEl::Se] = 18;
    heteroAtomValues[ChemEl::S] = 19;
    heteroAtomValues[ChemEl::O] = 20;
    heteroAtomValues[ChemEl::I] = 21;
    heteroAtomValues[ChemEl::Br] = 22;
    heteroAtomValues[ChemEl::Cl] = 23;
    heteroAtomValues[ChemEl::F] = 24;
}

FusedRingNumberer::StaticConstructor FusedRingNumberer::staticConstructor;

void FusedRingNumberer::numberFusedRing(Fragment *fusedRing) throw(StructureBuildingException) {
    std::vector<Ring *> rings = SSSRFinder::getSetOfSmallestRings(fusedRing);
    if (rings.size() < 2) {
        throw StructureBuildingException(L"Ring perception system found less than 2 rings within input fragment!");
    }
    std::vector<Atom *> atomList = fusedRing->getAtomList();
    setupAdjacentFusedRingProperties(rings);
    if (!checkRingApplicability(rings)) {
        for (auto atom : atomList) {
            atom->clearLocants();
        }
        return;
    }
    std::vector<std::vector<Atom *>> atomSequences = determinePossiblePeripheryAtomOrders(rings, atomList.size());
    if (atomSequences.empty()) {
        for (auto atom : atomList) {
            atom->clearLocants();
        }
        return;
    }

    // add missing atoms to each path
    for (auto path : atomSequences) { //TODO properly support interior atom labelling
        for (auto atom : atomList) {
            if (!path.contains(atom)) {
                path.add(atom);
            }
        }
    }
    // find the preferred numbering scheme then relabel with this scheme
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(atomSequences, new SortAtomSequences());
    SortAtomSequences tempVar{};
    std::sort(atomSequences.begin(), atomSequences.end(), tempVar.cmp);
    FragmentTools::relabelLocantsAsFusedRingSystem(atomSequences[0]);
    fusedRing->reorderAtomCollection(atomSequences[0]);
}

void FusedRingNumberer::setupAdjacentFusedRingProperties(std::vector<Ring *> &rings) {
    for (int i = 0, l = rings.size(); i < l; i++) {
        Ring *curRing = rings[i];
        for (auto bond : curRing->getBondList()) { // go through all the bonds for the current ring
            for (int j = i + 1; j < l; j++) {
                Ring *otherRing = rings[j];
                if (std::find(otherRing->getBondList().begin(), otherRing->getBondList().end(), bond) !=
                    otherRing->getBondList().end()) { // check if this bond belongs to any other ring
                    otherRing->addNeighbour(bond, curRing);
                    curRing->addNeighbour(bond, otherRing); // if so, then associate the bond with the adjacent ring
                    goto bondLoopContinue;
                }
            }
            bondLoopContinue:
        }
        bondLoopBreak:
    }
}

bool FusedRingNumberer::checkRingApplicability(std::vector<Ring *> &rings) {
    for (auto ring : rings) {
        if (ring->size() <= 2) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Invalid ring size: " +ring.size());
            throw std::exception();
        }
        if (ring->size() > 8 && ring->getNumberOfFusedBonds() > 2) {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<Atom *>> FusedRingNumberer::determinePossiblePeripheryAtomOrders(std::vector<Ring *> &rings,
                                                                                         int atomCountOfFusedRingSystem) throw(StructureBuildingException) {
    std::vector<Ring *> tRings = findTerminalRings(rings);
    if (tRings.size() < 1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unable to find a terminal ring in fused ring system");
        throw std::exception();
    }
    Ring *tRing = tRings[0];
    Bond *b1 = getStartingNonFusedBond(tRing);
    if (b1 == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Non-fused bond from terminal ring not found");
        throw std::exception();
    }

    std::vector<RingConnectivityTable *> cts;
    RingConnectivityTable *startingCT = new RingConnectivityTable();
    cts.push_back(startingCT);
    buildRingConnectionTables(tRing, nullptr, 0, b1, b1->getFromAtom(), startingCT, cts);
    //The preference against fusion to elongated edges is built into the construction of the ring table

    /* FR 5.1.1/FR 5.1.2 Preferred shapes preferred to distorted shapes */
    removeCTsWithDistortedRingShapes(cts);
    //TODO better implement the corner cases of FR 5.1.3-5.1.5

    /* FR-5.2a. Maximum number of rings in a horizontal row */
    std::unordered_map<RingConnectivityTable *, std::vector<int>> horizonalRowDirections = findLongestChainDirections(
            cts);
    std::vector<std::vector<std::vector<Ring *>>> ringMaps = createRingMapsAlignedAlongGivenhorizonalRowDirections(
            horizonalRowDirections);
    /* FR-5.2b-d */
    return findPossiblePaths(ringMaps, atomCountOfFusedRingSystem);
}

std::vector<Ring *> FusedRingNumberer::findTerminalRings(std::vector<Ring *> &rings) {
    std::vector<Ring *> tRings;

    int minFusedBonds = std::numeric_limits<int>::max();
    for (auto ring : rings) {
        if (ring->getNumberOfFusedBonds() < minFusedBonds) {
            minFusedBonds = ring->getNumberOfFusedBonds();
        }
    }

    for (auto ring : rings) {
        if (ring->getNumberOfFusedBonds() == minFusedBonds) {
            tRings.push_back(ring);
        }
    }
    return tRings;
}

std::vector<RingConnectivityTable *>
FusedRingNumberer::buildRingConnectionTables(Ring *currentRing, Ring *previousRing, int previousDir, Bond *previousBond,
                                             Atom *atom, RingConnectivityTable *ct,
                                             std::vector<RingConnectivityTable *> &cts) {
    // order atoms and bonds in the ring
    currentRing->makeCyclicLists(previousBond, atom);
    std::vector<RingConnectivityTable *> generatedCts;
    std::vector<FusionRingShape> allowedShapes = getAllowedShapesForRing(currentRing, previousBond);
    if (allowedShapes.empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN limitation, unsupported ring size in fused ring numbering");
        throw std::exception();
    }
    ct->usedRings.push_back(currentRing);
    for (int i = allowedShapes.size() - 1; i >= 0; i--) {
        FusionRingShape fusionRingShape = allowedShapes[i];
        RingConnectivityTable *currentCT;
        if (i == 0) {
            currentCT = ct;
        } else {
            currentCT = ct->copy();
            cts.push_back(currentCT);
            generatedCts.push_back(currentCT);
        }
        RingShape *ringShape = new RingShape(currentRing, fusionRingShape);
        std::vector<RingConnectivityTable *> ctsToExpand;
        ctsToExpand.push_back(currentCT); //all the cts to consider, the currentCT and generated clones
        for (auto neighbourRing : currentRing->getNeighbours()) {
            //find the directions between the current ring and all neighbouring rings including the previous ring
            // this means that the direction to the previous ring will then be known in both directions

            // find direction
            Bond *currentBond = findFusionBond(currentRing, neighbourRing);

            int dir = 0;
            if (neighbourRing == previousRing) {
                dir = getOppositeDirection(previousDir);
            } else {
                dir = calculateRingDirection(ringShape, previousBond, currentBond, previousDir);
            }
            //System.out.println(currentRing +"|" +neighbourRing +"|" +dir +"|" +(neighbourRing==previousRing));

            // place into connectivity table, like graph, rings and their connection
            for (auto ctToExpand : ctsToExpand) {
                ctToExpand->ringShapes.add(ringShape);
                ctToExpand->neighbouringRings.add(neighbourRing);
                ctToExpand->directionFromRingToNeighbouringRing.add(dir);
            }
            if (!std::find(currentCT->usedRings.begin(), currentCT->usedRings.end(), neighbourRing) !=
                currentCT->usedRings.end()) {
                std::vector<RingConnectivityTable *> newCts;
                for (auto ctToExpand : ctsToExpand) {
                    Atom *a = getAtomFromBond(currentRing, currentBond);
                    std::vector<RingConnectivityTable *> generatedDownStreamCts = buildRingConnectionTables(
                            neighbourRing, currentRing, dir, currentBond, a, ctToExpand, cts);
                    newCts.insert(newCts.end(), generatedDownStreamCts.begin(), generatedDownStreamCts.end());
                }
                ctsToExpand.insert(ctsToExpand.end(), newCts.begin(), newCts.end());
                generatedCts.insert(generatedCts.end(), newCts.begin(), newCts.end());
            }
        }
    }
    return generatedCts;
}

std::vector<FusionRingShape> FusedRingNumberer::getAllowedShapesForRing(Ring *ring, Bond *startingBond) {
    std::vector<FusionRingShape> allowedRingShapes;
    int size = ring->size();
    if (size == 5) {
        std::vector<Bond *> fusedBonds = ring->getFusedBonds();
        int fusedBondCount = fusedBonds.size();
        if (fusedBondCount == 1) {
            allowedRingShapes.push_back(FusionRingShape::enterFromLeftHouse);
        } else if (fusedBondCount == 2 || fusedBondCount == 3 || fusedBondCount == 4) {
            std::vector<int> distances; //one distance is likely to be 0
            for (auto fusedBond : fusedBonds) {
                distances.push_back(calculateDistanceBetweenBonds(startingBond, fusedBond, ring));
            }
            if (!std::find(distances.begin(), distances.end(), 1) != distances.end()) {
                allowedRingShapes.push_back(FusionRingShape::enterFromLeftHouse);
            }
            if (!std::find(distances.begin(), distances.end(), 4) != distances.end()) {
                allowedRingShapes.push_back(FusionRingShape::enterFromRightHouse);
            }

            if (!std::find(distances.begin(), distances.end(), 2) != distances.end()) {
                allowedRingShapes.push_back(FusionRingShape::enterFromTopLeftHouse);
            } else if (!std::find(distances.begin(), distances.end(), 3) != distances.end()) {
                allowedRingShapes.push_back(FusionRingShape::enterFromTopRightHouse);
            }
            allowedRingShapes = removeDegenerateRingShapes(allowedRingShapes, distances, 5);
        } else if (fusedBondCount == 5) {
            allowedRingShapes.push_back(FusionRingShape::enterFromLeftHouse);
            allowedRingShapes.push_back(FusionRingShape::enterFromRightHouse);
            //top left and top right are the same other than position of the elongated bond which will invariably be used anyway
            allowedRingShapes.push_back(FusionRingShape::enterFromTopLeftHouse);
        }
    } else if (size == 7) {
        std::vector<Bond *> fusedBonds = ring->getFusedBonds();
        int fusedBondCount = fusedBonds.size();
        if (fusedBondCount == 1) {
            allowedRingShapes.push_back(FusionRingShape::enterFromLeftSevenMembered);
        } else {
            std::vector<int> distances; //one distance is likely to be 0
            for (auto fusedBond : fusedBonds) {
                distances.push_back(calculateDistanceBetweenBonds(startingBond, fusedBond, ring));
            }
            if (!std::find(distances.begin(), distances.end(), 4) != distances.end() && !distances.contains(6)) {
                allowedRingShapes.push_back(FusionRingShape::enterFromLeftSevenMembered);
            }
            if (!std::find(distances.begin(), distances.end(), 1) != distances.end() && !distances.contains(6)) {
                allowedRingShapes.push_back(FusionRingShape::enterFromTopSevenMembered);
            }
            if (!std::find(distances.begin(), distances.end(), 1) != distances.end() && !distances.contains(3)) {
                allowedRingShapes.push_back(FusionRingShape::enterFromRightSevenMembered);
            }
            if (!std::find(distances.begin(), distances.end(), 2) != distances.end() && !distances.contains(4)) {
                allowedRingShapes.push_back(FusionRingShape::enterFromBottomRightSevenMembered);
            }
            if (!std::find(distances.begin(), distances.end(), 3) != distances.end() && !distances.contains(5)) {
                allowedRingShapes.push_back(FusionRingShape::enterFromBottomLeftSevenMembered);
            }
            allowedRingShapes = removeDegenerateRingShapes(allowedRingShapes, distances, 7);
        }
    } else {
        allowedRingShapes.push_back(FusionRingShape::standard);
    }
    return allowedRingShapes;
}

std::vector<FusionRingShape>
FusedRingNumberer::removeDegenerateRingShapes(std::vector<FusionRingShape> &allowedRingShapes,
                                              std::vector<int> &distances, int ringSize) {
    distances = std::vector<int>(distances);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    distances.remove(
            static_cast<boost::optional<int> *>(0)); //remove distance 0 if present, this invariably comes from the starting bond and is not of interest (and breaks getDirectionFromDist)
    for (int i = allowedRingShapes.size() - 1; i >= 0; i--) {
        FusionRingShape shapeToConsiderRemoving = allowedRingShapes[i];
        for (int j = i - 1; j >= 0; j--) {
            FusionRingShape shapeToCompareWith = allowedRingShapes[j];
            bool foundDifference = false;
            for (* : :optional < int > distance : distances)
            {
                if (getDirectionFromDist(shapeToConsiderRemoving, ringSize, distance) !=
                    getDirectionFromDist(shapeToCompareWith, ringSize, distance)) {
                    foundDifference = true;
                    break;
                }
            }
            if (!foundDifference) {
                allowedRingShapes.erase(allowedRingShapes.begin() + i);
                break;
            }
        }
    }

    return allowedRingShapes;
}

int FusedRingNumberer::calculateRingDirection(RingShape *ringShape, Bond *previousBond, Bond *currentBond,
                                              int previousDir) {
    // take the ring fused to one from the previous loop step
    Ring *ring = ringShape->getRing();
    if (ring->getCyclicBondList().empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: cyclic bond set should have already been populated");
        throw std::exception();
    }

    int dist = calculateDistanceBetweenBonds(previousBond, currentBond, ring);

    if (dist == 0) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Distance between bonds is equal to 0");
        throw std::exception();
    }

    int relativeDir = getDirectionFromDist(ringShape->getShape(), ring->size(), dist);
    return determineAbsoluteDirectionUsingPreviousDirection(ringShape->getShape(), ring->size(), relativeDir,
                                                            previousDir);
}

int FusedRingNumberer::calculateDistanceBetweenBonds(Bond *bond1, Bond *bond2, Ring *ring) {
    std::vector<Bond *> cyclicBondList = ring->getCyclicBondList();
    int previousBondIndice = cyclicBondList.find(bond1);
    int currentBondIndice = cyclicBondList.find(bond2);
    if (previousBondIndice == -1 || currentBondIndice == -1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: previous and current bond were not present in the cyclic bond list of the current ring");
        throw std::exception();
    }
    int ringSize = ring->size();
    int dist = (ringSize + currentBondIndice - previousBondIndice) % ringSize;
    return dist;
}

int FusedRingNumberer::getDirectionFromDist(FusionRingShape fusionRingShape, int ringSize, int dist) {
    int dir = 0;
    if (ringSize == 3) { // 3 member ring
        if (dist == 1) {
            dir = -1;
        } else if (dist == 2) {
            dir = 1;
        } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 3 membered ring");
            throw std::exception();
        }
    } else if (ringSize == 4) { // 4 member ring
        if (dist == 1) {
            dir = -2;
        } else if (dist == 2) {
            dir = 0;
        } else if (dist == 3) {
            dir = 2;
        } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 4 membered ring");
            throw std::exception();
        }
    } else if (ringSize == 5) { // 5 member ring
        switch (fusionRingShape) {
            case enterFromLeftHouse:
                if (dist == 1) {
                    dir = -2; //fusion to an elongated bond
                } else if (dist == 2) {
                    dir = 0;
                } else if (dist == 3) {
                    dir = 1;
                } else if (dist == 4) {
                    dir = 3;
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 5 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromTopLeftHouse:
                if (dist == 1) {
                    dir = -3;
                } else if (dist == 2) {
                    dir = -1; //fusion to an elongated bond
                } else if (dist == 3) {
                    dir = 1;
                } else if (dist == 4) {
                    dir = 3;
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 5 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromTopRightHouse:
                if (dist == 1) {
                    dir = -3;
                } else if (dist == 2) {
                    dir = -1;
                } else if (dist == 3) {
                    dir = 1; //fusion to an elongated bond
                } else if (dist == 4) {
                    dir = 3;
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 5 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromRightHouse:
                if (dist == 1) {
                    dir = -3;
                } else if (dist == 2) {
                    dir = -1;
                } else if (dist == 3) {
                    dir = 0;
                } else if (dist == 4) {
                    dir = 2; //fusion to an elongated bond
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 5 membered ring");
                    throw std::exception();
                }
                break;
            default :
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Unrecognised fusion ring shape for 5 membered ring");
                throw std::exception();
        }
    } else if (ringSize == 7) { // 7 member ring
        switch (fusionRingShape) {
            case enterFromLeftSevenMembered:
                if (dist == 1) {
                    dir = -3;
                } else if (dist == 2) {
                    dir = -1;
                } else if (dist == 3) {
                    dir = 0;
                } else if (dist == 4) {
                    dir = 1; //fusion to an abnormally angled bond
                } else if (dist == 5) {
                    dir = 2;
                } else if (dist == 6) {
                    dir = 3; //fusion to an abnormally angled bond
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 7 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromTopSevenMembered:
                if (dist == 1) {
                    dir = -3; //fusion to an abnormally angled bond
                } else if (dist == 2) {
                    dir = -2;
                } else if (dist == 3) {
                    dir = -1;
                } else if (dist == 4) {
                    dir = 1;
                } else if (dist == 5) {
                    dir = 2;
                } else if (dist == 6) {
                    dir = 3; //fusion to an abnormally angled bond
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 7 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromRightSevenMembered:
                if (dist == 1) {
                    dir = -3; //fusion to an abnormally angled bond
                } else if (dist == 2) {
                    dir = -2;
                } else if (dist == 3) {
                    dir = -1; //fusion to an abnormally angled bond
                } else if (dist == 4) {
                    dir = 0;
                } else if (dist == 5) {
                    dir = 1;
                } else if (dist == 6) {
                    dir = 3;
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 7 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromBottomRightSevenMembered:
                if (dist == 1) {
                    dir = -3;
                } else if (dist == 2) {
                    dir = -2; //fusion to an abnormally angled bond
                } else if (dist == 3) {
                    dir = -1;
                } else if (dist == 4) {
                    dir = 0; //fusion to an abnormally angled bond
                } else if (dist == 5) {
                    dir = 1;
                } else if (dist == 6) {
                    dir = 3;
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 7 membered ring");
                    throw std::exception();
                }
                break;
            case enterFromBottomLeftSevenMembered:
                if (dist == 1) {
                    dir = -3;
                } else if (dist == 2) {
                    dir = -1;
                } else if (dist == 3) {
                    dir = 0; //fusion to an abnormally angled bond
                } else if (dist == 4) {
                    dir = 1;
                } else if (dist == 5) {
                    dir = 2; //fusion to an abnormally angled bond
                } else if (dist == 6) {
                    dir = 3;
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Impossible distance between bonds for a 7 membered ring");
                    throw std::exception();
                }
                break;
            default:
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Unrecognised fusion ring shape for 7 membered ring");
                throw std::exception();
        }
    } else if (ringSize % 2 == 0) { //general case even number of atoms ring (a 6 membered ring or distortion of)
        if (dist == 1) {
            dir = -3;
        } else if (dist == ringSize - 1) {
            dir = 3;
        } else {
            dir = dist - ringSize / 2;
            if (std::abs(dir) > 2 && ringSize >= 8) { // 8 and more neighbours
                dir = -2 * Integer::signum(dir);
            }
        }
    } else { // general case odd number of atoms ring (distortion of an even numbered ring by insertion of one atom).
        if (dist == 1) {
            dir = -3;
        } else if (dist == ringSize / 2 || dist == ringSize / 2 +
                                                   1) { //0 in both cases as effectively we are using a different depiction of the ring system. See FR-5.1.1 (this is done to give the longest horizontal row)
            dir = 0;
        } else if (dist == ringSize - 1) {
            dir = 3;
        } else if (dist < ringSize / 2) {
            dir = -2;
        } else if (dist > ringSize / 2 + 1) {
            dir = 2;
        } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Unable to determine direction between odd number of atoms ring and next ring");
            throw std::exception();
        }
    }
    return dir;
}

void FusedRingNumberer::removeCTsWithDistortedRingShapes(std::vector<RingConnectivityTable *> &cts) {
    std::unordered_map<RingConnectivityTable *, std::vector<int>> ctToDistortedRings;
    for (auto ct : cts) {
        std::vector<int> distortedRingSizes;
        ctToDistortedRings[ct] = distortedRingSizes;
        std::vector<RingShape *> &ringShapes = ct->ringShapes;
        for (int i = 0; i < ringShapes.size(); i++) {
            Ring *r1 = ringShapes[i]->getRing();
            Ring *r2 = ct->neighbouringRings.get(i);
            for (int j = i + 1; j < ringShapes.size(); j++) {
                if (ringShapes[j]->getRing().equals(r2) && ct->neighbouringRings.get(j)->equals(
                        r1)) { //look for the reverse entry in the ring connection table
                    int expectedDir = getOppositeDirection(ct->directionFromRingToNeighbouringRing.get(i));
                    if (expectedDir != ct->directionFromRingToNeighbouringRing.get(j)) {
                        distortedRingSizes.push_back(r2->size());
                    }
                }
            }
        }
    }
    int minDistortedRings = std::numeric_limits<int>::max(); //find the minimum number of distorted rings
    for (auto distortedRingSizes : ctToDistortedRings) {
        if (distortedRingSizes.second->size() < minDistortedRings) {
            minDistortedRings = distortedRingSizes.second->size();
        }
    }
    for (int i = cts.size() - 1; i >= 0; i--) {
        if (ctToDistortedRings[cts[i]].size() > minDistortedRings) {
            cts.erase(cts.begin() + i);
        }
    }
}

std::unordered_map<RingConnectivityTable *, std::vector<int>>
FusedRingNumberer::findLongestChainDirections(std::vector<RingConnectivityTable *> &cts) {
    std::unordered_map<RingConnectivityTable *, std::vector<int>> horizonalRowDirections = new LinkedHashMap<RingConnectivityTable *, std::vector<int>>();
    int maxChain = 0;
    for (auto ct : cts) {
        if (ct->ringShapes.size() != ct->neighbouringRings.size() ||
            ct->neighbouringRings.size() != ct->directionFromRingToNeighbouringRing.size()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Sizes of arrays in fused ring numbering connection table are not equal");
            throw std::exception();
        }
        int ctEntriesSize = ct->ringShapes.size();
        std::vector<int> directions;
        horizonalRowDirections[ct] = directions;

        for (int i = 0; i < ctEntriesSize; i++) {
            Ring *neighbour = ct->neighbouringRings.get(i);
            int curChain = 1;
            int curDir = ct->directionFromRingToNeighbouringRing.get(i);

            for (int k = 0; k <= ct->usedRings.size(); k++) { //<= rather than < so buggy behaviour can be caught
                int indexOfNeighbour = indexOfCorrespondingRingshape(ct->ringShapes, neighbour);

                if (indexOfNeighbour >= 0) {
                    for (int j = indexOfNeighbour; j < ctEntriesSize; j++) {
                        if (ct->ringShapes.get(j)->getRing() == neighbour &&
                            ct->directionFromRingToNeighbouringRing.get(j) == curDir) {
                            curChain++;
                            neighbour = ct->neighbouringRings.get(j);
                            goto nextRingInChainLoopContinue;
                        }
                    }
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: fused ring numbering: Ring missing from connection table");
                    throw std::exception();
                }
                if (curChain >= maxChain) {
                    int oDir = getOppositeDirection(curDir);
                    if (curChain > maxChain) { //new longest chain found
                        for (auto previousDirections : horizonalRowDirections) {
                            previousDirections.second->clear();
                        }
                    }
                    // if we has this direction before or its opposite, it is the same orientation
                    if (curChain > maxChain ||
                        (!std::find(directions.begin(), directions.end(), curDir) != directions.end() &&
                         !directions.contains(oDir))) {
                        directions.push_back(curDir);
                    }
                    maxChain = curChain;
                }
                break;
                nextRingInChainLoopContinue:
            }
            nextRingInChainLoopBreak:
            if (maxChain > ct->usedRings.size()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: fused ring layout contained a loop: more rings in a chain than there were rings!");
                throw std::exception();
            }
        }
    }
    return horizonalRowDirections;
}

int FusedRingNumberer::indexOfCorrespondingRingshape(std::vector<RingShape *> &ringShapes, Ring *ring) {
    for (int i = 0; i < ringShapes.size(); i++) {
        if (ringShapes[i]->getRing().equals(ring)) {
            return i;
        }
    }
    return -1;
}

std::vector<std::vector<std::vector<Ring *>>> FusedRingNumberer::createRingMapsAlignedAlongGivenhorizonalRowDirections(
        std::unordered_map<RingConnectivityTable *, std::vector<int>> &horizonalRowDirectionsMap) throw(StructureBuildingException) {
    std::vector<std::vector<std::vector<Ring *>>> ringMaps;
    for (auto entry : horizonalRowDirectionsMap) {
        RingConnectivityTable *ct = entry.first;
        if (ct->ringShapes.size() != ct->neighbouringRings.size() ||
            ct->neighbouringRings.size() != ct->directionFromRingToNeighbouringRing.size() ||
            ct->ringShapes.size() <= 0) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Sizes of arrays in fused ring numbering connection table are not equal");
            throw std::exception();
        }
        int ctEntriesSize = ct->ringShapes.size();
        for (* : :optional < int > horizonalRowDirection : entry.second)
        {
            std::vector<int> directionFromRingToNeighbouringRing(ctEntriesSize);
            // turn the ring system such as to be aligned along the horizonalRowDirection
            for (int i = 0; i < ctEntriesSize; i++) {
                RingShape *ringShape = ct->ringShapes[i];
                directionFromRingToNeighbouringRing[i] = determineAbsoluteDirectionUsingPreviousDirection(
                        ringShape->getShape(), ringShape->getRing()->size(), ct->directionFromRingToNeighbouringRing[i],
                        -horizonalRowDirection);
            }
            std::vector<std::vector<Ring *>> ringMap = generateRingMap(ct, directionFromRingToNeighbouringRing);
            if (ringMap.size() > 0) { //null if overlapping bonds rings present
                ringMaps.push_back(ringMap);
            }
        }
    }
    if (ringMaps.empty()) {
        throw StructureBuildingException(
                L"Fused ring systems with overlapping rings such as in helices cannot currently be numbered");
    }
    return ringMaps;
}

std::vector<std::vector<Atom *>>
FusedRingNumberer::findPossiblePaths(std::vector<std::vector<std::vector<Ring *>>> &ringMaps,
                                     int atomCountOfFusedRingSystem) {
    std::vector<std::vector<double>> chainQs;
    std::vector<std::vector<std::vector<Ring *>>> correspondingRingMap;
    for (auto ringMap : ringMaps) {
        std::vector<Chain *> chains = findChainsOfMaximumLengthInHorizontalDir(ringMap);
        // For each chain count the number of rings in each quadrant
        for (auto chain : chains) {
            int midChainXcoord = chain->getLength() + chain->getStartingX() -
                                 1; //Remember the X axis is measured in 1/2s so don't need to 1/2 length

            std::vector<boost::optional<double>> qs = countQuadrants(ringMap, midChainXcoord, chain->getY());
            chainQs.push_back(qs);
            correspondingRingMap.push_back(ringMap);
        }
    }

    /*
     * The quadrant numbers are as follows:
     *
     *  1  |  0
     * ----+----
     *  2  |  3
     *
     *  But at this stage it is not known what the mapping between these numbers and the/a preferred orientation of the structure is
     */
    //  order for each right corner candidates for each chain
    std::vector<std::vector<int>> allowedUpperRightQuadrantsForEachChain = rulesBCD(chainQs);

    std::vector<std::vector<Atom *>> paths;
    for (int c = 0; c < chainQs.size(); c++) {
        std::vector<std::vector<Ring *>> ringMap = correspondingRingMap[c];
        std::vector<int> allowedUpperRightQuadrants = allowedUpperRightQuadrantsForEachChain[c];

        for (* : :optional < int > upperRightQuadrant : allowedUpperRightQuadrants)
        {
            std::vector<std::vector<Ring *>> qRingMap = transformQuadrantToUpperRightOfRingMap(ringMap,
                                                                                               upperRightQuadrant);
            if (LOG->isTraceEnabled()) {
                debugRingMap(qRingMap);
            }
            bool inverseAtoms = (upperRightQuadrant == 2 || upperRightQuadrant == 0);
            std::vector<Atom *> peripheralAtomPath = orderAtoms(qRingMap, inverseAtoms, atomCountOfFusedRingSystem);
            paths.push_back(peripheralAtomPath);
        }
    }

    return paths;
}

std::vector<std::vector<Ring *>>
FusedRingNumberer::generateRingMap(RingConnectivityTable *ct, std::vector<int> &directionFromRingToNeighbouringRing) {
    int ctEntriesSize = ct->ringShapes.size();
    // Find max and min coordinates for ringMap
    // we put the first ring into takenRings to start with it in the connection table
    int nRings = ct->usedRings.size();
    std::vector<std::vector<int>> coordinates(nRings); // correspondent to usedRings
    std::vector<Ring *> takenRings(nRings);
    int takenRingsCnt = 0;
    int maxX = 0;
    int minX = 0;
    int maxY = 0;
    int minY = 0;

    takenRings[takenRingsCnt++] = ct->ringShapes[0]->getRing();
    coordinates[0] = std::vector<int>{0, 0};

    // Go through the rings in a system
    // Find the rings connected to them and assign coordinates according to the direction
    // Each time we go to the ring, whose coordinates were already identified.
    for (int tr = 0; tr < nRings - 1; tr++) {
        Ring *currentRing = takenRings[tr];
        if (currentRing == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected null ring in fused ring numbering");
            throw std::exception();
        }

        int indexOfCurrentRing = indexOfCorrespondingRingshape(ct->ringShapes, currentRing);

        std::vector<int> xy = coordinates[tr]; // find the correspondent coordinates for the ring

        if (indexOfCurrentRing >= 0) {
            for (int j = indexOfCurrentRing; j < ctEntriesSize; j++) {
                if (ct->ringShapes[j]->getRing() == currentRing) {
                    Ring *neighbour = ct->neighbouringRings[j];
                    if (arrayContains(takenRings, neighbour)) {
                        continue;
                    }

                    std::vector<int> newXY(2);
                    newXY[0] = xy[0] + std::round(2 * countDX(directionFromRingToNeighbouringRing[j]));
                    newXY[1] = xy[1] + countDY(directionFromRingToNeighbouringRing[j]);

                    if (takenRingsCnt > takenRings.size()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Fused ring numbering bug");
                        throw std::exception();
                    }
                    takenRings[takenRingsCnt] = neighbour;
                    coordinates[takenRingsCnt] = newXY;
                    takenRingsCnt++;

                    if (newXY[0] > maxX) {
                        maxX = newXY[0];
                    } else if (newXY[0] < minX) {
                        minX = newXY[0];
                    }

                    if (newXY[1] > maxY) {
                        maxY = newXY[1];
                    } else if (newXY[1] < minY) {
                        minY = newXY[1];
                    }
                }
            }
        } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: fused ring numbering: Ring missing from connection table");
            throw std::exception();
        }
    }
    // the height and the width of the map
    int h = maxY - minY + 1;
    int w = maxX - minX + 1;

//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: Ring[][] ringMap = new Ring[w][h];
//							std::vector<std::vector<Ring*>> ringMap = RectangularVectors::ReturnRectangularVector(w, h);
    std::vector<std::vector<Ring *>> ringMap = RectangularVectors::ReturnRectangularVector(w, h);
//							Ring* ringMap[w][h] = {};

    // Map rings using coordinates calculated in the previous step, and transform them according to found minX and minY

    int ix = -minX;
    int iy = -minY;
    if (ix >= w || iy >= h) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Fused ring numbering bug, Coordinates have been calculated wrongly");
        throw std::exception();
    }

    int curX = 0;
    int curY = 0;
    for (int ti = 0; ti < takenRings.size(); ti++) {
        std::vector<int> xy = coordinates[ti];
        curX = xy[0] - minX;
        curY = xy[1] - minY;
        if (curX < 0 || curX > w || curY < 0 || curY > h) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Fused ring numbering bug, Coordinates have been calculated wrongly");
            throw std::exception();
        }
        if (ringMap[curX][curY] != nullptr) {
            return nullptr;
        }
        ringMap[curX][curY] = takenRings[ti];
    }
    return ringMap;
}

std::vector<Chain *>
FusedRingNumberer::findChainsOfMaximumLengthInHorizontalDir(std::vector<std::vector<Ring *>> &ringMap) {
    int w = ringMap.size();
    int h = ringMap[0].length;

    std::vector<Chain *> chains;

    int maxChain = 0;
    int chain = 0;

    // Find the longest chain
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (ringMap[i][j] != nullptr) {
                chain = 1;
                while (i + 2 * chain < w && ringMap[i + 2 * chain][j] != nullptr) {
                    chain++; // *2 because along the x axis the step is 2
                }
                if (chain > maxChain) {
                    chains.clear();
                    maxChain = chain;
                }
                if (chain >= maxChain) {
                    Chain tempVar(chain, i, j);
                    chains.push_back(&tempVar);
                }
                i += 2 * chain;
            }
        }
    }
    return chains;
}

std::vector<boost::optional<double>>
FusedRingNumberer::countQuadrants(std::vector<std::vector<Ring *>> &ringMap, int midChainXcoord, int yChain) {
    std::vector<boost::optional<double>> qs = std::vector<boost::optional<double> *>(4);
    qs[0] = 0;
    qs[1] = 0;
    qs[2] = 0;
    qs[3] = 0;
    int w = ringMap.size();
    int h = ringMap[0].length;

    // Count rings in each quadrants
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (ringMap[x][y] == nullptr) {
                continue;
            }

            if (x == midChainXcoord || y == yChain) { // if the ring is on the axis
                if (x == midChainXcoord && y > yChain) {
                    qs[0] += 0.5;
                    qs[1] += 0.5;
                } else if (x == midChainXcoord && y < yChain) {
                    qs[2] += 0.5;
                    qs[3] += 0.5;
                } else if (x < midChainXcoord && y == yChain) {
                    qs[1] += 0.5;
                    qs[2] += 0.5;
                } else if (x > midChainXcoord && y == yChain) {
                    qs[0] += 0.5;
                    qs[3] += 0.5;
                }
                if (x == midChainXcoord && y == yChain) {
                    qs[0] += 0.25;
                    qs[1] += 0.25;
                    qs[2] += 0.25;
                    qs[3] += 0.25;
                }
            } else if (x > midChainXcoord && y > yChain) {
                qs[0]++;
            } else if (x < midChainXcoord && y > yChain) {
                qs[1]++;
            } else if (x < midChainXcoord && y < yChain) {
                qs[2]++;
            } else if (x > midChainXcoord && y < yChain) {
                qs[3]++;
            }
        }
    }

    return qs;
}

std::vector<std::vector<int>> FusedRingNumberer::rulesBCD(std::vector<std::vector<double>> &chainQs) {
    std::vector<std::vector<int>> possibleUpperRightQuadrantsForEachChain;
    int nChains = chainQs.size();
    if (nChains == 0) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Fused ring numbering, no chains found?");
        throw std::exception();
    }

    // Rule B: Maximum number of rings in upper right quadrant. Upper right corner candidates (it is not at this stage known which quadrant is the upper right one)
    double qmax = 0;

    for (* : :optional<double>[]
    chainQ :
    chainQs)
    {
        for (int j = 0; j < 4; j++) {
            boost::optional<double> q = chainQ[j];
            if (q > qmax) {
                qmax = q;
            }
        }
    }

    for (* : :optional<double>[]
    chainQ :
    chainQs)
    {
        std::vector<int> allowedUpperRightQuadrants;
        for (int j = 0; j < 4; j++) {
            if (chainQ[j] == qmax) {
                allowedUpperRightQuadrants.push_back(j);
            }
        }
        possibleUpperRightQuadrantsForEachChain.push_back(allowedUpperRightQuadrants);
    }

    // Rule C: Minimum number of rings in lower left quadrant
    double qmin = std::numeric_limits<double>::max();

    for (int c = 0; c < nChains; c++) {
        std::vector<int> possibleUpperRightQuadrant = possibleUpperRightQuadrantsForEachChain[c];
        for (* : :optional < int > upperRightQuad : possibleUpperRightQuadrant)
        {
            int qdiagonal = (upperRightQuad + 2) % 4;
            if (chainQs[c][qdiagonal] < qmin) {
                qmin = chainQs[c][qdiagonal];
            }
        }
    }
    for (int c = 0; c < nChains; c++) {
        std::vector<int> possibleUpperRightQuadrant = possibleUpperRightQuadrantsForEachChain[c];
        std::vector<int> allowedUpperRightQuadrants;
        for (* : :optional < int > upperRightQuad : possibleUpperRightQuadrant)
        {
            int qdiagonal = (upperRightQuad + 2) % 4;
            if (chainQs[c][qdiagonal] == qmin) {
                allowedUpperRightQuadrants.push_back(upperRightQuad);
            }
        }
        possibleUpperRightQuadrantsForEachChain[c] = allowedUpperRightQuadrants;
    }

    // Rule D: Maximum number of rings above the horizontal row
    double rMax = 0;
    for (int c = 0; c < nChains; c++) {
        std::vector<int> possibleUpperRightQuadrant = possibleUpperRightQuadrantsForEachChain[c];
        for (* : :optional < int > upperRightQuad : possibleUpperRightQuadrant)
        {
            int upperLeftQuad;
            if (upperRightQuad % 2 == 0) {
                upperLeftQuad = upperRightQuad + 1;
            } else {
                upperLeftQuad = upperRightQuad - 1;
            }

            if (chainQs[c][upperLeftQuad] + chainQs[c][upperRightQuad] > rMax) {
                rMax = chainQs[c][upperLeftQuad] + chainQs[c][upperRightQuad];
            }
        }
    }
    for (int c = 0; c < nChains; c++) {
        std::vector<int> possibleUpperRightQuadrant = possibleUpperRightQuadrantsForEachChain[c];
        std::vector<int> allowedUpperRightQuadrants;
        for (* : :optional < int > upperRightQuad : possibleUpperRightQuadrant)
        {
            int upperLeftQuad;
            if (upperRightQuad % 2 == 0) {
                upperLeftQuad = upperRightQuad + 1;
            } else {
                upperLeftQuad = upperRightQuad - 1;
            }

            if (chainQs[c][upperLeftQuad] + chainQs[c][upperRightQuad] == rMax) {
                allowedUpperRightQuadrants.push_back(upperRightQuad);
            }
        }
        possibleUpperRightQuadrantsForEachChain[c] = allowedUpperRightQuadrants;
    }
    return possibleUpperRightQuadrantsForEachChain;
}

std::vector<Atom *> FusedRingNumberer::orderAtoms(std::vector<std::vector<Ring *>> &ringMap, bool inverseAtoms,
                                                  int atomCountOfFusedRingSystem) {
    int w = ringMap.size();
    int h = ringMap[0].length;

    // find upper right ring
    Ring *upperRightRing = nullptr;
    for (int i = w - 1; i >= 0; i--) {
        if (ringMap[i][h - 1] != nullptr) {
            upperRightRing = ringMap[i][h - 1];
            break;
        }
    }
    if (upperRightRing == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Upper right ring not found when performing fused ring numbering");
        throw std::exception();
    }
    std::vector<Ring *> visitedRings;
    visitedRings.push_back(upperRightRing);
    while (isEntirelyFusionAtoms(upperRightRing)) { //c.f cyclopropa[de]anthracene
        upperRightRing = findClockwiseRingFromUpperRightRing(ringMap, upperRightRing, visitedRings);
        if (upperRightRing == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Unabled to find clockwise ring without fusion atoms");
            throw std::exception();
        }
        visitedRings.push_back(upperRightRing);
    }

    Ring *prevRing = findUpperLeftNeighbourOfUpperRightRing(ringMap, upperRightRing);
    Bond *prevBond = findFusionBond(upperRightRing, prevRing);
    Bond *nextBond = nullptr;

    Ring *currentRing = upperRightRing;
    Ring *nextRing = nullptr;
    std::vector<Atom *> atomPath;
    int count = 0;
    for (; count <= atomCountOfFusedRingSystem; count++) {
        int ringSize = currentRing->size();

        int startingBondIndex = currentRing->getBondIndex(prevBond);

        std::vector<Bond *> cyclicBonds = currentRing->getCyclicBondList();
        std::vector<Bond *> fusedBonds = currentRing->getFusedBonds();
        if (!inverseAtoms) {
            for (int bondIndex = 0; bondIndex < ringSize; bondIndex++) {
                int i = (startingBondIndex + bondIndex + 1) %
                        ringSize; // +1 because we start from the bond next to stBond and end with it
                // if this bond is fused then it indicates the next ring to move to
                Bond *bond = cyclicBonds[i];
                if (std::find(fusedBonds.begin(), fusedBonds.end(), bond) != fusedBonds.end()) {
                    nextBond = bond;
                    break;
                }
            }
        } else {
            for (int bondIndex = 0; bondIndex < ringSize; bondIndex++) {
                int i = (startingBondIndex - bondIndex - 1 + ringSize) %
                        ringSize; // -1 because we start from the bond next to stBond and end with it
                // if this bond is fused then it indicates the next ring to move to
                Bond *bond = cyclicBonds[i];
                if (std::find(fusedBonds.begin(), fusedBonds.end(), bond) != fusedBonds.end()) {
                    nextBond = bond;
                    break;
                }
            }
        }
        if (nextBond == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: None of the bonds from this ring were fused, but this is not possible ");
            throw std::exception();
        }

        // next ring
        nextRing = currentRing->getNeighbourOfFusedBond(nextBond);

        int endNumber = currentRing->getBondIndex(nextBond);

        // Add atoms in order, considering inverse or not inverse
        if (!inverseAtoms) {
            // if distance between prev bond and cur bond = 1 (it means that fused bonds are next to each other) i.e. come under interior atom numbering
            // we don't add that atom, cause it was added already
            if ((endNumber - startingBondIndex + ringSize) % ringSize != 1) {
                startingBondIndex = (startingBondIndex + 1) % ringSize;
                endNumber = (endNumber - 1 + ringSize) % ringSize;
                if (startingBondIndex > endNumber) {
                    endNumber += ringSize;
                }

                // start from the atom next to fusion
                for (int j = startingBondIndex; j <= endNumber; j++) {
                    Atom *atom = currentRing->getCyclicAtomList()[j % ringSize];
                    if (std::find(atomPath.begin(), atomPath.end(), atom) != atomPath.end()) {
                        goto mainLoopBreak;
                    }
                    atomPath.push_back(atom);
                }
            }
        } else {
            if ((startingBondIndex - endNumber + ringSize) % ringSize != 1) {
                startingBondIndex = (startingBondIndex - 2 + ringSize) % ringSize;
                endNumber = endNumber % ringSize;
                if (startingBondIndex < endNumber) {
                    startingBondIndex += ringSize;
                }

                for (int j = startingBondIndex; j >= endNumber; j--) {
                    Atom *atom = currentRing->getCyclicAtomList()[j % ringSize];
                    if (std::find(atomPath.begin(), atomPath.end(), atom) != atomPath.end()) {
                        goto mainLoopBreak;
                    }
                    atomPath.push_back(atom);
                }
            }
        }
        prevBond = nextBond;
        prevRing = currentRing;
        currentRing = nextRing;
        mainLoopContinue:
    }
    mainLoopBreak:
    if (count == atomCountOfFusedRingSystem) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Fused ring numbering may have been stuck in an infinite loop while enumerating peripheral numbering");
        throw std::exception();
    }
    return atomPath;
}

bool FusedRingNumberer::isEntirelyFusionAtoms(Ring *upperRightRing) {
    std::vector<Atom *> atomList = upperRightRing->getAtomList();
    for (auto atom : atomList) {
        if (atom->getBondCount() < 3) {
            return false;
        }
    }
    return true;
}

Ring *
FusedRingNumberer::findClockwiseRingFromUpperRightRing(std::vector<std::vector<Ring *>> &ringMap, Ring *upperRightRing,
                                                       std::vector<Ring *> &visitedRings) {
    Ring *clockwiseRing = nullptr;
    int maxX = 0;
    int maxY = 0;

    for (auto ring : upperRightRing->getNeighbours()) {
        if (std::find(visitedRings.begin(), visitedRings.end(), ring) != visitedRings.end()) {
            continue;
        }
        std::vector<int> xy = findRingPosition(ringMap, ring);
        if (xy.empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Ring not found in ringMap when performing fused ring numbering");
            throw std::exception();
        }

        if (xy[0] > maxX || xy[0] == maxX && xy[1] > maxY) {
            maxX = xy[0];
            maxY = xy[1];
            clockwiseRing = ring;
        }
    }
    return clockwiseRing;
}

Ring *FusedRingNumberer::findUpperLeftNeighbourOfUpperRightRing(std::vector<std::vector<Ring *>> &ringMap,
                                                                Ring *upperRightRing) {
    Ring *nRing = nullptr;
    int minX = std::numeric_limits<int>::max();
    int maxY = 0;

    for (auto ring : upperRightRing->getNeighbours()) {
        // upper left would be previous ring
        std::vector<int> xy = findRingPosition(ringMap, ring);
        if (xy.empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Ring not found in ringMap when performing fused ring numbering");
            throw std::exception();
        }

        if (xy[1] > maxY || xy[1] == maxY && xy[0] < minX) {
            minX = xy[0];
            maxY = xy[1];
            nRing = ring;
        }
    }
    return nRing;
}

std::vector<int> FusedRingNumberer::findRingPosition(std::vector<std::vector<Ring *>> &ringMap, Ring *ring) {
    int w = ringMap.size();
    int h = ringMap[0].length;

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (ringMap[i][j] == ring) {
                return std::vector<int>{i, j};
            }
        }
    }
    return nullptr;
}

std::vector<std::vector<Ring *>>
FusedRingNumberer::transformQuadrantToUpperRightOfRingMap(std::vector<std::vector<Ring *>> &ringMap,
                                                          int upperRightQuadrant) {
    int w = ringMap.size();
    int h = ringMap[0].length;

//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: Ring[][] rearrangedMap = new Ring[w][h];
    std::vector<std::vector<Ring *>> rearrangedMap = RectangularVectors::ReturnRectangularVector(w, h);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (upperRightQuadrant == 0) { //already is in the upper right
                rearrangedMap[i][j] = ringMap[i][j];
            }
            if (upperRightQuadrant == 1) { //flip in y axis
                rearrangedMap[w - i - 1][j] = ringMap[i][j];
            } else if (upperRightQuadrant == 2) { //flip in x and y axes
                rearrangedMap[w - i - 1][h - j - 1] = ringMap[i][j];
            } else if (upperRightQuadrant == 3) { //flip in x axis
                rearrangedMap[i][h - j - 1] = ringMap[i][j];
            }
        }
    }

    return rearrangedMap;
}

bool FusedRingNumberer::arrayContains(std::vector<void *> &array_Renamed, void *obj) {
    for (auto arrObj : array_Renamed) {
        if (arrObj == obj) {
            return true;
        }
    }
    return false;
}

Bond *FusedRingNumberer::getStartingNonFusedBond(Ring *tRing) {
    std::vector<Bond *> allBonds(tRing->getBondList());
    for (auto fusedBond : tRing->getFusedBonds()) {
        std::vector<Bond *> neighbouringBonds = fusedBond->getFromAtom()->getBonds();
        for (auto bond : neighbouringBonds) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            allBonds.remove(bond);
        }
        neighbouringBonds = fusedBond->getToAtom()->getBonds();
        for (auto bond : neighbouringBonds) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            allBonds.remove(bond);
        }
    }
    if (allBonds.size() > 0) {
        return allBonds[0];
    }
    for (auto bond : tRing->getBondList()) {
        if (tRing->getNeighbourOfFusedBond(bond) == nullptr) {
            // return a non-fused bond
            return bond;
        }
    }
    return nullptr;
}

int FusedRingNumberer::getOppositeDirection(int prevDir) {
    int dir;
    if (prevDir == 0) {
        dir = 4;
    } else if (std::abs(prevDir) == 4) {
        dir = 0;
    } else if (std::abs(prevDir) == 2) {
        dir = 2 * -1 * Integer::signum(prevDir);
    } else if (std::abs(prevDir) == 1) {
        dir = 3 * -1 * Integer::signum(prevDir);
    } else { //prevDir will be +-3
        dir = 1 * -1 * Integer::signum(prevDir);
    }
    return dir;
}

Atom *FusedRingNumberer::getAtomFromBond(Ring *ring, Bond *curBond) {
    if (ring->getCyclicBondList().empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("The cyclic bond list should already have been generated");
        throw std::exception();
    }
    int bondIndice = ring->getCyclicBondList().find(curBond);
    int atomIndice = (bondIndice - 1 + ring->size()) % ring->size();
    return ring->getCyclicAtomList()[atomIndice];
}

Bond *FusedRingNumberer::findFusionBond(Ring *r1, Ring *r2) {
    std::vector<Bond *> b2 = r2->getBondList();
    for (auto bond : r1->getBondList()) {
        if (std::find(b2.begin(), b2.end(), bond) != b2.end()) {
            return bond;
        }
    }
    return nullptr;
}

float FusedRingNumberer::countDX(int val) {
    float dX = 0;
    if (std::abs(val) == 1) {
        dX += 0.5f;
    } else if (std::abs(val) == 3) {
        dX -= 0.5f;
    } else if (std::abs(val) == 0) {
        dX += 1.0f;
    } else if (std::abs(val) == 4) {
        dX -= 1.0f;
    }
    return dX;
}

int FusedRingNumberer::countDY(int val) {
    int dY = 0;
    if (std::abs(val) != 4) {
        if (val > 0) {
            dY = 1;
        }
        if (val < 0) {
            dY = -1;
        }
    }
    return dY;
}

int FusedRingNumberer::determineAbsoluteDirectionUsingPreviousDirection(FusionRingShape fusionRingShape, int ringSize,
                                                                        int relativeDirection, int previousDir) {
    int interimDirection;
    if (std::abs(previousDir) == 4) {
        if (relativeDirection == 0) {
            interimDirection = 4;
        } else {
            interimDirection = relativeDirection +
                               4 * -1 * Integer::signum(relativeDirection); // if dir<0 we add 4, if dir>0 we add -4
        }
    } else {
        interimDirection = relativeDirection + previousDir;
    }
    if (std::abs(interimDirection) > 4) { // Added
        interimDirection = (8 - std::abs(interimDirection)) * Integer::signum(interimDirection) * -1;
    }
    //TODO investigate this function and unit test
    /* Even numbered rings when angled do not have direction 2.
    * Almost true for 5 member except for corner case where fusion to elongated bond occurs
    */
    if (std::abs(interimDirection) == 2 && ((ringSize % 2 == 0) || ringSize == 5 || ringSize == 7)) {
        // if (one of them equal to 1 and another is equal to 3, we decrease absolute value and conserve the sign)
        if (std::abs(relativeDirection) == 1 && std::abs(previousDir) == 3 ||
            std::abs(relativeDirection) == 3 && std::abs(previousDir) == 1) {
            interimDirection = 1 * Integer::signum(interimDirection);
        }
            // if both are equal to 1
        else if (std::abs(relativeDirection) == 1 && std::abs(previousDir) == 1) {
            interimDirection = 3 * Integer::signum(interimDirection);
        }
            // if both are equal to 3
        else if (std::abs(relativeDirection) == 3 && std::abs(previousDir) == 3) {
            interimDirection = 3 * Integer::signum(interimDirection);
        }
        // else it is correctly 2
    }

    if (interimDirection == -4) {
        interimDirection = 4;
    }

    return interimDirection;
}

void FusedRingNumberer::debugRingMap(std::vector<std::vector<Ring *>> &ringMap) {
//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: Ring[][] yxOrdered = new Ring[ringMap[0].length][ringMap.length];
    std::vector<std::vector<Ring *>> yxOrdered = RectangularVectors::ReturnRectangularVector(ringMap[0].length,
                                                                                                 ringMap.size());
    for (int x = 0; x < ringMap.size(); x++) {
        std::vector<Ring *> yRings = ringMap[x];
        for (int y = 0; y < yRings.size(); y++) {
            yxOrdered[y][x] = yRings[y];
        }
    }
    for (int y = yxOrdered.size() - 1; y >= 0; y--) {
        std::vector<Ring *> xRings = yxOrdered[y];
        StringBuilder *sb = new StringBuilder();
        for (auto ring : xRings) {
            if (ring != nullptr) {
                int size = ring->size();
                if (size > 9) {
                    if (size == 10) {
                        sb->append(L"0");
                    } else if (size % 2 == 0) {
                        sb->append(L"2");
                    } else {
                        sb->append(L"1");
                    }
                } else {
                    sb->append(size);
                }
            } else {
                sb->append(L" ");
            }
        }
        LOG->trace(sb->toString());
    }
    LOG->trace(L"#########");

}
