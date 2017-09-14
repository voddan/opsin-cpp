#include "SortParses.h"
#include "Element.h"
#include "WordRule.h"
#include "OpsinTools.h"


//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.WORDRULE_ATR;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.WORDRULE_EL;

int SortParses::compare(Element *el1, Element *el2) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    bool isSubstituent1 = WordRule::substituent->toString()->equals(
            el1->getFirstChildElement(WORDRULE_EL)->getAttributeValue(WORDRULE_ATR));
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    bool isSubstituent2 = WordRule::substituent->toString()->equals(
            el2->getFirstChildElement(WORDRULE_EL)->getAttributeValue(WORDRULE_ATR));
    if (isSubstituent1 && !isSubstituent2) {
        return 1;
    }
    if (!isSubstituent1 && isSubstituent2) {
        return -1;
    }

    std::vector<int> counts1 = OpsinTools::countNumberOfElementsAndNumberOfChildLessElements(el1);
    std::vector<int> counts2 = OpsinTools::countNumberOfElementsAndNumberOfChildLessElements(el2);
    int childLessElementsInEl1 = counts1[1];
    int childLessElementsInEl2 = counts2[1];
    if (childLessElementsInEl1 > childLessElementsInEl2) {
        return 1;
    } else if (childLessElementsInEl1 < childLessElementsInEl2) {
        return -1;
    }

    int elementsInEl1 = counts1[0];
    int elementsInEl2 = counts2[0];
    if (elementsInEl1 > elementsInEl2) {
        return 1;
    } else if (elementsInEl1 < elementsInEl2) {
        return -1;
    } else {
        return 0;
    }
}
