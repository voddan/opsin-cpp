#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <boost/optional.hpp>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class Atom;
class Element;
class Fragment;
class StructureBuildingException;
class BuildResults;


using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.StructureBuildingMethods.*;

/// <summary>
///Constructs a single OPSIN fragment which describes the molecule from the ComponentGenerator/ComponentProcessor results.
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class StructureBuilder {
private:
    BuildState *const state;
    const std::vector<Atom *> polymerAttachmentPoints = std::vector<Atom *>(); //rGroups need to be represented as normal atoms for the purpose of working out stereochemistry. They will be converted to a suitable representation later

    int currentTopLevelWordRuleCount = 0;

public:
    virtual ~StructureBuilder() {
        delete state;
    }

    StructureBuilder(BuildState *state);

    /// <summary>
    ///	Builds a molecule as a Fragment based on ComponentProcessor output. </summary>
    /// <param name="molecule"> The ComponentProcessor output. </param>
    /// <returns> A single Fragment - the built molecule. </returns>
    /// <exception cref="StructureBuildingException"> If the molecule won't build - there may be many reasons. </exception>
    virtual Fragment *buildFragment(Element *molecule) throw(StructureBuildingException);


private:
    void processWordRuleChildrenThenRule(Element *wordRule) throw(StructureBuildingException);

    void processWordRule(Element *wordRuleEl) throw(StructureBuildingException);


    void buildEster(std::vector<Element *> &words) throw(StructureBuildingException);


    void buildDiValentFunctionalGroup(std::vector<Element *> &words) throw(StructureBuildingException);

    void buildMonovalentFunctionalGroup(std::vector<Element *> &words) throw(StructureBuildingException);

    void buildFunctionalClassEster(std::vector<Element *> &words) throw(StructureBuildingException);

    /// <summary>
    /// Handles names like thiophene 1,1-dioxide; carbon dioxide; benzene oxide
    /// Does the same for sulfide/selenide/telluride </summary>
    /// <param name="words"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void buildOxide(std::vector<Element *> &words) throw(StructureBuildingException);

    /// <summary>
    /// Decides whether an oxide should double bond e.g. P=O or single bond as a zwitterionic form e.g. [N+]-[O-]
    /// Corrects the charges if necessary and forms the bond </summary>
    /// <param name="atomToAddOxideTo"> </param>
    /// <param name="oxideAtom"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void formAppropriateBondToOxideAndAdjustCharges(Atom *atomToAddOxideTo,
                                                    Atom *oxideAtom) throw(StructureBuildingException);

    void buildCarbonylDerivative(std::vector<Element *> &words) throw(StructureBuildingException);

    void replaceCarbonylOxygenWithReplacementFragments(std::vector<Element *> &words,
                                                       std::vector<Fragment *> &replacementFragments,
                                                       std::vector<Atom *> &carbonylOxygens,
                                                       int functionalReplacementsToPerform) throw(StructureBuildingException);

    /// <summary>
    /// Given a fragment and optionally a list of locants finds carbonyl atoms
    /// If locants are given the carbonyl must be assoicated with one of the given locants </summary>
    /// <param name="fragment"> </param>
    /// <param name="locantForCarbonylAtom">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    std::vector<Atom *> findCarbonylOxygens(Fragment *fragment,
                                            std::vector<std::wstring> &locantForCarbonylAtom) throw(StructureBuildingException);

    void buildAnhydride(std::vector<Element *> &words) throw(StructureBuildingException);

    /// <summary>
    /// Given buildResults for both the acids and the SMILES of the anhydride forms the anhydride bond using the first functionalAtom on each BuildResults </summary>
    /// <param name="anhydrideSmiles"> </param>
    /// <param name="acidBr1"> </param>
    /// <param name="acidBr2"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void formAnhydrideLink(const std::wstring &anhydrideSmiles, BuildResults *acidBr1,
                           BuildResults *acidBr2) throw(StructureBuildingException);

    /// <summary>
    /// Given two atoms and the SMILES of the anhydride forms the anhydride bond </summary>
    /// <param name="anhydrideSmiles"> </param>
    /// <param name="oxygen1"> </param>
    /// <param name="oxygen2"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void formAnhydrideLink(const std::wstring &anhydrideSmiles, Atom *oxygen1,
                           Atom *oxygen2) throw(StructureBuildingException);

    void buildAcidHalideOrPseudoHalide(std::vector<Element *> &words) throw(StructureBuildingException);

    void buildAdditionCompound(std::vector<Element *> &words) throw(StructureBuildingException);

    void applyAluminiumHydrideSpecialCase(Element *firstWord, Atom *elementaryAtom,
                                          std::vector<Fragment *> &functionalGroupFragments) throw(StructureBuildingException);


    void buildGlycol(std::vector<Element *> &words) throw(StructureBuildingException);


    /// <summary>
    /// Handles Glcyol ethers nomenclature e.g.
    /// triethylene glycol n-butyl ether
    /// tripropylene glycol methyl ether
    /// dipropylene glycol methyl ether acetate </summary>
    /// <param name="words"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void buildGlycolEther(std::vector<Element *> &words) throw(StructureBuildingException);

    /// <summary>
    /// Builds acetals/ketals/hemiacetals/hemiketals and chalcogen analogues
    /// The distinction between acetals and ketals is not enforced (ketals are a subset of acetals) </summary>
    /// <param name="words"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void buildAcetal(std::vector<Element *> &words) throw(StructureBuildingException);

    void replaceChalcogensInAcetal(Element *functionalClassEl,
                                   std::vector<std::wstring> &elements) throw(StructureBuildingException);

    Fragment *formAcetal(std::vector<Atom *> &carbonylOxygen,
                         std::vector<std::wstring> &elements) throw(StructureBuildingException);

    bool buildAlcoholEster(std::vector<Element *> &words, int numberOfWordRules) throw(StructureBuildingException);

    void buildAmineDiConjunctiveSuffix(std::vector<Element *> &words) throw(StructureBuildingException);

    static Pattern *const matchCommonCarboxylicSalt = Pattern::compile(L"tri-?fluoro-?acetate?$",
                                                                       Pattern::CASE_INSENSITIVE);
    static Pattern *const matchCommonEsterFormingInorganicSalt = Pattern::compile(
            L"(ortho-?)?(bor|phosphor|phosphate?|phosphite?)|carbam|carbon|sulfur|sulfate?|sulfite?|diphosphate?|triphosphate?",
            Pattern::CASE_INSENSITIVE);

    /// <summary>
    /// CAS endorses the use of ...ol ...ate names means esters
    /// but only for cases involving "common acids":
    /// Acetic acid; Benzenesulfonic acid; Benzenesulfonic acid, 4-methyl-; Benzoic acid and its monoamino, mononitro, and dinitro derivatives;
    /// Boric acid (H3BO3); Carbamic acid; Carbamic acid, N-methyl-; Carbamic acid, N-phenyl-; Carbonic acid; Formic acid; Methanesulfonic acid;
    /// Nitric acid; Phosphoric acid; Phosphorodithioic acid; Phosphorothioic acid; Phosphorous acid; Propanoic acid; Sulfuric acid; and Sulfurous acid.
    /// ...unless the alcohol component is also common.
    ///
    /// As in practice a lot of use won't be from CAS names we use the following heuristic:
    /// Is locanted OR
    /// Has 1 functional atom  (And not common salt e.g. Trifluoroacetate) OR
    /// common phosphorus/sulfur ate including di/tri phosphate </summary>
    /// <param name="ateWord"> </param>
    /// <param name="wordBr">
    /// @return </param>
    /// <exception cref="StructureBuildingException">  </exception>
    bool isAppropriateAteGroupForAlcoholEster(Element *ateWord, BuildResults *wordBr) throw(StructureBuildingException);

    void splitAlcoholEsterRuleIntoTwoSimpleWordRules(std::vector<Element *> &words);

    void connectSubstituentsToAcetal(std::vector<Fragment *> &acetalFrags, BuildResults *subBr,
                                     bool hemiacetal) throw(StructureBuildingException);

    void buildCyclicPeptide(std::vector<Element *> &words) throw(StructureBuildingException);

    void buildPolymer(std::vector<Element *> &words) throw(StructureBuildingException);

    /// <summary>
    /// Finds a suitable functional atom corresponding to the given locant
    /// Takes into account situations where function replacement may have resulted in the wrong atoms being functional atoms </summary>
    /// <param name="locant"> </param>
    /// <param name="mainGroupBR"> </param>
    /// <returns> functionalAtomToUse </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    Atom *determineFunctionalAtomToUse(const std::wstring &locant,
                                       BuildResults *mainGroupBR) throw(StructureBuildingException);

    /// <summary>
    /// Applies explicit stoichiometry, charge balancing and fractional multipliers </summary>
    /// <param name="molecule"> </param>
    /// <param name="wordRules"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void
    manipulateStoichiometry(Element *molecule, std::vector<Element *> &wordRules) throw(StructureBuildingException);

    bool applyExplicitStoichiometryIfProvided(std::vector<Element *> &wordRules) throw(StructureBuildingException);


    void deprotonateAcidIfSaltWithMetal(Element *molecule);

    /// <summary>
    /// A net charge is present; Given the molecule element the overallCharge is there an unambiguous way of
    /// multiplying fragments to make the net charge 0
    /// metals without specified charge may be given an implicit positive charge
    ///
    /// If this fails look for the case where there are multiple molecules and the mixture is only negative due to negatively charged functional Atoms e.g. pyridine acetate and remove the negative charge </summary>
    /// <param name="molecule"> </param>
    /// <param name="explicitStoichiometryPresent"> </param>
    /// <param name="overallCharge"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void balanceChargeIfPossible(Element *molecule, int overallCharge,
                                 bool explicitStoichiometryPresent) throw(StructureBuildingException);

    std::vector<Element *> getMetalsThatCanBeImplicitlyCations(Element *molecule);


    /// <summary>
    /// Sets the cationicElements to the lowest typical charge as specified by the COMMONOXIDATIONSTATESANDMAX_ATR that is >= incoming valency
    /// The valency incoming to the cationicElement is taken into account e.g. phenylmagnesium chloride is [Mg+] </summary>
    /// <param name="cationicElements"> </param>
    /// <param name="overallCharge">
    /// @return </param>
    int setCationicElementsToTypicalCharge(std::vector<Element *> &cationicElements, int overallCharge);

    /// <summary>
    /// Checks for tribromide/triodide and joins the ions if found </summary>
    /// <param name="wordRules">
    /// @return </param>
    bool triHalideSpecialCase(std::vector<Element *> &wordRules);

    /// <summary>
    /// Multiplies out charged word rules to balance charge
    /// Return true if balancing was possible else false </summary>
    /// <param name="negativelyChargedComponents"> </param>
    /// <param name="positivelyChargedComponents"> </param>
    /// <param name="componentToChargeMapping"> </param>
    /// <param name="overallCharge">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    bool multiplyChargedComponents(std::vector<Element *> &negativelyChargedComponents,
                                   std::vector<Element *> &positivelyChargedComponents,
                                   std::unordered_map<Element *, int> &componentToChargeMapping,
                                   int overallCharge) throw(StructureBuildingException);

    bool componentCanBeMultiplied(Element *componentToMultiply);

    bool setChargeOnCationicElementAppropriately(int overallCharge, Element *cationicElement);

    Element *findRightMostGroupInWordOrWordRule(Element *wordOrWordRule) throw(StructureBuildingException);

    /// <summary>
    /// Nasty special case to cope with oxido and related groups acting as O= or even [O-][N+]
    /// This nasty behaviour is in generated ChemDraw names and is supported by most nameToStructure tools so it is supported here
    /// Acting as O= notably is often correct behaviour for inorganics
    ///
    /// Methionine (and the like) when substituted at the sulfur/selenium/tellurium are implicitly positively charged </summary>
    /// <param name="groups"> </param>
    void processOxidoAndMethionineSpecialCases(std::vector<Element *> &groups);

    /// <summary>
    /// Is the atom connected to an atom whose fragment has an xml entry called "oxo" </summary>
    /// <param name="atom">
    /// @return </param>
    bool checkForConnectedOxo(Atom *atom);


    /// <summary>
    /// Sets the charge according to the oxidation number if the oxidation number atom property has been set </summary>
    /// <param name="groups"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void processOxidationNumbers(std::vector<Element *> &groups) throw(StructureBuildingException);

    /// <summary>
    /// Handles the application of stereochemistry and checking
    /// existing stereochemical specification is still relevant. </summary>
    /// <param name="molecule"> </param>
    /// <param name="uniFrag"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void processStereochemistry(Element *molecule, Fragment *uniFrag) throw(StructureBuildingException);

    /// <summary>
    /// Finds stereochemistry els in a recursive right to left manner.
    /// Within the same scope though stereochemistry els are found left to right </summary>
    /// <param name="parentEl">
    /// @return </param>
    std::vector<Element *> findStereochemistryElsInProcessingOrder(Element *parentEl);

    void convertOutAtomsToAttachmentAtoms(Fragment *uniFrag) throw(StructureBuildingException);

    /// <summary>
    /// Returns the atom corresponding to position i in the outAtoms list
    /// If the outAtom is not set explicitly a suitable atom will be found </summary>
    /// <param name="buildResults"> </param>
    /// <param name="i"> index </param>
    /// <returns> atom </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    Atom *getOutAtomTakingIntoAccountWhetherSetExplicitly(BuildResults *buildResults,
                                                          int i) throw(StructureBuildingException);
};

