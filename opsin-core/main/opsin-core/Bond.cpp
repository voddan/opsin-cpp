#include "Bond.h"
#include "Atom.h"


using BondStereoValue = uk::ac::cam::ch::wwmm::opsin::BondStereo::BondStereoValue;

Bond::Bond(Atom *from, Atom *to, int order) : from(from), to(to) {
    if (from == to) {
        throw std::invalid_argument("Bonds must be made between different atoms");
    }
    if (order < 1 || order > 3) {
        throw std::invalid_argument("Bond order must be 1, 2 or 3");
    }
    if (from == nullptr) {
        throw std::invalid_argument("From atom was null!");
    }
    if (to == nullptr) {
        throw std::invalid_argument("To atom was null!");
    }
    this->order = order;
}

int Bond::getFrom() {
    return from->getID();
}

int Bond::getTo() {
    return to->getID();
}

int Bond::getOrder() {
    return order;
}

void Bond::setOrder(int order) {
    this->order = order;
}

Atom *Bond::getFromAtom() {
    return from;
}

Atom *Bond::getToAtom() {
    return to;
}

void Bond::addOrder(int o) {
    order += o;
}

Bond::SMILES_BOND_DIRECTION Bond::getSmilesStereochemistry() {
    return smilesBondDirection;
}

void Bond::setSmilesStereochemistry(SMILES_BOND_DIRECTION bondDirection) {
    this->smilesBondDirection = bondDirection;
}

BondStereo *Bond::getBondStereo() {
    return bondStereo;
}

void Bond::setBondStereo(BondStereo *bondStereo) {
    this->bondStereo = bondStereo;
}

void Bond::setBondStereoElement(std::vector<Atom *> &atomRefs4, BondStereoValue cOrT) {
    bondStereo = new BondStereo(atomRefs4, cOrT);
}

Atom *Bond::getOtherAtom(Atom *atom) {
    if (from == atom) {
        return to;
    } else if (to == atom) {
        return from;
    } else {
        return nullptr;
    }
}

int Bond::hashCode() {
    constexpr int prime = 31;
    int result = 1;
    result = prime * result + from->getID();
    result = prime * result + to->getID();
    return result;
}

bool Bond::equals(void *obj) {
    if (this == obj) {
        return true;
    }
    if (obj == nullptr) {
        return false;
    }
    if (getClass() != obj->getClass()) {
        return false;
    }
    Bond *other = static_cast<Bond *>(obj);

    if (from == other->from && to == other->to) {
        return true;
    }
    if (from == other->to && to == other->from) {
        return true;
    }

    return false;
}
