#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class OutAtom;
class FunctionalAtom;
class Fragment;
class Element;
class Atom;
class StructureBuildingException;


/// <summary>
/// A "struct" to hold the results of fragment building.
/// @author dl387
///
/// </summary>
class BuildResults {
    /// <summary>
    ///Holds the atoms that are currently marked as radicals. An atom may be listed twice for say diyl
    /// Typically these will be utilised by a word rule e.g. the ethyl of ethyl ethanoate has one
    /// Also holds the order of the bond that will be created when it is used (valency)
    /// setExplicitly says whether the outAtom absolutely definitely refers to that atom or not.
    /// e.g. propyl is stored as prop-1-yl with this set to false while prop-2-yl has it set to true
    /// These OutAtoms are the same objects as are present in the fragments
    /// </summary>
private:
    const std::vector<OutAtom *> outAtoms = std::vector<OutAtom *>();

    /// <summary>
    ///The atoms that may be used to from things like esters </summary>
    const std::vector<FunctionalAtom *> functionalAtoms = std::vector<FunctionalAtom *>();

    /// <summary>
    ///A list of fragments that have been evaluated to form this BuildResults. They are in the order they would be found in the XML </summary>
    Set<Fragment *> *const fragments = new LinkedHashSet<Fragment *>();

    /// <summary>
    ///A BuildResults is constructed from a list of Fragments.
    /// This constructor creates this list from the groups present in an XML word/bracket/sub element. </summary>
    /// <param name="wordSubOrBracket"> </param>
public:
    virtual ~BuildResults() {
        delete fragments;
    }

    BuildResults(Element *wordSubOrBracket);

    /// <summary>
    /// Construct a blank buildResults
    /// </summary>
    BuildResults();

    /// <summary>
    /// Returns a read only view of the fragments in this BuildResults
    /// @return
    /// </summary>
    virtual Set<Fragment *> *getFragments();

    virtual int getFragmentCount();

    virtual OutAtom *getOutAtom(int i);

    virtual int getOutAtomCount();

    virtual OutAtom *removeOutAtom(int i);

    virtual void removeAllOutAtoms();

    /// <summary>
    /// Returns the atom corresponding to position i in the functionalAtoms list </summary>
    /// <param name="i"> index </param>
    /// <returns> atom </returns>
    virtual Atom *getFunctionalAtom(int i);

    virtual FunctionalAtom *removeFunctionalAtom(int i);

    virtual int getFunctionalAtomCount();

    /// <summary>
    /// Returns the first OutAtom </summary>
    /// <returns> OutAtom </returns>
    virtual OutAtom *getFirstOutAtom();

    /// <summary>
    /// Returns the atom corresponding to the given id assuming the atom the id corresponds to is within the list of fragment in this Buildresults </summary>
    /// <param name="id"> index </param>
    /// <returns> atom </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Atom *getAtomByIdOrThrow(int id) throw(StructureBuildingException);

    virtual void mergeBuildResults(BuildResults *otherBR);

    /// <summary>
    /// Returns the sum of the charges of the fragments in the buildResults
    /// @return
    /// </summary>
    virtual int getCharge();
};

