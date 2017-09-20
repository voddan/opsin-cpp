#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <boost/optional.hpp>
#include <set>
#include <utils/Comparator.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;
class Fragment;
class StructureBuildingException;
class Bond;
class OutAtom;
class BuildState;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

/// <summary>
/// Sorts a list of atoms such that their order agrees with the order symbolic locants are typically assigned
///
/// Preferred atoms are sorted to the START of the list
/// @author dl387
///
/// </summary>
class SortAtomsForElementSymbols : public Comparator<Atom> {

public:
    virtual int compare(Atom *a, Atom *b) override;
};

/// <summary>
/// Performs a very crude sort of atoms such that those that are more likely to be substitued are preferred for low locants
/// Preferred atoms are sorted to the START of the list
/// @author dl387
///
/// </summary>
class SortAtomsForMainGroupElementSymbols : public Comparator<Atom> {

public:
    virtual int compare(Atom *a, Atom *b) override;
};

class FragmentTools {
    /// <summary>
    /// Sorts by number, then by letter e.g. 4,3,3b,5,3a,2 -->2,3,3a,3b,4,5
    /// @author dl387
    ///
    /// </summary>
public:
    class SortByLocants : public Comparator<Atom> {
    public:
        static Pattern *const locantSegmenter = Pattern::compile(L"(\\d+)([a-z]?)('*)");

        virtual int compare(Atom *atoma, Atom *atomb) override;
    };

    /// <summary>
    /// Assign element locants to groups/suffixes. These are in addition to any numerical locants that are present.
    /// Adds primes to make each locant unique.
    /// For groups a locant is not given to carbon atoms
    /// If an element appears in a suffix then element locants are not assigned to occurrences of that element in the parent group
    /// HeteroAtoms in acidStems connected to the first Atom of the fragment are treated as if they were suffix atoms </summary>
    /// <param name="suffixableFragment"> </param>
    /// <param name="suffixFragments"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
public:
    static void assignElementLocants(Fragment *suffixableFragment,
                                     std::vector<Fragment *> &suffixFragments) throw(StructureBuildingException);

private:
    static void detectAndCorrectHydrazoneDerivativeViolation(std::vector<Fragment *> &suffixFragments);

    static void
    processMainGroupLabelling(Fragment *suffixableFragment, std::unordered_map<std::wstring, int> &elementCount,
                              std::set<Atom *> *atomsToIgnore);

    static void processSuffixLabelling(std::vector<Fragment *> &suffixFragments,
                                       std::unordered_map<std::wstring, int> &elementCount, std::set<Atom *> *atomsToIgnore);

    static void
    processNonCarboxylicAcidLabelling(Fragment *suffixableFragment, std::unordered_map<std::wstring, int> &elementCount,
                                      std::set<Atom *> *atomsToIgnore);

    static void
    assignLocantsAndExploreNeighbours(std::unordered_map<std::wstring, int> &elementCount, std::set<Atom *> *atomsToIgnore,
                                      std::set<Atom *> *atomsVisited, Deque<Atom *> *atomsToConsider);

    /// <summary>
    /// Gets the neighbours of an atom that claim to be within the same frag
    /// The order of bond taken to get to the neighbour is set on the neighbours Atom.VISITED property </summary>
    /// <param name="atom">
    /// @return </param>
    static std::vector<Atom *> getIntraFragmentNeighboursAndSetVisitedBondOrder(Atom *atom);

    static void assignLocant(Atom *atom, std::unordered_map<std::wstring, int> &elementCount);

    /// <summary>
    /// Adjusts the order of a bond in a fragment.
    /// </summary>
    /// <param name="fromAtom"> The lower-numbered atom in the bond </param>
    /// <param name="bondOrder"> The new bond order </param>
    /// <param name="fragment"> The fragment </param>
    /// <returns> The bond that was unsaturated </returns>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static Bond *unsaturate(Atom *fromAtom, int bondOrder, Fragment *fragment) throw(StructureBuildingException);

    /// <summary>
    /// Adjusts the order of a bond in a fragment.
    /// </summary>
    /// <param name="fromAtom"> The first atom in the bond </param>
    /// <param name="locantTo"> The locant of the other atom in the bond </param>
    /// <param name="bondOrder"> The new bond order </param>
    /// <param name="fragment"> The fragment </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void unsaturate(Atom *fromAtom, const std::wstring &locantTo, int bondOrder,
                           Fragment *fragment) throw(StructureBuildingException);

    /// <summary>
    ///Adjusts the labeling on a fused ring system, such that bridgehead atoms
    /// have locants endings in 'a' or 'b' etc. Example: naphthalene
    /// 1,2,3,4,5,6,7,8,9,10->1,2,3,4,4a,5,6,7,8,8a </summary>
    /// <param name="atomList"> </param>
    static void relabelLocantsAsFusedRingSystem(std::vector<Atom *> &atomList);

    /// <summary>
    /// Adds the given string to all the locants of the atoms. </summary>
    /// <param name="atomList"> </param>
    /// <param name="stringToAdd"> </param>
    static void relabelLocants(std::vector<Atom *> &atomList, const std::wstring &stringToAdd);

    /// <summary>
    /// Adds the given string to all the numeric locants of the atoms. </summary>
    /// <param name="atomList"> </param>
    /// <param name="stringToAdd"> </param>
    static void relabelNumericLocants(std::vector<Atom *> &atomList, const std::wstring &stringToAdd);


    static void splitOutAtomIntoValency1OutAtoms(OutAtom *outAtom);

    /// <summary>
    /// Checks if the specified Nitrogen is potentially involved in [NH]C=N <-> N=C[NH] tautomerism
    /// Given the starting nitrogen returns the other nitrogen or null if that nitrogen does not appear to be involved in such tautomerism </summary>
    /// <param name="nitrogen"> </param>
    /// <returns> null or the other nitrogen </returns>
    static Atom *detectSimpleNitrogenTautomer(Atom *nitrogen);

    /// <summary>
    ///Increases the order of bonds joining atoms with spareValencies,
    /// and uses up said spareValencies.
    /// [spare valency is an indication of the atom's desire to form the maximum number of non-cumulative double bonds] </summary>
    /// <param name="frag"> </param>
    /// <exception cref="StructureBuildingException"> If the algorithm can't work out where to put the bonds </exception>
    static void convertSpareValenciesToDoubleBonds(Fragment *frag) throw(StructureBuildingException);

private:
    static Atom *findBestAtomToRemoveSpareValencyFrom(Fragment *frag, std::vector<Atom *> &atomCollection);


public:
    static Atom *
    getAtomByAminoAcidStyleLocant(Atom *backboneAtom, const std::wstring &elementSymbol, const std::wstring &primes);


    /// <summary>
    /// Determines whether the bond between two elements is likely to be covalent
    /// This is crudely determined based on whether the combination of elements fall outside the ionic and
    /// metallic sections of a van Arkel diagram </summary>
    /// <param name="chemEl1"> </param>
    /// <param name="chemEl2">
    /// @return </param>
    static bool isCovalent(ChemEl chemEl1, ChemEl chemEl2);

    /// <summary>
    /// Is the atom a suffix atom/carbon of an aldehyde atom/chalcogen functional atom/hydroxy (or chalcogen equivalent)
    /// (by special step heterostems are not considered hydroxy e.g. disulfane) </summary>
    /// <param name="atom">
    /// @return </param>
    static bool isCharacteristicAtom(Atom *atom);

    /// <summary>
    /// Is the atom an aldehyde atom or a chalcogen functional atom </summary>
    /// <param name="atom">
    /// @return </param>
    static bool isFunctionalAtomOrAldehyde(Atom *atom);

    /// <summary>
    /// Is the atom a chalcogen functional atom </summary>
    /// <param name="atom">
    /// @return </param>
    static bool isFunctionalAtom(Atom *atom);


    /// <summary>
    /// Checks that all atoms in a ring appear to be equivalent </summary>
    /// <param name="ring"> </param>
    /// <returns> true if all equivalent, else false </returns>
    static bool allAtomsInRingAreIdentical(Fragment *ring);

    static void removeTerminalAtom(BuildState *state, Atom *atomToRemove);

    /// <summary>
    /// Removes a terminal oxygen from the atom
    /// An exception is thrown if no suitable oxygen could be found connected to the atom
    /// Note that [N+][O-] is treated as N=O </summary>
    /// <param name="state"> </param>
    /// <param name="atom"> </param>
    /// <param name="desiredBondOrder"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void
    removeTerminalOxygen(BuildState *state, Atom *atom, int desiredBondOrder) throw(StructureBuildingException);


    /// <summary>
    /// Finds terminal atoms of the given element type from the list given
    /// The terminal atoms be single bonded, not radicals and uncharged </summary>
    /// <param name="atoms"> </param>
    /// <param name="chemEl"> </param>
    /// <returns>  </returns>
    static std::vector<Atom *> findHydroxyLikeTerminalAtoms(std::vector<Atom *> &atoms, ChemEl chemEl);

    /// <summary>
    /// Checks whether a bond is part of a 6 member or smaller ring.
    /// This is necessary as such double bonds are assumed to not be capable of having E/Z stereochemistry </summary>
    /// <param name="bond"> </param>
    /// <returns> true unless in a 6 member or smaller rings </returns>
    static bool notIn6MemberOrSmallerRing(Bond *bond);

    /// <summary>
    /// Finds the hydroxy atom of all hydroxy functional groups in a fragment
    /// i.e. not in carboxylic acid or oxime </summary>
    /// <param name="frag">
    /// @return </param>
    /// <exception cref="StructureBuildingException">  </exception>
    static std::vector<Atom *> findHydroxyGroups(Fragment *frag) throw(StructureBuildingException);

    static std::vector<Atom *>
    findnAtomsForSubstitution(std::vector<Atom *> &atomList, Atom *preferredAtom, int numberOfSubstitutionsRequired,
                              int bondOrder, bool takeIntoAccountOutValency);

    static std::vector<Atom *>
    findnAtomsForSubstitution(Fragment *frag, Atom *preferredAtom, int numberOfSubstitutionsRequired, int bondOrder,
                              bool takeIntoAccountOutValency);

    /// <summary>
    /// Returns a list of atoms of size >= numberOfSubstitutionsDesired (or null if this not possible)
    /// An atom must have have sufficient valency to support a substituent requiring a bond of order bondOrder
    /// If an atom can support multiple substituents it will appear in the list multiple times
    /// This method iterates over the the fragment atoms attempting to fulfil these requirements with incrementally more lenient constraints:
    /// aromaticity preserved, standard valency assumed, characteristic atoms ignored
    /// aromaticity preserved, standard valency assumed, functional suffixes ignored
    /// aromaticity preserved, any sensible valency allowed, anything substitutable
    /// aromaticity dropped, any sensible valency allowed, anything substitutable
    ///
    /// Iteration starts from the defaultInAtom (if applicable, else the first atom) i.e. the defaultInAtom if substitutable will be the first atom in the list </summary>
    /// <param name="frag"> </param>
    /// <param name="numberOfSubstitutionsRequired"> </param>
    /// <param name="bondOrder">
    /// @return </param>
    static std::vector<Atom *>
    findnAtomsForSubstitution(Fragment *frag, int numberOfSubstitutionsRequired, int bondOrder);

    /// <summary>
    /// Returns a list of the most preferable atoms for substitution (empty list if none are)
    /// An atom must have have sufficient valency to support a substituent requiring a bond of order bondOrder
    /// If an atom can support multiple substituents it will appear in the list multiple times
    /// This method iterates over the the fragment atoms attempting to fulfil these requirements with incrementally more lenient constraints:
    /// aromaticity preserved, standard valency assumed, characteristic atoms ignored
    /// aromaticity preserved, standard valency assumed, functional suffixes ignored
    /// aromaticity preserved, any sensible valency allowed, anything substitutable
    /// aromaticity dropped, any sensible valency allowed, anything substitutable
    ///
    /// Iteration starts from the defaultInAtom (if applicable, else the first atom) i.e. the defaultInAtom if substitutable will be the first atom in the list </summary>
    /// <param name="frag"> </param>
    /// <param name="bondOrder">
    /// @return </param>
    static std::vector<Atom *> findSubstituableAtoms(Fragment *frag, int bondOrder);

    static Atom *lastNonSuffixCarbonWithSufficientValency(Fragment *conjunctiveFragment);


};

