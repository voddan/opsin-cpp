#include "NameToStructureException.h"


NameToStructureException::NameToStructureException() : RuntimeException() {
}

NameToStructureException::NameToStructureException(const std::wstring &message) : RuntimeException(message) {
}

NameToStructureException::NameToStructureException(const std::wstring &message, std::exception cause)
        : RuntimeException(message, cause) {
}

NameToStructureException::NameToStructureException(std::exception cause) : RuntimeException(cause) {
}
