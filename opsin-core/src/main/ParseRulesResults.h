#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ParseTokens;


/// <summary>
/// A wrapper for the results from parsing a chemical name or part of a chemical name
/// through ParseRules
///
/// @author dl387
/// </summary>
class ParseRulesResults {
private:
    const std::vector<ParseTokens *> parseTokensList;
    const std::wstring uninterpretableName;
    const std::wstring unparseableName;

public:
    ParseRulesResults(std::vector<ParseTokens *> &parseTokensList, const std::wstring &uninterpretableName,
                      const std::wstring &unparseableName);

    /// <summary>
    /// One ParseTokens object is returned for each possible interpretation of a chemical name
    /// If none of the name can be interpreted this list will be empty </summary>
    /// <returns> List of possible tokenisations/annotation of tokens </returns>
    virtual std::vector<ParseTokens *> getParseTokensList();

    /// <summary>
    /// The substring of the name that could not be classified into a substituent/full/functionalTerm
    /// e.g. in ethyl-2H-fooarene  "2H-fooarene" will be returned </summary>
    /// <returns> String of uninterpetable chemical name </returns>
    virtual std::wstring getUninterpretableName();

    /// <summary>
    /// The substring of the name that could not be tokenised at all.
    /// This will always be the same or shorter than the uninterpetable substring of name
    /// e.g. in ethyl-2H-fooarene  "fooarene" will be returned </summary>
    /// <returns> String of unparseable chemical name </returns>
    virtual std::wstring getUnparseableName();

    virtual std::wstring toString() override;

};

