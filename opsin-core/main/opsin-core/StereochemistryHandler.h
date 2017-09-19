#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class Atom;
class StereoAnalyser;
class StereoCentre;
class Bond;
class StereoBond;
class Element;
class StructureBuildingException;
class StereochemistryException;
class Fragment;


using StereoBond = uk::ac::cam::ch::wwmm::opsin::StereoAnalyser::StereoBond;
using StereoCentre = uk::ac::cam::ch::wwmm::opsin::StereoAnalyser::StereoCentre;
using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

/// <summary>
/// Identifies stereocentres, assigns stereochemistry elements to them and then uses the CIP rules to calculate appropriates atomParity/bondstereo tags
/// @author dl387
///
/// </summary>
class StereochemistryHandler {

private:
    BuildState *const state;
    const std::unordered_map<Atom *, StereoCentre *> atomStereoCentreMap;
    const std::unordered_map<Bond *, StereoBond *> bondStereoBondMap;
    const std::unordered_map<Atom *, StereoCentre *> notExplicitlyDefinedStereoCentreMap;
    const std::unordered_map<Bond *, StereoBond *> notExplicitlyDefinedStereoBondMap;

public:
    virtual ~StereochemistryHandler() {
        delete state;
    }

    StereochemistryHandler(BuildState *state, std::unordered_map<Atom *, StereoCentre *> &atomStereoCentreMap,
                           std::unordered_map<Bond *, StereoBond *> &bondStereoBondMap);

    /// <summary>
    /// Processes and assigns stereochemistry elements to appropriate fragments </summary>
    /// <param name="stereoChemistryEls"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void
    applyStereochemicalElements(std::vector<Element *> &stereoChemistryEls) throw(StructureBuildingException);

    /// <summary>
    /// Checks that all atomParity and bondStereo elements correspond to identified stereocentres.
    /// If they do not, they have assumedly been removed by substitution and hence the atomPaity/bondStereo is removed </summary>
    /// <param name="bondsWithPreDefinedBondStereo"> </param>
    /// <param name="atomsWithPreDefinedAtomParity">  </param>
    virtual void removeRedundantStereoCentres(std::vector<Atom *> &atomsWithPreDefinedAtomParity,
                                              std::vector<Bond *> &bondsWithPreDefinedBondStereo);

    /// <summary>
    /// Attempts to locate a suitable atom/bond for the stereochemistryEl, applies it and detaches the stereochemsitry </summary>
    /// <param name="stereoChemistryEl"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="StereochemistryException">  </exception>
private:
    void matchStereochemistryToAtomsAndBonds(
            Element *stereoChemistryEl) throw(StructureBuildingException, StereochemistryException);

    /// <summary>
    /// Groups carbohydrateStereoChemistryEls by their parent element and
    /// sends them for further processing </summary>
    /// <param name="carbohydrateStereoChemistryEls"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void processCarbohydrateStereochemistry(
            std::vector<Element *> &carbohydrateStereoChemistryEls) throw(StructureBuildingException);

    /// <summary>
    /// Handles R/S stereochemistry. r/s is not currently handled </summary>
    /// <param name="stereoChemistryEl"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="StereochemistryException">  </exception>
    void assignStereoCentre(Element *stereoChemistryEl) throw(StructureBuildingException, StereochemistryException);


    bool attemptAssignmentOfStereoCentreToFragment(Fragment *fragment, const std::wstring &rOrS,
                                                   const std::wstring &locant) throw(StereochemistryException, StructureBuildingException);

    /// <summary>
    /// Assigns atom parity to the given atom in accordance with the CIP rules </summary>
    /// <param name="atom"> The stereoAtom </param>
    /// <param name="stereoCentre"> </param>
    /// <param name="rOrS"> The description given in the name </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="StereochemistryException"> </exception>
    void applyStereoChemistryToStereoCentre(Atom *atom, StereoCentre *stereoCentre,
                                            const std::wstring &rOrS) throw(StructureBuildingException, StereochemistryException);


    /// <summary>
    /// Handles E/Z stereochemistry and cis/trans in cases where cis/trans unambiguously corresponds to E/Z </summary>
    /// <param name="stereoChemistryEl"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="StereochemistryException">  </exception>
    void assignStereoBond(Element *stereoChemistryEl) throw(StructureBuildingException, StereochemistryException);


    bool
    attemptAssignmentOfStereoBondToFragment(Fragment *fragment, const std::wstring &eOrZ, const std::wstring &locant,
                                            bool isCisTrans) throw(StereochemistryException);

    /// <summary>
    /// Does the stereoBond have a hydrogen connected to both ends of it.
    /// If not it is ambiguous when used in conjunction with cis/trans and E/Z should be used. </summary>
    /// <param name="potentialBond">
    /// @return </param>
public:
    static bool cisTransUnambiguousOnBond(Bond *potentialBond);


    /// <summary>
    /// Sorts bonds such that those originating from the given fragment are preferred </summary>
    /// <param name="interFragmentBonds"> A set of interFragment Bonds </param>
    /// <param name="preferredOriginatingFragment"> </param>
    /// <returns> A sorted list </returns>
private:
    std::vector<Bond *> sortInterFragmentBonds(Set<Bond *> *interFragmentBonds, Fragment *preferredOriginatingFragment);


    /// <summary>
    /// Assigns bondstereo to the given bond in accordance with the CIP rules </summary>
    /// <param name="bond"> The stereobond </param>
    /// <param name="stereoBond"> </param>
    /// <param name="eOrZ"> The stereo description given in the name </param>
    /// <exception cref="StereochemistryException">  </exception>
    void applyStereoChemistryToStereoBond(Bond *bond, StereoBond *stereoBond,
                                          const std::wstring &eOrZ) throw(StereochemistryException);

    /// <summary>
    /// Searches for instances of two tetrahedral stereocentres/psuedo-stereocentres
    /// then sets their configuration such that the substituents at these centres are cis or trans to each other </summary>
    /// <param name="stereoChemistryEl">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    bool assignCisTransOnRing(Element *stereoChemistryEl) throw(StructureBuildingException);


    bool attemptAssignmentOfCisTransRingStereoToFragment(Fragment *fragment,
                                                         Element *stereoChemistryEl) throw(StructureBuildingException);

    Set<Bond *> *determinePeripheryBonds(Fragment *fragment);

    void applyStereoChemistryToCisTransOnRing(Atom *a1, Atom *a2, std::vector<std::vector<Atom *>> &paths,
                                              std::vector<Atom *> &fragmentAtoms,
                                              const std::wstring &cisOrTrans) throw(StructureBuildingException);

    Atom *getHydrogenOrAcyclicOrOutsideOfFragment(std::vector<Atom *> &atoms, std::vector<Atom *> &fragmentAtoms);

    /// <summary>
    /// Handles assignment of alpha and beta stereochemistry to appropriate ring systems
    /// Currently these are only assignable to natural products
    /// Xi (unknown) stereochemistry is applicable to any tetrahedral centre </summary>
    /// <param name="stereoChemistryEl"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void assignAlphaBetaXiStereochem(Element *stereoChemistryEl) throw(StructureBuildingException);


    /// <summary>
    /// Converts the alpha/beta descriptor into an atomRefs4 and parity.
    /// The ordering of atoms in the atomsRefs4 is determined by using the two adjacent atoms along the rings edge as defined by ALPHABETACLOCKWISEATOMORDERING_ATR.
    /// by what atom is also part of the ring or is a hydrogen
    /// and by the substituent atom (as determined by the optional substituentGroup group parameter or by being a non-hydrogen) </summary>
    /// <param name="stereoAtom"> </param>
    /// <param name="fragment"> </param>
    /// <param name="alphaBetaClockWiseAtomOrdering"> </param>
    /// <param name="alphaOrBeta"> </param>
    /// <param name="substituentGroup"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void applyAlphaBetaStereochemistryToStereoCentre(Atom *stereoAtom, Fragment *fragment,
                                                     const std::wstring &alphaBetaClockWiseAtomOrdering,
                                                     const std::wstring &alphaOrBeta,
                                                     Fragment *substituentGroup) throw(StructureBuildingException);


    /// <summary>
    /// Applies carbohydate configurational prefixes to the appropriate carbohydrateStem </summary>
    /// <param name="carbohydrateGroup"> </param>
    /// <param name="carbohydrateStereoChemistryEls"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void assignCarbohydratePrefixStereochem(Element *carbohydrateGroup,
                                            std::vector<Element *> &carbohydrateStereoChemistryEls) throw(StructureBuildingException);

    void assignDlStereochem(Element *stereoChemistryEl) throw(StructureBuildingException);


    bool attemptAssignmentOfDlStereoToFragment(Fragment *fragment,
                                               const std::wstring &dOrL) throw(StereochemistryException, StructureBuildingException);

public:
    static int swapsRequiredToSort(std::vector<Atom *> &atomRefs4);

    static bool checkEquivalencyOfAtomsRefs4AndParity(std::vector<Atom *> &atomRefs1, int atomParity1,
                                                      std::vector<Atom *> &atomRefs2, int atomParity2);
};


