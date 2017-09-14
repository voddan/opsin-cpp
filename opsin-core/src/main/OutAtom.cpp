#include "OutAtom.h"
#include "Atom.h"


OutAtom::OutAtom(Atom *atom, int valency, boost::optional<bool> setExplicitly) : OutAtom(atom, valency, setExplicitly,
                                                                                         nullptr) {
}

OutAtom::OutAtom(Atom *atom, int valency, boost::optional<bool> setExplicitly, const std::wstring &locant) {
    this->atom = atom;
    this->valency = valency;
    this->setExplicitly = setExplicitly;
    this->locant = locant;
    if (setExplicitly) {
        atom->addOutValency(valency);
    }
}

Atom *OutAtom::getAtom() {
    return atom;
}

void OutAtom::setAtom(Atom *atom) {
    this->atom = atom;
}

int OutAtom::getValency() {
    return valency;
}

void OutAtom::setValency(int valency) {
    if (setExplicitly) {
        atom->addOutValency(valency - this->valency);
    }
    this->valency = valency;
}

bool OutAtom::isSetExplicitly() {
    return setExplicitly;
}

void OutAtom::setSetExplicitly(bool setExplicitly) {
    if (!this->setExplicitly && setExplicitly) {
        atom->addOutValency(valency);
    } else if (this->setExplicitly && !setExplicitly) {
        atom->addOutValency(-valency);
    }
    this->setExplicitly = setExplicitly;
}

std::wstring OutAtom::getLocant() {
    return locant;
}

void OutAtom::setLocant(const std::wstring &locant) {
    this->locant = locant;
}
