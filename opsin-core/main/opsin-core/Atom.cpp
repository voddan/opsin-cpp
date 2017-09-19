#include "Atom.h"
#include "AtomParity.h"
#include "Bond.h"
#include "PropertyKey.h"
#include "Fragment.h"
#include "ValencyChecker.h"
#include "OpsinTools.h"
#include "StructureBuildingException.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
PropertyKey<java::util::Set < Atom * >*> *
const Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT =
        new PropertyKey<java::util::Set < Atom * > * > (L"ambiguousElementAssignment");
PropertyKey<int> *const Atom::ATOM_CLASS = new PropertyKey<int>(L"atomClass");
PropertyKey<std::wstring> *const Atom::HOMOLOGY_GROUP = new PropertyKey<std::wstring>(L"homologyGroup");
PropertyKey<std::vector<Atom *>> *const Atom::POSITION_VARIATION_BOND = new PropertyKey<std::vector<Atom *>>(
        L"positionVariationBond");
PropertyKey<int> *const Atom::SMILES_HYDROGEN_COUNT = new PropertyKey<int>(L"smilesHydrogenCount");
PropertyKey<int> *const Atom::OXIDATION_NUMBER = new PropertyKey<int>(L"oxidationNumber");
PropertyKey<bool> *const Atom::ISALDEHYDE = new PropertyKey<bool>(L"isAldehyde");
PropertyKey<bool> *const Atom::ISANOMERIC = new PropertyKey<bool>(L"isAnomeric");
PropertyKey<int> *const Atom::VISITED = new PropertyKey<int>(L"visited");

Atom::Atom(int id, ChemEl chemlEl, Fragment *frag) : id(id) {
    if (frag == nullptr) {
        throw std::invalid_argument("Atom is not in a fragment!");
    }
    if (chemlEl == nullptr) {
        throw std::invalid_argument("Atom does not have an element!");
    }
    this->frag = frag;
    this->chemEl = chemlEl;
    this->type = frag->getType();
}

Atom::Atom(ChemEl chemlEl) : id(0) {
    this->chemEl = chemlEl;
}

int Atom::determineValency(bool considerOutValency) {
    if (lambdaConventionValency) {
        return lambdaConventionValency + protonsExplicitlyAddedOrRemoved;
    }
    int currentValency = getIncomingValency();
    if (considerOutValency) {
        currentValency += outValency;
    }
    boost::optional<int> calculatedMinValency = !minimumValency ? nullptr : minimumValency +
                                                                            protonsExplicitlyAddedOrRemoved;
    if (charge == 0 || protonsExplicitlyAddedOrRemoved != 0) {
        boost::optional<int> defaultValency = ValencyChecker::getDefaultValency(chemEl);
        if (defaultValency) {
            defaultValency += protonsExplicitlyAddedOrRemoved;
            if (currentValency <= defaultValency && (!calculatedMinValency || defaultValency >= calculatedMinValency)) {
                return defaultValency;
            }
        }
    }
    std::vector<boost::optional<int>> possibleValencies = ValencyChecker::getPossibleValencies(chemEl, charge);
    if (possibleValencies.size() > 0) {
        if (calculatedMinValency && calculatedMinValency >= currentValency) {
            return calculatedMinValency;
        }
        for (* : :optional < int > possibleValency : possibleValencies)
        {
            if (calculatedMinValency && possibleValency < calculatedMinValency) {
                continue;
            }
            if (currentValency <= possibleValency) {
                return possibleValency;
            }
        }
    }
    if (calculatedMinValency && calculatedMinValency >= currentValency) {
        return calculatedMinValency;
    } else {
        return currentValency;
    }
}

void Atom::addLocant(const std::wstring &locant) {
    locants.push_back(locant);
    frag->addMappingToAtomLocantMap(locant, this);
}

void Atom::replaceLocants(const std::wstring &locant) {
    clearLocants();
    addLocant(locant);
}

void Atom::removeLocant(const std::wstring &locantToRemove) {
    int locantArraySize = locants.size();
    for (int i = locantArraySize - 1; i >= 0; i--) {
        if (locants[i] == locantToRemove) {
            locants.erase(locants.begin() + i);
            frag->removeMappingFromAtomLocantMap(locantToRemove);
        }
    }
}

void Atom::clearLocants() {
    for (int i = 0, l = locants.size(); i < l; i++) {
        frag->removeMappingFromAtomLocantMap(locants[i]);
    }
    locants.clear();
}

void Atom::removeElementSymbolLocants() {
    for (int i = locants.size() - 1; i >= 0; i--) {
        std::wstring locant = locants[i];
        if (OpsinTools::MATCH_ELEMENT_SYMBOL_LOCANT->matcher(locant).matches()) {
            frag->removeMappingFromAtomLocantMap(locant);
            locants.erase(locants.begin() + i);
        }
    }
}

void Atom::removeLocantsOtherThanElementSymbolLocants() {
    for (int i = locants.size() - 1; i >= 0; i--) {
        std::wstring locant = locants[i];
        if (!OpsinTools::MATCH_ELEMENT_SYMBOL_LOCANT->matcher(locant).matches()) {
            frag->removeMappingFromAtomLocantMap(locant);
            locants.erase(locants.begin() + i);
        }
    }
}

bool Atom::hasLocant(const std::wstring &locant) {
    if (std::find(locants.begin(), locants.end(), locant) != locants.end()) {
        return true;
    }
    Matcher *m = OpsinTools::MATCH_AMINOACID_STYLE_LOCANT->matcher(locant);
    if (m->matches()) { //e.g. N'5
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (chemEl::toString()->equals(m->group(1))) { //element symbol
            if (!m->group(2).equals(L"") && (!hasLocant(m->group(1) + m->group(2)))) { //has primes
                return false; //must have exact locant e.g. N'
            }
            if (OpsinTools::depthFirstSearchForNonSuffixAtomWithLocant(this, m->group(3)) != nullptr) {
                return true;
            }
        }
    }
    return false;
}

std::wstring Atom::getFirstLocant() {
    return locants.size() > 0 ? locants[0] : nullptr;
}

std::vector<std::wstring> Atom::getLocants() {
    return Collections::unmodifiableList(locants);
}

std::vector<std::wstring> Atom::getElementSymbolLocants() {
    std::vector<std::wstring> elementSymbolLocants(1);
    for (int i = 0, l = locants.size(); i < l; i++) {
        std::wstring locant = locants[i];
        if (OpsinTools::MATCH_ELEMENT_SYMBOL_LOCANT->matcher(locant).matches()) {
            elementSymbolLocants.push_back(locant);
        }
    }
    return elementSymbolLocants;
}

void Atom::setFrag(Fragment *f) {
    frag = f;
}

Fragment *Atom::getFrag() {
    return frag;
}

int Atom::getID() {
    return id;
}

ChemEl Atom::getElement() {
    return chemEl;
}

void Atom::setElement(ChemEl chemEl) {
    this->chemEl = chemEl;
}

int Atom::getCharge() {
    return charge;
}

void Atom::addChargeAndProtons(int charge, int protons) {
    this->charge += charge;
    protonsExplicitlyAddedOrRemoved += protons;
}

void Atom::setCharge(int c) {
    charge = c;
}

void Atom::neutraliseCharge() {
    charge = 0;
    protonsExplicitlyAddedOrRemoved = 0;
}

boost::optional<int> Atom::getIsotope() {
    return isotope;
}

void Atom::setIsotope(boost::optional<int> isotope) {
    if (isotope && isotope < chemEl::ATOMIC_NUM) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Isotopic mass cannot be less than the element's number of protons: " + chemEl.toString() + " " + isotope + " < " + chemEl.ATOMIC_NUM);
        throw std::exception();
    }
    this->isotope = isotope;
}

void Atom::addBond(Bond *b) {
    if (std::find(bonds.begin(), bonds.end(), b) != bonds.end()) {
        throw std::invalid_argument(
                "Atom already has given bond (This is not allowed as this would give two bonds between the same atoms!)");
    }
    bonds.push_back(b);
}

bool Atom::removeBond(Bond *b) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    return bonds.remove(b);
}

int Atom::getIncomingValency() {
    int v = 0;
    for (int i = 0, l = bonds.size(); i < l; i++) {
        v += bonds[i]->getOrder();
    }
    return v;
}

int Atom::getProtonsExplicitlyAddedOrRemoved() {
    return protonsExplicitlyAddedOrRemoved;
}

void Atom::setProtonsExplicitlyAddedOrRemoved(int protonsExplicitlyAddedOrRemoved) {
    this->protonsExplicitlyAddedOrRemoved = protonsExplicitlyAddedOrRemoved;
}

bool Atom::hasSpareValency() {
    return spareValency;
}

void Atom::setSpareValency(bool sv) {
    spareValency = sv;
}

int Atom::getOutValency() {
    return outValency;
}

void Atom::addOutValency(int outV) {
    outValency += outV;
}

std::vector<Bond *> Atom::getBonds() {
    return Collections::unmodifiableList(bonds);
}

int Atom::getBondCount() {
    return bonds.size();
}

std::vector<Atom *> Atom::getAtomNeighbours() {
    int bondCount = bonds.size();
    std::vector<Atom *> results(bondCount);
    for (int i = 0; i < bondCount; i++) {
        results.push_back(bonds[i]->getOtherAtom(this));
    }
    return results;
}

boost::optional<int> Atom::getLambdaConventionValency() {
    return lambdaConventionValency;
}

void Atom::setLambdaConventionValency(boost::optional<int> valency) {
    this->lambdaConventionValency = valency;
}

std::wstring Atom::getType() {
    return type;
}

void Atom::setType(const std::wstring &type) {
    this->type = type;
}

bool Atom::getAtomIsInACycle() {
    return atomIsInACycle;
}

void Atom::setAtomIsInACycle(bool atomIsInACycle) {
    this->atomIsInACycle = atomIsInACycle;
}

AtomParity *Atom::getAtomParity() {
    return atomParity;
}

void Atom::setAtomParity(AtomParity *atomParity) {
    this->atomParity = atomParity;
}

void Atom::setAtomParity(std::vector<Atom *> &atomRefs4, int parity) {
    atomParity = new AtomParity(atomRefs4, parity);
}

boost::optional<int> Atom::getMinimumValency() {
    return minimumValency;
}

void Atom::setMinimumValency(boost::optional<int> minimumValency) {
    this->minimumValency = minimumValency;
}

template<typename T>
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unchecked") <T> T getProperty(PropertyKey<T> propertyKey)
T Atom::getProperty(PropertyKey<T> *propertyKey) {
    return static_cast<T>(properties[propertyKey]);
}

template<typename T>
void Atom::setProperty(PropertyKey<T> *propertyKey, T value) {
    properties[propertyKey] = value;
}

void Atom::ensureSVIsConsistantWithValency(bool takeIntoAccountExternalBonds) throw(StructureBuildingException) {
    if (spareValency) {
        boost::optional<int> maxValency;
        if (lambdaConventionValency) {
            maxValency = lambdaConventionValency + protonsExplicitlyAddedOrRemoved;
        } else {
            boost::optional<int> hwValency = ValencyChecker::getHWValency(chemEl);
            if (!hwValency) {
                throw StructureBuildingException(chemEl + std::wstring(L" is not expected to be aromatic!"));
            }
            maxValency = hwValency + protonsExplicitlyAddedOrRemoved;
        }
        int maxSpareValency;
        if (takeIntoAccountExternalBonds) {
            maxSpareValency = maxValency - getIncomingValency() - outValency;
        } else {
            maxSpareValency = maxValency - frag->getIntraFragmentIncomingValency(this);
        }
        if (maxSpareValency < 1) {
            setSpareValency(false);
        }
    }
}

Bond *Atom::getFirstBond() {
    if (bonds.size() > 0) {
        return bonds[0];
    }
    return nullptr;
}

Bond *Atom::getBondToAtom(Atom *a) {
    for (int i = 0, l = bonds.size(); i < l; i++) {
        Bond *b = bonds[i];
        if (b->getOtherAtom(this) == a) {
            return b;
        }
    }
    return nullptr;
}

Bond *Atom::getBondToAtomOrThrow(Atom *a) throw(StructureBuildingException) {
    Bond *b = getBondToAtom(a);
    if (b == nullptr) {
        throw StructureBuildingException(L"Couldn't find specified bond");
    }
    return b;
}