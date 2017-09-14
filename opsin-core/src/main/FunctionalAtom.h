#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;


/// <summary>
/// Struct for a FunctionalAtom. As expected holds the atom.
/// This is used to indicate, for example, that this atom may form an ester
///
/// @author dl387
///
/// </summary>
class FunctionalAtom {
private:
    Atom *const atom;

public:
    virtual ~FunctionalAtom() {
        delete atom;
    }

    FunctionalAtom(Atom *atom);

    virtual Atom *getAtom();
};

