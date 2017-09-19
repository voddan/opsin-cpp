#pragma once

#include "SuffixRuleType.h"
#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Attribute;


class SuffixRule {

private:
    const SuffixRuleType type;
    const std::vector<Attribute *> attributes;

public:
    SuffixRule(SuffixRuleType type, std::vector<Attribute *> &attributes);

    virtual SuffixRuleType getType();

    /// <summary>
    /// Returns the value of the attribute with the given name
    /// or null if the attribute doesn't exist </summary>
    /// <param name="name">
    /// @return </param>
    virtual std::wstring getAttributeValue(const std::wstring &name);
};

