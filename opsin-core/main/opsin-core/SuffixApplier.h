#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <boost/optional.hpp>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class SuffixRules;
class ComponentGenerationException;
class Element;
class StructureBuildingException;
class Fragment;
class Atom;
class SuffixRule;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;




class SuffixApplier {

private:
    BuildState *const state;
    SuffixRules *const suffixRules;

public:
    virtual ~SuffixApplier() {
        delete state;
        delete suffixRules;
    }

    SuffixApplier(BuildState *state, SuffixRules *suffixRules);

    /// <summary>
    /// Does suffixApplicability.xml have an entry for this group type? </summary>
    /// <param name="groupType">
    /// @return </param>
    virtual bool isGroupTypeWithSpecificSuffixRules(const std::wstring &groupType);


    /// <summary>
    ///Process the effects of suffixes upon a fragment.
    /// Unlocanted non-terminal suffixes are not attached yet. All other suffix effects are performed </summary>
    /// <param name="group"> The group element for the fragment to which the suffixes will be added </param>
    /// <param name="suffixes"> The suffix elements for a fragment. </param>
    /// <exception cref="StructureBuildingException"> If the suffixes can't be resolved properly. </exception>
    /// <exception cref="ComponentGenerationException"> </exception>
    virtual void resolveSuffixes(Element *group,
                                 std::vector<Element *> &suffixes) throw(StructureBuildingException, ComponentGenerationException);

private:
    void applyIsotopeToSuffix(Fragment *frag, Element *isotopeSpecification,
                              bool mustBeApplied) throw(StructureBuildingException);


    /// <summary>
    /// Return the subset of atoms that are "pro-ketone"
    /// i.e. a [CD2](C)C </summary>
    /// <param name="atoms">
    /// @return </param>
    std::vector<Atom *> getProKetonePositions(std::vector<Atom *> &atoms);

    void processCycleFormingSuffix(Fragment *suffixFrag, Fragment *suffixableFragment,
                                   Element *suffix) throw(StructureBuildingException, ComponentGenerationException);

    Atom *getFragAtomToUse(Fragment *frag, Element *suffix,
                           const std::wstring &suffixTypeToUse) throw(StructureBuildingException);

    /// <summary>
    /// Preference is given to mono cation/anions as they are expected to be more likely
    /// Additionally, Typically if a locant has not been specified then it was intended to refer to a nitrogen even if the nitrogen is not at locant 1 e.g. isoquinolinium
    /// Hence preference is given to nitrogen atoms and then to non carbon atoms </summary>
    /// <param name="atomList"> </param>
    /// <param name="chargeChange"> </param>
    /// <param name="protonChange"> </param>
    void applyUnlocantedChargeModification(std::vector<Atom *> &atomList, int chargeChange, int protonChange);


    /// <summary>
    /// e.g. if element is "S" changes C(=S)O -->C(=O)S </summary>
    /// <param name="frag"> </param>
    /// <param name="chemEl"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void swapElementsSuchThatThisElementIsAcidic(Fragment *frag, ChemEl chemEl) throw(StructureBuildingException);

    /// <summary>
    /// Creates bonds between the parentAtom and the atoms connected to the R atoms.
    /// Removes bonds to the R atom </summary>
    /// <param name="parentAtom"> </param>
    /// <param name="suffixRAtom"> </param>
    void makeBondsToSuffix(Atom *parentAtom, Atom *suffixRAtom);

public:
    virtual std::vector<SuffixRule *>
    getSuffixRuleTags(const std::wstring &suffixTypeToUse, const std::wstring &suffixValue,
                      const std::wstring &subgroupType) throw(ComponentGenerationException);
};

