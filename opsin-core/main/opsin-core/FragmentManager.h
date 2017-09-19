#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Bond;
class Fragment;
class SMILESFragmentBuilder;
class IDManager;
class StructureBuildingException;
class Element;
class Atom;
class BuildState;


using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


/// <summary>
/// Holds the Fragments during the construction of the molecule,
/// handles the building of new fragments and handles the creation/deletion of atoms/bonds
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class FragmentManager {

    /// <summary>
    /// A mapping between fragments and inter fragment bonds </summary>
private:
    const std::unordered_map<Fragment *, Set < Bond * >*>
    fragToInterFragmentBond = new LinkedHashMap < Fragment *, Set<Bond *>
    *>();

    /// <summary>
    /// All of the atom-containing fragments in the molecule </summary>
    Set<Fragment *> *const fragments = fragToInterFragmentBond.keySet();

    /// <summary>
    /// A builder for fragments specified as SMILES </summary>
    SMILESFragmentBuilder *const sBuilder;

    /// <summary>
    /// A source of unique integers </summary>
    IDManager *const idManager;

    /// <summary>
    /// Sets up a new Fragment manager, containing no fragments.
    /// </summary>
    /// <param name="sBuilder"> A SMILESFragmentBuilder - dependency injection. </param>
    /// <param name="idManager"> An IDManager. </param>
public:
    virtual ~FragmentManager() {
        delete fragments;
        delete sBuilder;
        delete idManager;
    }

    FragmentManager(SMILESFragmentBuilder *sBuilder, IDManager *idManager);

    /// <summary>
    /// Builds a fragment, based on an SMILES string
    /// The fragment will not correspond to a token
    /// </summary>
    /// <param name="smiles"> The fragment to build </param>
    /// <returns> The built fragment </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Fragment *buildSMILES(const std::wstring &smiles) throw(StructureBuildingException);

    /// <summary>
    /// Builds a fragment, based on an SMILES string
    /// The fragment will not correspond to a token
    /// </summary>
    /// <param name="smiles"> </param>
    /// <param name="type"> </param>
    /// <param name="labelMapping">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Fragment *buildSMILES(const std::wstring &smiles, const std::wstring &type,
                                  const std::wstring &labelMapping) throw(StructureBuildingException);

    /// <summary>
    /// Builds a fragment, based on an SMILES string
    /// The fragment will correspond to the given tokenEl
    /// </summary>
    /// <param name="smiles"> The fragment to build </param>
    /// <param name="tokenEl"> The corresponding tokenEl </param>
    /// <param name="labelMapping"> How to label the fragment </param>
    /// <returns> The built fragment </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Fragment *buildSMILES(const std::wstring &smiles, Element *tokenEl,
                                  const std::wstring &labelMapping) throw(StructureBuildingException);

    /// <summary>
    ///Creates a new fragment, containing all of the atoms and bonds
    /// of all of the other fragments - i.e. the whole molecule. This updates
    /// which fragments the atoms think they are in to the new super fragment
    /// but does not change the original fragments.
    /// Hence the original fragments remain associated with their atoms
    /// Atoms and Bonds are not copied.
    /// </summary>
    /// <returns> The unified fragment </returns>
    virtual Fragment *getUnifiedFragment();

    /// <summary>
    /// Incorporates a fragment, usually a suffix, into a parent fragment
    /// This does:
    /// Imports all of the atoms and bonds from another fragment into this one.
    /// Also imports outAtoms and functionalAtoms
    /// Reassigns inter-fragment bonds of the child fragment as either intra-fragment bonds
    /// of the parent fragment or as inter-fragment bonds of the parent fragment
    ///
    /// The original fragment still maintains its original atomList/bondList
    /// </summary>
    /// <param name="childFrag"> The fragment to be incorporated </param>
    /// <param name="parentFrag"> The parent fragment </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void incorporateFragment(Fragment *childFrag, Fragment *parentFrag) throw(StructureBuildingException);

    /// <summary>
    /// Incorporates a fragment, usually a suffix, into a parent fragment, creating a bond between them.
    /// </summary>
    /// <param name="childFrag"> The fragment to be incorporated </param>
    /// <param name="fromAtom"> An atom on that fragment </param>
    /// <param name="parentFrag"> The parent fragment </param>
    /// <param name="toAtom"> An atom on that fragment </param>
    /// <param name="bondOrder"> The order of the joining bond </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void incorporateFragment(Fragment *childFrag, Atom *fromAtom, Fragment *parentFrag, Atom *toAtom,
                                     int bondOrder) throw(StructureBuildingException);

    /// <summary>
    /// Converts an atom in a fragment to a different atomic symbol described by a SMILES string
    /// Charged atoms can also be specified eg. [NH4+]
    /// </summary>
    /// <param name="a"> The atom to change to a heteroatom </param>
    /// <param name="smiles"> The SMILES for one atom </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void replaceAtomWithSmiles(Atom *a, const std::wstring &smiles) throw(StructureBuildingException);

    /// <summary>
    /// Converts the smiles for a heteroatom to an atom </summary>
    /// <param name="smiles">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Atom *getHeteroatom(const std::wstring &smiles) throw(StructureBuildingException);

    /// <summary>
    /// Uses the information given in the given heteroatom to change the atomic symbol
    /// and charge of the given atom
    /// </summary>
    /// <param name="a"> The atom to change to a heteroatom </param>
    /// <param name="heteroAtom"> The atom to copy element/charge properties from </param>
    /// <param name="assignLocant"> Whether a locant should be assigned to the heteroatom if the locant is not used elsewhere </param>
    /// <exception cref="StructureBuildingException"> if a charge disagreement occurs </exception>
    virtual void replaceAtomWithAtom(Atom *a, Atom *heteroAtom, bool assignLocant) throw(StructureBuildingException);

    /// <summary>
    /// Gets an atom, given an id number
    /// Use this if you don't know what fragment the atom is in </summary>
    /// <param name="id"> The id of the atom </param>
    /// <returns> The atom, or null if no such atom exists. </returns>
    virtual Atom *getAtomByID(int id);

    /// <summary>
    /// Gets an atom, given an id number, throwing if fails.
    /// Use this if you don't know what fragment the atom is in </summary>
    /// <param name="id"> The id of the atom </param>
    /// <returns> The atom </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Atom *getAtomByIDOrThrow(int id) throw(StructureBuildingException);

    /// <summary>
    ///Turns all of the spare valencies in the fragments into double bonds.
    /// </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void convertSpareValenciesToDoubleBonds() throw(StructureBuildingException);

    /// <summary>
    /// Checks valencies are all chemically reasonable. An exception is thrown if any are not </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void checkValencies() throw(StructureBuildingException);

    virtual Set<Fragment *> *getFragments();

    /// <summary>
    /// Registers a fragment </summary>
    /// <param name="frag"> </param>
private:
    void addFragment(Fragment *frag);

    /// <summary>
    /// Removes a fragment
    /// Any inter-fragment bonds of this fragment are removed from the fragments it was connected to
    /// Throws an exception if fragment wasn't present </summary>
    /// <param name="frag"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    virtual void removeFragment(Fragment *frag) throw(StructureBuildingException);

    virtual int getOverallCharge();

    /// <summary>
    /// Creates a copy of a fragment by copying data
    /// labels the atoms using new ids from the idManager </summary>
    /// <param name="originalFragment"> </param>
    /// <returns> the clone of the fragment </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Fragment *copyFragment(Fragment *originalFragment) throw(StructureBuildingException);

    /// <summary>
    /// Creates a copy of a fragment by copying data
    /// labels the atoms using new ids from the idManager </summary>
    /// <param name="originalFragment"> </param>
    /// <param name="primesToAdd">: The minimum number of primes to add to the cloned atoms. More primes will be added if necessary to keep the locants unique e.g. N in the presence of N' becomes N'' when this is 1 </param>
    /// <returns> the clone of the fragment </returns>
    virtual Fragment *copyAndRelabelFragment(Fragment *originalFragment, int primesToAdd);

    /// <summary>
    /// Takes an element and produces a copy of it. Groups and suffixes are copied so that the new element
    /// has its own group and suffix fragments </summary>
    /// <param name="elementToBeCloned"> </param>
    /// <param name="state"> The current buildstate
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Element *cloneElement(BuildState *state, Element *elementToBeCloned) throw(StructureBuildingException);

    /// <summary>
    /// Takes an element and produces a copy of it. Groups and suffixes are copied so that the new element
    /// has its own group and suffix fragments </summary>
    /// <param name="elementToBeCloned"> </param>
    /// <param name="state"> The current buildstate </param>
    /// <param name="primesToAdd">: The minimum number of primes to add to the cloned atoms. More primes will be added if necessary to keep the locants unique e.g. N in the presence of N' becomes N'' when this is 1
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Element *
    cloneElement(BuildState *state, Element *elementToBeCloned, int primesToAdd) throw(StructureBuildingException);

    /// <summary>
    /// Takes an atom, removes it and bonds everything that was bonded to it to the replacementAtom with the original bond orders.
    /// Non element symbol locants are copied to the replacement atom </summary>
    /// <param name="atomToBeReplaced"> </param>
    /// <param name="replacementAtom"> </param>
    virtual void replaceAtomWithAnotherAtomPreservingConnectivity(Atom *atomToBeReplaced, Atom *replacementAtom);

    /// <summary>
    /// Removes a bond from the inter-fragment bond mappings if it was present </summary>
    /// <param name="bond"> </param>
private:
    void removeInterFragmentBondIfPresent(Bond *bond);

    /// <summary>
    /// Adds a bond to the fragment to inter-fragment bond mappings </summary>
    /// <param name="bond"> </param>
    void addInterFragmentBond(Bond *bond);

    /// <summary>
    /// Gets an unmodifiable view of the set of the inter-fragment bonds a fragment is involved in </summary>
    /// <param name="frag"> </param>
    /// <returns> set of inter fragment bonds </returns>
public:
    virtual Set<Bond *> *getInterFragmentBonds(Fragment *frag);

    /// <summary>
    /// Create a new Atom of the given element belonging to the given fragment </summary>
    /// <param name="chemEl"> </param>
    /// <param name="frag"> </param>
    /// <returns> Atom </returns>
    virtual Atom *createAtom(ChemEl chemEl, Fragment *frag);

    /// <summary>
    /// Create a new bond between two atoms.
    /// The bond is associated with these atoms.
    /// It is also listed as an inter-fragment bond or associated with a fragment </summary>
    /// <param name="fromAtom"> </param>
    /// <param name="toAtom"> </param>
    /// <param name="bondOrder"> </param>
    /// <returns> Bond </returns>
    virtual Bond *createBond(Atom *fromAtom, Atom *toAtom, int bondOrder);

    virtual void removeAtomAndAssociatedBonds(Atom *atom);

    virtual void removeBond(Bond *bond);

    /// <summary>
    /// Valency is used to determine the expected number of hydrogen
    /// Hydrogens are then added to bring the number of connections up to the minimum required to satisfy the atom's valency
    /// This allows the valency of the atom to be encoded e.g. phopshane-3 hydrogen, phosphorane-5 hydrogen.
    /// It is also necessary when considering stereochemistry as a hydrogen beats nothing in the CIP rules </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void makeHydrogensExplicit() throw(StructureBuildingException);

private:
    void modifyAtomParityToTakeIntoAccountExplicitHydrogen(Atom *atom) throw(StructureBuildingException);
};
