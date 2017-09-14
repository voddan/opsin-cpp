#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class OpsinRadixTrie;
class ResourceManager;
class ParseRulesResults;
class ParsingException;
class AnnotatorState;
class ParseTokens;


using dk::brics::automaton::RunAutomaton;

/// <summary>
/// The same as ParseRules but works from right to left
///
/// Performs finite-state allocation of roles ("annotations") to tokens:
/// The chemical name is broken down into tokens e.g. ethyl -->eth yl by applying the chemical grammar in regexes.xml
/// The tokens eth and yl are associated with a letter which is referred to here as an annotation which is the role of the token.
/// These letters are defined in regexes.xml and would in this case have the meaning alkaneStem and inlineSuffix
///
/// The chemical grammar employs the annotations associated with the tokens when deciding what may follow what has already been seen
/// e.g. you cannot start a chemical name with yl and an optional e is valid after an arylGroup
///
/// @author dl387
///
/// </summary>
class ReverseParseRules {

    /// <summary>
    /// A DFA encompassing the grammar of a chemical word. </summary>
private:
    RunAutomaton *const chemAutomaton;
    /// <summary>
    /// The allowed symbols in chemAutomaton </summary>
    std::vector<wchar_t> const stateSymbols;

    std::vector<OpsinRadixTrie *> const symbolTokenNamesDictReversed;
    std::vector<RunAutomaton *> const symbolRegexAutomataDictReversed;
    std::vector<Pattern *> const symbolRegexesDictReversed;

    /// <summary>
    /// Creates a right to left parser that can parse a substituent/full/functional word </summary>
    /// <param name="resourceManager"> </param>
    /// <exception cref="IOException">  </exception>
public:
    virtual ~ReverseParseRules() {
        delete chemAutomaton;
    }

    ReverseParseRules(ResourceManager *resourceManager) throw(IOException);

    /// <summary>
    ///Determines the possible annotations for a chemical word
    /// Returns a list of parses and how much of the word could not be interpreted
    /// e.g. usually the list will have only one parse and the string will equal ""
    /// For something like ethyloxime. The list will contain the parse for ethyl and the string will equal "oxime" as it was unparsable
    /// For something like eth no parses would be found and the string will equal "eth"
    /// </summary>
    /// <param name="chemicalWord">
    /// @return </param>
    /// <exception cref="ParsingException"> </exception>
    virtual ParseRulesResults *getParses(const std::wstring &chemicalWord) throw(ParsingException);

    /// <summary>
    /// Returns the length of the longest accepted run of the given string
    /// starting at pos in the string and working backwards </summary>
    /// <param name="automaton"> </param>
    /// <param name="s"> the string </param>
    /// <param name="indexAfterFirstchar"> pos in string to start at </param>
    /// <returns> length of the longest accepted run, -1 if no run is accepted </returns>
private:
    int runInReverse(RunAutomaton *automaton, const std::wstring &s, int indexAfterFirstchar);

    ParseTokens *convertAnnotationStateToParseTokens(AnnotatorState *as, const std::wstring &chemicalWord,
                                                     const std::wstring &chemicalWordLowerCase);
};

