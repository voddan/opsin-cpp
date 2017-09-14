#include "Fragment.h"
#include "Atom.h"
#include "Bond.h"
#include "Element.h"
#include "OutAtom.h"
#include "FunctionalAtom.h"
#include "TokenEl.h"
#include "XmlDeclarations.h"
#include "StructureBuildingException.h"
#include "OpsinTools.h"
#include "FragmentTools.h"
#include "ValencyChecker.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

Fragment::Fragment(Element *tokenEl) {
    this->tokenEl = tokenEl;
}

Fragment::Fragment(const std::wstring &type) {
    this->tokenEl = new TokenEl(L"");
    this->tokenEl->addAttribute(XmlDeclarations::TYPE_ATR, type);
}

void Fragment::addAtom(Atom *atom) {
    std::vector<std::wstring> locants = atom->getLocants();
    for (auto locant : locants) {
        atomMapFromLocant[locant] = atom;
    }
    atomMapFromId[atom->getID()] = atom;
    atom->setFrag(this);
}

int Fragment::getAtomCount() {
    return atomCollection->size();
}

std::vector<Atom *> Fragment::getAtomList() {
    return std::vector<Atom *>(atomCollection);
}

void Fragment::addBond(Bond *bond) {
    bondSet->add(bond);
}

bool Fragment::removeBond(Bond *bond) {
    return bondSet->remove(bond);
}

Set<Bond *> *Fragment::getBondSet() {
    return Collections::unmodifiableSet(bondSet);
}

int Fragment::getIDFromLocant(const std::wstring &locant) {
    Atom *a = getAtomByLocant(locant);
    if (a != nullptr) {
        return a->getID();
    }
    return 0;
}

int Fragment::getIDFromLocantOrThrow(const std::wstring &locant) throw(StructureBuildingException) {
    int id = getIDFromLocant(locant);
    if (id == 0) {
        throw StructureBuildingException(std::wstring(L"Couldn't find id from locant ") + locant + std::wstring(L"."));
    }
    return id;
}

Atom *Fragment::getAtomByLocant(const std::wstring &locant) {
    Atom *a = atomMapFromLocant[locant];
    if (a != nullptr) {
        return a;
    }
    Matcher *m = OpsinTools::MATCH_AMINOACID_STYLE_LOCANT->matcher(locant);
    if (m->matches()) { //e.g. N5
        Atom *backboneAtom = atomMapFromLocant[m->group(3)]; //the atom corresponding to the numeric or greek component
        if (backboneAtom == nullptr) {
            return nullptr;
        }
        a = FragmentTools::getAtomByAminoAcidStyleLocant(backboneAtom, m->group(1), m->group(2));
        if (a != nullptr) {
            return a;
        }
    }
    return nullptr;
}

Atom *Fragment::getAtomByLocantOrThrow(const std::wstring &locant) throw(StructureBuildingException) {
    Atom *a = getAtomByLocant(locant);
    if (a == nullptr) {
        throw StructureBuildingException(
                std::wstring(L"Could not find the atom with locant ") + locant + std::wstring(L"."));
    }
    return a;
}

Atom *Fragment::getAtomByID(int id) {
    return atomMapFromId[id];
}

Atom *Fragment::getAtomByIDOrThrow(int id) throw(StructureBuildingException) {
    Atom *a = getAtomByID(id);
    if (a == nullptr) {
        throw StructureBuildingException(std::wstring(L"Couldn't find atom with id ") + id + std::wstring(L"."));
    }
    return a;
}

Bond *Fragment::findBond(int ID1, int ID2) {
    Atom *a = atomMapFromId[ID1];
    if (a != nullptr) {
        for (auto b : a->getBonds()) {
            if ((b->getFrom() == ID1 && b->getTo() == ID2) || (b->getTo() == ID1 && b->getFrom() == ID2)) {
                return b;
            }
        }
    }
    return nullptr;
}

Bond *Fragment::findBondOrThrow(int ID1, int ID2) throw(StructureBuildingException) {
    Bond *b = findBond(ID1, ID2);
    if (b == nullptr) {
        throw StructureBuildingException(L"Couldn't find specified bond");
    }
    return b;
}

int Fragment::getChainLength() {
    int length = 0;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    Atom *next = getAtomByLocant(Integer::toString(length + 1));
    Atom *previous = nullptr;
    while (next != nullptr) {
        if (previous != nullptr && previous->getBondToAtom(next) == nullptr) {
            break;
        }
        length++;
        previous = next;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        next = getAtomByLocant(Integer::toString(length + 1));
    }
    return length;
}

std::wstring Fragment::getType() {
    std::wstring type = tokenEl->getAttributeValue(XmlDeclarations::TYPE_ATR);
    return type != L"" ? type : L"";
}

std::wstring Fragment::getSubType() {
    std::wstring subType = tokenEl->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
    return subType != L"" ? subType : L"";
}

Element *Fragment::getTokenEl() {
    return tokenEl;
}

void Fragment::setTokenEl(Element *tokenEl) {
    this->tokenEl = tokenEl;
}

int Fragment::getOutAtomCount() {
    return outAtoms.size();
}

OutAtom *Fragment::getOutAtom(int i) {
    return outAtoms[i];
}

void Fragment::addOutAtom(int id, int valency, boost::optional<bool> setExplicitly) throw(StructureBuildingException) {
    addOutAtom(getAtomByIDOrThrow(id), valency, setExplicitly);
}

void Fragment::addOutAtom(Atom *atom, int valency, boost::optional<bool> setExplicitly) {
    OutAtom tempVar(atom, valency, setExplicitly);
    outAtoms.push_back(&tempVar);
}

void Fragment::incorporateOutAtoms(Fragment *frag) {
    outAtoms.insert(outAtoms.end(), frag->outAtoms.begin(), frag->outAtoms.end());
}

void Fragment::removeOutAtom(int i) {
    OutAtom *removedOutAtom = outAtoms.erase(outAtoms.begin() + i);
    if (removedOutAtom->isSetExplicitly()) {
        removedOutAtom->getAtom()->addOutValency(-removedOutAtom->getValency());
    }
}

void Fragment::removeOutAtom(OutAtom *outAtom) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    if (outAtoms.remove(outAtom) && outAtom->isSetExplicitly()) {
        outAtom->getAtom()->addOutValency(-outAtom->getValency());
    }
}

int Fragment::getFunctionalAtomCount() {
    return functionalAtoms.size();
}

FunctionalAtom *Fragment::getFunctionalAtom(int i) {
    return functionalAtoms[i];
}

void Fragment::addFunctionalAtom(Atom *atom) {
    FunctionalAtom tempVar(atom);
    functionalAtoms.push_back(&tempVar);
}

void Fragment::incorporateFunctionalAtoms(Fragment *frag) {
    functionalAtoms.insert(functionalAtoms.end(), frag->functionalAtoms.begin(), frag->functionalAtoms.end());
}

FunctionalAtom *Fragment::removeFunctionalAtom(int i) {
    return functionalAtoms.erase(functionalAtoms.begin() + i);
}

void Fragment::removeFunctionalAtom(FunctionalAtom *functionalAtom) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    functionalAtoms.remove(functionalAtom);
}

std::vector<Atom *> Fragment::getPolymerAttachmentPoints() {
    return polymerAttachmentPoints;
}

void Fragment::setPolymerAttachmentPoints(std::vector<Atom *> &polymerAttachmentPoints) {
    this->polymerAttachmentPoints = polymerAttachmentPoints;
}

std::vector<Atom *> Fragment::getIntraFragmentAtomNeighbours(Atom *atom) {
    std::vector<Atom *> results(atom->getBondCount());
    for (auto b : atom->getBonds()) {
        Atom *otherAtom = b->getOtherAtom(atom);
        if (otherAtom == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: A bond associated with an atom does not involve it");
            throw std::exception();
        }
        //If the other atom is in atomMapFromId then it is in this fragment
        if (atomMapFromId[otherAtom->getID()] != nullptr) {
            results.push_back(otherAtom);
        }
    }
    return results;
}

int Fragment::getIntraFragmentIncomingValency(Atom *atom) throw(StructureBuildingException) {
    int v = 0;
    for (auto b : atom->getBonds()) {
        //recalled atoms will be null if they are not part of this fragment
        if (b->getFromAtom() == atom) {
            Atom *a = getAtomByID(b->getTo());
            if (a != nullptr && a->getType() != XmlDeclarations::SUFFIX_TYPE_VAL) {
                v += b->getOrder();
            }
        } else if (b->getToAtom() == atom) {
            Atom *a = getAtomByID(b->getFrom());
            if (a != nullptr && a->getType() != XmlDeclarations::SUFFIX_TYPE_VAL) {
                v += b->getOrder();
            }
        } else {
            throw StructureBuildingException(L"A bond associated with an atom does not involve it");
        }
    }
    return v;
}

void Fragment::checkValencies() throw(StructureBuildingException) {
    for (auto a : atomCollection) {
        if (!ValencyChecker::checkValency(a)) {
            throw StructureBuildingException(
                    std::wstring(L"Atom is in unphysical valency state! Element: ") + a->getElement() +
                    std::wstring(L" valency: ") + std::to_wstring(a->getIncomingValency()));
        }
    }
}

void Fragment::removeAtom(Atom *atom) {
    int atomID = atom->getID();
    atomMapFromId.erase(atomID);
    for (auto l : atom->getLocants()) {
        atomMapFromLocant.erase(l);
    }
    if (defaultInAtom == atom) {
        defaultInAtom = nullptr;
    }
}

int Fragment::getCharge() {
    int charge = 0;
    for (auto a : atomCollection) {
        charge += a->getCharge();
    }
    return charge;
}

Atom *Fragment::getDefaultInAtom() {
    return defaultInAtom;
}

void Fragment::setDefaultInAtom(Atom *inAtom) {
    this->defaultInAtom = inAtom;
}

Atom *Fragment::getDefaultInAtomOrFirstAtom() {
    return defaultInAtom != nullptr ? defaultInAtom : getFirstAtom();
}

void Fragment::addMappingToAtomLocantMap(const std::wstring &locant, Atom *a) {
    atomMapFromLocant[locant] = a;
}

void Fragment::removeMappingFromAtomLocantMap(const std::wstring &locant) {
    atomMapFromLocant.erase(locant);
}

bool Fragment::hasLocant(const std::wstring &locant) {
    return getAtomByLocant(locant) != nullptr;
}

Set <std::wstring> *Fragment::getLocants() {
    return Collections::unmodifiableSet(atomMapFromLocant.keySet());
}

std::vector<Atom *> Fragment::getIndicatedHydrogen() {
    return indicatedHydrogen;
}

void Fragment::addIndicatedHydrogen(Atom *atom) {
    indicatedHydrogen.push_back(atom);
}

int Fragment::getIdOfFirstAtom() {
    return getFirstAtom()->getID();
}

Atom *Fragment::getFirstAtom() {
    Collection<Atom *>::const_iterator atomIterator = atomCollection->begin();
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
    if (atomIterator->hasNext()) {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
        return atomIterator->next();
    }
    return nullptr;
}

void Fragment::reorderAtomCollection(std::vector<Atom *> &atomList) throw(StructureBuildingException) {
    if (atomMapFromId.size() != atomList.size()) {
        throw StructureBuildingException(L"atom list is not the same size as the number of atoms in the fragment");
    }
    atomMapFromId.clear();
    for (auto atom : atomList) {
        atomMapFromId[atom->getID()] = atom;
    }
}

void Fragment::sortAtomListByLocant() throw(StructureBuildingException) {
    std::vector<Atom *> atomList = getAtomList();
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(atomList, new FragmentTools.SortByLocants());
    FragmentTools::SortByLocants tempVar();
    std::sort(atomList.begin(), atomList.end(), &tempVar);
    reorderAtomCollection(atomList);
}
