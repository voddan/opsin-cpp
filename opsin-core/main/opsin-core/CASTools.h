#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ParseRules;
class ParsingException;


/// <summary>
/// Tools for converting CAS nomenclature into IUPAC nomenclature.
/// @author dl387
/// </summary>
class CASTools {

private:
    static Pattern *const matchCasCollectiveIndex = Pattern::compile(
            L"([\\[\\(\\{]([1-9][0-9]?[cC][iI][, ]?)+[\\]\\)\\}])+|[1-9][0-9]?[cC][iI]", Pattern::CASE_INSENSITIVE);
    static Pattern *const matchAcid = Pattern::compile(L"acid[\\]\\)\\}]*", Pattern::CASE_INSENSITIVE);
    static Pattern *const matchCommaSpace = Pattern::compile(L", ");
    static Pattern *const matchCompoundWithPhrase = Pattern::compile(L"(compd\\. with|compound with|and) ",
                                                                     Pattern::CASE_INSENSITIVE);
    static Pattern *const matchFunctionalTermAllowingSubstituentPrefix = Pattern::compile(
            L"(amide|hydrazide|(thi|selen|tellur)?oxime|hydrazone|(iso)?(semicarbazone|thiosemicarbazone|selenosemicarbazone|tellurosemicarbazone)|imide|imine|semioxamazone)[\\]\\)\\}]*",
            Pattern::CASE_INSENSITIVE);

    /// <summary>
    /// Inverts a CAS name.
    /// Throws an exception is OPSIN is unable to determine whether something is a substituent or functional term
    /// or if something unexpected in a CAS name is encountered </summary>
    /// <param name="name">
    /// @return </param>
    /// <exception cref="ParsingException"> </exception>
public:
    static std::wstring uninvertCASName(const std::wstring &name, ParseRules *parseRules) throw(ParsingException);

private:
    static boost::optional<wchar_t> missingCloseBracketCharIfApplicable(const std::wstring &component);

    /// <summary>
    /// Modifies the name of the parent acid from ic to ate (or ous to ite)
    /// hence allowing the formation of the uninverted ester </summary>
    /// <param name="parent">
    /// @return </param>
    /// <exception cref="ParsingException"> </exception>
    static std::wstring uninvertEster(const std::wstring &parent) throw(ParsingException);
};

