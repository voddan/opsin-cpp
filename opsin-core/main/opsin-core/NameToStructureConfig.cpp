#include "NameToStructureConfig.h"


NameToStructureConfig::NameToStructureConfig() {
}

bool NameToStructureConfig::isAllowRadicals() {
    return allowRadicals;
}

void NameToStructureConfig::setAllowRadicals(bool allowRadicals) {
    this->allowRadicals = allowRadicals;
}

bool NameToStructureConfig::isOutputRadicalsAsWildCardAtoms() {
    return outputRadicalsAsWildCardAtoms;
}

void NameToStructureConfig::setOutputRadicalsAsWildCardAtoms(bool outputRadicalsAsWildCardAtoms) {
    this->outputRadicalsAsWildCardAtoms = outputRadicalsAsWildCardAtoms;
}

bool NameToStructureConfig::isDetailedFailureAnalysis() {
    return detailedFailureAnalysis;
}

void NameToStructureConfig::setDetailedFailureAnalysis(bool detailedFailureAnalysis) {
    this->detailedFailureAnalysis = detailedFailureAnalysis;
}

bool NameToStructureConfig::allowInterpretationOfAcidsWithoutTheWordAcid() {
    return interpretAcidsWithoutTheWordAcid;
}

void NameToStructureConfig::setInterpretAcidsWithoutTheWordAcid(bool interpretAcidsWithoutTheWordAcid) {
    this->interpretAcidsWithoutTheWordAcid = interpretAcidsWithoutTheWordAcid;
}

bool NameToStructureConfig::warnRatherThanFailOnUninterpretableStereochemistry() {
    return warnRatherThanFailOnUninterpretableStereochemistry_Renamed;
}

void NameToStructureConfig::setWarnRatherThanFailOnUninterpretableStereochemistry(
        bool warnRatherThanFailOnUninterpretableStereochemistry) {
    this->warnRatherThanFailOnUninterpretableStereochemistry_Renamed = warnRatherThanFailOnUninterpretableStereochemistry;
}

NameToStructureConfig *NameToStructureConfig::getDefaultConfigInstance() {
    return new NameToStructureConfig();
}

NameToStructureConfig *NameToStructureConfig::clone() {
    try {
        return static_cast<NameToStructureConfig *>(__super::clone());
    }
    catch (const CloneNotSupportedException &e) {
        // Can only be thrown if we *don't* implement Cloneable, which we do...
        throw Error(L"Impossible!", e);
    }
}
