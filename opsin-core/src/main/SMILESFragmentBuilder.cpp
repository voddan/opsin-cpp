#include "SMILESFragmentBuilder.h"
#include "Atom.h"
#include "IDManager.h"
#include "Fragment.h"
#include "StructureBuildingException.h"
#include "AtomParity.h"
#include "XmlDeclarations.h"
#include "Element.h"
#include "CycleDetector.h"
#include "FragmentTools.h"
#include "BondStereo.h"
#include "ValencyChecker.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
using SMILES_BOND_DIRECTION = uk::ac::cam::ch::wwmm::opsin::Bond::SMILES_BOND_DIRECTION;
using BondStereoValue = uk::ac::cam::ch::wwmm::opsin::BondStereo::BondStereoValue;

SMILESFragmentBuilder::StackFrame::StackFrame(Atom *a, int bondOrderVal) {
    atom = a;
    bondOrder = bondOrderVal;
}

SMILESFragmentBuilder::StackFrame::StackFrame(const StackFrame &sf) {
    atom = sf->atom;
    bondOrder = sf->bondOrder;
}

java::util::Set <std::wstring> *const SMILESFragmentBuilder::organicAtoms = std::unordered_set<std::wstring>();
java::util::Set <std::wstring> *const SMILESFragmentBuilder::aromaticAtoms = std::unordered_set<std::wstring>();

SMILESFragmentBuilder::StaticConstructor::StaticConstructor() {
    organicAtoms->add(L"B");
    organicAtoms->add(L"C");
    organicAtoms->add(L"N");
    organicAtoms->add(L"O");
    organicAtoms->add(L"P");
    organicAtoms->add(L"S");
    organicAtoms->add(L"F");
    organicAtoms->add(L"Cl");
    organicAtoms->add(L"Br");
    organicAtoms->add(L"I");

    aromaticAtoms->add(L"c");
    aromaticAtoms->add(L"n");
    aromaticAtoms->add(L"o");
    aromaticAtoms->add(L"p");
    aromaticAtoms->add(L"s");
    aromaticAtoms->add(L"si");
    aromaticAtoms->add(L"as");
    aromaticAtoms->add(L"se");
    aromaticAtoms->add(L"sb");
    aromaticAtoms->add(L"te");
}

SMILESFragmentBuilder::StaticConstructor SMILESFragmentBuilder::staticConstructor;

SMILESFragmentBuilder::SMILESFragmentBuilder(IDManager *idManager) : idManager(idManager) {
}

SMILESFragmentBuilder::ParserInstance::ParserInstance(SMILESFragmentBuilder *outerInstance, const std::wstring &smiles,
                                                      Fragment *fragment) : smiles(smiles),
                                                                            endOfSmiles(smiles.length()),
                                                                            fragment(fragment),
                                                                            outerInstance(outerInstance) {
}

void SMILESFragmentBuilder::ParserInstance::parseSmiles() throw(StructureBuildingException) {
    StackFrame tempVar(nullptr, 1);
    stack->add(&tempVar);
    for (; i < endOfSmiles; i++) {
        wchar_t ch = smiles[i];
        switch (ch) {
            case L'(':
                StackFrame tempVar2(stack->getLast());
                stack->add(&tempVar2);
                break;
            case L')':
                stack->removeLast();
                break;
            case L'-':
                stack->getLast()->bondOrder = 1;
                break;
            case L'=':
                if (stack->getLast().bondOrder != 1) {
                    throw StructureBuildingException(L"= in unexpected position: bond order already defined!");
                }
                stack->getLast()->bondOrder = 2;
                break;
            case L'#':
                if (stack->getLast().bondOrder != 1) {
                    throw StructureBuildingException(L"# in unexpected position: bond order already defined!");
                }
                stack->getLast()->bondOrder = 3;
                break;
            case L'/':
                if (stack->getLast().slash != nullptr) {
                    throw StructureBuildingException(L"/ in unexpected position: bond configuration already defined!");
                }
                stack->getLast()->slash = SMILES_BOND_DIRECTION::RSLASH;
                break;
            case L'\\':
                if (stack->getLast().slash != nullptr) {
                    throw StructureBuildingException(L"\\ in unexpected position: bond configuration already defined!");
                }
                stack->getLast()->slash = SMILES_BOND_DIRECTION::LSLASH;
                break;
            case L'.':
                stack->getLast()->atom = nullptr;
                break;
            case L'a':
            case L'b':
            case L'c':
            case L'd':
            case L'e':
            case L'f':
            case L'g':
            case L'h':
            case L'i':
            case L'j':
            case L'k':
            case L'l':
            case L'm':
            case L'n':
            case L'o':
            case L'p':
            case L'q':
            case L'r':
            case L's':
            case L't':
            case L'u':
            case L'v':
            case L'w':
            case L'x':
            case L'y':
            case L'z':
            case L'A':
            case L'B':
            case L'C':
            case L'D':
            case L'E':
            case L'F':
            case L'G':
            case L'H':
            case L'I':
            case L'J':
            case L'K':
            case L'L':
            case L'M':
            case L'N':
            case L'O':
            case L'P':
            case L'Q':
            case L'R':
            case L'S':
            case L'T':
            case L'U':
            case L'V':
            case L'W':
            case L'X':
            case L'Y':
            case L'Z':
            case L'*':
                processOrganicAtom(ch);
                break;
            case L'[':
                processBracketedAtom();
                break;
            case L'0':
            case L'1':
            case L'2':
            case L'3':
            case L'4':
            case L'5':
            case L'6':
            case L'7':
            case L'8':
            case L'9':
            case L'%':
                processRingOpeningOrClosure(ch);
                break;
            default:
                throw StructureBuildingException(StringHelper::toString(ch) + std::wstring(
                        L" is in an unexpected position. Check this is not a mistake and that this feature of SMILES is supported by OPSIN's SMILES parser"));
        }
    }
    if (!ringClosures.empty()) {
        throw StructureBuildingException(L"Unmatched ring opening");
    }
}

void SMILESFragmentBuilder::ParserInstance::processOrganicAtom(wchar_t ch) throw(StructureBuildingException) {
    std::wstring elementType = StringHelper::toString(ch);
    bool spareValency = false;
    if (outerInstance->is_A_to_Z(ch)) { //normal atoms
        if (i + 1 < endOfSmiles && outerInstance->is_a_to_z(smiles[i + 1]) &&
            organicAtoms->contains(smiles.substr(i, 2))) {
            elementType = smiles.substr(i, 2);
            i++;
        } else if (!organicAtoms->contains(elementType)) {
            throw StructureBuildingException(elementType + std::wstring(
                    L" is not an organic Element. If it is actually an element it should be in square brackets"));
        }
    } else if (outerInstance->is_a_to_z(ch)) { //aromatic atoms
        if (!aromaticAtoms->contains(elementType)) {
            throw StructureBuildingException(elementType + std::wstring(
                    L" is not an aromatic Element. If it is actually an element it should not be in lower case"));
        }
        elementType = StringHelper::toString(static_cast<wchar_t>(ch - 32));
        spareValency = true;
    } else if (ch == L'*') {
        elementType = L"R";
    }
    Atom *atom = outerInstance->createAtom(elementType, fragment);
    atom->setSpareValency(spareValency);
    fragment->addAtom(atom);

    StackFrame *currentFrame = stack->getLast();
    if (currentFrame->atom != nullptr) {
        Bond *b = outerInstance->createBond(currentFrame->atom, atom, currentFrame->bondOrder);
        if (currentFrame->slash != nullptr) {
            b->setSmilesStereochemistry(currentFrame->slash);
            currentFrame->slash = nullptr;
        }
        if (currentFrame->atom->getAtomParity() != nullptr) {
            addAtomToAtomParity(currentFrame->atom->getAtomParity(), atom);
        }
    }
    currentFrame->atom = atom;
    currentFrame->bondOrder = 1;
}

void SMILESFragmentBuilder::ParserInstance::processBracketedAtom() throw(StructureBuildingException) {
    i++;
    int indexOfRightSquareBracket = (int) smiles.find(L']', i);
    if (indexOfRightSquareBracket == -1) {
        throw StructureBuildingException(L"[ without matching \"]\"");
    }
    // isotope
    std::wstring isotope = L"";
    while (outerInstance->is_0_to_9(smiles[i])) {
        isotope += smiles[i];
        i++;
    }

    wchar_t ch;
    if (i < indexOfRightSquareBracket) {
        ch = smiles[i];
        i++;
    } else {
        throw StructureBuildingException(L"No element found in square brackets");
    }
    // elementType
    std::wstring elementType = StringHelper::toString(ch);
    bool spareValency = false;
    if (outerInstance->is_A_to_Z(ch)) { //normal atoms
        if (outerInstance->is_a_to_z(smiles[i])) {
            elementType += smiles[i];
            i++;
        }
    } else if (outerInstance->is_a_to_z(ch)) { //aromatic atoms
        if (outerInstance->is_a_to_z(smiles[i])) {
            if (aromaticAtoms->contains(elementType + StringHelper::toString(smiles[i]))) {
                elementType = StringHelper::toString(static_cast<wchar_t>(ch - 32)) + smiles[i];
                i++;
            } else {
                throw StructureBuildingException(elementType + smiles[i] + std::wstring(
                        L" is not an aromatic Element. If it is actually an element it should not be in lower case"));
            }
        } else {
            if (!aromaticAtoms->contains(elementType)) {
                throw StructureBuildingException(elementType + std::wstring(L" is not an aromatic Element."));
            }
            elementType = StringHelper::toString(static_cast<wchar_t>(ch - 32));
        }
        spareValency = true;
    } else if (elementType == L"*") {
        elementType = L"R";
    } else {
        throw StructureBuildingException(elementType + std::wstring(L" is not a valid element type!"));
    }
    Atom *atom = outerInstance->createAtom(elementType, fragment);
    atom->setSpareValency(spareValency);
    if (isotope.length() > 0) {
        atom->setIsotope(std::stoi(isotope));
    }
    fragment->addAtom(atom);
    StackFrame *currentFrame = stack->getLast();
    if (currentFrame->atom != nullptr) {
        Bond *b = outerInstance->createBond(currentFrame->atom, atom, currentFrame->bondOrder);
        if (currentFrame->slash != nullptr) {
            b->setSmilesStereochemistry(currentFrame->slash);
            currentFrame->slash = nullptr;
        }
        if (currentFrame->atom->getAtomParity() != nullptr) {
            addAtomToAtomParity(currentFrame->atom->getAtomParity(), atom);
        }
    }
    Atom *previousAtom = currentFrame->atom; //needed for setting atomParity elements up
    currentFrame->atom = atom;
    currentFrame->bondOrder = 1;

    boost::optional<int> hydrogenCount = 0;
    int charge = 0;
    boost::optional<bool> chiralitySet = false;
    for (; i < indexOfRightSquareBracket; i++) {
        ch = smiles[i];
        if (ch == L'@') { // chirality-sets atom parity
            if (chiralitySet) {
                throw StructureBuildingException(
                        L"Atom parity appeared to be specified twice for an atom in a square bracket!");
            }
            processTetrahedralStereochemistry(atom, previousAtom);
            chiralitySet = true;
        } else if (ch == L'H') { // hydrogenCount
            if (!hydrogenCount || hydrogenCount != 0) {
                throw StructureBuildingException(
                        L"Hydrogen count appeared to be specified twice for an atom in a square bracket!");
            }
            if (smiles[i + 1] == L'?') {
                //extension to allow standard valency (as determined by the group in the periodic table) to dictate hydrogens
                i++;
                hydrogenCount = boost::none;
            } else {
                std::wstring hydrogenCountString = L"";
                while (outerInstance->is_0_to_9(smiles[i + 1])) {
                    hydrogenCountString += smiles[i + 1];
                    i++;
                }
                if (hydrogenCountString.length() == 0) {
                    hydrogenCount = 1;
                } else {
                    hydrogenCount = std::stoi(hydrogenCountString);
                }
                if (atom->hasSpareValency()) {
                    if ((elementType != L"C" && elementType != L"Si") || hydrogenCount >= 2) {
                        fragment->addIndicatedHydrogen(atom);
                    }
                }
            }
        } else if (ch == L'+' || ch == L'-') { // formalCharge
            if (charge != 0) {
                throw StructureBuildingException(
                        L"Charge appeared to be specified twice for an atom in a square bracket!");
            }
            charge = (ch == L'+') ? 1 : -1;
            std::wstring changeChargeStr = L"";
            int changeCharge = 1;
            while (outerInstance->is_0_to_9(smiles[i + 1])) { //e.g. [C+2]
                changeChargeStr += smiles[i + 1];
                i++;
            }
            if (changeChargeStr.length() == 0) {
                while (i + 1 < indexOfRightSquareBracket) { //e.g. [C++]
                    ch = smiles[i + 1];
                    if (ch == L'+') {
                        if (charge != 1) {
                            throw StructureBuildingException(
                                    L"Atom has both positive and negative charges specified!"); //e.g. [C+-]
                        }
                    } else if (ch == L'-') {
                        if (charge != -1) {
                            throw StructureBuildingException(L"Atom has both negative and positive charges specified!");
                        }
                    } else {
                        break;
                    }
                    changeCharge++;
                    i++;
                }
            }
            changeCharge = changeChargeStr.length() == 0 ? changeCharge : std::stoi(changeChargeStr);
            atom->setCharge(charge * changeCharge);
        } else if (ch == L'|') {
            StringBuilder *lambda = new StringBuilder();
            while (i < endOfSmiles && outerInstance->is_0_to_9(smiles[i + 1])) {
                lambda->append(smiles[i + 1]);
                i++;
            }
            atom->setLambdaConventionValency(static_cast<Integer>(lambda->toString()));
        } else {
            throw StructureBuildingException(L"Unexpected character found in square bracket");
        }
    }
    atom->setProperty(Atom::SMILES_HYDROGEN_COUNT, hydrogenCount);
}

void SMILESFragmentBuilder::ParserInstance::processTetrahedralStereochemistry(Atom *atom, Atom *previousAtom) {
    boost::optional<bool> chiralityClockwise = false;
    if (smiles[i + 1] == L'@') {
        chiralityClockwise = true;
        i++;
    }
    AtomParity *atomParity;
    if (chiralityClockwise) {
        atomParity = new AtomParity(std::vector<Atom *>(4), 1);
    } else {
        atomParity = new AtomParity(std::vector<Atom *>(4), -1);
    }
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    int index = 0;
    if (previousAtom != nullptr) {
        atomRefs4[index] = previousAtom;
        index++;
    }
    if (smiles[i + 1] == L'H') {
        atomRefs4[index] = AtomParity::hydrogen;
        //this character will also be checked by the hydrogen count check, hence don't increment i
    }
    atom->setAtomParity(atomParity);
}

void SMILESFragmentBuilder::ParserInstance::processRingOpeningOrClosure(wchar_t ch) throw(StructureBuildingException) {
    std::wstring closure = StringHelper::toString(ch);
    if (ch == L'%') {
        if (i + 2 < endOfSmiles && outerInstance->is_0_to_9(smiles[i + 1]) && outerInstance->is_0_to_9(smiles[i + 2])) {
            closure = smiles.substr(i + 1, (i + 3) - (i + 1));
            i += 2;
        } else {
            throw StructureBuildingException(L"A ring opening indice after a % must be two digits long");
        }
    }
    if (ringClosures.find(closure) != ringClosures.end()) {
        processRingClosure(closure);
    } else {
        if (getInscopeAtom() == nullptr) {
            throw StructureBuildingException(L"A ring opening has appeared before any atom!");
        }
        processRingOpening(closure);
    }
}

void SMILESFragmentBuilder::ParserInstance::processRingOpening(
        const std::wstring &closure) throw(StructureBuildingException) {
    StackFrame *currentFrame = stack->getLast();
    StackFrame *sf = new StackFrame(currentFrame);
    if (currentFrame->slash != nullptr) {
        sf->slash = currentFrame->slash;
        currentFrame->slash = nullptr;
    }
    AtomParity *atomParity = sf->atom->getAtomParity();
    if (atomParity != nullptr) { //replace ringclosureX with actual reference to id when it is known
        sf->indexOfDummyAtom = addAtomToAtomParity(atomParity, ringOpeningDummyAtom);
    }
    ringClosures[closure] = sf;
    currentFrame->bondOrder = 1;
}

void SMILESFragmentBuilder::ParserInstance::processRingClosure(
        const std::wstring &closure) throw(StructureBuildingException) {
    StackFrame *sf = ringClosures.erase(closure);
    StackFrame *currentFrame = stack->getLast();
    int bondOrder = 1;
    if (sf->bondOrder > 1) {
        if (currentFrame->bondOrder > 1 && sf->bondOrder != currentFrame->bondOrder) {
            throw StructureBuildingException(L"ring closure has two different bond orders specified!");
        }
        bondOrder = sf->bondOrder;
    } else if (currentFrame->bondOrder > 1) {
        bondOrder = currentFrame->bondOrder;
    }
    Bond *b;
    if (currentFrame->slash != nullptr) {
        //stereochemistry specified on ring closure
        //special case e.g. CC1=C/F.O\1  Bond is done from the O to the the C due to the presence of the \
								b = outerInstance->createBond(currentFrame->atom, sf->atom, bondOrder);
        b->setSmilesStereochemistry(currentFrame->slash);
        if (sf->slash != nullptr && sf->slash::equals(currentFrame->slash)) { //specified twice check for contradiction
            throw StructureBuildingException(L"Contradictory double bond stereoconfiguration");
        }
        currentFrame->slash = nullptr;
    } else {
        b = outerInstance->createBond(sf->atom, currentFrame->atom, bondOrder);
        if (sf->slash != nullptr) {
            //stereochemistry specified on ring opening
            b->setSmilesStereochemistry(sf->slash);
        }
    }

    AtomParity *currentAtomParity = currentFrame->atom->getAtomParity();
    if (currentAtomParity != nullptr) {
        addAtomToAtomParity(currentAtomParity, sf->atom);
    }

    AtomParity *closureAtomParity = sf->atom->getAtomParity();
    if (closureAtomParity !=
        nullptr) { //replace dummy atom with actual atom e.g. N[C@@H]1C.F1 where the 1 initially holds a dummy atom before being replaced with the F atom
        std::vector<Atom *> atomRefs4 = closureAtomParity->getAtomRefs4();
        if (!sf->indexOfDummyAtom) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Index of dummy atom representing ring closure atom not set");
            throw std::exception();
        }
        atomRefs4[sf->indexOfDummyAtom] = currentFrame->atom;
    }
    currentFrame->bondOrder = 1;
}

int SMILESFragmentBuilder::ParserInstance::addAtomToAtomParity(AtomParity *atomParity,
                                                               Atom *atom) throw(StructureBuildingException) {
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    bool setAtom = false;
    int i = 0;
    for (; i < atomRefs4.size(); i++) {
        if (atomRefs4[i] == nullptr) {
            atomRefs4[i] = atom;
            setAtom = true;
            break;
        }
    }
    if (!setAtom) {
        throw StructureBuildingException(
                L"Tetrahedral stereocentre specified in SMILES appears to involve more than 4 atoms");
    }
    return i;
}

Atom *SMILESFragmentBuilder::ParserInstance::getInscopeAtom() {
    return stack->getLast().atom;
}

Fragment *SMILESFragmentBuilder::build(const std::wstring &smiles) throw(StructureBuildingException) {
    return build(smiles, L"", XmlDeclarations::NONE_LABELS_VAL);
}

Fragment *SMILESFragmentBuilder::build(const std::wstring &smiles, const std::wstring &type,
                                       const std::wstring &labelMapping) throw(StructureBuildingException) {
    Fragment tempVar(type);
    return build(smiles, &tempVar, labelMapping);
}

Fragment *SMILESFragmentBuilder::build(const std::wstring &smiles, Element *tokenEl,
                                       const std::wstring &labelMapping) throw(StructureBuildingException) {
    if (tokenEl == nullptr) {
        throw std::invalid_argument("tokenEl is null. FragmentManager's DUMMY_TOKEN should be used instead");
    }
    Fragment tempVar(tokenEl);
    return build(smiles, &tempVar, labelMapping);
}

Fragment *SMILESFragmentBuilder::build(const std::wstring &smiles, Fragment *fragment,
                                       const std::wstring &labelMapping) throw(StructureBuildingException) {
    if (smiles == L"") {
        throw std::invalid_argument("SMILES specified is null");
    }
    if (labelMapping == L"") {
        throw std::invalid_argument(
                "labelMapping is null use \"none\" if you do not want any numbering or \"numeric\" if you would like default numbering");
    }
    if (smiles.length() == 0) {
        return fragment;
    }
    int firstIndex = 0;
    int lastIndex = smiles.length();
    wchar_t firstCharacter = smiles[0];
    if (firstCharacter == L'-' || firstCharacter == L'=' ||
        firstCharacter == L'#') { //used by OPSIN to specify the valency with which this fragment connects
        firstIndex++;
    }
    wchar_t lastCharacter = smiles[lastIndex - 1];
    if (lastCharacter == L'-' || lastCharacter == L'=' || lastCharacter ==
                                                          L'#') { //used by OPSIN to specify the valency with which this fragment connects and to indicate it connects via the last atom in the SMILES
        lastIndex--;
    }
    ParserInstance *instance = new ParserInstance(this, smiles.substr(firstIndex, lastIndex - firstIndex), fragment);
    instance->parseSmiles();

    std::vector<Atom *> atomList = fragment->getAtomList();
    processLabelling(labelMapping, atomList);

    verifyAndTakeIntoAccountLonePairsInAtomParities(atomList);
    addBondStereoElements(fragment);

    if (firstCharacter == L'-') {
        fragment->addOutAtom(fragment->getFirstAtom(), 1, true);
    } else if (firstCharacter == L'=') {
        fragment->addOutAtom(fragment->getFirstAtom(), 2, true);
    } else if (firstCharacter == L'#') {
        fragment->addOutAtom(fragment->getFirstAtom(), 3, true);
    }

    if (lastCharacter == L'-' || lastCharacter == L'=' || lastCharacter == L'#') {
        Atom *lastAtom = instance->getInscopeAtom(); //note that in something like C(=O)- this would be the carbon not the oxygen
        if (lastCharacter == L'#') {
            fragment->addOutAtom(lastAtom, 3, true);
        } else if (lastCharacter == L'=') {
            fragment->addOutAtom(lastAtom, 2, true);
        } else {
            fragment->addOutAtom(lastAtom, 1, true);
        }
    }

    for (auto atom : atomList) {
        if (atom->getProperty(Atom::SMILES_HYDROGEN_COUNT) != nullptr &&
            atom->getLambdaConventionValency() == nullptr) {
            setupAtomValency(atom);
        }
    }
    CycleDetector::assignWhetherAtomsAreInCycles(fragment);
    return fragment;
}

void SMILESFragmentBuilder::processLabelling(const std::wstring &labelMapping,
                                             std::vector<Atom *> &atomList) throw(StructureBuildingException) {
    if (labelMapping == XmlDeclarations::NONE_LABELS_VAL || labelMapping.length() == 0) {
        return;
    }
    if (labelMapping == XmlDeclarations::NUMERIC_LABELS_VAL) {
        int atomNumber = 1;
        for (auto atom : atomList) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            atom->addLocant(Integer::toString(atomNumber++));
        }
    } else if (labelMapping ==
               XmlDeclarations::FUSEDRING_LABELS_VAL) { //fragment is a fusedring with atoms in the correct order for fused ring numbering
        //this will do stuff like changing labels from 1,2,3,4,5,6,7,8,9,10->1,2,3,4,4a,5,6,7,8,8a
        FragmentTools::relabelLocantsAsFusedRingSystem(atomList);
    } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> labelMap = labelMapping.split(L"/", -1); //place slash delimited labels into an array
        int numOfAtoms = atomList.size();
        if (labelMap.size() != numOfAtoms) {
            throw StructureBuildingException(
                    std::wstring(L"Group numbering has been invalidly defined in resource file: labels: ") +
                    labelMap.size() + std::wstring(L", atoms: ") + std::to_wstring(numOfAtoms));
        }
        for (int i = 0; i < numOfAtoms; i++) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            std::vector<std::wstring> labels = labelMap[i].split(L",");
            for (auto label : labels) {
                if (label.length() > 0) {
                    atomList[i]->addLocant(label);
                }
            }
        }
    }
}

void SMILESFragmentBuilder::verifyAndTakeIntoAccountLonePairsInAtomParities(
        std::vector<Atom *> &atomList) throw(StructureBuildingException) {
    for (auto atom : atomList) {
        AtomParity *atomParity = atom->getAtomParity();
        if (atomParity != nullptr) {
            std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
            int nullAtoms = 0;
            int hydrogen = 0;
            for (auto atomRefs4Atom : atomRefs4) {
                if (atomRefs4Atom == nullptr) {
                    nullAtoms++;
                } else if (atomRefs4Atom->equals(AtomParity::hydrogen)) {
                    hydrogen++;
                }
            }
            if (nullAtoms != 0) {
                if (nullAtoms == 1 && hydrogen == 0 &&
                    (atom->getElement() == ChemEl::N || atom->getElement() == ChemEl::S ||
                     atom->getElement() == ChemEl::Se)) { //special case where lone pair is part of the tetrahedron
                    if (atomList.find(atomRefs4[0]) <
                        atomList.find(atom)) { //is there an atom in the SMILES in front of the stereocentre?
                        atomRefs4[3] = atomRefs4[2];
                        atomRefs4[2] = atomRefs4[1];
                        atomRefs4[1] = atom;
                    } else {
                        atomRefs4[3] = atomRefs4[2];
                        atomRefs4[2] = atomRefs4[1];
                        atomRefs4[1] = atomRefs4[0];
                        atomRefs4[0] = atom;
                    }
                } else {
                    throw StructureBuildingException(
                            L"SMILES is malformed. Tetrahedral stereochemistry defined on a non tetrahedral centre");
                }
            }
        }
    }
}

void SMILESFragmentBuilder::addBondStereoElements(Fragment *currentFrag) throw(StructureBuildingException) {
    Set < Bond * > *bonds = currentFrag->getBondSet();
    for (auto centralBond : bonds) { //identify cases of E/Z stereochemistry and add appropriate bondstereo tags
        if (centralBond->getOrder() == 2) {
            std::vector<Bond *> fromAtomBonds = centralBond->getFromAtom()->getBonds();
            for (auto preceedingBond : fromAtomBonds) {
                if (preceedingBond->getSmilesStereochemistry() != nullptr) {
                    std::vector<Bond *> toAtomBonds = centralBond->getToAtom()->getBonds();
                    for (auto followingBond : toAtomBonds) {
                        if (followingBond->getSmilesStereochemistry() !=
                            nullptr) { //now found a double bond surrounded by two bonds with slashs
                            bool upFirst;
                            bool upSecond;
                            Atom *atom2 = centralBond->getFromAtom();
                            Atom *atom3 = centralBond->getToAtom();
                            Atom *atom1 = preceedingBond->getOtherAtom(atom2);
                            Atom *atom4 = followingBond->getOtherAtom(atom3);
                            if (preceedingBond->getSmilesStereochemistry() == SMILES_BOND_DIRECTION::LSLASH) {
                                upFirst = preceedingBond->getToAtom() ==
                                          atom2; //in normally constructed SMILES this will be the case but you could write C(/F)=C/F instead of F\C=C/F
                            } else if (preceedingBond->getSmilesStereochemistry() == SMILES_BOND_DIRECTION::RSLASH) {
                                upFirst = preceedingBond->getToAtom() != atom2;
                            } else {
                                throw StructureBuildingException(
                                        preceedingBond->getSmilesStereochemistry() + std::wstring(L" is not a slash!"));
                            }

                            if (followingBond->getSmilesStereochemistry() == SMILES_BOND_DIRECTION::LSLASH) {
                                upSecond = followingBond->getFromAtom() != atom3;
                            } else if (followingBond->getSmilesStereochemistry() == SMILES_BOND_DIRECTION::RSLASH) {
                                upSecond = followingBond->getFromAtom() == atom3;
                            } else {
                                throw StructureBuildingException(
                                        followingBond->getSmilesStereochemistry() + std::wstring(L" is not a slash!"));
                            }
                            BondStereoValue cisTrans =
                                    upFirst == upSecond ? BondStereoValue::CIS : BondStereoValue::TRANS;
                            if (centralBond->getBondStereo() != nullptr) {
                                //double bond has redundant specification e.g. C/C=C\\1/NC1 hence need to check it is consistent
                                std::vector<Atom *> atomRefs4 = centralBond->getBondStereo()->getAtomRefs4();
                                if (atomRefs4[0]->equals(atom1) || atomRefs4[3]->equals(atom4)) {
                                    if (centralBond->getBondStereo()->getBondStereoValue()::equals(cisTrans))
                                    {
                                        throw StructureBuildingException(
                                                L"Contradictory double bond stereoconfiguration");
                                    }
                                } else {
                                    if (!centralBond->getBondStereo()->getBondStereoValue()::equals(cisTrans))
                                    {
                                        throw StructureBuildingException(
                                                L"Contradictory double bond stereoconfiguration");
                                    }
                                }
                            } else {
                                std::vector<Atom *> atomRefs4(4);
                                atomRefs4[0] = atom1;
                                atomRefs4[1] = atom2;
                                atomRefs4[2] = atom3;
                                atomRefs4[3] = atom4;
                                centralBond->setBondStereoElement(atomRefs4, cisTrans);
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto bond : bonds) {
        bond->setSmilesStereochemistry(nullptr);
    }
}

void SMILESFragmentBuilder::setupAtomValency(Atom *atom) throw(StructureBuildingException) {
    int hydrogenCount = atom->getProperty(Atom::SMILES_HYDROGEN_COUNT);
    int incomingValency = atom->getIncomingValency() + hydrogenCount + atom->getOutValency();
    int charge = atom->getCharge();
    int absoluteCharge = std::abs(charge);
    ChemEl chemEl = atom->getElement();
    if (atom->hasSpareValency()) {
        boost::optional<int> hwValency = ValencyChecker::getHWValency(chemEl);
        if (!hwValency || absoluteCharge > 1) {
            throw StructureBuildingException(chemEl + std::wstring(L" is not expected to be aromatic!"));
        }
        if (absoluteCharge != 0) {
            std::vector<boost::optional<int>> possibleVal = ValencyChecker::getPossibleValencies(chemEl, charge);
            if (possibleVal.size() > 0 && possibleVal.size() > 0) {
                hwValency = possibleVal[0];
            } else {
                throw StructureBuildingException(chemEl + std::wstring(L" with charge ") + charge +
                                                 std::wstring(L" is not expected to be aromatic!"));
            }
        }
        if (incomingValency < hwValency) {
            incomingValency++;
        }
    }
    boost::optional<int> defaultVal = ValencyChecker::getDefaultValency(chemEl);
    if (defaultVal) { //s or p block element
        if (defaultVal != incomingValency || charge != 0) {
            if (std::abs(incomingValency - defaultVal) == absoluteCharge) {
                atom->setProtonsExplicitlyAddedOrRemoved(incomingValency - defaultVal);
            } else {
                std::vector<boost::optional<int>> unchargedStableValencies = ValencyChecker::getPossibleValencies(
                        chemEl, 0);
                bool hasPlausibleValency = false;
                for (* : :optional < int > unchargedStableValency : unchargedStableValencies)
                {
                    if (std::abs(incomingValency - unchargedStableValency) == std::abs(charge)) {
                        atom->setProtonsExplicitlyAddedOrRemoved(incomingValency - unchargedStableValency);
                        //we strictly set the valency if a charge is specified but are more loose about things if uncharged e.g. allow penta substituted phosphine
                        if (charge != 0) {
                            atom->setLambdaConventionValency(unchargedStableValency);
                        } else {
                            atom->setMinimumValency(incomingValency);
                        }
                        hasPlausibleValency = true;
                        break;
                    }
                }
                if (!hasPlausibleValency) { //could be something like [Sn] which would be expected to be attached to later
                    atom->setMinimumValency(incomingValency);
                }
            }
        }
    } else {
        if (hydrogenCount > 0) { //make hydrogen explicit
            Fragment *frag = atom->getFrag();
            for (int i = 0; i < hydrogenCount; i++) {
                Atom *hydrogen = createAtom(ChemEl::H, frag);
                createBond(atom, hydrogen, 1);
            }
        }
    }
}

Atom *SMILESFragmentBuilder::createAtom(const std::wstring &elementSymbol, Fragment *frag) {
    return createAtom(StringHelper::fromString<ChemEl>(elementSymbol), frag);
}

Atom *SMILESFragmentBuilder::createAtom(ChemEl chemEl, Fragment *frag) {
    Atom *a = new Atom(idManager->getNextID(), chemEl, frag);
    frag->addAtom(a);
    return a;
}

Bond *SMILESFragmentBuilder::createBond(Atom *fromAtom, Atom *toAtom, int bondOrder) {
    Bond *b = new Bond(fromAtom, toAtom, bondOrder);
    fromAtom->addBond(b);
    toAtom->addBond(b);
    fromAtom->getFrag()->addBond(b);
    return b;
}

bool SMILESFragmentBuilder::is_A_to_Z(wchar_t ch) {
    return ch >= L'A' && ch <= L'Z';
}

bool SMILESFragmentBuilder::is_a_to_z(wchar_t ch) {
    return ch >= L'a' && ch <= L'z';
}

bool SMILESFragmentBuilder::is_0_to_9(wchar_t ch) {
    return ch >= L'0' && ch <= L'9';
}
