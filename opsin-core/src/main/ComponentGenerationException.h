#pragma once

#include <string>
#include <stdexcept>


/// <summary>
///Thrown during component generation.
///
/// @author ptc24
///
/// </summary>
class ComponentGenerationException : public std::exception {

private:
    static constexpr long long serialVersionUID = 1LL;

public:
    ComponentGenerationException();

    ComponentGenerationException(const std::wstring &message);

    ComponentGenerationException(const std::wstring &message, std::exception cause);

    ComponentGenerationException(std::exception cause);

};

