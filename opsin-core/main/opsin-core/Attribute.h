#pragma once

#include <string>
#include "stringbuilder.h"


class Attribute {

private:
    const std::wstring name;
    std::wstring value;

public:
    Attribute(const std::wstring &name, const std::wstring &value);

    /// <summary>
    /// Creates a copy </summary>
    /// <param name="attribute"> </param>
    Attribute(const Attribute &attribute);

    virtual std::wstring getValue();

    virtual std::wstring getName();

    virtual void setValue(const std::wstring &value);

    virtual std::wstring toXML();

    virtual std::wstring toString() override;

private:
    std::wstring escapeText(const std::wstring &s);
};
