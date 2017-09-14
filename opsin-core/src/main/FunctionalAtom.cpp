#include "FunctionalAtom.h"
#include "Atom.h"


FunctionalAtom::FunctionalAtom(Atom *atom) : atom(atom) {
}

Atom *FunctionalAtom::getAtom() {
    return atom;
}
