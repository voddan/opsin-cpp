#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <boost/optional.hpp>
#include <set>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;
class Bond;
class Element;
class OutAtom;
class FunctionalAtom;
class StructureBuildingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

/// <summary>
///A fragment of a molecule, holds bonds and atoms.
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class Fragment {

    /// <summary>
    ///A mapping between IDs and the atoms in this fragment, by default is ordered by the order atoms are added to the fragment </summary>
private:
    const std::unordered_map<int, Atom *> atomMapFromId{};  // LinkedHasMap

    /// <summary>
    ///Equivalent to and synced to atomMapFromId.values() </summary>
    std::vector<Atom *> *const atomCollection{};

    /// <summary>
    ///A mapping between locants and the atoms in this fragment </summary>
    const std::unordered_map<std::wstring, Atom *> atomMapFromLocant = std::unordered_map<std::wstring, Atom *>();

    /// <summary>
    ///The bonds in the fragment </summary>
    std::set<Bond *> *const bondSet{};  // LinkedHashSet

    /// <summary>
    ///The associated token element </summary>
    Element *tokenEl;

    /// <summary>
    ///The atoms that are used when this fragment is connected to another fragment. Unused outAtoms means that the fragment is a radical or an error has occurred
    /// Initially empty
    /// </summary>
    const std::vector<OutAtom *> outAtoms = std::vector<OutAtom *>();

    /// <summary>
    ///The atoms that are used on this fragment to form things like esters
    /// Initially empty
    /// </summary>
    const std::vector<FunctionalAtom *> functionalAtoms = std::vector<FunctionalAtom *>();

    /// <summary>
    ///The atom that fragments connecting to this fragment should connect to in preference
    /// e.g. for amino acids the alpha amino group
    /// Null by default
    /// </summary>
    Atom *defaultInAtom = nullptr;

    /// <summary>
    ///The atoms in the fragment that have been indicated to have hydrogen at the SMILES level. </summary>
    const std::vector<Atom *> indicatedHydrogen = std::vector<Atom *>();

    /// <summary>
    ///Pseudo atoms indicating start and end of polymer structure repeat unit </summary>
    std::vector<Atom *> polymerAttachmentPoints;

    /// <summary>
    /// DO NOT CALL DIRECTLY EXCEPT FOR TESTING
    /// Makes an empty Fragment associated with the given tokenEl </summary>
    /// <param name="tokenEl"> </param>
public:
    virtual ~Fragment() {
        delete atomCollection;
        delete bondSet;
        delete tokenEl;
        delete defaultInAtom;
    }

    Fragment(Element *tokenEl);

    /// <summary>
    /// DO NOT CALL DIRECTLY EXCEPT FOR TESTING
    /// Makes an empty Fragment with the given type
    /// </summary>
    /// <param name="type"> </param>
    Fragment(const std::wstring &type);

    /// <summary>
    ///Adds an atom to the fragment and associates it with this fragment </summary>
    virtual void addAtom(Atom *atom);

    /// <summary>
    /// Return the number of atoms in the fragment
    /// @return
    /// </summary>
    virtual int getAtomCount();

    /// <summary>
    /// Returns a copy of the fragment's atoms
    /// @return
    /// </summary>
    virtual std::vector<Atom *> getAtomList();

    /// <summary>
    /// Adds a bond to the fragment. </summary>
    /// <param name="bond"> </param>
    virtual void addBond(Bond *bond);

    /// <summary>
    ///Removes a bond to the fragment if it is present. </summary>
    /// <param name="bond">
    /// @return </param>
    virtual bool removeBond(Bond *bond);

    /// <summary>
    ///Gets bondSet. </summary>
    virtual std::set<Bond *> *getBondSet();

    /// <summary>
    ///Gets the id of the atom in the fragment with the specified locant.
    /// </summary>
    /// <param name="locant"> The locant to look for </param>
    /// <returns> The id of the found atom, or 0 if it is not found </returns>
    virtual int getIDFromLocant(const std::wstring &locant);

    /// <summary>
    ///Gets the id of the atom in the fragment with the specified locant, throwing if this fails.
    /// </summary>
    /// <param name="locant"> The locant to look for </param>
    /// <returns> The id of the found atom </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual int getIDFromLocantOrThrow(const std::wstring &locant) throw(StructureBuildingException);

    /// <summary>
    ///Gets the atom in the fragment with the specified locant.
    /// </summary>
    /// <param name="locant"> The locant to look for </param>
    /// <returns> The found atom, or null if it is not found </returns>
    virtual Atom *getAtomByLocant(const std::wstring &locant);

    /// <summary>
    ///Gets the atom in the fragment with the specified locant, throwing if this fails.
    /// </summary>
    /// <param name="locant"> The locant to look for </param>
    /// <returns> The found atom </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Atom *getAtomByLocantOrThrow(const std::wstring &locant) throw(StructureBuildingException);

    /// <summary>
    ///Gets the atom in the fragment with the specified ID.
    /// </summary>
    /// <param name="id"> The id of the atom. </param>
    /// <returns> The found atom, or null. </returns>
    virtual Atom *getAtomByID(int id);

    /// <summary>
    ///Gets the atom in the fragment with the specified ID, throwing if this fails.
    /// </summary>
    /// <param name="id"> The id of the atom. </param>
    /// <returns> The found atom </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Atom *getAtomByIDOrThrow(int id) throw(StructureBuildingException);

    /// <summary>
    ///Finds a bond between two specified atoms the first of which must be within the fragment
    /// </summary>
    /// <param name="ID1"> The id of one atom </param>
    /// <param name="ID2"> The id of the other atom </param>
    /// <returns> The bond found, or null </returns>
    virtual Bond *findBond(int ID1, int ID2);

    /// <summary>
    ///Finds a bond between two specified atoms the first of which must be within the fragment, throwing if it fails.
    /// </summary>
    /// <param name="ID1"> The id of one atom </param>
    /// <param name="ID2"> The id of the other atom </param>
    /// <returns> The bond found </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Bond *findBondOrThrow(int ID1, int ID2) throw(StructureBuildingException);

    /// <summary>
    ///Works out how many atoms there are in the fragment there are
    /// with consecutive locants, starting from 1 that are in a chain
    /// </summary>
    /// <returns> The number of atoms in the locant chain </returns>
    virtual int getChainLength();

    /// <summary>
    /// Gets the type of the corresponding tokenEl
    /// Returns "" if undefined
    /// @return
    /// </summary>
    virtual std::wstring getType();

    /// <summary>
    /// Gets the subType of the corresponding tokenEl
    /// Returns "" if undefined
    /// @return
    /// </summary>
    virtual std::wstring getSubType();

    /// <summary>
    /// Gets the associate tokenEl
    /// Whether or not this is a real token can be tested by whether it has a parent
    /// @return
    /// </summary>
    virtual Element *getTokenEl();

    /// <summary>
    /// Sets the associated tokenEl
    /// Type/subType are inherited from the tokenEl </summary>
    /// <param name="tokenEl"> </param>
    virtual void setTokenEl(Element *tokenEl);

    /// <summary>
    /// How many OutAtoms (i.e. radicals) are associated with this fragment
    /// @return
    /// </summary>
    virtual int getOutAtomCount();

    /// <summary>
    /// Gets the outAtom at a specific index of the outAtoms linkedList </summary>
    /// <param name="i">
    /// @return </param>
    virtual OutAtom *getOutAtom(int i);

    /// <summary>
    /// Adds an outAtom </summary>
    /// <param name="id"> </param>
    /// <param name="valency"> </param>
    /// <param name="setExplicitly"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    virtual void addOutAtom(int id, int valency, boost::optional<bool> setExplicitly) throw(StructureBuildingException);

    /// <summary>
    /// Adds an outAtom </summary>
    /// <param name="atom"> </param>
    /// <param name="valency"> </param>
    /// <param name="setExplicitly"> </param>
    virtual void addOutAtom(Atom *atom, int valency, boost::optional<bool> setExplicitly);

    /// <summary>
    /// Includes the OutAtoms of a given fragment into this fragment
    /// Note that no OutAtoms are created in doing this </summary>
    /// <param name="frag"> </param>
    virtual void incorporateOutAtoms(Fragment *frag);

    /// <summary>
    /// Removes the outAtom at a specific index of the outAtom linkedList </summary>
    /// <param name="i"> </param>
    virtual void removeOutAtom(int i);

    /// <summary>
    /// Removes the specified outAtom from the outAtoms linkedList </summary>
    /// <param name="outAtom"> </param>
    virtual void removeOutAtom(OutAtom *outAtom);

    /// <summary>
    /// How many functionalAtoms (i.e. locations that can form esters) are associated with this fragment
    /// @return
    /// </summary>
    virtual int getFunctionalAtomCount();

    /// <summary>
    /// Gets the functionalAtom at a specific index of the functionalAtoms linkedList </summary>
    /// <param name="i">
    /// @return </param>
    virtual FunctionalAtom *getFunctionalAtom(int i);

    /// <summary>
    ///Adds a functionalAtom </summary>
    /// <param name="atom"> </param>
    virtual void addFunctionalAtom(Atom *atom);

    /// <summary>
    /// Includes the FunctionalAtoms of a given fragment into this fragment
    /// Note that no FunctionalAtoms are created in doing this </summary>
    /// <param name="frag"> </param>
    virtual void incorporateFunctionalAtoms(Fragment *frag);

    /// <summary>
    /// Removes the functionalAtom at a specific index of the functionalAtoms linkedList </summary>
    /// <param name="i"> </param>
    /// <returns>  </returns>
    virtual FunctionalAtom *removeFunctionalAtom(int i);

    /// <summary>
    /// Removes the specified functionalAtom from the functionalAtoms linkedList </summary>
    /// <param name="functionalAtom"> </param>
    virtual void removeFunctionalAtom(FunctionalAtom *functionalAtom);

    virtual std::vector<Atom *> getPolymerAttachmentPoints();

    virtual void setPolymerAttachmentPoints(std::vector<Atom *> &polymerAttachmentPoints);

    /// <summary>
    ///Gets a list of atoms in the fragment that connect to a specified atom
    /// </summary>
    /// <param name="atom"> The reference atom </param>
    /// <returns> The list of atoms connected to the atom </returns>
    virtual std::vector<Atom *> getIntraFragmentAtomNeighbours(Atom *atom);

    /// <summary>
    ///Calculates the number of bonds connecting to the atom, excluding bonds to implicit
    /// hydrogens. Double bonds count as
    /// two bonds, etc. Eg ethene - both C's have an incoming valency of 2.
    ///
    /// Only bonds to atoms within the fragment are counted. Suffix atoms are excluded
    /// </summary>
    /// <param name="atom"> </param>
    /// <returns> Incoming Valency </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual int getIntraFragmentIncomingValency(Atom *atom) throw(StructureBuildingException);

    /// <summary>
    /// Checks valencies are sensible </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void checkValencies() throw(StructureBuildingException);

    /// <summary>
    /// Removes an atom from this fragment </summary>
    /// <param name="atom"> </param>
    virtual void removeAtom(Atom *atom);

    /// <summary>
    /// Retrieves the overall charge of the fragment by querying all its atoms
    /// @return
    /// </summary>
    virtual int getCharge();

    virtual Atom *getDefaultInAtom();

    virtual void setDefaultInAtom(Atom *inAtom);

    virtual Atom *getDefaultInAtomOrFirstAtom();

    /// <summary>
    /// Adds a mapping between the locant and atom object </summary>
    /// <param name="locant"> A locant as a string </param>
    /// <param name="a"> An atom </param>
    virtual void addMappingToAtomLocantMap(const std::wstring &locant, Atom *a);

    /// <summary>
    /// Removes a mapping between a locant </summary>
    /// <param name="locant"> A locant as a string </param>
    virtual void removeMappingFromAtomLocantMap(const std::wstring &locant);

    /// <summary>
    /// Checks to see whether a locant is present on this fragment </summary>
    /// <param name="locant">
    /// @return </param>
    virtual bool hasLocant(const std::wstring &locant);


    /// <summary>
    /// Returns an unmodifiable list of the locants associated with this fragment
    /// @return
    /// </summary>
    virtual std::set <std::wstring> *getLocants();

    virtual std::vector<Atom *> getIndicatedHydrogen();

    virtual void addIndicatedHydrogen(Atom *atom);

    /// <summary>
    /// Returns the id of the first atom in the fragment
    /// @return </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual int getIdOfFirstAtom();

    /// <summary>
    /// Returns the the first atom in the fragment or null if it has no atoms
    /// Typically the first atom will be the first atom that was added to the fragment </summary>
    /// <returns> firstAtom </returns>
    virtual Atom *getFirstAtom();

    /// <summary>
    /// Clears and recreates atomMapFromId (and hence AtomCollection) using the order of the atoms in atomList </summary>
    /// <param name="atomList"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void reorderAtomCollection(std::vector<Atom *> &atomList) throw(StructureBuildingException);

    /// <summary>
    /// Reorders the fragment's internal atomList by the value of the first locant of the atoms
    /// e.g. 1,2,3,3a,3b,4
    /// Used for assuring the correct order of atom iteration when performing ring fusion </summary>
    /// <exception cref="StructureBuildingException">  </exception>
    virtual void sortAtomListByLocant() throw(StructureBuildingException);
};




