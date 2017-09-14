#pragma once

#include "ChemEl.h"
#include <unordered_map>
#include <vector>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;


/// <summary>
/// Provides valency checking features and a lookup on the possible valencies
/// for an atom given its element and charge
///
/// Also used to perform a final check on the output of OPSIN, to reject interpretations
/// that result in hypervalent structures due to incorrect names or misinterpreted names
///
/// @author ptc24
/// @author dl387
///
/// </summary>
class ValencyChecker {

    /// <summary>
    /// used to decide on the likely valency state </summary>
private:
    static const std::unordered_map<ChemEl, int> expectedDefaultValency;

    /// <summary>
    /// used to decide whether an atom has spare valency in a ring, these are the same as specified in the Hantzch-Widman system </summary>
    static const std::unordered_map<ChemEl, int> valencyInHW;

    /// <summary>
    /// used to decide on the likely valency state </summary>
    static const std::unordered_map<ChemEl, std::unordered_map<int, std::vector<Integer>>> possibleStableValencies;

private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static ValencyChecker::StaticConstructor staticConstructor;


    /// <summary>
    /// Given a chemical element (e.g. Na) and charge (e.g. 1) returns the highest stable valency that OPSIN knows is possible
    /// If for the particular combination of chemical element and charge the highest stable valency is not known null is returned </summary>
    /// <param name="chemEl"> </param>
    /// <param name="charge">
    /// @return </param>
public:
    static boost::optional<int> getMaximumValency(ChemEl chemEl, int charge);

    /// <summary>
    /// Return the lambda convention derived valency if set otherwise returns the same as <seealso cref="#getMaximumValency(ChemEl, int)"/>
    /// Returns null if the maximum valency is not known </summary>
    /// <param name="a">
    /// @return </param>
    static boost::optional<int> getMaximumValency(Atom *a);

    /// <summary>
    /// Checks whether the total incoming valency to an atom exceeds its expected valency
    /// outValency e.g. on radicals is taken into account </summary>
    /// <param name="a">
    /// @return </param>
    static bool checkValency(Atom *a);

    /// <summary>
    /// Check whether valency is available on the atom to form a bond of the given order.
    /// spareValency and outValency are not taken into account. </summary>
    /// <param name="a"> atom you are interested in </param>
    /// <param name="bondOrder"> order of bond required
    /// @return </param>
    static bool checkValencyAvailableForBond(Atom *a, int bondOrder);

    /// <summary>
    /// Check whether changing to a heteroatom will result in valency being exceeded
    /// spareValency and outValency is taken into account </summary>
    /// <param name="a"> atom you are interested in </param>
    /// <param name="heteroatom"> atom which will be replacing it
    /// @return </param>
    static bool checkValencyAvailableForReplacementByHeteroatom(Atom *a, Atom *heteroatom);

    /// <summary>
    /// Returns the default valency of an element when uncharged or null if unknown </summary>
    /// <param name="chemlEl">
    /// @return </param>
    static boost::optional<int> getDefaultValency(ChemEl chemlEl);

    /// <summary>
    /// Returns the valency of an element in the HW system (useful for deciding whether something should have double bonds in a ring) or null if unknown
    /// Note that the HW system makes no claim about valency when the atom is charged </summary>
    /// <param name="chemEl">
    /// @return </param>
    static boost::optional<int> getHWValency(ChemEl chemEl);

    /// <summary>
    /// Returns the maximum valency of an element with a given charge or null if unknown </summary>
    /// <param name="chemEl"> </param>
    /// <param name="charge">
    /// @return </param>
    static std::vector<boost::optional<int>> getPossibleValencies(ChemEl chemEl, int charge);
};

