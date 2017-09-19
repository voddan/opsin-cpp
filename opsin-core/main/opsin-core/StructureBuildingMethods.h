#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>
#include "stringhelper.h"
#include <set>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class Element;
class StructureBuildingException;
class Fragment;
class Atom;
class Bond;
class OutAtom;
class BuildResults;


using org::apache::log4j::Logger;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

/// <summary>
/// Methods for processing the substitutive and additive operations that connect all the fragments together
/// as well as indicated hydrogen/unsaturation/heteroatom replacement
/// @author dl387
///
/// </summary>
class StructureBuildingMethods {
private:
    static Logger *const LOG = Logger::getLogger(StructureBuildingMethods::
    typeid);
    static const boost::regex matchCompoundLocant {L"[\\[\\(\\{](\\d+[a-z]?'*)[\\]\\)\\}]"};

    StructureBuildingMethods();

    /// <summary>
    /// Resolves a word/bracket:
    /// Locanted attributes of words are resolved onto their group
    /// Locanted substitution is performed
    /// Connections involving multi radicals are processed
    /// Unlocanted attributes of words are resolved onto their group
    ///
    /// If word is a wordRule the function will instantly return
    /// </summary>
    /// <param name="state"> </param>
    /// <param name="word"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static void resolveWordOrBracket(BuildState *state, Element *word) throw(StructureBuildingException);

    /// <summary>
    /// Performs locanted attribute resolution
    /// then additive joining of fragments
    /// then locanted substitutive joining of fragments
    /// </summary>
    /// <param name="state"> </param>
    /// <param name="word"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void recursivelyResolveLocantedFeatures(BuildState *state, Element *word) throw(StructureBuildingException);

    /// <summary>
    /// Performs locanted attribute resolution
    /// then additive joining of fragments
    /// then locanted substitutive joining of fragments
    /// </summary>
    /// <param name="state"> </param>
    /// <param name="word"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void
    recursivelyResolveUnLocantedFeatures(BuildState *state, Element *word) throw(StructureBuildingException);

    static void
    resolveRootOrSubstituentLocanted(BuildState *state, Element *subOrRoot) throw(StructureBuildingException);

    static void
    resolveRootOrSubstituentUnLocanted(BuildState *state, Element *subOrRoot) throw(StructureBuildingException);


private:
    static void performLocantedSubstitutiveOperations(BuildState *state,
                                                      Element *subBracketOrRoot) throw(StructureBuildingException);

    static void performUnLocantedSubstitutiveOperations(BuildState *state,
                                                        Element *subBracketOrRoot) throw(StructureBuildingException);

    /// <summary>
    /// Clones the perhalogenFrag sufficiently to replace all in scope hydrogen with halogens.
    /// The cloned fragments are merged into the perhalogenFrag </summary>
    /// <param name="state"> </param>
    /// <param name="perhalogenFrag"> </param>
    /// <param name="subBracketOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void performPerHalogenoSubstitution(BuildState *state, Fragment *perhalogenFrag,
                                               Element *subBracketOrRoot) throw(StructureBuildingException);

    /// <summary>
    /// Multiplies out groups/brakets and substitutes them. The attribute "locant" is checked for locants
    /// If it is present it should contain a comma separated list of locants
    /// The strategy employed is to clone subOrBracket and its associated fragments as many times as the multiplier attribute
    /// perform(Un)LocantedSubstitutiveOperations is then called with on each call a different clone (or the original element) being in position
    /// Hence bonding between the clones is impossible </summary>
    /// <param name="state"> </param>
    /// <param name="subOrBracket"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void multiplyOutAndSubstitute(BuildState *state, Element *subOrBracket) throw(StructureBuildingException);

    static void performUnlocantedSubstitutiveOperations(BuildState *state,
                                                        std::vector<Element *> &multipliedElements) throw(StructureBuildingException);

    /// <summary>
    /// Adds locanted unsaturators, heteroatoms and hydrogen elements to the group within the sub or root </summary>
    /// <param name="state"> </param>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static void resolveLocantedFeatures(BuildState *state, Element *subOrRoot) throw(StructureBuildingException);

    /// <summary>
    /// ACD/Name has a known bug where it produces names in which a suffixed saturated ring in a polycyclic spiro
    /// is treated as if it is unsaturated and hence has indicated hydrogens
    /// e.g. 1',3'-dihydro-2H,5H-spiro[imidazolidine-4,2'-indene]-2,5-dione </summary>
    /// <param name="group"> </param>
    /// <param name="indicatedHydrogenLocant">
    /// @return </param>
private:
    static bool acdNameSpiroIndicatedHydrogenBug(Element *group, const std::wstring &indicatedHydrogenLocant);

    /// <summary>
    /// Removes a terminal atom of a particular element e.g. oxygen
    /// The locant specifies the atom adjacent to the atom to be removed
    /// Formally the atom is replaced by hydrogen, hence stereochemistry is intentionally preserved </summary>
    /// <param name="state"> </param>
    /// <param name="fragment"> </param>
    /// <param name="chemEl"> </param>
    /// <param name="locant"> A locant or null </param>
    /// <exception cref="StructureBuildingException">  </exception>
public:
    static void applySubtractivePrefix(BuildState *state, Fragment *fragment, ChemEl chemEl,
                                       const std::wstring &locant) throw(StructureBuildingException);

    /// <summary>
    /// Removes terminal atoms of a particular element e.g. oxygen
    /// The number to remove is decided by the count
    /// Formally the atom is replaced by hydrogen, hence stereochemistry is intentionally preserved </summary>
    /// <param name="state"> </param>
    /// <param name="fragment"> </param>
    /// <param name="chemEl"> </param>
    /// <param name="count"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void applyUnlocantedSubtractivePrefixes(BuildState *state, Fragment *fragment, ChemEl chemEl,
                                                   int count) throw(StructureBuildingException);

private:
    static void
    applyAnhydroPrefix(BuildState *state, Fragment *frag, Element *subtractivePrefix) throw(StructureBuildingException);

    /// <summary>
    /// Attempts to form triple bond between the atoms in atomsToFormTripleBondsBetween
    /// Throws an exception if the list contains duplicates or atoms with no adjacent atom in the list </summary>
    /// <param name="atomsToFormTripleBondsBetween"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void
    addDehydroInducedTripleBonds(std::vector<Atom *> &atomsToFormTripleBondsBetween) throw(StructureBuildingException);

    /// <summary>
    /// Adds locanted unsaturators, heteroatoms and hydrogen elements to the group within the sub or root </summary>
    /// <param name="state"> </param>
    /// <param name="subOrRoot"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static void resolveUnLocantedFeatures(BuildState *state, Element *subOrRoot) throw(StructureBuildingException);

private:
    static void applyUnlocantedHydro(BuildState *state, Fragment *frag,
                                     std::vector<Element *> &hydrogenElements) throw(StructureBuildingException);

    static void unsaturateBonds(BuildState *state, Fragment *frag,
                                std::vector<int> &unsaturationBondOrders) throw(StructureBuildingException);

    static void unsaturateBonds(BuildState *state, Fragment *frag, int bondOrder,
                                int numToUnsaturate) throw(StructureBuildingException);

    static bool
    isCycloAlkaneSpecialCase(Fragment *frag, int numToUnsaturate, std::vector<Bond *> &bondsThatCouldBeUnsaturated);

    static bool isCycloAlkaneHeteroatomSpecialCase(Fragment *frag, int numHeteroatoms,
                                                   std::vector<Atom *> &atomsThatCouldBeReplaced);

private:
    class HeteroAtomSmilesAndLambda {
    private:
        const std::wstring smiles;
        const std::wstring lambdaConvention;

    public:
        HeteroAtomSmilesAndLambda(const std::wstring &smiles, const std::wstring &lambdaConvention);

        virtual int hashCode() override;

        virtual bool equals(void *obj) override;


    };

private:
    static void applyUnlocantedHeteroatoms(BuildState *state, Fragment *frag,
                                           std::vector<Element *> &heteroatoms) throw(StructureBuildingException);

    static void
    applyIsotopeSpecifications(BuildState *state, Fragment *frag, std::vector<Element *> &isotopeSpecifications,
                               bool applyLocanted) throw(StructureBuildingException);

    static bool casIsotopeAmbiguitySpecialCase(Fragment *frag, std::vector<Atom *> &parentAtomsToApplyTo,
                                               int multiplier) throw(StructureBuildingException);

public:
    static Atom *
    findAtomForUnlocantedRadical(BuildState *state, Fragment *frag, OutAtom *outAtom) throw(StructureBuildingException);


private:
    static std::vector<Bond *>
    findAlternativeBondsToUnSaturate(Fragment *frag, int bondOrder, std::vector<Bond *> *bondsToIgnore);

    /// <summary>
    /// Finds bond within the fragment that can have their bondOrder increased to the specified bond order
    /// Depending on the value of allowAdjacentUnsaturatedBonds adjacent higher bonds are prevented </summary>
    /// <param name="frag"> </param>
    /// <param name="bondOrder"> </param>
    /// <param name="allowAdjacentUnsaturatedBonds">
    /// @return </param>
public:
    static std::vector<Bond *> findBondsToUnSaturate(Fragment *frag, int bondOrder, bool allowAdjacentUnsaturatedBonds);

private:
    static std::vector<Bond *> findBondsToUnSaturate(Fragment *frag, int bondOrder, bool allowAdjacentUnsaturatedBonds,
                                                     std::set<Bond *> *bondsToIgnore);


    /// <summary>
    /// Return the lambda convention derived valency + protons if set
    /// Otherwise if charge is 0 returns <seealso cref="ValencyChecker#getHWValency(ChemEl)"/>
    /// Otherwise return <seealso cref="ValencyChecker#getMaximumValency(ChemEl, int)"/>
    /// Returns null if the maximum valency is not known </summary>
    /// <param name="a">
    /// @return </param>
public:
    static boost::optional<int> getLambdaValencyOrHwValencyOrMaxValIfCharged(Atom *a);

private:
    static void
    performAdditiveOperations(BuildState *state, Element *subBracketOrRoot) throw(StructureBuildingException);

    /// <summary>
    /// Searches the input for something that either is a multiRadical or has no outAtoms i.e. not dimethyl </summary>
    /// <param name="subBracketOrRoot">
    /// @return </param>
    static bool hasRootLikeOrMultiRadicalGroup(Element *subBracketOrRoot);

    /// <summary>
    /// Multiply out subOrBracket and additively bond all substituents to the specified fragment </summary>
    /// <param name="state"> </param>
    /// <param name="subOrBracket"> </param>
    /// <param name="fragToAdditivelyBondTo"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void multiplyOutAndAdditivelyBond(BuildState *state, Element *subOrBracket,
                                             Fragment *fragToAdditivelyBondTo) throw(StructureBuildingException);

    /// <summary>
    /// Creates a build results from the input group for use as the input to the real performMultiplicativeOperations function </summary>
    /// <param name="state"> </param>
    /// <param name="group"> </param>
    /// <param name="multipliedParent"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static void performMultiplicativeOperations(BuildState *state, Element *group,
                                                Element *multipliedParent) throw(StructureBuildingException);

    static void performMultiplicativeOperations(BuildState *state, BuildResults *multiRadicalBR,
                                                Element *multipliedParent) throw(StructureBuildingException);

    /// <summary>
    /// Applies special case to prefer the end of chains with the usableAsAJoiner attributes cf. p-phenylenedipropionic acid
    /// Such cases will still be considered to be formally ambiguous </summary>
    /// <param name="multipliedGroup"> </param>
    /// <param name="multipliedFrag"> </param>
    /// <param name="bondOrder"> </param>
    /// <param name="primesAdded">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static std::vector<Atom *>
    getPossibleAtomsForUnlocantedConnectionToMultipliedRoot(Element *multipliedGroup, int bondOrder,
                                                            int primesAdded) throw(StructureBuildingException);

    static std::wstring getLocantOfEndOfChainIfGreaterThan1(Fragment *frag, int primes);

    /// <summary>
    /// Given a subsituent/bracket finds the next multi valent substituent/root that is in scope and hence its group
    /// e.g. for oxy(dichloromethyl)methylene given oxy substituent the methylene group would be found
    /// for oxy(dichloroethylene) given oxy substituent the ethylene group would be found
    /// for oxy(carbonylimino) given oxy carbonyl would be found </summary>
    /// <param name="substituentOrBracket"> </param>
    /// <returns> frag </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    static Fragment *getNextInScopeMultiValentFragment(Element *substituentOrBracket) throw(StructureBuildingException);

    /// <summary>
    /// Given a bracket searches in a depth first manner for the first multi valent group </summary>
    /// <param name="bracket"> </param>
    /// <returns> group </returns>
    /// <exception cref="StructureBuildingException"> </exception>
    static Element *getFirstMultiValentGroup(Element *bracket) throw(StructureBuildingException);

    static void joinFragmentsAdditively(BuildState *state, Fragment *fragToBeJoined,
                                        Fragment *parentFrag) throw(StructureBuildingException);

    static void joinFragmentsSubstitutively(BuildState *state, Fragment *fragToBeJoined,
                                            Atom *atomToJoinTo) throw(StructureBuildingException);

    /// <summary>
    /// Forms a bridge using the given fragment.
    /// The bridgingFragment's outAtoms locants or a combination of the atomToJoinTo and a suitable atom
    /// are used to decide what atoms to form the bridge between </summary>
    /// <param name="state"> </param>
    /// <param name="bridgingFragment"> </param>
    /// <param name="atomToJoinTo"> </param>
    /// <returns> Atoms that the bridgingFragment attached to </returns>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static std::vector<Atom *>
    formEpoxide(BuildState *state, Fragment *bridgingFragment, Atom *atomToJoinTo) throw(StructureBuildingException);

    /// <summary>
    /// Attempts to find an in-scope fragment capable of forming the given numberOfSubstitutions each with the given bondOrder </summary>
    /// <param name="subOrBracket"> </param>
    /// <param name="numberOfSubstitutions"> </param>
    /// <param name="bondOrder">
    /// @return </param>
private:
    static std::vector<Atom *>
    findAtomsForSubstitution(Element *subOrBracket, int numberOfSubstitutions, int bondOrder);

    /// <summary>
    /// Finds all the fragments accessible from the startingElement taking into account brackets
    /// i.e. those that it is feasible that the group of the startingElement could substitute onto </summary>
    /// <param name="startingElement"> </param>
    /// <returns> A list of fragments in the order to try them as possible parent fragments (for substitutive operations) </returns>
public:
    static std::vector<Fragment *> findAlternativeFragments(Element *startingElement);

    /// <summary>
    /// Finds all the groups accessible from the startingElement taking into account brackets
    /// i.e. those that it is feasible that the group of the startingElement could substitute onto </summary>
    /// <param name="startingElement"> </param>
    /// <returns> A list of groups in the order to try them as possible parent groups (for substitutive operations) </returns>
    static std::vector<Element *> findAlternativeGroups(Element *startingElement);

    /// <summary>
    /// Checks through the groups accessible from the currentElement taking into account brackets
    /// i.e. those that it is feasible that the group of the currentElement could substitute onto </summary>
    /// <param name="startingElement"> </param>
    /// <param name="locant">: the locant string to check for the presence of </param>
    /// <returns> The fragment with the locant, or null </returns>
    /// <exception cref="StructureBuildingException"> </exception>
private:
    static Fragment *
    findFragmentWithLocant(Element *startingElement, const std::wstring &locant) throw(StructureBuildingException);

public:
    static Element *findRightMostGroupInBracket(Element *bracket);

    static Element *findRightMostGroupInSubBracketOrRoot(Element *subBracketOrRoot);

private:
    static Element *findRightMostGroupInSubOrRoot(Element *subOrRoot);

    static bool potentiallyCanSubstitute(Element *subBracketOrRoot);

public:
    static std::wstring
    checkForBracketedPrimedLocantSpecialCase(Element *subBracketOrRoot, const std::wstring &locantString);

    /// <summary>
    /// In cases such as methylenecyclohexane two outAtoms are combined to form a single outAtom with valency
    /// equal to sum of the valency of the other outAtoms.
    /// This is only allowed on substituents where all the outAtoms are on the same atom </summary>
    /// <param name="frag"> </param>
    /// <param name="group"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
private:
    static void checkAndApplySpecialCaseWhereOutAtomsCanBeCombinedOrThrow(Fragment *frag,
                                                                          Element *group) throw(StructureBuildingException);

    /// <summary>
    /// Calculates the number of substitutable hydrogen by taking into account:
    /// Specified valency if applicable, outAtoms and the lowest valency state that will satisfy these
    /// e.g. thio has 2 outAtoms and no bonds hence -->2 outgoing, lowest stable valency = 2 hence no substitutable hydrogen
    /// e.g. phosphonyl has 2 outAtoms and one double bond -->4 outgoing, lowest stable valency =5 hence 1 substitutable hydrogen </summary>
    /// <param name="atom">
    /// @return </param>
public:
    static int calculateSubstitutableHydrogenAtoms(Atom *atom);

    /// <summary>
    /// Stereochemistry terms are assigned right at the end so that checks can be done on whether the indicated atom is in fact chiral.
    /// In the process of multiplication locants are primed. This function adds the appropriate number of primes to any locanted stereochemistry locants
    /// The primesString is the string containing the primes to add to each locant </summary>
    /// <param name="subOrBracket"> </param>
    /// <param name="primesString"> </param>
private:
    static void addPrimesToLocantedStereochemistryElements(Element *subOrBracket, const std::wstring &primesString);

    /// <summary>
    /// Calculates the number of times getParent() must be called to reach a word element
    /// Returns null if element does not have an enclosing word element. </summary>
    /// <param name="element">
    /// @return </param>
    static boost::optional<int> levelsToWordEl(Element *element);
};

