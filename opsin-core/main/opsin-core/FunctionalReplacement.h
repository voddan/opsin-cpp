#pragma once

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <utils/Comparator.hpp>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class ComponentGenerationException;
class Element;
class StructureBuildingException;
class Fragment;
class Atom;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


/// <summary>
/// Methods for performing functional replacement
/// @author dl387
///
/// </summary>
class FunctionalReplacement {

    /// <summary>
    /// Sorts infix transformations by the number of acceptable inputs for the transformation.
    /// e.g. thio ends up towards the end of the list as it accepts both -O or =O whilst say imido only accepts =O
    /// @author dl387
    ///
    /// </summary>
private:
    class SortInfixTransformations : public Comparator<std::wstring> {
    public:
        virtual int compare(std::wstring *infixTransformation1, std::wstring *infixTransformation2) override;
    };

private:
    enum class PREFIX_REPLACEMENT_TYPE {
        chalcogen, //ambiguous
        halideOrPseudoHalide, //only mean functional replacement when applied to non carboxylic acids
        dedicatedFunctionalReplacementPrefix, //no ambiguity exists
        hydrazono, //ambiguous, only applies to non carboxylic acid
        peroxy //ambiguous, also applies to etheric oxygen
    };

private:
    static Pattern *const matchChalcogenReplacement = Pattern::compile(L"thio|seleno|telluro");

    BuildState *const state;

public:
    virtual ~FunctionalReplacement() {
        delete state;
    }

    FunctionalReplacement(BuildState *state);

    /// <summary>
    /// Applies the effects of acid replacing functional class nomenclature
    /// This must be performed early so that prefix/infix functional replacement is performed correctly
    /// and so that element symbol locants are assigned appropriately </summary>
    /// <param name="finalSubOrRootInWord"> </param>
    /// <param name="word"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException">  </exception>
    virtual void processAcidReplacingFunctionalClassNomenclature(Element *finalSubOrRootInWord,
                                                                 Element *word) throw(ComponentGenerationException, StructureBuildingException);

    /// <summary>
    /// Performs prefix functional replacement e.g. thio in thioacetic acid replaces an O with S
    /// Prefixes will present themselves as substituents. There is potential ambiguity between usage as a substituent
    /// and as a functional replacement term in some cases. If the substituent is deemed to indicate functional replacement
    /// it will be detached and its effects applied to the subsequent group
    ///
    /// The list of groups and substituents given to this method will be mutated in the process.
    ///
    /// For heterocyclic rings functional replacement should technically be limited to :
    /// pyran, morpholine, chromene, isochromene and xanthene, chromane and isochromane.
    /// but this is not currently enforced </summary>
    /// <param name="groups"> </param>
    /// <param name="substituents"> </param>
    /// <returns> boolean: has any functional replacement occurred </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException">  </exception>
    virtual bool processPrefixFunctionalReplacementNomenclature(std::vector<Element *> &groups,
                                                                std::vector<Element *> &substituents) throw(StructureBuildingException, ComponentGenerationException);

    /// <summary>
    /// Currently prefix replacement terms must be directly adjacent to the groupToBeModified with an exception made
    /// for carbohydrate stereochemistry prefixes e.g. 'gluco' and for substractive prefixes e.g. 'deoxy' </summary>
    /// <param name="groupToBeModified">
    /// @return </param>
private:
    bool groupPrecededByElementThatBlocksPrefixReplacementInterpetation(Element *groupToBeModified);


    /*
     *
     */

    /// <summary>
    /// Performs functional replacement using infixes e.g. thio in ethanthioic acid replaces an O with S </summary>
    /// <param name="suffixFragments"> May be modified if a multiplier is determined to mean multiplication of a suffix, usually untouched </param>
    /// <param name="suffixes"> The suffix elements  May be modified if a multiplier is determined to mean multiplication of a suffix, usually untouched </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
public:
    virtual void processInfixFunctionalReplacementNomenclature(std::vector<Element *> &suffixes,
                                                               std::vector<Fragment *> &suffixFragments) throw(StructureBuildingException, ComponentGenerationException);

    /*
     * Functional class nomenclature
     */

    /// <summary>
    /// Replaces the appropriate number of functional oxygen atoms with the corresponding fragment </summary>
    /// <param name="acidContainingRoot"> </param>
    /// <param name="acidReplacingWord"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
private:
    void processAcidReplacingFunctionalClassNomenclatureFullWord(Element *acidContainingRoot,
                                                                 Element *acidReplacingWord) throw(ComponentGenerationException, StructureBuildingException);


    /// <summary>
    /// Replaces the appropriate number of functional oxygen atoms with the corresponding fragment </summary>
    /// <param name="acidContainingRoot"> </param>
    /// <param name="functionalWord"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void processAcidReplacingFunctionalClassNomenclatureFunctionalWord(Element *acidContainingRoot,
                                                                       Element *functionalWord) throw(ComponentGenerationException, StructureBuildingException);

    Atom *removeOxygenWithAppropriateLocant(std::vector<Atom *> &oxygenAtoms,
                                            const std::wstring &locant) throw(ComponentGenerationException);


    /*
     * Prefix functional replacement nomenclature
     */


    bool acidHasSufficientHydrogenForSubstitutionInterpretation(Fragment *acidFrag,
                                                                int hydrogenRequiredForSubstitutionInterpretation,
                                                                Element *locantEl);

    /// <summary>
    /// Performs replacement of oxygen atoms by chalogen atoms
    /// If this is ambiguous e.g. thioacetate then Atom.AMBIGUOUS_ELEMENT_ASSIGNMENT is populated </summary>
    /// <param name="groupToBeModified"> </param>
    /// <param name="locantEl"> </param>
    /// <param name="numberOfAtomsToReplace"> </param>
    /// <param name="replacementSmiles">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    int performChalcogenFunctionalReplacement(Element *groupToBeModified, Element *locantEl, int numberOfAtomsToReplace,
                                              const std::wstring &replacementSmiles) throw(StructureBuildingException);


    /// <summary>
    /// Converts functional oxygen to peroxy e.g. peroxybenzoic acid
    /// Returns the number of oxygen replaced </summary>
    /// <param name="groupToBeModified"> </param>
    /// <param name="locantEl"> </param>
    /// <param name="numberOfAtomsToReplace">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    int performPeroxyFunctionalReplacement(Element *groupToBeModified, Element *locantEl,
                                           int numberOfAtomsToReplace) throw(StructureBuildingException);

    /// <summary>
    /// Replaces double bonded oxygen and/or single bonded oxygen depending on the input SMILES
    /// SMILES with a valency 1 outAtom replace -O, SMILES with a valency 2 outAtom replace =O
    /// SMILES with a valency 3 outAtom replace -O and =O (nitrido)
    /// Returns the number of oxygen replaced </summary>
    /// <param name="groupToBeModified"> </param>
    /// <param name="locantEl"> </param>
    /// <param name="numberOfAtomsToReplace"> </param>
    /// <param name="replacementSmiles">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    int performFunctionalReplacementOnAcid(Element *groupToBeModified, Element *locantEl, int numberOfAtomsToReplace,
                                           const std::wstring &replacementSmiles) throw(StructureBuildingException);

    /*
     * Infix functional replacement nomenclature
     */

    /// <summary>
    /// This block handles infix multiplication. Unless brackets are provided this is ambiguous without knowledge of the suffix that is being modified
    /// For example butandithione could be intepreted as butandi(thione) or butan(dithi)one.
    /// Obviously the latter is wrong in this case but it is the correct interpretation for butandithiate </summary>
    /// <param name="suffixes"> </param>
    /// <param name="suffixFragments"> </param>
    /// <param name="suffix"> </param>
    /// <param name="infixTransformations"> </param>
    /// <param name="oxygenAvailable"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void disambiguateMultipliedInfixMeaning(std::vector<Element *> &suffixes, std::vector<Fragment *> &suffixFragments,
                                            Element *suffix, std::vector<std::wstring> &infixTransformations,
                                            int oxygenAvailable) throw(ComponentGenerationException, StructureBuildingException);

    /*
     * Convenience Methods
     */

    /// <summary>
    /// Given an atom that is to be replaced by a functional replacement fragment
    /// determines whether this atom is a functional atom and, if it is, performs the following processes:
    /// The functionalAtom is removed. If the the replacement fragment is an atom of O/S/Se/Te or the
    /// the terminal atom of the fragment is a single bonded O/S/Se/Te a functionAom is added to this atom. </summary>
    /// <param name="atomToBeReplaced"> </param>
    /// <param name="replacementFrag"> </param>
    void removeOrMoveObsoleteFunctionalAtoms(Atom *atomToBeReplaced, Fragment *replacementFrag);

    /// <summary>
    /// Given an atom that is to be replaced by a functional replacement fragment
    /// determines whether this atom has outvalency and if it does removes the outatom from the atom's fragment
    /// and adds an outatom to the replacementFrag </summary>
    /// <param name="atomToBeReplaced"> </param>
    /// <param name="replacementFrag"> </param>
    void moveObsoleteOutAtoms(Atom *atomToBeReplaced, Fragment *replacementFrag);

    void removeAssociatedFunctionalAtom(Atom *atomWithFunctionalAtom) throw(StructureBuildingException);


    /// <summary>
    /// Returns the subset of oxygenAtoms that possess one of the locants in locantEl
    /// Searches for locant on nearest non suffix atom in case of suffixes </summary>
    /// <param name="locantEl"> </param>
    /// <param name="oxygenAtoms">
    /// @return </param>
    std::vector<Atom *> pickOxygensWithAppropriateLocants(Element *locantEl, std::vector<Atom *> &oxygenAtoms);

    /// <summary>
    /// Returns oxygen atoms in suffixes with functionalAtoms </summary>
    /// <param name="groupToBeModified">
    /// @return </param>
    std::vector<Atom *> findFunctionalOxygenAtomsInApplicableSuffixes(Element *groupToBeModified);

    /// <summary>
    /// Returns functional oxygen atoms in groupToBeModified </summary>
    /// <param name="groupToBeModified">
    /// @return </param>
    std::vector<Atom *> findFunctionalOxygenAtomsInGroup(Element *groupToBeModified);


    /// <summary>
    /// Returns etheric oxygen atoms in groupToBeModified </summary>
    /// <param name="groupToBeModified">
    /// @return </param>
    std::vector<Atom *> findEthericOxygenAtomsInGroup(Element *groupToBeModified);


    /// <summary>
    /// Returns oxygen atoms in suffixes with functionalAtoms or acidStem suffixes or aldehyde suffixes (1979 C-531) </summary>
    /// <param name="groupToBeModified">
    /// @return </param>
    std::vector<Atom *> findOxygenAtomsInApplicableSuffixes(Element *groupToBeModified);

    /// <summary>
    /// Returns oxygen atoms in groupToBeModified </summary>
    /// <param name="groupToBeModified">
    /// @return </param>
    std::vector<Atom *> findOxygenAtomsInGroup(Element *groupToBeModified);


    void
    populateTerminalSingleAndDoubleBondedOxygen(std::vector<Atom *> &atomList, std::vector<Atom *> &singleBondedOxygen,
                                                std::vector<Atom *> &doubleBondedOxygen) throw(StructureBuildingException);
};

