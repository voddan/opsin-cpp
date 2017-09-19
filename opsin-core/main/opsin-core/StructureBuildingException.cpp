#include "StructureBuildingException.h"


StructureBuildingException::StructureBuildingException() : Exception() {
}

StructureBuildingException::StructureBuildingException(const std::wstring &message) : Exception(message) {
}

StructureBuildingException::StructureBuildingException(const std::wstring &message, std::exception cause) : Exception(
        message, cause) {
}

StructureBuildingException::StructureBuildingException(std::exception cause) : Exception(cause) {
}
