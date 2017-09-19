#pragma once

#include <string>


///
/// <summary>
/// @author dl387
/// </summary>
/// @param <T> </param>
template<typename T>
class PropertyKey {
private:
    const std::wstring name;

public:
    PropertyKey(const std::wstring &name) : name(name) {
    }

    virtual int hashCode() override {
        return 37 * (name != L"" ? name.hashCode() : 0);
    }

    virtual bool equals(void *obj) override {
        return this == obj;
    }

    virtual std::wstring toString() override {
        return std::wstring(L"Key{") + name + std::wstring(L"}");
    }
};

