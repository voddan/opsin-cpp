#pragma once

#include "ChemEl.h"
#include <string>
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Element;
class StructureBuildingException;


class IsotopeSpecificationParser {

private:
    static Pattern *const matchBoughtonIsotope = Pattern::compile(L"(?:-([^,]+(?:,[^,]+)*))?-d(\\d+)?");
    static Pattern *const matchIupacIsotope = Pattern::compile(L"(?:([^,]+(?:,[^,]+)*)-)?(\\d+)([A-Z][a-z]?)(\\d+)?");

public:
    class IsotopeSpecification {
    private:
        const ChemEl chemEl;
        const int isotope;
        const int multiplier;
        std::vector<std::wstring> const locants;

    public:
        IsotopeSpecification(ChemEl chemEl, int isotope, int multiplier, std::vector<std::wstring> &locants);

        virtual ChemEl getChemEl();

        virtual int getIsotope();

        virtual int getMultiplier();

        virtual std::vector<std::wstring> getLocants();
    };

public:
    static IsotopeSpecification *
    parseIsotopeSpecification(Element *isotopeSpecification) throw(StructureBuildingException);

private:
    static IsotopeSpecification *
    processBoughtonIsotope(Element *isotopeSpecification) throw(StructureBuildingException);

    static IsotopeSpecification *processIupacIsotope(Element *isotopeSpecification) throw(StructureBuildingException);
};

