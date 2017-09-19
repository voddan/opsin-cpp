#include "ParseTokens.h"


ParseTokens::ParseTokens(std::vector<std::wstring> &tokens, std::vector<wchar_t> &annotations) : tokens(
        Collections::unmodifiableList(std::vector<std::wstring>(tokens))), annotations(
        Collections::unmodifiableList(std::vector<wchar_t>(annotations))) {
    if (tokens.size() != annotations.size()) {
        throw std::invalid_argument("OPSIN bug: mismatch between the sizes of tokens list and annotation list");
    }
}

std::vector<std::wstring> ParseTokens::getTokens() {
    return tokens;
}

std::vector<wchar_t> ParseTokens::getAnnotations() {
    return annotations;
}

std::wstring ParseTokens::toString() {
    return std::wstring(L"[") + tokens + std::wstring(L", ") + annotations + std::wstring(L"]");
}

bool ParseTokens::equals(void *other) {
    if (this == other) {
        return true;
    }
    if (dynamic_cast<ParseTokens *>(other) != nullptr) {
        ParseTokens *otherPT = static_cast<ParseTokens *>(other);
        return this->tokens.equals(otherPT->tokens) && this->annotations.equals(otherPT->annotations);
    }
    return false;
}

int ParseTokens::hashCode() {
    return (3 * this->tokens.hashCode()) * (7 * this->annotations.hashCode());
}
