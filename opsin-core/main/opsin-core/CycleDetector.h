#pragma once

#include <vector>
#include <algorithm>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Fragment;
class Atom;
class Bond;


/// <summary>
/// Assigns whether atoms are in rings or not
/// @author dl387
///
/// </summary>
class CycleDetector {

    /// <summary>
    /// Performs a depth first search for rings hence assigning whether atoms are in rings or not
    /// This is necessary for deciding the applicability, and in some cases meaning, of suffixes and to determine what atoms are capable of having spare valency
    /// Fragments made of disconnected sections are supported </summary>
    /// <param name="frag"> </param>
public:
    static void assignWhetherAtomsAreInCycles(Fragment *frag);

private:
    static int traverseRings(Atom *currentAtom, Atom *previousAtom, int depth);

private:
    class PathSearchState {
    public:
        Atom *const currentAtom;
        const std::vector<Atom *> orderAtomsVisited;

        virtual ~PathSearchState() {
            delete currentAtom;
        }

        PathSearchState(Atom *currentAtom, std::vector<Atom *> &orderAtomsVisited);

        virtual Atom *getCurrentAtom();

        virtual std::vector<Atom *> getOrderAtomsVisited();
    };

    /// <summary>
    /// Attempts to find paths from a1 to a2 using only the given bonds </summary>
    /// <param name="a1"> </param>
    /// <param name="a2"> </param>
    /// <param name="peripheryBonds">
    /// @return </param>
public:
    static std::vector<std::vector<Atom *>>
    getPathBetweenAtomsUsingBonds(Atom *a1, Atom *a2, Set<Bond *> *peripheryBonds);
};

