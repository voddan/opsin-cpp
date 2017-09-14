#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <boost/optional.hpp>
#include <set>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;
class Bond;
class StereoAnalyser;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.ELEMENTARYATOM_SUBTYPE_VAL;


class AmbiguityChecker {

public:
    static bool isSubstitutionAmbiguous(std::vector<Atom *> &substitutableAtoms, int numberToBeSubstituted);

    static bool allAtomsEquivalent(std::vector<Atom *> atoms);

    static bool allBondsEquivalent(Collection<Bond *> *bonds);

private:
    static std::wstring bondToCanonicalEnvironString(StereoAnalyser *analyser, Bond *b);

public:
    static std::wstring getAtomEnviron(StereoAnalyser *analyser, Atom *a);

private:
    static bool allAtomsConnectToDefaultInAtom(std::vector<Atom *> &substitutableAtoms, int numberToBeSubstituted);

public:
    static StereoAnalyser *analyseRelevantAtomsAndBonds(Collection<Atom *> *startingAtoms);

    static std::vector<Atom *>
    useAtomEnvironmentsToGivePlausibleSubstitution(std::vector<Atom *> &substitutableAtoms, int numberToBeSubstituted);

private:
    static std::vector<Atom *>
    findPlausibleSubstitutionPatternUsingSymmmetry(std::vector<Atom *> &substitutableAtoms, int numberToBeSubstituted);

    static std::vector<Atom *>
    findPlausibleSubstitutionPatternUsingLocalEnvironment(std::vector<Atom *> &substitutableAtoms,
                                                          int numberToBeSubstituted);
};

