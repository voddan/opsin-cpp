#include "OpsinResult.h"
#include "Fragment.h"
#include "OpsinWarning.h"
#include "CMLWriter.h"
#include "SMILESWriter.h"


using org::apache::log4j::Logger;
using OpsinWarningType = uk::ac::cam::ch::wwmm::opsin::OpsinWarning::OpsinWarningType;

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
OpsinResult::OpsinResult(Fragment *frag, OPSIN_RESULT_STATUS status, std::vector<OpsinWarning *> &warnings,
                         const std::wstring &chemicalName) : structure(frag), status(status), message(sb->toString()),
                                                             chemicalName(chemicalName), warnings(warnings) {
    StringBuilder *sb = new StringBuilder();
    for (int i = 0, l = warnings.size(); i < l; i++) {
        OpsinWarning *warning = warnings[i];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        sb->append(warning->getType()
        ::toString());
        sb->append(L": ");
        sb->append(warning->getMessage());
        if (i + 1 < l) {
            sb->append(L"; ");
        }
    }
}

OpsinResult::OpsinResult(Fragment *frag, OPSIN_RESULT_STATUS status, const std::wstring &message,
                         const std::wstring &chemicalName) : structure(frag), status(status), message(message),
                                                             chemicalName(chemicalName),
                                                             warnings(Collections::emptyList()) {
}

Fragment *OpsinResult::getStructure() {
    return structure;
}

OpsinResult::OPSIN_RESULT_STATUS OpsinResult::getStatus() {
    return status;
}

std::wstring OpsinResult::getMessage() {
    return message;
}

std::wstring OpsinResult::getChemicalName() {
    return chemicalName;
}

std::wstring OpsinResult::getCml() {
    if (structure != nullptr) {
        try {
            return CMLWriter::generateCml(structure, chemicalName);
        }
        catch (const std::exception &e) {
            LOG->debug(L"CML generation failed", e);
        }
    }
    return L"";
}

std::wstring OpsinResult::getPrettyPrintedCml() {
    if (structure != nullptr) {
        try {
            return CMLWriter::generateIndentedCml(structure, chemicalName);
        }
        catch (const std::exception &e) {
            LOG->debug(L"CML generation failed", e);
        }
    }
    return L"";
}

std::wstring OpsinResult::getSmiles() {
    if (structure != nullptr) {
        try {
            return SMILESWriter::generateSmiles(structure);
        }
        catch (const std::exception &e) {
            LOG->debug(L"SMILES generation failed", e);
        }
    }
    return L"";
}

std::wstring OpsinResult::getExtendedSmiles() {
    if (structure != nullptr) {
        try {
            return SMILESWriter::generateExtendedSmiles(structure);
        }
        catch (const std::exception &e) {
            LOG->debug(L"Extended SMILES generation failed", e);
        }
    }
    return L"";
}

std::vector<OpsinWarning *> OpsinResult::getWarnings() {
    return Collections::unmodifiableList(warnings);
}

bool OpsinResult::nameAppearsToBeAmbiguous() {
    for (auto warning : warnings) {
        if (warning->getType() == OpsinWarningType::APPEARS_AMBIGUOUS) {
            return true;
        }
    }
    return false;
}

bool OpsinResult::stereochemistryIgnored() {
    for (auto warning : warnings) {
        if (warning->getType() == OpsinWarningType::STEREOCHEMISTRY_IGNORED) {
            return true;
        }
    }
    return false;
}
