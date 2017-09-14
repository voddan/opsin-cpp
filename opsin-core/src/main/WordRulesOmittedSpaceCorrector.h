#pragma once

#include <string>
#include <vector>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class BuildState;
class Element;
class StructureBuildingException;
class Fragment;
class Atom;


using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.StructureBuildingMethods.findRightMostGroupInSubBracketOrRoot;


class WordRulesOmittedSpaceCorrector {
private:
    static Pattern *const matchAteOrIteEnding = Pattern::compile(L"[ai]t[e]?[\\])}]*$", Pattern::CASE_INSENSITIVE);

    BuildState *const state;
    Element *const parse;

public:
    virtual ~WordRulesOmittedSpaceCorrector() {
        delete state;
        delete parse;
    }

    WordRulesOmittedSpaceCorrector(BuildState *state, Element *parse);

    virtual void correctOmittedSpaces() throw(StructureBuildingException);

    /// <summary>
    /// Corrects cases like "methylethyl ether" to "methyl ethyl ether" </summary>
    /// <param name="divalentFunctionalGroupWordRule"> </param>
private:
    void checkAndCorrectOmittedSpacesInDivalentFunctionalGroupRule(Element *divalentFunctionalGroupWordRule);

    /// <summary>
    /// Corrects cases like methyl-2-ethylacetate --> methyl 2-ethylacetate </summary>
    /// <param name="wordRule"> </param>
    /// <exception cref="StructureBuildingException">  </exception>
    void checkAndCorrectOmittedSpaceEster(Element *wordRule) throw(StructureBuildingException);

    bool allBarFirstSubstituentHaveLocants(std::vector<Element *> &substituentsAndBrackets);

    bool
    insufficientSubstitutableHydrogenForSubstitution(std::vector<Element *> &substituentsAndBrackets, Fragment *frag,
                                                     int rootMultiplier);

    int getTotalOutAtomValency(Fragment *f);

    /// <summary>
    /// Ester form preferred when:
    /// mono is used on substituent
    /// alkyl chain is used on formate/acetate e.g. ethylacetate
    /// Root is carbamate, >=2 substituents, and this is the only word rule
    /// (ester and non-ester carbamates differ only by whether or not there is a space, heuristically the ester is almost always intended under these conditions) </summary>
    /// <param name="substituentGroupEl"> </param>
    /// <param name="multiplierValue"> </param>
    /// <param name="rootGroup"> </param>
    /// <param name="numOfSubstituents">
    /// @return </param>
    bool
    specialCaseWhereEsterPreferred(Element *substituentGroupEl, const std::wstring &multiplierValue, Element *rootGroup,
                                   int numOfSubstituents);

    bool substitutionWouldBeAmbiguous(Fragment *frag, const std::wstring &multiplierValue);

    bool checkSuitabilityOfSubstituentForEsterFormation(Element *subOrBracket, int rootFunctionalAtomsCount);

    bool hasSingleMonoValentCarbonOrSiliconRadical(Fragment *frag);

    std::vector<Atom *> getAtomForEachSubstitutableHydrogen(Fragment *frag);

    void
    transformToEster(Element *parentSimpleWordRule, Element *substituentOrBracket) throw(StructureBuildingException);
};

