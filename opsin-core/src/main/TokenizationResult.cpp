#include "TokenizationResult.h"
#include "Parse.h"


TokenizationResult::TokenizationResult(const std::wstring &name) : parse(new Parse(name)) {
    this->workingName = L"";
    this->unparsableName = L"";
    this->unparsedName = name;
    this->uninterpretableName = L"";
}

bool TokenizationResult::isSuccessfullyTokenized() {
    return unparsedName.length() == 0;
}

Parse *TokenizationResult::getParse() {
    return parse;
}

void TokenizationResult::setUninterpretableName(const std::wstring &name) {
    this->uninterpretableName = name;
}

std::wstring TokenizationResult::getUninterpretableName() {
    return this->uninterpretableName;
}

std::wstring TokenizationResult::getWorkingName() {
    return workingName;
}

void TokenizationResult::setWorkingName(const std::wstring &name) {
    this->workingName = name;
}

bool TokenizationResult::isFullyInterpretable() {
    return (std::wstring(L"")) == workingName;
}

std::wstring TokenizationResult::getUnparsableName() {
    return unparsableName;
}

void TokenizationResult::setUnparsableName(const std::wstring &name) {
    this->unparsableName = name;
}

std::wstring TokenizationResult::getUnparsedName() {
    return unparsedName;
}

void TokenizationResult::setUnparsedName(const std::wstring &name) {
    this->unparsedName = name;
}

void TokenizationResult::setErrorFields(const std::wstring &unparsedName, const std::wstring &uninterpretableName,
                                        const std::wstring &unparsableName) {
    this->unparsedName = unparsedName;
    this->uninterpretableName = uninterpretableName;
    this->unparsableName = unparsableName;
}
