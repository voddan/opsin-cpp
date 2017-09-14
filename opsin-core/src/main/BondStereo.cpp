#include "BondStereo.h"
#include "Atom.h"


BondStereo::BondStereo(std::vector<Atom *> &atomRefs4, BondStereoValue cOrT) {
    if (atomRefs4.size() != 4) {
        throw std::invalid_argument("atomRefs4 must contain references to 4 atoms");
    }
    this->atomRefs4 = atomRefs4;
    this->bondStereoValue = cOrT;
}

std::vector<Atom *> BondStereo::getAtomRefs4() {
    return atomRefs4;
}

void BondStereo::setAtomRefs4(std::vector<Atom *> &atomRefs4) {
    this->atomRefs4 = atomRefs4;
}

BondStereo::BondStereoValue BondStereo::getBondStereoValue() {
    return bondStereoValue;
}

void BondStereo::setBondStereoValue(BondStereoValue bondStereoValue) {
    this->bondStereoValue = bondStereoValue;
}
