#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class AtomParity;

class Bond;

template<typename T>
class PropertyKey;

class Fragment;

class StructureBuildingException;


using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

/// <summary>
/// An atom. Carries information about which fragment it is in, and an ID
/// number and a list of bonds that it is involved. It may also have other information such as
/// whether it has "spare valencies" due to unsaturation, its charge, locant labels, stereochemistry and notes
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class Atom {

    /// <summary>
    ///The (unique over the molecule) ID of the atom. </summary>
private:
    const int id;

    /// <summary>
    ///The chemical element of the atom. </summary>
    ChemEl chemEl = static_cast<ChemEl>(0);

    /// <summary>
    ///The locants that pertain to the atom. </summary>
    const std::vector<std::wstring> locants = std::vector<std::wstring>(2);

    /// <summary>
    ///The formal charge on the atom. </summary>
    int charge = 0;

    /// <summary>
    ///The isotope of the atom. Null if not defined explicitly. </summary>
    boost::optional<int> isotope = boost::none;

    /// <summary>
    /// Holds the atomParity object associated with this object
    /// null by default
    /// </summary>
    AtomParity *atomParity = nullptr;

    /// <summary>
    ///The bonds that involve the atom </summary>
    const std::vector<Bond *> bonds = std::vector<Bond *>(4);

    /// <summary>
    ///A map between PropertyKey s as declared here and useful atom properties, usually relating to some kind of special case. </summary>
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("rawtypes") private final java.util.Map<PropertyKey, Object> properties = new java.util.HashMap<PropertyKey, Object>();
    const std::unordered_map<PropertyKey *, void *> properties = std::unordered_map<PropertyKey *, void *>();
    /// <summary>
    /// A set of atoms that were equally plausible to perform functional replacement on </summary>
public:
    static PropertyKey<Set < Atom * >*> *
    const AMBIGUOUS_ELEMENT_ASSIGNMENT;
    /// <summary>
    /// The atom class which will be output when serialised to SMILES. Useful for distinguishing attachment points </summary>
    static PropertyKey<int> *const ATOM_CLASS;
    /// <summary>
    /// Used on wildcard atoms to indicate their meaning </summary>
    static PropertyKey<std::wstring> *const HOMOLOGY_GROUP;
    /// <summary>
    /// Used on wildcard atoms to indicate that they are a position variation bond </summary>
    static PropertyKey<std::vector<Atom *>> *const POSITION_VARIATION_BOND;
    /// <summary>
    /// The hydrogen count as set in the SMILES </summary>
    static PropertyKey<int> *const SMILES_HYDROGEN_COUNT;
    /// <summary>
    /// The oxidation number as specified by Roman numerals in the name </summary>
    static PropertyKey<int> *const OXIDATION_NUMBER;
    /// <summary>
    /// Is this atom the carbon of an aldehyde? (however NOT formaldehyde) </summary>
    static PropertyKey<bool> *const ISALDEHYDE;
    /// <summary>
    /// Indicates that this atom is an anomeric atom in a cyclised carbohydrate </summary>
    static PropertyKey<bool> *const ISANOMERIC;
    /// <summary>
    /// Transient integer used to indicate traversal of fragments </summary>
    static PropertyKey<int> *const VISITED;

    /// <summary>
    ///The fragment to which the atom belongs. </summary>
private:
    Fragment *frag;

    /// <summary>
    /// Whether an atom is part of a delocalised set of double bonds. A double bond in a kekule structure
    /// can be mapped to a single bond with this attribute set to true on both atoms that were in the double bond
    /// For example, benzene could be temporarily represented by six singly-bonded atoms, each with a set
    /// spare valency attribute , and later converted into a fully-specified valence structure.
    /// </summary>
    bool spareValency = false;

    /// <summary>
    ///The total bond order of all bonds that are expected to be used for inter fragment bonding
    /// e.g. in butan-2-ylidene this would be 2 for the atom at position 2 and 0 for the other 3
    /// </summary>
    int outValency = 0;

    /// <summary>
    /// Null by default or set by the lambda convention. </summary>
    boost::optional<int> lambdaConventionValency;

    /// <summary>
    /// Null by default or set by the SMILES builder </summary>
    boost::optional<int> minimumValency;

    /// <summary>
    /// This is modified by ium/ide/ylium/uide and is used to choose the appropriate valency for the atom </summary>
    int protonsExplicitlyAddedOrRemoved = 0;

    /// <summary>
    /// Takes same values as type in Fragment. Useful for discriminating suffix atoms from other atoms when a suffix is incorporated into another fragments
    /// </summary>
    std::wstring type;

    /// <summary>
    /// Is this atom in a ring. Default false. Set by the CycleDetector.
    /// Double bonds are only converted to spareValency if atom is in a ring
    /// Some suffixes have different meanings if an atom is part of a ring or not c.g. cyclohexanal vs ethanal
    /// </summary>
    bool atomIsInACycle = false;

    /// <summary>
    /// Builds an Atom from scratch.
    /// GENERALLY EXCEPT FOR TESTING SHOULD NOT BE CALLED EXCEPT FROM THE FRAGMANAGER </summary>
    /// <param name="id"> The ID number, unique to the atom in the molecule being built </param>
    /// <param name="chemlEl"> The chemical element </param>
    /// <param name="frag"> the Fragment to contain the Atom </param>
public:
    virtual ~Atom() {
        delete atomParity;
        delete frag;
    }

    Atom(int id, ChemEl chemlEl, Fragment *frag);

    /// <summary>
    /// Used to build a DUMMY atom.
    /// Does not have an id/frag/type as would be expected for a proper atom </summary>
    /// <param name="chemlEl"> The chemical element </param>
    Atom(ChemEl chemlEl);

    /// <summary>
    /// Uses the lambdaConventionValency or if that is not available
    /// the default valency assuming this is >= the current valency
    /// If not then allowed the chemically sensible valencies of the atom are checked with the one that is closest and >= to the current valency
    /// being returned. If the valency has still not been determined the current valency i.e. assuming the atom to have 0 implicit hydrogen is returned.
    /// This is the correct behaviour for inorganics. For p block elements it means that OPSIN does not believe the atom to be in a valid valency (too high)
    ///
    /// if considerOutValency is true, the valency that will be used to form bonds using the outAtoms is
    /// taken into account i.e. if any radicals were used to form bonds </summary>
    /// <param name="considerOutValency">
    /// @return </param>
    virtual int determineValency(bool considerOutValency);

    /// <summary>
    ///Adds a locant to the Atom. Other locants are preserved.
    /// Also associates the locant with the atom in the parent fragments hash
    /// </summary>
    /// <param name="locant"> The new locant </param>
    virtual void addLocant(const std::wstring &locant);

    /// <summary>
    ///Replaces all existing locants with a new one.
    /// </summary>
    /// <param name="locant"> The new locant </param>
    virtual void replaceLocants(const std::wstring &locant);

    virtual void removeLocant(const std::wstring &locantToRemove);

    /// <summary>
    ///Removes all locants from the Atom.
    ///
    /// </summary>
    virtual void clearLocants();

    /// <summary>
    /// Removes only elementSymbolLocants: e.g. N, S', Se
    /// </summary>
    virtual void removeElementSymbolLocants();

    /// <summary>
    /// Removes all locants other than elementSymbolLocants (e.g. N, S', Se)
    /// Hence removes numeric locants and greek locants
    /// </summary>
    virtual void removeLocantsOtherThanElementSymbolLocants();

    /// <summary>
    ///Checks if the Atom has a given locant.
    /// </summary>
    /// <param name="locant"> The locant to test for </param>
    /// <returns> true if it has, false if not </returns>
    virtual bool hasLocant(const std::wstring &locant);

    /// <summary>
    ///Gets the first locant for the Atom. This may be the locant that was initially
    /// specified, or the most recent locant specified using replaceLocant, or first
    /// locant to be added since the last invocation of clearLocants.
    /// </summary>
    /// <returns> The locant, or null if there is no locant </returns>
    virtual std::wstring getFirstLocant();

    /// <summary>
    ///Returns the array of locants containing all locants associated with the atom
    /// </summary>
    /// <returns> The list of locants (may be empty) </returns>
    virtual std::vector<std::wstring> getLocants();

    /// <summary>
    ///Returns the subset of the locants which are element symbol locants e.g. N, S', Se
    /// </summary>
    /// <returns> The list of locants (may be empty) </returns>
    virtual std::vector<std::wstring> getElementSymbolLocants();

    virtual void setFrag(Fragment *f);

    virtual Fragment *getFrag();

    /// <summary>
    ///Gets the ID of the atom.
    /// </summary>
    /// <returns> The ID of the atom </returns>
    virtual int getID();

    /// <summary>
    ///Gets the chemical element corresponding to the element of the atom.
    /// </summary>
    /// <returns> The chemical element corresponding to the element of the atom </returns>
    virtual ChemEl getElement();

    /// <summary>
    ///Sets the chemical element corresponding to the element of the atom.
    /// </summary>
    /// <param name="chemEl"> The chemical element corresponding to the element of the atom </param>
    virtual void setElement(ChemEl chemEl);

    /// <summary>
    ///Gets the formal charge on the atom.
    /// </summary>
    /// <returns> The formal charge on the atom </returns>
    virtual int getCharge();

    /// <summary>
    ///Modifies the charge of this atom by the amount given. This can be any integer
    /// The number of protons changed is noted so as to calculate the correct valency for the atom. This can be any integer.
    /// For example ide is the loss of a proton so is charge=-1, protons =-1 </summary>
    /// <param name="charge"> </param>
    /// <param name="protons"> </param>
    virtual void addChargeAndProtons(int charge, int protons);

    /// <summary>
    ///Sets the formal charge on the atom.
    /// NOTE: make sure to update protonsExplicitlyAddedOrRemoved if necessary
    /// </summary>
    /// <param name="c"> The formal charge on the atom </param>
    virtual void setCharge(int c);

    /// <summary>
    /// Sets the formal charge and number of protonsExplicitlyAddedOrRemoved to 0
    /// </summary>
    virtual void neutraliseCharge();

    /// <summary>
    /// Gets the mass number of the atom or null if not explicitly defined
    /// e.g. 3 for tritium
    /// @return
    /// </summary>
    virtual boost::optional<int> getIsotope();

    /// <summary>
    /// Sets the mass number of the atom explicitly </summary>
    /// <param name="isotope"> </param>
    virtual void setIsotope(boost::optional<int> isotope);

    /// <summary>
    ///Adds a bond to the atom
    /// </summary>
    /// <param name="b"> The bond to be added </param>
    virtual void addBond(Bond *b);

    /// <summary>
    ///Removes a bond to the atom
    /// </summary>
    /// <param name="b"> The bond to be removed </param>
    /// <returns> whether bond was present </returns>
    virtual bool removeBond(Bond *b);

    /// <summary>
    ///Calculates the number of bonds connecting to the atom, excluding bonds to implicit
    /// hydrogens. Double bonds count as
    /// two bonds, etc. Eg ethene - both C's have an incoming valency of 2.
    /// </summary>
    /// <returns> Incoming Valency </returns>
    virtual int getIncomingValency();

    virtual int getProtonsExplicitlyAddedOrRemoved();

    virtual void setProtonsExplicitlyAddedOrRemoved(int protonsExplicitlyAddedOrRemoved);

    /// <summary>
    ///Does the atom have spare valency to form double bonds?
    /// </summary>
    /// <returns> true if atom has spare valency </returns>
    virtual bool hasSpareValency();

    /// <summary>
    ///Set whether an atom has spare valency
    /// </summary>
    /// <param name="sv"> The spare valency </param>
    virtual void setSpareValency(bool sv);

    /// <summary>
    ///Gets the total bond order of the bonds expected to be created from this atom for inter fragment bonding
    /// </summary>
    /// <returns> The outValency </returns>
    virtual int getOutValency();

    /// <summary>
    ///Adds to the total bond order of the bonds expected to be created from this atom for inter fragment bonding
    /// </summary>
    /// <param name="outV"> The outValency to be added </param>
    virtual void addOutValency(int outV);

    virtual std::vector<Bond *> getBonds();

    virtual int getBondCount();

    /// <summary>
    ///Gets a list of atoms that connect to the atom
    /// </summary>
    /// <returns> The list of atoms connected to the atom </returns>
    virtual std::vector<Atom *> getAtomNeighbours();

    virtual boost::optional<int> getLambdaConventionValency();

    virtual void setLambdaConventionValency(boost::optional<int> valency);

    virtual std::wstring getType();

    virtual void setType(const std::wstring &type);

    virtual bool getAtomIsInACycle();

    /// <summary>
    /// Sets whether atom is in a cycle, true if it is </summary>
    /// <param name="atomIsInACycle"> </param>
    virtual void setAtomIsInACycle(bool atomIsInACycle);

    virtual AtomParity *getAtomParity();

    virtual void setAtomParity(AtomParity *atomParity);

    virtual void setAtomParity(std::vector<Atom *> &atomRefs4, int parity);

    virtual boost::optional<int> getMinimumValency();

    virtual void setMinimumValency(boost::optional<int> minimumValency);

    template<typename T>
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unchecked") <T> T getProperty(PropertyKey<T> propertyKey)
    T getProperty(PropertyKey<T> *propertyKey);

    template<typename T>
    void setProperty(PropertyKey<T> *propertyKey, T value);

    /// <summary>
    /// Checks if the valency of this atom allows it to have the amount of spare valency that the atom currently has
    /// May reduce the spare valency on the atom to be consistent with the valency of the atom
    /// Does nothing if the atom has no spare valency </summary>
    /// <param name="takeIntoAccountExternalBonds"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void ensureSVIsConsistantWithValency(bool takeIntoAccountExternalBonds) throw(StructureBuildingException);

    /// <summary>
    /// Returns the the first bond in the atom's bond list or null if it has no bonds
    /// @return
    /// </summary>
    virtual Bond *getFirstBond();

    /// <summary>
    ///Gets the bond between this atom and a given atom
    /// </summary>
    /// <param name="a"> The atom to find a bond to </param>
    /// <returns> The bond, or null if there is no bond </returns>
    virtual Bond *getBondToAtom(Atom *a);

    /// <summary>
    ///Gets the bond between this atom and a given atom, throwing if fails.
    /// </summary>
    /// <param name="a"> The atom to find a bond to </param>
    /// <returns> The bond found </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual Bond *getBondToAtomOrThrow(Atom *a) throw(StructureBuildingException);
};