#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Tokeniser;
class WordRules;
class ResourceManager;
class ParseRules;
class Element;
class NameToStructureConfig;
class ParsingException;
class TokenizationResult;
class Parse;


using org::apache::log4j::Logger;

using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

/// <summary>
///Conducts finite-state parsing on chemical names.
/// Adds XML annotation to the semantic constituents of the name.
///
/// @author ptc24/dl387
///
/// </summary>
class Parser {

    /// <summary>
    ///Uses ParseRules to intelligently parse chemical names into substituents/full terms/functional terms </summary>
private:
    Tokeniser *const tokeniser;
    /// <summary>
    ///The rules by which words are grouped together (e.g. in functional class nomenclature) </summary>
    WordRules *const wordRules;
    /// <summary>
    ///Holds the various tokens used. </summary>
    ResourceManager *const resourceManager;
    ParseRules *const parseRules;

    static Pattern *const matchSemiColonSpace = Pattern::compile(L"; ");
    static Pattern *const matchStoichiometryIndication = Pattern::compile(
            L"[ ]?[\\{\\[\\(](\\d+|\\?)([:/](\\d+|\\?))+[\\}\\]\\)]$");
    static Logger *const LOG = Logger::getLogger(Parser::
    typeid);

    /// <summary>
    /// No-argument constructor. Uses ResouceGetter found at
    /// uk/ac/cam/ch/wwmm/opsin/resources/ </summary>
    /// <exception cref="IOException">  </exception>
public:
    virtual ~Parser() {
        delete tokeniser;
        delete wordRules;
        delete resourceManager;
        delete parseRules;
    }

    Parser() throw(IOException);

    /// <summary>
    ///Initialises the parser. </summary>
    /// <param name="resourceManager"> </param>
    /// <param name="tokeniser"> </param>
    /// <param name="wordRules"> </param>
    Parser(WordRules *wordRules, Tokeniser *tokeniser, ResourceManager *resourceManager);

    /// <summary>
    ///Parses a chemical name to an XML representation of the parse. </summary>
    /// <param name="n2sConfig">
    /// </param>
    /// <param name="name"> The name to parse. </param>
    /// <returns> The parse. </returns>
    /// <exception cref="ParsingException"> If the name is unparsable. </exception>
    virtual std::vector<Element *>
    parse(NameToStructureConfig *n2sConfig, const std::wstring &name) throw(ParsingException);

    static std::vector<boost::optional<int>>
    processStoichiometryIndication(const std::wstring &ratioString) throw(ParsingException);

private:
    void generateExactParseFailureReason(TokenizationResult *tokenizationResult,
                                         const std::wstring &name) throw(ParsingException);

    /// <summary>
    /// For cases where any of the parse's parseWords contain multiple annotations create a
    /// parse for each possibility. Hence after this process there may be multiple Parse objects but
    /// the parseWords they contain will each only have one parseTokens object. </summary>
    /// <param name="parse">
    /// @return </param>
    /// <exception cref="ParsingException">  </exception>
    std::vector<Parse *> generateParseCombinations(Parse *parse) throw(ParsingException);

    /// <summary>
    ///Write the XML corresponding to a particular word in a parse.
    /// </summary>
    /// <param name="wordEl"> The empty XML word element to be written into. </param>
    /// <param name="tokens"> The list of tokens. </param>
    /// <param name="annotations"> The lists of annotations. This has been divided into a separate list per substituent/root/functionalTerm </param>
    /// <exception cref="ParsingException"> </exception>
public:
    virtual void writeWordXML(Element *wordEl, std::vector<std::wstring> &tokens,
                              std::vector<std::vector<wchar_t>> &annotations) throw(ParsingException);

    /// <summary>
    /// Assigns an indication of stoichiometry to each child word rule of the moleculeEl.
    /// Throws an exception if there is a mismatch between the number of word rules and ratio. </summary>
    /// <param name="moleculeEl"> </param>
    /// <param name="componentRatios"> </param>
    /// <exception cref="ParsingException"> </exception>
private:
    void applyStoichiometryIndicationToWordRules(Element *moleculeEl,
                                                 std::vector<Integer> &componentRatios) throw(ParsingException);

};

