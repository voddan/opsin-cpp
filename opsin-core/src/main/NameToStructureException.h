#pragma once

#include <string>
#include <stdexcept>


/// <summary>
///Thrown if OPSIN failed to initialise
///
/// @author ptc24
///
/// </summary>
class NameToStructureException : public std::exception {

private:
    static constexpr long long serialVersionUID = 1LL;

public:
    NameToStructureException();

    NameToStructureException(const std::wstring &message);

    NameToStructureException(const std::wstring &message, std::exception cause);

    NameToStructureException(std::exception cause);

};

