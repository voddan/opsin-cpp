#include "IsotopeSpecificationParser.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "XmlDeclarations.h"


IsotopeSpecificationParser::IsotopeSpecification::IsotopeSpecification(ChemEl chemEl, int isotope, int multiplier,
                                                                       std::vector<std::wstring> &locants) : chemEl(
        chemEl), isotope(isotope), multiplier(multiplier), locants(locants) {
}

ChemEl IsotopeSpecificationParser::IsotopeSpecification::getChemEl() {
    return chemEl;
}

int IsotopeSpecificationParser::IsotopeSpecification::getIsotope() {
    return isotope;
}

int IsotopeSpecificationParser::IsotopeSpecification::getMultiplier() {
    return multiplier;
}

std::vector<std::wstring> IsotopeSpecificationParser::IsotopeSpecification::getLocants() {
    return locants;
}

IsotopeSpecificationParser::IsotopeSpecification *
IsotopeSpecificationParser::parseIsotopeSpecification(Element *isotopeSpecification) throw(StructureBuildingException) {
    std::wstring type = isotopeSpecification->getAttributeValue(XmlDeclarations::TYPE_ATR);
    if (XmlDeclarations::BOUGHTONSYSTEM_TYPE_VAL == type) {
        return processBoughtonIsotope(isotopeSpecification);
    } else if (XmlDeclarations::IUPACSYSTEM_TYPE_VAL == type) {
        return processIupacIsotope(isotopeSpecification);
    } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Unsupported isotope specification syntax");
        throw std::exception();
    }
}

IsotopeSpecificationParser::IsotopeSpecification *
IsotopeSpecificationParser::processBoughtonIsotope(Element *isotopeSpecification) throw(StructureBuildingException) {
    std::wstring val = isotopeSpecification->getValue();
    Matcher *m = matchBoughtonIsotope->matcher(val);
    if (!m->matches()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed isotope specification: " + val);
        throw std::exception();
    }
    ChemEl chemEl = ChemEl::H;
    int isotope = 2;

    int multiplier = 1;
    std::wstring multiplierStr = m->group(2);
    if (multiplierStr != L"") {
        multiplier = std::stoi(multiplierStr);
    }

    std::wstring locantsStr = m->group(1);
    std::vector<std::wstring> locants;
    if (locantsStr != L"") {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        locants = locantsStr.split(L",");
        if (locants.size() != multiplier) {
            throw StructureBuildingException(std::wstring(L"Mismatch between number of locants: ") + locants.size() +
                                             std::wstring(L" and number of hydrogen isotopes requested: ") +
                                             std::to_wstring(multiplier));
        }
    }
    return new IsotopeSpecification(chemEl, isotope, multiplier, locants);
}

IsotopeSpecificationParser::IsotopeSpecification *
IsotopeSpecificationParser::processIupacIsotope(Element *isotopeSpecification) throw(StructureBuildingException) {
    std::wstring val = isotopeSpecification->getValue();
    Matcher *m = matchIupacIsotope->matcher(val);
    if (!m->matches()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed isotope specification: " + val);
        throw std::exception();
    }

    int isotope = static_cast<Integer>(m->group(2));
    ChemEl chemEl = static_cast<ChemEl>(m->group(3));

    int multiplier = 1;
    std::wstring multiplierStr = m->group(4);
    if (multiplierStr != L"") {
        multiplier = std::stoi(multiplierStr);
    }

    std::wstring locantsStr = m->group(1);
    std::vector<std::wstring> locants;
    if (locantsStr != L"") {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        locants = locantsStr.split(L",");
        if (multiplierStr == L"") {
            multiplier = locants.size();
        } else if (locants.size() != multiplier) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            throw StructureBuildingException(std::wstring(L"Mismatch between number of locants: ") + locants.size() +
                                             std::wstring(L" and number of ") + chemEl::toString() +
                                             std::wstring(L" isotopes requested: ") + std::to_wstring(multiplier));
        }
    }
    return new IsotopeSpecification(chemEl, isotope, multiplier, locants);
}
