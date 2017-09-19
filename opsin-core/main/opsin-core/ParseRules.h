#pragma once

#include <string>
#include <vector>
#include <algorithm>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class OpsinRadixTrie;
class AnnotatorState;
class ResourceManager;
class ParseRulesResults;
class ParsingException;
class ParseTokens;


using dk::brics::automaton::RunAutomaton;

/// <summary>
/// Instantiate via NameToStructure.getOpsinParser()
///
/// Performs finite-state allocation of roles ("annotations") to tokens:
/// The chemical name is broken down into tokens e.g. ethyl --&gt;eth yl by applying the chemical grammar in regexes.xml
/// The tokens eth and yl are associated with a letter which is referred to here as an annotation which is the role of the token.
/// These letters are defined in regexes.xml and would in this case have the meaning alkaneStem and inlineSuffix
///
/// The chemical grammar employs the annotations associated with the tokens when deciding what may follow what has already been seen
/// e.g. you cannot start a chemical name with yl and an optional e is valid after an arylGroup
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class ParseRules {

    /// <summary>
    /// A DFA encompassing the grammar of a chemical word. </summary>
private:
    RunAutomaton *const chemAutomaton;
    /// <summary>
    /// The allowed symbols in chemAutomaton </summary>
    std::vector<wchar_t> const stateSymbols;

    std::vector<OpsinRadixTrie *> const symbolTokenNamesDict;
    std::vector<RunAutomaton *> const symbolRegexAutomataDict;
    std::vector<Pattern *> const symbolRegexesDict;

    AnnotatorState *const initialState;

    /// <summary>
    /// Creates a left to right parser that can parse a substituent/full/functional word </summary>
    /// <param name="resourceManager"> </param>
public:
    virtual ~ParseRules() {
        delete chemAutomaton;
        delete initialState;
    }

    ParseRules(ResourceManager *resourceManager);

    /// <summary>
    ///Determines the possible annotations for a chemical word
    /// Returns a list of parses and how much of the word could not be interpreted
    /// e.g. usually the list will have only one parse and the string will equal ""
    /// For something like ethyloxime. The list will contain the parse for ethyl and the string will equal "oxime" as it was unparsable
    /// For something like eth no parses would be found and the string will equal "eth"
    /// </summary>
    /// <param name="chemicalWord"> </param>
    /// <returns> Results of parsing </returns>
    /// <exception cref="ParsingException"> </exception>
    virtual ParseRulesResults *getParses(const std::wstring &chemicalWord) throw(ParsingException);

private:
    ParseTokens *convertAnnotationStateToParseTokens(AnnotatorState *as, const std::wstring &chemicalWord,
                                                     const std::wstring &chemicalWordLowerCase);
};

