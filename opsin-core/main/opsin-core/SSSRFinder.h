#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <set>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Fragment;
class Ring;
class Atom;
class Bond;


/// <summary>
/// Class for finding SSSR
/// The algorithm employed does not work in some corner cases
///
/// @author pm286
/// @author dl387
///
/// </summary>
class SSSRFinder {

    /// <summary>
    /// get set of smallest rings.
    /// In corner cases the list of rings returned will not be the SSSR </summary>
    /// <param name="frag"> </param>
    /// <returns> list of rings </returns>
public:
    static std::vector<Ring *> getSetOfSmallestRings(Fragment *frag);

    /// <summary>
    /// get list of rings.
    /// not necessarily SSSR </summary>
    /// <param name="atomSet"> </param>
    /// <returns> list of rings </returns>
private:
    static std::vector<Ring *> getRings(std::vector<Atom *> &atomSet);

    static Ring *getRing(Bond *bond, std::unordered_map<Atom *, Atom *> &atomToParentMap);

    static std::vector<Bond *> getAncestors1(Atom *atom, std::unordered_map<Atom *, Atom *> &atomToParentMap);

    static void
    expand(Atom *atom, Atom *parentAtom, std::set<Atom *> *usedAtoms, std::unordered_map<Atom *, Atom *> &atomToParentMap,
           std::set<Bond *> *linkBondSet);

    static bool reduceRingSizes(Ring *ring, std::vector<Ring *> &newList);


    static std::vector<Bond *> symmetricDifference(std::vector<Bond *> &bondSet1, std::vector<Bond *> &bondSet2);

};

