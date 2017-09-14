#pragma once

#include <string>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class PreProcessingException;


/// <summary>
/// Takes a name:
/// strips leading/trailing white space
/// Normalises representation of greeks and some other characters
/// @author dl387
///
/// </summary>
class PreProcessor {
private:
    static const std::unordered_map<std::wstring, std::wstring> DOTENCLOSED_TO_DESIRED;
    static const std::unordered_map<std::wstring, std::wstring> XMLENTITY_TO_DESIRED;

private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static PreProcessor::StaticConstructor staticConstructor;


    /// <summary>
    /// Master method for PreProcessing </summary>
    /// <param name="chemicalName">
    /// @return </param>
    /// <exception cref="PreProcessingException">  </exception>
public:
    static std::wstring preProcess(const std::wstring &chemicalName) throw(PreProcessingException);

private:
    static std::wstring performMultiCharacterReplacements(const std::wstring &chemicalName);

    static std::wstring getLowerCasedDotEnclosedString(const std::wstring &chemicalName, int indexOfFirstDot);

    static std::wstring getLowerCasedXmlEntityString(const std::wstring &chemicalName, int indexOfAmpersand);

    static std::wstring getReplacementForDollarGreek(wchar_t ch);

};

