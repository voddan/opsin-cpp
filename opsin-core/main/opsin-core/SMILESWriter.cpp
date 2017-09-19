#include "SMILESWriter.h"
#include "Bond.h"
#include "Atom.h"
#include "Fragment.h"
#include "StringTools.h"
#include "BondStereo.h"
#include "AtomParity.h"
#include "StereochemistryHandler.h"


using SMILES_BOND_DIRECTION = Bond::SMILES_BOND_DIRECTION;
using BondStereoValue = BondStereo::BondStereoValue;
const std::unordered_map<ChemEl, std::vector<Integer>> SMILESWriter::organicAtomsToStandardValencies = new java::util::EnumMap<ChemEl, std::vector<Integer>>(
        ChemEl::
typeid);
const std::vector<std::wstring> SMILESWriter::closureSymbols = std::vector<std::wstring>();

SMILESWriter::StaticConstructor::StaticConstructor() {
    organicAtomsToStandardValencies[ChemEl::B] = std::vector<boost::optional<int> *>{3};
    organicAtomsToStandardValencies[ChemEl::C] = std::vector<boost::optional<int> *>{4};
    organicAtomsToStandardValencies[ChemEl::N] = std::vector<boost::optional<int> *>{3,
                                                                                     5}; //note that OPSIN doesn't accept valency 5 nitrogen without the lambda convention
    organicAtomsToStandardValencies[ChemEl::O] = std::vector<boost::optional<int> *>{2};
    organicAtomsToStandardValencies[ChemEl::P] = std::vector<boost::optional<int> *>{3, 5};
    organicAtomsToStandardValencies[ChemEl::S] = std::vector<boost::optional<int> *>{2, 4, 6};
    organicAtomsToStandardValencies[ChemEl::F] = std::vector<boost::optional<int> *>{1};
    organicAtomsToStandardValencies[ChemEl::Cl] = std::vector<boost::optional<int> *>{1};
    organicAtomsToStandardValencies[ChemEl::Br] = std::vector<boost::optional<int> *>{1};
    organicAtomsToStandardValencies[ChemEl::I] = std::vector<boost::optional<int> *>{1};

    organicAtomsToStandardValencies[ChemEl::R] = std::vector<boost::optional<int> *>{1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 1; i <= 9; i++) {
        closureSymbols.push_back(std::to_wstring(i));
    }
    for (int i = 10; i <= 99; i++) {
        closureSymbols.push_back(std::wstring(L"%") + std::to_wstring(i));
    }
    closureSymbols.push_back(L"0");
}

SMILESWriter::StaticConstructor SMILESWriter::staticConstructor;

SMILESWriter::SMILESWriter(Fragment *structure, bool outputExtendedSmiles) : structure(structure), outputExtendedSmiles(
        outputExtendedSmiles) {
}

std::wstring SMILESWriter::generateSmiles(Fragment *structure) {
    SMILESWriter tempVar(structure, false);
    return (&tempVar)->writeSmiles();
}

std::wstring SMILESWriter::generateExtendedSmiles(Fragment *structure) {
    SMILESWriter tempVar(structure, true);
    return (&tempVar)->writeSmiles();
}

std::wstring SMILESWriter::writeSmiles() {
    assignSmilesOrder();
    assignDoubleBondStereochemistrySlashes();

    std::vector<Atom *> atomList = structure->getAtomList();
    smilesOutputOrder = std::vector<Atom *>(atomList.size());

    bool isEmpty = true;
    for (auto currentAtom : atomList) {
        boost::optional<int> visitedDepth = currentAtom->getProperty(Atom::VISITED);
        if (visitedDepth && visitedDepth == 0) { //new component
            if (!isEmpty) {
                smilesBuilder->append(L'.');
            }
            traverseSmiles(currentAtom);
            isEmpty = false;
        }
    }

    if (outputExtendedSmiles) {
        writeExtendedSmilesLayer();
    }

    return smilesBuilder->toString();
}

void SMILESWriter::writeExtendedSmilesLayer() {
    std::vector<std::wstring> atomLabels;
    std::vector<std::wstring> positionVariationBonds;
    boost::optional<int> lastLabel = boost::none;
    int attachmentPointCounter = 1;
    std::set<int> *seenAttachmentpoints = std::unordered_set<int>();
    std::vector<Atom *> polymerAttachPoints = structure->getPolymerAttachmentPoints();
    bool isPolymer = polymerAttachPoints.size() > 0 && polymerAttachPoints.size() > 0;
    for (int i = 0, l = smilesOutputOrder.size(); i < l; i++) {
        Atom *a = smilesOutputOrder[i];
        std::wstring homologyGroup = a->getProperty(Atom::HOMOLOGY_GROUP);
        if (homologyGroup != L"") {
            homologyGroup = escapeExtendedSmilesLabel(homologyGroup);
            if (boost::starts_with(homologyGroup, L"_")) {
                atomLabels.push_back(homologyGroup);
            } else {
                atomLabels.push_back(homologyGroup + std::wstring(L"_p"));
            }
            lastLabel = i;
        } else if (a->getElement() == ChemEl::R) {
            if (isPolymer) {
                atomLabels.push_back(L"star_e");
            } else {
                boost::optional<int> atomClass = a->getProperty(Atom::ATOM_CLASS);
                if (atomClass) {
                    seenAttachmentpoints->add(atomClass);
                } else {
                    do {
                        atomClass = attachmentPointCounter++;
                    } while (seenAttachmentpoints->contains(atomClass));
                }
                atomLabels.push_back(std::wstring(L"_AP") + static_cast<std::wstring>(atomClass));
            }
            lastLabel = i;
        } else {
            atomLabels.push_back(L"");
        }
        std::vector<Atom *> atomsInPositionVariationBond = a->getProperty(Atom::POSITION_VARIATION_BOND);
        if (atomsInPositionVariationBond.size() > 0) {
            StringBuilder *sb = new StringBuilder();
            sb->append(i);
            for (int j = 0; j < atomsInPositionVariationBond.size(); j++) {
                sb->append(j == 0 ? L':' : L'.');
                Atom *referencedAtom = atomsInPositionVariationBond[j];
                int referencedAtomIndex = smilesOutputOrder.find(referencedAtom);
                if (referencedAtomIndex == -1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Failed to resolve position variation bond atom");
                    throw std::exception();
                }
                sb->append(referencedAtomIndex);
            }
            positionVariationBonds.push_back(sb->toString());
        }
    }
    std::vector<std::wstring> extendedSmiles(2);
    if (lastLabel) {
        extendedSmiles.push_back(
                std::wstring(L"$") + StringTools::stringListToString(atomLabels.subList(0, lastLabel + 1), L";") +
                std::wstring(L"$"));
    }
    if (positionVariationBonds.size() > 0) {
        extendedSmiles.push_back(std::wstring(L"m:") + StringTools::stringListToString(positionVariationBonds, L","));
    }
    if (isPolymer) {
        StringBuilder *sruContents = new StringBuilder();
        sruContents->append(L"Sg:n:");
        bool appendDelimiter = false;
        for (int i = 0, l = smilesOutputOrder.size(); i < l; i++) {
            if (smilesOutputOrder[i]->getElement() != ChemEl::R) {
                if (appendDelimiter) {
                    sruContents->append(L',');
                }
                sruContents->append(i);
                appendDelimiter = true;
            }
        }
        sruContents->append(L"::ht");
        extendedSmiles.push_back(sruContents->toString());
    }
    if (extendedSmiles.size() > 0) {
        smilesBuilder->append(L" |");
        smilesBuilder->append(StringTools::stringListToString(extendedSmiles, L","));
        smilesBuilder->append(L'|');
    }
}

std::wstring SMILESWriter::escapeExtendedSmilesLabel(const std::wstring &str) {
    StringBuilder *sb = new StringBuilder();
    for (int i = 0, len = str.length(); i < len; i++) {
        wchar_t ch = str[i];
        if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9')) {
            sb->append(ch);
        } else {
            sb->append(L"&#");
            sb->append(std::to_wstring(static_cast<int>(ch)));
            sb->append(L';');
        }
    }
    return sb->toString();
}

void SMILESWriter::assignSmilesOrder() {
    std::vector<Atom *> atomList = structure->getAtomList();
    for (auto atom : atomList) {
        atom->setProperty(Atom::VISITED, nullptr);
    }
    for (auto a : atomList) {
        if (a->getProperty(Atom::VISITED) == nullptr &&
            !isSmilesImplicitProton(a)) { //true for only the first atom in a fully connected molecule
            traverseMolecule(a);
        }
    }
}

SMILESWriter::TraversalState::TraversalState(Atom *atom, Bond *bondTaken, int depth) : atom(atom), bondTaken(bondTaken),
                                                                                       depth(depth) {
}

void SMILESWriter::traverseMolecule(Atom *startingAtom) {
    Deque < TraversalState * > *stack = new ArrayDeque<TraversalState *>();
    TraversalState tempVar(startingAtom, nullptr, 0);
    stack->add(&tempVar);
    while (!stack->isEmpty()) {
        TraversalState *currentstate = stack->removeLast();
        Atom *currentAtom = currentstate->atom;
        Bond *bondtaken = currentstate->bondTaken;
        if (bondtaken != nullptr) {
            bondToNextAtomMap[bondtaken] = currentAtom;
        }
        if (currentAtom->getProperty(Atom::VISITED) != nullptr) {
            continue;
        }
        int depth = currentstate->depth;
        currentAtom->setProperty(Atom::VISITED, depth);
        std::vector<Bond *> bonds = currentAtom->getBonds();
        for (int i = bonds.size() - 1; i >= 0; i--) {
            Bond *bond = bonds[i];
            if (bond->equals(bondtaken)) {
                continue;
            }
            Atom *neighbour = bond->getOtherAtom(currentAtom);
            if (isSmilesImplicitProton(neighbour)) {
                continue;
            }
            TraversalState tempVar2(neighbour, bond, depth + 1);
            stack->add(&tempVar2);
        }
    }
}

bool SMILESWriter::isSmilesImplicitProton(Atom *atom) {
    if (atom->getElement() != ChemEl::H) {
        //not hydrogen
        return false;
    }
    if (atom->getIsotope() && atom->getIsotope() != 1) {
        //deuterium/tritium
        return false;
    }
    std::vector<Atom *> neighbours = atom->getAtomNeighbours();
    int neighbourCount = neighbours.size();
    if (neighbourCount > 1) {
        //bridging hydrogen
        return false;
    }
    if (neighbourCount == 0) {
        //just a hydrogen atom
        return false;
    }

    Atom *neighbour = neighbours[0];
    ChemEl chemEl = neighbour->getElement();
    if (chemEl == ChemEl::H || chemEl == ChemEl::R) {
        //only connects to hydrogen or an R-group
        return false;
    }
    if (chemEl == ChemEl::N) {
        std::vector<Bond *> bondsFromNitrogen = neighbour->getBonds();
        if (bondsFromNitrogen.size() == 2) {
            for (auto bond : bondsFromNitrogen) {
                if (bond->getBondStereo() != nullptr) {
                    //special case where hydrogen is connected to a nitrogen with imine double bond stereochemistry
                    return false;
                }
            }
        }
    }
    return true;
}

void SMILESWriter::assignDoubleBondStereochemistrySlashes() {
    Set < Bond * > *bonds = bondToNextAtomMap.keySet();
    for (auto bond : bonds) {
        bond->setSmilesStereochemistry(nullptr);
    }
    for (auto bond : bonds) {
        BondStereo *bondStereo = bond->getBondStereo();
        if (bondStereo != nullptr) {
            std::vector<Atom *> atomRefs4 = bondStereo->getAtomRefs4();
            Bond *bond1 = atomRefs4[0]->getBondToAtom(atomRefs4[1]);
            Bond *bond2 = atomRefs4[2]->getBondToAtom(atomRefs4[3]);
            if (bond1 == nullptr || bond2 == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Bondstereo described atoms that are not bonded");
                throw std::exception();
            }
            Atom *bond1ToAtom = bondToNextAtomMap[bond1];
            Atom *bond2ToAtom = bondToNextAtomMap[bond2];
            SMILES_BOND_DIRECTION bond1Slash = bond1->getSmilesStereochemistry(); //null except in conjugated systems
            SMILES_BOND_DIRECTION bond2Slash = bond2->getSmilesStereochemistry();
            SMILES_BOND_DIRECTION bond1Direction = SMILES_BOND_DIRECTION::LSLASH;
            SMILES_BOND_DIRECTION bond2Direction = SMILES_BOND_DIRECTION::LSLASH;
            if (bondStereo->getBondStereoValue()::equals(BondStereoValue::CIS))
            {
                bond2Direction = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH; //flip the slash type to be used from \ to /
            }
            if (!bond1ToAtom->equals(atomRefs4[1])) {
                bond1Direction = bond1Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
            }
            if (!bond2ToAtom->equals(atomRefs4[3])) {
                bond2Direction = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
            }

            //One of the bonds may have already have a defined slash from a previous bond stereo. If so make sure that we don't change it.
            if (bond1Slash != nullptr && !bond1Slash::equals(bond1Direction)) {
                bond1Direction = bond1Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
                bond2Direction = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
            } else if (bond2Slash != nullptr && !bond2Slash::equals(bond2Direction)) {
                bond1Direction = bond1Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
                bond2Direction = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
            }

            //Also need to investigate the bonds which are implicitly set by the bondStereo
            //F   Cl
            // C=C
            //N   O
            //e.g. the bonds from the C-N and C-O (the higher priority atoms will always be used for bond1/2)
            Bond *bond1Other = nullptr;
            Bond *bond2Other = nullptr;
            SMILES_BOND_DIRECTION bond1OtherDirection = nullptr;
            SMILES_BOND_DIRECTION bond2OtherDirection = nullptr;

            std::vector<Bond *> bondsFrom2ndAtom(atomRefs4[1]->getBonds());
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            bondsFrom2ndAtom.remove(bond1);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            bondsFrom2ndAtom.remove(bond);
            if (bondsFrom2ndAtom.size() == 1) { //can be 0 for imines
                if (bondToNextAtomMap.find(bondsFrom2ndAtom[0]) !=
                    bondToNextAtomMap.end()) { //ignore bonds to implicit hydrogen
                    bond1Other = bondsFrom2ndAtom[0];
                    bond1OtherDirection = bond1Direction::equals(SMILES_BOND_DIRECTION::LSLASH)
                                          ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                    if (!bond1ToAtom->equals(atomRefs4[1])) {
                        bond1OtherDirection = bond1OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                              ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                    }
                    if (!bondToNextAtomMap[bond1Other]->equals(atomRefs4[1])) {
                        bond1OtherDirection = bond1OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                              ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                    }
                }
            }

            std::vector<Bond *> bondsFrom3rdAtom(atomRefs4[2]->getBonds());
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            bondsFrom3rdAtom.remove(bond2);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            bondsFrom3rdAtom.remove(bond);
            if (bondsFrom3rdAtom.size() == 1) {
                if (bondToNextAtomMap.find(bondsFrom3rdAtom[0]) != bondToNextAtomMap.end()) {
                    bond2Other = bondsFrom3rdAtom[0];
                    bond2OtherDirection = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH)
                                          ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                    if (!bond2ToAtom->equals(atomRefs4[3])) {
                        bond2OtherDirection = bond2OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                              ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                    }
                    if (!bondToNextAtomMap[bond2Other]->equals(bond2Other->getOtherAtom(atomRefs4[2]))) {
                        bond2OtherDirection = bond2OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                              ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                    }
                }
            }

            //One of the bonds may have already have a defined slash from a previous bond stereo. If so make sure that we don't change it.
            if (bond1Other != nullptr && bond1Other->getSmilesStereochemistry() != nullptr &&
                !bond1Other->getSmilesStereochemistry()
                ::equals(bond1OtherDirection))
            {
                bond1Direction = bond1Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
                bond2Direction = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
                bond1OtherDirection = bond1OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                      ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                if (bond2Other != nullptr) {
                    bond2OtherDirection = bond2OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                          ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                }
            }
            else if (bond2Other != nullptr && bond2Other->getSmilesStereochemistry() != nullptr &&
                     !bond2Other->getSmilesStereochemistry()
                ::equals(bond2OtherDirection))
            {
                bond1Direction = bond1Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
                bond2Direction = bond2Direction::equals(SMILES_BOND_DIRECTION::LSLASH) ? SMILES_BOND_DIRECTION::RSLASH
                                                                                       : SMILES_BOND_DIRECTION::LSLASH;
                bond2OtherDirection = bond2OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                      ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                if (bond1Other != nullptr) {
                    bond1OtherDirection = bond1OtherDirection::equals(SMILES_BOND_DIRECTION::LSLASH)
                                          ? SMILES_BOND_DIRECTION::RSLASH : SMILES_BOND_DIRECTION::LSLASH;
                }
            }

            //Set slashes for all bonds that are not to implicit hydrogen
            //In non conjugated systems this will yield redundant, but consistent, information
            bond1->setSmilesStereochemistry(bond1Direction);
            bond2->setSmilesStereochemistry(bond2Direction);

            if (bond1Other != nullptr) {
                bond1Other->setSmilesStereochemistry(bond1OtherDirection);
            }
            if (bond2Other != nullptr) {
                bond2Other->setSmilesStereochemistry(bond2OtherDirection);
            }
        }
    }
}

void SMILESWriter::traverseSmiles(Atom *startingAtom) {
    Deque < TraversalState * > *stack = new ArrayDeque<TraversalState *>();
    TraversalState tempVar(startingAtom, nullptr, 0);
    stack->add(&tempVar);
    while (!stack->isEmpty()) {
        TraversalState *currentstate = stack->removeLast();
        if (currentstate == startBranch) {
            smilesBuilder->append(L'(');
            continue;
        }
        if (currentstate == endBranch) {
            smilesBuilder->append(L')');
            continue;
        }
        Atom *currentAtom = currentstate->atom;
        Bond *bondtaken = currentstate->bondTaken;
        if (bondtaken != nullptr) {
            smilesBuilder->append(bondToSmiles(bondtaken));
        }
        int depth = currentstate->depth;

        smilesBuilder->append(atomToSmiles(currentAtom, depth, bondtaken));
        smilesOutputOrder.push_back(currentAtom);
        std::vector<Bond *> bonds = currentAtom->getBonds();
        std::vector<std::wstring> newlyAvailableClosureSymbols;
        for (auto bond : bonds) { //ring closures
            if (bond->equals(bondtaken)) {
                continue;
            }
            Atom *neighbour = bond->getOtherAtom(currentAtom);
            boost::optional<int> nDepth = neighbour->getProperty(Atom::VISITED);
            if (nDepth && nDepth <= depth) {
                std::wstring closure = bondToClosureSymbolMap[bond];
                smilesBuilder->append(closure);
                if (newlyAvailableClosureSymbols.empty()) {
                    newlyAvailableClosureSymbols = std::vector<std::wstring>();
                }
                newlyAvailableClosureSymbols.push_back(closure);
            }
        }
        for (auto bond : bonds) { //ring openings
            Atom *neighbour = bond->getOtherAtom(currentAtom);
            boost::optional<int> nDepth = neighbour->getProperty(Atom::VISITED);
            if (nDepth && nDepth > (depth + 1)) {
                std::wstring closure = availableClosureSymbols->removeFirst();
                bondToClosureSymbolMap[bond] = closure;
                smilesBuilder->append(bondToSmiles(bond));
                smilesBuilder->append(closure);
            }
        }

        if (newlyAvailableClosureSymbols.size() > 0) {
            //By not immediately adding to availableClosureSymbols we avoid using the same digit
            //to both close and open on the same atom
            for (int i = newlyAvailableClosureSymbols.size() - 1; i >= 0; i--) {
                availableClosureSymbols->addFirst(newlyAvailableClosureSymbols[i]);
            }
        }

        bool seenFirstBranch = false;
        for (int i = bonds.size() - 1; i >= 0; i--) {
            //adjacent atoms which have not been previously written
            Bond *bond = bonds[i];
            Atom *neighbour = bond->getOtherAtom(currentAtom);
            boost::optional<int> nDepth = neighbour->getProperty(Atom::VISITED);
            if (nDepth && nDepth == depth + 1) {
                if (!seenFirstBranch) {
                    TraversalState tempVar2(neighbour, bond, depth + 1);
                    stack->add(&tempVar2);
                    seenFirstBranch = true;
                } else {
                    stack->add(endBranch);
                    TraversalState tempVar3(neighbour, bond, depth + 1);
                    stack->add(&tempVar3);
                    stack->add(startBranch);
                }
            }
        }
    }
}

std::wstring SMILESWriter::atomToSmiles(Atom *atom, int depth, Bond *bondtaken) {
    StringBuilder *atomSmiles = new StringBuilder();
    int hydrogenCount = calculateNumberOfBondedExplicitHydrogen(atom);
    bool needsSquareBrackets = determineWhetherAtomNeedsSquareBrackets(atom, hydrogenCount);
    if (needsSquareBrackets) {
        atomSmiles->append(L'[');
    }
    if (atom->getIsotope()) {
        atomSmiles->append(atom->getIsotope());
    }
    ChemEl chemEl = atom->getElement();
    if (chemEl == ChemEl::R) { //used for polymers
        atomSmiles->append(L'*');
    } else {
        if (atom->hasSpareValency()) { //spare valency corresponds directly to lower case SMILES in OPSIN's SMILES reader
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            atomSmiles->append(chemEl::toString()->toLowerCase(Locale::ROOT));
        } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            atomSmiles->append(chemEl::toString());
        }
    }
    if (atom->getAtomParity() != nullptr) {
        atomSmiles->append(atomParityToSmiles(atom, depth, bondtaken));
    }
    if (hydrogenCount != 0 && needsSquareBrackets && chemEl != ChemEl::H) {
        atomSmiles->append(L'H');
        if (hydrogenCount != 1) {
            atomSmiles->append(std::to_wstring(hydrogenCount));
        }
    }
    int charge = atom->getCharge();
    if (charge != 0) {
        if (charge == 1) {
            atomSmiles->append(L'+');
        } else if (charge == -1) {
            atomSmiles->append(L'-');
        } else {
            if (charge > 0) {
                atomSmiles->append(L'+');
            }
            atomSmiles->append(charge);
        }
    }
    if (needsSquareBrackets) {
        boost::optional<int> atomClass = atom->getProperty(Atom::ATOM_CLASS);
        if (atomClass) {
            atomSmiles->append(L':');
            atomSmiles->append(static_cast<std::wstring>(atomClass));
        }
        atomSmiles->append(L']');
    }
    return atomSmiles->toString();
}

int SMILESWriter::calculateNumberOfBondedExplicitHydrogen(Atom *atom) {
    std::vector<Atom *> neighbours = atom->getAtomNeighbours();
    int count = 0;
    for (auto neighbour : neighbours) {
        if (neighbour->getProperty(Atom::VISITED) == nullptr) {
            count++;
        }
    }
    return count;
}

bool SMILESWriter::determineWhetherAtomNeedsSquareBrackets(Atom *atom, int hydrogenCount) {
    std::vector<boost::optional<int>> expectedValencies = organicAtomsToStandardValencies[atom->getElement()];
    if (expectedValencies.empty()) {
        return true;
    }
    if (atom->getCharge() != 0) {
        return true;
    }
    if (atom->getIsotope()) {
        return true;
    }
    if (atom->getAtomParity() != nullptr) {
        return true;
    }

    int valency = atom->getIncomingValency();
    bool valencyCanBeDescribedImplicitly = Arrays::binarySearch(expectedValencies, valency) >= 0;
    int targetImplicitValency = valency;
    if (valency > expectedValencies[expectedValencies.size() - 1]) {
        valencyCanBeDescribedImplicitly = true;
    }
    if (!valencyCanBeDescribedImplicitly) {
        return true;
    }

    int nonHydrogenValency = valency - hydrogenCount;
    int implicitValencyThatWouldBeGenerated = nonHydrogenValency;
    for (int i = expectedValencies.size() - 1; i >= 0; i--) {
        if (expectedValencies[i] >= nonHydrogenValency) {
            implicitValencyThatWouldBeGenerated = expectedValencies[i];
        }
    }
    if (targetImplicitValency != implicitValencyThatWouldBeGenerated) {
        return true;
    }
    if (atom->getProperty(Atom::ATOM_CLASS) != nullptr) {
        return true;
    }
    return false;
}

std::wstring SMILESWriter::atomParityToSmiles(Atom *currentAtom, int depth, Bond *bondtaken) {
    AtomParity *atomParity = currentAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4().clone();

    std::vector<Atom *> atomrefs4Current;

    if (bondtaken != nullptr) { //previous atom
        Atom *neighbour = bondtaken->getOtherAtom(currentAtom);
        atomrefs4Current.push_back(neighbour);
    }

    for (auto atom : atomRefs4) { //lone pair as in tetrahedral sulfones
        if (atom->equals(currentAtom)) {
            atomrefs4Current.push_back(currentAtom);
        }
    }

    std::vector<Bond *> bonds = currentAtom->getBonds();
    for (auto bond : bonds) { //implicit hydrogen
        Atom *neighbour = bond->getOtherAtom(currentAtom);
        if (neighbour->getProperty(Atom::VISITED) == nullptr) {
            atomrefs4Current.push_back(currentAtom);
        }
    }
    for (auto bond : bonds) { //ring closures
        if (bond->equals(bondtaken)) {
            continue;
        }
        Atom *neighbour = bond->getOtherAtom(currentAtom);
        if (neighbour->getProperty(Atom::VISITED) == nullptr) {
            continue;
        }
        if (neighbour->getProperty(Atom::VISITED) <= depth) {
            atomrefs4Current.push_back(neighbour);
        }
    }
    for (auto bond : bonds) { //ring openings
        Atom *neighbour = bond->getOtherAtom(currentAtom);
        if (neighbour->getProperty(Atom::VISITED) == nullptr) {
            continue;
        }
        if (neighbour->getProperty(Atom::VISITED) > (depth + 1)) {
            atomrefs4Current.push_back(neighbour);
        }

    }
    for (auto bond : bonds) { //next atom/s
        Atom *neighbour = bond->getOtherAtom(currentAtom);
        if (neighbour->getProperty(Atom::VISITED) == nullptr) {
            continue;
        }
        if (neighbour->getProperty(Atom::VISITED) == depth + 1) {
            atomrefs4Current.push_back(neighbour);
        }
    }
    std::vector<Atom *> atomrefs4CurrentArr(4);
    for (int i = 0; i < atomrefs4Current.size(); i++) {
        atomrefs4CurrentArr[i] = atomrefs4Current[i];
    }
    for (int i = 0; i <
                    atomRefs4.size(); i++) { //replace mentions of explicit hydrogen with the central atom the hydrogens are attached to, to be consistent with the SMILES representation
        if (atomRefs4[i]->getProperty(Atom::VISITED) == nullptr) {
            atomRefs4[i] = currentAtom;
        }
    }

    bool equivalent = StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(atomRefs4, atomParity->getParity(),
                                                                                    atomrefs4CurrentArr, 1);
    if (equivalent) {
        return L"@@";
    } else {
        return L"@";
    }
}

std::wstring SMILESWriter::bondToSmiles(Bond *bond) {
    std::wstring bondSmiles = L"";
    int bondOrder = bond->getOrder();
    if (bondOrder == 2) {
        bondSmiles = L"=";
    } else if (bondOrder == 3) {
        bondSmiles = L"#";
    } else if (bond->getSmilesStereochemistry() != nullptr) {
        if (bond->getSmilesStereochemistry() == SMILES_BOND_DIRECTION::RSLASH) {
            bondSmiles = L"/";
        } else {
            bondSmiles = L"\\";
        }
    }
    return bondSmiles;
}
