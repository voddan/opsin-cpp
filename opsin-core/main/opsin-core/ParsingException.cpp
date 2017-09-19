#include "ParsingException.h"


ParsingException::ParsingException() : Exception() {
}

ParsingException::ParsingException(const std::wstring &message) : Exception(message) {
}

ParsingException::ParsingException(const std::wstring &message, std::exception cause) : Exception(message, cause) {
}

ParsingException::ParsingException(std::exception cause) : Exception(cause) {
}
