#include "SuffixApplier.h"
#include "BuildState.h"
#include "SuffixRules.h"
#include "ComponentGenerationException.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "Fragment.h"
#include "Atom.h"
#include "XmlDeclarations.h"
#include "OpsinTools.h"
#include "SuffixRule.h"
#include "FragmentTools.h"
#include "AmbiguityChecker.h"
#include "Bond.h"
#include "CycleDetector.h"
#include "IsotopeSpecificationParser.h"
#include "ValencyChecker.h"
#include "StructureBuildingMethods.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
using IsotopeSpecification = uk::ac::cam::ch::wwmm::opsin::IsotopeSpecificationParser::IsotopeSpecification;

SuffixApplier::SuffixApplier(BuildState *state, SuffixRules *suffixRules) : state(state), suffixRules(suffixRules) {
}

bool SuffixApplier::isGroupTypeWithSpecificSuffixRules(const std::wstring &groupType) {
    return suffixRules->isGroupTypeWithSpecificSuffixRules(groupType);
}

void SuffixApplier::resolveSuffixes(Element *group,
                                    std::vector<Element *> &suffixes) throw(StructureBuildingException, ComponentGenerationException) {
    Fragment *frag = group->getFrag();
    std::vector<Atom *> atomList = frag->getAtomList(); //this instance of atomList will not change even once suffixes are merged into the fragment
    std::wstring groupType = frag->getType();
    std::wstring subgroupType = frag->getSubType();
    std::wstring suffixTypeToUse = isGroupTypeWithSpecificSuffixRules(groupType) ? groupType
                                                                                 : XmlDeclarations::STANDARDGROUP_TYPE_VAL;

    std::vector<Fragment *> associatedSuffixFrags = state->xmlSuffixMap[group];
    if (associatedSuffixFrags.size() > 0) { //null for non-final group in polycyclic spiro systems
        associatedSuffixFrags.clear();
    }
    std::unordered_map<std::wstring, std::vector<Element *>> suffixValToSuffixes = new LinkedHashMap<std::wstring, std::vector<Element *>>(); //effectively undoes the effect of multiplying out suffixes
    for (auto suffix : suffixes) {
        std::wstring suffixValue = suffix->getAttributeValue(XmlDeclarations::VALUE_ATR);
        std::vector<Element *> suffixesWithThisVal = suffixValToSuffixes[suffixValue];
        if (suffixesWithThisVal.empty()) {
            suffixesWithThisVal = std::vector<Element *>();
            suffixValToSuffixes[suffixValue] = suffixesWithThisVal;
        }
        suffixesWithThisVal.push_back(suffix);

        //Apply isotopes to suffixes if present
        if (suffix->getFrag() != nullptr) {
            //boughton system applies to preceding suffix
            //iupac system applies to following suffix
            Element *boughtonIsotopeSpecification = OpsinTools::getNextSibling(suffix);
            if (boughtonIsotopeSpecification != nullptr &&
                boughtonIsotopeSpecification->getName() == XmlDeclarations::ISOTOPESPECIFICATION_EL) {
                if (XmlDeclarations::BOUGHTONSYSTEM_TYPE_VAL ==
                    boughtonIsotopeSpecification->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
                    applyIsotopeToSuffix(suffix->getFrag(), boughtonIsotopeSpecification, false);
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Unexpected isotope specification after suffix");
                    throw std::exception();
                }
            }
            Element *iupacIsotopeSpecification = OpsinTools::getPreviousSibling(suffix);
            while (iupacIsotopeSpecification != nullptr &&
                   iupacIsotopeSpecification->getName() == XmlDeclarations::ISOTOPESPECIFICATION_EL &&
                   XmlDeclarations::IUPACSYSTEM_TYPE_VAL ==
                   iupacIsotopeSpecification->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
                Element *next = OpsinTools::getPreviousSibling(iupacIsotopeSpecification);
                applyIsotopeToSuffix(suffix->getFrag(), iupacIsotopeSpecification, true);
                iupacIsotopeSpecification = next;
            }
        }
    }

    bool reDetectCycles = false;
    std::vector<Fragment *> fragsToMerge;
    for (auto entry : suffixValToSuffixes) {
        std::wstring suffixValue = entry.first;
        std::vector<Element *> suffixesWithThisVal = entry.second;
        std::vector<Atom *> possibleAtomsToAttachSuffixTo;
        std::vector<SuffixRule *> rulesToApply = suffixRules->getSuffixRuleTags(suffixTypeToUse, suffixValue,
                                                                                subgroupType);
        for (int suffixIndex = 0; suffixIndex < suffixesWithThisVal.size(); suffixIndex++) {
            Element *suffix = suffixesWithThisVal[suffixIndex];
            Fragment *suffixFrag = nullptr;
            for (auto suffixRule : rulesToApply) {
                switch (suffixRule->getType()) {
                    case addgroup:
                        if (suffixFrag == nullptr) {
                            suffixFrag = suffix->getFrag();
                            if (suffixFrag == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Suffix was expected to have an associated fragment but it wasn't found");
                                throw std::exception();
                            }
                            Atom *firstAtomInSuffix = suffixFrag->getFirstAtom();
                            if (firstAtomInSuffix->getBondCount() <= 0) {
                                throw ComponentGenerationException(
                                        L"OPSIN Bug: Dummy atom in suffix should have at least one bond to it");
                            }
                            if (XmlDeclarations::CYCLEFORMER_SUBTYPE_VAL ==
                                suffix->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                                processCycleFormingSuffix(suffixFrag, frag, suffix);
                                reDetectCycles = true;
                            } else {
                                int bondOrderRequired = firstAtomInSuffix->getIncomingValency();
                                Atom *fragAtomToUse = getFragAtomToUse(frag, suffix, suffixTypeToUse);
                                if (fragAtomToUse == nullptr) {
                                    if (possibleAtomsToAttachSuffixTo.empty()) {
                                        int substitutionsRequired = suffixesWithThisVal.size();
                                        possibleAtomsToAttachSuffixTo = FragmentTools::findnAtomsForSubstitution(frag,
                                                                                                                 atomList[0],
                                                                                                                 substitutionsRequired,
                                                                                                                 bondOrderRequired,
                                                                                                                 true);
                                        if (possibleAtomsToAttachSuffixTo.empty()) {
                                            throw StructureBuildingException(
                                                    std::wstring(L"No suitable atom found to attach ") + suffixValue +
                                                    std::wstring(L" suffix"));
                                        }
                                        for (auto atom : possibleAtomsToAttachSuffixTo) {
                                            if (FragmentTools::isCharacteristicAtom(atom)) {
                                                throw StructureBuildingException(
                                                        L"No suitable atom found to attach suffix");
                                            }
                                        }
                                        if ((std::wstring(L"yes")) == suffixRule->getAttributeValue(
                                                XmlDeclarations::SUFFIXRULES_KETONELOCANT_ATR) &&
                                            !atomList[0]->getAtomIsInACycle()) {
                                            std::vector<Atom *> proKetoneAtoms = getProKetonePositions(
                                                    possibleAtomsToAttachSuffixTo);
                                            //Note that names like "ethanone" are allowable as the fragment may subsequently be substituted to form an actual ketone
                                            if (proKetoneAtoms.size() >= substitutionsRequired) {
                                                possibleAtomsToAttachSuffixTo = proKetoneAtoms;
                                            }
                                        }
                                        if (!(substitutionsRequired == 1 &&
                                              (XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == frag->getSubType() ||
                                               XmlDeclarations::HETEROSTEM_SUBTYPE_VAL == frag->getSubType()) &&
                                              possibleAtomsToAttachSuffixTo[0]->equals(frag->getFirstAtom()))) {
                                            if (AmbiguityChecker::isSubstitutionAmbiguous(possibleAtomsToAttachSuffixTo,
                                                                                          substitutionsRequired)) {
                                                state->addIsAmbiguous(std::wstring(L"Addition of ") + suffixValue +
                                                                      std::wstring(L" suffix to: ") +
                                                                      group->getValue());
                                            }
                                        }
                                    }
                                    fragAtomToUse = possibleAtomsToAttachSuffixTo[suffixIndex];
                                }

                                //create a new bond and associate it with the suffixfrag and both atoms. Remember the suffixFrag has not been imported into the frag yet
                                std::vector<Bond *> bonds(firstAtomInSuffix->getBonds());
                                for (auto bondToSuffix : bonds) {
                                    Atom *suffixAtom = bondToSuffix->getOtherAtom(firstAtomInSuffix);
                                    state->fragManager->createBond(fragAtomToUse, suffixAtom, bondToSuffix->getOrder());
                                    state->fragManager->removeBond(bondToSuffix);
                                    if (fragAtomToUse->getIncomingValency() > 2 &&
                                        (suffixValue == L"aldehyde" || suffixValue == L"al" || suffixValue ==
                                                                                               L"aldoxime")) { //formaldehyde/methanal are excluded as they are substitutable
                                        if ((std::wstring(L"X")) == suffixAtom->getFirstLocant()) { //carbaldehyde
                                            suffixAtom->setProperty(Atom::ISALDEHYDE, true);
                                        } else {
                                            fragAtomToUse->setProperty(Atom::ISALDEHYDE, true);
                                        }
                                    }
                                }
                            }
                        } else {
                            throw ComponentGenerationException(
                                    std::wstring(L"OPSIN bug: Suffix may only have one addgroup rule: ") +
                                    suffix->getValue());
                        }
                        break;
                    case changecharge: {
                        int chargeChange = std::stoi(
                                suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_CHARGE_ATR));
                        int protonChange = std::stoi(
                                suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_PROTONS_ATR));
                        if (suffix->getAttribute(XmlDeclarations::SUFFIXPREFIX_ATR) == nullptr) {
                            Atom *fragAtomToUse = getFragAtomToUse(frag, suffix, suffixTypeToUse);
                            if (fragAtomToUse != nullptr) {
                                fragAtomToUse->addChargeAndProtons(chargeChange, protonChange);
                            } else {
                                applyUnlocantedChargeModification(atomList, chargeChange, protonChange);
                            }
                        } else { //a suffix prefixed acylium suffix
                            if (suffixFrag == nullptr) {
                                throw StructureBuildingException(
                                        L"OPSIN bug: ordering of elements in suffixRules.xml wrong; changeCharge found before addGroup");
                            }
                            Set < Bond * > *bonds = state->fragManager->getInterFragmentBonds(suffixFrag);
                            if (bonds->size() != 1) {
                                throw StructureBuildingException(
                                        L"OPSIN bug: Wrong number of bonds between suffix and group");
                            }
                            for (auto bond : bonds) {
                                if (bond->getFromAtom()->getFrag() == suffixFrag) {
                                    bond->getFromAtom()->addChargeAndProtons(chargeChange, protonChange);
                                } else {
                                    bond->getToAtom()->addChargeAndProtons(chargeChange, protonChange);
                                }
                            }
                        }
                        break;
                    }
                    case setOutAtom: {
                        std::wstring outValencyAtr = suffixRule->getAttributeValue(
                                XmlDeclarations::SUFFIXRULES_OUTVALENCY_ATR);
                        int outValency = outValencyAtr != L"" ? std::stoi(outValencyAtr) : 1;
                        if (suffix->getAttribute(XmlDeclarations::SUFFIXPREFIX_ATR) == nullptr) {
                            Atom *fragAtomToUse = getFragAtomToUse(frag, suffix, suffixTypeToUse);
                            if (fragAtomToUse != nullptr) {
                                frag->addOutAtom(fragAtomToUse, outValency, true);
                            } else {
                                frag->addOutAtom(frag->getFirstAtom(), outValency, false);
                            }
                        } else { //something like oyl on a ring, which means it is now carbonyl and the outAtom is on the suffix and not frag
                            if (suffixFrag == nullptr) {
                                throw StructureBuildingException(
                                        L"OPSIN bug: ordering of elements in suffixRules.xml wrong; setOutAtom found before addGroup");
                            }
                            Set < Bond * > *bonds = state->fragManager->getInterFragmentBonds(suffixFrag);
                            if (bonds->size() != 1) {
                                throw StructureBuildingException(
                                        L"OPSIN bug: Wrong number of bonds between suffix and group");
                            }
                            for (auto bond : bonds) {
                                if (bond->getFromAtom()->getFrag() == suffixFrag) {
                                    suffixFrag->addOutAtom(bond->getFromAtom(), outValency, true);
                                } else {
                                    suffixFrag->addOutAtom(bond->getToAtom(), outValency, true);
                                }
                            }
                        }
                        break;
                    }
                    case setAcidicElement: {
                        ChemEl chemEl = StringHelper::fromString<ChemEl>(
                                suffixRule->getAttributeValue(XmlDeclarations::SUFFIXRULES_ELEMENT_ATR));
                        swapElementsSuchThatThisElementIsAcidic(suffixFrag, chemEl);
                        break;
                    }
                    case addSuffixPrefixIfNonePresentAndCyclic:
                    case addFunctionalAtomsToHydroxyGroups:
                    case chargeHydroxyGroups:
                    case removeTerminalOxygen:
                    case convertHydroxyGroupsToOutAtoms:
                    case convertHydroxyGroupsToPositiveCharge:
                        //already processed
                        break;
                }
            }

            if (suffixFrag != nullptr) { //merge suffix frag and parent fragment
                fragsToMerge.push_back(suffixFrag);
            }
        }
    }
    for (auto suffixFrag : fragsToMerge) {
        state->fragManager->removeAtomAndAssociatedBonds(suffixFrag->getFirstAtom()); //the dummy R atom
        Set <std::wstring> *suffixLocants = std::unordered_set<std::wstring>(suffixFrag->getLocants());
        for (auto suffixLocant : suffixLocants) {
            if (std::isdigit(suffixLocant.charAt(
                    0))) { //check that numeric locants do not conflict with the parent fragment e.g. hydrazide 2' with biphenyl 2'
                if (frag->hasLocant(suffixLocant)) {
                    suffixFrag->getAtomByLocant(suffixLocant)->removeLocant(suffixLocant);
                }
            }
        }
        state->fragManager->incorporateFragment(suffixFrag, frag);
    }
    if (reDetectCycles) {
        CycleDetector::assignWhetherAtomsAreInCycles(frag);
    }

}

void SuffixApplier::applyIsotopeToSuffix(Fragment *frag, Element *isotopeSpecification,
                                         bool mustBeApplied) throw(StructureBuildingException) {
    IsotopeSpecification *isotopeSpec = IsotopeSpecificationParser::parseIsotopeSpecification(isotopeSpecification);
    ChemEl chemEl = isotopeSpec->getChemEl();
    int isotope = isotopeSpec->getIsotope();
    int multiplier = isotopeSpec->getMultiplier();
    std::vector<std::wstring> locants = isotopeSpec->getLocants();
    if (locants.size() > 0 && !mustBeApplied) {
        //locanted boughton isotope probably applies to the group rather than the suffix
        return;
    }
    if (locants.empty()) {
        std::vector<Atom *> atoms = frag->getAtomList();
        atoms.erase(atoms.begin());
        if (chemEl == ChemEl::H) {
            std::vector<Atom *> parentAtomsToApplyTo = FragmentTools::findnAtomsForSubstitution(atoms, nullptr,
                                                                                                multiplier, 1, true);
            if (parentAtomsToApplyTo.empty()) {
                if (mustBeApplied) {
                    throw StructureBuildingException(
                            L"Failed to find sufficient hydrogen atoms for unlocanted hydrogen isotope replacement");
                } else {
                    return;
                }
            }
            if (AmbiguityChecker::isSubstitutionAmbiguous(parentAtomsToApplyTo, multiplier)) {
                state->addIsAmbiguous(
                        std::wstring(L"Position of hydrogen isotope on ") + frag->getTokenEl()->getValue());
            }
            for (int j = 0; j < multiplier; j++) {
                Atom *atomWithHydrogenIsotope = parentAtomsToApplyTo[j];
                Atom *hydrogen = state->fragManager->createAtom(isotopeSpec->getChemEl(), frag);
                hydrogen->setIsotope(isotope);
                state->fragManager->createBond(atomWithHydrogenIsotope, hydrogen, 1);
            }
        } else {
            std::vector<Atom *> parentAtomsToApplyTo;
            for (auto atom : atoms) {
                if (atom->getElement() == chemEl) {
                    parentAtomsToApplyTo.push_back(atom);
                }
            }
            if (parentAtomsToApplyTo.size() < multiplier) {
                if (mustBeApplied) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    throw StructureBuildingException(
                            std::wstring(L"Failed to find sufficient atoms for ") + chemEl::toString() +
                            std::wstring(L" isotope replacement"));
                } else {
                    return;
                }
            }
            if (AmbiguityChecker::isSubstitutionAmbiguous(parentAtomsToApplyTo, multiplier)) {
                state->addIsAmbiguous(std::wstring(L"Position of isotope on ") + frag->getTokenEl()->getValue());
            }
            for (int j = 0; j < multiplier; j++) {
                parentAtomsToApplyTo[j]->setIsotope(isotope);
            }
        }
    } else {
        if (chemEl == ChemEl::H) {
            for (int j = 0; j < locants.size(); j++) {
                Atom *atomWithHydrogenIsotope = frag->getAtomByLocantOrThrow(locants[j]);
                Atom *hydrogen = state->fragManager->createAtom(isotopeSpec->getChemEl(), frag);
                hydrogen->setIsotope(isotope);
                state->fragManager->createBond(atomWithHydrogenIsotope, hydrogen, 1);
            }
        } else {
            for (int j = 0; j < locants.size(); j++) {
                Atom *atom = frag->getAtomByLocantOrThrow(locants[j]);
                if (chemEl != atom->getElement()) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    throw StructureBuildingException(
                            std::wstring(L"The atom at locant: ") + locants[j] + std::wstring(L" was not a ") +
                            chemEl::toString());
                }
                atom->setIsotope(isotope);
            }
        }
    }
    isotopeSpecification->detach();
}

std::vector<Atom *> SuffixApplier::getProKetonePositions(std::vector<Atom *> &atoms) {
    std::vector<Atom *> proKetonePositions;
    for (auto atom : atoms) {
        std::vector<Bond *> bonds = atom->getBonds();
        if (bonds.size() == 2 && bonds[0]->getOrder() == 1 && bonds[1]->getOrder() == 1 &&
            bonds[0]->getOtherAtom(atom).getElement() == ChemEl::C &&
            bonds[1]->getOtherAtom(atom).getElement() == ChemEl::C) {
            proKetonePositions.push_back(atom);
        }
    }
    return proKetonePositions;
}

void SuffixApplier::processCycleFormingSuffix(Fragment *suffixFrag, Fragment *suffixableFragment,
                                              Element *suffix) throw(StructureBuildingException, ComponentGenerationException) {
    std::vector<Atom *> rAtoms;
    for (auto a : suffixFrag->getAtomList()) {
        if (a->getElement() == ChemEl::R) {
            rAtoms.push_back(a);
        }
    }
    if (rAtoms.size() != 2) {
        throw ComponentGenerationException(L"OPSIN bug: Incorrect number of R atoms associated with cyclic suffix");
    }
    if (rAtoms[0]->getBondCount() <= 0 || rAtoms[1]->getBondCount() <= 0) {
        throw ComponentGenerationException(L"OPSIN Bug: Dummy atoms in suffix should have at least one bond to them");
    }

    Atom *parentAtom1;
    Atom *parentAtom2;

    std::wstring locant = suffix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
    std::wstring locantId = suffix->getAttributeValue(XmlDeclarations::LOCANTID_ATR);
    if (locant != L"") {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> locants = locant.split(L",");
        if (locants.size() == 2) {
            parentAtom1 = suffixableFragment->getAtomByLocantOrThrow(locants[0]);
            parentAtom2 = suffixableFragment->getAtomByLocantOrThrow(locants[1]);
        } else if (locants.size() == 1) {
            parentAtom1 = suffixableFragment->getAtomByLocantOrThrow(L"1");
            parentAtom2 = suffixableFragment->getAtomByLocantOrThrow(locants[0]);
        } else {
            throw ComponentGenerationException(std::wstring(
                    L"Incorrect number of locants associated with cycle forming suffix, expected 2 found: ") +
                                               locants.size());
        }
    } else if (locantId != L"") {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> locantIds = locantId.split(L",");
        if (locantIds.size() != 2) {
            throw ComponentGenerationException(
                    L"OPSIN bug: Should be exactly 2 locants associated with a cyclic suffix");
        }
        parentAtom1 = suffixableFragment->getAtomByIDOrThrow(std::stoi(locantIds[0]));
        parentAtom2 = suffixableFragment->getAtomByIDOrThrow(std::stoi(locantIds[1]));
    } else {
        int chainLength = suffixableFragment->getChainLength();
        if (chainLength > 1 && chainLength == suffixableFragment->getAtomCount()) {
            parentAtom1 = suffixableFragment->getAtomByLocantOrThrow(L"1");
            parentAtom2 = suffixableFragment->getAtomByLocantOrThrow(std::to_wstring(chainLength));
        } else {
            std::vector<Atom *> hydroxyAtoms = FragmentTools::findHydroxyGroups(suffixableFragment);
            if (hydroxyAtoms.size() == 1 && suffixableFragment->getAtomByLocant(L"1") != nullptr) {
                parentAtom1 = suffixableFragment->getAtomByLocantOrThrow(L"1");
                parentAtom2 = hydroxyAtoms[0];
            } else {
                throw ComponentGenerationException(std::wstring(L"cycle forming suffix: ") + suffix->getValue() +
                                                   std::wstring(L" should be locanted!"));
            }
        }
    }
    if (parentAtom1->equals(parentAtom2)) {
        throw ComponentGenerationException(std::wstring(L"cycle forming suffix: ") + suffix->getValue() +
                                           std::wstring(L" attempted to form a cycle involving the same atom twice!"));
    }

    if (suffixableFragment->getType() == XmlDeclarations::CARBOHYDRATE_TYPE_VAL) {
        FragmentTools::removeTerminalOxygen(state, parentAtom1, 2);
        FragmentTools::removeTerminalOxygen(state, parentAtom1, 1);
        std::vector<Atom *> chainHydroxy = FragmentTools::findHydroxyLikeTerminalAtoms(parentAtom2->getAtomNeighbours(),
                                                                                       ChemEl::O);
        if (chainHydroxy.size() == 1) {
            FragmentTools::removeTerminalAtom(state, chainHydroxy[0]); //make sure to retain stereochemistry
        } else {
            throw ComponentGenerationException(
                    L"The second locant of a carbohydrate lactone should point to a carbon in the chain with a hydroxyl group");
        }
    } else {
        if (parentAtom2->getElement() ==
            ChemEl::O) { //cyclic suffixes like lactone formally indicate the removal of hydroxy cf. 1979 rule 472.1
            //...although in most cases they are used on structures that don't actually have a hydroxy group
            std::vector<Atom *> neighbours = parentAtom2->getAtomNeighbours();
            if (neighbours.size() == 1) {
                std::vector<Atom *> suffixNeighbours = rAtoms[1]->getAtomNeighbours();
                if (suffixNeighbours.size() == 1 && suffixNeighbours[0]->getElement() == ChemEl::O) {
                    state->fragManager->removeAtomAndAssociatedBonds(parentAtom2);
                    parentAtom2 = neighbours[0];
                }
            }
        }
    }
    makeBondsToSuffix(parentAtom1, rAtoms[0]);
    makeBondsToSuffix(parentAtom2, rAtoms[1]);
    state->fragManager->removeAtomAndAssociatedBonds(rAtoms[1]);
}

Atom *SuffixApplier::getFragAtomToUse(Fragment *frag, Element *suffix,
                                      const std::wstring &suffixTypeToUse) throw(StructureBuildingException) {
    std::wstring locant = suffix->getAttributeValue(XmlDeclarations::LOCANT_ATR);
    if (locant != L"") {
        return frag->getAtomByLocantOrThrow(locant);
    }
    std::wstring locantId = suffix->getAttributeValue(XmlDeclarations::LOCANTID_ATR);
    if (locantId != L"") {
        return frag->getAtomByIDOrThrow(std::stoi(locantId));
    }
    std::wstring defaultLocantId = suffix->getAttributeValue(XmlDeclarations::DEFAULTLOCANTID_ATR);
    if (defaultLocantId != L"") {
        return frag->getAtomByIDOrThrow(std::stoi(defaultLocantId));
    } else if (suffixTypeToUse == XmlDeclarations::ACIDSTEM_TYPE_VAL ||
               suffixTypeToUse == XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL || suffixTypeToUse ==
                                                                                 XmlDeclarations::CHALCOGENACIDSTEM_TYPE_VAL) { //means that e.g. sulfonyl, has an explicit outAtom
        return frag->getFirstAtom();
    }
    return nullptr;
}

void
SuffixApplier::applyUnlocantedChargeModification(std::vector<Atom *> &atomList, int chargeChange, int protonChange) {
    //List of atoms that can accept this charge while remaining in a reasonable valency
    std::vector<Atom *> nitrogens; //most likely
    std::vector<Atom *> otherHeteroatoms; //plausible
    std::vector<Atom *> carbonsAtoms; //rare
    std::vector<Atom *> chargedAtoms; //very rare
    if (atomList.empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: List of atoms to add charge suffix to was empty");
        throw std::exception();
    }
    for (auto a : atomList) {
        ChemEl chemEl = a->getElement();
        std::vector<boost::optional<int>> stableValencies = ValencyChecker::getPossibleValencies(chemEl,
                                                                                                 a->getCharge() +
                                                                                                 chargeChange);
        if (stableValencies.empty()) { //unstable valency so seems unlikely
            continue;
        }
        int resultantExpectedValency =
                (a->getLambdaConventionValency() == nullptr ? ValencyChecker::getDefaultValency(chemEl)
                                                            : a->getLambdaConventionValency()) +
                a->getProtonsExplicitlyAddedOrRemoved() + protonChange;

        if (!Arrays::asList(stableValencies)->contains(resultantExpectedValency)) {
            //unstable valency so seems unlikely
            continue;
        }
        if (protonChange < 0) {
            int substitableHydrogen = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a);
            if (a->hasSpareValency() &&
                !std::find(a->getFrag()->getIndicatedHydrogen().begin(), a->getFrag()->getIndicatedHydrogen().end(),
                           a) != a->getFrag()->getIndicatedHydrogen().end()) {
                substitableHydrogen--;
            }
            if (substitableHydrogen < 1) {
                //no hydrogens so operation can't remove one!
                continue;
            }
        }
        if (a->getCharge() == 0) {
            if (chemEl == ChemEl::N) {
                nitrogens.push_back(a);
            } else if (chemEl != ChemEl::C) {
                otherHeteroatoms.push_back(a);
            } else {
                carbonsAtoms.push_back(a);
            }
        } else {
            chargedAtoms.push_back(a);
        }
    }
    std::vector<Atom *> listFromWhichToChoose;
    if (!nitrogens.empty()) {
        listFromWhichToChoose = nitrogens;
        if (XmlDeclarations::AMINOACID_TYPE_VAL == atomList[0]->getFrag().getType()) {
            //By convention treat names like lysinium as unambiguous (prefer alpha nitrogen)
            if (std::find(listFromWhichToChoose.begin(), listFromWhichToChoose.end(), atomList[0]) !=
                listFromWhichToChoose.end()) {
                listFromWhichToChoose = std::vector<Atom *>();
                listFromWhichToChoose.push_back(atomList[0]);
            }
        }
    } else if (!otherHeteroatoms.empty()) {
        listFromWhichToChoose = otherHeteroatoms;
    } else if (!carbonsAtoms.empty()) {
        listFromWhichToChoose = carbonsAtoms;
    } else if (!chargedAtoms.empty()) {
        listFromWhichToChoose = chargedAtoms;
    } else {
        listFromWhichToChoose = atomList;
    }

    Atom *chosenAtom = listFromWhichToChoose[0];
    if (!AmbiguityChecker::allAtomsEquivalent(listFromWhichToChoose)) {
        state->addIsAmbiguous(
                std::wstring(L"Addition of charge suffix to: ") + chosenAtom->getFrag()->getTokenEl()->getValue());
    }

    chosenAtom->addChargeAndProtons(chargeChange, protonChange);
}

void SuffixApplier::swapElementsSuchThatThisElementIsAcidic(Fragment *frag,
                                                            ChemEl chemEl) throw(StructureBuildingException) {
    for (int i = 0, l = frag->getFunctionalAtomCount(); i < l; i++) {
        Atom *atom = frag->getFunctionalAtom(i)->getAtom();
        Set < Atom * > *ambiguouslyElementedAtoms = atom->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT);
        if (ambiguouslyElementedAtoms != nullptr) {
            Atom *atomToSwapWith = nullptr;
            for (auto ambiguouslyElementedAtom : ambiguouslyElementedAtoms) {
                if (ambiguouslyElementedAtom->getElement() == chemEl) {
                    atomToSwapWith = ambiguouslyElementedAtom;
                    break;
                }
            }
            if (atomToSwapWith != nullptr) {
                if (atomToSwapWith != atom) {
                    //swap locants and element type
                    std::vector<std::wstring> tempLocants1(atom->getLocants());
                    std::vector<std::wstring> tempLocants2(atomToSwapWith->getLocants());
                    atom->clearLocants();
                    atomToSwapWith->clearLocants();
                    for (auto locant : tempLocants1) {
                        atomToSwapWith->addLocant(locant);
                    }
                    for (auto locant : tempLocants2) {
                        atom->addLocant(locant);
                    }
                    ChemEl a2ChemEl = atomToSwapWith->getElement();
                    atomToSwapWith->setElement(atom->getElement());
                    atom->setElement(a2ChemEl);
                    ambiguouslyElementedAtoms->remove(atomToSwapWith);
                }
                ambiguouslyElementedAtoms->remove(atom);
                return;
            }
        }
    }
    throw StructureBuildingException(std::wstring(L"Unable to find potential acidic atom with element: ") + chemEl);
}

void SuffixApplier::makeBondsToSuffix(Atom *parentAtom, Atom *suffixRAtom) {
    std::vector<Bond *> bonds(suffixRAtom->getBonds());
    for (auto bondToSuffix : bonds) {
        Atom *suffixAtom = bondToSuffix->getOtherAtom(suffixRAtom);
        state->fragManager->createBond(parentAtom, suffixAtom, bondToSuffix->getOrder());
        state->fragManager->removeBond(bondToSuffix);
    }
}

std::vector<SuffixRule *>
SuffixApplier::getSuffixRuleTags(const std::wstring &suffixTypeToUse, const std::wstring &suffixValue,
                                 const std::wstring &subgroupType) throw(ComponentGenerationException) {
    return suffixRules->getSuffixRuleTags(suffixTypeToUse, suffixValue, subgroupType);
}
