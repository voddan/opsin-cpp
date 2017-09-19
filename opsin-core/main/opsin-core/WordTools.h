#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ParseTokens;
class ParseWord;
class ParsingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

/// <summary>
/// Tools for dealing uniformly with unusually-formed words.
/// </summary>
class WordTools {
    /// <summary>
    /// Splits cases where the parseTokensList describes a functionalTerm in addition to another mainGroup/substituent into two parseWords
    /// This occurs if the name is formally missing a space e.g. ethylthiocyanate.
    /// If multiple parses are present then it may be possible to disambiguate between them:
    /// 	parses with omitted spaces are discarded if a parse without omitted space is found
    /// 	parses with shorter functional terms are discarded e.g. ethylthiocyanate is [ethyl] [thiocyanate] not [ethylthio] [cyanate] </summary>
    /// <param name="parseTokensList"> </param>
    /// <param name="chemicalName">
    /// @return </param>
public:
    static std::vector<ParseWord *>
    splitIntoParseWords(std::vector<ParseTokens *> &parseTokensList, const std::wstring &chemicalName);

private:
    static bool containsOmittedSpace(std::vector<std::vector<wchar_t>> &chunkedAnnotations);

    /// <summary>
    ///Groups the token annotations for a given word into substituent/s and/or a maingroup and/or functionalTerm by
    /// looking for the endOfSubstituent/endOfMainGroup/endOfFunctionalTerm annotations
    /// </summary>
    /// <param name="annots"> The annotations for a word. </param>
    /// <returns> A List of lists of annotations, each list corresponds to a substituent/maingroup/functionalTerm </returns>
public:
    static std::vector<std::vector<wchar_t>> chunkAnnotations(std::vector<wchar_t> &annots);

    /// <summary>
    /// Works left to right removing spaces if there are too many opening brackets </summary>
    /// <param name="name">
    /// @return </param>
    /// <exception cref="ParsingException"> If brackets are unbalanced and cannot be balanced by removing whitespace </exception>
    static std::wstring removeWhiteSpaceIfBracketsAreUnbalanced(const std::wstring &name) throw(ParsingException);
};

