#pragma once

#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Element;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.WORDRULE_ATR;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.WORDRULE_EL;


/// <summary>
/// Prefer non-substituent word rules to substituent word rule e.g. ethylene is C=C not -CC-
/// Prefer the parse with the least elements that have 0 children e.g. benzal beats benz al (1 childless element vs 2 childless elements)
/// Prefer less elements e.g. <acryl(acidStem)amide(suffix)> beats <acryl(substituent)><amide(group)>
/// </summary>
class SortParses : public Comparator<Element *> {
public:
    virtual int compare(Element *el1, Element *el2);
};
