#pragma once

#include "ChemEl.h"
#include <unordered_map>
#include <boost/optional.hpp>


/// <summary>
/// Holds useful atomic properties
/// @author dl387
///
/// </summary>
class AtomProperties {

private:
    static const std::unordered_map<ChemEl, double> elementToPaulingElectronegativity;
    static const std::unordered_map<ChemEl, int> elementToHwPriority;

private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static AtomProperties::StaticConstructor staticConstructor;


    /// <summary>
    /// Useful to give an indication of whether a bond is like to be ionic (diff >1.8), polar or covalent (diff < 1.2) </summary>
    /// <param name="chemEl">
    /// @return </param>
public:
    static boost::optional<double> getPaulingElectronegativity(ChemEl chemEl);

    /// <summary>
    /// Maps chemEl to the priority of that atom in Hantzch-Widman system. A higher value indicates a higher priority. </summary>
    /// <param name="chemEl">
    /// @return </param>
    static boost::optional<int> getHwpriority(ChemEl chemEl);
};

