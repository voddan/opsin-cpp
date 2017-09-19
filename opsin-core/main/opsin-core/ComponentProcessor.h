#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cctype>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class FunctionalReplacement;
class SuffixApplier;
class BuildState;
class ComponentGenerationException;
class Element;
class StructureBuildingException;
class Fragment;
class Atom;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

/// <summary>
///Performs structure-aware destructive procedural parsing on parser results.
///
/// @author dl387
///
/// </summary>

class ComponentProcessor {
private:
    static const boost::regex matchAddedHydrogenBracket {L"[\\[\\(\\{]([^\\[\\(\\{]*)H[\\]\\)\\}]"};
    static const boost::regex matchElementSymbolOrAminoAcidLocant {L"[A-Z][a-z]?'*(\\d+[a-z]?'*)?"};
    static const boost::regex matchChalcogenReplacement {L"thio|seleno|telluro"};
    static const boost::regex matchGroupsThatAreAlsoInlineSuffixes {L"carbon|oxy|sulfen|sulfin|sulfon|selenen|selenin|selenon|telluren|tellurin|telluron"};
    static std::vector<std::wstring> const traditionalAlkanePositionNames;

    FunctionalReplacement *const functionalReplacement;
    SuffixApplier *const suffixApplier;
    BuildState *const state;

    //rings that look like HW rings but have other meanings. For the HW like inorganics the true meaning is given
    static const std::unordered_map<std::wstring, std::vector<std::wstring>> specialHWRings;
private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static ComponentProcessor::StaticConstructor staticConstructor;


public:
    virtual ~ComponentProcessor() {
        delete functionalReplacement;
        delete suffixApplier;
        delete state;
    }

    ComponentProcessor(BuildState *state, SuffixApplier *suffixApplier);

    /// <summary>
    /// Processes a parse result that has already gone through the ComponentGenerator.
    /// At this stage one can expect all substituents/roots to have at least 1 group.
    /// Multiple groups are present in, for example, fusion nomenclature. By the end of this function there will be exactly 1 group
    /// associated with each substituent/root. Multiplicative nomenclature can result in there being multiple roots </summary>
    /// <param name="parse"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    virtual void processParse(Element *parse) throw(ComponentGenerationException, StructureBuildingException);

    /// <summary>
    ///Resolves the contents of a group element
    /// </summary>
    /// <param name="group"> The group element </param>
    /// <returns> The fragment specified by the group element. </returns>
    /// <exception cref="StructureBuildingException"> If the group can't be built. </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    static Fragment *
    resolveGroup(BuildState *state, Element *group) throw(StructureBuildingException, ComponentGenerationException);

private:
    enum class AtomReferenceType {
        ID,
        DEFAULTID,
        LOCANT,
        DEFAULTLOCANT
    };

private:
    class AtomReference {
    private:
        const AtomReferenceType referenceType;
        const std::wstring reference;

    public:
        AtomReference(AtomReferenceType referenceType, const std::wstring &reference);
    };

private:
    class AddGroup {
    private:
        Fragment *const frag;
        AtomReference *atomReference;

    public:
        virtual ~AddGroup() {
            delete frag;
            delete atomReference;
        }

        AddGroup(Fragment *frag, AtomReference *atomReference);
    };

private:
    class AddHeteroatom {
    private:
        const std::wstring heteroAtomSmiles;
        AtomReference *atomReference;

    public:
        virtual ~AddHeteroatom() {
            delete atomReference;
        }

        AddHeteroatom(const std::wstring &heteroAtomSmiles, AtomReference *atomReference);
    };

private:
    class AddBond {
    private:
        const int bondOrder;
        AtomReference *atomReference;

    public:
        virtual ~AddBond() {
            delete atomReference;
        }

        AddBond(int bondOrder, AtomReference *atomReference);
    };


    /// <summary>
    /// Checks for groups with the addGroup/addBond/addHeteroAtom attributes. For the addGroup attribute adds the group defined by the SMILES described within
    /// e.g. for xylene  this function would add two methyls. Xylene is initially generated using the structure of benzene!
    /// See tokenList dtd for more information on the syntax of these attributes if it is not clear from the code </summary>
    /// <param name="state"> </param>
    /// <param name="group">: The group element </param>
    /// <param name="parentFrag">: The fragment that has been generated from the group element </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
private:
    static void processXyleneLikeNomenclature(BuildState *state, Element *group,
                                              Fragment *parentFrag) throw(StructureBuildingException, ComponentGenerationException);

    /// <summary>
    /// Checks that all locants are present within the front locants expected attribute of the group </summary>
    /// <param name="locantValues"> </param>
    /// <param name="group">
    /// @return </param>
    static bool locantAreAcceptableForXyleneLikeNomenclatures(std::vector<std::wstring> &locantValues, Element *group);


    /// <summary>
    /// Looks for the presence of <seealso cref="XmlDeclarations#DEFAULTINLOCANT_ATR"/> and <seealso cref="XmlDeclarations#DEFAULTINID_ATR"/> on the group and applies them to the fragment </summary>
    /// <param name="thisFrag"> </param>
    /// <param name="group"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void
    setFragmentDefaultInAtomIfSpecified(Fragment *thisFrag, Element *group) throw(StructureBuildingException);


    /// <summary>
    /// Looks for the presence of FUNCTIONALIDS_ATR on the group and applies them to the fragment </summary>
    /// <param name="group"> </param>
    /// <param name="thisFrag"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void
    setFragmentFunctionalAtomsIfSpecified(Element *group, Fragment *thisFrag) throw(StructureBuildingException);


    static void applyTraditionalAlkaneNumberingIfAppropriate(Element *group, Fragment *thisFrag);

    static void applyHomologyGroupLabelsIfSpecified(Element *group, Fragment *frag);

    void processChargeAndOxidationNumberSpecification(Element *group, Fragment *frag);

    /// <summary>
    /// Removes a substituent is just hydro/perhydro elements and moves its contents to be in front of the next in scope ring </summary>
    /// <param name="substituent"> </param>
    /// <returns> true if the substituent was a hydro substituent and hence was removed </returns>
    /// <exception cref="ComponentGenerationException"> </exception>
    bool removeAndMoveToAppropriateGroupIfHydroSubstituent(Element *substituent) throw(ComponentGenerationException);


    /// <summary>
    /// Removes substituents which are just a subtractivePrefix element e.g. deoxy and moves their contents to be in front of the next in scope biochemical fragment (or failing that group) </summary>
    /// <param name="substituent"> </param>
    /// <returns> true if the substituent was a subtractivePrefix substituent and hence was removed </returns>
    /// <exception cref="ComponentGenerationException"> </exception>
public:
    static bool
    removeAndMoveToAppropriateGroupIfSubtractivePrefix(Element *substituent) throw(ComponentGenerationException);

    /// <summary>
    /// Removes substituents which are just a fused ring element and moves their contents to be in front of the next in scope ring </summary>
    /// <param name="substituent"> </param>
    /// <returns> true if the substituent was a ring bridge and hence was removed </returns>
    /// <exception cref="ComponentGenerationException"> </exception>
private:
    bool removeAndMoveToAppropriateGroupIfRingBridge(Element *substituent) throw(ComponentGenerationException);

    bool containsCyclicAtoms(Element *potentialRing);

    /// <summary>
    /// Checks for agreement between the number of locants and multipliers.
    /// If a locant element contains multiple elements and is not next to a multiplier the various cases where this is the case will be checked for
    /// This may result in a locant being moved if it is more convenient for subsequent processing </summary>
    /// <param name="subOrBracketOrRoot"> The substituent/root/bracket to looks for locants in. </param>
    /// <param name="finalSubOrRootInWord"> : used to check if a locant is referring to the root as in multiplicative nomenclature </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void determineLocantMeaning(Element *subOrBracketOrRoot,
                                Element *finalSubOrRootInWord) throw(StructureBuildingException, ComponentGenerationException);


    /// <summary>
    ///Looks for Hantzch-Widman systems, and sees if the number of locants
    /// agrees with the number of heteroatoms.
    /// If this is not the case alternative possibilities are tested:
    /// 	The locants could be intended to indicate the position of outAtoms e.g. 1,4-phenylene
    /// 	The locants could be intended to indicate the attachement points of the root groups in multiplicative nomenclature e.g. 4,4'-methylenedibenzoic acid </summary>
    /// <param name="locant"> The element corresponding to the locant group to be tested </param>
    /// <param name="locantValues"> The locant values; </param>
    /// <param name="finalSubOrRootInWord"> : used to check if a locant is referring to the root as in multiplicative nomenclatures) </param>
    /// <returns> true if there's a HW system, and agreement; or if the locants conform to one of the alternative possibilities, otherwise false. </returns>
    /// <exception cref="ComponentGenerationException">  </exception>
    bool checkSpecialLocantUses(Element *locant, std::vector<std::wstring> &locantValues,
                                Element *finalSubOrRootInWord) throw(ComponentGenerationException);


    /// <summary>
    /// Detects multiplicative nomenclature. If it does then the locant will be moved, changed to a multiplicative locant and true will be returned </summary>
    /// <param name="locant"> </param>
    /// <param name="locantValues"> </param>
    /// <param name="finalSubOrRootInWord">
    /// @return </param>
    bool detectMultiplicativeNomenclature(Element *locant, std::vector<std::wstring> &locantValues,
                                          Element *finalSubOrRootInWord);

    void applyDLPrefixes(Element *subOrRoot) throw(ComponentGenerationException);

public:
    virtual bool applyDlStereochemistryToAminoAcid(Element *aminoAcidEl,
                                                   const std::wstring &dlStereochemistryValue) throw(ComponentGenerationException);

    virtual void applyDlStereochemistryToCarbohydrate(Element *carbohydrateEl,
                                                      const std::wstring &dlStereochemistryValue) throw(ComponentGenerationException);

    static void applyDlStereochemistryToCarbohydrateConfigurationalPrefix(Element *elementToApplyTo,
                                                                          const std::wstring &dlStereochemistryValue) throw(ComponentGenerationException);

    /// <summary>
    /// Cyclises carbohydrates and regularises their suffixes </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
private:
    void processCarbohydrates(Element *subOrRoot) throw(StructureBuildingException);

    void applyUnspecifiedRingSizeCyclisationIfPresent(Element *group,
                                                      Atom *potentialCarbonyl) throw(StructureBuildingException);

    /// <summary>
    /// Indicates that the compound is a ketose.
    /// This may take the form of replacement of the aldose functionality with ketose functionality or the additon of ketose functionality
    /// The carbonyl may be subsequently used in cyclisation e.g. non-2-ulopyranose
    /// A potentialcarbonyl is returned </summary>
    /// <param name="group"> </param>
    /// <param name="suffix"> </param>
    /// <param name="potentialCarbonyl"> </param>
    /// <returns> </returns>
    /// <exception cref="StructureBuildingException">  </exception>
    Atom *
    processUloseSuffix(Element *group, Element *suffix, Atom *potentialCarbonyl) throw(StructureBuildingException);

    /// <summary>
    /// Cyclises carbohydrate configuration prefixes according to the ring size indicator
    /// Alpha/beta stereochemistry is then applied if present </summary>
    /// <param name="carbohydrateGroup"> </param>
    /// <param name="ringSize"> </param>
    /// <param name="potentialCarbonyl"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void cycliseCarbohydrateAndApplyAlphaBetaStereo(Element *carbohydrateGroup, Element *ringSize,
                                                    Atom *potentialCarbonyl) throw(StructureBuildingException);

    void applyAlphaBetaStereoToCyclisedCarbohydrate(Element *carbohydrateGroup, Atom *carbonylCarbon);

    void processAldoseDiSuffix(const std::wstring &suffixValue, Element *group,
                               Atom *aldehydeAtom) throw(StructureBuildingException);

    /// <summary>
    /// Gets the configurationalAtom currently i.e. the defined stereocentre with the highest locant </summary>
    /// <param name="frag">
    /// @return </param>
    Atom *getAnomericReferenceAtom(Fragment *frag);

    void
    applyAnomerStereochemistryIfPresent(Element *alphaOrBetaLocantEl, Atom *anomericAtom, Atom *anomericReferenceAtom);

    std::vector<Atom *> getDeterministicAtomRefs4ForReferenceAtom(Atom *referenceAtom);

    std::vector<Atom *> getDeterministicAtomRefs4ForAnomericAtom(Atom *anomericAtom);

    /// <summary>
    /// Look for multipliers, and multiply out suffixes/unsaturators/heteroatoms/hydros.
    /// Locants are assigned if the number of locants matches the multiplier
    /// associated with them. Eg. triol - > ololol.
    /// Note that infix multiplication is handled seperately as resolution of suffixes is required to perform this unambiguously </summary>
    /// <param name="subOrRoot"> The substituent/root to looks for multipliers in. </param>
    void processMultipliers(Element *subOrRoot);


    /// <summary>
    /// Converts group elements that are identified as being conjunctive suffixes to CONJUNCTIVESUFFIXGROUP_EL
    /// and labels them appropriately. Any suffixes that the conjunctive suffix may have are resolved onto it </summary>
    /// <param name="subOrRoot"> </param>
    /// <param name="allGroups"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void detectConjunctiveSuffixGroups(Element *subOrRoot,
                                       std::vector<Element *> &allGroups) throw(ComponentGenerationException, StructureBuildingException);


    /// <summary>
    /// Match each locant to the next applicable "feature". Assumes that processLocants
    /// has done a good job and rejected cases where no match can be made.
    /// Handles cases where the locant is next to the feature it refers to
    /// </summary>
    /// <param name="subOrRoot"> The substituent/root to look for locants in. </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    void matchLocantsToDirectFeatures(Element *subOrRoot) throw(ComponentGenerationException);


    /// <summary>
    /// Looks for a suffix/suffix/heteroatom/hydro element adjacent to the given locant
    /// and if the locant element describes just 1 locant asssigns it </summary>
    /// <param name="locants"> </param>
    void assignSingleLocantsToAdjacentFeatures(std::vector<Element *> &locants);


    /// <summary>
    /// Handles suffixes, passes them to resolveGroupAddingSuffixes.
    /// Processes the suffixAppliesTo command which multiplies a suffix and attaches the suffixes to the atoms described by the given IDs </summary>
    /// <param name="group"> </param>
    /// <param name="suffixes"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void preliminaryProcessSuffixes(Element *group,
                                    std::vector<Element *> &suffixes) throw(ComponentGenerationException, StructureBuildingException);

    void applyDefaultLocantsToSuffixesIfApplicable(Element *group, Fragment *suffixableFragment);

    /// <summary>
    /// Processes the effects of the suffixAppliesTo attribute </summary>
    /// <param name="group"> </param>
    /// <param name="suffixes"> </param>
    /// <param name="suffixableFragment"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    void processSuffixAppliesTo(Element *group, std::vector<Element *> &suffixes,
                                Fragment *suffixableFragment) throw(ComponentGenerationException);


    /// <summary>
    ///Processes a suffix and returns any fragment the suffix intends to add to the molecule </summary>
    /// <param name="suffixes"> The suffix elements for a fragment. </param>
    /// <param name="frag"> The fragment to which the suffix will be applied </param>
    /// <returns> An arrayList containing the generated fragments </returns>
    /// <exception cref="StructureBuildingException"> If the suffixes can't be resolved properly. </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    std::vector<Fragment *> resolveGroupAddingSuffixes(std::vector<Element *> &suffixes,
                                                       Fragment *frag) throw(StructureBuildingException, ComponentGenerationException);

    /// <summary>
    ///Processes any convertHydroxyGroupsToOutAtoms and convertHydroxyGroupsToPositiveCharge instructions
    /// This is not handled as part of resolveGroupAddingSuffixes as something like carbonochloridoyl involves infix replacement
    /// on a hydroxy that would otherwise actually be removed by this rule! </summary>
    /// <param name="suffixes"> The suffix elements for a fragment. </param>
    /// <param name="frag"> The fragment to which the suffix will be applied </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    void processRemovalOfHydroxyGroupsRules(std::vector<Element *> &suffixes,
                                            Fragment *frag) throw(ComponentGenerationException);

    /// <summary>
    /// Finds all hydroxy groups connected to a given atom and adds a functionalAtom to each of them </summary>
    /// <param name="atom"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void addFunctionalAtomsToHydroxyGroups(Atom *atom) throw(StructureBuildingException);

    /// <summary>
    /// Finds all hydroxy groups connected to a given atom and makes them negatively charged </summary>
    /// <param name="atom"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void chargeHydroxyGroups(Atom *atom) throw(StructureBuildingException);

    /// <summary>
    /// Given a fragment removes all hydroxy groups and adds a valency 1 outAtom to the adjacent atom for each hydroxy group
    /// Note that O[OH] is not considered a hydroxy c.f. carbonoperoxoyl </summary>
    /// <param name="frag"> </param>
    void convertHydroxyGroupsToOutAtoms(Fragment *frag);

    /// <summary>
    /// Given a fragment removes all hydroxy groups and applies ylium to the adjacent atom (+1 charge -1 proton)
    /// Note that O[OH] is not considered a hydroxy </summary>
    /// <param name="frag"> </param>
    void convertHydroxyGroupsToPositiveCharge(Fragment *frag);

    /// <summary>
    /// Searches for suffix elements with the suffixPrefix attribute set
    /// A suffixPrefix is something like sulfon in sulfonamide. It would in this case take the value S(=O) </summary>
    /// <param name="suffixes"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void processSuffixPrefixes(std::vector<Element *> &suffixes) throw(StructureBuildingException);

    /// <summary>
    /// Checks through the groups accessible from the startingElement taking into account brackets (i.e. those that it is feasible that the group of the startingElement could substitute onto).
    /// It is assumed that one does not intentionally locant onto something in a deeper level of bracketing (not implicit bracketing). e.g. 2-propyl(ethyl)ammonia will give prop-2-yl </summary>
    /// <param name="state"> </param>
    /// <param name="startingElement"> </param>
    /// <param name="locant">: the locant string to check for the presence of </param>
    /// <returns> whether the locant was found </returns>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static bool checkLocantPresentOnPotentialRoot(BuildState *state, Element *startingElement,
                                                  const std::wstring &locant) throw(StructureBuildingException);

    /// <summary>
    /// Handles special cases in IUPAC nomenclature that are most elegantly solved by modification of the fragment
    ///  Also sets the default in atom for alkanes so that say methylethyl is prop-2-yl rather than propyl </summary>
    /// <param name="groups"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException">  </exception>
private:
    void handleGroupIrregularities(
            std::vector<Element *> &groups) throw(StructureBuildingException, ComponentGenerationException);


    /// <summary>
    /// Handles Hantzsch-Widman rings. Adds SMILES to the group corresponding to the ring's structure </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    void processHW(Element *subOrRoot) throw(StructureBuildingException, ComponentGenerationException);


    /// <summary>
    /// Assigns Element symbols to groups, suffixes and conjunctive suffixes.
    /// Suffixes have preference. </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void assignElementSymbolLocants(Element *subOrRoot) throw(StructureBuildingException);


    /// <summary>
    /// Processes constructs such as biphenyl, 1,1':4',1''-Terphenyl, 2,2'-Bipyridylium, m-Quaterphenyl </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void processRingAssemblies(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException);

    /// <summary>
    /// Given the element after the ring assembly multiplier determines which siblings should be resolved by adding them to elementToResolve </summary>
    /// <param name="multiplier"> </param>
    /// <param name="ringJoiningLocants"> </param>
    /// <param name="outAtomCount">
    /// @return </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    Element *determineElementsToResolveIntoRingAssembly(Element *multiplier, int ringJoiningLocants,
                                                        int outAtomCount) throw(ComponentGenerationException);


    /// <summary>
    /// Proccess any polycyclic spiro systems present in subOrRoot
    /// It is assumed that at this stage all hantzch widman rings/fused rings have been resolved to single groups allowing them to be simply spiro fused
    ///
    /// http://www.chem.qmul.ac.uk/iupac/spiro/ (SP-2 through SP-6) </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void processPolyCyclicSpiroNomenclature(
            Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException);

    void processNonIdenticalPolyCyclicSpiro(
            Element *polyCyclicSpiroDescriptor) throw(ComponentGenerationException, StructureBuildingException);


    /// <summary>
    /// Processes spiro systems described using the now deprectated method described in the 1979 guidelines Rule A-42 </summary>
    /// <param name="spiroElements"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void processOldMethodPolyCyclicSpiro(
            std::vector<Element *> &spiroElements) throw(ComponentGenerationException, StructureBuildingException);


    /// <summary>
    /// Two or three copies of the fragment after polyCyclicSpiroDescriptor are spiro fused at one centre </summary>
    /// <param name="polyCyclicSpiroDescriptor"> </param>
    /// <param name="components"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void processSpiroBiOrTer(Element *polyCyclicSpiroDescriptor,
                             int components) throw(ComponentGenerationException, StructureBuildingException);

    /// <summary>
    /// Three copies of the fragment after polyCyclicSpiroDescriptor are spiro fused at two centres </summary>
    /// <param name="polyCyclicSpiroDescriptor"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    void processDispiroter(
            Element *polyCyclicSpiroDescriptor) throw(StructureBuildingException, ComponentGenerationException);

    /// <summary>
    /// The features between the polyCyclicSpiroDescriptor and the first group element, or beween the STRUCTURALOPENBRACKET_EL and STRUCTURALCLOSEBRACKET_EL
    /// are found and then passed to resolveFeaturesOntoGroup </summary>
    /// <param name="polyCyclicSpiroDescriptor"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException">  </exception>
    void determineFeaturesToResolveInSingleComponentSpiro(
            Element *polyCyclicSpiroDescriptor) throw(StructureBuildingException, ComponentGenerationException);

    /// <summary>
    /// Given some elements including a group element resolves all locanted and unlocanted features.
    /// If suffixes are present these are resolved and detached </summary>
    /// <param name="elementsToResolve"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException">  </exception>
    void resolveFeaturesOntoGroup(
            std::vector<Element *> &elementsToResolve) throw(StructureBuildingException, ComponentGenerationException);

private:
    class SortBridgesByHighestLocantedBridgehead : public Comparator<Fragment *> {

    private:
        const std::unordered_map<Fragment *, std::vector<Atom *>> bridgeToRingAtoms;
    public:
        SortBridgesByHighestLocantedBridgehead(std::unordered_map<Fragment *, std::vector<Atom *>> &bridgeToRingAtoms);

        virtual int compare(Fragment *bridge1, Fragment *bridge2);

    };

    /// <summary>
    /// Processes bridges e.g. 4,7-methanoindene
    /// Resolves and attaches said bridges to the adjacent ring fragment
    /// Numbers the bridges in accordance with FR-8.6/FR-8.7 </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
private:
    void processFusedRingBridges(Element *subOrRoot) throw(StructureBuildingException);

    static int getLocantNumber(Atom *atom);

    int getHighestNumericLocant(Fragment *ringFrag);

    /// <summary>
    /// Searches for lambdaConvention elements and applies the valency they specify to the atom
    /// they specify on the substituent/root's fragment </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void applyLambdaConvention(Element *subOrRoot) throw(StructureBuildingException);


    /// <summary>
    /// Uses the number of outAtoms that are present to assign the number of outAtoms on substituents that can have a variable number of outAtoms
    /// Hence at this point it can be determined if a multi radical susbtituent is present in the name
    /// This would be expected in multiplicative nomenclature and is noted in the state so that the StructureBuilder knows to resolve the
    /// section of the name from that point onwards in a left to right manner rather than right to left </summary>
    /// <param name="subOrRoot">: The sub/root to look in </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void handleMultiRadicals(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException);

    /// <summary>
    /// Checks for cases where multiplier(methylene) or multiplier(thio) and the like should not be interpreted as one fragment
    /// Something like nitrilotrithiotriacetic acid or oxetane-3,3-diyldimethylene </summary>
    /// <param name="group"> </param>
    /// <param name="multiplierBeforeGroup">
    /// @return </param>
    bool unsuitableForFormingChainMultiradical(Element *group, Element *multiplierBeforeGroup);


    /// <summary>
    /// Calculates number of OutAtoms that the resolveSuffixes method will add. </summary>
    /// <param name="group"> </param>
    /// <param name="suffixes"> </param>
    /// <returns> numberOfOutAtoms that will be added by resolveSuffixes </returns>
    /// <exception cref="ComponentGenerationException"> </exception>
    int calculateOutAtomsToBeAddedFromInlineSuffixes(Element *group,
                                                     std::vector<Element *> &suffixes) throw(ComponentGenerationException);

    /// <summary>
    /// Corrects something like L-alanyl-L-glutaminyl-L-arginyl-O-phosphono-L-seryl-L-alanyl-L-proline to:
    /// ((((L-alanyl-L-glutaminyl)-L-arginyl)-O-phosphono-L-seryl)-L-alanyl)-L-proline
    /// i.e. substituents go onto the last mentioned amino acid; amino acids chain together to form peptides </summary>
    /// <param name="groups"> </param>
    /// <param name="brackets"> </param>
    void addImplicitBracketsToAminoAcids(std::vector<Element *> &groups, std::vector<Element *> &brackets);

    /// <summary>
    /// Looks for whether this substituent should be bracketed to the substituent before it
    /// E.g. dimethylaminobenzene -> (dimethylamino)benzene when the substituent is the amino
    /// The list of brackets is modified if the method does something </summary>
    /// <param name="substituent"> </param>
    /// <param name="brackets"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    void implicitlyBracketToPreviousSubstituentIfAppropriate(Element *substituent,
                                                             std::vector<Element *> &brackets) throw(ComponentGenerationException, StructureBuildingException);

    bool
    implicitBracketWouldPreventAdditiveBonding(Element *elementBeforeSubstituent, Element *elementAftersubstituent);

    /// <summary>
    /// Retrusn true in the case that:
    /// the given substituent is a direct child of a word element
    /// The preceding substituent/bracket is the first element in the word element
    /// The current word rule invovles locanted ester like linkages </summary>
    /// <param name="substituent"> </param>
    /// <param name="elementBeforeSubstituent">
    /// @return </param>
    bool locantedEsterImplicitBracketSpecialCase(Element *substituent, Element *elementBeforeSubstituent);

    /// <summary>
    /// Attempts to match locants to non adjacent suffixes/unsatuators
    /// e.g.  2-propanol, 3-furyl, 2'-Butyronaphthone </summary>
    /// <param name="subOrRoot"> The substituent/root to look for locants in. </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void matchLocantsToIndirectFeatures(Element *subOrRoot) throw(StructureBuildingException);


    /// <summary>
    /// Finds locants that are before a group element and not immediately followed by a multiplier </summary>
    /// <param name="subOrRoot">
    /// @return </param>
    std::vector<Element *> findLocantsThatCouldBeIndirectLocants(Element *subOrRoot);


    /// <summary>
    /// Find elements that can have indirect locants but don't currently
    /// This requirement excludes hydro and heteroatoms as it is assumed that locants for these are always adjacent (or handled by the special HW code in the case of heteroatoms) </summary>
    /// <param name="subOrRoot"> The subOrRoot of interest </param>
    /// <param name="locantEl"> the locant, only elements after it will be considered </param>
    /// <returns> An arrayList of locantable elements </returns>
    std::vector<Element *> findElementsMissingIndirectLocants(Element *subOrRoot, Element *locantEl);


    /// <summary>
    /// Put di-carbon modifying suffixes e.g. oic acids, aldehydes on opposite ends of chain </summary>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void assignImplicitLocantsToDiTerminalSuffixes(Element *subOrRoot) throw(StructureBuildingException);


    /// <summary>
    /// Checks whether a suffix element is:
    /// a suffix, an inline suffix OR terminal root suffix, has no current locant </summary>
    /// <param name="suffix">
    /// @return </param>
    bool isATerminalSuffix(Element *suffix);

    void
    processConjunctiveNomenclature(Element *subOrRoot) throw(ComponentGenerationException, StructureBuildingException);

    /// <summary>
    /// Converts a biochemical linkage description e.g. (1->4) into an O[1-9] locant
    /// If the carbohydrate is preceded by substituents these are placed into a bracket and the bracket locanted </summary>
    /// <param name="substituents"> </param>
    /// <param name="brackets"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    void processBiochemicalLinkageDescriptors(std::vector<Element *> &substituents,
                                              std::vector<Element *> &brackets) throw(StructureBuildingException);

    bool isSubBracketOrRoot(Element *element);

    void checkAndApplyFirstLocantOfBiochemicalLinkage(Element *substituent,
                                                      const std::wstring &biochemicalLinkage) throw(StructureBuildingException);

    /// <summary>
    /// Moves a multiplier out of a bracket if the bracket contains only one substituent
    /// e.g. (trimethyl) --> tri(methyl).
    /// The multiplier may have locants e.g. [N,N-bis(2-hydroxyethyl)]
    /// This is done because OPSIN has no idea what to do with (trimethyl) as there is nothing within the scope to substitute onto! </summary>
    /// <param name="brackets"> </param>
    void moveErroneouslyPositionedLocantsAndMultipliers(std::vector<Element *> &brackets);


    /// <summary>
    /// Checks for case where the term is a substituent that starts with two multipliers
    /// Interprets the first as a word level multiplier and the second as a substituent multiplier by adding an implicit bracket </summary>
    /// <param name="substituent"> </param>
    /// <param name="brackets"> </param>
    void
    addImplicitBracketsWhenFirstSubstituentHasTwoMultipliers(Element *substituent, std::vector<Element *> &brackets);


    /// <summary>
    /// Given the right most child of a word:
    /// Checks whether this is multiplied e.g. methylenedibenzene
    /// If it is then it checks for the presence of locants e.g. 4,4'-oxydibenzene which has been changed to oxy-4,4'-dibenzene
    /// An attribute called inLocants is then added that is either INLOCANTS_DEFAULT or a comma seperated list of locants </summary>
    /// <param name="rightMostElement"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException"> </exception>
    void assignLocantsToMultipliedRootIfPresent(
            Element *rightMostElement) throw(ComponentGenerationException, StructureBuildingException);


    /// <summary>
    /// Adds an implicit bracket in the case where two locants have been given.
    /// One for the locanting of substituent on to the next substituent and one
    /// for the locanting of this combined substituent onto a parent group
    /// e.g. 5-p-hydroxyphenyl-1,2-dithiole-3-thione --> e.g. 5-(p-hydroxyphenyl)-1,2-dithiole-3-thione </summary>
    /// <param name="substituent"> </param>
    /// <param name="brackets"> </param>
    void addImplicitBracketsWhenSubstituentHasTwoLocants(Element *substituent, std::vector<Element *> &brackets);

    /// <summary>
    /// Retrieves the first elements of a substituent which are locants skipping over stereochemistry elements </summary>
    /// <param name="substituent">
    /// @return </param>
    std::vector<Element *> getLocantsAtStartOfSubstituent(Element *substituent);

    /// <summary>
    /// Checks that none of the locants contain commas </summary>
    /// <param name="locants">
    /// @return </param>
    bool locantsAreSingular(std::vector<Element *> &locants);

    /// <summary>
    /// Assigns locants and multipliers to substituents/brackets
    /// If both locants and multipliers are present a final check is done that the number of them agree.
    /// WordLevel multipliers are processed e.g. diethyl ethanoate
    /// Adding a locant to a root or any other group that cannot engage in substitive nomenclature will result in an exception being thrown
    /// An exception is made for cases where the locant could be referring to a position on another word </summary>
    /// <param name="subOrBracket"> </param>
    /// <exception cref="ComponentGenerationException"> </exception>
    /// <exception cref="StructureBuildingException">  </exception>
    void
    assignLocantsAndMultipliers(Element *subOrBracket) throw(ComponentGenerationException, StructureBuildingException);

    std::wstring locantsToDebugString(std::vector<Element *> &locants);


    bool wordLevelLocantsAllowed(Element *subBracketOrRoot, int numberOflocants);

    /// <summary>
    /// If a word level multiplier is present e.g. diethyl butandioate then this is processed to ethyl ethyl butandioate
    /// If wordCount is 1 then an exception is thrown if a multiplier is encountered e.g. triphosgene parsed as tri phosgene </summary>
    /// <param name="word"> </param>
    /// <param name="roots"> </param>
    /// <param name="wordCount"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    void processWordLevelMultiplierIfApplicable(Element *word, std::vector<Element *> &roots,
                                                int wordCount) throw(StructureBuildingException, ComponentGenerationException);

    void checkForNonConfusedWithNona(Element *multiplier) throw(StructureBuildingException);

    /// <summary>
    /// Names like monooxygen may be used to emphasise that a molecule is not being described </summary>
    /// <param name="multiplier"> </param>
    /// <param name="multiVal">
    /// @return </param>
    bool isMonoFollowedByElement(Element *multiplier, int multiVal);
};

