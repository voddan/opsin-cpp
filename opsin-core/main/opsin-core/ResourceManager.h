#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <boost/optional.hpp>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class TokenEl;
class ResourceGetter;
class AutomatonInitialiser;
class OpsinRadixTrie;
class ParsingException;


using dk::brics::automaton::RunAutomaton;
using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

/// <summary>
///Holds all of the tokens used in parsing of chemical names.
/// Holds all automata
/// Generates XML Elements for tokens.
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class ResourceManager {
private:
    static TokenEl *const IGNORE_WHEN_WRITING_PARSE_TREE = new TokenEl(L"");

    /// <summary>
    ///Used to load XML files. </summary>
    ResourceGetter *const resourceGetter;

    /// <summary>
    ///Used to serialise and deserialise automata. </summary>
    AutomatonInitialiser *const automatonInitialiser;

    /// <summary>
    ///A mapping between primitive tokens, and annotation->Token object mappings. </summary>
    const std::unordered_map<std::wstring, std::unordered_map<wchar_t, TokenEl *>> tokenDict = std::unordered_map<std::wstring, std::unordered_map<wchar_t, TokenEl *>>();
    /// <summary>
    ///A mapping between regex tokens, and annotation->Token object mappings. </summary>
    const std::unordered_map<wchar_t, TokenEl *> reSymbolTokenDict = std::unordered_map<wchar_t, TokenEl *>();


    /// <summary>
    ///A mapping between annotation symbols and a trie of tokens. </summary>
    std::vector<OpsinRadixTrie *> const symbolTokenNamesDict;
    /// <summary>
    ///A mapping between annotation symbols and DFAs (annotation->automata mapping). </summary>
    std::vector<RunAutomaton *> const symbolRegexAutomataDict;
    /// <summary>
    ///A mapping between annotation symbols and regex patterns (annotation->regex pattern mapping). </summary>
    std::vector<Pattern *> const symbolRegexesDict;

    /// <summary>
    ///The automaton which describes the grammar of a chemical name from left to right </summary>
    RunAutomaton *const chemicalAutomaton;


    /// <summary>
    ///As symbolTokenNamesDict but the tokens are reversed </summary>
    std::vector<OpsinRadixTrie *> symbolTokenNamesDictReversed;
    /// <summary>
    ///As symbolRegexAutomataDict but automata are reversed </summary>
    std::vector<RunAutomaton *> symbolRegexAutomataDictReversed;
    /// <summary>
    ///As symbolRegexesDict but regexes match the end of string </summary>
    std::vector<Pattern *> symbolRegexesDictReversed;

    /// <summary>
    ///The automaton which describes the grammar of a chemical name from right to left </summary>
    RunAutomaton *reverseChemicalAutomaton;

    /// <summary>
    ///Generates the ResourceManager.
    /// This involves reading in the token files, the regexToken file (regexTokens.xml) and the grammar file (regexes.xml).
    /// DFA are built or retrieved for the regexTokens and the chemical grammar.
    ///
    /// Throws an exception if the XML token and regex files can't be read in properly or the grammar cannot be built. </summary>
    /// <param name="resourceGetter"> </param>
    /// <exception cref="IOException">  </exception>
public:
    virtual ~ResourceManager() {
        delete resourceGetter;
        delete automatonInitialiser;
        delete chemicalAutomaton;
        delete reverseChemicalAutomaton;
    }

    ResourceManager(ResourceGetter *resourceGetter) throw(IOException);

    /// <summary>
    /// Processes tokenFiles </summary>
    /// <param name="reversed"> Should the tokens be reversed </param>
    /// <exception cref="IOException">  </exception>
private:
    void processTokenFiles(bool reversed) throw(IOException);

    void processTokenFile(const std::wstring &fileName, bool reversed) throw(IOException);

    void processTokenList(XMLStreamReader *reader, bool reversed) throw(XMLStreamException);

    void addToken(const std::wstring &text, TokenEl *el, boost::optional<wchar_t> symbol, int index, bool reversed);

    void processRegexTokenFiles(bool reversed) throw(IOException);

    void addRegexToken(XMLStreamReader *reader, const std::wstring &regex, bool reversed);

    RunAutomaton *processChemicalGrammar(bool reversed) throw(IOException);

public:
//JAVA TO C++ CONVERTER WARNING: The following method was originally marked 'synchronized':
    virtual void populatedReverseTokenMappings() throw(IOException);

    /// <summary>
    ///Given a token string and an annotation character, makes the XML element for
    /// the token string. </summary>
    /// <param name="tokenString"> The token string. </param>
    /// <param name="symbol"> The annotation character.
    /// </param>
    /// <returns> The XML element produced. </returns>
    /// <exception cref="ParsingException"> </exception>
    virtual TokenEl *
    makeTokenElement(const std::wstring &tokenString, boost::optional<wchar_t> symbol) throw(ParsingException);

    virtual RunAutomaton *getChemicalAutomaton();

    virtual std::vector<OpsinRadixTrie *> getSymbolTokenNamesDict();

    virtual std::vector<RunAutomaton *> getSymbolRegexAutomataDict();

    virtual std::vector<Pattern *> getSymbolRegexesDict();

    virtual RunAutomaton *getReverseChemicalAutomaton();

    virtual std::vector<OpsinRadixTrie *> getSymbolTokenNamesDictReversed();

    virtual std::vector<RunAutomaton *> getSymbolRegexAutomataDictReversed();

    virtual std::vector<Pattern *> getSymbolRegexesDictReversed();

};

