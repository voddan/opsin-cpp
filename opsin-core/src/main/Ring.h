#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;
class Bond;


/// <summary>
/// Class representing a single ring (i.e. NOT a fused ring which is formed from multiple rings)
/// @author dl387
///
/// </summary>
class Ring {
private:
    const std::vector<Atom *> atomList = std::vector<Atom *>();
    const std::vector<Bond *> bondList;
    const std::unordered_map<Bond *, Ring *> bondToNeighbourRings = new LinkedHashMap<Bond *, Ring *>();

    std::vector<Atom *> cyclicAtomList;
    std::vector<Bond *> cyclicBondList;

public:
    Ring(std::vector<Bond *> &bondList);


    virtual std::vector<Bond *> getBondList();

    virtual std::vector<Atom *> getAtomList();

    /// <summary>
    /// Number of ring atoms/bonds
    /// @return
    /// </summary>
    virtual int size();

    virtual int getNumberOfFusedBonds();

    /// <summary>
    /// Return bonds utilised in multiple rings </summary>
    /// <returns> List<Bond> </returns>
    virtual std::vector<Bond *> getFusedBonds();

    virtual int getBondIndex(Bond *bond);

    virtual std::vector<Bond *> getCyclicBondList();

    virtual std::vector<Atom *> getCyclicAtomList();

    virtual std::vector<Ring *> getNeighbours();

    virtual Ring *getNeighbourOfFusedBond(Bond *fusedBond);

    virtual void addNeighbour(Bond *bond, Ring *ring);

    /// <summary>
    /// Stores atoms and bonds in the order defined by atom and bond </summary>
    /// <param name="stBond"> - the first bond </param>
    /// <param name="stAtom"> - the atom defining in which direction to go </param>
    virtual void makeCyclicLists(Bond *stBond, Atom *stAtom);

    virtual std::wstring toString() override;

};

