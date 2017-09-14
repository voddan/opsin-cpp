#include "StructureBuilder.h"
#include "BuildState.h"
#include "Atom.h"
#include "Element.h"
#include "Fragment.h"
#include "StructureBuildingException.h"
#include "XmlDeclarations.h"
#include "OpsinTools.h"
#include "ChemEl.h"
#include "WordRule.h"
#include "WordType.h"
#include "StructureBuildingMethods.h"
#include "BuildResults.h"
#include "OutAtom.h"
#include "FragmentTools.h"
#include "StringTools.h"
#include "Bond.h"
#include "ValencyChecker.h"
#include "AmbiguityChecker.h"
#include "GroupingEl.h"
#include "StereoAnalyser.h"
#include "StereochemistryHandler.h"


using StereoBond = uk::ac::cam::ch::wwmm::opsin::StereoAnalyser::StereoBond;
using StereoCentre = uk::ac::cam::ch::wwmm::opsin::StereoAnalyser::StereoCentre;
using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;
//						import static uk.ac.cam.ch.wwmm.opsin.StructureBuildingMethods.*;

StructureBuilder::StructureBuilder(BuildState *state) : state(state) {
}

Fragment *StructureBuilder::buildFragment(Element *molecule) throw(StructureBuildingException) {
    std::vector<Element *> wordRules = molecule->getChildElements(XmlDeclarations::WORDRULE_EL);

    currentTopLevelWordRuleCount = wordRules.size();
    if (currentTopLevelWordRuleCount == 0) {
        throw StructureBuildingException(L"Molecule contains no word rules!?");
    }

    for (auto wordRule : wordRules) {
        processWordRuleChildrenThenRule(wordRule);
    }

    if (currentTopLevelWordRuleCount != wordRules.size()) {
        wordRules = molecule->getChildElements(
                XmlDeclarations::WORDRULE_EL); //very rarely a word rule adds a top level word rule
    }

    std::vector<Element *> groupElements = OpsinTools::getDescendantElementsWithTagName(molecule,
                                                                                        XmlDeclarations::GROUP_EL);
    processOxidoAndMethionineSpecialCases(groupElements);
    processOxidationNumbers(groupElements);
    state->fragManager->convertSpareValenciesToDoubleBonds();
    state->fragManager->checkValencies();

    manipulateStoichiometry(molecule, wordRules);

    state->fragManager->makeHydrogensExplicit();

    Fragment *uniFrag = state->fragManager->getUnifiedFragment();
    processStereochemistry(molecule, uniFrag);

    if (uniFrag->getOutAtomCount() > 0) {
        if (!state->n2sConfig->isAllowRadicals()) {
            throw StructureBuildingException(L"Radicals are currently set to not convert to structures");
        }
        if (state->n2sConfig->isOutputRadicalsAsWildCardAtoms()) {
            convertOutAtomsToAttachmentAtoms(uniFrag);
        }
    }

    if (polymerAttachmentPoints.size() > 0) {
        for (auto rAtom : polymerAttachmentPoints) {
            rAtom->setElement(ChemEl::R);
        }
        uniFrag->setPolymerAttachmentPoints(polymerAttachmentPoints);
    }
    return uniFrag;
}

void StructureBuilder::processWordRuleChildrenThenRule(Element *wordRule) throw(StructureBuildingException) {
    std::vector<Element *> wordRuleChildren = wordRule->getChildElements(XmlDeclarations::WORDRULE_EL);
    for (auto wordRuleChild : wordRuleChildren) {
        processWordRuleChildrenThenRule(wordRuleChild);
    }
    processWordRule(wordRule);
}

void StructureBuilder::processWordRule(Element *wordRuleEl) throw(StructureBuildingException) {
    WordRule wordRule = StringHelper::fromString<WordRule>(
            wordRuleEl->getAttributeValue(XmlDeclarations::WORDRULE_ATR));
    std::vector<Element *> words = OpsinTools::getChildElementsWithTagNames(wordRuleEl, std::vector<std::wstring>{
            XmlDeclarations::WORD_EL, XmlDeclarations::WORDRULE_EL});
    state->currentWordRule = wordRule;
    switch (wordRule) {
        case simple:
            for (auto word : words) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                if (!word->getName().equals(XmlDeclarations::WORD_EL) ||
                    !word->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(WordType::full->toString())) {
                    throw StructureBuildingException(L"OPSIN bug: Unexpected contents of 'simple' wordRule");
                }
                StructureBuildingMethods::resolveWordOrBracket(state, word);
            }
            break;
        case substituent:
            for (auto word : words) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                if (!word->getName().equals(XmlDeclarations::WORD_EL) ||
                    !word->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(WordType::substituent->toString()) ||
                    !state->n2sConfig->isAllowRadicals()) {
                    throw StructureBuildingException(L"OPSIN bug: Unexpected contents of 'substituent' wordRule");
                }
                StructureBuildingMethods::resolveWordOrBracket(state, word);
            }
            break;
        case ester:
        case multiEster:
            buildEster(words); //e.g. ethyl ethanoate, dimethyl terephthalate,  methyl propanamide
            break;
        case divalentFunctionalGroup:
            buildDiValentFunctionalGroup(words); // diethyl ether or methyl propyl ketone
            break;
        case monovalentFunctionalGroup:
            buildMonovalentFunctionalGroup(
                    words); // ethyl chloride, isophthaloyl dichloride, diethyl ether, ethyl alcohol
            break;
        case functionalClassEster:
            buildFunctionalClassEster(
                    words); //e.g. ethanoic acid ethyl ester, tetrathioterephthalic acid dimethyl ester
            break;
        case acidReplacingFunctionalGroup:
            //e.g. ethanoic acid ethyl amide, terephthalic acid dimethyl amide,
            //ethanoic acid amide, carbonic dihydrazide
            //already processed by the ComponentProcessor
            for (auto word : words) {
                StructureBuildingMethods::resolveWordOrBracket(state, word);
            }
            break;
        case oxide:
            buildOxide(
                    words); //e.g. styrene oxide, triphenylphosphane oxide, thianthrene 5,5-dioxide, propan-2-one oxide
            break;
        case carbonylDerivative:
            buildCarbonylDerivative(words); //e.g. Imidazole-2-carboxamide O-ethyloxime, pentan-3-one oxime
            break;
        case anhydride:
            buildAnhydride(words); //e.g. acetic anhydride
            break;
        case acidHalideOrPseudoHalide:
            buildAcidHalideOrPseudoHalide(words); //e.g. phosphinimidic chloride
            break;
        case additionCompound:
            buildAdditionCompound(words); //e.g. carbon tetrachloride
            break;
        case glycol:
            buildGlycol(words); //e.g. ethylene glycol
            break;
        case glycolEther:
            buildGlycolEther(words); //e.g. octaethyleneglycol monododecyl ether
            break;
        case acetal:
            buildAcetal(words); //e.g. propanal diethyl acetal
            break;
        case potentialAlcoholEster:
            //e.g. uridine 5'-(tetrahydrogen triphosphate)
            if (!buildAlcoholEster(words, currentTopLevelWordRuleCount)) {
                //should be processed as two "simple" wordrules if no hydroxy found, hence number of top level word rules may change
                //These simple word rules have already been processed
                splitAlcoholEsterRuleIntoTwoSimpleWordRules(words);
                currentTopLevelWordRuleCount++;
            }
            break;
        case cyclicPeptide:
            buildCyclicPeptide(words);
            break;
        case amineDiConjunctiveSuffix:
            //e.g. glycine N,N-diacetic acid
            buildAmineDiConjunctiveSuffix(words);
            break;
        case polymer:
            buildPolymer(words);
            break;
        default:
            throw StructureBuildingException(L"Unexpected Word Rule");
    }
}

void StructureBuilder::buildEster(std::vector<Element *> &words) throw(StructureBuildingException) {
    bool inSubstituents = true;
    BuildResults *substituentsBr = new BuildResults();
    std::vector<BuildResults *> ateGroups;
    std::unordered_map<BuildResults *, std::wstring> buildResultsToLocant; //typically locant will be null

    for (auto word : words) {
        StructureBuildingMethods::resolveWordOrBracket(state, word);
        BuildResults *br = new BuildResults(word);
        if (inSubstituents && br->getFunctionalAtomCount() > 0) {
            inSubstituents = false;
        }
        if (inSubstituents) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            if (!word->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(WordType::substituent->toString())) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                if (word->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(WordType::full->toString())) {
                    throw StructureBuildingException(L"bug? ate group did not have any functional atoms!");
                } else {
                    throw StructureBuildingException(
                            L"OPSIN bug: Non substituent word found where substituent expected in ester");
                }
            }
            int outAtomCount = br->getOutAtomCount();
            bool traditionalEster = false;
            for (int i = 0; i < outAtomCount; i++) {
                OutAtom *out = br->getOutAtom(i);
                if (out->getValency() > 1) {
                    FragmentTools::splitOutAtomIntoValency1OutAtoms(out);
                    traditionalEster = true;
                }
            }
            if (traditionalEster) { //e.g. ethylidene dipropanoate
                br = new BuildResults(word);
                outAtomCount = br->getOutAtomCount();
            }
            if (outAtomCount == 1) { //TODO add support for locanted terepthaloyl
                std::wstring locantForSubstituent = word->getAttributeValue(XmlDeclarations::LOCANT_ATR);
                if (locantForSubstituent != L"") {
                    br->getFirstOutAtom()->setLocant(
                            locantForSubstituent); //indexes which functional atom to connect to when there is a choice. Also can disambiguate which atom is a S in things like thioates
                }
            } else if (outAtomCount == 0) {
                throw StructureBuildingException(L"Substituent was expected to have at least one outAtom");
            }
            substituentsBr->mergeBuildResults(br);
        } else {
            std::wstring locant = word->getAttributeValue(
                    XmlDeclarations::LOCANT_ATR); //specifying a locant for an ateWord is very unusual as this information is typically redundant c.f. dodecamethylene 1,12-bis(chloroformate)
            if (br->getFunctionalAtomCount() < 1) {
                throw StructureBuildingException(L"bug? ate group did not have any functional atoms!");
            }
            ateGroups.push_back(br);
            buildResultsToLocant[br] = locant;
        }
    }
    if (ateGroups.empty()) {
        throw StructureBuildingException(L"OPSIN bug: Missing ate group in ester");
    }

    int outAtomCount = substituentsBr->getOutAtomCount();
    if (outAtomCount == 0) {
        throw StructureBuildingException(L"OPSIN bug: Missing outatom on ester substituents");
    }
    int esterIdCount = 0;
    for (auto br : ateGroups) {
        esterIdCount += br->getFunctionalAtomCount();
    }
    if (outAtomCount > esterIdCount) {
        throw StructureBuildingException(std::wstring(L"There are more radicals in the substituents(") + outAtomCount +
                                         std::wstring(L") than there are places to form esters(") + esterIdCount +
                                         std::wstring(L")"));
    }
    if (esterIdCount > outAtomCount && outAtomCount % ateGroups.size() != 0) {
        //actually checks if the same number of ester forming points would be used in each ate group e.g. ethyl diacetate is wrong
        throw StructureBuildingException(std::wstring(L"There are less radicals in the substituents(") + outAtomCount +
                                         std::wstring(L") than there are places to form esters(") + esterIdCount +
                                         std::wstring(L")"));
    }
    for (int i = 0; i < outAtomCount; i++) {
        BuildResults *ateBr = ateGroups[i % ateGroups.size()];
        Atom *ateAtom;
        if (substituentsBr->getFirstOutAtom()->getLocant() != L"") {
            ateAtom = determineFunctionalAtomToUse(substituentsBr->getFirstOutAtom()->getLocant(), ateBr);
        } else {
            ateAtom = ateBr->getFunctionalAtom(0);
            ateBr->removeFunctionalAtom(0);
        }
        std::wstring locant = buildResultsToLocant[ateBr];
        if (locant == L"") { //typical case
            Atom *atomOnSubstituentToUse = getOutAtomTakingIntoAccountWhetherSetExplicitly(substituentsBr, 0);
            state->fragManager->createBond(ateAtom, atomOnSubstituentToUse, 1);
            substituentsBr->removeOutAtom(0);
        } else {
            boost::optional<int> outAtomPosition = nullptr;
            for (int j = 0; j < substituentsBr->getOutAtomCount(); j++) {
                if (substituentsBr->getOutAtom(j)->getAtom()->hasLocant(locant)) {
                    outAtomPosition = j;
                    break;
                }
            }
            if (!outAtomPosition) {
                throw StructureBuildingException(std::wstring(L"Unable to find substituent with locant: ") + locant +
                                                 std::wstring(L" to form ester!"));
            }
            Atom *atomOnSubstituentToUse = substituentsBr->getOutAtom(outAtomPosition)->getAtom();
            state->fragManager->createBond(ateAtom, atomOnSubstituentToUse, 1);
            substituentsBr->removeOutAtom(outAtomPosition);
        }
        ateAtom->neutraliseCharge();
    }
}

void StructureBuilder::buildDiValentFunctionalGroup(std::vector<Element *> &words) throw(StructureBuildingException) {
    int wordIndice = 0;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (words[wordIndice]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::substituent->toString()) {
        throw StructureBuildingException(
                std::wstring(L"word: ") + wordIndice + std::wstring(L" was expected to be a substituent"));
    }
    StructureBuildingMethods::resolveWordOrBracket(state, words[wordIndice]);
    BuildResults *substituent1 = new BuildResults(words[wordIndice]);
    if (substituent1->getOutAtom(0)->getValency() != 1) {
        throw StructureBuildingException(std::wstring(L"OutAtom has unexpected valency. Expected 1. Actual: ") +
                                         std::to_wstring(substituent1->getOutAtom(0)->getValency()));
    }
    BuildResults *substituent2;
    if (substituent1->getOutAtomCount() == 2) { // e.g. tetramethylene sulfone
        if (substituent1->getOutAtom(1)->getValency() != 1) {
            throw StructureBuildingException(std::wstring(L"OutAtom has unexpected valency. Expected 1. Actual: ") +
                                             std::to_wstring(substituent1->getOutAtom(1)->getValency()));
        }
        substituent2 = substituent1;
    } else {
        if (substituent1->getOutAtomCount() != 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected one outAtom. Found ") + std::to_wstring(substituent1->getOutAtomCount()));
        }
        wordIndice++;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (words[wordIndice]->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
            WordType::functionalTerm->toString()) { //e.g. methyl sulfoxide rather than dimethyl sulfoxide
            Element *clone = state->fragManager->cloneElement(state, words[0]);
            OpsinTools::insertAfter(words[0], clone);
            words = words[0]->getParent().getChildElements();
        } else {
            StructureBuildingMethods::resolveWordOrBracket(state, words[wordIndice]);
        }
        substituent2 = new BuildResults(words[wordIndice]);
        if (substituent2->getOutAtomCount() != 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected one outAtom. Found ") + std::to_wstring(substituent2->getOutAtomCount()));
        }
        if (substituent2->getOutAtom(0)->getValency() != 1) {
            throw StructureBuildingException(std::wstring(L"OutAtom has unexpected valency. Expected 1. Actual: ") +
                                             std::to_wstring(substituent2->getOutAtom(0)->getValency()));
        }
    }
    wordIndice++;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (words[wordIndice] == nullptr ||
        words[wordIndice]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::functionalTerm->toString()) {
        throw StructureBuildingException(
                words[wordIndice]->getValue() + std::wstring(L" was expected to be a functionalTerm"));
    }
    std::vector<Element *> functionalGroup = OpsinTools::getDescendantElementsWithTagName(words[wordIndice],
                                                                                          XmlDeclarations::FUNCTIONALGROUP_EL);
    if (functionalGroup.size() != 1) {
        throw StructureBuildingException(
                L"Unexpected number of functionalGroups found, could be a bug in OPSIN's grammar");
    }
    std::wstring smilesOfGroup = functionalGroup[0]->getAttributeValue(XmlDeclarations::VALUE_ATR);
    Fragment *diValentGroup = state->fragManager->buildSMILES(smilesOfGroup, XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL,
                                                              XmlDeclarations::NONE_LABELS_VAL);

    Atom *outAtom1 = getOutAtomTakingIntoAccountWhetherSetExplicitly(substituent1, 0);
    substituent1->removeOutAtom(0);
    Atom *outAtom2 = getOutAtomTakingIntoAccountWhetherSetExplicitly(substituent2, 0);
    substituent2->removeOutAtom(0);
    if (diValentGroup->getOutAtomCount() == 1) { //c.f. peroxide where it is a linker
        state->fragManager->createBond(outAtom1, diValentGroup->getOutAtom(0)->getAtom(), 1);
        diValentGroup->removeOutAtom(0);
        state->fragManager->createBond(outAtom2, diValentGroup->getFirstAtom(), 1);
    } else {
        if (outAtom1 != outAtom2) { //general case
            state->fragManager->createBond(outAtom1, diValentGroup->getFirstAtom(), 1);
            state->fragManager->createBond(outAtom2, diValentGroup->getFirstAtom(), 1);
        } else { //e.g. carbonyl sulfide
            state->fragManager->createBond(outAtom1, diValentGroup->getFirstAtom(), 2);
        }
    }
    state->fragManager->incorporateFragment(diValentGroup, outAtom1->getFrag());
}

void StructureBuilder::buildMonovalentFunctionalGroup(std::vector<Element *> &words) throw(StructureBuildingException) {
    StructureBuildingMethods::resolveWordOrBracket(state, words[0]);
    std::vector<Element *> groups = OpsinTools::getDescendantElementsWithTagName(words[0], XmlDeclarations::GROUP_EL);
    for (auto group : groups) { //replaces outAtoms with valency greater than 1 with multiple outAtoms; e.g. ylidene -->diyl
        Fragment *frag = group->getFrag();
        for (int i = frag->getOutAtomCount() - 1; i >= 0; i--) {
            OutAtom *outAtom = frag->getOutAtom(i);
            if (outAtom->getValency() > 1) {
                FragmentTools::splitOutAtomIntoValency1OutAtoms(outAtom);
            }
        }
    }
    BuildResults *substituentBR = new BuildResults(words[0]);

    std::vector<Fragment *> functionalGroupFragments;
    for (int i = 1; i < words.size(); i++) {
        Element *functionalGroupWord = words[i];
        std::vector<Element *> functionalGroups = OpsinTools::getDescendantElementsWithTagName(functionalGroupWord,
                                                                                               XmlDeclarations::FUNCTIONALGROUP_EL);
        if (functionalGroups.size() != 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected exactly 1 functionalGroup. Found ") + functionalGroups.size());
        }

        Fragment *monoValentFunctionGroup = state->fragManager->buildSMILES(
                functionalGroups[0]->getAttributeValue(XmlDeclarations::VALUE_ATR),
                XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
        if (functionalGroups[0]->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
            XmlDeclarations::MONOVALENTSTANDALONEGROUP_TYPE_VAL) {
            Atom *ideAtom = monoValentFunctionGroup->getDefaultInAtomOrFirstAtom();
            ideAtom->addChargeAndProtons(1, 1); //e.g. make cyanide charge netural
        }
        Element *possibleMultiplier = OpsinTools::getPreviousSibling(functionalGroups[0]);
        functionalGroupFragments.push_back(monoValentFunctionGroup);
        if (possibleMultiplier != nullptr) {
            int multiplierValue = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            for (int j = 1; j < multiplierValue; j++) {
                functionalGroupFragments.push_back(state->fragManager->copyFragment(monoValentFunctionGroup));
            }
            possibleMultiplier->detach();
        }
    }

    int outAtomCount = substituentBR->getOutAtomCount();
    if (outAtomCount >
        functionalGroupFragments.size()) { //something like isophthaloyl chloride (more precisely written isophthaloyl dichloride)
        if (functionalGroupFragments.size() != 1) {
            throw StructureBuildingException(L"Incorrect number of functional groups found to balance outAtoms");
        }
        Fragment *monoValentFunctionGroup = functionalGroupFragments[0];
        for (int j = 1; j < outAtomCount; j++) {
            functionalGroupFragments.push_back(state->fragManager->copyFragment(monoValentFunctionGroup));
        }
    } else if (functionalGroupFragments.size() > outAtomCount) {
        throw StructureBuildingException(
                L"There are more function groups to attach than there are positions to attach them to!");
    }
    for (int i = 0; i < outAtomCount; i++) {
        Fragment *ideFrag = functionalGroupFragments[i];
        Atom *ideAtom = ideFrag->getDefaultInAtomOrFirstAtom();
        Atom *subAtom = getOutAtomTakingIntoAccountWhetherSetExplicitly(substituentBR, 0);
        state->fragManager->createBond(ideAtom, subAtom, 1);
        substituentBR->removeOutAtom(0);
        state->fragManager->incorporateFragment(ideFrag, subAtom->getFrag());
    }
}

void StructureBuilder::buildFunctionalClassEster(std::vector<Element *> &words) throw(StructureBuildingException) {
    Element *firstWord = words[0];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (firstWord->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::full->toString()) {
        throw StructureBuildingException(L"Don't alter wordRules.xml without checking the consequences!");
    }
    StructureBuildingMethods::resolveWordOrBracket(state, firstWord); //the group
    BuildResults *acidBr = new BuildResults(firstWord);

    if (acidBr->getFunctionalAtomCount() == 0) {
        throw StructureBuildingException(L"No functionalAtoms detected!");
    }

    int wordCountMinus1 = words.size() - 1;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (wordCountMinus1 < 2 ||
        words[wordCountMinus1]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::functionalTerm->toString()) {
        throw StructureBuildingException(
                L"OPSIN Bug: Bug in functionalClassEster rule; 'ester' not found where it was expected");
    }

    for (int i = 1; i < wordCountMinus1; i++) {
        Element *currentWord = words[i];
        std::wstring wordType = currentWord->getAttributeValue(XmlDeclarations::TYPE_ATR);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (wordType != WordType::substituent->toString()) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            if (wordType == WordType::functionalTerm->toString() &&
                currentWord->getAttributeValue(XmlDeclarations::VALUE_ATR).equalsIgnoreCase(L"ester")) {
                //superfluous ester word
                continue;
            }
            throw StructureBuildingException(
                    std::wstring(L"OPSIN Bug: Bug in functionalClassEster rule; Encountered: ") +
                    currentWord->getAttributeValue(XmlDeclarations::VALUE_ATR));
        }
        StructureBuildingMethods::resolveWordOrBracket(state, currentWord);
        BuildResults *substituentBr = new BuildResults(currentWord);
        int outAtomCount = substituentBr->getOutAtomCount();
        if (acidBr->getFunctionalAtomCount() < outAtomCount) {
            throw StructureBuildingException(L"Insufficient functionalAtoms on acid");
        }
        for (int j = 0; j < outAtomCount; j++) {
            std::wstring locantForSubstituent = currentWord->getAttributeValue(XmlDeclarations::LOCANT_ATR);
            Atom *functionalAtom;
            if (locantForSubstituent != L"") {
                functionalAtom = determineFunctionalAtomToUse(locantForSubstituent, acidBr);
            } else {
                functionalAtom = acidBr->getFunctionalAtom(0);
                acidBr->removeFunctionalAtom(0);
            }
            if (substituentBr->getOutAtom(j)->getValency() != 1) {
                throw StructureBuildingException(
                        L"Substituent was expected to have only have an outgoing valency of 1");
            }
            state->fragManager->createBond(functionalAtom,
                                           getOutAtomTakingIntoAccountWhetherSetExplicitly(substituentBr, j), 1);
            if (functionalAtom->getCharge() == -1) {
                functionalAtom->neutraliseCharge();
            }
        }
        substituentBr->removeAllOutAtoms();
    }
}

void StructureBuilder::buildOxide(std::vector<Element *> &words) throw(StructureBuildingException) {
    StructureBuildingMethods::resolveWordOrBracket(state, words[0]); //the group
    std::vector<Fragment *> oxideFragments;
    std::vector<std::wstring> locantsForOxide; //often not specified
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (words[1]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::functionalTerm->toString()) {
        throw StructureBuildingException(L"Oxide functional term not found where expected!");
    }
    Element *rightMostGroup;
    if (words[0]->getName() == XmlDeclarations::WORDRULE_EL) { //e.g. Nicotinic acid N-oxide
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        std::vector<Element *> fullWords = OpsinTools::getDescendantElementsWithTagNameAndAttribute(words[0],
                                                                                                    XmlDeclarations::WORD_EL,
                                                                                                    XmlDeclarations::TYPE_ATR,
                                                                                                    WordType::full->toString());
        if (fullWords.empty()) {
            throw StructureBuildingException(
                    L"OPSIN is entirely unsure where the oxide goes so has decided not to guess");
        }
        rightMostGroup = StructureBuildingMethods::findRightMostGroupInBracket(fullWords[fullWords.size() - 1]);
    } else {
        rightMostGroup = StructureBuildingMethods::findRightMostGroupInBracket(words[0]);
    }

    int numberOfOxygenToAdd = 1;
    std::vector<Element *> multipliers = OpsinTools::getDescendantElementsWithTagName(words[1],
                                                                                      XmlDeclarations::MULTIPLIER_EL);
    if (multipliers.size() > 1) {
        throw StructureBuildingException(std::wstring(L"Expected 0 or 1 multiplier found: ") + multipliers.size());
    }
    if (multipliers.size() == 1) {
        numberOfOxygenToAdd = std::stoi(multipliers[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
        multipliers[0]->detach();
    } else {
        if (XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL ==
            rightMostGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
            Atom *elementaryAtom = rightMostGroup->getFrag()->getFirstAtom();
            int charge = elementaryAtom->getCharge();
            if (charge > 0 && charge % 2 == 0) {
                numberOfOxygenToAdd = charge / 2;
            } else if (elementaryAtom->getProperty(Atom::OXIDATION_NUMBER) != nullptr) {
                int valency =
                        elementaryAtom->getProperty(Atom::OXIDATION_NUMBER) - elementaryAtom->getIncomingValency();
                if (valency > 0 && valency % 2 == 0) {
                    numberOfOxygenToAdd = valency / 2;
                }
            }
        }
    }
    std::vector<Element *> functionalGroup = OpsinTools::getDescendantElementsWithTagName(words[1],
                                                                                          XmlDeclarations::FUNCTIONALGROUP_EL);
    if (functionalGroup.size() != 1) {
        throw StructureBuildingException(std::wstring(L"Expected 1 group element found: ") + functionalGroup.size());
    }
    std::wstring smilesReplacement = functionalGroup[0]->getAttributeValue(XmlDeclarations::VALUE_ATR);
    std::wstring labels = functionalGroup[0]->getAttributeValue(XmlDeclarations::LABELS_ATR);
    for (int i = 0; i < numberOfOxygenToAdd; i++) {
        oxideFragments.push_back(
                state->fragManager->buildSMILES(smilesReplacement, XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL,
                                                labels != L"" ? labels : XmlDeclarations::NONE_LABELS_VAL));
    }
    std::vector<Element *> locantEls = OpsinTools::getDescendantElementsWithTagName(words[1],
                                                                                    XmlDeclarations::LOCANT_EL);
    if (locantEls.size() > 1) {
        throw StructureBuildingException(std::wstring(L"Expected 0 or 1 locant elements found: ") + locantEls.size());
    }
    if (locantEls.size() == 1) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> locants = StringTools::removeDashIfPresent(locantEls[0]->getValue()).split(L",");
        locantsForOxide.addAll(Arrays::asList(locants));
        locantEls[0]->detach();
    }
    if (!locantsForOxide.empty() && locantsForOxide.size() != oxideFragments.size()) {
        throw StructureBuildingException(L"Mismatch between number of locants and number of oxides specified");
    }
    std::vector<Fragment *> orderedPossibleFragments; //In preference suffixes are substituted onto e.g. acetonitrile oxide
    std::vector<Element *> suffixEls = rightMostGroup->getParent()->getChildElements(XmlDeclarations::SUFFIX_EL);
    for (int i = suffixEls.size() - 1; i >= 0; i--) { //suffixes (if any) from right to left
        Element *suffixEl = suffixEls[i];
        Fragment *suffixFrag = suffixEl->getFrag();
        if (suffixFrag != nullptr) {
            orderedPossibleFragments.push_back(suffixFrag);
        }
    }
    Fragment *groupToModify = rightMostGroup->getFrag(); //all the suffixes are actually part of this fragment already
    orderedPossibleFragments.push_back(groupToModify);
    for (int i = 0; i < oxideFragments.size(); i++) {
        Atom *oxideAtom = oxideFragments[i]->getFirstAtom();
        if (!locantsForOxide.empty()) {
            Atom *atomToAddOxideTo = groupToModify->getAtomByLocantOrThrow(locantsForOxide[i]);
            if (atomToAddOxideTo->getElement() == ChemEl::C &&
                XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL != groupToModify->getSubType()) {
                throw StructureBuildingException(std::wstring(L"Locant ") + locantsForOxide[i] + std::wstring(
                        L" indicated oxide applied to carbon, but this would lead to hypervalency!"));
            }
            formAppropriateBondToOxideAndAdjustCharges(atomToAddOxideTo, oxideAtom);
        } else {
            for (auto frag : orderedPossibleFragments) {
                std::wstring subTypeVal = frag->getSubType();
                if (XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL == subTypeVal) {
                    Atom *elementaryAtom = frag->getFirstAtom();
                    formAppropriateBondToOxideAndAdjustCharges(elementaryAtom, oxideAtom); //e.g. carbon dioxide
                    int chargeOnAtom = elementaryAtom->getCharge();
                    if (chargeOnAtom >= 2) {
                        elementaryAtom->setCharge(chargeOnAtom - 2);
                    }
                    goto mainLoopContinue;
                } else {
                    std::vector<Atom *> atomList = frag->getAtomList();
                    for (auto atom : atomList) {
                        if (atom->getElement() != ChemEl::C && atom->getElement() != ChemEl::O) {
                            formAppropriateBondToOxideAndAdjustCharges(atom, oxideAtom);
                            goto mainLoopContinue;
                        }
                    }
                }
            }
            //No heteroatoms could be found. Perhaps it's supposed to be something like styrene oxide
            Set < Bond * > *bondSet = groupToModify->getBondSet(); //looking for double bond
            for (auto bond : bondSet) {
                if (bond->getOrder() == 2 && bond->getFromAtom()->getElement() == ChemEl::C &&
                    bond->getToAtom()->getElement() == ChemEl::C) {
                    bond->setOrder(1);
                    state->fragManager->createBond(bond->getFromAtom(), oxideAtom, 1);
                    state->fragManager->createBond(bond->getToAtom(), oxideAtom, 1);
                    goto mainLoopContinue;
                }
            }

            //...or maybe something a bit iffy nomenclature wise like benzene oxide :-S
            for (auto bond : bondSet) {
                Atom *fromAtom = bond->getFromAtom();
                Atom *toAtom = bond->getToAtom();
                if (fromAtom->hasSpareValency() && toAtom->hasSpareValency() && fromAtom->getElement() == ChemEl::C &&
                    toAtom->getElement() == ChemEl::C) {
                    fromAtom->setSpareValency(false);
                    toAtom->setSpareValency(false);
                    state->fragManager->createBond(fromAtom, oxideAtom, 1);
                    state->fragManager->createBond(toAtom, oxideAtom, 1);
                    goto mainLoopContinue;
                }
            }
            for (auto frag : orderedPossibleFragments) { //something like where oxide goes on an oxygen propan-2-one oxide
                std::vector<Atom *> atomList = frag->getAtomList();
                for (auto atom : atomList) {
                    if (atom->getElement() != ChemEl::C) {
                        formAppropriateBondToOxideAndAdjustCharges(atom, oxideAtom);
                        goto mainLoopContinue;
                    }
                }
            }
            throw StructureBuildingException(L"Unable to find suitable atom or a double bond to add oxide to");
        }
        mainLoopContinue:
    }
    mainLoopBreak:
    for (auto oxide : oxideFragments) {
        state->fragManager->incorporateFragment(oxide, groupToModify);
    }
}

void StructureBuilder::formAppropriateBondToOxideAndAdjustCharges(Atom *atomToAddOxideTo,
                                                                  Atom *oxideAtom) throw(StructureBuildingException) {
    boost::optional<int> maxVal = ValencyChecker::getMaximumValency(atomToAddOxideTo->getElement(),
                                                                    atomToAddOxideTo->getCharge());
    if (!maxVal || (atomToAddOxideTo->getIncomingValency() + atomToAddOxideTo->getOutValency() + 2) <= maxVal) {
        if (!atomToAddOxideTo->getLambdaConventionValency() ||
            !ValencyChecker::checkValencyAvailableForBond(atomToAddOxideTo,
                                                          2)) { //probably in well formed names 2 protons should always be added but some names use the lambdaConvention to specify the valency after oxide has been applied
            atomToAddOxideTo->addChargeAndProtons(0, 2); //this is an additive operation, up the proton count by 2
        }
        state->fragManager->createBond(atomToAddOxideTo, oxideAtom, 2);
    } else {
        if (atomToAddOxideTo->getCharge() != 0 || oxideAtom->getCharge() != 0) {
            throw StructureBuildingException(
                    L"Oxide appeared to refer to an atom that has insufficent valency to accept the addition of oxygen");
        }
        atomToAddOxideTo->addChargeAndProtons(1, 1);
        oxideAtom->addChargeAndProtons(-1, -1);
        maxVal = ValencyChecker::getMaximumValency(atomToAddOxideTo->getElement(), atomToAddOxideTo->getCharge());
        if (maxVal && (atomToAddOxideTo->getIncomingValency() + atomToAddOxideTo->getOutValency() + 1) > maxVal) {
            throw StructureBuildingException(
                    L"Oxide appeared to refer to an atom that has insufficent valency to accept the addition of oxygen");
        }
        state->fragManager->createBond(atomToAddOxideTo, oxideAtom, 1);
    }
}

void StructureBuilder::buildCarbonylDerivative(std::vector<Element *> &words) throw(StructureBuildingException) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (!WordType::full->toString()->equals(words[0]->getAttributeValue(XmlDeclarations::TYPE_ATR))) {
        throw StructureBuildingException(
                L"OPSIN bug: Wrong word type encountered when applying carbonylDerivative wordRule");
    }
    std::vector<Fragment *> replacementFragments;
    std::vector<std::wstring> locantForFunctionalTerm; //usually not specified
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (words[1]->getAttributeValue(XmlDeclarations::TYPE_ATR) !=
        WordType::functionalTerm->toString()) { //e.g. acetone O-ethyloxime or acetone 1-chloro-1-methylhydrazone
        for (int i = 1; i < words.size(); i++) {
            Fragment *frag = findRightMostGroupInWordOrWordRule(words[i])->getFrag();
            replacementFragments.push_back(frag);
            std::vector<Element *> children = words[i]->getChildElements();
            if (children.size() == 1 && children[0]->getName() == XmlDeclarations::BRACKET_EL &&
                children[0]->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr) {
                locantForFunctionalTerm.push_back(children[0]->getAttributeValue(XmlDeclarations::LOCANT_ATR));
            } else if (children.size() == 2 && children[0]->getAttribute(XmlDeclarations::LOCANT_ATR) != nullptr) {
                std::wstring locant = children[0]->getAttributeValue(XmlDeclarations::LOCANT_ATR);
                if (children[1]->getName() == XmlDeclarations::ROOT_EL && !frag->hasLocant(locant) &&
                    OpsinTools::MATCH_NUMERIC_LOCANT->matcher(
                            locant).matches()) { //e.g. 1,3-benzothiazole-2-carbaldehyde 2-phenylhydrazone
                    locantForFunctionalTerm.push_back(children[0]->getAttributeValue(XmlDeclarations::LOCANT_ATR));
                    children[0]->removeAttribute(children[0]->getAttribute(XmlDeclarations::LOCANT_ATR));
                }
            }
        }
    } else { //e.g. butan-2,3-dione dioxime or hexan2,3-dione 2-oxime
        int numberOfCarbonylReplacements = 1;
        std::vector<Element *> multipliers = OpsinTools::getDescendantElementsWithTagName(words[1],
                                                                                          XmlDeclarations::MULTIPLIER_EL);
        if (multipliers.size() > 1) {
            throw StructureBuildingException(std::wstring(L"Expected 0 or 1 multiplier found: ") + multipliers.size());
        }
        if (multipliers.size() == 1) {
            numberOfCarbonylReplacements = std::stoi(multipliers[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
            multipliers[0]->detach();
        }
        std::vector<Element *> functionalGroup = OpsinTools::getDescendantElementsWithTagName(words[1],
                                                                                              XmlDeclarations::FUNCTIONALGROUP_EL);
        if (functionalGroup.size() != 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected 1 functionalGroup element found: ") + functionalGroup.size());
        }
        std::wstring smilesReplacement = functionalGroup[0]->getAttributeValue(XmlDeclarations::VALUE_ATR);
        std::wstring labels = functionalGroup[0]->getAttributeValue(XmlDeclarations::LABELS_ATR);
        for (int i = 0; i < numberOfCarbonylReplacements; i++) {
            Fragment *replacementFragment = state->fragManager->buildSMILES(smilesReplacement,
                                                                            XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL,
                                                                            labels != L"" ? labels
                                                                                          : XmlDeclarations::NONE_LABELS_VAL);
            if (i > 0) {
                FragmentTools::relabelLocants(replacementFragment->getAtomList(), StringTools::multiplyString(L"'", i));
            }
            std::vector<Atom *> atomList = replacementFragment->getAtomList();
            for (auto atom : atomList) {
                atom->removeLocantsOtherThanElementSymbolLocants(); //prevents numeric locant locanted substitution from outside the functional word
            }
            replacementFragments.push_back(replacementFragment);
        }
        std::vector<Element *> locantEls = OpsinTools::getDescendantElementsWithTagName(words[1],
                                                                                        XmlDeclarations::LOCANT_EL);
        if (locantEls.size() > 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected 0 or 1 locant elements found: ") + locantEls.size());
        }
        if (locantEls.size() == 1) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            std::vector<std::wstring> locants = StringTools::removeDashIfPresent(locantEls[0]->getValue()).split(L",");
            locantForFunctionalTerm.addAll(Arrays::asList(locants));
            locantEls[0]->detach();
        }
    }
    if (!locantForFunctionalTerm.empty() && locantForFunctionalTerm.size() != replacementFragments.size()) {
        throw StructureBuildingException(L"Mismatch between number of locants and number of carbonyl replacements");
    }

    Element *rightMostGroup = findRightMostGroupInWordOrWordRule(words[0]);
    Element *parent = rightMostGroup->getParent();
    bool multiplied = false;
    while (!parent->equals(words[0])) {
        if (parent->getAttribute(XmlDeclarations::MULTIPLIER_ATR) != nullptr) {
            multiplied = true;
        }
        parent = parent->getParent();
    }
    if (!multiplied) {
        std::vector<Atom *> carbonylOxygens = findCarbonylOxygens(rightMostGroup->getFrag(), locantForFunctionalTerm);
        int replacementsToPerform = std::min(replacementFragments.size(), carbonylOxygens.size());
        replaceCarbonylOxygenWithReplacementFragments(words, replacementFragments, carbonylOxygens,
                                                      replacementsToPerform);
    }

    StructureBuildingMethods::resolveWordOrBracket(state, words[0]); //the component
    if (replacementFragments.size() > 0) {
        //Note that the right most group may be multiplied e.g. 3,3'-methylenebis(2,4,6-trimethylbenzaldehyde) disemicarbazone
        //or the carbonyl may not even be on the right most group e.g.  4-oxocyclohexa-2,5-diene-1-carboxylic acid 4-oxime
        BuildResults *br = new BuildResults(words[0]);
        std::vector<Atom *> carbonylOxygens;
        std::vector<Fragment *> fragments(br->getFragments());
        for (ListIterator < Fragment * > *iterator = fragments.listIterator(
                fragments.size()); iterator->hasPrevious();) { //iterate in reverse order - right most groups preferred
            carbonylOxygens.addAll(findCarbonylOxygens(iterator->previous(), locantForFunctionalTerm));
        }
        replaceCarbonylOxygenWithReplacementFragments(words, replacementFragments, carbonylOxygens,
                                                      replacementFragments.size());
    }
}

void StructureBuilder::replaceCarbonylOxygenWithReplacementFragments(std::vector<Element *> &words,
                                                                     std::vector<Fragment *> &replacementFragments,
                                                                     std::vector<Atom *> &carbonylOxygens,
                                                                     int functionalReplacementsToPerform) throw(StructureBuildingException) {
    if (functionalReplacementsToPerform > carbonylOxygens.size()) {
        throw StructureBuildingException(L"Insufficient carbonyl groups found!");
    }
    for (int i = 0; i < functionalReplacementsToPerform; i++) {
        Atom *carbonylOxygen = carbonylOxygens.erase(carbonylOxygens.begin()); //the oxygen of the carbonyl
        Fragment *carbonylFrag = carbonylOxygen->getFrag();
        Fragment *replacementFrag = replacementFragments.erase(replacementFragments.begin());
        std::vector<Atom *> atomList = replacementFrag->getAtomList();
        if (atomList.size() == 2) {
            //special case for oxime
            //adds a locant like O1 giving another way of referencing this atom
            Atom *numericLocantAtomConnectedToCarbonyl = OpsinTools::depthFirstSearchForAtomWithNumericLocant(
                    carbonylOxygen);
            if (numericLocantAtomConnectedToCarbonyl != nullptr) {
                Atom *lastatom = atomList[1];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                lastatom->addLocant(lastatom->getElement()
                ::toString() + numericLocantAtomConnectedToCarbonyl->getFirstLocant());
            }
        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (words[1]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::functionalTerm->toString()) {
            StructureBuildingMethods::resolveWordOrBracket(state, words[1 + i]);
        }
        for (auto atom : atomList) {
            atom->removeLocantsOtherThanElementSymbolLocants(); //prevents numeric locant locanted substitution from outside the functional word
            std::vector<std::wstring> locants = atom->getLocants();
            for (int j = locants.size() - 1; j >= 0; j--) {
                std::wstring locant = locants[j];
                if (carbonylFrag->hasLocant(locant)) {
                    atom->removeLocant(locant);
                }
            }
        }
        if (replacementFrag->getOutAtomCount() != 1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Carbonyl replacement fragment expected to have one outatom");
            throw std::exception();
        }
        Atom *atomToReplaceCarbonylOxygen = replacementFrag->getOutAtom(0)->getAtom();
        replacementFrag->removeOutAtom(0);
        state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(carbonylOxygen,
                                                                             atomToReplaceCarbonylOxygen);
        atomToReplaceCarbonylOxygen->setType(
                carbonylOxygen->getType()); //copy the type e.g. if the carbonyl was a suffix this should appear as a suffix
        if (replacementFrag->getTokenEl()->getParent() ==
            nullptr) { //incorporate only for the case that replacementFrag came from a functional class element
            state->fragManager->incorporateFragment(replacementFrag, carbonylFrag);
        }
    }
}

std::vector<Atom *> StructureBuilder::findCarbonylOxygens(Fragment *fragment,
                                                          std::vector<std::wstring> &locantForCarbonylAtom) throw(StructureBuildingException) {
    std::vector<Atom *> matches;
    std::vector<Atom *> rootFragAtomList = fragment->getAtomList();
    for (auto atom : rootFragAtomList) { //find all carbonyl oxygen
        if (atom->getElement() == ChemEl::O && atom->getCharge() == 0) {
            std::vector<Atom *> neighbours = atom->getAtomNeighbours();
            if (neighbours.size() == 1) {
                if (neighbours[0]->getElement() == ChemEl::C) {
                    if (!locantForCarbonylAtom.empty()) {
                        Atom *numericLocantAtomConnectedToCarbonyl = OpsinTools::depthFirstSearchForAtomWithNumericLocant(
                                atom);
                        if (numericLocantAtomConnectedToCarbonyl !=
                            nullptr) { //could be the carbon of the carbonyl or the ring the carbonyl connects to in say a carbaldehyde
                            bool matchesLocant = false;
                            for (auto locant : locantForCarbonylAtom) {
                                if (numericLocantAtomConnectedToCarbonyl->hasLocant(locant)) {
                                    matchesLocant = true;
                                }
                            }
                            if (!matchesLocant) {
                                continue;
                            }
                        } else {
                            continue;
                        }
                    }
                    Bond *b = atom->getBondToAtomOrThrow(neighbours[0]);
                    if (b->getOrder() == 2) {
                        matches.push_back(atom);
                    }
                }
            }
        }
    }
    return matches;
}

void StructureBuilder::buildAnhydride(std::vector<Element *> &words) throw(StructureBuildingException) {
    if (words.size() != 2 && words.size() != 3) {
        throw StructureBuildingException(
                L"Unexpected number of words in anhydride. Check wordRules.xml, this is probably a bug");
    }
    Element *anhydrideWord = words[words.size() - 1];
    std::vector<Element *> functionalClass = OpsinTools::getDescendantElementsWithTagName(anhydrideWord,
                                                                                          XmlDeclarations::FUNCTIONALGROUP_EL);
    if (functionalClass.size() != 1) {
        throw StructureBuildingException(std::wstring(L"Expected 1 group element found: ") + functionalClass.size());
    }
    std::wstring anhydrideSmiles = functionalClass[0]->getAttributeValue(XmlDeclarations::VALUE_ATR);
    int numberOfAnhydrideLinkages = 1;
    std::vector<Element *> multipliers = OpsinTools::getDescendantElementsWithTagName(anhydrideWord,
                                                                                      XmlDeclarations::MULTIPLIER_EL);
    if (multipliers.size() > 1) {
        throw StructureBuildingException(std::wstring(L"Expected 0 or 1 multiplier found: ") + multipliers.size());
    }
    if (multipliers.size() == 1) {
        numberOfAnhydrideLinkages = std::stoi(multipliers[0]->getAttributeValue(XmlDeclarations::VALUE_ATR));
        multipliers[0]->detach();
    }
    std::wstring anhydrideLocant = L"";
    std::vector<Element *> anhydrideLocants = OpsinTools::getDescendantElementsWithTagNames(anhydrideWord,
                                                                                            std::vector<std::wstring>{
                                                                                                    XmlDeclarations::LOCANT_EL,
                                                                                                    XmlDeclarations::COLONORSEMICOLONDELIMITEDLOCANT_EL});
    if (anhydrideLocants.size() > 1) {
        throw StructureBuildingException(
                std::wstring(L"Expected 0 or 1 anhydrideLocants found: ") + anhydrideLocants.size());
    }
    if (anhydrideLocants.size() == 1) {
        anhydrideLocant = anhydrideLocants[0]->getValue();
        anhydrideLocants[0]->detach();
    }
    StructureBuildingMethods::resolveWordOrBracket(state, words[0]);
    BuildResults *br1 = new BuildResults(words[0]);
    if (br1->getFunctionalAtomCount() == 0) {
        throw StructureBuildingException(L"Cannot find functionalAtom to form anhydride");
    }
    if (words.size() == 3) { //asymmetric anhydride
        if (anhydrideLocant != L"") {
            throw StructureBuildingException(L"Unsupported or invalid anhydride");
        }
        StructureBuildingMethods::resolveWordOrBracket(state, words[1]);
        BuildResults *br2 = new BuildResults(words[1]);
        if (br2->getFunctionalAtomCount() == 0) {
            throw StructureBuildingException(L"Cannot find functionalAtom to form anhydride");
        }
        if (numberOfAnhydrideLinkages > 1) {
            for (int i = numberOfAnhydrideLinkages - 1; i >= 0; i--) {
                if (br2->getFunctionalAtomCount() == 0) {
                    throw StructureBuildingException(L"Cannot find functionalAtom to form anhydride");
                }
                BuildResults *newAcidBr;
                if (i != 0) {
                    Element *newAcid = state->fragManager->cloneElement(state, words[0]);
                    OpsinTools::insertAfter(words[0], newAcid);
                    newAcidBr = new BuildResults(newAcid);
                } else {
                    newAcidBr = br1;
                }
                formAnhydrideLink(anhydrideSmiles, newAcidBr, br2);
            }

        } else {
            if (br1->getFunctionalAtomCount() != 1 && br2->getFunctionalAtomCount() != 1) {
                throw StructureBuildingException(L"Invalid anhydride description");
            }
            formAnhydrideLink(anhydrideSmiles, br1, br2);
        }
    } else { //symmetric anhydride
        if (br1->getFunctionalAtomCount() > 1) { //cyclic anhydride
            if (br1->getFunctionalAtomCount() == 2) {
                if (numberOfAnhydrideLinkages != 1 || anhydrideLocant != L"") {
                    throw StructureBuildingException(L"Unsupported or invalid anhydride");
                }
                formAnhydrideLink(anhydrideSmiles, br1, br1);
            } else { //cyclic anhydride where group has more than 2 acids
                if (anhydrideLocant == L"") {
                    throw StructureBuildingException(
                            L"Anhydride formation appears to be ambiguous; More than 2 acids, no locants");
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                std::vector<std::wstring> acidLocants = OpsinTools::MATCH_COLONORSEMICOLON->split(
                        StringTools::removeDashIfPresent(anhydrideLocant));
                if (acidLocants.size() != numberOfAnhydrideLinkages) {
                    throw StructureBuildingException(
                            L"Mismatch between number of locants and number of anhydride linkages to form");
                }
                if (br1->getFunctionalAtomCount() < (numberOfAnhydrideLinkages * 2)) {
                    throw StructureBuildingException(
                            L"Mismatch between number of acid atoms and number of anhydride linkages to form");
                }
                std::vector<Atom *> functionalAtoms;
                for (int i = 0; i < br1->getFunctionalAtomCount(); i++) {
                    functionalAtoms.push_back(br1->getFunctionalAtom(i));
                }

                for (int i = 0; i < numberOfAnhydrideLinkages; i++) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                    std::vector<std::wstring> locants = acidLocants[i].split(L",");
                    Atom *oxygen1 = nullptr;
                    for (int j = functionalAtoms.size() - 1; j >= 0; j--) {
                        Atom *functionalAtom = functionalAtoms[j];
                        Atom *numericLocantAtomConnectedToFunctionalAtom = OpsinTools::depthFirstSearchForAtomWithNumericLocant(
                                functionalAtom);
                        if (numericLocantAtomConnectedToFunctionalAtom->hasLocant(locants[0])) {
                            oxygen1 = functionalAtom;
                            functionalAtoms.erase(functionalAtoms.begin() + j);
                            break;
                        }
                    }
                    Atom *oxygen2 = nullptr;
                    for (int j = functionalAtoms.size() - 1; j >= 0; j--) {
                        Atom *functionalAtom = functionalAtoms[j];
                        Atom *numericLocantAtomConnectedToFunctionalAtom = OpsinTools::depthFirstSearchForAtomWithNumericLocant(
                                functionalAtom);
                        if (numericLocantAtomConnectedToFunctionalAtom->hasLocant(locants[1])) {
                            oxygen2 = functionalAtom;
                            functionalAtoms.erase(functionalAtoms.begin() + j);
                            break;
                        }
                    }
                    if (oxygen1 == nullptr || oxygen2 == nullptr) {
                        throw StructureBuildingException(L"Unable to find locanted atom for anhydride formation");
                    }
                    formAnhydrideLink(anhydrideSmiles, oxygen1, oxygen2);
                }
            }
        } else {
            if (numberOfAnhydrideLinkages != 1 || anhydrideLocant != L"") {
                throw StructureBuildingException(L"Unsupported or invalid anhydride");
            }
            Element *newAcid = state->fragManager->cloneElement(state, words[0]);
            OpsinTools::insertAfter(words[0], newAcid);
            BuildResults *br2 = new BuildResults(newAcid);
            formAnhydrideLink(anhydrideSmiles, br1, br2);
        }
    }
}

void StructureBuilder::formAnhydrideLink(const std::wstring &anhydrideSmiles, BuildResults *acidBr1,
                                         BuildResults *acidBr2) throw(StructureBuildingException) {
    Atom *oxygen1 = acidBr1->getFunctionalAtom(0);
    acidBr1->removeFunctionalAtom(0);
    Atom *oxygen2 = acidBr2->getFunctionalAtom(0);
    acidBr2->removeFunctionalAtom(0);
    formAnhydrideLink(anhydrideSmiles, oxygen1, oxygen2);
}

void StructureBuilder::formAnhydrideLink(const std::wstring &anhydrideSmiles, Atom *oxygen1,
                                         Atom *oxygen2) throw(StructureBuildingException) {
    if (oxygen1->getElement() != ChemEl::O || oxygen2->getElement() != ChemEl::O || oxygen1->getBondCount() != 1 ||
        oxygen2->getBondCount() != 1) {
        throw StructureBuildingException(L"Problem building anhydride");
    }
    Atom *atomOnSecondAcidToConnectTo = oxygen2->getAtomNeighbours()[0];
    state->fragManager->removeAtomAndAssociatedBonds(oxygen2);
    Fragment *anhydride = state->fragManager->buildSMILES(anhydrideSmiles, XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL,
                                                          XmlDeclarations::NONE_LABELS_VAL);
    Fragment *acidFragment1 = oxygen1->getFrag();
    state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(oxygen1, anhydride->getFirstAtom());
    std::vector<Atom *> atomsInAnhydrideLinkage = anhydride->getAtomList();
    state->fragManager->createBond(atomsInAnhydrideLinkage[atomsInAnhydrideLinkage.size() - 1],
                                   atomOnSecondAcidToConnectTo, 1);
    state->fragManager->incorporateFragment(anhydride, acidFragment1);
}

void StructureBuilder::buildAcidHalideOrPseudoHalide(std::vector<Element *> &words) throw(StructureBuildingException) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (words[0]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::full->toString()) {
        throw StructureBuildingException(L"Don't alter wordRules.xml without checking the consequences!");
    }
    StructureBuildingMethods::resolveWordOrBracket(state, words[0]);
    BuildResults *acidBr = new BuildResults(words[0]);
    int functionalAtomCount = acidBr->getFunctionalAtomCount();
    if (functionalAtomCount == 0) {
        throw StructureBuildingException(L"No functionalAtoms detected!");
    }

    bool monoMultiplierDetected = false;
    std::vector<Fragment *> functionalGroupFragments;
    for (int i = 1; i < words.size(); i++) {
        Element *functionalGroupWord = words[i];
        std::vector<Element *> functionalGroups = OpsinTools::getDescendantElementsWithTagName(functionalGroupWord,
                                                                                               XmlDeclarations::FUNCTIONALGROUP_EL);
        if (functionalGroups.size() != 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected exactly 1 functionalGroup. Found ") + functionalGroups.size());
        }

        Fragment *monoValentFunctionGroup = state->fragManager->buildSMILES(
                functionalGroups[0]->getAttributeValue(XmlDeclarations::VALUE_ATR),
                XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
        if (functionalGroups[0]->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
            XmlDeclarations::MONOVALENTSTANDALONEGROUP_TYPE_VAL) {
            Atom *ideAtom = monoValentFunctionGroup->getDefaultInAtomOrFirstAtom();
            ideAtom->addChargeAndProtons(1, 1); //e.g. make cyanide charge netural
        }
        Element *possibleMultiplier = OpsinTools::getPreviousSibling(functionalGroups[0]);
        functionalGroupFragments.push_back(monoValentFunctionGroup);
        if (possibleMultiplier != nullptr) {
            int multiplierValue = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            if (multiplierValue == 1) {
                monoMultiplierDetected = true;
            }
            for (int j = 1; j < multiplierValue; j++) {
                functionalGroupFragments.push_back(state->fragManager->copyFragment(monoValentFunctionGroup));
            }
            possibleMultiplier->detach();
        }
    }
    int halideCount = functionalGroupFragments.size();
    if (halideCount < functionalAtomCount && halideCount == 1 && !monoMultiplierDetected) {
        //e.g. phosphoric chloride, chloride is implicitly multiplied
        Fragment *ideFrag = functionalGroupFragments[0];
        for (int i = halideCount; i < functionalAtomCount; i++) {
            functionalGroupFragments.push_back(state->fragManager->copyFragment(ideFrag));
        }
        halideCount = functionalAtomCount;
    } else if (halideCount > functionalAtomCount || (!monoMultiplierDetected && halideCount < functionalAtomCount)) {
        throw StructureBuildingException(
                L"Mismatch between number of halide/pseudo halide fragments and acidic oxygens");
    }
    for (int i = halideCount - 1; i >= 0; i--) {
        Fragment *ideFrag = functionalGroupFragments[i];
        Atom *ideAtom = ideFrag->getDefaultInAtomOrFirstAtom();
        Atom *acidAtom = acidBr->getFunctionalAtom(i);
        if (acidAtom->getElement() != ChemEl::O) {
            throw StructureBuildingException(
                    std::wstring(L"Atom type expected to be oxygen but was: ") + acidAtom->getElement());
        }
        acidBr->removeFunctionalAtom(i);
        Fragment *acidFragment = acidAtom->getFrag();
        state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(acidAtom, ideAtom);
        state->fragManager->incorporateFragment(ideFrag, acidFragment);
    }
}

void StructureBuilder::buildAdditionCompound(std::vector<Element *> &words) throw(StructureBuildingException) {
    Element *firstWord = words[0];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (firstWord->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::full->toString()) {
        throw StructureBuildingException(L"Don't alter wordRules.xml without checking the consequences!");
    }
    StructureBuildingMethods::resolveWordOrBracket(state, firstWord);
    Element *elementaryAtomEl = StructureBuildingMethods::findRightMostGroupInBracket(firstWord);
    Fragment *elementaryAtomFrag = elementaryAtomEl->getFrag();
    Atom *elementaryAtom = elementaryAtomFrag->getFirstAtom();
    int charge = elementaryAtom->getCharge();
    std::vector<Fragment *> functionalGroupFragments;
    for (int i = 1; i < words.size(); i++) {
        Element *functionalGroupWord = words[i];
        std::vector<Element *> functionalGroups = OpsinTools::getDescendantElementsWithTagName(functionalGroupWord,
                                                                                               XmlDeclarations::FUNCTIONALGROUP_EL);
        if (functionalGroups.size() != 1) {
            throw StructureBuildingException(
                    std::wstring(L"Expected exactly 1 functionalGroup. Found ") + functionalGroups.size());
        }
        Element *functionGroup = functionalGroups[0];

        Fragment *monoValentFunctionGroup = state->fragManager->buildSMILES(
                functionGroup->getAttributeValue(XmlDeclarations::VALUE_ATR), XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL,
                XmlDeclarations::NONE_LABELS_VAL);
        if (functionGroup->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
            XmlDeclarations::MONOVALENTSTANDALONEGROUP_TYPE_VAL) {
            Atom *ideAtom = monoValentFunctionGroup->getDefaultInAtomOrFirstAtom();
            ideAtom->addChargeAndProtons(1, 1); //e.g. make cyanide charge netural
        }
        Element *possibleMultiplier = OpsinTools::getPreviousSibling(functionGroup);
        functionalGroupFragments.push_back(monoValentFunctionGroup);
        if (possibleMultiplier != nullptr) {
            int multiplierValue = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            for (int j = 1; j < multiplierValue; j++) {
                functionalGroupFragments.push_back(state->fragManager->copyFragment(monoValentFunctionGroup));
            }
            possibleMultiplier->detach();
        } else if (words.size() == 2) { //silicon chloride -->silicon tetrachloride
            int incomingBondOrder = elementaryAtom->getIncomingValency();
            int expectedValency;
            if (charge > 0) {
                expectedValency = incomingBondOrder + charge;
            } else {
                if (elementaryAtom->getProperty(Atom::OXIDATION_NUMBER) != nullptr) {
                    expectedValency = elementaryAtom->getProperty(Atom::OXIDATION_NUMBER);
                } else {
                    if (elementaryAtomEl->getAttribute(XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR) != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                        std::vector<std::wstring> typicalOxidationStates = elementaryAtomEl->getAttributeValue(
                                XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR).split(L":")[0]->split(L",");
                        expectedValency = std::stoi(typicalOxidationStates[0]);
                    } else {
                        expectedValency = ValencyChecker::getPossibleValencies(elementaryAtom->getElement(), charge)[0];
                    }
                }
            }
            int implicitMultiplier = expectedValency - incomingBondOrder > 1 ? expectedValency - incomingBondOrder : 1;
            for (int j = 1; j < implicitMultiplier; j++) {
                functionalGroupFragments.push_back(state->fragManager->copyFragment(monoValentFunctionGroup));
            }
        }
    }
    if (charge > 0) {
        elementaryAtom->setCharge(charge - functionalGroupFragments.size());
    }

    //[AlH3] --> [AlH4-] , [AlH4] --> [AlH4-]
    applyAluminiumHydrideSpecialCase(firstWord, elementaryAtom, functionalGroupFragments);

    int halideCount = functionalGroupFragments.size();
    boost::optional<int> maximumVal = ValencyChecker::getMaximumValency(elementaryAtom->getElement(),
                                                                        elementaryAtom->getCharge());
    if (maximumVal && halideCount > maximumVal) {
        throw StructureBuildingException(
                std::wstring(L"Too many halides/psuedo halides addded to ") + elementaryAtom->getElement());
    }
    for (int i = halideCount - 1; i >= 0; i--) {
        Fragment *ideFrag = functionalGroupFragments[i];
        Atom *ideAtom = ideFrag->getDefaultInAtomOrFirstAtom();
        state->fragManager->incorporateFragment(ideFrag, ideAtom, elementaryAtomFrag, elementaryAtom, 1);
    }
}

void StructureBuilder::applyAluminiumHydrideSpecialCase(Element *firstWord, Atom *elementaryAtom,
                                                        std::vector<Fragment *> &functionalGroupFragments) throw(StructureBuildingException) {
    if ((elementaryAtom->getElement() == ChemEl::Al || elementaryAtom->getElement() == ChemEl::B) &&
        elementaryAtom->getCharge() == 0) {
        if (functionalGroupFragments.size() == 3) {
            if (functionalGroupFragments[0]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H &&
                functionalGroupFragments[1]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H &&
                functionalGroupFragments[2]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H) {
                Element *counterCationWordRule = OpsinTools::getPreviousSibling(firstWord->getParent());
                if (counterCationWordRule != nullptr && counterCationWordRule->getChildCount() == 1) {
                    Element *word = counterCationWordRule->getFirstChildElement(XmlDeclarations::WORD_EL);
                    if (word != nullptr && word->getChildCount() == 1) {
                        Element *root = word->getFirstChildElement(XmlDeclarations::ROOT_EL);
                        if (root != nullptr && root->getChildCount() == 1) {
                            Element *group = root->getFirstChildElement(XmlDeclarations::GROUP_EL);
                            if (group != nullptr && XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL ==
                                                    group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                                ChemEl chemEl = group->getFrag()->getFirstAtom()->getElement();
                                if (chemEl == ChemEl::Li || chemEl == ChemEl::Na || chemEl == ChemEl::K ||
                                    chemEl == ChemEl::Rb || chemEl == ChemEl::Cs) {
                                    functionalGroupFragments.push_back(
                                            state->fragManager->copyFragment(functionalGroupFragments[0]));
                                    elementaryAtom->setCharge(-1);
                                }
                            }
                        }
                    }
                }

            }
        } else if (functionalGroupFragments.size() == 4) {
            if (functionalGroupFragments[0]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H &&
                functionalGroupFragments[1]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H &&
                functionalGroupFragments[2]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H &&
                functionalGroupFragments[3]->getDefaultInAtomOrFirstAtom().getElement() == ChemEl::H) {
                elementaryAtom->setCharge(-1);
            }
        }
    }
}

void StructureBuilder::buildGlycol(std::vector<Element *> &words) throw(StructureBuildingException) {
    int wordIndice = 0;
    StructureBuildingMethods::resolveWordOrBracket(state, words[wordIndice]); //the group
    Element *finalGroup = findRightMostGroupInWordOrWordRule(words[wordIndice]);
    Fragment *theDiRadical = finalGroup->getFrag();
    if (theDiRadical->getOutAtomCount() != 2) {
        throw StructureBuildingException(
                std::wstring(L"Glycol class names (e.g. ethylene glycol) expect two outAtoms. Found: ") +
                std::to_wstring(theDiRadical->getOutAtomCount()));
    }
    wordIndice++;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (wordIndice >= words.size() ||
        words[wordIndice]->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::functionalTerm->toString()) {
        throw StructureBuildingException(L"Glycol functionalTerm word expected");
    }
    std::vector<Element *> functionalClassEls = OpsinTools::getDescendantElementsWithTagName(words[wordIndice],
                                                                                             XmlDeclarations::FUNCTIONALCLASS_EL);
    if (functionalClassEls.size() != 1) {
        throw StructureBuildingException(L"Glycol functional class not found where expected");
    }

    OutAtom *outAtom1 = theDiRadical->getOutAtom(0);
    Atom *chosenAtom1 = outAtom1->isSetExplicitly() ? outAtom1->getAtom()
                                                    : StructureBuildingMethods::findAtomForUnlocantedRadical(state,
                                                                                                             theDiRadical,
                                                                                                             outAtom1);
    Fragment *functionalFrag = state->fragManager->buildSMILES(
            functionalClassEls[0]->getAttributeValue(XmlDeclarations::VALUE_ATR),
            XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    if (outAtom1->getValency() != 1) {
        throw StructureBuildingException(std::wstring(L"OutAtom has unexpected valency. Expected 1. Actual: ") +
                                         std::to_wstring(outAtom1->getValency()));
    }
    state->fragManager->createBond(chosenAtom1, functionalFrag->getFirstAtom(), 1);
    state->fragManager->incorporateFragment(functionalFrag, theDiRadical);

    OutAtom *outAtom2 = theDiRadical->getOutAtom(1);
    Atom *chosenAtom2 = outAtom2->isSetExplicitly() ? outAtom2->getAtom()
                                                    : StructureBuildingMethods::findAtomForUnlocantedRadical(state,
                                                                                                             theDiRadical,
                                                                                                             outAtom2);
    Fragment *hydroxy = state->fragManager->buildSMILES(L"O", XmlDeclarations::FUNCTIONALCLASS_TYPE_VAL,
                                                        XmlDeclarations::NONE_LABELS_VAL);
    if (outAtom2->getValency() != 1) {
        throw StructureBuildingException(std::wstring(L"OutAtom has unexpected valency. Expected 1. Actual: ") +
                                         std::to_wstring(outAtom2->getValency()));
    }
    state->fragManager->createBond(chosenAtom2, hydroxy->getFirstAtom(), 1);
    state->fragManager->incorporateFragment(hydroxy, theDiRadical);
    theDiRadical->removeOutAtom(1);
    theDiRadical->removeOutAtom(0);
}

void StructureBuilder::buildGlycolEther(std::vector<Element *> &words) throw(StructureBuildingException) {
    std::vector<Element *> wordsToAttachToGlycol;
    Element *glycol = words[0];
    StructureBuildingMethods::resolveWordOrBracket(state,
                                                   glycol); //if this actually is something like ethylene glycol this is a no-op as it will already have been resolved
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (glycol->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::full->toString()) {
        throw StructureBuildingException(L"OPSIN Bug: Cannot find glycol word!");
    }
    for (int i = 1; i < words.size(); i++) {
        Element *wordOrWordRule = words[i];
        //ether ignored
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (wordOrWordRule->getAttributeValue(XmlDeclarations::TYPE_ATR) != WordType::functionalTerm->toString()) {
            StructureBuildingMethods::resolveWordOrBracket(state, wordOrWordRule); //the substituent to attach
            wordsToAttachToGlycol.push_back(wordOrWordRule);
        } else if (!wordOrWordRule->getAttributeValue(XmlDeclarations::VALUE_ATR).equalsIgnoreCase(L"ether")) {
            throw StructureBuildingException(
                    std::wstring(L"Unexpected word encountered when applying glycol ether word rule ") +
                    wordOrWordRule->getAttributeValue(XmlDeclarations::VALUE_ATR));
        }
    }
    int numOfEthers = wordsToAttachToGlycol.size();
    if (numOfEthers == 0) {
        throw StructureBuildingException(L"OPSIN Bug: Unexpected number of substituents for glycol ether");
    }
    Element *finalGroup = findRightMostGroupInWordOrWordRule(glycol);
    std::vector<Atom *> hydroxyAtoms = FragmentTools::findHydroxyGroups(finalGroup->getFrag());
    if (hydroxyAtoms.empty()) {
        throw StructureBuildingException(std::wstring(L"No hydroxy groups found in: ") + finalGroup->getValue() +
                                         std::wstring(L" to form ether"));
    }
    if (hydroxyAtoms.size() < numOfEthers) {
        throw StructureBuildingException(
                std::wstring(L"Insufficient hydroxy groups found in: ") + finalGroup->getValue() +
                std::wstring(L" to form required number of ethers"));
    }
    for (int i = 0; i < numOfEthers; i++) {
        BuildResults *br = new BuildResults(wordsToAttachToGlycol[i]);
        if (br->getOutAtomCount() > 0) { //form ether
            state->fragManager->createBond(hydroxyAtoms[i], br->getOutAtom(0)->getAtom(), 1);
            br->removeOutAtom(0);
        } else if (br->getFunctionalAtomCount() > 0) { //form ester
            Atom *ateAtom = br->getFunctionalAtom(0);
            ateAtom->neutraliseCharge();
            state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(hydroxyAtoms[i],
                                                                                 br->getFunctionalAtom(0));
            br->removeFunctionalAtom(0);
        } else {
            throw StructureBuildingException(std::wstring(
                    L"Word had neither an outAtom or a functionalAtom! hence neither and ether or ester could be formed : ") +
                                             wordsToAttachToGlycol[i]->getAttributeValue(XmlDeclarations::VALUE_ATR));
        }
    }
}

void StructureBuilder::buildAcetal(std::vector<Element *> &words) throw(StructureBuildingException) {
    for (int i = 0; i < words.size() - 1; i++) {
        StructureBuildingMethods::resolveWordOrBracket(state, words[i]);
    }
    BuildResults *substituentsBr = new BuildResults();
    for (int i = 1; i < words.size() - 1; i++) {
        Element *currentWord = words[i];
        BuildResults *substituentBr = new BuildResults(currentWord);
        int outAtomCount = substituentBr->getOutAtomCount();
        if (outAtomCount == 1) {
            std::wstring locantForSubstituent = currentWord->getAttributeValue(XmlDeclarations::LOCANT_ATR);
            if (locantForSubstituent != L"") {
                substituentBr->getFirstOutAtom()->setLocant(locantForSubstituent);
            }
        } else if (outAtomCount == 0) {
            throw StructureBuildingException(L"Substituent was expected to have at least one outAtom");
        }
        substituentsBr->mergeBuildResults(substituentBr);
    }
    Element *rightMostGroup = findRightMostGroupInWordOrWordRule(words[0]);
    Fragment *rootFragment = rightMostGroup->getFrag(); //the group which will be modified
    std::vector<Atom *> carbonylOxygen = findCarbonylOxygens(rootFragment, std::vector<std::wstring>());
    Element *functionalWord = words[words.size() - 1];
    std::vector<Element *> functionalClasses = OpsinTools::getDescendantElementsWithTagName(functionalWord,
                                                                                            XmlDeclarations::FUNCTIONALCLASS_EL);
    if (functionalClasses.size() != 1) {
        throw StructureBuildingException(L"OPSIN bug: unable to find acetal functionalClass");
    }
    Element *functionalClassEl = functionalClasses[0];
    std::wstring functionalClass = functionalClassEl->getValue();
    Element *beforeAcetal = OpsinTools::getPreviousSibling(functionalClassEl);
    int numberOfAcetals = 1;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> elements = functionalClassEl->getAttributeValue(XmlDeclarations::VALUE_ATR).split(L",");
    if (beforeAcetal != nullptr) {
        if (beforeAcetal->getName() == XmlDeclarations::MULTIPLIER_EL) {
            numberOfAcetals = std::stoi(beforeAcetal->getAttributeValue(XmlDeclarations::VALUE_ATR));
        } else {
            replaceChalcogensInAcetal(functionalClassEl, elements);
        }
    }

    if (carbonylOxygen.size() < numberOfAcetals) {
        throw StructureBuildingException(
                std::wstring(L"Insufficient carbonyls to form ") + numberOfAcetals + std::wstring(L" ") +
                functionalClass);
    }
    bool hemiacetal = functionalClass.find(L"hemi") != std::wstring::npos;
    std::vector<Fragment *> acetalFrags;
    for (int i = 0; i < numberOfAcetals; i++) {
        acetalFrags.push_back(formAcetal(carbonylOxygen, elements));
    }
    int bondsToForm = hemiacetal ? numberOfAcetals : 2 * numberOfAcetals;
    if (substituentsBr->getOutAtomCount() != bondsToForm) {
        throw StructureBuildingException(
                std::wstring(L"incorrect number of susbtituents when forming ") + functionalClass);
    }
    connectSubstituentsToAcetal(acetalFrags, substituentsBr, hemiacetal);
}

void StructureBuilder::replaceChalcogensInAcetal(Element *functionalClassEl,
                                                 std::vector<std::wstring> &elements) throw(StructureBuildingException) {
    Element *currentEl = functionalClassEl->getParent()->getChild(0);
    int multiplier = 1;
    if (currentEl->getName() == XmlDeclarations::MULTIPLIER_EL) {
        multiplier = std::stoi(currentEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
        if (multiplier > 2) {
            throw StructureBuildingException(functionalClassEl->getValue() + std::wstring(L" only has two oxygen!"));
        }
        currentEl = OpsinTools::getNextSibling(currentEl);
    }
    int i = 0;
    while (currentEl != functionalClassEl) {
        if (currentEl->getName() == XmlDeclarations::GROUP_EL) {
            for (int j = 0; j < multiplier; j++) {
                if (i == 2) {
                    throw StructureBuildingException(
                            functionalClassEl->getValue() + std::wstring(L" only has two oxygen!"));
                }
                if (elements[i] != L"O") {
                    throw StructureBuildingException(std::wstring(L"Replacement on ") + functionalClassEl->getValue() +
                                                     std::wstring(L" can only be used to replace oxygen!"));
                }
                elements[i++] = currentEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
            }
        } else {
            throw StructureBuildingException(L"Unexpected element before acetal");
        }
        currentEl = OpsinTools::getNextSibling(currentEl);
    }
}

Fragment *StructureBuilder::formAcetal(std::vector<Atom *> &carbonylOxygen,
                                       std::vector<std::wstring> &elements) throw(StructureBuildingException) {
    Atom *neighbouringCarbon = carbonylOxygen[0]->getAtomNeighbours()[0];
    state->fragManager->removeAtomAndAssociatedBonds(carbonylOxygen[0]);
    carbonylOxygen.erase(carbonylOxygen.begin());
    Fragment *acetalFrag = state->fragManager->buildSMILES(StringTools::arrayToString(elements, L"."), L"",
                                                           XmlDeclarations::NONE_LABELS_VAL);
    FragmentTools::assignElementLocants(acetalFrag, std::vector<Fragment *>());
    std::vector<Atom *> acetalAtomList = acetalFrag->getAtomList();
    Atom *atom1 = acetalAtomList[0];
    state->fragManager->createBond(neighbouringCarbon, atom1, 1);
    Atom *atom2 = acetalAtomList[1];
    state->fragManager->createBond(neighbouringCarbon, atom2, 1);
    state->fragManager->incorporateFragment(acetalFrag, neighbouringCarbon->getFrag());
    return acetalFrag;
}

bool StructureBuilder::buildAlcoholEster(std::vector<Element *> &words,
                                         int numberOfWordRules) throw(StructureBuildingException) {
    for (auto word : words) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (!WordType::full->toString()->equals(word->getAttributeValue(XmlDeclarations::TYPE_ATR))) {
            throw StructureBuildingException(L"Bug in word rule for potentialAlcoholEster");
        }
        StructureBuildingMethods::resolveWordOrBracket(state, word);
    }
    int ateWords = words.size() - 1;
    if (ateWords < 1) {
        throw StructureBuildingException(L"Bug in word rule for potentialAlcoholEster");
    }

    Fragment *potentialAlcoholFragment = findRightMostGroupInWordOrWordRule(words[0])->getFrag();
    std::vector<Atom *> hydroxyAtoms = FragmentTools::findHydroxyGroups(potentialAlcoholFragment);

    std::vector<Atom *> chosenHydroxyAtoms;
    std::vector<BuildResults *> ateBuildResults;
    for (int i = 1; i < words.size(); i++) {
        Element *ateWord = words[i];
        BuildResults *wordBr = new BuildResults(ateWord);
        if (isAppropriateAteGroupForAlcoholEster(ateWord, wordBr)) {
            std::wstring locant = ateWord->getAttributeValue(XmlDeclarations::LOCANT_ATR);
            if (locant != L"") {
                Atom *atomOnAlcoholFragment = potentialAlcoholFragment->getAtomByLocantOrThrow(locant);
                if (!std::find(hydroxyAtoms.begin(), hydroxyAtoms.end(), atomOnAlcoholFragment) != hydroxyAtoms.end() ||
                    std::find(chosenHydroxyAtoms.begin(), chosenHydroxyAtoms.end(), atomOnAlcoholFragment) !=
                    chosenHydroxyAtoms.end()) {
                    atomOnAlcoholFragment = potentialAlcoholFragment->getAtomByLocantOrThrow(
                            std::wstring(L"O") + locant);
                }
                if (!std::find(hydroxyAtoms.begin(), hydroxyAtoms.end(), atomOnAlcoholFragment) != hydroxyAtoms.end() ||
                    std::find(chosenHydroxyAtoms.begin(), chosenHydroxyAtoms.end(), atomOnAlcoholFragment) !=
                    chosenHydroxyAtoms.end()) {
                    throw StructureBuildingException(
                            locant + std::wstring(L" did not point to a hydroxy group to be used for ester formation"));
                }
                chosenHydroxyAtoms.push_back(atomOnAlcoholFragment);
            } else if (words.size() == 2) {
                //special case for adenosine triphosphate and the like
                //guess that locant might be 5'
                Atom *atomOnAlcoholFragment = potentialAlcoholFragment->getAtomByLocant(L"O5'");
                if (std::find(hydroxyAtoms.begin(), hydroxyAtoms.end(), atomOnAlcoholFragment) != hydroxyAtoms.end()) {
                    chosenHydroxyAtoms.push_back(atomOnAlcoholFragment);
                }
            }
            ateBuildResults.push_back(wordBr);
        } else {
            return false;
        }
    }

    if (chosenHydroxyAtoms.size() < ateWords) {
        if (!chosenHydroxyAtoms.empty()) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Either all or none of the esters should be locanted in alcohol ester rule");
            throw std::exception();
        }
        if (hydroxyAtoms.size() == ateWords || hydroxyAtoms.size() > ateWords &&
                                               (AmbiguityChecker::allAtomsEquivalent(hydroxyAtoms) ||
                                                potentialAlcoholFragment->getTokenEl()->getValue() == L"glycerol")) {
            for (int i = 0; i < ateWords; i++) {
                chosenHydroxyAtoms.push_back(hydroxyAtoms[i]);
            }
        } else {
            return false;
        }
    }

    for (int i = 0; i < ateWords; i++) {
        BuildResults *br = ateBuildResults[i];
        Element *ateWord = words[i + 1];
        Element *ateGroup = findRightMostGroupInWordOrWordRule(ateWord);
        if (ateGroup->getAttribute(XmlDeclarations::NUMBEROFFUNCTIONALATOMSTOREMOVE_ATR) == nullptr &&
            numberOfWordRules == 1) {
            //by convention [O-] are implicitly converted to [OH] when phosphates/sulfates are attached
            //If word rules is > 1 this will be done or not done as part of charge balancing
            for (int j = br->getFunctionalAtomCount() - 1; j >= 1; j--) {
                Atom *atomToDefunctionalise = br->getFunctionalAtom(j);
                br->removeFunctionalAtom(j);
                atomToDefunctionalise->neutraliseCharge();
            }
        }
        Atom *functionalAtom = br->getFunctionalAtom(0);
        br->removeFunctionalAtom(0);
        functionalAtom->neutraliseCharge();
        state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(functionalAtom, chosenHydroxyAtoms[i]);
    }
    return true;
}

void StructureBuilder::buildAmineDiConjunctiveSuffix(std::vector<Element *> &words) throw(StructureBuildingException) {
    for (auto word : words) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        if (!WordType::full->toString()->equals(word->getAttributeValue(XmlDeclarations::TYPE_ATR))) {
            throw StructureBuildingException(L"Bug in word rule for amineDiConjunctiveSuffix");
        }
        StructureBuildingMethods::resolveWordOrBracket(state, word);
    }
    if (words.size() != 3) {
        throw StructureBuildingException(std::wstring(
                L"Unexpected number of words encountered when processing name of type amineDiConjunctiveSuffix, expected 3 but found: ") +
                                         words.size());
    }
    Element *aminoAcid = findRightMostGroupInWordOrWordRule(words[0]);
    if (aminoAcid == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: failed to find amino acid");
        throw std::exception();
    }
    Atom *amineAtom = aminoAcid->getFrag()->getDefaultInAtom();
    if (amineAtom == nullptr) {
        throw StructureBuildingException(L"OPSIN did not know where the amino acid amine was located");
    }

    for (int i = 1; i < words.size(); i++) {
        Element *word = words[i];
        Fragment *suffixLikeGroup = findRightMostGroupInWordOrWordRule(word)->getFrag();
        std::wstring locant = word->getAttributeValue(XmlDeclarations::LOCANT_ATR);
        if (locant != L"") {
            if (locant != L"N") {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: locant expected to be N but was: " + locant);
                throw std::exception();
            }
        }
        Atom *atomToConnectToOnConjunctiveFrag = FragmentTools::lastNonSuffixCarbonWithSufficientValency(
                suffixLikeGroup);
        if (atomToConnectToOnConjunctiveFrag == nullptr) {
            throw StructureBuildingException(L"OPSIN Bug: Unable to find non suffix carbon with sufficient valency");
        }
        state->fragManager->createBond(atomToConnectToOnConjunctiveFrag, amineAtom, 1);
    }
}

bool StructureBuilder::isAppropriateAteGroupForAlcoholEster(Element *ateWord,
                                                            BuildResults *wordBr) throw(StructureBuildingException) {
    if (wordBr->getFunctionalAtomCount() > 0) {
        if (ateWord->getAttributeValue(XmlDeclarations::LOCANT_ATR) != L"") {
            //locanted, so locant must be used for this purpose
            return true;
        }
        if (wordBr->getFunctionalAtomCount() == 1) {
            if (matchCommonCarboxylicSalt->matcher(ateWord->getAttributeValue(XmlDeclarations::VALUE_ATR)).find()) {
                return false;
            }
            return true;
        }
        std::wstring ateGroupText = findRightMostGroupInWordOrWordRule(ateWord)->getValue();
        //e.g. triphosphate
        if (matchCommonEsterFormingInorganicSalt->matcher(ateGroupText).matches()) {
            return true;
        }

    }
    return false;
}

void StructureBuilder::splitAlcoholEsterRuleIntoTwoSimpleWordRules(std::vector<Element *> &words) {
    Element *firstGroup = words[0];
    Element *wordRule = firstGroup->getParent();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    wordRule->getAttribute(XmlDeclarations::WORDRULE_ATR)->setValue(WordRule::simple->toString());
    wordRule->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(
            firstGroup->getAttributeValue(XmlDeclarations::VALUE_ATR));

    Element *newWordRule = new GroupingEl(XmlDeclarations::WORDRULE_EL);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    newWordRule->addAttribute(XmlDeclarations::TYPE_ATR, WordType::full->toString());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    newWordRule->addAttribute(XmlDeclarations::WORDRULE_ATR, WordRule::simple->toString());
    newWordRule->addAttribute(XmlDeclarations::VALUE_ATR, words[1]->getAttributeValue(XmlDeclarations::VALUE_ATR));
    OpsinTools::insertAfter(wordRule, newWordRule);
    for (int i = 1; i < words.size(); i++) {
        Element *word = words[i];
        word->detach();
        newWordRule->addChild(word);
    }
}

void StructureBuilder::connectSubstituentsToAcetal(std::vector<Fragment *> &acetalFrags, BuildResults *subBr,
                                                   bool hemiacetal) throw(StructureBuildingException) {
    std::unordered_map<Fragment *, int> usageMap;
    for (int i = subBr->getOutAtomCount() - 1; i >= 0; i--) {
        OutAtom *out = subBr->getOutAtom(i);
        subBr->removeOutAtom(i);
        Atom *atomToUse = nullptr;
        if (out->getLocant() != L"") {
            bool numericLocant = OpsinTools::MATCH_NUMERIC_LOCANT->matcher(out->getLocant()).matches();
            for (auto possibleAcetalFrag : acetalFrags) {
                if (numericLocant) {
                    Atom *a = OpsinTools::depthFirstSearchForNonSuffixAtomWithLocant(possibleAcetalFrag->getFirstAtom(),
                                                                                     out->getLocant());
                    if (a != nullptr) {
                        std::vector<Atom *> atomList = possibleAcetalFrag->getAtomList();
                        if (atomList[0]->getBondCount() == 1) {
                            atomToUse = atomList[0];
                            break;
                        } else if (atomList[1]->getBondCount() == 1) {
                            atomToUse = atomList[1];
                            break;
                        }
                    }
                } else if (possibleAcetalFrag->hasLocant(out->getLocant())) {
                    atomToUse = possibleAcetalFrag->getAtomByLocantOrThrow(out->getLocant());
                    break;
                }
            }
            if (atomToUse == nullptr) {
                throw StructureBuildingException(L"Unable to find suitable acetalFrag");
            }
        } else {
            std::vector<Atom *> atomList = acetalFrags[0]->getAtomList();
            if (atomList[0]->getBondCount() == 1) {
                atomToUse = atomList[0];
            } else if (atomList[1]->getBondCount() == 1) {
                atomToUse = atomList[1];
            } else {
                throw StructureBuildingException(L"OPSIN bug: unable to find acetal atom");
            }
        }
        Fragment *acetalFrag = atomToUse->getFrag();
        int usage = usageMap[acetalFrag] != nullptr ? usageMap[acetalFrag] : 0;
        state->fragManager->createBond(out->getAtom(), atomToUse, out->getValency());
        usage++;
        if (usage >= 2 || hemiacetal) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
            acetalFrags.remove(acetalFrag);
        }
        usageMap[acetalFrag] = usage;
    }
}

void StructureBuilder::buildCyclicPeptide(std::vector<Element *> &words) throw(StructureBuildingException) {
    if (words.size() != 2) {
        throw StructureBuildingException(
                std::wstring(L"OPSIN Bug: Expected 2 words in cyclic peptide name, found: ") + words.size());
    }
    Element *peptide = words[1];
    StructureBuildingMethods::resolveWordOrBracket(state, peptide);
    BuildResults *peptideBr = new BuildResults(peptide);
    if (peptideBr->getOutAtomCount() == 1) {
        Atom *outAtom = getOutAtomTakingIntoAccountWhetherSetExplicitly(peptideBr, 0);
        std::vector<Element *> aminoAcids = OpsinTools::getDescendantElementsWithTagNameAndAttribute(peptide,
                                                                                                     XmlDeclarations::GROUP_EL,
                                                                                                     XmlDeclarations::TYPE_ATR,
                                                                                                     XmlDeclarations::AMINOACID_TYPE_VAL);
        if (aminoAcids.size() < 2) {
            throw StructureBuildingException(L"Cyclic peptide building failed: Requires at least two amino acids!");
        }
        Atom *inAtom = aminoAcids[0]->getFrag().getDefaultInAtomOrFirstAtom();

        state->fragManager->createBond(outAtom, inAtom, peptideBr->getOutAtom(0)->getValency());
        peptideBr->removeAllOutAtoms();
    } else {
        throw StructureBuildingException(std::wstring(L"Cyclic peptide building failed: Expected 1 outAtoms, found: ") +
                                         std::to_wstring(peptideBr->getOutAtomCount()));
    }
}

void StructureBuilder::buildPolymer(std::vector<Element *> &words) throw(StructureBuildingException) {
    if (words.size() != 2) {
        throw StructureBuildingException(L"Currently unsupported polymer name type");
    }
    Element *polymer = words[1];
    StructureBuildingMethods::resolveWordOrBracket(state, polymer);
    BuildResults *polymerBr = new BuildResults(polymer);
    if (polymerBr->getOutAtomCount() == 2) {
        Atom *inAtom = getOutAtomTakingIntoAccountWhetherSetExplicitly(polymerBr, 0);
        Atom *outAtom = getOutAtomTakingIntoAccountWhetherSetExplicitly(polymerBr, 1);
        /*
         * We assume the polymer repeats so as an approximation we create an R group with the same element as the group at the other end of polymer (with valency equal to the bondorder of the Rgroup so no H added)
         */
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        Atom *rGroup1 = state->fragManager->buildSMILES(std::wstring(L"[") + outAtom->getElement()
        ::toString() + std::wstring(L"|") + polymerBr->getOutAtom(0)->getValency() +
        std::wstring(L"]"), L"", L"alpha")->getFirstAtom();
        rGroup1->setProperty(Atom::ATOM_CLASS, 1);
        state->fragManager->createBond(inAtom, rGroup1, polymerBr->getOutAtom(0)->getValency());

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        Atom *rGroup2 = state->fragManager->buildSMILES(std::wstring(L"[") + inAtom->getElement()
        ::toString() + std::wstring(L"|") + polymerBr->getOutAtom(1)->getValency() +
        std::wstring(L"]"), L"", L"omega")->getFirstAtom();
        rGroup2->setProperty(Atom::ATOM_CLASS, 2);
        state->fragManager->createBond(outAtom, rGroup2, polymerBr->getOutAtom(1)->getValency());
        polymerAttachmentPoints.push_back(rGroup1);
        polymerAttachmentPoints.push_back(rGroup2);
        polymerBr->removeAllOutAtoms();
    } else {
        throw StructureBuildingException(
                std::wstring(L"Polymer building failed: Two termini were not found; Expected 2 outAtoms, found: ") +
                std::to_wstring(polymerBr->getOutAtomCount()));
    }
}

Atom *StructureBuilder::determineFunctionalAtomToUse(const std::wstring &locant,
                                                     BuildResults *mainGroupBR) throw(StructureBuildingException) {
    for (int i = 0; i < mainGroupBR->getFunctionalAtomCount(); i++) {
        //look for exact locant match
        Atom *possibleAtom = mainGroupBR->getFunctionalAtom(i);
        if (possibleAtom->hasLocant(locant)) {
            mainGroupBR->removeFunctionalAtom(i);
            Set < Atom * > *degenerateAtoms = possibleAtom->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT);
            if (degenerateAtoms != nullptr) {
                degenerateAtoms->remove(possibleAtom);
            }
            return possibleAtom;
        }
    }
    if (OpsinTools::MATCH_NUMERIC_LOCANT->matcher(locant).matches()) {
        //None of the functional atoms had an appropriate locant. Look for the case whether the locant refers to the backbone. e.g. 5-methyl 2-aminopentanedioate
        for (int i = 0; i < mainGroupBR->getFunctionalAtomCount(); i++) {
            Atom *possibleAtom = mainGroupBR->getFunctionalAtom(i);
            if (OpsinTools::depthFirstSearchForNonSuffixAtomWithLocant(possibleAtom, locant) != nullptr) {
                mainGroupBR->removeFunctionalAtom(i);
                Set < Atom * > *degenerateAtoms = possibleAtom->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT);
                if (degenerateAtoms != nullptr) {
                    degenerateAtoms->remove(possibleAtom);
                }
                return possibleAtom;
            }
        }
    } else if (OpsinTools::MATCH_ELEMENT_SYMBOL_LOCANT->matcher(locant).matches()) {
        //None of the functional atoms had an appropriate locant. Look for the special cases:
        //	Where the lack of primes on an element symbol locant should be ignored e.g. O,O-diethyl carbonate
        //	Where the locant is used to decide on the ester configuration c.f. O-methyl ..thioate and S-methyl ..thioate
        bool isElementSymbol = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(locant).matches();
        for (int i = 0; i < mainGroupBR->getFunctionalAtomCount(); i++) {
            Atom *possibleAtom = mainGroupBR->getFunctionalAtom(i);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            if (isElementSymbol &&possibleAtom->getElement()
            ::toString()->equals(locant))
            {
                mainGroupBR->removeFunctionalAtom(i);
                return possibleAtom;
            }
            Set < Atom * > *degenerateAtoms = possibleAtom->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT);
            if (degenerateAtoms != nullptr) {
                bool foundAtom = false;
                for (auto a : degenerateAtoms) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    if (a->hasLocant(locant) || (isElementSymbol && a->getElement()::toString()->equals(locant)))
                    {
                        //swap locants and element type
                        std::vector<std::wstring> tempLocants(a->getLocants());
                        std::vector<std::wstring> tempLocants2(possibleAtom->getLocants());
                        a->clearLocants();
                        possibleAtom->clearLocants();
                        for (auto l : tempLocants) {
                            possibleAtom->addLocant(l);
                        }
                        for (auto l : tempLocants2) {
                            a->addLocant(l);
                        }
                        ChemEl originalChemEl = possibleAtom->getElement();
                        possibleAtom->setElement(a->getElement());
                        a->setElement(originalChemEl);
                        mainGroupBR->removeFunctionalAtom(i);
                        foundAtom = true;
                        break;
                    }
                }
                if (foundAtom) {
                    degenerateAtoms->remove(possibleAtom);
                    return possibleAtom;
                }
            }
        }
    }

    throw StructureBuildingException(std::wstring(L"Cannot find functional atom with locant: ") + locant +
                                     std::wstring(L" to form an ester with"));
}

void StructureBuilder::manipulateStoichiometry(Element *molecule,
                                               std::vector<Element *> &wordRules) throw(StructureBuildingException) {
    bool explicitStoichiometryPresent = applyExplicitStoichiometryIfProvided(wordRules);
    bool chargedFractionalGroup = false;
    std::vector<Element *> wordRulesWithFractionalMultipliers(0);
    for (auto wordRule : wordRules) {
        Element *fractionalMultiplier = wordRule->getChild(0);
        while (fractionalMultiplier->getChildCount() != 0) {
            fractionalMultiplier = fractionalMultiplier->getChild(0);
        }
        if (fractionalMultiplier->getName() == XmlDeclarations::FRACTIONALMULTIPLIER_EL) {
            if (explicitStoichiometryPresent) {
                throw StructureBuildingException(
                        L"Fractional multipliers should not be used in conjunction with explicit stoichiometry");
            }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            std::vector<std::wstring> value = fractionalMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR).split(
                    L"/");
            if (value.size() != 2) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: malformed fractional multiplier: " + fractionalMultiplier.getAttributeValue(VALUE_ATR));
                throw std::exception();
            }
            try {
                int numerator = std::stoi(value[0]);
                int denominator = std::stoi(value[1]);
                if (denominator != 2) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Only fractions of a 1/2 currently supported");
                    throw std::exception();
                }
                for (int j = 1; j < numerator; j++) {
                    Element *clone = state->fragManager->cloneElement(state, wordRule);
                    OpsinTools::insertAfter(wordRule, clone);
                    wordRulesWithFractionalMultipliers.push_back(clone);
                }
            }
            catch (const NumberFormatException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: malformed fractional multiplier: " + fractionalMultiplier.getAttributeValue(VALUE_ATR));
                throw std::exception();
            }
            //don't detach the fractional multiplier to avoid charge balancing multiplication (cf. handling of mono)
            wordRulesWithFractionalMultipliers.push_back(wordRule);
            BuildResults tempVar(wordRule);
            if ((&tempVar)->getCharge() != 0) {
                chargedFractionalGroup = true;
            }
        }
    }
    if (wordRulesWithFractionalMultipliers.size() > 0) {
        if (wordRules.size() == 1) {
            throw StructureBuildingException(L"Unexpected fractional multiplier found at start of word");
        }
        if (chargedFractionalGroup) {
            for (auto wordRule : wordRules) {
                if (std::find(wordRulesWithFractionalMultipliers.begin(), wordRulesWithFractionalMultipliers.end(),
                              wordRule) != wordRulesWithFractionalMultipliers.end()) {
                    continue;
                }
                Element *clone = state->fragManager->cloneElement(state, wordRule);
                OpsinTools::insertAfter(wordRule, clone);
            }
        }
    }
    bool saltExpected = molecule->getAttribute(XmlDeclarations::ISSALT_ATR) != nullptr;
    if (saltExpected) {
        deprotonateAcidIfSaltWithMetal(molecule);
    }
    int overallCharge = state->fragManager->getOverallCharge();
    if (overallCharge != 0 &&
        wordRules.size() > 1) { //a net charge is present! Could just mean the counterion has not been specified though
        balanceChargeIfPossible(molecule, overallCharge, explicitStoichiometryPresent);
    }
    if (wordRulesWithFractionalMultipliers.size() > 0 && !chargedFractionalGroup) {
        for (auto wordRule : molecule->getChildElements(XmlDeclarations::WORDRULE_EL)) {
            if (std::find(wordRulesWithFractionalMultipliers.begin(), wordRulesWithFractionalMultipliers.end(),
                          wordRule) != wordRulesWithFractionalMultipliers.end()) {
                continue;
            }
            Element *clone = state->fragManager->cloneElement(state, wordRule);
            OpsinTools::insertAfter(wordRule, clone);
        }
    }

}

bool StructureBuilder::applyExplicitStoichiometryIfProvided(
        std::vector<Element *> &wordRules) throw(StructureBuildingException) {
    bool explicitStoichiometryPresent = false;
    for (auto wordRule : wordRules) {
        if (wordRule->getAttribute(XmlDeclarations::STOICHIOMETRY_ATR) != nullptr) {
            int stoichiometry = std::stoi(wordRule->getAttributeValue(XmlDeclarations::STOICHIOMETRY_ATR));
            wordRule->removeAttribute(wordRule->getAttribute(XmlDeclarations::STOICHIOMETRY_ATR));
            for (int j = 1; j < stoichiometry; j++) {
                Element *clone = state->fragManager->cloneElement(state, wordRule);
                OpsinTools::insertAfter(wordRule, clone);
            }
            explicitStoichiometryPresent = true;
        }
    }
    return explicitStoichiometryPresent;
}

void StructureBuilder::deprotonateAcidIfSaltWithMetal(Element *molecule) {
    std::vector<BuildResults *> positivelyChargedComponents;
    std::vector<BuildResults *> negativelyChargedComponents;
    std::vector<BuildResults *> neutralComponents;
    std::vector<Element *> wordRules = molecule->getChildElements(XmlDeclarations::WORDRULE_ATR);
    for (auto wordRule : wordRules) {
        BuildResults *br = new BuildResults(wordRule);
        int charge = br->getCharge();
        if (charge > 0) {
            positivelyChargedComponents.push_back(br);
        } else if (charge < 0) {
            negativelyChargedComponents.push_back(br);
        } else {
            neutralComponents.push_back(br);
        }
    }
    if (negativelyChargedComponents.empty() &&
        (positivelyChargedComponents.size() > 0 || getMetalsThatCanBeImplicitlyCations(molecule).size() > 0)) {
        for (int i = neutralComponents.size() - 1; i >= 0; i--) {
            BuildResults *br = neutralComponents[i];
            for (int j = br->getFunctionalAtomCount() - 1; j >= 0; j--) {
                Atom *functionalAtom = br->getFunctionalAtom(j);
                if (functionalAtom->getCharge() == 0 && functionalAtom->getIncomingValency() == 1) {
                    functionalAtom->addChargeAndProtons(-1, -1);
                }
            }
        }
    }
}

void StructureBuilder::balanceChargeIfPossible(Element *molecule, int overallCharge,
                                               bool explicitStoichiometryPresent) throw(StructureBuildingException) {
    std::vector<Element *> wordRules = molecule->getChildElements(XmlDeclarations::WORDRULE_ATR);

    std::vector<Element *> positivelyChargedComponents;
    std::vector<Element *> negativelyChargedComponents;
    std::unordered_map<Element *, int> componentToChargeMapping;
    std::unordered_map<Element *, BuildResults *> componentToBR;

    std::vector<Element *> cationicElements = getMetalsThatCanBeImplicitlyCations(molecule);
    overallCharge = setCationicElementsToTypicalCharge(cationicElements, overallCharge);
    if (overallCharge == 0) {
        return;
    }
    if (cationicElements.size() == 1 && overallCharge < 0) { //e.g. nickel tetrachloride [Ni2+]-->[Ni4+]
        if (setChargeOnCationicElementAppropriately(overallCharge, cationicElements[0])) {
            return;
        }
    }
    if (overallCharge == -2) {
        if (triHalideSpecialCase(wordRules)) {
            //e.g. three iodides --> triiodide ion
            return;
        }
    }

    for (auto wordRule : wordRules) {
        BuildResults *br = new BuildResults(wordRule);
        componentToBR[wordRule] = br;
        int charge = br->getCharge();
        if (charge > 0) {
            positivelyChargedComponents.push_back(wordRule);
        } else if (charge < 0) {
            negativelyChargedComponents.push_back(wordRule);
        }
        componentToChargeMapping[wordRule] = charge;
    }

    if (!explicitStoichiometryPresent && (positivelyChargedComponents.size() == 1 && cationicElements.empty() &&
                                          negativelyChargedComponents.size() >= 1 ||
                                          positivelyChargedComponents.size() >= 1 &&
                                          negativelyChargedComponents.size() == 1)) {
        bool success = multiplyChargedComponents(negativelyChargedComponents, positivelyChargedComponents,
                                                 componentToChargeMapping, overallCharge);
        if (success) {
            return;
        }
    }
    if (cationicElements.size() == 1) { //e.g. magnesium monochloride [Mg2+]-->[Mg+]
        bool success = setChargeOnCationicElementAppropriately(overallCharge, cationicElements[0]);
        if (success) {
            return;
        }
    }
    if (overallCharge <
        0) { //neutralise functionalAtoms if they are the sole cause of the negative charge and multiple molecules are present
        int chargeOnFunctionalAtoms = 0;
        for (auto wordRule : wordRules) {
            BuildResults *br = componentToBR[wordRule];
            int functionalAtomCount = br->getFunctionalAtomCount();
            for (int i = functionalAtomCount - 1; i >= 0; i--) {
                chargeOnFunctionalAtoms += br->getFunctionalAtom(i)->getCharge();
            }
        }
        if (chargeOnFunctionalAtoms <= overallCharge) {
            for (auto wordRule : wordRules) {
                BuildResults *br = componentToBR[wordRule];
                int functionalAtomCount = br->getFunctionalAtomCount();
                for (int i = functionalAtomCount - 1; i >= 0; i--) {
                    if (overallCharge == 0) {
                        return;
                    }
                    overallCharge -= br->getFunctionalAtom(i)->getCharge();
                    br->getFunctionalAtom(i)->neutraliseCharge();
                    br->removeFunctionalAtom(i);
                }
            }
        }
    }
}

std::vector<Element *> StructureBuilder::getMetalsThatCanBeImplicitlyCations(Element *molecule) {
    std::vector<Element *> cationicElements;
    std::vector<Element *> elementaryAtoms = OpsinTools::getDescendantElementsWithTagNameAndAttribute(molecule,
                                                                                                      XmlDeclarations::GROUP_EL,
                                                                                                      XmlDeclarations::SUBTYPE_ATR,
                                                                                                      XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL);
    for (auto elementaryAtom : elementaryAtoms) {
        if (elementaryAtom->getAttribute(XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR) != nullptr) {
            Atom *metalAtom = elementaryAtom->getFrag()->getFirstAtom();
            if (metalAtom->getCharge() == 0 && metalAtom->getProperty(Atom::OXIDATION_NUMBER) ==
                                               nullptr) { //if not 0 charge cannot be implicitly modified
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                std::vector<std::wstring> typicalOxidationStates = elementaryAtom->getAttributeValue(
                        XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR).split(L":")[0]->split(L",");
                int typicalCharge = std::stoi(typicalOxidationStates[typicalOxidationStates.size() - 1]);
                if (typicalCharge > metalAtom->getBondCount()) {
                    cationicElements.push_back(elementaryAtom);
                }
            }
        }
    }
    return cationicElements;
}

int StructureBuilder::setCationicElementsToTypicalCharge(std::vector<Element *> &cationicElements, int overallCharge) {
    for (auto cationicElement : cationicElements) {
        Fragment *cationicFrag = cationicElement->getFrag();
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> typicalOxidationStates = cationicElement->getAttributeValue(
                XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR).split(L":")[0]->split(L",");
        int incomingValency = cationicFrag->getFirstAtom()->getIncomingValency();
        for (auto typicalOxidationState : typicalOxidationStates) {
            int charge = std::stoi(typicalOxidationState);
            if (charge >= incomingValency) {
                charge -= incomingValency;
                overallCharge += charge;
                cationicFrag->getFirstAtom()->setCharge(charge);
                break;
            }
        }
    }
    return overallCharge;
}

bool StructureBuilder::triHalideSpecialCase(std::vector<Element *> &wordRules) {
    for (auto wordRule : wordRules) {
        if (wordRule->getChildCount() == 3) {
            std::wstring value = wordRule->getAttributeValue(XmlDeclarations::VALUE_ATR);
            if ((std::wstring(L"tribromide")) == value || (std::wstring(L"tribromid")) == value ||
                (std::wstring(L"triiodide")) == value || (std::wstring(L"triiodid")) == value) {
                std::vector<Element *> groups1 = OpsinTools::getDescendantElementsWithTagName(wordRule->getChild(0),
                                                                                              XmlDeclarations::GROUP_EL);
                std::vector<Element *> groups2 = OpsinTools::getDescendantElementsWithTagName(wordRule->getChild(1),
                                                                                              XmlDeclarations::GROUP_EL);
                std::vector<Element *> groups3 = OpsinTools::getDescendantElementsWithTagName(wordRule->getChild(2),
                                                                                              XmlDeclarations::GROUP_EL);
                if (groups1.size() != 1 || groups2.size() != 1 || groups3.size() != 1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: Unexpected trihalide representation");
                    throw std::exception();
                }
                Atom *centralAtom = groups1[0]->getFrag().getFirstAtom();
                Atom *otherAtom1 = groups2[0]->getFrag().getFirstAtom();
                otherAtom1->setCharge(0);
                Atom *otherAtom2 = groups3[0]->getFrag().getFirstAtom();
                otherAtom2->setCharge(0);
                state->fragManager->createBond(centralAtom, otherAtom1, 1);
                state->fragManager->createBond(centralAtom, otherAtom2, 1);
                return true;
            }
        }
    }
    return false;
}

bool StructureBuilder::multiplyChargedComponents(std::vector<Element *> &negativelyChargedComponents,
                                                 std::vector<Element *> &positivelyChargedComponents,
                                                 std::unordered_map<Element *, int> &componentToChargeMapping,
                                                 int overallCharge) throw(StructureBuildingException) {
    Element *componentToMultiply;
    if (overallCharge > 0) {
        if (negativelyChargedComponents.size() > 1) {
            return false; //ambiguous as to which to multiply
        }
        componentToMultiply = negativelyChargedComponents[0];
    } else {
        if (positivelyChargedComponents.size() > 1) {
            return false; //ambiguous as to which to multiply
        }
        componentToMultiply = positivelyChargedComponents[0];
    }

    int charge = componentToChargeMapping[componentToMultiply];
    if (overallCharge % charge == 0) { //e.g. magnesium chloride
        if (!componentCanBeMultiplied(componentToMultiply)) {
            return false;
        }
        int timesToDuplicate = std::abs(overallCharge / charge);
        for (int i = 0; i < timesToDuplicate; i++) {
            OpsinTools::insertAfter(componentToMultiply, state->fragManager->cloneElement(state, componentToMultiply));
        }
    } else { //e.g. iron(3+) sulfate -->2:3 mixture
        if (positivelyChargedComponents.size() > 1 || !componentCanBeMultiplied(positivelyChargedComponents[0])) {
            return false;
        }
        if (negativelyChargedComponents.size() > 1 || !componentCanBeMultiplied(negativelyChargedComponents[0])) {
            return false;
        }
        int positiveCharge = componentToChargeMapping[positivelyChargedComponents[0]];
        int negativeCharge = std::abs(componentToChargeMapping[negativelyChargedComponents[0]]);
        int targetTotalAbsoluteCharge = positiveCharge * negativeCharge;
        for (int i = (targetTotalAbsoluteCharge / negativeCharge); i > 1; i--) {
            OpsinTools::insertAfter(negativelyChargedComponents[0],
                                    state->fragManager->cloneElement(state, negativelyChargedComponents[0]));
        }
        for (int i = (targetTotalAbsoluteCharge / positiveCharge); i > 1; i--) {
            OpsinTools::insertAfter(positivelyChargedComponents[0],
                                    state->fragManager->cloneElement(state, positivelyChargedComponents[0]));
        }
    }
    return true;
}

bool StructureBuilder::componentCanBeMultiplied(Element *componentToMultiply) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (componentToMultiply->getAttributeValue(XmlDeclarations::WORDRULE_ATR) == WordRule::simple->toString() &&
        OpsinTools::getChildElementsWithTagNameAndAttribute(componentToMultiply, XmlDeclarations::WORD_EL,
                                                            XmlDeclarations::TYPE_ATR,
                                                            WordType::full->toString()).size() > 1) {
        return false; //already has been multiplied e.g. dichloride
    }
    Element *firstChild = componentToMultiply->getChild(0);
    while (firstChild->getChildCount() != 0) {
        firstChild = firstChild->getChild(0);
    }
    if (firstChild->getName() == XmlDeclarations::MULTIPLIER_EL || firstChild->getName() ==
                                                                   XmlDeclarations::FRACTIONALMULTIPLIER_EL) { //e.g. monochloride. Allows specification of explicit stoichiometry
        return false;
    }
    return true;
}

bool StructureBuilder::setChargeOnCationicElementAppropriately(int overallCharge, Element *cationicElement) {
    Atom *cation = cationicElement->getFrag()->getFirstAtom();
    int chargeOnCationNeeded = -(overallCharge - cation->getCharge());
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    int maximumCharge = static_cast<Integer>(cationicElement->getAttributeValue(
            XmlDeclarations::COMMONOXIDATIONSTATESANDMAX_ATR).split(L":")[1]);
    if (chargeOnCationNeeded >= 0 && chargeOnCationNeeded <= maximumCharge) {
        cation->setCharge(chargeOnCationNeeded);
        return true;
    }
    return false;
}

Element *
StructureBuilder::findRightMostGroupInWordOrWordRule(Element *wordOrWordRule) throw(StructureBuildingException) {
    if (wordOrWordRule->getName() == XmlDeclarations::WORDRULE_EL) {
        std::vector<Element *> words = OpsinTools::getDescendantElementsWithTagName(wordOrWordRule,
                                                                                    XmlDeclarations::WORD_EL);
        for (int i = words.size() - 1; i >= 0; i--) { //ignore functionalTerm Words
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            if (words[i]->getAttributeValue(XmlDeclarations::TYPE_ATR) == WordType::functionalTerm->toString()) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                words.remove(words[i]);
            }
        }
        if (words.empty()) {
            throw StructureBuildingException(L"OPSIN bug: word element not found where expected");
        }
        return StructureBuildingMethods::findRightMostGroupInBracket(words[words.size() - 1]);
    } else if (wordOrWordRule->getName() ==
               XmlDeclarations::WORD_EL) { //word element can be treated just like a bracket
        return StructureBuildingMethods::findRightMostGroupInBracket(wordOrWordRule);
    } else {
        throw StructureBuildingException(L"OPSIN bug: expected word or wordRule");
    }
}

void StructureBuilder::processOxidoAndMethionineSpecialCases(std::vector<Element *> &groups) {
    for (auto group : groups) {
        std::wstring subType = group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
        if (XmlDeclarations::OXIDOLIKE_SUBTYPE_VAL == subType) {
            Atom *oxidoAtom = group->getFrag()->getFirstAtom();
            Atom *connectedAtom = oxidoAtom->getAtomNeighbours()[0];
            ChemEl chemEl = connectedAtom->getElement();
            if (checkForConnectedOxo(connectedAtom)) { //e.g. not oxido(trioxo)ruthenium
                continue;
            }
            if (XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL == connectedAtom->getFrag()->getSubType() ||
                ((chemEl == ChemEl::S || chemEl == ChemEl::P) && connectedAtom->getCharge() == 0 &&
                 ValencyChecker::checkValencyAvailableForBond(connectedAtom, 1))) {
                oxidoAtom->neutraliseCharge();
                oxidoAtom->getFirstBond()->setOrder(2);
            } else if (chemEl == ChemEl::N && connectedAtom->getCharge() == 0) {
                int incomingValency = connectedAtom->getIncomingValency();
                if ((incomingValency + connectedAtom->getOutValency()) == 3 && connectedAtom->hasSpareValency()) {
                    connectedAtom->addChargeAndProtons(1, 1); //e.g. N-oxidopyridine
                } else if ((incomingValency + connectedAtom->getOutValency()) == 4) {
                    if (connectedAtom->getLambdaConventionValency() &&
                        connectedAtom->getLambdaConventionValency() == 5) {
                        oxidoAtom->setCharge(0);
                        oxidoAtom->setProtonsExplicitlyAddedOrRemoved(0);
                        oxidoAtom->getFirstBond()->setOrder(2);
                    } else {
                        connectedAtom->addChargeAndProtons(1, 1);
                    }
                }
            }
        } else if (XmlDeclarations::AMINOACID_TYPE_VAL == group->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
            for (auto atom : group->getFrag()->getAtomList()) {
                if (atom->getElement()
                    ::isChalcogen() && atom->getElement() != ChemEl::O && atom->getBondCount() == 3 &&
                    atom->getIncomingValency() == 3 && atom->getCharge() == 0)
                {
                    atom->addChargeAndProtons(1, 1);
                }
            }
        }
    }
}

bool StructureBuilder::checkForConnectedOxo(Atom *atom) {
    std::vector<Bond *> bonds = atom->getBonds();
    for (auto bond : bonds) {
        Atom *connectedAtom;
        if (bond->getFromAtom() == atom) {
            connectedAtom = bond->getToAtom();
        } else {
            connectedAtom = bond->getFromAtom();
        }
        Element *correspondingEl = connectedAtom->getFrag()->getTokenEl();
        if (correspondingEl->getValue() == L"oxo") {
            return true;
        }
    }
    return false;
}

void StructureBuilder::processOxidationNumbers(std::vector<Element *> &groups) throw(StructureBuildingException) {
    for (auto group : groups) {
        if (XmlDeclarations::ELEMENTARYATOM_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
            Atom *atom = group->getFrag()->getFirstAtom();
            if (atom->getProperty(Atom::OXIDATION_NUMBER) != nullptr) {
                std::vector<Atom *> neighbours = atom->getAtomNeighbours();
                int chargeThatWouldFormIfLigandsWereRemoved = 0;
                for (auto neighbour : neighbours) {
                    Element *neighbourEl = neighbour->getFrag()->getTokenEl();
                    Bond *b = atom->getBondToAtomOrThrow(neighbour);
                    //carbonyl and nitrosyl are neutral ligands
                    if (!((neighbourEl->getValue() == L"carbon" && XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL ==
                                                                   neighbourEl->getAttributeValue(
                                                                           XmlDeclarations::TYPE_ATR)) ||
                          neighbourEl->getValue() == L"nitrosyl")) {
                        chargeThatWouldFormIfLigandsWereRemoved += b->getOrder();
                    }
                }

                atom->setCharge(atom->getProperty(Atom::OXIDATION_NUMBER) - chargeThatWouldFormIfLigandsWereRemoved);
            }
        }
    }
}

void StructureBuilder::processStereochemistry(Element *molecule, Fragment *uniFrag) throw(StructureBuildingException) {
    std::vector<Element *> stereoChemistryEls = findStereochemistryElsInProcessingOrder(molecule);
    std::vector<Atom *> atomList = uniFrag->getAtomList();
    std::vector<Atom *> atomsWithPreDefinedAtomParity;
    for (auto atom : atomList) {
        if (atom->getAtomParity() != nullptr) {
            atomsWithPreDefinedAtomParity.push_back(atom);
        }
    }
    Set < Bond * > *bonds = uniFrag->getBondSet();
    std::vector<Bond *> bondsWithPreDefinedBondStereo;
    for (auto bond : bonds) {
        if (bond->getBondStereo() != nullptr) {
            bondsWithPreDefinedBondStereo.push_back(bond);
        }
    }
    if (stereoChemistryEls.size() > 0 || atomsWithPreDefinedAtomParity.size() > 0 ||
        bondsWithPreDefinedBondStereo.size() > 0) {
        StereoAnalyser *stereoAnalyser = new StereoAnalyser(uniFrag);
        std::unordered_map<Atom *, StereoCentre *> atomStereoCentreMap; //contains all atoms that are stereo centres with a mapping to the corresponding StereoCentre object
        std::vector<StereoCentre *> stereoCentres = stereoAnalyser->findStereoCentres();
        for (auto stereoCentre : stereoCentres) {
            atomStereoCentreMap[stereoCentre->getStereoAtom()] = stereoCentre;
        }
        std::unordered_map<Bond *, StereoBond *> bondStereoBondMap;
        std::vector<StereoBond *> stereoBonds = stereoAnalyser->findStereoBonds();
        for (auto stereoBond : stereoBonds) {
            Bond *b = stereoBond->getBond();
            if (FragmentTools::notIn6MemberOrSmallerRing(b)) {
                bondStereoBondMap[b] = stereoBond;
            }
        }
        StereochemistryHandler *stereoChemistryHandler = new StereochemistryHandler(state, atomStereoCentreMap,
                                                                                    bondStereoBondMap);
        stereoChemistryHandler->applyStereochemicalElements(stereoChemistryEls);
        stereoChemistryHandler->removeRedundantStereoCentres(atomsWithPreDefinedAtomParity,
                                                             bondsWithPreDefinedBondStereo);
    }
}

std::vector<Element *> StructureBuilder::findStereochemistryElsInProcessingOrder(Element *parentEl) {
    std::vector<Element *> matchingElements;
    std::vector<Element *> children = parentEl->getChildElements();
    std::vector<Element *> stereochemistryElsAtThisLevel;
    for (int i = children.size() - 1; i >= 0; i--) {
        Element *child = children[i];
        if (child->getName() == XmlDeclarations::STEREOCHEMISTRY_EL) {
            stereochemistryElsAtThisLevel.push_back(child);
        } else {
            matchingElements.addAll(findStereochemistryElsInProcessingOrder(child));
        }
    }
    std::reverse(stereochemistryElsAtThisLevel.begin(), stereochemistryElsAtThisLevel.end());
    matchingElements.insert(matchingElements.end(), stereochemistryElsAtThisLevel.begin(),
                            stereochemistryElsAtThisLevel.end());
    return matchingElements;
}

void StructureBuilder::convertOutAtomsToAttachmentAtoms(Fragment *uniFrag) throw(StructureBuildingException) {
    int outAtomCount = uniFrag->getOutAtomCount();
    for (int i = outAtomCount - 1; i >= 0; i--) {
        OutAtom *outAtom = uniFrag->getOutAtom(i);
        uniFrag->removeOutAtom(i);
        Atom *rGroup = state->fragManager->createAtom(ChemEl::R, uniFrag);
        state->fragManager->createBond(outAtom->getAtom(), rGroup, outAtom->getValency());
    }
}

Atom *StructureBuilder::getOutAtomTakingIntoAccountWhetherSetExplicitly(BuildResults *buildResults,
                                                                        int i) throw(StructureBuildingException) {
    OutAtom *outAtom = buildResults->getOutAtom(i);
    if (outAtom->isSetExplicitly()) {
        return outAtom->getAtom();
    } else {
        return StructureBuildingMethods::findAtomForUnlocantedRadical(state, outAtom->getAtom()->getFrag(), outAtom);
    }
}
