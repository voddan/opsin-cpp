#include "SuffixRule.h"
#include "Attribute.h"


SuffixRule::SuffixRule(SuffixRuleType type, std::vector<Attribute *> &attributes) : type(type), attributes(attributes) {
}

SuffixRuleType SuffixRule::getType() {
    return type;
}

std::wstring SuffixRule::getAttributeValue(const std::wstring &name) {
    for (auto a : attributes) {
        if (a->getName().equals(name)) {
            return a->getValue();
        }
    }
    return L"";
}
