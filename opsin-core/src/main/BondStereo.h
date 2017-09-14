#pragma once

#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;


/// <summary>
/// Holds information about the positions of 2 atoms relative to a double bond allowing the specification of cis/trans stereochemistry
/// @author dl387
///
/// </summary>
class BondStereo {

private:
    std::vector<Atom *> atomRefs4;
    BondStereoValue bondStereoValue = static_cast<BondStereoValue>(0);

    /// <summary>
    /// Possible values for a bondStereo element
    /// @author dl387
    ///
    /// </summary>
public:
    enum class BondStereoValue {
//JAVA TO C++ CONVERTER TODO TASK: Enum values cannot be strings in C++:
                CIS(L"C"),
//JAVA TO C++ CONVERTER TODO TASK: Enum values cannot be strings in C++:
                TRANS(L"T");

//JAVA TO C++ CONVERTER TODO TASK: Enums cannot contain fields in C++:
//								private final String value;
//JAVA TO C++ CONVERTER TODO TASK: Enums cannot contain methods in C++:
//								BondStereoValue(String value)
        //		{
        //			this.value = value;
        //		}
//JAVA TO C++ CONVERTER TODO TASK: Enums cannot contain methods in C++:
//								public String toString()
        //		{
        //			return value;
        //		}
    };

    /// <summary>
    /// Create a bondStereo from an array of 4 atoms. The 2nd and 3rd atoms of this array are connected via a double bond.
    /// The 1st and 4th atoms are at either end of this bond and indication is given as to whether they are cis or trans to each other. </summary>
    /// <param name="atomRefs4"> </param>
    /// <param name="cOrT"> </param>
public:
    BondStereo(std::vector<Atom *> &atomRefs4, BondStereoValue cOrT);

    virtual std::vector<Atom *> getAtomRefs4();

    virtual void setAtomRefs4(std::vector<Atom *> &atomRefs4);

    virtual BondStereoValue getBondStereoValue();

    virtual void setBondStereoValue(BondStereoValue bondStereoValue);
};

