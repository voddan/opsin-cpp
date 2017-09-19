#include "StereochemistryHandler.h"
#include "BuildState.h"
#include "Atom.h"
#include "StereoAnalyser.h"
#include "Bond.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "XmlDeclarations.h"
#include "StereochemistryException.h"
#include "OpsinWarning.h"
#include "OpsinTools.h"
#include "Fragment.h"
#include "StructureBuildingMethods.h"
#include "WordType.h"
#include "ChemEl.h"
#include "BondStereo.h"
#include "CycleDetector.h"
#include "Ring.h"
#include "SSSRFinder.h"
#include "FusedRingNumberer.h"
#include "StringTools.h"
#include "AtomParity.h"
#include "FragmentTools.h"


using BondStereoValue = BondStereo::BondStereoValue;
using OpsinWarningType = OpsinWarning::OpsinWarningType;
using StereoBond = StereoAnalyser::StereoBond;
using StereoCentre = StereoAnalyser::StereoCentre;

//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

StereochemistryHandler::StereochemistryHandler(BuildState *state,
                                               std::unordered_map<Atom *, StereoCentre *> &atomStereoCentreMap,
                                               std::unordered_map<Bond *, StereoBond *> &bondStereoBondMap) : state(
        state), atomStereoCentreMap(atomStereoCentreMap), bondStereoBondMap(bondStereoBondMap),
                                                                                                              notExplicitlyDefinedStereoCentreMap(
                                                                                                                      std::unordered_map<Atom *, StereoCentre *>(
                                                                                                                              atomStereoCentreMap)),
                                                                                                              notExplicitlyDefinedStereoBondMap(
                                                                                                                      std::unordered_map<Bond *, StereoBond *>(
                                                                                                                              bondStereoBondMap)) {
}

void StereochemistryHandler::applyStereochemicalElements(
        std::vector<Element *> &stereoChemistryEls) throw(StructureBuildingException) {
    std::vector<Element *> locantedStereoChemistryEls;
    std::vector<Element *> unlocantedStereoChemistryEls;
    std::vector<Element *> carbohydrateStereoChemistryEls;
    for (auto stereoChemistryElement : stereoChemistryEls) {
        if (stereoChemistryElement->getAttributeValue(XmlDeclarations::LOCANT_ATR) != nullptr) {
            locantedStereoChemistryEls.push_back(stereoChemistryElement);
        } else if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL)) {
            carbohydrateStereoChemistryEls.push_back(stereoChemistryElement);
        } else {
            unlocantedStereoChemistryEls.push_back(stereoChemistryElement);
        }
    }
    //perform locanted before unlocanted to avoid unlocanted elements using the stereocentres a locanted element refers to
    for (auto stereochemistryEl : locantedStereoChemistryEls) {
        try {
            matchStereochemistryToAtomsAndBonds(stereochemistryEl);
        }
        catch (const StereochemistryException &e) {
            if (state->n2sConfig->warnRatherThanFailOnUninterpretableStereochemistry()) {
                state->addWarning(OpsinWarningType::STEREOCHEMISTRY_IGNORED, e->what());
            } else {
                throw e;
            }
        }
    }
    if (!carbohydrateStereoChemistryEls.empty()) {
        processCarbohydrateStereochemistry(carbohydrateStereoChemistryEls);
    }
    for (auto stereochemistryEl : unlocantedStereoChemistryEls) {
        try {
            matchStereochemistryToAtomsAndBonds(stereochemistryEl);
        }
        catch (const StereochemistryException &e) {
            if (state->n2sConfig->warnRatherThanFailOnUninterpretableStereochemistry()) {
                state->addWarning(OpsinWarningType::STEREOCHEMISTRY_IGNORED, e->what());
            } else {
                throw e;
            }
        }
    }
}

void StereochemistryHandler::removeRedundantStereoCentres(std::vector<Atom *> &atomsWithPreDefinedAtomParity,
                                                          std::vector<Bond *> &bondsWithPreDefinedBondStereo) {
    for (auto atom : atomsWithPreDefinedAtomParity) {
        if (atomStereoCentreMap.find(atom) == atomStereoCentreMap.end()) {
            atom->setAtomParity(nullptr);
        }
    }
    for (auto bond : bondsWithPreDefinedBondStereo) {
        if (bondStereoBondMap.find(bond) == bondStereoBondMap.end()) {
            bond->setBondStereo(nullptr);
        }
    }
}

void StereochemistryHandler::matchStereochemistryToAtomsAndBonds(
        Element *stereoChemistryEl) throw(StructureBuildingException, StereochemistryException) {
    std::wstring stereoChemistryType = stereoChemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR);
    if (stereoChemistryType == XmlDeclarations::R_OR_S_TYPE_VAL) {
        assignStereoCentre(stereoChemistryEl);
    } else if (stereoChemistryType == XmlDeclarations::E_OR_Z_TYPE_VAL) {
        assignStereoBond(stereoChemistryEl);
    } else if (stereoChemistryType == XmlDeclarations::CISORTRANS_TYPE_VAL) {
        if (!assignCisTransOnRing(stereoChemistryEl)) {
            assignStereoBond(stereoChemistryEl);
        }
    } else if (stereoChemistryType == XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL) {
        assignAlphaBetaXiStereochem(stereoChemistryEl);
    } else if (stereoChemistryType == XmlDeclarations::DLSTEREOCHEMISTRY_TYPE_VAL) {
        assignDlStereochem(stereoChemistryEl);
    } else if (stereoChemistryType == XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL) {
        throw StereochemistryException(
                stereoChemistryType + std::wstring(L" stereochemistry is not currently interpretable by OPSIN"));
    } else if (stereoChemistryType == XmlDeclarations::RELATIVECISTRANS_TYPE_VAL) {
        throw StereochemistryException(
                stereoChemistryType + std::wstring(L" stereochemistry is not currently interpretable by OPSIN"));
    } else if (stereoChemistryType == XmlDeclarations::AXIAL_TYPE_VAL) {
        throw StereochemistryException(
                stereoChemistryType + std::wstring(L" stereochemistry is not currently interpretable by OPSIN"));
    } else if (stereoChemistryType == XmlDeclarations::OPTICALROTATION_TYPE_VAL) {
        state->addWarning(OpsinWarningType::STEREOCHEMISTRY_IGNORED, std::wstring(
                L"Optical rotation cannot be algorithmically used to assign stereochemistry. This term was ignored: ") +
                                                                     stereoChemistryEl->getValue());
    } else {
        throw StructureBuildingException(std::wstring(L"Unexpected stereochemistry type: ") + stereoChemistryType);
    }
    stereoChemistryEl->detach();
}

void StereochemistryHandler::processCarbohydrateStereochemistry(
        std::vector<Element *> &carbohydrateStereoChemistryEls) throw(StructureBuildingException) {
    std::unordered_map<Element *, std::vector<Element *>> groupToStereochemEls;
    for (auto carbohydrateStereoChemistryEl : carbohydrateStereoChemistryEls) {
        Element *nextGroup = OpsinTools::getNextSibling(carbohydrateStereoChemistryEl, XmlDeclarations::GROUP_EL);
        if (nextGroup == nullptr || (XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMALDOSE_SUBTYPE_VAL !=
                                     nextGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) &&
                                     XmlDeclarations::SYSTEMATICCARBOHYDRATESTEMKETOSE_SUBTYPE_VAL !=
                                     nextGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR))) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Could not find carbohydrate chain stem to apply stereochemistry to");
            throw std::exception();
        }
        if (groupToStereochemEls[nextGroup].empty()) {
            groupToStereochemEls[nextGroup] = std::vector<Element *>();
        }
        groupToStereochemEls[nextGroup].push_back(carbohydrateStereoChemistryEl);
    }
    for (auto entry : groupToStereochemEls) {
        assignCarbohydratePrefixStereochem(entry.first, entry.second);
    }
}

void StereochemistryHandler::assignStereoCentre(
        Element *stereoChemistryEl) throw(StructureBuildingException, StereochemistryException) {
    //generally the LAST group in this list will be the appropriate group e.g. (5S)-5-ethyl-6-methylheptane where the heptane is the appropriate group
    //we use the same algorithm as for unlocanted substitution so as to deprecate assignment into brackets
    Element *parentSubBracketOrRoot = stereoChemistryEl->getParent();
    std::vector<Fragment *> possibleFragments = StructureBuildingMethods::findAlternativeFragments(
            parentSubBracketOrRoot);
    std::vector<Element *> adjacentGroupEls = OpsinTools::getDescendantElementsWithTagName(parentSubBracketOrRoot,
                                                                                           XmlDeclarations::GROUP_EL);
    for (int i = adjacentGroupEls.size() - 1; i >= 0; i--) {
        possibleFragments.push_back(adjacentGroupEls[i]->getFrag());
    }
    std::wstring locant = stereoChemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR);
    std::wstring rOrS = stereoChemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
    for (auto fragment : possibleFragments) {
        if (attemptAssignmentOfStereoCentreToFragment(fragment, rOrS, locant)) {
            return;
        }
    }
    Element *possibleWordParent = parentSubBracketOrRoot->getParent();
    if (possibleWordParent->getName() == XmlDeclarations::WORD_EL &&
        possibleWordParent->getChild(0)->equals(parentSubBracketOrRoot)) {
        //something like (3R,4R,5R)-ethyl 4-acetamido-5-amino-3-(pentan-3-yloxy)cyclohex-1-enecarboxylate
        //i.e. the stereochemistry is in a different word to what it is applied to
        std::vector<Element *> words = OpsinTools::getNextSiblingsOfType(possibleWordParent, XmlDeclarations::WORD_EL);
        for (auto word : words) {
            std::vector<Element *> possibleGroups = OpsinTools::getDescendantElementsWithTagName(word,
                                                                                                 XmlDeclarations::GROUP_EL);
            for (int i = possibleGroups.size() - 1; i >= 0; i--) {
                if (attemptAssignmentOfStereoCentreToFragment(possibleGroups[i]->getFrag(), rOrS, locant)) {
                    return;
                }
            }
        }
    }
    throw StereochemistryException(std::wstring(L"Could not find atom that: ") + stereoChemistryEl->toXML() +
                                   std::wstring(L" appeared to be referring to"));
}

bool StereochemistryHandler::attemptAssignmentOfStereoCentreToFragment(Fragment *fragment, const std::wstring &rOrS,
                                                                       const std::wstring &locant) throw(StereochemistryException, StructureBuildingException) {
    if (locant == L"") { //undefined locant
        std::vector<Atom *> atomList = fragment->getAtomList();
        for (auto potentialStereoAtom : atomList) {
            if (notExplicitlyDefinedStereoCentreMap.find(potentialStereoAtom) !=
                notExplicitlyDefinedStereoCentreMap.end()) {
                applyStereoChemistryToStereoCentre(potentialStereoAtom,
                                                   notExplicitlyDefinedStereoCentreMap[potentialStereoAtom], rOrS);
                notExplicitlyDefinedStereoCentreMap.erase(potentialStereoAtom);
                return true;
            }
        }
    } else {
        Atom *potentialStereoAtom = fragment->getAtomByLocant(locant);
        if (potentialStereoAtom != nullptr && notExplicitlyDefinedStereoCentreMap.find(potentialStereoAtom) !=
                                              notExplicitlyDefinedStereoCentreMap.end()) {
            applyStereoChemistryToStereoCentre(potentialStereoAtom,
                                               notExplicitlyDefinedStereoCentreMap[potentialStereoAtom], rOrS);
            notExplicitlyDefinedStereoCentreMap.erase(potentialStereoAtom);
            return true;
        }
    }
    return false;
}

void StereochemistryHandler::applyStereoChemistryToStereoCentre(Atom *atom, StereoCentre *stereoCentre,
                                                                const std::wstring &rOrS) throw(StructureBuildingException, StereochemistryException) {
    std::vector<Atom *> cipOrderedAtoms = stereoCentre->getCipOrderedAtoms();
    if (cipOrderedAtoms.size() != 4) {
        throw StructureBuildingException(L"Only tetrahedral chirality is currently supported");
    }
    std::vector<Atom *> atomRefs4(4);
    atomRefs4[0] = cipOrderedAtoms[cipOrderedAtoms.size() - 1];
    for (int i = 0;
         i < cipOrderedAtoms.size() - 1; i++) { //from highest to lowest (true for S) hence atomParity 1 for S
        atomRefs4[i + 1] = cipOrderedAtoms[i];
    }
    if (rOrS == L"R") {
        atom->setAtomParity(atomRefs4, -1);
    } else if (rOrS == L"S") {
        atom->setAtomParity(atomRefs4, 1);
    } else if (rOrS == L"RS" || rOrS == L"SR") {
        atom->setAtomParity(nullptr);
    } else {
        throw StructureBuildingException(std::wstring(L"Unexpected stereochemistry type: ") + rOrS);
    }
}

void StereochemistryHandler::assignStereoBond(
        Element *stereoChemistryEl) throw(StructureBuildingException, StereochemistryException) {
    //generally the LAST group in this list will be the appropriate groups e.g. (2Z)-5-ethyl-6-methylhex-2-ene where the hex-2-ene is the appropriate group
    //we use the same algorithm as for unlocanted substitution so as to deprecate assignment into brackets
    Element *parentSubBracketOrRoot = stereoChemistryEl->getParent();
    std::vector<Fragment *> possibleFragments = StructureBuildingMethods::findAlternativeFragments(
            parentSubBracketOrRoot);
    std::vector<Element *> adjacentGroupEls = OpsinTools::getDescendantElementsWithTagName(parentSubBracketOrRoot,
                                                                                           XmlDeclarations::GROUP_EL);
    for (int i = adjacentGroupEls.size() - 1; i >= 0; i--) {
        possibleFragments.push_back(adjacentGroupEls[i]->getFrag());
    }
    std::wstring locant = stereoChemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR);
    std::wstring eOrZ = stereoChemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
    bool isCisTrans = false;
    if (stereoChemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::CISORTRANS_TYPE_VAL) {
        isCisTrans = true;
        std::wstring cisOrTrans = stereoChemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
        if (cisOrTrans.equalsIgnoreCase(L"cis")) {
            eOrZ = L"Z";
        } else if (cisOrTrans.equalsIgnoreCase(L"trans")) {
            eOrZ = L"E";
        } else {
            throw StructureBuildingException(std::wstring(L"Unexpected cis/trans stereochemistry type: ") + cisOrTrans);
        }
    }
    for (auto fragment : possibleFragments) {
        if (attemptAssignmentOfStereoBondToFragment(fragment, eOrZ, locant, isCisTrans)) {
            return;
        }
    }
    Element *possibleWordParent = parentSubBracketOrRoot->getParent();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (possibleWordParent->getName() == XmlDeclarations::WORD_EL &&
        possibleWordParent->getAttributeValue(XmlDeclarations::TYPE_ATR) == WordType::substituent->toString()) {
        //the element is in front of a substituent and may refer to the full group
        //i.e. the stereochemistry is in a different word to what it is applied to
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        std::vector<Element *> words = OpsinTools::getChildElementsWithTagNameAndAttribute(
                possibleWordParent->getParent(), XmlDeclarations::WORD_EL, XmlDeclarations::TYPE_ATR,
                WordType::full->toString());
        for (auto word : words) {
            std::vector<Element *> possibleGroups = OpsinTools::getDescendantElementsWithTagName(word,
                                                                                                 XmlDeclarations::GROUP_EL);
            for (int i = possibleGroups.size() - 1; i >= 0; i--) {
                if (attemptAssignmentOfStereoBondToFragment(possibleGroups[i]->getFrag(), eOrZ, locant, isCisTrans)) {
                    return;
                }
            }
        }
    }
    if (isCisTrans) {
        throw StereochemistryException(std::wstring(L"Could not find bond that: ") + stereoChemistryEl->toXML() +
                                       std::wstring(L" could refer unambiguously to"));
    } else {
        throw StereochemistryException(std::wstring(L"Could not find bond that: ") + stereoChemistryEl->toXML() +
                                       std::wstring(L" was referring to"));
    }
}

bool StereochemistryHandler::attemptAssignmentOfStereoBondToFragment(Fragment *fragment, const std::wstring &eOrZ,
                                                                     const std::wstring &locant,
                                                                     bool isCisTrans) throw(StereochemistryException) {
    if (locant == L"") { //undefined locant
        Set < Bond * > *bondSet = fragment->getBondSet();
        for (auto potentialBond : bondSet) {
            if (notExplicitlyDefinedStereoBondMap.find(potentialBond) != notExplicitlyDefinedStereoBondMap.end() &&
                (!isCisTrans || cisTransUnambiguousOnBond(potentialBond))) {
                applyStereoChemistryToStereoBond(potentialBond, notExplicitlyDefinedStereoBondMap[potentialBond], eOrZ);
                notExplicitlyDefinedStereoBondMap.erase(potentialBond);
                return true;
            }
        }
        std::vector<Bond *> sortedInterFragmentBonds = sortInterFragmentBonds(
                state->fragManager->getInterFragmentBonds(fragment), fragment);
        for (auto potentialBond : sortedInterFragmentBonds) {
            if (notExplicitlyDefinedStereoBondMap.find(potentialBond) != notExplicitlyDefinedStereoBondMap.end() &&
                (!isCisTrans || cisTransUnambiguousOnBond(potentialBond))) {
                applyStereoChemistryToStereoBond(potentialBond, notExplicitlyDefinedStereoBondMap[potentialBond], eOrZ);
                notExplicitlyDefinedStereoBondMap.erase(potentialBond);
                return true;
            }
        }
    } else {
        Atom *firstAtomInBond = fragment->getAtomByLocant(locant);
        if (firstAtomInBond != nullptr) {
            std::vector<Bond *> bonds = firstAtomInBond->getBonds();
            for (auto potentialBond : bonds) {
                if (notExplicitlyDefinedStereoBondMap.find(potentialBond) != notExplicitlyDefinedStereoBondMap.end() &&
                    (!isCisTrans || cisTransUnambiguousOnBond(potentialBond))) {
                    applyStereoChemistryToStereoBond(potentialBond, notExplicitlyDefinedStereoBondMap[potentialBond],
                                                     eOrZ);
                    notExplicitlyDefinedStereoBondMap.erase(potentialBond);
                    return true;
                }
            }
        }
    }
    return false;
}

bool StereochemistryHandler::cisTransUnambiguousOnBond(Bond *potentialBond) {
    std::vector<Atom *> neighbours1 = potentialBond->getFromAtom()->getAtomNeighbours();
    bool foundHydrogen1 = false;
    for (auto neighbour : neighbours1) {
        if (neighbour->getElement() == ChemEl::H) {
            foundHydrogen1 = true;
        }
    }

    std::vector<Atom *> neighbours2 = potentialBond->getToAtom()->getAtomNeighbours();
    bool foundHydrogen2 = false;
    for (auto neighbour : neighbours2) {
        if (neighbour->getElement() == ChemEl::H) {
            foundHydrogen2 = true;
        }
    }
    return (foundHydrogen1 && foundHydrogen2);
}

std::vector<Bond *> StereochemistryHandler::sortInterFragmentBonds(Set < Bond * > *interFragmentBonds,
                                                                   Fragment * preferredOriginatingFragment) {
    std::vector<Bond *> interFragmentBondList;
    for (auto bond : interFragmentBonds) {
        if (bond->getFromAtom()->getFrag() == preferredOriginatingFragment) {
            interFragmentBondList.push_back(0, bond);
        } else {
            interFragmentBondList.push_back(bond);
        }
    }
    return interFragmentBondList;
}

void StereochemistryHandler::applyStereoChemistryToStereoBond(Bond *bond, StereoBond *stereoBond,
                                                              const std::wstring &eOrZ) throw(StereochemistryException) {
    std::vector<Atom *> stereoBondAtoms = stereoBond->getOrderedStereoAtoms();
    //stereoBondAtoms contains the higher priority atom at one end, the two bond atoms and the higher priority atom at the other end
    std::vector<Atom *> atomRefs4(4);
    atomRefs4[0] = stereoBondAtoms[0];
    atomRefs4[1] = stereoBondAtoms[1];
    atomRefs4[2] = stereoBondAtoms[2];
    atomRefs4[3] = stereoBondAtoms[3];
    if (eOrZ == L"E") {
        bond->setBondStereoElement(atomRefs4, BondStereoValue::TRANS);
    } else if (eOrZ == L"Z") {
        bond->setBondStereoElement(atomRefs4, BondStereoValue::CIS);
    } else if (eOrZ == L"EZ") {
        bond->setBondStereo(nullptr);
    } else {
        throw std::invalid_argument(std::wstring(L"Unexpected stereochemistry type: ") + eOrZ);
    }
}

bool StereochemistryHandler::assignCisTransOnRing(Element *stereoChemistryEl) throw(StructureBuildingException) {
    if (stereoChemistryEl->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr) {
        return false;
    }
    Element *parentSubBracketOrRoot = stereoChemistryEl->getParent();
    std::vector<Fragment *> possibleFragments = StructureBuildingMethods::findAlternativeFragments(
            parentSubBracketOrRoot);
    std::vector<Element *> adjacentGroupEls = OpsinTools::getDescendantElementsWithTagName(parentSubBracketOrRoot,
                                                                                           XmlDeclarations::GROUP_EL);
    for (int i = adjacentGroupEls.size() - 1; i >= 0; i--) {
        possibleFragments.push_back(adjacentGroupEls[i]->getFrag());
    }
    for (auto fragment : possibleFragments) {
        if (attemptAssignmentOfCisTransRingStereoToFragment(fragment, stereoChemistryEl)) {
            return true;
        }
    }

    Element *possibleWordParent = parentSubBracketOrRoot->getParent();
    if (possibleWordParent->getName() == XmlDeclarations::WORD_EL &&
        possibleWordParent->getChild(0)->equals(parentSubBracketOrRoot)) {
        //stereochemistry is in a different word to what it is applied to
        std::vector<Element *> words = OpsinTools::getNextSiblingsOfType(possibleWordParent, XmlDeclarations::WORD_EL);
        for (auto word : words) {
            std::vector<Element *> possibleGroups = OpsinTools::getDescendantElementsWithTagName(word,
                                                                                                 XmlDeclarations::GROUP_EL);
            for (int i = possibleGroups.size() - 1; i >= 0; i--) {
                if (attemptAssignmentOfCisTransRingStereoToFragment(possibleGroups[i]->getFrag(), stereoChemistryEl)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool StereochemistryHandler::attemptAssignmentOfCisTransRingStereoToFragment(Fragment *fragment,
                                                                             Element *stereoChemistryEl) throw(StructureBuildingException) {
    std::vector<Atom *> atomList = fragment->getAtomList();
    std::vector<Atom *> stereoAtoms;
    for (auto potentialStereoAtom : atomList) {
        if (potentialStereoAtom->getAtomIsInACycle()) {
            std::vector<Atom *> neighbours = potentialStereoAtom->getAtomNeighbours();
            if (neighbours.size() == 4) {
                int hydrogenCount = 0;
                int acylicOrNotInFrag = 0;
                for (auto neighbour : neighbours) {
                    if (neighbour->getElement() == ChemEl::H) {
                        hydrogenCount++;
                    }
                    if (!neighbour->getAtomIsInACycle() ||
                        !std::find(atomList.begin(), atomList.end(), neighbour) != atomList.end()) {
                        acylicOrNotInFrag++;
                    }
                }
                if (hydrogenCount == 1 || (hydrogenCount == 0 && acylicOrNotInFrag == 1)) {
                    stereoAtoms.push_back(potentialStereoAtom);
                }
            }
        }
    }
    if (stereoAtoms.size() == 2) {
        Atom *a1 = stereoAtoms[0];
        Atom *a2 = stereoAtoms[1];

        if (a1->getAtomParity() != nullptr &&
            a2->getAtomParity() != nullptr) { //one can have defined stereochemistry but not both
            return false;
        }

        Set < Bond * > *peripheryBonds = determinePeripheryBonds(fragment);
        std::vector<std::vector<Atom *>> paths = CycleDetector::getPathBetweenAtomsUsingBonds(a1, a2, peripheryBonds);
        if (paths.size() != 2) {
            return false;
        }
        applyStereoChemistryToCisTransOnRing(a1, a2, paths, atomList,
                                             stereoChemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
        notExplicitlyDefinedStereoCentreMap.erase(stereoAtoms[0]);
        notExplicitlyDefinedStereoCentreMap.erase(stereoAtoms[1]);
        return true;
    }
    return false;
}

Set<Bond *> *StereochemistryHandler::determinePeripheryBonds(Fragment *fragment) {
    std::vector<Ring *> rings = SSSRFinder::getSetOfSmallestRings(fragment);
    FusedRingNumberer::setupAdjacentFusedRingProperties(rings);
    Set < Bond * > *bondsToConsider = std::unordered_set<Bond *>();
    for (auto ring : rings) {
        for (auto bond : ring->getBondList()) {
            bondsToConsider->add(bond);
        }
    }
    for (auto ring : rings) {
        bondsToConsider->removeAll(ring->getFusedBonds());
    }
    return bondsToConsider;
}

void StereochemistryHandler::applyStereoChemistryToCisTransOnRing(Atom *a1, Atom *a2,
                                                                  std::vector<std::vector<Atom *>> &paths,
                                                                  std::vector<Atom *> &fragmentAtoms,
                                                                  const std::wstring &cisOrTrans) throw(StructureBuildingException) {
    std::vector<Atom *> a1Neighbours = a1->getAtomNeighbours();
    std::vector<Atom *> atomRefs4a1(4);
    Atom *firstPathAtom = paths[0].size() > 0 ? paths[0][0] : a2;
    atomRefs4a1[2] = firstPathAtom;
    Atom *secondPathAtom = paths[1].size() > 0 ? paths[1][0] : a2;
    atomRefs4a1[3] = secondPathAtom;
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    a1Neighbours.remove(firstPathAtom);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    a1Neighbours.remove(secondPathAtom);
    if (firstPathAtom->equals(secondPathAtom)) {
        throw StructureBuildingException(L"OPSIN Bug: cannot assign cis/trans on ring stereochemistry");
    }
    atomRefs4a1[1] = getHydrogenOrAcyclicOrOutsideOfFragment(a1Neighbours, fragmentAtoms);
    if (atomRefs4a1[1] == nullptr) {
        throw StructureBuildingException(L"OPSIN Bug: cannot assign cis/trans on ring stereochemistry");
    }
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    a1Neighbours.remove(atomRefs4a1[1]);
    atomRefs4a1[0] = a1Neighbours[0];


    std::vector<Atom *> a2Neighbours = a2->getAtomNeighbours();
    std::vector<Atom *> atomRefs4a2(4);
    firstPathAtom = paths[0].size() > 0 ? paths[0][paths[0].size() - 1] : a1;
    atomRefs4a2[2] = firstPathAtom;
    secondPathAtom = paths[1].size() > 0 ? paths[1][paths[1].size() - 1] : a1;
    atomRefs4a2[3] = secondPathAtom;
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    a2Neighbours.remove(firstPathAtom);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    a2Neighbours.remove(secondPathAtom);
    if (firstPathAtom->equals(secondPathAtom)) {
        throw StructureBuildingException(L"OPSIN Bug: cannot assign cis/trans on ring stereochemistry");
    }
    atomRefs4a2[1] = getHydrogenOrAcyclicOrOutsideOfFragment(a2Neighbours, fragmentAtoms);
    if (atomRefs4a2[1] == nullptr) {
        throw StructureBuildingException(L"OPSIN Bug: cannot assign cis/trans on ring stereochemistry");
    }
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    a2Neighbours.remove(atomRefs4a2[1]);
    atomRefs4a2[0] = a2Neighbours[0];
    bool enantiomer = false;
    if (a1->getAtomParity() != nullptr) {
        if (!checkEquivalencyOfAtomsRefs4AndParity(atomRefs4a1, 1, a1->getAtomParity()->getAtomRefs4(),
                                                   a1->getAtomParity()->getParity())) {
            enantiomer = true;
        }
    } else if (a2->getAtomParity() != nullptr) {
        if (cisOrTrans == L"cis") {
            if (!checkEquivalencyOfAtomsRefs4AndParity(atomRefs4a2, -1, a2->getAtomParity()->getAtomRefs4(),
                                                       a2->getAtomParity()->getParity())) {
                enantiomer = true;
            }
        } else if (cisOrTrans == L"trans") {
            if (!checkEquivalencyOfAtomsRefs4AndParity(atomRefs4a2, 1, a2->getAtomParity()->getAtomRefs4(),
                                                       a2->getAtomParity()->getParity())) {
                enantiomer = true;
            }
        }
    }
    if (enantiomer) {
        if (cisOrTrans == L"cis") {
            a1->setAtomParity(atomRefs4a1, -1);
            a2->setAtomParity(atomRefs4a2, 1);
        } else if (cisOrTrans == L"trans") {
            a1->setAtomParity(atomRefs4a1, -1);
            a2->setAtomParity(atomRefs4a2, -1);
        }
    } else {
        if (cisOrTrans == L"cis") {
            a1->setAtomParity(atomRefs4a1, 1);
            a2->setAtomParity(atomRefs4a2, -1);
        } else if (cisOrTrans == L"trans") {
            a1->setAtomParity(atomRefs4a1, 1);
            a2->setAtomParity(atomRefs4a2, 1);
        }
    }
}

Atom *StereochemistryHandler::getHydrogenOrAcyclicOrOutsideOfFragment(std::vector<Atom *> &atoms,
                                                                      std::vector<Atom *> &fragmentAtoms) {
    for (auto atom : atoms) {
        if (atom->getElement() == ChemEl::H) {
            return atom;
        }
    }
    for (auto atom : atoms) {
        if (!atom->getAtomIsInACycle() ||
            !std::find(fragmentAtoms.begin(), fragmentAtoms.end(), atom) != fragmentAtoms.end()) {
            return atom;
        }
    }
    return nullptr;
}

void StereochemistryHandler::assignAlphaBetaXiStereochem(Element *stereoChemistryEl) throw(StructureBuildingException) {
    Element *parentSubBracketOrRoot = stereoChemistryEl->getParent();
    std::vector<Fragment *> possibleFragments = StructureBuildingMethods::findAlternativeFragments(
            parentSubBracketOrRoot);
    Fragment *substituentGroup = nullptr;
    if (parentSubBracketOrRoot->getName() == XmlDeclarations::SUBSTITUENT_EL) {
        substituentGroup = parentSubBracketOrRoot->getFirstChildElement(XmlDeclarations::GROUP_EL)->getFrag();
    }
    std::vector<Element *> adjacentGroupEls = OpsinTools::getDescendantElementsWithTagName(parentSubBracketOrRoot,
                                                                                           XmlDeclarations::GROUP_EL);
    for (int i = adjacentGroupEls.size() - 1; i >= 0; i--) {
        possibleFragments.push_back(adjacentGroupEls[i]->getFrag());
    }
    std::wstring locant = stereoChemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR);
    std::wstring alphaOrBeta = stereoChemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
    for (auto fragment : possibleFragments) {
        Atom *potentialStereoAtom = fragment->getAtomByLocant(locant);
        if (potentialStereoAtom != nullptr && atomStereoCentreMap.find(potentialStereoAtom) !=
                                              atomStereoCentreMap.end()) { //same stereocentre can be defined twice e.g. one subsituent alpha the other beta
            if (alphaOrBeta == L"xi") {
                potentialStereoAtom->setAtomParity(nullptr);
            } else {
                std::wstring alphaBetaClockWiseAtomOrdering = fragment->getTokenEl()->getAttributeValue(
                        XmlDeclarations::ALPHABETACLOCKWISEATOMORDERING_ATR);
                if (alphaBetaClockWiseAtomOrdering == L"") {
                    throw StructureBuildingException(
                            L"Identified fragment is not known to be able to support alpha/beta stereochemistry");
                }
                applyAlphaBetaStereochemistryToStereoCentre(potentialStereoAtom, fragment,
                                                            alphaBetaClockWiseAtomOrdering, alphaOrBeta,
                                                            substituentGroup);
            }
            notExplicitlyDefinedStereoCentreMap.erase(potentialStereoAtom);
            return;
        }
    }
    throw StructureBuildingException(std::wstring(L"Could not find atom that: ") + stereoChemistryEl->toXML() +
                                     std::wstring(L" appeared to be referring to"));
}

void StereochemistryHandler::applyAlphaBetaStereochemistryToStereoCentre(Atom *stereoAtom, Fragment *fragment,
                                                                         const std::wstring &alphaBetaClockWiseAtomOrdering,
                                                                         const std::wstring &alphaOrBeta,
                                                                         Fragment *substituentGroup) throw(StructureBuildingException) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> ringOrder = StringTools::arrayToList(alphaBetaClockWiseAtomOrdering.split(L"/"));
    int positionInList = ringOrder.find(stereoAtom->getFirstLocant());
    if (stereoAtom->getAtomIsInACycle() && positionInList != -1) {
        std::vector<Atom *> atomRefs4(4);
        std::vector<Atom *> neighbours = stereoAtom->getAtomNeighbours();
        if (neighbours.size() == 4) {
            int previousIndice = positionInList == 0 ? ringOrder.size() - 1 : positionInList - 1;
            int nextindice = positionInList == ringOrder.size() - 1 ? 0 : positionInList + 1;
            atomRefs4[0] = fragment->getAtomByLocantOrThrow(ringOrder[previousIndice]);
            atomRefs4[3] = fragment->getAtomByLocantOrThrow(ringOrder[nextindice]);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            neighbours.remove(atomRefs4[0]);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            neighbours.remove(atomRefs4[3]);
            Atom *a1 = neighbours[0];
            Atom *a2 = neighbours[1];
            if ((std::find(fragment->getAtomList().begin(), fragment->getAtomList().end(), a1) !=
                 fragment->getAtomList().end() &&
                 std::find(ringOrder.begin(), ringOrder.end(), a1->getFirstLocant()) != ringOrder.end())))
            {
                atomRefs4[1] = a1;
                atomRefs4[2] = a2;
            }
            else if ((std::find(fragment->getAtomList().begin(), fragment->getAtomList().end(), a2) !=
                      fragment->getAtomList().end() &&
                      std::find(ringOrder.begin(), ringOrder.end(), a2->getFirstLocant()) != ringOrder.end())))
            {
                atomRefs4[1] = a2;
                atomRefs4[2] = a1;
            }
            else if (a1->getElement() == ChemEl::H && a2->getElement() != ChemEl::H) {
                atomRefs4[1] = a2;
                atomRefs4[2] = a1;
            } else if (a2->getElement() == ChemEl::H && a1->getElement() != ChemEl::H) {
                atomRefs4[1] = a1;
                atomRefs4[2] = a2;
            } //TODO support case where alpha/beta are applied prior to a suffix (and the stereocentre doesn't have a hydrogen) e.g. 17alpha-yl
            else if (substituentGroup != nullptr && fragment != substituentGroup &&
                     std::find(substituentGroup->getAtomList().begin(), substituentGroup->getAtomList().end(), a1) !=
                     substituentGroup->getAtomList().end()) {
                atomRefs4[1] = a1;
                atomRefs4[2] = a2;
            } else if (substituentGroup != nullptr && fragment != substituentGroup &&
                       std::find(substituentGroup->getAtomList().begin(), substituentGroup->getAtomList().end(), a2) !=
                       substituentGroup->getAtomList().end()) {
                atomRefs4[1] = a2;
                atomRefs4[2] = a1;
            } else {
                throw StructureBuildingException(
                        std::wstring(L"alpha/beta stereochemistry could not be determined at position ") +
                        stereoAtom->getFirstLocant());
            }
            AtomParity *previousAtomParity = stereoAtom->getAtomParity();
            if (alphaOrBeta == L"alpha") {
                stereoAtom->setAtomParity(atomRefs4, 1);
            } else if (alphaOrBeta == L"beta") {
                stereoAtom->setAtomParity(atomRefs4, -1);
            } else {
                throw StructureBuildingException(L"OPSIN Bug: malformed alpha/beta stereochemistry value");
            }
            if (notExplicitlyDefinedStereoCentreMap.find(stereoAtom) ==
                notExplicitlyDefinedStereoCentreMap.end()) { //stereocentre has already been defined, need to check for contradiction!
                AtomParity *newAtomParity = stereoAtom->getAtomParity();
                if (previousAtomParity == nullptr) {
                    if (newAtomParity != nullptr) {
                        throw StructureBuildingException(
                                std::wstring(L"contradictory alpha/beta stereochemistry at position ") +
                                stereoAtom->getFirstLocant());
                    }
                } else if (newAtomParity == nullptr) {
                    if (previousAtomParity != nullptr) {
                        throw StructureBuildingException(
                                std::wstring(L"contradictory alpha/beta stereochemistry at position ") +
                                stereoAtom->getFirstLocant());
                    }
                } else if (!checkEquivalencyOfAtomsRefs4AndParity(previousAtomParity->getAtomRefs4(),
                                                                  previousAtomParity->getParity(),
                                                                  newAtomParity->getAtomRefs4(),
                                                                  newAtomParity->getParity())) {
                    throw StructureBuildingException(
                            std::wstring(L"contradictory alpha/beta stereochemistry at position ") +
                            stereoAtom->getFirstLocant());
                }
            }
        } else {
            throw StructureBuildingException(L"Unsupported stereocentre type for alpha/beta stereochemistry");
        }
    } else {
        throw StructureBuildingException(L"Unsupported stereocentre type for alpha/beta stereochemistry");
    }
}

void StereochemistryHandler::assignCarbohydratePrefixStereochem(Element *carbohydrateGroup,
                                                                std::vector<Element *> &carbohydrateStereoChemistryEls) throw(StructureBuildingException) {
    Fragment *carbohydrate = carbohydrateGroup->getFrag();
    Set < Atom * > *atoms = notExplicitlyDefinedStereoCentreMap.keySet();
    std::vector<Atom *> stereocentresInCarbohydrate;
    for (auto atom : atoms) {
        if (carbohydrate->getAtomByID(atom->getID()) != nullptr) {
            boost::optional<bool> isAnomeric = atom->getProperty(Atom::ISANOMERIC);
            if (!isAnomeric || !isAnomeric) {
                stereocentresInCarbohydrate.push_back(atom);
            }
        }
    }
    //stereoconfiguration is specified from the farthest from C-1 to nearest to C-1
    //but it is easier to set it the other way around hence this reverse
    std::reverse(carbohydrateStereoChemistryEls.begin(), carbohydrateStereoChemistryEls.end());
    std::vector<std::wstring> stereocentreConfiguration;
    for (auto carbohydrateStereoChemistryEl : carbohydrateStereoChemistryEls) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> values = carbohydrateStereoChemistryEl->getAttributeValue(
                XmlDeclarations::VALUE_ATR).split(L"/");
        Collections::addAll(stereocentreConfiguration, values);
    }

    if (stereocentresInCarbohydrate.size() != stereocentreConfiguration.size()) {
        throw StructureBuildingException(
                std::wstring(L"Disagreement between number of stereocentres on carbohydrate: ") +
                stereocentresInCarbohydrate.size() +
                std::wstring(L" and centres defined by configurational prefixes: ") + stereocentreConfiguration.size());
    }
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(stereocentresInCarbohydrate, new FragmentTools.SortByLocants());
    FragmentTools::SortByLocants tempVar();
    std::sort(stereocentresInCarbohydrate.begin(), stereocentresInCarbohydrate.end(), &tempVar);
    for (int i = 0; i < stereocentresInCarbohydrate.size(); i++) {
        Atom *stereoAtom = stereocentresInCarbohydrate[i];
        std::wstring configuration = stereocentreConfiguration[i];
        if (configuration == L"r") {
            AtomParity *atomParity = stereoAtom->getAtomParity();
            if (atomParity == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: stereochemistry was not defined on a carbohydrate stem, but it should been");
                throw std::exception();
            }
            //do nothing, r by default
        } else if (configuration == L"l") {
            AtomParity *atomParity = stereoAtom->getAtomParity();
            if (atomParity == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: stereochemistry was not defined on a carbohydrate stem, but it should been");
                throw std::exception();
            }
            atomParity->setParity(-atomParity->getParity());
        } else if (configuration == L"?") {
            stereoAtom->setAtomParity(nullptr);
        } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: unexpected carbohydrate stereochemistry configuration: " + configuration);
            throw std::exception();
        }
        notExplicitlyDefinedStereoCentreMap.erase(stereoAtom);
    }
}

void StereochemistryHandler::assignDlStereochem(Element *stereoChemistryEl) throw(StructureBuildingException) {
    std::wstring dOrL = stereoChemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
    Element *elementToApplyTo = OpsinTools::getNextSiblingIgnoringCertainElements(stereoChemistryEl,
                                                                                  std::vector<std::wstring>{
                                                                                          XmlDeclarations::STEREOCHEMISTRY_EL});
    if (elementToApplyTo != nullptr && elementToApplyTo->getName() == XmlDeclarations::GROUP_EL &&
        attemptAssignmentOfDlStereoToFragment(elementToApplyTo->getFrag(), dOrL)) {
        // D/L adjacent to group that now has an appropriate stereocentre e.g. glycine
        return;
    }

    Element *parentSubBracketOrRoot = stereoChemistryEl->getParent();
    //generally the LAST group in this list will be the appropriate group
    //we use the same algorithm as for unlocanted substitution so as to deprecate assignment into brackets
    std::vector<Fragment *> possibleFragments = StructureBuildingMethods::findAlternativeFragments(
            parentSubBracketOrRoot);
    std::vector<Element *> adjacentGroupEls = OpsinTools::getDescendantElementsWithTagName(parentSubBracketOrRoot,
                                                                                           XmlDeclarations::GROUP_EL);
    for (int i = adjacentGroupEls.size() - 1; i >= 0; i--) {
        possibleFragments.push_back(adjacentGroupEls[i]->getFrag());
    }
    for (auto fragment : possibleFragments) {
        if (attemptAssignmentOfDlStereoToFragment(fragment, dOrL)) {
            return;
        }
    }
    throw StereochemistryException(
            std::wstring(L"Could not find stereocentre to apply ") + dOrL.toUpperCase(Locale::ROOT) +
            std::wstring(L" stereochemistry to"));
}

bool StereochemistryHandler::attemptAssignmentOfDlStereoToFragment(Fragment *fragment,
                                                                   const std::wstring &dOrL) throw(StereochemistryException, StructureBuildingException) {
    std::vector<Atom *> atomList = fragment->getAtomList();
    for (auto potentialStereoAtom : atomList) {
        if (notExplicitlyDefinedStereoCentreMap.find(potentialStereoAtom) !=
            notExplicitlyDefinedStereoCentreMap.end() && potentialStereoAtom->getBondCount() == 4) {
            std::vector<Atom *> neighbours = potentialStereoAtom->getAtomNeighbours();
            Atom *acidGroup = nullptr; //A carbon connected to non-carbons e.g. COOH
            Atom *amineOrAlcohol = nullptr; //N or O e.g. NH2 (as this may be substituted don't check H count)
            Atom *sideChain = nullptr; //A carbon
            Atom *hydrogen = nullptr; //A hydrogen
            for (auto atom : neighbours) {
                ChemEl el = atom->getElement();
                if (el == ChemEl::H) {
                    hydrogen = atom;
                } else if (el == ChemEl::C) {
                    int chalcogenNeighbours = 0;
                    for (auto neighbour2 : atom->getAtomNeighbours()) {
                        if (atom == neighbour2) {
                            continue;
                        }
                        if (neighbour2->getElement()::isChalcogen())
                        {
                            chalcogenNeighbours++;
                        }
                    }
                    if (chalcogenNeighbours > 0) {
                        acidGroup = atom;
                    } else {
                        sideChain = atom;
                    }
                } else if (el == ChemEl::O || el == ChemEl::N) {
                    amineOrAlcohol = atom;
                }
            }
            if (acidGroup != nullptr && amineOrAlcohol != nullptr && sideChain != nullptr && hydrogen != nullptr) {
                std::vector<Atom *> atomRefs4 = {acidGroup, sideChain, amineOrAlcohol, hydrogen};
                if (dOrL == L"l" || dOrL == L"ls") {
                    potentialStereoAtom->setAtomParity(atomRefs4, -1);
                } else if (dOrL == L"d" || dOrL == L"ds") {
                    potentialStereoAtom->setAtomParity(atomRefs4, 1);
                } else if (dOrL == L"dl") {
                    //racemic
                    potentialStereoAtom->setAtomParity(nullptr);
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: Unexpected value for D/L stereochemistry found: " + dOrL);
                    throw std::exception();
                }
                notExplicitlyDefinedStereoCentreMap.erase(potentialStereoAtom);
                return true;
            }
        }
    }
    return false;
}

int StereochemistryHandler::swapsRequiredToSort(std::vector<Atom *> &atomRefs4) {
    std::vector<Atom *> atomRefs4Copy = atomRefs4.clone();
    int swapsPerformed = 0;
    int i, j;

    for (i = atomRefs4Copy.size(); --i >= 0;) {
        bool swapped = false;
        for (j = 0; j < i; j++) {
            if (atomRefs4Copy[j]->getID() > atomRefs4Copy[j + 1]->getID()) {
                Atom *temp = atomRefs4Copy[j + 1];
                atomRefs4Copy[j + 1] = atomRefs4Copy[j];
                atomRefs4Copy[j] = temp;
                swapsPerformed++;
                swapped = true;
            }
        }
        if (!swapped) {
            return swapsPerformed;
        }
    }
    return swapsPerformed;
}

bool StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(std::vector<Atom *> &atomRefs1, int atomParity1,
                                                                   std::vector<Atom *> &atomRefs2, int atomParity2) {
    int swaps1 = swapsRequiredToSort(atomRefs1);
    int swaps2 = swapsRequiredToSort(atomRefs2);
    if (atomParity1 < 0 && atomParity2 > 0 || atomParity1 > 0 && atomParity2 < 0) {
        swaps1++;
    }
    return swaps1 % 2 == swaps2 % 2;
}
