#pragma once


enum class SuffixRuleType {
    addgroup,
    addSuffixPrefixIfNonePresentAndCyclic,
    setOutAtom,
    changecharge,
    addFunctionalAtomsToHydroxyGroups,
    chargeHydroxyGroups,
    removeTerminalOxygen,
    convertHydroxyGroupsToOutAtoms,
    convertHydroxyGroupsToPositiveCharge,
    setAcidicElement
};

