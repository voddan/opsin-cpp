#pragma once

#include "ChemEl.h"
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;


/// <summary>
/// Hold information about 4 atoms and their chiral determinant allowing the description of tetrahedral stereochemistry
/// @author dl387
///
/// </summary>
class AtomParity {
    /// <summary>
    /// A dummy hydrogen atom. Used to represent an implicit hydrogen that is attached to a tetrahedral stereocentre
    /// </summary>
public:
    static Atom *const hydrogen = new Atom(ChemEl::H);
    /// <summary>
    /// A dummy hydrogen atom. Used to represent the hydrogen that replaced a hydroxy at a tetrahedral stereocentre
    /// </summary>
    static Atom *const deoxyHydrogen = new Atom(ChemEl::H);
private:
    std::vector<Atom *> atomRefs4;
    int parity = 0;

    /// <summary>
    /// Create an atomParity from an array of 4 atoms and the parity of the chiral determinant </summary>
    /// <param name="atomRefs4"> </param>
    /// <param name="parity"> </param>
public:
    AtomParity(std::vector<Atom *> &atomRefs4, int parity);

    virtual std::vector<Atom *> getAtomRefs4();

    virtual void setAtomRefs4(std::vector<Atom *> &atomRefs4);

    virtual int getParity();

    virtual void setParity(int parity);
};

