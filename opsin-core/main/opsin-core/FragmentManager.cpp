#include "FragmentManager.h"
#include "Bond.h"
#include "Fragment.h"
#include "SMILESFragmentBuilder.h"
#include "IDManager.h"
#include "StructureBuildingException.h"
#include "XmlDeclarations.h"
#include "Element.h"
#include "Atom.h"
#include "StructureBuildingMethods.h"
#include "FragmentTools.h"
#include "TokenEl.h"
#include "StringTools.h"
#include "AtomParity.h"
#include "OutAtom.h"
#include "FunctionalAtom.h"
#include "BuildState.h"
#include "OpsinTools.h"
#include "StereoAnalyser.h"
#include "CipSequenceRules.h"
#include "CipOrderingException.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

FragmentManager::FragmentManager(SMILESFragmentBuilder *sBuilder, IDManager *idManager) : sBuilder(sBuilder),
                                                                                          idManager(idManager) {
    if (sBuilder == nullptr || idManager == nullptr) {
        throw std::invalid_argument("FragmentManager was parsed a null object in its constructor!");
    }
}

Fragment *FragmentManager::buildSMILES(const std::wstring &smiles) throw(StructureBuildingException) {
    return buildSMILES(smiles, L"", XmlDeclarations::NONE_LABELS_VAL);
}

Fragment *FragmentManager::buildSMILES(const std::wstring &smiles, const std::wstring &type,
                                       const std::wstring &labelMapping) throw(StructureBuildingException) {
    Fragment *newFrag = sBuilder->build(smiles, type, labelMapping);
    addFragment(newFrag);
    return newFrag;
}

Fragment *FragmentManager::buildSMILES(const std::wstring &smiles, Element *tokenEl,
                                       const std::wstring &labelMapping) throw(StructureBuildingException) {
    Fragment *newFrag = sBuilder->build(smiles, tokenEl, labelMapping);
    addFragment(newFrag);
    return newFrag;
}

Fragment *FragmentManager::getUnifiedFragment() {
    Fragment *uniFrag = new Fragment(L"");
    for (auto entry : fragToInterFragmentBond) {
        Fragment *f = entry.first;
        Set<Bond *> * interFragmentBonds = entry.second;
        for (auto atom : f->getAtomList()) {
            uniFrag->addAtom(atom);
        }
        for (auto bond : f->getBondSet()) {
            uniFrag->addBond(bond);
        }
        uniFrag->incorporateOutAtoms(f);
        uniFrag->incorporateFunctionalAtoms(f);

        for (auto interFragmentBond : interFragmentBonds) {
            uniFrag->addBond(interFragmentBond);
        }
    }
    addFragment(uniFrag);
    return uniFrag;
}

void FragmentManager::incorporateFragment(Fragment *childFrag, Fragment *parentFrag) throw(StructureBuildingException) {
    for (auto atom : childFrag->getAtomList()) {
        parentFrag->addAtom(atom);
    }
    for (auto bond : childFrag->getBondSet()) {
        parentFrag->addBond(bond);
    }
    parentFrag->incorporateOutAtoms(childFrag);
    parentFrag->incorporateFunctionalAtoms(childFrag);

    Set<Bond *> * interFragmentBonds = fragToInterFragmentBond[childFrag];
    if (interFragmentBonds == nullptr) {
        throw StructureBuildingException(L"Fragment not registered with this FragmentManager!");
    }
    for (auto bond : interFragmentBonds) { //reassign inter-fragment bonds of child
        if (bond->getFromAtom()->getFrag() == parentFrag && bond->getToAtom()->getFrag() == parentFrag) {
            //bond is now enclosed within parentFrag so make it an intra-fragment bond
            //and remove it from the inter-fragment set of the parentFrag
            parentFrag->addBond(bond);
            fragToInterFragmentBond[parentFrag]->remove(bond);
        } else {
            //bond was an inter-fragment bond between the childFrag and another frag
            //It is now between the parentFrag and another frag
            addInterFragmentBond(bond);
        }
    }
    fragToInterFragmentBond.erase(childFrag);
}

void FragmentManager::incorporateFragment(Fragment *childFrag, Atom *fromAtom, Fragment *parentFrag, Atom *toAtom,
                                          int bondOrder) throw(StructureBuildingException) {
    if (!fromAtom->getFrag()->equals(childFrag)) {
        throw StructureBuildingException(L"OPSIN Bug: fromAtom was not associated with childFrag!");
    }
    if (!toAtom->getFrag()->equals(parentFrag)) {
        throw StructureBuildingException(L"OPSIN Bug: toAtom was not associated with parentFrag!");
    }
    incorporateFragment(childFrag, parentFrag);
    createBond(fromAtom, toAtom, bondOrder);
}

void FragmentManager::replaceAtomWithSmiles(Atom *a, const std::wstring &smiles) throw(StructureBuildingException) {
    replaceAtomWithAtom(a, getHeteroatom(smiles), false);
}

Atom *FragmentManager::getHeteroatom(const std::wstring &smiles) throw(StructureBuildingException) {
    Fragment *heteroAtomFrag = sBuilder->build(smiles);
    if (heteroAtomFrag->getAtomCount() != 1) {
        throw StructureBuildingException(L"Heteroatom smiles described a fragment with multiple SMILES!");
    }
    return heteroAtomFrag->getFirstAtom();
}

void
FragmentManager::replaceAtomWithAtom(Atom *a, Atom *heteroAtom, bool assignLocant) throw(StructureBuildingException) {
    ChemEl chemEl = heteroAtom->getElement();
    int replacementCharge = heteroAtom->getCharge();
    if (replacementCharge != 0) {
        if (a->getCharge() == 0) {
            a->addChargeAndProtons(replacementCharge, heteroAtom->getProtonsExplicitlyAddedOrRemoved());
        } else if (a->getCharge() == replacementCharge) {
            a->setProtonsExplicitlyAddedOrRemoved(heteroAtom->getProtonsExplicitlyAddedOrRemoved());
        } else {
            throw StructureBuildingException(L"Charge conflict between replacement term and atom to be replaced");
        }
    }
    a->setElement(chemEl);
    a->removeElementSymbolLocants();
    if (assignLocant) {
        std::wstring primes = L"";
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        while (a->getFrag()->getAtomByLocant(chemEl::toString() + primes) !=
               nullptr) { //if element symbol already assigned, add a prime and try again
            primes += std::wstring(L"'");
        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        a->addLocant(chemEl::toString() + primes);
    }
}

Atom *FragmentManager::getAtomByID(int id) {
    for (auto f : fragments) {
        Atom *a = f->getAtomByID(id);
        if (a != nullptr) {
            return a;
        }
    }
    return nullptr;
}

Atom *FragmentManager::getAtomByIDOrThrow(int id) throw(StructureBuildingException) {
    Atom *a = getAtomByID(id);
    if (a == nullptr) {
        throw StructureBuildingException(L"Couldn't get atom by id");
    }
    return a;
}

void FragmentManager::convertSpareValenciesToDoubleBonds() throw(StructureBuildingException) {
    for (auto f : fragments) {
        FragmentTools::convertSpareValenciesToDoubleBonds(f);
    }
}

void FragmentManager::checkValencies() throw(StructureBuildingException) {
    for (auto f : fragments) {
        f->checkValencies();
    }
}

Set<Fragment *> *FragmentManager::getFragments() {
    return Collections::unmodifiableSet(fragments);
}

void FragmentManager::addFragment(Fragment *frag) {
    fragToInterFragmentBond[frag] = new LinkedHashSet<Bond *>();
}

void FragmentManager::removeFragment(Fragment *frag) throw(StructureBuildingException) {
    Set<Bond *> * interFragmentBondsInvolvingFragmentSet = fragToInterFragmentBond[frag];
    if (interFragmentBondsInvolvingFragmentSet == nullptr) {
        throw StructureBuildingException(L"Fragment not registered with this FragmentManager!");
    }
    std::vector<Bond *> interFragmentBondsInvolvingFragment(interFragmentBondsInvolvingFragmentSet);
    for (auto bond : interFragmentBondsInvolvingFragment) {
        if (bond->getFromAtom()->getFrag() == frag) {
            fragToInterFragmentBond[bond->getToAtom()->getFrag()]->remove(bond);
        } else {
            fragToInterFragmentBond[bond->getFromAtom()->getFrag()]->remove(bond);
        }
    }
    fragToInterFragmentBond.erase(frag);
}

int FragmentManager::getOverallCharge() {
    int totalCharge = 0;
    for (auto frag : fragments) {
        totalCharge += frag->getCharge();
    }
    return totalCharge;
}

Fragment *FragmentManager::copyFragment(Fragment *originalFragment) throw(StructureBuildingException) {
    return copyAndRelabelFragment(originalFragment, 0);
}

Fragment *FragmentManager::copyAndRelabelFragment(Fragment *originalFragment, int primesToAdd) {
    Element *tokenEl = new TokenEl(L"");
    tokenEl->addAttribute(XmlDeclarations::TYPE_ATR, originalFragment->getType());
    tokenEl->addAttribute(XmlDeclarations::SUBTYPE_ATR, originalFragment->getSubType());
    Fragment *newFragment = new Fragment(tokenEl);
    std::unordered_map<Atom *, Atom *> oldToNewAtomMap; //maps old Atom to new Atom
    std::vector<Atom *> atomList = originalFragment->getAtomList();
    for (auto atom : atomList) {
        int id = idManager->getNextID();
        std::vector<std::wstring> newLocants(atom->getLocants());
        if (primesToAdd != 0) {
            for (int i = 0; i < newLocants.size(); i++) {
                std::wstring currentLocant = newLocants[i];
                int currentPrimes = StringTools::countTerminalPrimes(currentLocant);
                std::wstring locantSansPrimes = currentLocant.substr(0, currentLocant.length() - currentPrimes);
                int highestNumberOfPrimesWithThisLocant = currentPrimes;
                while (originalFragment->getAtomByLocant(locantSansPrimes + StringTools::multiplyString(L"'",
                                                                                                        highestNumberOfPrimesWithThisLocant +
                                                                                                        1)) !=
                       nullptr) {
                    highestNumberOfPrimesWithThisLocant++;
                }
                newLocants[i] = locantSansPrimes + StringTools::multiplyString(L"'",
                                                                               ((highestNumberOfPrimesWithThisLocant +
                                                                                 1) * primesToAdd) + currentPrimes);
            }
        }
        Atom *newAtom = new Atom(id, atom->getElement(), newFragment);
        for (auto newLocant : newLocants) {
            newAtom->addLocant(newLocant);
        }
        newAtom->setCharge(atom->getCharge());
        newAtom->setIsotope(atom->getIsotope());
        newAtom->setSpareValency(atom->hasSpareValency());
        newAtom->setProtonsExplicitlyAddedOrRemoved(atom->getProtonsExplicitlyAddedOrRemoved());
        newAtom->setLambdaConventionValency(atom->getLambdaConventionValency());
        //outValency is derived from the outAtoms so is automatically cloned
        newAtom->setAtomIsInACycle(atom->getAtomIsInACycle());
        newAtom->setType(
                atom->getType()); //may be different from fragment type if the original atom was formerly in a suffix
        newAtom->setMinimumValency(atom->getMinimumValency());
        newFragment->addAtom(newAtom);
        oldToNewAtomMap[atom] = newAtom;
    }
    for (auto atom : atomList) {
        if (atom->getAtomParity() != nullptr) {
            std::vector<Atom *> oldAtomRefs4 = atom->getAtomParity()->getAtomRefs4();
            std::vector<Atom *> newAtomRefs4(4);
            for (int i = 0; i < oldAtomRefs4.size(); i++) {
                Atom *oldAtom = oldAtomRefs4[i];
                if (oldAtom->equals(AtomParity::hydrogen)) {
                    newAtomRefs4[i] = AtomParity::hydrogen;
                } else if (oldAtom->equals(AtomParity::deoxyHydrogen)) {
                    newAtomRefs4[i] = AtomParity::deoxyHydrogen;
                } else {
                    newAtomRefs4[i] = oldToNewAtomMap[oldAtom];
                }
            }
            AtomParity *newAtomParity = new AtomParity(newAtomRefs4, atom->getAtomParity()->getParity());
            oldToNewAtomMap[atom]->setAtomParity(newAtomParity);
        }
        Set<Atom *> * oldAmbiguousElementAssignmentAtoms = atom->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT);
        if (oldAmbiguousElementAssignmentAtoms != nullptr) {
            Set<Atom *> * newAtoms = new LinkedHashSet<Atom *>();
            for (auto oldAtom : oldAmbiguousElementAssignmentAtoms) {
                newAtoms->add(oldToNewAtomMap[oldAtom]);
            }
            oldToNewAtomMap[atom]->setProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT, newAtoms);
        }
        boost::optional<int> smilesHydrogenCount = atom->getProperty(Atom::SMILES_HYDROGEN_COUNT);
        if (smilesHydrogenCount) {
            oldToNewAtomMap[atom]->setProperty(Atom::SMILES_HYDROGEN_COUNT, smilesHydrogenCount);
        }
        boost::optional<int> oxidationNumber = atom->getProperty(Atom::OXIDATION_NUMBER);
        if (oxidationNumber) {
            oldToNewAtomMap[atom]->setProperty(Atom::OXIDATION_NUMBER, oxidationNumber);
        }
        boost::optional<bool> isAldehyde = atom->getProperty(Atom::ISALDEHYDE);
        if (isAldehyde) {
            oldToNewAtomMap[atom]->setProperty(Atom::ISALDEHYDE, isAldehyde);
        }
        boost::optional<bool> isAnomeric = atom->getProperty(Atom::ISANOMERIC);
        if (isAnomeric) {
            oldToNewAtomMap[atom]->setProperty(Atom::ISANOMERIC, isAnomeric);
        }
        boost::optional<int> atomClass = atom->getProperty(Atom::ATOM_CLASS);
        if (atomClass) {
            oldToNewAtomMap[atom]->setProperty(Atom::ATOM_CLASS, atomClass);
        }
        std::wstring homologyGroup = atom->getProperty(Atom::HOMOLOGY_GROUP);
        if (homologyGroup != L"") {
            oldToNewAtomMap[atom]->setProperty(Atom::HOMOLOGY_GROUP, homologyGroup);
        }
        std::vector<Atom *> oldPositionVariationAtoms = atom->getProperty(Atom::POSITION_VARIATION_BOND);
        if (oldPositionVariationAtoms.size() > 0) {
            std::vector<Atom *> newAtoms;
            for (auto oldAtom : oldPositionVariationAtoms) {
                newAtoms.push_back(oldToNewAtomMap[oldAtom]);
            }
            oldToNewAtomMap[atom]->setProperty(Atom::POSITION_VARIATION_BOND, newAtoms);
        }
    }
    for (int i = 0, l = originalFragment->getOutAtomCount(); i < l; i++) {
        OutAtom *outAtom = originalFragment->getOutAtom(i);
        newFragment->addOutAtom(oldToNewAtomMap[outAtom->getAtom()], outAtom->getValency(), outAtom->isSetExplicitly());
        if (outAtom->getLocant() != L"") {
            newFragment->getOutAtom(newFragment->getOutAtomCount() - 1)->setLocant(
                    outAtom->getLocant() + StringTools::multiplyString(L"'", primesToAdd));
        }
    }
    for (int i = 0, l = originalFragment->getFunctionalAtomCount(); i < l; i++) {
        FunctionalAtom *functionalAtom = originalFragment->getFunctionalAtom(i);
        newFragment->addFunctionalAtom(oldToNewAtomMap[functionalAtom->getAtom()]);
    }
    if (originalFragment->getDefaultInAtom() != nullptr) {
        newFragment->setDefaultInAtom(oldToNewAtomMap[originalFragment->getDefaultInAtom()]);
    }
    Set<Bond *> * bondSet = originalFragment->getBondSet();
    for (auto bond : bondSet) {
        Bond *newBond = createBond(oldToNewAtomMap[bond->getFromAtom()], oldToNewAtomMap[bond->getToAtom()],
                                   bond->getOrder());
        newBond->setSmilesStereochemistry(bond->getSmilesStereochemistry());
        if (bond->getBondStereo() != nullptr) {
            std::vector<Atom *> oldAtomRefs4 = bond->getBondStereo()->getAtomRefs4();
            std::vector<Atom *> newAtomRefs4(4);
            for (int i = 0; i < oldAtomRefs4.size(); i++) {
                newAtomRefs4[i] = oldToNewAtomMap[oldAtomRefs4[i]];
            }
            newBond->setBondStereoElement(newAtomRefs4, bond->getBondStereo()->getBondStereoValue());
        }
    }
    std::vector<Atom *> indicatedHydrogenAtoms = originalFragment->getIndicatedHydrogen();
    for (auto atom : indicatedHydrogenAtoms) {
        newFragment->addIndicatedHydrogen(oldToNewAtomMap[atom]);
    }
    addFragment(newFragment);
    return newFragment;
}

Element *
FragmentManager::cloneElement(BuildState *state, Element *elementToBeCloned) throw(StructureBuildingException) {
    return cloneElement(state, elementToBeCloned, 0);
}

Element *FragmentManager::cloneElement(BuildState *state, Element *elementToBeCloned,
                                       int primesToAdd) throw(StructureBuildingException) {
    Element *clone = elementToBeCloned->copy();
    std::vector<Element *> originalGroups = OpsinTools::getDescendantElementsWithTagName(elementToBeCloned,
                                                                                         XmlDeclarations::GROUP_EL);
    std::vector<Element *> clonedGroups = OpsinTools::getDescendantElementsWithTagName(clone,
                                                                                       XmlDeclarations::GROUP_EL);
    std::unordered_map<Fragment *, Fragment *> oldNewFragmentMapping = new LinkedHashMap<Fragment *, Fragment *>();
    for (int i = 0; i < originalGroups.size(); i++) {
        Fragment *originalFragment = originalGroups[i]->getFrag();
        Fragment *newFragment = copyAndRelabelFragment(originalFragment, primesToAdd);
        oldNewFragmentMapping[originalFragment] = newFragment;
        newFragment->setTokenEl(clonedGroups[i]);
        clonedGroups[i]->setFrag(newFragment);
        std::vector<Fragment *> originalSuffixes = state->xmlSuffixMap[originalGroups[i]];
        std::vector<Fragment *> newSuffixFragments;
        for (auto suffix : originalSuffixes) {
            newSuffixFragments.push_back(copyFragment(suffix));
        }
        state->xmlSuffixMap[clonedGroups[i]] = newSuffixFragments;
    }
    Set<Bond *> * interFragmentBondsToClone = new LinkedHashSet<Bond *>();
    for (auto originalFragment : oldNewFragmentMapping) { //add inter fragment bonds to cloned fragments
        for (auto bond : fragToInterFragmentBond[originalFragment->first]) {
            interFragmentBondsToClone->add(bond);
        }
    }
    for (auto bond : interFragmentBondsToClone) {
        Atom *originalFromAtom = bond->getFromAtom();
        Atom *originalToAtom = bond->getToAtom();
        Fragment *originalFragment1 = originalFromAtom->getFrag();
        Fragment *originalFragment2 = originalToAtom->getFrag();
        if (oldNewFragmentMapping.find(originalFragment1) == oldNewFragmentMapping.end() ||
            (oldNewFragmentMapping.find(originalFragment2) == oldNewFragmentMapping.end())) {
            throw StructureBuildingException(
                    L"An element that was a clone contained a bond that went outside the scope of the cloning");
        }
        Fragment *newFragment1 = oldNewFragmentMapping[originalFragment1];
        Fragment *newFragment2 = oldNewFragmentMapping[originalFragment2];
        Atom *fromAtom = newFragment1->getAtomList()[originalFragment1->getAtomList().find(originalFromAtom)];
        Atom *toAtom = newFragment2->getAtomList()[originalFragment2->getAtomList().find(originalToAtom)];
        createBond(fromAtom, toAtom, bond->getOrder());
    }
    return clone;
}

void FragmentManager::replaceAtomWithAnotherAtomPreservingConnectivity(Atom *atomToBeReplaced, Atom *replacementAtom) {
    atomToBeReplaced->removeElementSymbolLocants();
    std::vector<std::wstring> locants(atomToBeReplaced->getLocants());
    for (auto locant : locants) {
        atomToBeReplaced->removeLocant(locant);
        replacementAtom->addLocant(locant);
    }
    std::vector<Bond *> bonds = atomToBeReplaced->getBonds();
    for (auto bond : bonds) {
        Atom *connectedAtom = bond->getOtherAtom(atomToBeReplaced);
        if (connectedAtom->getAtomParity() != nullptr) {
            std::vector<Atom *> atomRefs4 = connectedAtom->getAtomParity()->getAtomRefs4();
            for (int i = 0; i < 4; i++) {
                if (atomRefs4[i] == atomToBeReplaced) {
                    atomRefs4[i] = replacementAtom;
                    break;
                }
            }
        }
        if (bond->getBondStereo() != nullptr) {
            std::vector<Atom *> atomRefs4 = bond->getBondStereo()->getAtomRefs4();
            for (int i = 0; i < 4; i++) {
                if (atomRefs4[i] == atomToBeReplaced) {
                    atomRefs4[i] = replacementAtom;
                    break;
                }
            }
        }
        createBond(replacementAtom, bond->getOtherAtom(atomToBeReplaced), bond->getOrder());
    }
    removeAtomAndAssociatedBonds(atomToBeReplaced);
}

void FragmentManager::removeInterFragmentBondIfPresent(Bond *bond) {
    fragToInterFragmentBond[bond->getFromAtom()->getFrag()]->remove(bond);
    fragToInterFragmentBond[bond->getToAtom()->getFrag()]->remove(bond);
}

void FragmentManager::addInterFragmentBond(Bond *bond) {
    fragToInterFragmentBond[bond->getFromAtom()->getFrag()]->add(bond);
    fragToInterFragmentBond[bond->getToAtom()->getFrag()]->add(bond);
}

Set<Bond *> *FragmentManager::getInterFragmentBonds(Fragment *frag) {
    Set<Bond *> * interFragmentBonds = fragToInterFragmentBond[frag];
    if (interFragmentBonds == nullptr) {
        throw std::invalid_argument("Fragment not registered with this FragmentManager!");
    }
    return Collections::unmodifiableSet(interFragmentBonds);
}

Atom *FragmentManager::createAtom(ChemEl chemEl, Fragment *frag) {
    Atom *a = new Atom(idManager->getNextID(), chemEl, frag);
    frag->addAtom(a);
    return a;
}

Bond *FragmentManager::createBond(Atom *fromAtom, Atom *toAtom, int bondOrder) {
    Bond *b = new Bond(fromAtom, toAtom, bondOrder);
    fromAtom->addBond(b);
    toAtom->addBond(b);
    if (fromAtom->getFrag() == toAtom->getFrag()) {
        fromAtom->getFrag()->addBond(b);
    } else {
        addInterFragmentBond(b);
    }
    return b;
}

void FragmentManager::removeAtomAndAssociatedBonds(Atom *atom) {
    std::vector<Bond *> bondsToBeRemoved(atom->getBonds());
    for (auto bond : bondsToBeRemoved) {
        removeBond(bond);
    }
    atom->getFrag()->removeAtom(atom);
    Set<Atom *> * ambiguousElementAssignment = atom->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT);
    if (ambiguousElementAssignment != nullptr) {
        ambiguousElementAssignment->remove(atom);
        if (ambiguousElementAssignment->size() == 1) {
            ambiguousElementAssignment->begin()->next().setProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT, nullptr);
        }
    }
}

void FragmentManager::removeBond(Bond *bond) {
    bond->getFromAtom()->getFrag()->removeBond(bond);
    bond->getFromAtom()->removeBond(bond);
    bond->getToAtom()->removeBond(bond);
    removeInterFragmentBondIfPresent(bond);
}

void FragmentManager::makeHydrogensExplicit() throw(StructureBuildingException) {
    for (auto fragment : fragments) {
        if (fragment->getSubType().equals(
                XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL)) { //these do not have implicit hydrogen e.g. phosphorus is literally just a phosphorus atom
            continue;
        }
        std::vector<Atom *> atomList = fragment->getAtomList();
        for (auto parentAtom : atomList) {
            int explicitHydrogensToAdd = StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(parentAtom);
            for (int i = 0; i < explicitHydrogensToAdd; i++) {
                Atom *hydrogen = createAtom(ChemEl::H, fragment);
                createBond(parentAtom, hydrogen, 1);
            }
            if (parentAtom->getAtomParity() != nullptr) {
                if (explicitHydrogensToAdd > 1) {
                    //Cannot have tetrahedral chirality and more than 2 hydrogens
                    parentAtom->setAtomParity(nullptr); //probably caused by deoxy
                } else {
                    modifyAtomParityToTakeIntoAccountExplicitHydrogen(parentAtom);
                }
            }
        }
    }
}

void FragmentManager::modifyAtomParityToTakeIntoAccountExplicitHydrogen(Atom *atom) throw(StructureBuildingException) {
    AtomParity *atomParity = atom->getAtomParity();
    if (!StereoAnalyser::isPossiblyStereogenic(atom)) {
        //no longer a stereoCentre e.g. due to unsaturation
        atom->setAtomParity(nullptr);
    } else {
        std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
        boost::optional<int> positionOfImplicitHydrogen = boost::none;
        boost::optional<int> positionOfDeoxyHydrogen = boost::none;
        for (int i = 0; i < atomRefs4.size(); i++) {
            Atom *a = atomRefs4[i];
            if (a->equals(AtomParity::hydrogen)) {
                positionOfImplicitHydrogen = i;
            } else if (a->equals(AtomParity::deoxyHydrogen)) {
                positionOfDeoxyHydrogen = i;
            }
        }
        if (positionOfImplicitHydrogen || positionOfDeoxyHydrogen) {
            //atom parity was set in SMILES, the dummy hydrogen atom has now been substituted
            std::vector<Atom *> neighbours = atom->getAtomNeighbours();
            for (auto atomRef : atomRefs4) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                neighbours.remove(atomRef);
            }
            if (neighbours.empty()) {
                throw StructureBuildingException(
                        L"OPSIN Bug: Unable to determine which atom has substituted a hydrogen at stereocentre");
            } else if (neighbours.size() == 1 && positionOfDeoxyHydrogen) {
                atomRefs4[positionOfDeoxyHydrogen] = neighbours[0];
                if (positionOfImplicitHydrogen) {
                    throw StructureBuildingException(
                            L"OPSIN Bug: Unable to determine which atom has substituted a hydrogen at stereocentre");
                }
            } else if (neighbours.size() == 1 && positionOfImplicitHydrogen) {
                atomRefs4[positionOfImplicitHydrogen] = neighbours[0];
            } else if (neighbours.size() == 2 && positionOfDeoxyHydrogen && positionOfImplicitHydrogen) {
                try {
                    CipSequenceRules tempVar(atom);
                    std::vector<Atom *> cipOrderedAtoms = (&tempVar)->getNeighbouringAtomsInCipOrder();
                    //higher priority group replaces the former hydroxy groups (deoxyHydrogen)
                    if (cipOrderedAtoms.find(neighbours[0]) > cipOrderedAtoms.find(neighbours[1])) {
                        atomRefs4[positionOfDeoxyHydrogen] = neighbours[0];
                        atomRefs4[positionOfImplicitHydrogen] = neighbours[1];
                    } else {
                        atomRefs4[positionOfDeoxyHydrogen] = neighbours[1];
                        atomRefs4[positionOfImplicitHydrogen] = neighbours[0];
                    }
                }
                catch (const CipOrderingException &e) {
                    //assume ligands equivalent so it makes no difference which is which
                    atomRefs4[positionOfDeoxyHydrogen] = neighbours[0];
                    atomRefs4[positionOfImplicitHydrogen] = neighbours[1];
                }
            } else {
                throw StructureBuildingException(
                        L"OPSIN Bug: Unable to determine which atom has substituted a hydrogen at stereocentre");
            }
        }
    }
}
