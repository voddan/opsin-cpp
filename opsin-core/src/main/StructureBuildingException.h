#pragma once

#include <string>
#include <stdexcept>


/// <summary>
///Thrown during assembly of the structure
///
/// @author ptc24
///
/// </summary>
class StructureBuildingException : public std::exception {

private:
    static constexpr long long serialVersionUID = 1LL;

public:
    StructureBuildingException();

    StructureBuildingException(const std::wstring &message);

    StructureBuildingException(const std::wstring &message, std::exception cause);

    StructureBuildingException(std::exception cause);

};

