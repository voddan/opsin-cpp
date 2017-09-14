#include "FragmentTools.h"
#include "Atom.h"
#include "StructureBuildingMethods.h"
#include "XmlDeclarations.h"
#include "Fragment.h"
#include "StructureBuildingException.h"
#include "StringTools.h"
#include "AtomProperties.h"
#include "OpsinTools.h"
#include "Bond.h"
#include "OutAtom.h"
#include "BuildState.h"
#include "AtomParity.h"
#include "CyclicAtomList.h"
#include "ValencyChecker.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

int SortAtomsForElementSymbols::compare(Atom *a, Atom *b) {
    int bondOrderA = a->getProperty(Atom::VISITED);
    int bondOrderB = b->getProperty(Atom::VISITED);
    if (bondOrderA > bondOrderB) { //lower order bond is preferred
        return 1;
    }
    if (bondOrderA < bondOrderB) {
        return -1;
    }

    if (a->getOutValency() > b->getOutValency()) { //prefer atoms with outValency
        return -1;
    }
    if (a->getOutValency() < b->getOutValency()) {
        return 1;
    }

    int expectedHydrogenA = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a);
    int expectedHydrogenB = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(b);

    if (expectedHydrogenA > expectedHydrogenB) { //prefer atoms with more hydrogen
        return -1;
    }
    if (expectedHydrogenA < expectedHydrogenB) {
        return 1;
    }
    return 0;
}

int SortAtomsForMainGroupElementSymbols::compare(Atom *a, Atom *b) {
    int compare = a->getElement()
    ::compareTo(b->getElement());
    if (compare != 0) { //only bother comparing properly if elements are the same
        return compare;
    }

    int aExpectedHydrogen = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a);
    int bExpectedHydrogen = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(b);
    if (aExpectedHydrogen > 0 && bExpectedHydrogen == 0) { //having substitutable hydrogen preferred
        return -1;
    }
    if (aExpectedHydrogen == 0 && bExpectedHydrogen > 0) {
        return 1;
    }
    std::vector<std::wstring> locantsA = a->getLocants();
    std::vector<std::wstring> locantsB = b->getLocants();
    if (locantsA.empty() && locantsB.size() > 0) { //having no locants preferred
        return -1;
    }
    if (locantsA.size() > 0 && locantsB.empty()) {
        return 1;
    }
    return 0;
}

int FragmentTools::SortByLocants::compare(Atom *atoma, Atom *atomb) {
    if (atoma->getType() == XmlDeclarations::SUFFIX_TYPE_VAL &&
        atomb->getType() != XmlDeclarations::SUFFIX_TYPE_VAL) { //suffix atoms go to the back
        return 1;
    }
    if (atomb->getType() == XmlDeclarations::SUFFIX_TYPE_VAL && atoma->getType() != XmlDeclarations::SUFFIX_TYPE_VAL) {
        return -1;
    }

    std::wstring locanta = atoma->getFirstLocant();
    std::wstring locantb = atomb->getFirstLocant();
    if (locanta == L"" || locantb == L"") {
        return 0;
    }

    Matcher *m1 = locantSegmenter->matcher(locanta);
    Matcher *m2 = locantSegmenter->matcher(locantb);
    if (!m1->matches() || !m2->matches()) { //inappropriate locant
        return 0;
    }
    std::wstring locantaPrimes = m1->group(3);
    std::wstring locantbPrimes = m2->group(3);
    if (locantaPrimes.compare(locantbPrimes) >= 1) {
        return 1; //e.g. 1'' vs 1'
    } else if (locantbPrimes.compare(locantaPrimes) >= 1) {
        return -1; //e.g. 1' vs 1''
    } else {
        int locantaNumber = static_cast<Integer>(m1->group(1));
        int locantbNumber = static_cast<Integer>(m2->group(1));

        if (locantaNumber > locantbNumber) {
            return 1; //e.g. 3 vs 2 or 3a vs 2
        } else if (locantbNumber > locantaNumber) {
            return -1; //e.g. 2 vs 3 or 2 vs 3a
        } else {
            std::wstring locantaLetter = m1->group(2);
            std::wstring locantbLetter = m2->group(2);
            if (locantaLetter.compare(locantbLetter) >= 1) {
                return 1; //e.g. 1b vs 1a
            } else if (locantbLetter.compare(locantaLetter) >= 1) {
                return -1; //e.g. 1a vs 1b
            }
            return 0;
        }
    }
}

void FragmentTools::assignElementLocants(Fragment *suffixableFragment,
                                         std::vector<Fragment *> &suffixFragments) throw(StructureBuildingException) {

    std::unordered_map<std::wstring, int> elementCount; //keeps track of how many times each element has been seen
    Set < Atom * > *atomsToIgnore = std::unordered_set<Atom *>(); //atoms which already have a symbolic locant

    std::vector<Fragment *> allFragments(suffixFragments);
    allFragments.push_back(suffixableFragment);
    /*
     * First check whether any element locants have already been assigned, these will take precedence
     */
    for (auto fragment : allFragments) {
        std::vector<Atom *> atomList = fragment->getAtomList();
        for (auto atom : atomList) {
            std::vector<std::wstring> elementSymbolLocants = atom->getElementSymbolLocants();
            for (auto locant : elementSymbolLocants) {
                int primeCount = StringTools::countTerminalPrimes(locant);
                std::wstring element = locant.substr(0, locant.length() - primeCount);
                boost::optional<int> seenCount = elementCount[element];
                if (!seenCount || (seenCount < primeCount + 1)) {
                    elementCount[element] = primeCount + 1;
                }
                atomsToIgnore->add(atom);
            }
        }
    }

    {
        Set <std::wstring> *elementsToIgnore = elementCount.keySet();

        for (auto fragment : allFragments) {
            std::vector<Atom *> atomList = fragment->getAtomList();
            for (auto atom : atomList) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                if (elementsToIgnore->contains(atom->getElement()::toString()))
                {
                    atomsToIgnore->add(atom);
                }
            }
        }
    }

    std::wstring fragType = suffixableFragment->getType();
    if (fragType == XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL ||
        fragType == XmlDeclarations::CHALCOGENACIDSTEM_TYPE_VAL) {
        if (suffixFragments.size() != 0) {
            throw StructureBuildingException(L"No suffix fragments were expected to be present on non carboxylic acid");
        }
        processNonCarboxylicAcidLabelling(suffixableFragment, elementCount, atomsToIgnore);
    } else {
        if (suffixFragments.size() > 0) {
            processSuffixLabelling(suffixFragments, elementCount, atomsToIgnore);
            boost::optional<int> seenCount = elementCount[L"N"];
            if (seenCount && seenCount >
                             1) { //look for special case violation of IUPAC rule, =(N)=(NN) is N//N' in practice rather than N/N'/N''
                //this method will put both locants on the N with substituable hydrogen
                detectAndCorrectHydrazoneDerivativeViolation(suffixFragments);
            }
        }
        processMainGroupLabelling(suffixableFragment, elementCount, atomsToIgnore);
    }
}

void FragmentTools::detectAndCorrectHydrazoneDerivativeViolation(std::vector<Fragment *> &suffixFragments) {
    for (auto suffixFrag : suffixFragments) {
        std::vector<Atom *> atomList = suffixFrag->getAtomList();
        for (auto atom : atomList) {
            if (atom->getElement() == ChemEl::N && atom->getIncomingValency() == 3) {
                std::vector<std::wstring> locants = atom->getLocants();
                if (locants.size() == 1 && OpsinTools::MATCH_ELEMENT_SYMBOL_LOCANT->matcher(locants[0]).matches()) {
                    std::vector<Atom *> neighbours = atom->getAtomNeighbours();
                    for (auto neighbour : neighbours) {
                        if (neighbour->getElement() == ChemEl::N && neighbour->getIncomingValency() == 1) {
                            std::wstring locantToAdd = locants[0];
                            atom->clearLocants();
                            neighbour->addLocant(locantToAdd);
                            goto fragmentLoopContinue;
                        }
                    }
                }
            }
        }
        fragmentLoopContinue:
    }
    fragmentLoopBreak:
}

void FragmentTools::processMainGroupLabelling(Fragment *suffixableFragment,
                                              std::unordered_map<std::wstring, int> &elementCount,
                                              Set<Atom *> *atomsToIgnore) {
    Set <std::wstring> *elementToIgnore = std::unordered_set<std::wstring>(elementCount.keySet());
    std::vector<Atom *> atomList = suffixableFragment->getAtomList();
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(atomList, new SortAtomsForMainGroupElementSymbols());
    SortAtomsForMainGroupElementSymbols tempVar();
    std::sort(atomList.begin(), atomList.end(), &tempVar);
    Atom *atomToAddCLabelTo = nullptr; //only add a C label if there is only one C in the main group
    bool seenMoreThanOneC = false;
    for (auto atom : atomList) {
        if (atomsToIgnore->contains(atom)) {
            continue;
        }
        ChemEl chemEl = atom->getElement();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (elementToIgnore->contains(chemEl::toString())) {
            continue;
        }
        if (chemEl == ChemEl::C) {
            if (seenMoreThanOneC) {
                continue;
            }
            if (atomToAddCLabelTo != nullptr) {
                atomToAddCLabelTo = nullptr;
                seenMoreThanOneC = true;
            } else {
                atomToAddCLabelTo = atom;
            }
        } else {
            assignLocant(atom, elementCount);
        }
    }
    if (atomToAddCLabelTo != nullptr) {
        atomToAddCLabelTo->addLocant(L"C");
    }
}

void FragmentTools::processSuffixLabelling(std::vector<Fragment *> &suffixFragments,
                                           std::unordered_map<std::wstring, int> &elementCount,
                                           Set<Atom *> *atomsToIgnore) {
    std::vector<Atom *> startingAtoms;
    Set < Atom * > *atomsVisited = std::unordered_set<Atom *>();
    for (auto fragment : suffixFragments) {
        Atom *rAtom = fragment->getFirstAtom();
        std::vector<Atom *> nextAtoms = getIntraFragmentNeighboursAndSetVisitedBondOrder(rAtom);
        atomsVisited->addAll(nextAtoms);
        startingAtoms.insert(startingAtoms.end(), nextAtoms.begin(), nextAtoms.end());
    }
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(startingAtoms, new SortAtomsForElementSymbols());
    SortAtomsForElementSymbols tempVar();
    std::sort(startingAtoms.begin(), startingAtoms.end(), &tempVar);

    Deque < Atom * > *atomsToConsider = new ArrayDeque<Atom *>(startingAtoms);
    while (atomsToConsider->size() > 0) {
        assignLocantsAndExploreNeighbours(elementCount, atomsToIgnore, atomsVisited, atomsToConsider);
    }
}

void FragmentTools::processNonCarboxylicAcidLabelling(Fragment *suffixableFragment,
                                                      std::unordered_map<std::wstring, int> &elementCount,
                                                      Set<Atom *> *atomsToIgnore) {
    Set < Atom * > *atomsVisited = std::unordered_set<Atom *>();
    Atom *firstAtom = suffixableFragment->getFirstAtom();
    std::vector<Atom *> startingAtoms = getIntraFragmentNeighboursAndSetVisitedBondOrder(firstAtom);

//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(startingAtoms, new SortAtomsForElementSymbols());
    SortAtomsForElementSymbols tempVar();
    std::sort(startingAtoms.begin(), startingAtoms.end(), &tempVar);
    atomsVisited->add(firstAtom);
    Deque < Atom * > *atomsToConsider = new ArrayDeque<Atom *>(startingAtoms);
    while (atomsToConsider->size() > 0) {
        assignLocantsAndExploreNeighbours(elementCount, atomsToIgnore, atomsVisited, atomsToConsider);
    }
    if (!atomsToIgnore->contains(firstAtom) && firstAtom->determineValency(true) > firstAtom->getIncomingValency()) {
        //e.g. carbonimidoyl the carbon has locant C
        assignLocant(firstAtom, elementCount);
    }
}

void FragmentTools::assignLocantsAndExploreNeighbours(std::unordered_map<std::wstring, int> &elementCount,
                                                      Set<Atom *> *atomsToIgnore, Set<Atom *> *atomsVisited,
                                                      Deque<Atom *> *atomsToConsider) {
    Atom *atom = atomsToConsider->removeFirst();
    atomsVisited->add(atom);
    if (!atomsToIgnore->contains(atom)) { //assign locant
        assignLocant(atom, elementCount);
    }
    std::vector<Atom *> atomsToExplore = getIntraFragmentNeighboursAndSetVisitedBondOrder(atom);
    atomsToExplore.removeAll(atomsVisited);
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(atomsToExplore, new SortAtomsForElementSymbols());
    SortAtomsForElementSymbols tempVar();
    std::sort(atomsToExplore.begin(), atomsToExplore.end(), &tempVar);
    for (int i = atomsToExplore.size() - 1; i >= 0; i--) {
        atomsToConsider->addFirst(atomsToExplore[i]);
    }
}

std::vector<Atom *> FragmentTools::getIntraFragmentNeighboursAndSetVisitedBondOrder(Atom *atom) {
    std::vector<Atom *> atomsToExplore;
    std::vector<Bond *> bonds = atom->getBonds();
    for (auto bond : bonds) {
        Atom *neighbour = bond->getOtherAtom(atom);
        if (neighbour->getFrag()->equals(atom->getFrag())) {
            atomsToExplore.push_back(neighbour);
            neighbour->setProperty(Atom::VISITED, bond->getOrder());
        }
    }
    return atomsToExplore;
}

void FragmentTools::assignLocant(Atom *atom, std::unordered_map<std::wstring, int> &elementCount) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    std::wstring element = atom->getElement()
    ::toString();
    boost::optional<int> count = elementCount[element];
    if (!count) {
        atom->addLocant(element);
        elementCount[element] = 1;
    } else {
        atom->addLocant(element + StringTools::multiplyString(L"'", count));
        elementCount[element] = count + 1;
    }
}

Bond *FragmentTools::unsaturate(Atom *fromAtom, int bondOrder, Fragment *fragment) throw(StructureBuildingException) {
    Atom *toAtom = nullptr;
    boost::optional<int> locant = boost::none;
    try {
        std::wstring primes = L"";
        std::wstring locantStr = fromAtom->getFirstLocant();
        int numberOfPrimes = StringTools::countTerminalPrimes(locantStr);
        locant = std::stoi(locantStr.substr(0, locantStr.length() - numberOfPrimes));
        primes = StringTools::multiplyString(L"'", numberOfPrimes);
        Atom *possibleToAtom = fragment->getAtomByLocant(static_cast<std::wstring>(locant + 1) + primes);
        if (possibleToAtom != nullptr && fromAtom->getBondToAtom(possibleToAtom) != nullptr) {
            toAtom = possibleToAtom;
        } else if (possibleToAtom == nullptr &&
                   fromAtom->getAtomIsInACycle()) { //allow something like cyclohexan-6-ene, something like butan-4-ene will still fail
            possibleToAtom = fragment->getAtomByLocant(std::wstring(L"1") + primes);
            if (possibleToAtom != nullptr && fromAtom->getBondToAtom(possibleToAtom) != nullptr) {
                toAtom = possibleToAtom;
            }
        }
    }
    catch (const std::exception &e) {
        std::vector<Atom *> atomList = fragment->getAtomList();
        int initialIndice = atomList.find(fromAtom);
        if (initialIndice + 1 < atomList.size() && fromAtom->getBondToAtom(atomList[initialIndice + 1]) != nullptr) {
            toAtom = atomList[initialIndice + 1];
        }
    }
    if (toAtom == nullptr) {
        if (locant) {
            throw StructureBuildingException(
                    std::wstring(L"Could not find bond to unsaturate starting from the atom with locant: ") + locant);
        } else {
            throw StructureBuildingException(L"Could not find bond to unsaturate");
        }
    }
    Bond *b = fromAtom->getBondToAtomOrThrow(toAtom);
    b->setOrder(bondOrder);
    return b;
}

void FragmentTools::unsaturate(Atom *fromAtom, const std::wstring &locantTo, int bondOrder,
                               Fragment *fragment) throw(StructureBuildingException) {
    Atom *toAtom = fragment->getAtomByLocantOrThrow(locantTo);
    Bond *b = fromAtom->getBondToAtomOrThrow(toAtom);
    b->setOrder(bondOrder);
}

void FragmentTools::relabelLocantsAsFusedRingSystem(std::vector<Atom *> &atomList) {
    int locantVal = 0;
    wchar_t locantLetter = L'a';
    for (auto atom : atomList) {
        atom->clearLocants();
    }
    for (auto atom : atomList) {
        if (atom->getElement() != ChemEl::C || atom->getBondCount() < 3) {
            locantVal++;
            locantLetter = L'a';
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            atom->addLocant(Integer::toString(locantVal));
        } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            atom->addLocant(Integer::toString(locantVal) + locantLetter);
            locantLetter++;
        }
    }
}

void FragmentTools::relabelLocants(std::vector<Atom *> &atomList, const std::wstring &stringToAdd) {
    for (auto atom : atomList) {
        std::vector<std::wstring> locants(atom->getLocants());
        atom->clearLocants();
        for (auto locant : locants) {
            atom->addLocant(locant + stringToAdd);
        }
    }
}

void FragmentTools::relabelNumericLocants(std::vector<Atom *> &atomList, const std::wstring &stringToAdd) {
    for (auto atom : atomList) {
        std::vector<std::wstring> locants(atom->getLocants());
        for (auto locant : locants) {
            if (OpsinTools::MATCH_NUMERIC_LOCANT->matcher(locant).matches()) {
                atom->removeLocant(locant);
                atom->addLocant(locant + stringToAdd);
            }
        }
    }
}

void FragmentTools::splitOutAtomIntoValency1OutAtoms(OutAtom *outAtom) {
    Fragment *frag = outAtom->getAtom()->getFrag();
    for (int i = 1; i < outAtom->getValency(); i++) {
        frag->addOutAtom(outAtom->getAtom(), 1, outAtom->isSetExplicitly());
    }
    outAtom->setValency(1);
}

Atom *FragmentTools::detectSimpleNitrogenTautomer(Atom *nitrogen) {
    if (nitrogen->getElement() == ChemEl::N && nitrogen->getAtomIsInACycle()) {
        for (auto neighbour : nitrogen->getAtomNeighbours()) {
            if (neighbour->hasSpareValency() && neighbour->getElement() == ChemEl::C &&
                neighbour->getAtomIsInACycle()) {
                std::vector<Atom *> distance2Neighbours = neighbour->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                distance2Neighbours.remove(nitrogen);
                for (auto distance2Neighbour : distance2Neighbours) {
                    if (distance2Neighbour->hasSpareValency() && distance2Neighbour->getElement() == ChemEl::N &&
                        distance2Neighbour->getAtomIsInACycle() && distance2Neighbour->getCharge() == 0) {
                        return distance2Neighbour;
                    }
                }
            }
        }
    }
    return nullptr;
}

void FragmentTools::convertSpareValenciesToDoubleBonds(Fragment *frag) throw(StructureBuildingException) {
    std::vector<Atom *> atomCollection = frag->getAtomList();
    /* pick atom, getAtomNeighbours, decideIfTerminal, resolve */

    /*
     * Remove spare valency on atoms with valency precluding creation of double bonds
     */
    for (auto a : atomCollection) {
        a->ensureSVIsConsistantWithValency(true);
    }

    /*
     * Remove spare valency on atoms that are not adjacent to another atom with spare valency
     */
    for (auto a : atomCollection) {
        if (a->hasSpareValency()) {
            for (auto aa : frag->getIntraFragmentAtomNeighbours(a)) {
                if (aa->hasSpareValency()) {
                    goto atomLoopContinue;
                }
            }
            a->setSpareValency(false);
        }
        atomLoopContinue:
    }
    atomLoopBreak:

    /*
    *  The indicated hydrogen from the original SMILES definition of the fragment e.g. [nH] are used to disambiguate if there are
    *  an odd number of atoms with spare valency. Hence pyrrole is unambiguously 1H-pyrrole unless specified otherwise
    *  Things gets more complicated if the input contained multiple indicated hydrogen as it is unclear whether these still apply to the final molecule
    */
    Atom *atomToReduceValencyAt = nullptr;
    std::vector<Atom *> originalIndicatedHydrogen = frag->getIndicatedHydrogen();
    std::vector<Atom *> indicatedHydrogen(originalIndicatedHydrogen.size());
    for (auto atom : frag->getIndicatedHydrogen()) {
        if (atom->hasSpareValency() && atom->getCharge() == 0) {
            indicatedHydrogen.push_back(atom);
        }
    }
    if (indicatedHydrogen.size() > 0) {
        //typically there will be only one indicated hydrogen
        if (indicatedHydrogen.size() > 1) {
            for (auto indicatedAtom : indicatedHydrogen) {
                bool couldBeInvolvedInSimpleNitrogenTautomerism = false; //fix for guanine like purine derivatives
                if (indicatedAtom->getElement() == ChemEl::N && indicatedAtom->getAtomIsInACycle()) {
                    for (auto neighbour : indicatedAtom->getAtomNeighbours()) {
                        if (neighbour->getElement() == ChemEl::C && neighbour->getAtomIsInACycle()) {
                            std::vector<Atom *> distance2Neighbours = neighbour->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                            distance2Neighbours.remove(indicatedAtom);
                            for (auto distance2Neighbour : distance2Neighbours) {
                                if (distance2Neighbour->getElement() == ChemEl::N &&
                                    distance2Neighbour->getAtomIsInACycle() &&
                                    !std::find(originalIndicatedHydrogen.begin(), originalIndicatedHydrogen.end(),
                                               distance2Neighbour) != originalIndicatedHydrogen.end()) {
                                    couldBeInvolvedInSimpleNitrogenTautomerism = true;
                                    goto atomloopBreak;
                                }
                            }
                        }
                        atomloopContinue:
                    }
                    atomloopBreak:
                }
                //retain spare valency if has the cyclic [NH]C=N moiety but substitution has meant that this tautomerism doesn't actually occur cf. 8-oxoguanine
                if (!couldBeInvolvedInSimpleNitrogenTautomerism ||
                    detectSimpleNitrogenTautomer(indicatedAtom) != nullptr) {
                    indicatedAtom->setSpareValency(false);
                }
            }
        } else {
            atomToReduceValencyAt = indicatedHydrogen[0];
        }
    }

    int svCount = 0;
    for (auto a : atomCollection) {
        svCount += a->hasSpareValency() ? 1 : 0;
    }

    /*
     * Double-bonds go between pairs of atoms so if there are an off number of candidate atoms (e.g. pyrrole) an atom must be chosen
     * The atom with indicated hydrogen (see above) is used in preference else heuristics are used to chose a candidate
     */
    if ((svCount % 2) == 1) {
        if (atomToReduceValencyAt == nullptr) {
            atomToReduceValencyAt = findBestAtomToRemoveSpareValencyFrom(frag, atomCollection);
        }
        atomToReduceValencyAt->setSpareValency(false);
        svCount--;
    }

    while (svCount > 0) {
        bool foundTerminalFlag = false;
        bool foundNonBridgeHeadFlag = false;
        bool foundBridgeHeadFlag = false;
        //First handle cases where double bond placement is completely unambiguous i.e. an atom where only one neighbour has spare valency
        for (auto a : atomCollection) {
            if (a->hasSpareValency()) {
                int count = 0;
                for (auto aa : frag->getIntraFragmentAtomNeighbours(a)) {
                    if (aa->hasSpareValency()) {
                        count++;
                    }
                }
                if (count == 1) {
                    for (auto aa : frag->getIntraFragmentAtomNeighbours(a)) {
                        if (aa->hasSpareValency()) {
                            foundTerminalFlag = true;
                            a->setSpareValency(false);
                            aa->setSpareValency(false);
                            a->getBondToAtomOrThrow(aa)->addOrder(1);
                            svCount -= 2; //Two atoms where for one of them this bond is the only double bond it can possible form
                            break;
                        }
                    }
                }
            }
        }
        if (foundTerminalFlag) {
            continue;
        }

        //Find two atoms where one, or both, of them are not bridgeheads
        for (auto a : atomCollection) {
            std::vector<Atom *> neighbours = frag->getIntraFragmentAtomNeighbours(a);
            if (a->hasSpareValency() && neighbours.size() < 3) {
                for (auto aa : neighbours) {
                    if (aa->hasSpareValency()) {
                        foundNonBridgeHeadFlag = true;
                        a->setSpareValency(false);
                        aa->setSpareValency(false);
                        a->getBondToAtomOrThrow(aa)->addOrder(1);
                        svCount -= 2; //Two atoms where one of them is not a bridge head
                        break;
                    }
                }
            }
            if (foundNonBridgeHeadFlag) {
                break;
            }
        }
        if (foundNonBridgeHeadFlag) {
            continue;
        }

        //Find two atoms where both of them are bridgheads
        for (auto a : atomCollection) {
            std::vector<Atom *> neighbours = frag->getIntraFragmentAtomNeighbours(a);
            if (a->hasSpareValency()) {
                for (auto aa : neighbours) {
                    if (aa->hasSpareValency()) {
                        foundBridgeHeadFlag = true;
                        a->setSpareValency(false);
                        aa->setSpareValency(false);
                        a->getBondToAtomOrThrow(aa)->addOrder(1);
                        svCount -= 2; //Two atoms where both of them are a bridge head e.g. necessary for something like coronene
                        break;
                    }
                }
            }
            if (foundBridgeHeadFlag) {
                break;
            }
        }
        if (!foundBridgeHeadFlag) {
            throw StructureBuildingException(
                    L"Failed to assign all double bonds! (Check that indicated hydrogens have been appropriately specified)");
        }
    }
}

Atom *FragmentTools::findBestAtomToRemoveSpareValencyFrom(Fragment *frag, std::vector<Atom *> &atomCollection) {
    for (auto a : atomCollection) { //try and find an atom with SV that neighbours only one atom with SV
        if (a->hasSpareValency()) {
            int atomsWithSV = 0;
            for (auto aa : frag->getIntraFragmentAtomNeighbours(a)) {
                if (aa->hasSpareValency()) {
                    atomsWithSV++;
                }
            }
            if (atomsWithSV == 1) {
                return a;
            }
        }
    }
    for (auto a : atomCollection) { //try and find an atom with bridgehead atoms with SV on both sides c.f. phenoxastibinine == 10H-phenoxastibinine
        if (a->hasSpareValency()) {
            std::vector<Atom *> neighbours = frag->getIntraFragmentAtomNeighbours(a);
            if (neighbours.size() == 2) {
                for (auto aa : neighbours) {
                    if (frag->getIntraFragmentAtomNeighbours(aa).size() < 3) {
                        goto atomLoopContinue;
                    }
                }
                return a;
            }
        }
        atomLoopContinue:
    }
    atomLoopBreak:
    //Prefer nitrogen to carbon e.g. get NHC=C rather than N=CCH
    Atom *firstAtomWithSpareValency = nullptr;
    Atom *firstHeteroAtomWithSpareValency = nullptr;
    for (auto a : atomCollection) {
        if (a->hasSpareValency()) {
            if (a->getElement() != ChemEl::C) {
                if (a->getCharge() == 0) {
                    return a;
                }
                if (firstHeteroAtomWithSpareValency == nullptr) {
                    firstHeteroAtomWithSpareValency = a;
                }
            }
            if (firstAtomWithSpareValency == nullptr) {
                firstAtomWithSpareValency = a;
            }
        }
    }
    if (firstAtomWithSpareValency == nullptr) {
        throw std::invalid_argument("OPSIN Bug: No atom had spare valency!");
    }
    return firstHeteroAtomWithSpareValency != nullptr ? firstHeteroAtomWithSpareValency : firstAtomWithSpareValency;
}

Atom *FragmentTools::getAtomByAminoAcidStyleLocant(Atom *backboneAtom, const std::wstring &elementSymbol,
                                                   const std::wstring &primes) {
    //Search for appropriate atom by using the same algorithm as is used to assign locants initially

    std::vector<Atom *> startingAtoms;
    Set < Atom * > *atomsVisited = std::unordered_set<Atom *>();
    std::vector<Atom *> neighbours = getIntraFragmentNeighboursAndSetVisitedBondOrder(backboneAtom);
    for (auto neighbour : neighbours) {
        atomsVisited->add(neighbour);
        if (!neighbour->getType().equals(XmlDeclarations::SUFFIX_TYPE_VAL)) {
            for (auto neighbourLocant : neighbour->getLocants()) {
                if (OpsinTools::MATCH_NUMERIC_LOCANT->matcher(
                        neighbourLocant).matches()) { //gone to an inappropriate atom
                    goto mainLoopContinue;
                }
            }
        }
        startingAtoms.push_back(neighbour);
        mainLoopContinue:
    }
    mainLoopBreak:

//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(startingAtoms, new SortAtomsForElementSymbols());
    SortAtomsForElementSymbols tempVar();
    std::sort(startingAtoms.begin(), startingAtoms.end(), &tempVar);
    std::unordered_map<std::wstring, int> elementCount; //keeps track of how many times each element has been seen

    Deque < Atom * > *atomsToConsider = new ArrayDeque<Atom *>(startingAtoms);
    bool hydrazoneSpecialCase = false; //look for special case violation of IUPAC rule where the locant of the =N- atom is skipped. This flag is set when =N- is encountered
    while (atomsToConsider->size() > 0) {
        Atom *atom = atomsToConsider->removeFirst();
        atomsVisited->add(atom);
        int primesOnPossibleAtom = 0;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        std::wstring element = atom->getElement()
        ::toString();
        if (elementCount[element] == nullptr) {
            elementCount[element] = 1;
        } else {
            int count = elementCount[element];
            primesOnPossibleAtom = count;
            elementCount[element] = count + 1;
        }
        if (hydrazoneSpecialCase) {
            if (element == elementSymbol && primes.length() == primesOnPossibleAtom - 1) {
                return atom;
            }
            hydrazoneSpecialCase = false;
        }

        std::vector<Atom *> atomNeighbours = getIntraFragmentNeighboursAndSetVisitedBondOrder(atom);
        atomNeighbours.removeAll(atomsVisited);
        for (int i = atomNeighbours.size() - 1; i >= 0; i--) {
            Atom *neighbour = atomNeighbours[i];
            if (neighbour->getType() != XmlDeclarations::SUFFIX_TYPE_VAL) {
                for (auto neighbourLocant : neighbour->getLocants()) {
                    if (OpsinTools::MATCH_NUMERIC_LOCANT->matcher(
                            neighbourLocant).matches()) { //gone to an inappropriate atom
                        atomNeighbours.erase(atomNeighbours.begin() + i);
                        break;
                    }
                }
            }
        }
        if (atom->getElement() == ChemEl::N && atom->getIncomingValency() == 3 && atom->getCharge() == 0 &&
            atomNeighbours.size() == 1 && atomNeighbours[0]->getElement() == ChemEl::N) {
            hydrazoneSpecialCase = true;
        } else {
            if (element == elementSymbol) {
                if (primes.length() == primesOnPossibleAtom) {
                    return atom;
                }
            }
        }

//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(atomNeighbours, new SortAtomsForElementSymbols());
        SortAtomsForElementSymbols tempVar2();
        std::sort(atomNeighbours.begin(), atomNeighbours.end(), &tempVar2);
        for (int i = atomNeighbours.size() - 1; i >= 0; i--) {
            atomsToConsider->addFirst(atomNeighbours[i]);
        }
    }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (primes == L"" && backboneAtom->getElement()::toString()->equals(elementSymbol))
    { //maybe it meant the starting atom
        return backboneAtom;
    }
    return nullptr;
}

bool FragmentTools::isCovalent(ChemEl chemEl1, ChemEl chemEl2) {
    boost::optional<double> atom1Electrongegativity = AtomProperties::getPaulingElectronegativity(chemEl1);
    boost::optional<double> atom2Electrongegativity = AtomProperties::getPaulingElectronegativity(chemEl2);
    if (atom1Electrongegativity && atom2Electrongegativity) {
        double halfSum = (atom1Electrongegativity + atom2Electrongegativity) / 2;
        double difference = std::abs(atom1Electrongegativity - atom2Electrongegativity);
        if (halfSum < 1.6) {
            return false; //probably metallic
        }
        if (difference < 1.76 * halfSum - 3.03) {
            return true;
        }
    }
    return false;
}

bool FragmentTools::isCharacteristicAtom(Atom *atom) {
    if (atom->getType() == XmlDeclarations::SUFFIX_TYPE_VAL || (atom->getElement()::isChalcogen() &&
                                                                                  XmlDeclarations::HETEROSTEM_SUBTYPE_VAL !=
                                                                                  atom->getFrag()->getSubType() &&
                                                                                  atom->getIncomingValency() == 1 &&
                                                                                  atom->getOutValency() == 0 &&
                                                                                  atom->getCharge() == 0))
    {
        return true;
    }
    return isFunctionalAtomOrAldehyde(atom);
}

bool FragmentTools::isFunctionalAtomOrAldehyde(Atom *atom) {
    if (Boolean::TRUE::equals(
            atom->getProperty(Atom::ISALDEHYDE))) { //substituting an aldehyde would make it no longer an aldehyde
        return true;
    }
    return isFunctionalAtom(atom);
}

bool FragmentTools::isFunctionalAtom(Atom *atom) {
    ChemEl chemEl = atom->getElement();
    if (chemEl::isChalcogen()) { //potential chalcogen functional atom
        Fragment *frag = atom->getFrag();
        for (int i = 0, l = frag->getFunctionalAtomCount(); i < l; i++) {
            if (atom->equals(frag->getFunctionalAtom(i)->getAtom())) {
                return true;
            }
        }
    }
    return false;
}

bool FragmentTools::allAtomsInRingAreIdentical(Fragment *ring) {
    std::vector<Atom *> atomList = ring->getAtomList();
    Atom *firstAtom = atomList[0];
    ChemEl chemEl = firstAtom->getElement();
    int valency = firstAtom->getIncomingValency();
    bool spareValency = firstAtom->hasSpareValency();
    for (auto atom : atomList) {
        if (atom->getElement() != chemEl) {
            return false;
        }
        if (atom->getIncomingValency() != valency) {
            return false;
        }
        if (atom->hasSpareValency() != spareValency) {
            return false;
        }
    }
    return true;
}

void FragmentTools::removeTerminalAtom(BuildState *state, Atom *atomToRemove) {
    AtomParity *atomParity = atomToRemove->getAtomNeighbours()[0]->getAtomParity();
    if (atomParity != nullptr) { //replace reference to atom with reference to implicit hydrogen
        std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
        for (int i = 0; i < atomRefs4.size(); i++) {
            if (atomRefs4[i] == atomToRemove) {
                atomRefs4[i] = AtomParity::deoxyHydrogen;
                break;
            }
        }
    }
    state->fragManager->removeAtomAndAssociatedBonds(atomToRemove);
}

void FragmentTools::removeTerminalOxygen(BuildState *state, Atom *atom,
                                         int desiredBondOrder) throw(StructureBuildingException) {
    //TODO prioritise [N+][O-]
    std::vector<Atom *> neighbours = atom->getAtomNeighbours();
    for (auto neighbour : neighbours) {
        if (neighbour->getElement() == ChemEl::O && neighbour->getBondCount() == 1) {
            Bond *b = atom->getBondToAtomOrThrow(neighbour);
            if (b->getOrder() == desiredBondOrder && neighbour->getCharge() == 0) {
                FragmentTools::removeTerminalAtom(state, neighbour);
                if (atom->getLambdaConventionValency()) { //corrects valency for phosphin/arsin/stibin
                    atom->setLambdaConventionValency(atom->getLambdaConventionValency() - desiredBondOrder);
                }
                if (atom->getMinimumValency()) { //corrects valency for phosphin/arsin/stibin
                    atom->setMinimumValency(atom->getMinimumValency() - desiredBondOrder);
                }
                return;
            } else if (neighbour->getCharge() == -1 && b->getOrder() == 1 && desiredBondOrder == 2) {
                if (atom->getCharge() == 1 && atom->getElement() == ChemEl::N) {
                    FragmentTools::removeTerminalAtom(state, neighbour);
                    atom->neutraliseCharge();
                    return;
                }
            }
        }
    }
    if (desiredBondOrder == 2) {
        throw StructureBuildingException(
                L"Double bonded oxygen not found at suffix attachment position. Perhaps a suffix has been used inappropriately");
    } else if (desiredBondOrder == 1) {
        throw StructureBuildingException(
                L"Hydroxy oxygen not found at suffix attachment position. Perhaps a suffix has been used inappropriately");
    } else {
        throw StructureBuildingException(
                L"Suitable oxygen not found at suffix attachment position Perhaps a suffix has been used inappropriately");
    }

}

std::vector<Atom *> FragmentTools::findHydroxyLikeTerminalAtoms(std::vector<Atom *> &atoms, ChemEl chemEl) {
    std::vector<Atom *> matches;
    for (auto atom : atoms) {
        if (atom->getElement() == chemEl && atom->getIncomingValency() == 1 && atom->getOutValency() == 0 &&
            atom->getCharge() == 0) {
            matches.push_back(atom);
        }
    }
    return matches;
}

bool FragmentTools::notIn6MemberOrSmallerRing(Bond *bond) {
    Atom *fromAtom = bond->getFromAtom();
    Atom *toAtom = bond->getToAtom();
    if (fromAtom->getAtomIsInACycle() && toAtom->getAtomIsInACycle()) { //obviously both must be in rings
        //attempt to get from the fromAtom to the toAtom in 6 or fewer steps.
        std::vector<Atom *> visitedAtoms;
        Deque < Atom * >
        *atomsToInvestigate = new ArrayDeque<Atom *>(); //A queue is not used as I need to make sure that only up to depth 6 is investigated
        std::vector<Atom *> neighbours = fromAtom->getAtomNeighbours();
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
        neighbours.remove(toAtom);
        for (auto neighbour : neighbours) {
            atomsToInvestigate->add(neighbour);
        }
        visitedAtoms.push_back(fromAtom);
        for (int i = 0; i < 5; i++) { //up to 5 bonds from the neighbours of the fromAtom i.e. up to ring size 6
            if (atomsToInvestigate->isEmpty()) {
                break;
            }
            Deque < Atom * > *atomsToInvestigateNext = new ArrayDeque<Atom *>();
            while (!atomsToInvestigate->isEmpty()) {
                Atom *currentAtom = atomsToInvestigate->removeFirst();
                if (currentAtom == toAtom) {
                    return false;
                }
                visitedAtoms.push_back(currentAtom);
                neighbours = currentAtom->getAtomNeighbours();
                for (auto neighbour : neighbours) {
                    if (!std::find(visitedAtoms.begin(), visitedAtoms.end(), neighbour) != visitedAtoms.end() &&
                        neighbour->getAtomIsInACycle()) {
                        atomsToInvestigateNext->add(neighbour);
                    }
                }
            }
            atomsToInvestigate = atomsToInvestigateNext;
        }
    }
    return true;
}

std::vector<Atom *> FragmentTools::findHydroxyGroups(Fragment *frag) throw(StructureBuildingException) {
    std::vector<Atom *> hydroxyAtoms;
    std::vector<Atom *> atoms = frag->getAtomList();
    for (auto atom : atoms) {
        if (atom->getElement() == ChemEl::O && atom->getIncomingValency() == 1 && atom->getOutValency() == 0 &&
            atom->getCharge() == 0) {
            Atom *adjacentAtom = atom->getAtomNeighbours().get(0);
            std::vector<Atom *> neighbours = adjacentAtom->getAtomNeighbours();
            if (adjacentAtom->getElement() == ChemEl::C) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                neighbours.remove(atom);
                if (neighbours.size() >= 1 && neighbours[0]->getElement() == ChemEl::O &&
                    adjacentAtom->getBondToAtomOrThrow(neighbours[0])->getOrder() == 2) {
                    continue;
                }
                if (neighbours.size() >= 2 && neighbours[1]->getElement() == ChemEl::O &&
                    adjacentAtom->getBondToAtomOrThrow(neighbours[1])->getOrder() == 2) {
                    continue;
                }
                hydroxyAtoms.push_back(atom);
            }
        }
    }
    return hydroxyAtoms;
}

std::vector<Atom *> FragmentTools::findnAtomsForSubstitution(std::vector<Atom *> &atomList, Atom *preferredAtom,
                                                             int numberOfSubstitutionsRequired, int bondOrder,
                                                             bool takeIntoAccountOutValency) {
    int atomCount = atomList.size();
    int startingIndex = preferredAtom != nullptr ? atomList.find(preferredAtom) : 0;
    if (startingIndex < 0) {
        throw std::invalid_argument("OPSIN Bug: preferredAtom should be part of the list of atoms to search through");
    }
    CyclicAtomList *atoms = new CyclicAtomList(atomList,
                                               startingIndex - 1); //next() will retrieve the atom at the startingIndex
    std::vector<Atom *> substitutableAtoms;
    for (int i = 0;
         i < atomCount; i++) { //aromaticity preserved, standard valency assumed, characteristic atoms ignored
        Atom *atom = atoms->next();
        if (!FragmentTools::isCharacteristicAtom(atom) ||
            (numberOfSubstitutionsRequired == 1 && atom == preferredAtom)) {
            int currentExpectedValency = atom->determineValency(takeIntoAccountOutValency);
            int usedValency = atom->getIncomingValency() + (atom->hasSpareValency() ? 1 : 0) +
                              (takeIntoAccountOutValency ? atom->getOutValency() : 0);
            int timesAtomCanBeSubstitued = ((currentExpectedValency - usedValency) / bondOrder);
            for (int j = 1; j <= timesAtomCanBeSubstitued; j++) {
                substitutableAtoms.push_back(atom);
            }
        }
    }
    if (substitutableAtoms.size() >= numberOfSubstitutionsRequired) {
        return substitutableAtoms;
    }
    substitutableAtoms.clear();
    for (int i = 0; i < atomCount; i++) { //aromaticity preserved, standard valency assumed, functional suffixes ignored
        Atom *atom = atoms->next();
        if (!FragmentTools::isFunctionalAtomOrAldehyde(atom) ||
            (numberOfSubstitutionsRequired == 1 && atom == preferredAtom)) {
            int currentExpectedValency = atom->determineValency(takeIntoAccountOutValency);
            int usedValency = atom->getIncomingValency() + (atom->hasSpareValency() ? 1 : 0) +
                              (takeIntoAccountOutValency ? atom->getOutValency() : 0);
            int timesAtomCanBeSubstitued = ((currentExpectedValency - usedValency) / bondOrder);
            for (int j = 1; j <= timesAtomCanBeSubstitued; j++) {
                substitutableAtoms.push_back(atom);
            }
        }
    }
    if (substitutableAtoms.size() >= numberOfSubstitutionsRequired) {
        return substitutableAtoms;
    }
    substitutableAtoms.clear();

    for (int i = 0; i < atomCount; i++) { //aromaticity preserved, any sensible valency allowed, anything substitutable
        Atom *atom = atoms->next();
        boost::optional<int> maximumValency = ValencyChecker::getMaximumValency(atom);
        if (maximumValency) {
            int usedValency = atom->getIncomingValency() + (atom->hasSpareValency() ? 1 : 0) +
                              (takeIntoAccountOutValency ? atom->getOutValency() : 0);
            int timesAtomCanBeSubstitued = ((maximumValency - usedValency) / bondOrder);
            for (int j = 1; j <= timesAtomCanBeSubstitued; j++) {
                substitutableAtoms.push_back(atom);
            }
        } else {
            for (int j = 0; j < numberOfSubstitutionsRequired; j++) {
                substitutableAtoms.push_back(atom);
            }
        }
    }
    if (substitutableAtoms.size() >= numberOfSubstitutionsRequired) {
        return substitutableAtoms;
    }
    substitutableAtoms.clear();

    for (int i = 0; i < atomCount; i++) { //aromaticity dropped, any sensible valency allowed, anything substitutable
        Atom *atom = atoms->next();
        boost::optional<int> maximumValency = ValencyChecker::getMaximumValency(atom);
        if (maximumValency) {
            int usedValency = atom->getIncomingValency() + (takeIntoAccountOutValency ? atom->getOutValency() : 0);
            int timesAtomCanBeSubstitued = ((maximumValency - usedValency) / bondOrder);
            for (int j = 1; j <= timesAtomCanBeSubstitued; j++) {
                substitutableAtoms.push_back(atom);
            }
        } else {
            for (int j = 0; j < numberOfSubstitutionsRequired; j++) {
                substitutableAtoms.push_back(atom);
            }
        }
    }
    if (substitutableAtoms.size() >= numberOfSubstitutionsRequired) {
        return substitutableAtoms;
    }
    return nullptr;
}

std::vector<Atom *>
FragmentTools::findnAtomsForSubstitution(Fragment *frag, Atom *preferredAtom, int numberOfSubstitutionsRequired,
                                         int bondOrder, bool takeIntoAccountOutValency) {
    return findnAtomsForSubstitution(frag->getAtomList(), preferredAtom, numberOfSubstitutionsRequired, bondOrder,
                                     takeIntoAccountOutValency);
}

std::vector<Atom *>
FragmentTools::findnAtomsForSubstitution(Fragment *frag, int numberOfSubstitutionsRequired, int bondOrder) {
    return findnAtomsForSubstitution(frag->getAtomList(), frag->getDefaultInAtom(), numberOfSubstitutionsRequired,
                                     bondOrder, true);
}

std::vector<Atom *> FragmentTools::findSubstituableAtoms(Fragment *frag, int bondOrder) {
    std::vector<Atom *> potentialAtoms = findnAtomsForSubstitution(frag, 1, bondOrder);
    if (potentialAtoms.empty()) {
        return Collections::emptyList();
    }
    return potentialAtoms;
}

Atom *FragmentTools::lastNonSuffixCarbonWithSufficientValency(Fragment *conjunctiveFragment) {
    std::vector<Atom *> atomList = conjunctiveFragment->getAtomList();
    for (int i = atomList.size() - 1; i >= 0; i--) {
        Atom *a = atomList[i];
        if (a->getType() == XmlDeclarations::SUFFIX_TYPE_VAL) {
            continue;
        }
        if (a->getElement() != ChemEl::C) {
            continue;
        }
        if (ValencyChecker::checkValencyAvailableForBond(a, 1)) {
            return a;
        }
    }
    return nullptr;
}
