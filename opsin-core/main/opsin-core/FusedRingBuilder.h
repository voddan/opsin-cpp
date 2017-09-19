#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <boost/optional.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class Element;
class Fragment;
class Atom;
class StructureBuildingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

/// <summary>
/// Assembles fused rings named using fusion nomenclature
/// @author dl387
///
/// </summary>
class FusedRingBuilder {
private:
    BuildState *const state;
    const std::vector<Element *> groupsInFusedRing;
    Element *const lastGroup;
    Fragment *const parentRing;
    const std::unordered_map<int, Fragment *> fragmentInScopeForEachFusionLevel = std::unordered_map<int, Fragment *>();
    const std::unordered_map<Atom *, Atom *> atomsToRemoveToReplacementAtom = std::unordered_map<Atom *, Atom *>();

public:
    virtual ~FusedRingBuilder() {
        delete state;
        delete lastGroup;
        delete parentRing;
    }

private:
    FusedRingBuilder(BuildState *state, std::vector<Element *> &groupsInFusedRing);

    /// <summary>
    /// Master method for processing fused rings. Fuses groups together </summary>
    /// <param name="state">: contains the current id and fragment manager </param>
    /// <param name="subOrRoot"> Element (substituent or root) </param>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static void processFusedRings(BuildState *state, Element *subOrRoot) throw(StructureBuildingException);

    /// <summary>
    /// Combines the groups given in the <seealso cref="FusedRingBuilder"/> constructor to destructively create the fused ring system
    /// This fused ring is then numbered </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void buildFusedRing() throw(StructureBuildingException);

private:
    void removeMergedAtoms();

    /// <summary>
    /// Forms a list a list of all group and fusion elements between the first and last group in the fused ring
    /// @return
    /// </summary>
    std::vector<Element *> formNameComponentList();

    void processRingNumberingAndIrregularities() throw(StructureBuildingException);

    /// <summary>
    /// Interprets the unlocanted unsaturator after a partially unsaturated HW Rings as indication of spare valency and detaches it
    /// This is necessary as this unsaturator can only refer to the HW ring and for names like 2-Benzoxazolinone to avoid confusion as to what the 2 refers to. </summary>
    /// <param name="group"> </param>
    /// <param name="ring"> </param>
    void processPartiallyUnsaturatedHWSystems(Element *group, Fragment *ring);

    /// <summary>
    /// Given a cyclicAlkaneGroup determines whether or not it should be aromatised. Unlocanted ene will be detached if it is an aromatisation hint
    /// No unsaturators -->aromatise
    /// Just ane -->don't
    /// More than 1 ene or locants on ene -->don't
    /// yne --> don't </summary>
    /// <param name="cyclicAlkaneGroup"> </param>
    void aromatiseCyclicAlkane(Element *cyclicAlkaneGroup);

    int processMultiParentSystem(std::vector<Fragment *> &parentFragments, std::vector<Element *> &nameComponents,
                                 std::vector<Fragment *> &componentFragments) throw(StructureBuildingException);

    /// <summary>
    /// Splits a first order fusion component into it's numerical and letter parts
    /// Either one of these can be the blank string as they may have been omitted
    /// The first entry in the array is the numbers and the second the letters </summary>
    /// <param name="fusionDescriptor">
    /// @return </param>
    std::vector<std::wstring> determineNumericalAndLetterComponents(const std::wstring &fusionDescriptor);

    /// <summary>
    /// Searches groups for benz(o) components and fuses them in accordance with
    /// FR-2.2.8 Heterobicyclic components with a benzene ring </summary>
    /// <exception cref="StructureBuildingException"> </exception>
    void processBenzoFusions() throw(StructureBuildingException);

    /// <summary>
    /// Modifies nextComponent's locants according to the fusionLevel. </summary>
    /// <param name="component"> </param>
    /// <param name="fusionLevel"> </param>
    void relabelAccordingToFusionLevel(Fragment *component, int fusionLevel);

    /// <summary>
    /// Handles fusion between components where the fusion descriptor is of the form:
    /// comma separated locants dash letters
    /// e.g imidazo[4,5-d]pyridine
    /// The fusionDescriptor may be given as null or the letter/numerical part omitted.
    /// Sensible defaults will be found instead </summary>
    /// <param name="fusionDescriptor"> </param>
    /// <param name="childRing"> </param>
    /// <param name="parentRing"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void performSimpleFusion(const std::wstring &fusionDescriptor, Fragment *childRing,
                             Fragment *parentRing) throw(StructureBuildingException);

    /// <summary>
    /// Takes a ring an returns and array with one letter corresponding to a side/s
    /// that contains two adjacent non bridgehead carbons
    /// The number of sides is specified by edgeLength </summary>
    /// <param name="ring"> </param>
    /// <param name="edgeLength"> The number of bonds to be fused along
    /// @return </param>
    std::vector<std::wstring> findPossibleLetterLocants(Fragment *ring, int edgeLength);

    /// <summary>
    /// Takes a ring and returns an array of numbers corresponding to a side/s
    /// that contains two adjacent non bridgehead carbons
    /// The number of sides is specified by edgeLength </summary>
    /// <param name="ring"> </param>
    /// <param name="edgeLength"> The number of bonds to be fused along
    /// @return </param>
    std::vector<std::wstring> findPossibleNumericalLocants(Fragment *ring, int edgeLength);

    /// <summary>
    /// Performs a single ring fusion using the values in numericalLocantsOfChild/letterLocantsOfParent </summary>
    /// <param name="childRing"> </param>
    /// <param name="parentRing"> </param>
    /// <param name="numericalLocantsOfChild"> </param>
    /// <param name="letterLocantsOfParent"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void processFirstOrderFusionDescriptors(Fragment *childRing, Fragment *parentRing,
                                            std::vector<std::wstring> &numericalLocantsOfChild,
                                            std::vector<std::wstring> &letterLocantsOfParent) throw(StructureBuildingException);

    /// <summary>
    /// Returns the sublist of the given atoms that are peripheral atoms given that the list is ordered such that the interior atoms are at the end of the list </summary>
    /// <param name="atomList">
    /// @return </param>
    std::vector<Atom *> getPeripheralAtoms(std::vector<Atom *> &atomList);

    /// <summary>
    /// Handles fusion between components where the fusion descriptor is of the form:
    /// comma separated locants colon comma separated locants
    /// e.g pyrido[1'',2'':1',2']imidazo </summary>
    /// <param name="fusionDescriptor"> </param>
    /// <param name="nextComponent"> </param>
    /// <param name="fusedRing"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void performHigherOrderFusion(const std::wstring &fusionDescriptor, Fragment *nextComponent,
                                  Fragment *fusedRing) throw(StructureBuildingException);

    /// <summary>
    /// Performs a single ring fusion using the values in numericalLocantsOfChild/numericalLocantsOfParent </summary>
    /// <param name="childRing"> </param>
    /// <param name="parentRing"> </param>
    /// <param name="numericalLocantsOfChild"> </param>
    /// <param name="numericalLocantsOfParent"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void processHigherOrderFusionDescriptors(Fragment *childRing, Fragment *parentRing,
                                             std::vector<std::wstring> &numericalLocantsOfChild,
                                             std::vector<std::wstring> &numericalLocantsOfParent) throw(StructureBuildingException);

    /// <summary>
    /// Determines which atoms on a ring should be used for fusion given a set of numerical locants.
    /// If from the other ring involved in the fusion it is known how many atoms are expected to be found this should be provided
    /// If this is not known it should be set to null and the smallest number of fusion atoms will be returned. </summary>
    /// <param name="ring"> </param>
    /// <param name="numericalLocantsOnRing"> </param>
    /// <param name="expectedNumberOfAtomsToBeUsedForFusion">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    std::vector<Atom *> determineAtomsToFuse(Fragment *ring, std::vector<std::wstring> &numericalLocantsOnRing,
                                             boost::optional<int> expectedNumberOfAtomsToBeUsedForFusion) throw(StructureBuildingException);

    /// <summary>
    /// Creates the bonds required to fuse two rings together.
    /// The child atoms are recorded as atoms that should be removed later </summary>
    /// <param name="childAtoms"> </param>
    /// <param name="parentAtoms"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void fuseRings(std::vector<Atom *> &childAtoms, std::vector<Atom *> &parentAtoms) throw(StructureBuildingException);

    /// <summary>
    /// Fuse the benzo with the subsequent ring
    /// Uses locants in front of the benz/benzo group to assign heteroatoms on the now numbered fused ring system </summary>
    /// <param name="benzoEl"> </param>
    /// <param name="parentEl"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void benzoSpecificFusion(Element *benzoEl, Element *parentEl) throw(StructureBuildingException);

    /// <summary>
    /// Checks for locant(s) before benzo and uses these to set </summary>
    /// <param name="benzoEl"> </param>
    /// <param name="fusedRing"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void setBenzoHeteroatomPositioning(Element *benzoEl, Fragment *fusedRing) throw(StructureBuildingException);

    bool locantsCouldApplyToHeteroatomPositions(std::vector<std::wstring> &locants, Element *benzoEl);

    bool locantsAreAllNumeric(std::vector<std::wstring> &locants);
};

