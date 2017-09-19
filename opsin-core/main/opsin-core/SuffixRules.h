#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ApplicableSuffix;
class SuffixRule;
class ResourceGetter;
class ComponentGenerationException;


using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;




class SuffixRules {

    /// <summary>
    ///For a given group type what suffixes are applicable.
    /// Within this group type which are applicable for a given suffixValue
    /// Returns a list as different group subTypes can give different meanings
    /// </summary>
private:
    const std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::vector<ApplicableSuffix *>>> suffixApplicability;

private:
    class ApplicableSuffix {

    private:
        const std::wstring requiredSubType;
        const std::vector<SuffixRule *> suffixRules;

    public:
        ApplicableSuffix(const std::wstring &requiredSubType, std::vector<SuffixRule *> &suffixRules);
    };

public:
    SuffixRules(ResourceGetter *resourceGetter) throw(IOException);

private:
    std::unordered_map<std::wstring, std::vector<SuffixRule *>>
    generateSuffixRulesMap(ResourceGetter *resourceGetter) throw(IOException);


    std::vector<SuffixRule *> processSuffixRules(XMLStreamReader *reader) throw(XMLStreamException);

    std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::vector<ApplicableSuffix *>>>
    generateSuffixApplicabilityMap(ResourceGetter *resourceGetter,
                                   std::unordered_map<std::wstring, std::vector<SuffixRule *>> &suffixRulesMap) throw(IOException);


    /// <summary>
    /// Returns the appropriate suffixRules for the given arguments.
    /// The suffix rules are the children of the appropriate rule in suffixRules.xml </summary>
    /// <param name="suffixTypeToUse"> </param>
    /// <param name="suffixValue"> </param>
    /// <param name="subgroupType">
    /// @return </param>
    /// <exception cref="ComponentGenerationException"> </exception>
public:
    virtual std::vector<SuffixRule *>
    getSuffixRuleTags(const std::wstring &suffixTypeToUse, const std::wstring &suffixValue,
                      const std::wstring &subgroupType) throw(ComponentGenerationException);


    /// <summary>
    /// Does suffixApplicability.xml have an entry for this group type? </summary>
    /// <param name="groupType">
    /// @return </param>
    virtual bool isGroupTypeWithSpecificSuffixRules(const std::wstring &groupType);


};

