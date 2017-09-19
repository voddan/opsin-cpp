#include "ComponentGenerationException.h"


ComponentGenerationException::ComponentGenerationException() : Exception() {
}

ComponentGenerationException::ComponentGenerationException(const std::wstring &message) : Exception(message) {
}

ComponentGenerationException::ComponentGenerationException(const std::wstring &message, std::exception cause)
        : Exception(message, cause) {
}

ComponentGenerationException::ComponentGenerationException(std::exception cause) : Exception(cause) {
}
