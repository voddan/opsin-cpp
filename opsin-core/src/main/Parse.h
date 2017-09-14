#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ParseWord;


/// <summary>
///A "struct" containing data on the parsing of a chemical name.
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class Parse {
    /// <summary>
    ///The chemical name. </summary>
private:
    const std::wstring name;
    /// <summary>
    ///The words within the name, and their parsing data. </summary>
    const std::vector<ParseWord *> words = std::vector<ParseWord *>();

    /// <summary>
    /// Creates a parse object for a chemicalName </summary>
    /// <param name="chemicalName"> </param>
public:
    Parse(const std::wstring &chemicalName);

    virtual Parse *deepCopy();

    virtual std::wstring toString() override;

    virtual std::vector<ParseWord *> getWords();

    virtual bool addWord(ParseWord *word);

    virtual bool removeWord(ParseWord *word);

    virtual ParseWord *getWord(int indice);

    virtual std::wstring getName();

};

