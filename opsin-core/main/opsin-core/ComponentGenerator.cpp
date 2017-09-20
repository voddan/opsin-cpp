#include <utils/RectangularVectors.h>
#include "ComponentGenerator.h"
#include "NameToStructureConfig.h"
#include "ComponentGenerationException.h"
#include "Element.h"
#include "OpsinTools.h"
#include "XmlDeclarations.h"
#include "StringTools.h"
#include "TokenEl.h"
#include "Attribute.h"
#include "GroupingEl.h"
#include "AtomProperties.h"
#include "WordRule.h"



//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

int ComponentGenerator::VonBaeyerSecondaryBridgeSort::compare(std::unordered_map<std::wstring, int> &bridge1,
                                                              std::unordered_map<std::wstring, int> &bridge2) {
    //first we compare the larger coordinate, due to an earlier potential swapping of coordinates this is always in  "AtomId_Larger"
    int largerCoordinate1 = bridge1[L"AtomId_Larger"];
    int largerCoordinate2 = bridge2[L"AtomId_Larger"];
    if (largerCoordinate1 > largerCoordinate2) {
        return -1;
    } else if (largerCoordinate2 > largerCoordinate1) {
        return 1;
    }
    //tie
    int smallerCoordinate1 = bridge1[L"AtomId_Smaller"];
    int smallerCoordinate2 = bridge2[L"AtomId_Smaller"];
    if (smallerCoordinate1 > smallerCoordinate2) {
        return -1;
    } else if (smallerCoordinate2 > smallerCoordinate1) {
        return 1;
    }
    //tie
    int bridgelength1 = bridge1[L"Bridge Length"];
    int bridgelength2 = bridge2[L"Bridge Length"];
    if (bridgelength1 > bridgelength2) {
        return -1;
    } else if (bridgelength2 > bridgelength1) {
        return 1;
    } else {
        return 0;
    }
}

const std::wstring ComponentGenerator::elementSymbols = L"(?:He|Li|Be|B|C|N|O|F|Ne|Na|Mg|Al|Si|P|S|Cl|Ar|K|Ca|Sc|Ti|V|Cr|Mn|Fe|Co|Ni|Cu|Zn|Ga|Ge|As|Se|Br|Kr|Rb|Sr|Y|Zr|Nb|Mo|Tc|Ru|Rh|Pd|Ag|Cd|In|Sn|Sb|Te|I|Xe|Cs|Ba|La|Ce|Pr|Nd|Pm|Sm|Eu|Gd|Tb|Dy|Ho|Er|Tm|Yb|Lu|Hf|Ta|W|Re|Os|Ir|Pt|Au|Hg|Tl|Pb|Po|At|Rn|Fr|Ra|Ac|Th|Pa|U|Np|Pu|Am|Cm|Bk|Cf|Es|Fm|Md|No|Lr|Rf|Db|Sg|Bh|Hs|Mt|Ds)";

ComponentGenerator::ComponentGenerator(NameToStructureConfig *n2sConfig) : n2sConfig(n2sConfig) {
}

void ComponentGenerator::processParse(Element *parse) throw(ComponentGenerationException) {
    std::vector<Element *> substituentsAndRoot = OpsinTools::getDescendantElementsWithTagNames(parse,
                                                                                               std::vector<std::wstring>{
                                                                                                       XmlDeclarations::SUBSTITUENT_EL,
                                                                                                       XmlDeclarations::ROOT_EL});

    for (auto subOrRoot : substituentsAndRoot) {
        /* Throws exceptions for occurrences that are ambiguous and this parse has picked the incorrect interpretation */
        resolveAmbiguities(subOrRoot);

        processLocants(subOrRoot);
        convertOrthoMetaParaToLocants(subOrRoot);
        formAlkaneStemsFromComponents(subOrRoot);
        processAlkaneStemModifications(subOrRoot); //e.g. tert-butyl
        processHeterogenousHydrides(subOrRoot); //e.g. tetraphosphane, disiloxane
        processIndicatedHydrogens(subOrRoot);
        processStereochemistry(subOrRoot);
        processInfixes(subOrRoot);
        processSuffixPrefixes(subOrRoot);
        processLambdaConvention(subOrRoot);
    }
    std::vector<Element *> groups = OpsinTools::getDescendantElementsWithTagName(parse, XmlDeclarations::GROUP_EL);

    /* Converts open/close bracket elements to bracket elements and
							 *  places the elements inbetween within the newly created bracket */
    findAndStructureBrackets(substituentsAndRoot);

    for (auto subOrRoot : substituentsAndRoot) {
        processHydroCarbonRings(subOrRoot);
        handleSuffixIrregularities(subOrRoot); //handles quinone -->dioxo
    }
    for (auto group : groups) {
        detectAlkaneFusedRingBridges(group);
        processRings(group); //processes cyclo, von baeyer and spiro tokens
        handleGroupIrregularities(
                group); //handles benzyl, diethylene glycol, phenanthrone and other awkward bits of nomenclature
    }
}

void ComponentGenerator::resolveAmbiguities(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> multipliers = subOrRoot->getChildElements(XmlDeclarations::MULTIPLIER_EL);
    for (auto apparentMultiplier : multipliers) {
        if (XmlDeclarations::BASIC_TYPE_VAL != apparentMultiplier->getAttributeValue(XmlDeclarations::TYPE_ATR) &&
            XmlDeclarations::VONBAEYER_TYPE_VAL != apparentMultiplier->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
            continue;
        }
        int multiplierNum = std::stoi(apparentMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
        Element *nextEl = OpsinTools::getNextSibling(apparentMultiplier);
        if (multiplierNum >= 3) { //detects ambiguous use of things like tetradeca
            if (nextEl != nullptr) {
                if (nextEl->getName() ==
                    XmlDeclarations::ALKANESTEMCOMPONENT) { //can ignore the trivial alkanes as ambiguity does not exist for them
                    int alkaneChainLength = std::stoi(nextEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
                    if (alkaneChainLength >= 10 && alkaneChainLength > multiplierNum) {
                        Element *isThisALocant = OpsinTools::getPreviousSibling(apparentMultiplier);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                        if (isThisALocant == nullptr || isThisALocant->getName() != XmlDeclarations::LOCANT_EL ||
                            isThisALocant->getValue().split(L",")->length != multiplierNum) {
                            throw ComponentGenerationException(apparentMultiplier->getValue() + nextEl->getValue() +
                                                               std::wstring(
                                                                       L" should not have been lexed as two tokens!"));
                        }
                    }
                }
            }
        }

        if (multiplierNum >= 4 && nextEl != nullptr &&
            nextEl->getName() == XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL && nextEl->getValue() == L"phen" &&
            (std::wstring(L"e")) != nextEl->getAttributeValue(
                    XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR)) { //deals with tetra phenyl vs tetraphen yl
            Element *possibleLocantOrMultiplierOrSuffix = OpsinTools::getNextSibling(nextEl);
            if (possibleLocantOrMultiplierOrSuffix != nullptr) { //null if not used as substituent
                if (possibleLocantOrMultiplierOrSuffix->getName() ==
                    XmlDeclarations::SUFFIX_EL) { //for phen the aryl substituent, expect an adjacent suffix e.g. phenyl, phenoxy
                    Element *isThisALocant = OpsinTools::getPreviousSibling(apparentMultiplier);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                    if (isThisALocant == nullptr || isThisALocant->getName() != XmlDeclarations::LOCANT_EL ||
                        isThisALocant->getValue().split(L",")->length != 1) {
                        std::wstring multiplierAndGroup = apparentMultiplier->getValue() + nextEl->getValue();
                        throw ComponentGenerationException(
                                multiplierAndGroup + std::wstring(L" should not have been lexed as one token!"));
                    }
                }
            }
        }
        if (multiplierNum > 4 && !apparentMultiplier->getValue().endsWith(
                L"a")) { //disambiguate pent oxy and the like. Assume it means pentanoxy rather than 5 oxys
            if (nextEl != nullptr && nextEl->getName() == XmlDeclarations::GROUP_EL &&
                matchInlineSuffixesThatAreAlsoGroups->matcher(nextEl->getValue()).matches()) {
                throw ComponentGenerationException(apparentMultiplier->getValue() + nextEl->getValue() + std::wstring(
                        L" should have been lexed as [alkane stem, inline suffix], not [multiplier, group]!"));
            }
        }
    }

    std::vector<Element *> fusions = subOrRoot->getChildElements(XmlDeclarations::FUSION_EL);
    for (auto fusion : fusions) {
        std::wstring fusionText = fusion->getValue();
        if (matchNumberLocantsOnlyFusionBracket->matcher(fusionText).matches()) {
            Element *possibleHWRing = OpsinTools::getNextSiblingIgnoringCertainElements(fusion,
                                                                                        std::vector<std::wstring>{
                                                                                                XmlDeclarations::MULTIPLIER_EL,
                                                                                                XmlDeclarations::HETEROATOM_EL});
            if (possibleHWRing != nullptr && XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL ==
                                             possibleHWRing->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                int heteroCount = 0;
                int multiplierValue = 1;
                Element *currentElem = OpsinTools::getNextSibling(fusion);
                while (currentElem != nullptr && currentElem->getName() != XmlDeclarations::GROUP_EL) {
                    if (currentElem->getName() == XmlDeclarations::HETEROATOM_EL) {
                        heteroCount += multiplierValue;
                        multiplierValue = 1;
                    } else if (currentElem->getName() == XmlDeclarations::MULTIPLIER_EL) {
                        multiplierValue = std::stoi(currentElem->getAttributeValue(XmlDeclarations::VALUE_ATR));
                    }
                    currentElem = OpsinTools::getNextSibling(currentElem);
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                std::vector<std::wstring> locants = fusionText.substr(1, (fusionText.length() - 1) - 1)->split(L",");
                if (locants.size() == heteroCount) {
                    bool foundLocantNotInHwSystem = false;
                    for (auto locant : locants) {
                        if (std::stoi(locant) >
                            (possibleHWRing->getAttributeValue(XmlDeclarations::VALUE_ATR).length() - 2)) {
                            foundLocantNotInHwSystem = true;
                        }
                    }
                    if (!foundLocantNotInHwSystem) {
                        throw ComponentGenerationException(
                                L"This fusion bracket is in fact more likely to be a description of the locants of a HW ring");
                    }
                }
            }
        }
    }
}

void ComponentGenerator::processLocants(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> locants = subOrRoot->getChildElements(XmlDeclarations::LOCANT_EL);
    for (auto locant : locants) {
        std::vector<std::wstring> individualLocants = splitIntoIndividualLocants(
                StringTools::removeDashIfPresent(locant->getValue()));
        for (int i = 0, locantCount = individualLocants.size(); i < locantCount; i++) {
            std::wstring locantText = individualLocants[i];

            if (locantText.find(L"-") != std::wstring::npos) { //avoids this regex being invoked typically
                //rearranges locant to the older equivalent form
                Matcher *m = matchIUPAC2004ElementLocant->matcher(locantText);
                if (m->matches()) {
                    locantText = m->group(2) + m->group(1) + m->group(3);
                }
            }

            if (std::isalpha(locantText[0])) {
                //remove indications of superscript as the fact a locant is superscripted can be determined from context e.g. N~1~ ->N1
                Matcher *m = matchSuperscriptedLocant->matcher(locantText);
                if (m->lookingAt()) {
                    std::wstring replacementString = m->group(1) + m->group(2);
                    locantText = m->replaceFirst(replacementString);
                }
                if (locantText.length() >= 3) {
                    //convert greeks to lower case
                    m = matchGreek->matcher(locantText);
                    while (m->find()) {
                        locantText = locantText.substr(0, m->start()) + m->group()->toLowerCase(Locale::ROOT) +
                                     locantText.substr(m->end());
                    }
                }
            }
            wchar_t lastChar = locantText[locantText.length() - 1];
            if (lastChar == L')' || lastChar == L']' || lastChar == L'}') {
                //stereochemistry or added hydrogen that result from the application of this locant as a locant for a substituent may be included in brackets after the locant

                Matcher *m = matchBracketAtEndOfLocant->matcher(locantText);
                if (m->find()) {
                    std::wstring brackettedText = m->group(1);
                    if (StringTools::endsWithCaseInsensitive(brackettedText, L"H")) {
                        locantText = m->replaceFirst(L""); //strip the bracket from the locantText
                        //create individual tags for added hydrogen. Examples of bracketed text include "9H" or "2H,7H"
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                        std::vector<std::wstring> addedHydrogens = brackettedText.split(L",");
                        for (auto addedHydrogen : addedHydrogens) {
                            Element *addedHydrogenElement = new TokenEl(XmlDeclarations::ADDEDHYDROGEN_EL);
                            Attribute tempVar(XmlDeclarations::LOCANT_ATR,
                                              addedHydrogen.substr(0, addedHydrogen.length() - 1));
                            addedHydrogenElement->addAttribute(&tempVar);
                            OpsinTools::insertBefore(locant, addedHydrogenElement);
                        }
                        if (locant->getAttribute(XmlDeclarations::TYPE_ATR) == nullptr) {
                            Attribute tempVar2(XmlDeclarations::TYPE_ATR,
                                               XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL);
                            locant->addAttribute(&tempVar2); //this locant must not be used as an indirect locant
                        }
                    } else if (StringTools::endsWithCaseInsensitive(brackettedText, L"R") ||
                               StringTools::endsWithCaseInsensitive(brackettedText, L"S")) {
                        locantText = m->replaceFirst(L""); //strip the bracket from the locantText
                        std::wstring rs;
                        if (brackettedText.length() == 3 && (brackettedText[1] == L',' || brackettedText[1] == L'/')) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            StringBuilder tempVar3(2);
                            rs = (&tempVar3)->append(brackettedText[0])->append(brackettedText[2])->toString();
                        } else {
                            rs = brackettedText;
                        }

                        Element *newStereoChemEl = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL,
                                                               std::wstring(L"(") + locantText + rs +
                                                               std::wstring(L")"));
                        Attribute tempVar4(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
                        newStereoChemEl->addAttribute(&tempVar4);
                        OpsinTools::insertBefore(locant, newStereoChemEl);
                    } else if (matchDigit->matcher(brackettedText).matches()) {
                        //compounds locant e.g. 1(10). Leave as is, it will be handled by the function that handles unsaturation
                    } else {
                        throw ComponentGenerationException(L"OPSIN bug: malformed locant text");
                    }
                } else {
                    throw ComponentGenerationException(L"OPSIN bug: malformed locant text");
                }

            }
            individualLocants[i] = locantText;
        }

        locant->setValue(StringTools::stringListToString(individualLocants, L","));

        Element *afterLocants = OpsinTools::getNextSibling(locant);
        if (afterLocants == nullptr) {
            throw ComponentGenerationException(std::wstring(L"Nothing after locant tag: ") + locant->toXML());
        }

        if (individualLocants.size() == 1) {
            ifCarbohydrateLocantConvertToAminoAcidStyleLocant(locant);
        }
    }
}

void ComponentGenerator::ifCarbohydrateLocantConvertToAminoAcidStyleLocant(Element *locant) {
    if (OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(locant->getValue()).matches()) {
        Element *possibleMultiplier = OpsinTools::getPreviousSibling(locant);
        if (possibleMultiplier != nullptr && possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
            int multiplierValue = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            Element *possibleOtherLocant = OpsinTools::getPreviousSibling(possibleMultiplier);
            if (possibleOtherLocant != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                std::vector<std::wstring> locantValues = possibleOtherLocant->getValue().split(L",");
                if (locantValues.size() ==
                    std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR))) {
                    for (int i = 0; i < locantValues.size(); i++) {
                        locantValues[i] = locant->getValue() + locantValues[i];
                    }
                    possibleOtherLocant->setValue(StringTools::arrayToString(locantValues, L","));
                    locant->detach();
                }
            } else {
                StringBuilder *sb = new StringBuilder();
                for (int i = 0; i < multiplierValue - 1; i++) {
                    sb->append(locant->getValue());
                    sb->append(StringTools::multiplyString(L"'", i));
                    sb->append(L',');
                }
                sb->append(locant->getValue());
                sb->append(StringTools::multiplyString(L"'", multiplierValue - 1));
                Element *newLocant = new TokenEl(XmlDeclarations::LOCANT_EL, sb->toString());
                OpsinTools::insertBefore(possibleMultiplier, newLocant);
                locant->detach();
            }
        }
    }
}

std::vector<std::wstring> ComponentGenerator::splitIntoIndividualLocants(const std::wstring &locantString) {
    std::vector<std::wstring> individualLocants;
    std::vector<wchar_t> charArray = locantString.toCharArray();
    bool inBracket = false;
    int indiceOfLastMatch = 0;
    for (int i = 0; i < charArray.size(); i++) {
        wchar_t c = charArray[i];
        if (c == L',') {
            if (!inBracket) {
                individualLocants.push_back(locantString.substr(indiceOfLastMatch, i - indiceOfLastMatch));
                indiceOfLastMatch = i + 1;
            }
        } else if (c == L'(' || c == L'[' || c == L'{') {
            inBracket = true;
        } else if (c == L')' || c == L']' || c == L'}') {
            inBracket = false;
        }
    }
    individualLocants.push_back(locantString.substr(indiceOfLastMatch, charArray.size() - indiceOfLastMatch));
    return individualLocants;
}

void ComponentGenerator::convertOrthoMetaParaToLocants(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> ompLocants = subOrRoot->getChildElements(XmlDeclarations::ORTHOMETAPARA_EL);
    for (auto ompLocant : ompLocants) {
        std::wstring locantText = ompLocant->getValue();
        std::wstring firstChar = locantText.substr(0, 1);
        ompLocant->setName(XmlDeclarations::LOCANT_EL);
        Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ORTHOMETAPARA_TYPE_VAL);
        ompLocant->addAttribute(&tempVar);
        if (orthoMetaParaLocantIsTwoLocants(ompLocant)) {
            if ((std::wstring(L"o")).equalsIgnoreCase(firstChar)) {
                ompLocant->setValue(L"1,ortho");
            } else if ((std::wstring(L"m")).equalsIgnoreCase(firstChar)) {
                ompLocant->setValue(L"1,meta");
            } else if ((std::wstring(L"p")).equalsIgnoreCase(firstChar)) {
                ompLocant->setValue(L"1,para");
            } else {
                throw ComponentGenerationException(locantText + std::wstring(
                        L" was not identified as being either ortho, meta or para but according to the chemical grammar it should of been"));
            }
        } else {
            if ((std::wstring(L"o")).equalsIgnoreCase(firstChar)) {
                ompLocant->setValue(L"ortho");
            } else if ((std::wstring(L"m")).equalsIgnoreCase(firstChar)) {
                ompLocant->setValue(L"meta");
            } else if ((std::wstring(L"p")).equalsIgnoreCase(firstChar)) {
                ompLocant->setValue(L"para");
            } else {
                throw ComponentGenerationException(locantText + std::wstring(
                        L" was not identified as being either ortho, meta or para but according to the chemical grammar it should of been"));
            }
        }
    }
}

bool ComponentGenerator::orthoMetaParaLocantIsTwoLocants(Element *ompLocant) {
    Element *afterOmpLocant = OpsinTools::getNextSibling(ompLocant);
    if (afterOmpLocant != nullptr) {
        std::wstring elName = afterOmpLocant->getName();
        if (elName == XmlDeclarations::MULTIPLIER_EL &&
            afterOmpLocant->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"2") {
            //e.g. p-dimethyl
            return true;
        }
        std::wstring outIds = afterOmpLocant->getAttributeValue(XmlDeclarations::OUTIDS_ATR);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        if (outIds != L"" && outIds.split(L",")->length > 1) {
            //e.g. p-phenylene
            return true;
        }
        if (elName == XmlDeclarations::GROUP_EL) {
            Element *multiplier = OpsinTools::getNextSibling(afterOmpLocant);
            if (multiplier != nullptr && multiplier->getName() == XmlDeclarations::MULTIPLIER_EL &&
                multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"2") {
                Element *suffix = OpsinTools::getNextSiblingIgnoringCertainElements(multiplier,
                                                                                    std::vector<std::wstring>{
                                                                                            XmlDeclarations::INFIX_EL,
                                                                                            XmlDeclarations::SUFFIXPREFIX_EL});
                if (suffix->getName() == XmlDeclarations::SUFFIX_EL) {
                    //e.g. o-benzenediamine
                    return true;
                }
            }
        }
    }
    return false;
}

void ComponentGenerator::formAlkaneStemsFromComponents(Element *subOrRoot) {
    Deque < Element * > *alkaneStemComponents = new ArrayDeque<Element *>(
            subOrRoot->getChildElements(XmlDeclarations::ALKANESTEMCOMPONENT));
    while (!alkaneStemComponents->isEmpty()) {
        Element *alkaneStemComponent = alkaneStemComponents->removeFirst();
        int alkaneChainLength = 0;
        StringBuilder *alkaneName = new StringBuilder();
        alkaneChainLength += std::stoi(alkaneStemComponent->getAttributeValue(XmlDeclarations::VALUE_ATR));
        alkaneName->append(alkaneStemComponent->getValue());
        while (!alkaneStemComponents->isEmpty() &&
               OpsinTools::getNextSibling(alkaneStemComponent) == alkaneStemComponents->getFirst()) {
            alkaneStemComponent->detach();
            alkaneStemComponent = alkaneStemComponents->removeFirst();
            alkaneChainLength += std::stoi(alkaneStemComponent->getAttributeValue(XmlDeclarations::VALUE_ATR));
            alkaneName->append(alkaneStemComponent->getValue());
        }
        Element *alkaneStem = new TokenEl(XmlDeclarations::GROUP_EL, alkaneName->toString());
        Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::CHAIN_TYPE_VAL);
        alkaneStem->addAttribute(&tempVar);
        Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::ALKANESTEM_SUBTYPE_VAL);
        alkaneStem->addAttribute(&tempVar2);
        Attribute tempVar3(XmlDeclarations::VALUE_ATR, StringTools::multiplyString(L"C", alkaneChainLength));
        alkaneStem->addAttribute(&tempVar3);
        Attribute tempVar4(XmlDeclarations::USABLEASJOINER_ATR, L"yes");
        alkaneStem->addAttribute(&tempVar4);
        Attribute tempVar5(XmlDeclarations::LABELS_ATR, XmlDeclarations::NUMERIC_LABELS_VAL);
        alkaneStem->addAttribute(&tempVar5);
        OpsinTools::insertAfter(alkaneStemComponent, alkaneStem);
        alkaneStemComponent->detach();
    }
}

void ComponentGenerator::processAlkaneStemModifications(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> alkaneStemModifiers = subOrRoot->getChildElements(XmlDeclarations::ALKANESTEMMODIFIER_EL);
    for (auto alkaneStemModifier : alkaneStemModifiers) {
        Element *alkane = OpsinTools::getNextSibling(alkaneStemModifier);
        if (alkane == nullptr ||
            XmlDeclarations::CHAIN_TYPE_VAL != alkane->getAttributeValue(XmlDeclarations::TYPE_ATR) ||
            XmlDeclarations::ALKANESTEM_SUBTYPE_VAL != alkane->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
            throw ComponentGenerationException(L"OPSIN Bug: AlkaneStem not found after alkaneStemModifier");
        }
        std::wstring type;
        if (alkaneStemModifier->getAttribute(XmlDeclarations::VALUE_ATR) != nullptr) {
            type = alkaneStemModifier->getAttributeValue(XmlDeclarations::VALUE_ATR); //identified by token;
        } else {
            if (alkaneStemModifier->getValue().equals(L"n-")) {
                type = L"normal";
            } else if (alkaneStemModifier->getValue().equals(L"i-")) {
                type = L"iso";
            } else if (alkaneStemModifier->getValue().equals(L"s-")) {
                type = L"sec";
            } else {
                throw ComponentGenerationException(L"Unrecognised alkaneStem modifier");
            }
        }
        alkaneStemModifier->detach();
        int chainLength = alkane->getAttributeValue(XmlDeclarations::VALUE_ATR).length();
        std::wstring smiles;
        std::wstring labels = XmlDeclarations::NONE_LABELS_VAL;
        if (type == L"normal") {
            //normal behaviour is default so don't need to do anything
            //n-methyl and n-ethyl contain redundant information and are probably intended to mean N-methyl/N-ethyl
            if ((chainLength == 1 || chainLength == 2) && alkaneStemModifier->getValue().equals(L"n-")) {
                Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"N");
                OpsinTools::insertBefore(alkane, locant);
            }
            continue;
        } else if (type == L"tert") {
            if (chainLength < 4) {
                throw ComponentGenerationException(
                        std::wstring(L"ChainLength to small for tert modifier, required minLength 4. Found: ") +
                        std::to_wstring(chainLength));
            }
            if (chainLength > 8) {
                throw ComponentGenerationException(
                        std::wstring(L"Interpretation of tert on an alkane chain of length: ") + chainLength +
                        std::wstring(L" is ambiguous"));
            }
            if (chainLength == 8) {
                smiles = L"C(C)(C)CC(C)(C)C";
            } else {
                smiles = std::wstring(L"C(C)(C)C") + StringTools::multiplyString(L"C", chainLength - 4);
            }
        } else if (type == L"iso") {
            if (chainLength < 3) {
                throw ComponentGenerationException(
                        std::wstring(L"ChainLength to small for iso modifier, required minLength 3. Found: ") +
                        std::to_wstring(chainLength));
            }
            bool suffixPresent = subOrRoot->getChildElements(XmlDeclarations::SUFFIX_EL).size() > 0;
            if (chainLength == 3 && !suffixPresent) {
                throw ComponentGenerationException(
                        L"iso has no meaning without a suffix on an alkane chain of length 3");
            }
            if (chainLength == 8 && !suffixPresent) {
                smiles = L"C(C)(C)CC(C)(C)C";
            } else {
                smiles = StringTools::multiplyString(L"C", chainLength - 3) + std::wstring(L"C(C)C");
                StringBuilder *sb = new StringBuilder();
                for (int c = 1; c <= chainLength - 2; c++) {
                    sb->append(c);
                    sb->append(L'/');
                }
                sb->append(L'/');
                labels = sb->toString();
            }
        } else if (type == L"sec") {
            if (chainLength < 3) {
                throw ComponentGenerationException(
                        std::wstring(L"ChainLength to small for sec modifier, required minLength 3. Found: ") +
                        std::to_wstring(chainLength));
            }
            bool suffixPresent = subOrRoot->getChildElements(XmlDeclarations::SUFFIX_EL).size() > 0;
            if (!suffixPresent) {
                throw ComponentGenerationException(L"sec has no meaning without a suffix on an alkane chain");
            }
            smiles = std::wstring(L"C(C)C") + StringTools::multiplyString(L"C", chainLength - 3);
        } else if (type == L"neo") {
            if (chainLength < 5) {
                throw ComponentGenerationException(
                        std::wstring(L"ChainLength to small for neo modifier, required minLength 5. Found: ") +
                        std::to_wstring(chainLength));
            }
            smiles = StringTools::multiplyString(L"C", chainLength - 5) + std::wstring(L"CC(C)(C)C");
        } else {
            throw ComponentGenerationException(L"Unrecognised alkaneStem modifier");
        }
        alkane->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles);
        alkane->removeAttribute(alkane->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
        alkane->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(labels);
    }
}

void ComponentGenerator::processHeterogenousHydrides(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> multipliers = subOrRoot->getChildElements(XmlDeclarations::MULTIPLIER_EL);
    for (int i = 0; i < multipliers.size(); i++) {
        Element *m = multipliers[i];
        if (m->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::GROUP_TYPE_VAL) {
            continue;
        }
        Element *multipliedElem = OpsinTools::getNextSibling(m);

        if (multipliedElem->getName() == XmlDeclarations::GROUP_EL &&
            multipliedElem->getAttribute(XmlDeclarations::SUBTYPE_ATR) != nullptr &&
            multipliedElem->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) ==
            XmlDeclarations::HETEROSTEM_SUBTYPE_VAL) {
            int mvalue = std::stoi(m->getAttributeValue(XmlDeclarations::VALUE_ATR));

            Element *possiblyALocant = OpsinTools::getPreviousSibling(
                    m); //detect rare case where multiplier does not mean form a chain of heteroatoms e.g. something like 1,2-disulfanylpropane
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            if (possiblyALocant != nullptr && possiblyALocant->getName() == XmlDeclarations::LOCANT_EL &&
                mvalue == possiblyALocant->getValue().split(L",")->length) {
                Element *suffix = OpsinTools::getNextSibling(multipliedElem, XmlDeclarations::SUFFIX_EL);
                if (suffix != nullptr &&
                    suffix->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::INLINE_TYPE_VAL) {
                    Element *possibleMultiplier = OpsinTools::getPreviousSibling(suffix);
                    if (possibleMultiplier->getName() !=
                        XmlDeclarations::MULTIPLIER_EL) { //NOT something like 3,3'-diselane-1,2-diyl
                        continue;
                    }
                }
            }

            //chain of heteroatoms
            std::wstring heteroatomSmiles = multipliedElem->getAttributeValue(XmlDeclarations::VALUE_ATR);
            if (heteroatomSmiles == L"B" && OpsinTools::getPreviousSibling(m) == nullptr) {
                Element *possibleUnsaturator = OpsinTools::getNextSibling(multipliedElem);
                if (possibleUnsaturator != nullptr &&
                    possibleUnsaturator->getName() == XmlDeclarations::UNSATURATOR_EL &&
                    possibleUnsaturator->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"1") {
                    throw ComponentGenerationException(L"Polyboranes are not currently supported");
                }
            }
            std::wstring smiles = StringTools::multiplyString(heteroatomSmiles, mvalue);
            multipliedElem->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles);
            m->detach();
            multipliers.erase(multipliers.begin() + i--);
        }
    }
    for (auto m : multipliers) {
        if (m->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(XmlDeclarations::GROUP_TYPE_VAL)) {
            continue;
        }
        Element *multipliedElem = OpsinTools::getNextSibling(m);
        if (multipliedElem->getName() == XmlDeclarations::HETEROATOM_EL) {
            Element *possiblyAnotherHeteroAtom = OpsinTools::getNextSibling(multipliedElem);
            if (possiblyAnotherHeteroAtom != nullptr &&
                possiblyAnotherHeteroAtom->getName() == XmlDeclarations::HETEROATOM_EL) {
                Element *possiblyAnUnsaturator = OpsinTools::getNextSiblingIgnoringCertainElements(
                        possiblyAnotherHeteroAtom, std::vector<std::wstring>{XmlDeclarations::LOCANT_EL,
                                                                             XmlDeclarations::MULTIPLIER_EL}); //typically ane but can be ene or yne e.g. triphosphaza-1,3-diene
                if (possiblyAnUnsaturator != nullptr &&
                    possiblyAnUnsaturator->getName() == XmlDeclarations::UNSATURATOR_EL) {
                    StringBuilder *newGroupName = new StringBuilder(m->getValue());
                    newGroupName->append(multipliedElem->getValue());
                    newGroupName->append(possiblyAnotherHeteroAtom->getValue());
                    //chain of alternating heteroatoms
                    if (possiblyAnUnsaturator->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"1") {
                        checkForAmbiguityWithHWring(multipliedElem->getAttributeValue(XmlDeclarations::VALUE_ATR),
                                                    possiblyAnotherHeteroAtom->getAttributeValue(
                                                            XmlDeclarations::VALUE_ATR));
                    }
                    int mvalue = std::stoi(m->getAttributeValue(XmlDeclarations::VALUE_ATR));
                    StringBuilder *smilesSB = new StringBuilder();
                    Element *possiblyARingFormingEl = OpsinTools::getPreviousSibling(m);
                    bool heteroatomChainWillFormARing = false;
                    if (possiblyARingFormingEl != nullptr &&
                        (possiblyARingFormingEl->getName() == XmlDeclarations::CYCLO_EL ||
                         possiblyARingFormingEl->getName() == XmlDeclarations::VONBAEYER_EL ||
                         possiblyARingFormingEl->getName() == XmlDeclarations::SPIRO_EL)) {
                        heteroatomChainWillFormARing = true;
                        //will be cyclised later.
                        for (int j = 0; j < mvalue; j++) {
                            smilesSB->append(possiblyAnotherHeteroAtom->getAttributeValue(XmlDeclarations::VALUE_ATR));
                            smilesSB->append(multipliedElem->getAttributeValue(XmlDeclarations::VALUE_ATR));
                        }
                    } else {
                        for (int j = 0; j < mvalue - 1; j++) {
                            smilesSB->append(multipliedElem->getAttributeValue(XmlDeclarations::VALUE_ATR));
                            smilesSB->append(possiblyAnotherHeteroAtom->getAttributeValue(XmlDeclarations::VALUE_ATR));
                        }
                        smilesSB->append(multipliedElem->getAttributeValue(XmlDeclarations::VALUE_ATR));
                    }
                    std::wstring smiles = smilesSB->toString();
                    smiles = matchHdigit->matcher(smiles).replaceAll(
                            L"H?"); //hydrogen count will be determined by standard valency
                    multipliedElem->detach();

                    Element *addedGroup = new TokenEl(XmlDeclarations::GROUP_EL, newGroupName->toString());
                    Attribute tempVar(XmlDeclarations::VALUE_ATR, smiles);
                    addedGroup->addAttribute(&tempVar);
                    Attribute tempVar2(XmlDeclarations::LABELS_ATR, XmlDeclarations::NUMERIC_LABELS_VAL);
                    addedGroup->addAttribute(&tempVar2);
                    Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::CHAIN_TYPE_VAL);
                    addedGroup->addAttribute(&tempVar3);
                    Attribute tempVar4(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::HETEROSTEM_SUBTYPE_VAL);
                    addedGroup->addAttribute(&tempVar4);
                    if (!heteroatomChainWillFormARing) {
                        Attribute tempVar5(XmlDeclarations::USABLEASJOINER_ATR, L"yes");
                        addedGroup->addAttribute(&tempVar5);
                    }
                    OpsinTools::insertAfter(possiblyAnotherHeteroAtom, addedGroup);

                    possiblyAnotherHeteroAtom->detach();
                    m->detach();
                } else if (possiblyAnUnsaturator != nullptr && possiblyAnUnsaturator->getValue() == L"an" &&
                           XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL ==
                           possiblyAnUnsaturator->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                    //check for HWring that should be interpreted as a heterogenous hydride
                    bool foundLocantIndicatingHwRingHeteroatomPositions = false; //allow formally incorrect HW ring systems if they have locants
                    Element *possibleLocant = OpsinTools::getPreviousSibling(m);
                    if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL) {
                        int expected = std::stoi(m->getAttributeValue(XmlDeclarations::VALUE_ATR)) + 1;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                        if (expected == possibleLocant->getValue().split(L",")->length) {
                            foundLocantIndicatingHwRingHeteroatomPositions = true;
                        }
                    }
                    if (!foundLocantIndicatingHwRingHeteroatomPositions) {
                        checkForAmbiguityWithHeterogenousHydride(
                                multipliedElem->getAttributeValue(XmlDeclarations::VALUE_ATR),
                                possiblyAnotherHeteroAtom->getAttributeValue(XmlDeclarations::VALUE_ATR));
                    }
                }
            }
        }
    }
}

void ComponentGenerator::checkForAmbiguityWithHWring(const std::wstring &firstHeteroAtomSMILES,
                                                     const std::wstring &secondHeteroAtomSMILES) throw(ComponentGenerationException) {
    Matcher *m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(firstHeteroAtomSMILES);
    if (!m->find()) {
        throw ComponentGenerationException(L"Failed to extract element from heteroatom");
    }
    ChemEl atom1ChemEl = static_cast<ChemEl>(m->group());

    m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(secondHeteroAtomSMILES);
    if (!m->find()) {
        throw ComponentGenerationException(L"Failed to extract element from heteroatom");
    }
    ChemEl atom2ChemEl = static_cast<ChemEl>(m->group());
    if (AtomProperties::getHwpriority(atom1ChemEl) > AtomProperties::getHwpriority(atom2ChemEl)) {
        if (atom2ChemEl == ChemEl::O || atom2ChemEl == ChemEl::S || atom2ChemEl == ChemEl::Se ||
            atom2ChemEl == ChemEl::Te || atom2ChemEl == ChemEl::Bi || atom2ChemEl == ChemEl::Hg) {
            if (!hasSiorGeorSnorPb(atom1ChemEl, atom2ChemEl)) {
                throw ComponentGenerationException(
                        L"Hantzch-widman ring misparsed as a heterogeneous hydride with alternating atoms");
            }
        }
    }
}

bool ComponentGenerator::hasSiorGeorSnorPb(ChemEl atom1ChemEl, ChemEl atom2ChemEl) {
    return (atom1ChemEl == ChemEl::Si || atom1ChemEl == ChemEl::Ge || atom1ChemEl == ChemEl::Sn ||
            atom1ChemEl == ChemEl::Pb || atom2ChemEl == ChemEl::Si || atom2ChemEl == ChemEl::Ge ||
            atom2ChemEl == ChemEl::Sn || atom2ChemEl == ChemEl::Pb);
}

void ComponentGenerator::checkForAmbiguityWithHeterogenousHydride(const std::wstring &firstHeteroAtomSMILES,
                                                                  const std::wstring &secondHeteroAtomSMILES) throw(ComponentGenerationException) {
    Matcher *m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(firstHeteroAtomSMILES);
    if (!m->find()) {
        throw ComponentGenerationException(L"Failed to extract element from heteroatom");
    }
    std::wstring atom1Element = m->group();

    m = OpsinTools::MATCH_ELEMENT_SYMBOL->matcher(secondHeteroAtomSMILES);
    if (!m->find()) {
        throw ComponentGenerationException(L"Failed to extract element from heteroatom");
    }
    std::wstring atom2Element = m->group();
    if (AtomProperties::getHwpriority(StringHelper::fromString<ChemEl>(atom2Element)) >
        AtomProperties::getHwpriority(StringHelper::fromString<ChemEl>(atom1Element))) {
        throw ComponentGenerationException(
                L"heterogeneous hydride with alternating atoms misparsed as a Hantzch-widman ring");
    }
}

void ComponentGenerator::processIndicatedHydrogens(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> indicatedHydrogens = subOrRoot->getChildElements(XmlDeclarations::INDICATEDHYDROGEN_EL);
    for (auto indicatedHydrogenGroup : indicatedHydrogens) {
        std::wstring txt = StringTools::removeDashIfPresent(indicatedHydrogenGroup->getValue());
        if (!StringTools::endsWithCaseInsensitive(txt, L"h")) { //remove brackets if they are present
            txt = txt.substr(1, (txt.length() - 1) - 1);
        }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> hydrogenLocants = txt.split(L",");
        for (auto hydrogenLocant : hydrogenLocants) {
            if (StringTools::endsWithCaseInsensitive(hydrogenLocant, L"h")) {
                Element *indicatedHydrogenEl = new TokenEl(XmlDeclarations::INDICATEDHYDROGEN_EL);
                Attribute tempVar(XmlDeclarations::LOCANT_ATR, hydrogenLocant.substr(0, hydrogenLocant.length() - 1));
                indicatedHydrogenEl->addAttribute(&tempVar);
                OpsinTools::insertBefore(indicatedHydrogenGroup, indicatedHydrogenEl);
            } else {
                throw ComponentGenerationException(L"OPSIN Bug: malformed indicated hydrogen element!");
            }
        }
        indicatedHydrogenGroup->detach();
    }
}

void ComponentGenerator::processStereochemistry(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> stereoChemistryElements = subOrRoot->getChildElements(XmlDeclarations::STEREOCHEMISTRY_EL);
    std::vector<Element *> locantedUnbrackettedEzTerms;
    for (auto stereoChemistryElement : stereoChemistryElements) {
        if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL)) {
            processStereochemistryBracket(stereoChemistryElement);
        } else if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::CISORTRANS_TYPE_VAL)) {
            assignLocantUsingPreviousElementIfPresent(
                    stereoChemistryElement); //assign a locant if one is directly before the cis/trans
        } else if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::E_OR_Z_TYPE_VAL)) {
            Attribute tempVar(XmlDeclarations::VALUE_ATR, stereoChemistryElement->getValue().toUpperCase(Locale::ROOT));
            stereoChemistryElement->addAttribute(&tempVar);
            if (assignLocantUsingPreviousElementIfPresent(
                    stereoChemistryElement)) { //assign a locant if one is directly before the E/Z
                locantedUnbrackettedEzTerms.push_back(stereoChemistryElement);
            }
        } else if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL)) {
            processLocantAssigningForEndoExoSynAnti(
                    stereoChemistryElement); //assign a locant if one is directly before the endo/exo/syn/anti. Don't neccesarily detach it
        } else if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL)) {
            processUnbracketedAlphaBetaStereochemistry(stereoChemistryElement);
        } else if (stereoChemistryElement->getAttributeValue(XmlDeclarations::TYPE_ATR).equals(
                XmlDeclarations::RELATIVECISTRANS_TYPE_VAL)) {
            processRelativeCisTrans(stereoChemistryElement);
        }
    }
    if (locantedUnbrackettedEzTerms.size() > 0) {
        duplicateLocantFromStereoTermIfAdjacentToEneOrYlidene(locantedUnbrackettedEzTerms);
    }
}

void
ComponentGenerator::processStereochemistryBracket(Element *stereoChemistryElement) throw(ComponentGenerationException) {
    std::wstring txt = stereoChemistryElement->getValue();
    if (StringTools::startsWithCaseInsensitive(txt, L"rel-")) {
        txt = txt.substr(4);
    }
    txt = StringTools::removeDashIfPresent(txt);
    txt = matchStar->matcher(txt).replaceAll(L"");
    bool racemicStereo;
    Matcher *racemicMacher = matchRacemic->matcher(txt);
    if (racemicMacher->lookingAt()) {
        txt = txt.substr(racemicMacher->group()->length());
        racemicStereo = true;
    } else {
        racemicStereo = false;
    }
    if (txt.length() > 0) { //if txt is just "rel- or rac-" then it will be length 0 at this point
        std::vector<std::wstring> stereoChemistryDescriptors = splitStereoBracketIntoDescriptors(txt);
        bool exclusiveStereoTerm = false;
        if (stereoChemistryDescriptors.size() == 1) {
            std::wstring stereoChemistryDescriptor = stereoChemistryDescriptors[0];
            if (stereoChemistryDescriptor.equalsIgnoreCase(L"rel")) {
                exclusiveStereoTerm = true;
            }
            if (matchRacemic->matcher(stereoChemistryDescriptor).matches()) {
                racemicStereo = true;
                exclusiveStereoTerm = true;
            }
        }
        if (!exclusiveStereoTerm) {
            for (auto stereoChemistryDescriptor : stereoChemistryDescriptors) {
                Matcher *m = matchStereochemistry->matcher(stereoChemistryDescriptor);
                if (m->matches()) {
                    Element *stereoChemEl = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, stereoChemistryDescriptor);
                    std::wstring locantVal = m->group(1);
                    if (locantVal.length() > 0) {
                        Attribute tempVar(XmlDeclarations::LOCANT_ATR, StringTools::removeDashIfPresent(locantVal));
                        stereoChemEl->addAttribute(&tempVar);
                    }
                    OpsinTools::insertBefore(stereoChemistryElement, stereoChemEl);
                    if (matchRS->matcher(m->group(2)).matches()) {
                        Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::R_OR_S_TYPE_VAL);
                        stereoChemEl->addAttribute(&tempVar2);
                        std::wstring symbol = m->group(2)->toUpperCase(Locale::ROOT).replaceAll(L"/", L"");
                        if (racemicStereo && symbol.length() == 1) {
                            symbol = (symbol == L"R") ? L"RS" : L"SR";
                        }
                        Attribute tempVar3(XmlDeclarations::VALUE_ATR, symbol);
                        stereoChemEl->addAttribute(&tempVar3);
                    } else if (matchEZ->matcher(m->group(2)).matches()) {
                        Attribute tempVar4(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
                        stereoChemEl->addAttribute(&tempVar4);
                        std::wstring symbol = m->group(2)->toUpperCase(Locale::ROOT);
                        Attribute tempVar5(XmlDeclarations::VALUE_ATR, symbol);
                        stereoChemEl->addAttribute(&tempVar5);
                    } else if (matchAlphaBetaStereochem->matcher(m->group(2)).matches()) {
                        Attribute tempVar6(XmlDeclarations::TYPE_ATR, XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL);
                        stereoChemEl->addAttribute(&tempVar6);
                        if (std::tolower(m->group(2)->charAt(0)) == L'a') {
                            Attribute tempVar7(XmlDeclarations::VALUE_ATR, L"alpha");
                            stereoChemEl->addAttribute(&tempVar7);
                        } else if (std::tolower(m->group(2)->charAt(0)) == L'b') {
                            Attribute tempVar8(XmlDeclarations::VALUE_ATR, L"beta");
                            stereoChemEl->addAttribute(&tempVar8);
                        } else if (std::tolower(m->group(2)->charAt(0)) == L'x') {
                            Attribute tempVar9(XmlDeclarations::VALUE_ATR, L"xi");
                            stereoChemEl->addAttribute(&tempVar9);
                        } else {
                            throw ComponentGenerationException(
                                    std::wstring(L"Malformed alpha/beta stereochemistry element: ") +
                                    stereoChemistryElement->getValue());
                        }
                    } else if (matchCisTrans->matcher(m->group(2)).matches()) {
                        Attribute tempVar10(XmlDeclarations::TYPE_ATR, XmlDeclarations::CISORTRANS_TYPE_VAL);
                        stereoChemEl->addAttribute(&tempVar10);
                        Attribute tempVar11(XmlDeclarations::VALUE_ATR, m->group(2)->toLowerCase(Locale::ROOT));
                        stereoChemEl->addAttribute(&tempVar11);
                    } else if (matchEndoExoSynAnti->matcher(m->group(2)).matches()) {
                        Attribute tempVar12(XmlDeclarations::TYPE_ATR, XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL);
                        stereoChemEl->addAttribute(&tempVar12);
                        Attribute tempVar13(XmlDeclarations::VALUE_ATR, m->group(2)->toLowerCase(Locale::ROOT));
                        stereoChemEl->addAttribute(&tempVar13);
                    } else if (matchAxialStereo->matcher(m->group(2)).matches()) {
                        Attribute tempVar14(XmlDeclarations::TYPE_ATR, XmlDeclarations::AXIAL_TYPE_VAL);
                        stereoChemEl->addAttribute(&tempVar14);
                        Attribute tempVar15(XmlDeclarations::VALUE_ATR, m->group(2));
                        stereoChemEl->addAttribute(&tempVar15);
                    } else {
                        throw ComponentGenerationException(std::wstring(L"Malformed stereochemistry element: ") +
                                                           stereoChemistryElement->getValue());
                    }
                } else {
                    throw ComponentGenerationException(
                            std::wstring(L"Malformed stereochemistry element: ") + stereoChemistryElement->getValue());
                }
            }
        }
    }
    stereoChemistryElement->detach();
}

std::vector<std::wstring> ComponentGenerator::splitStereoBracketIntoDescriptors(const std::wstring &stereoBracket) {
    std::vector<std::wstring> stereoDescriptors;
    StringBuilder *sb = new StringBuilder();
    //ignore first and last character (opening and closing bracket)
    for (int i = 1, l = stereoBracket.length() - 1; i < l; i++) {
        wchar_t ch = stereoBracket[i];
        if (ch == L',') {
            stereoDescriptors.push_back(sb->toString());
            sb->setLength(0);
        } else if (ch == L'-') {
            if (matchStereochemistry->matcher(sb->toString()).matches()) {
                //delimiter between stereochemistry
                stereoDescriptors.push_back(sb->toString());
                sb->setLength(0);
            } else {
                //locanted stereochemistry term
                sb->append(ch);
            }
        } else {
            sb->append(ch);
        }
    }
    stereoDescriptors.push_back(sb->toString());
    return stereoDescriptors;
}

bool ComponentGenerator::assignLocantUsingPreviousElementIfPresent(Element *stereoChemistryElement) {
    Element *possibleLocant = OpsinTools::getPrevious(stereoChemistryElement);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL &&
        possibleLocant->getValue().split(L",")->length == 1) {
        Attribute tempVar(XmlDeclarations::LOCANT_ATR, possibleLocant->getValue());
        stereoChemistryElement->addAttribute(&tempVar);
        possibleLocant->detach();
        return true;
    }
    return false;
}

void ComponentGenerator::processLocantAssigningForEndoExoSynAnti(Element *stereoChemistryElement) {
    Element *possibleLocant = OpsinTools::getPrevious(stereoChemistryElement);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL &&
        possibleLocant->getValue().split(L",")->length == 1) {
        Attribute tempVar(XmlDeclarations::LOCANT_ATR, possibleLocant->getValue());
        stereoChemistryElement->addAttribute(&tempVar);
        Element *group = OpsinTools::getNextSibling(stereoChemistryElement, XmlDeclarations::GROUP_EL);
        if (group != nullptr && (XmlDeclarations::CYCLICUNSATURABLEHYDROCARBON_SUBTYPE_VAL ==
                                 group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) ||
                                 OpsinTools::getPreviousSibling(group)->getName() == XmlDeclarations::VONBAEYER_EL)) {
            //detach locant only if we're sure it has no other meaning
            //typically locants in front of endo/exo/syn/anti also indicate the position of a susbtituent/suffix e.g. 3-exo-amino
            possibleLocant->detach();
        }
    }
}

void ComponentGenerator::processUnbracketedAlphaBetaStereochemistry(
        Element *stereoChemistryElement) throw(ComponentGenerationException) {
    std::wstring txt = StringTools::removeDashIfPresent(stereoChemistryElement->getValue());
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> stereoChemistryDescriptors = txt.split(L",");
    std::vector<std::wstring> locants;
    bool createLocantsEl = false;
    for (auto stereoChemistryDescriptor : stereoChemistryDescriptors) {
        Matcher *digitMatcher = matchDigit->matcher(stereoChemistryDescriptor);
        if (digitMatcher->lookingAt()) {
            std::wstring locant = digitMatcher->group();
            std::wstring possibleAlphaBeta = digitMatcher->replaceAll(L"");
            locants.push_back(locant);
            Matcher *alphaBetaMatcher = matchAlphaBetaStereochem->matcher(possibleAlphaBeta);
            if (alphaBetaMatcher->matches()) {
                Element *stereoChemEl = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, stereoChemistryDescriptor);
                Attribute tempVar(XmlDeclarations::LOCANT_ATR, locant);
                stereoChemEl->addAttribute(&tempVar);
                OpsinTools::insertBefore(stereoChemistryElement, stereoChemEl);
                Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL);
                stereoChemEl->addAttribute(&tempVar2);
                if (std::tolower(possibleAlphaBeta[0]) == L'a') {
                    Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"alpha");
                    stereoChemEl->addAttribute(&tempVar3);
                } else if (std::tolower(possibleAlphaBeta[0]) == L'b') {
                    Attribute tempVar4(XmlDeclarations::VALUE_ATR, L"beta");
                    stereoChemEl->addAttribute(&tempVar4);
                } else if (std::tolower(possibleAlphaBeta[0]) == L'x') {
                    Attribute tempVar5(XmlDeclarations::VALUE_ATR, L"xi");
                    stereoChemEl->addAttribute(&tempVar5);
                } else {
                    throw ComponentGenerationException(std::wstring(L"Malformed alpha/beta stereochemistry element: ") +
                                                       stereoChemistryElement->getValue());
                }
            } else {
                createLocantsEl = true;
            }
        }
    }
    if (!createLocantsEl) {
        //create locants unless a group supporting alpha/beta stereochem is within this substituent/root
        createLocantsEl = true;
        std::vector<Element *> groups = OpsinTools::getNextSiblingsOfType(stereoChemistryElement,
                                                                          XmlDeclarations::GROUP_EL);
        for (auto group : groups) {
            if (group->getAttributeValue(XmlDeclarations::ALPHABETACLOCKWISEATOMORDERING_ATR) != nullptr) {
                createLocantsEl = false;
                break;
            }
        }
    }

    if (createLocantsEl) {
        Element *newLocantEl = new TokenEl(XmlDeclarations::LOCANT_EL, StringTools::stringListToString(locants, L","));
        OpsinTools::insertAfter(stereoChemistryElement, newLocantEl);
    }
    stereoChemistryElement->detach();
}

void ComponentGenerator::processRelativeCisTrans(Element *stereoChemistryElement) {
    std::wstring value = StringTools::removeDashIfPresent(stereoChemistryElement->getValue());
    StringBuilder *sb = new StringBuilder();
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> terms = value.split(L",");
    for (auto term : terms) {
        if (term.startsWith(L"c-") || term.startsWith(L"t-") || term.startsWith(L"r-")) {
            if (sb->length() > 0) {
                sb->append(L',');
            }
            sb->append(term.substr(2));
        } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Malformed relativeCisTrans element");
            throw std::exception();
        }
    }
    Element *locantEl = new TokenEl(XmlDeclarations::LOCANT_EL, sb->toString());
    OpsinTools::insertAfter(stereoChemistryElement, locantEl);
}

void ComponentGenerator::duplicateLocantFromStereoTermIfAdjacentToEneOrYlidene(
        std::vector<Element *> &locantedUnbrackettedEzTerms) {
    for (int i = 0, l = locantedUnbrackettedEzTerms.size(); i < l; i++) {
        Element *currentTerm = locantedUnbrackettedEzTerms[i];
        std::vector<Element *> groupedTerms;
        groupedTerms.push_back(currentTerm);
        while (i + 1 < l && locantedUnbrackettedEzTerms[i + 1]->equals(OpsinTools::getNextSibling(currentTerm))) {
            currentTerm = locantedUnbrackettedEzTerms[++i];
            groupedTerms.push_back(currentTerm);
        }
        Element *lastTermInGroup = groupedTerms[groupedTerms.size() - 1];
        Element *eneOrYlidene;
        if (groupedTerms.size() > 1) {
            Element *multiplier = OpsinTools::getNextSibling(lastTermInGroup);
            if (!(multiplier != nullptr && multiplier->getName() == XmlDeclarations::MULTIPLIER_EL &&
                  static_cast<String>(groupedTerms.size()).equals(
                          multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)))) {
                continue;
            }
            eneOrYlidene = OpsinTools::getNextSibling(multiplier);
        } else {
            eneOrYlidene = OpsinTools::getNextSibling(lastTermInGroup);
        }
        if (eneOrYlidene != nullptr) {
            std::wstring name = eneOrYlidene->getName();
            if (name == XmlDeclarations::UNSATURATOR_EL || name == XmlDeclarations::SUFFIX_EL) {
                if ((name == XmlDeclarations::UNSATURATOR_EL &&
                     eneOrYlidene->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"2") ||
                    (name == XmlDeclarations::SUFFIX_EL &&
                     eneOrYlidene->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"ylidene")) {
                    std::vector<std::wstring> locants;
                    for (auto stereochemistryTerm : groupedTerms) {
                        locants.push_back(stereochemistryTerm->getAttributeValue(XmlDeclarations::LOCANT_ATR));
                    }
                    Element *newLocant = new TokenEl(XmlDeclarations::LOCANT_EL,
                                                     StringTools::stringListToString(locants, L","));
                    OpsinTools::insertAfter(lastTermInGroup, newLocant);
                } else {
                    if (name == XmlDeclarations::UNSATURATOR_EL) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("After E/Z stereo expected ene but found: " + eneOrYlidene.getValue());
                        throw std::exception();
                    } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("After E/Z stereo expected yldiene but found: " + eneOrYlidene.getValue());
                        throw std::exception();
                    }
                }
            }
        }
    }
}

void ComponentGenerator::processSuffixPrefixes(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> suffixPrefixes = subOrRoot->getChildElements(XmlDeclarations::SUFFIXPREFIX_EL);
    for (auto suffixPrefix : suffixPrefixes) {
        Element *suffix = OpsinTools::getNextSibling(suffixPrefix);
        if (suffix == nullptr || suffix->getName() != XmlDeclarations::SUFFIX_EL) {
            throw ComponentGenerationException(
                    std::wstring(L"OPSIN bug: suffix not found after suffixPrefix: ") + suffixPrefix->getValue());
        }
        Attribute tempVar(XmlDeclarations::SUFFIXPREFIX_ATR,
                          suffixPrefix->getAttributeValue(XmlDeclarations::VALUE_ATR));
        suffix->addAttribute(&tempVar);
        suffixPrefix->detach();
    }
}

void ComponentGenerator::processInfixes(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> infixes = subOrRoot->getChildElements(XmlDeclarations::INFIX_EL);
    for (auto infix : infixes) {
        Element *suffix = OpsinTools::getNextSiblingIgnoringCertainElements(infix, std::vector<std::wstring>{
                XmlDeclarations::INFIX_EL, XmlDeclarations::SUFFIXPREFIX_EL, XmlDeclarations::MULTIPLIER_EL});
        if (suffix == nullptr || suffix->getName() != XmlDeclarations::SUFFIX_EL) {
            throw ComponentGenerationException(
                    std::wstring(L"No suffix found next next to infix: ") + infix->getValue());
        }
        std::vector<std::wstring> currentInfixInformation;
        if (suffix->getAttribute(XmlDeclarations::INFIX_ATR) == nullptr) {
            Attribute tempVar(XmlDeclarations::INFIX_ATR, L"");
            suffix->addAttribute(&tempVar);
            currentInfixInformation = std::vector<std::wstring>();
        } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            currentInfixInformation = StringTools::arrayToList(
                    suffix->getAttributeValue(XmlDeclarations::INFIX_ATR).split(L";"));
        }
        std::wstring infixValue = infix->getAttributeValue(XmlDeclarations::VALUE_ATR);
        currentInfixInformation.push_back(infixValue);
        Element *possibleMultiplier = OpsinTools::getPreviousSibling(infix);
        Element *possibleBracket;
        bool multiplierKnownToIndicateInfixMultiplicationPresent = false;
        if (possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
            //suffix prefix present so multiplier must indicate infix replacement
            Element *possibleSuffixPrefix = OpsinTools::getPreviousSiblingIgnoringCertainElements(infix,
                                                                                                  std::vector<std::wstring>{
                                                                                                          XmlDeclarations::MULTIPLIER_EL,
                                                                                                          XmlDeclarations::INFIX_EL});
            if (possibleSuffixPrefix != nullptr &&
                possibleSuffixPrefix->getName() == XmlDeclarations::SUFFIXPREFIX_EL) {
                multiplierKnownToIndicateInfixMultiplicationPresent = true;
            }
            Element *elementBeforeMultiplier = OpsinTools::getPreviousSibling(possibleMultiplier);
            //double multiplier indicates multiple suffixes which all have their infix multiplied
            //if currentInfixInformation contains more than 1 entry it contains information from an infix from before the multiplier so the interpretation of the multiplier as a suffix multiplier is impossible
            if (elementBeforeMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL ||
                currentInfixInformation.size() > 1) {
                multiplierKnownToIndicateInfixMultiplicationPresent = true;
            }
            possibleBracket = elementBeforeMultiplier;
        } else {
            possibleBracket = possibleMultiplier;
            possibleMultiplier = nullptr;
            infix->detach();
        }
        if (possibleBracket->getName() == XmlDeclarations::STRUCTURALOPENBRACKET_EL) {
            Element *bracket = OpsinTools::getNextSibling(suffix);
            if (bracket->getName() != XmlDeclarations::STRUCTURALCLOSEBRACKET_EL) {
                throw ComponentGenerationException(L"Matching closing bracket not found around infix/suffix block");
            }
            if (possibleMultiplier != nullptr) {
                int multiplierVal = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
                for (int i = 1; i < multiplierVal; i++) {
                    currentInfixInformation.push_back(infixValue);
                }
                possibleMultiplier->detach();
                infix->detach();
            }
            possibleBracket->detach();
            bracket->detach();
        } else if (multiplierKnownToIndicateInfixMultiplicationPresent) { //multiplier unambiguously means multiplication of the infix
            int multiplierVal = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            for (int i = 1; i < multiplierVal; i++) {
                currentInfixInformation.push_back(infixValue);
            }
            possibleMultiplier->detach();
            infix->detach();
        } else if (possibleMultiplier != nullptr && XmlDeclarations::GROUP_TYPE_VAL ==
                                                    possibleMultiplier->getAttributeValue(
                                                            XmlDeclarations::TYPE_ATR)) { //e.g. ethanbisthioic acid == ethanbis(thioic acid)
            infix->detach();
        }
        suffix->getAttribute(XmlDeclarations::INFIX_ATR)->setValue(
                StringTools::stringListToString(currentInfixInformation, L";"));
    }
}

void ComponentGenerator::processLambdaConvention(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> lambdaConventionEls = subOrRoot->getChildElements(XmlDeclarations::LAMBDACONVENTION_EL);
    bool fusedRingPresent = false;
    if (lambdaConventionEls.size() > 0) {
        if (subOrRoot->getChildElements(XmlDeclarations::GROUP_EL).size() > 1) {
            fusedRingPresent = true;
        }
    }
    for (auto lambdaConventionEl : lambdaConventionEls) {
        bool frontLocantsExpected = false; //Is the lambdaConvention el followed by benz/benzo of a fused ring system (these have front locants which correspond to the final fused rings numbering) or by a polycylicspiro system
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> lambdaValues = StringTools::removeDashIfPresent(lambdaConventionEl->getValue()).split(
                L",");
        Element *possibleHeteroatomOrMultiplier = OpsinTools::getNextSibling(lambdaConventionEl);
        int heteroCount = 0;
        int multiplierValue = 1;
        while (possibleHeteroatomOrMultiplier != nullptr) {
            if (possibleHeteroatomOrMultiplier->getName() == XmlDeclarations::HETEROATOM_EL) {
                heteroCount += multiplierValue;
                multiplierValue = 1;
            } else if (possibleHeteroatomOrMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                multiplierValue = std::stoi(
                        possibleHeteroatomOrMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            } else {
                break;
            }
            possibleHeteroatomOrMultiplier = OpsinTools::getNextSibling(possibleHeteroatomOrMultiplier);
        }
        bool assignLambdasToHeteroAtoms = false;
        if (lambdaValues.size() == heteroCount) { //heteroatom and number of locants +lambdas must match
            if (fusedRingPresent && possibleHeteroatomOrMultiplier != nullptr &&
                possibleHeteroatomOrMultiplier->getName() == XmlDeclarations::GROUP_EL &&
                possibleHeteroatomOrMultiplier->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) ==
                XmlDeclarations::HANTZSCHWIDMAN_SUBTYPE_VAL) {
                //You must not set the locants of a HW system which forms a component of a fused ring system. The locant specified corresponds to the complete fused ring system.
            } else {
                assignLambdasToHeteroAtoms = true;
            }
        } else if (possibleHeteroatomOrMultiplier != nullptr && ((heteroCount == 0 && OpsinTools::getNextSibling(
                lambdaConventionEl)->equals(possibleHeteroatomOrMultiplier) && fusedRingPresent &&
                                                                  possibleHeteroatomOrMultiplier->getName() ==
                                                                  XmlDeclarations::GROUP_EL &&
                                                                  (possibleHeteroatomOrMultiplier->getValue() ==
                                                                   L"benzo" ||
                                                                   possibleHeteroatomOrMultiplier->getValue() ==
                                                                   L"benz") && OpsinTools::getNextSibling(
                possibleHeteroatomOrMultiplier)->getName() != XmlDeclarations::FUSION_EL && OpsinTools::getNextSibling(
                possibleHeteroatomOrMultiplier)->getName() != XmlDeclarations::LOCANT_EL) ||
                                                                 (possibleHeteroatomOrMultiplier->getName() ==
                                                                  XmlDeclarations::POLYCYCLICSPIRO_EL &&
                                                                  (possibleHeteroatomOrMultiplier->getAttributeValue(
                                                                          XmlDeclarations::VALUE_ATR) == L"spirobi" ||
                                                                   possibleHeteroatomOrMultiplier->getAttributeValue(
                                                                           XmlDeclarations::VALUE_ATR) ==
                                                                   L"spiroter")))) {
            frontLocantsExpected = true; //a benzo fused ring e.g. 1lambda4,3-benzothiazole or a symmetrical poly cyclic spiro system
        }
        std::vector<Element *> heteroAtoms; //contains the heteroatoms to apply the lambda values too. Can be empty if the values are applied to a group directly rather than to a heteroatom
        if (assignLambdasToHeteroAtoms) { //populate heteroAtoms, multiplied heteroatoms are multiplied out
            Element *multiplier = nullptr;
            Element *heteroatomOrMultiplier = OpsinTools::getNextSibling(lambdaConventionEl);
            while (heteroatomOrMultiplier != nullptr) {
                if (heteroatomOrMultiplier->getName() == XmlDeclarations::HETEROATOM_EL) {
                    heteroAtoms.push_back(heteroatomOrMultiplier);
                    if (multiplier != nullptr) {
                        for (int i = 1; i < std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)); i++) {
                            Element *newHeteroAtom = heteroatomOrMultiplier->copy();
                            OpsinTools::insertBefore(heteroatomOrMultiplier, newHeteroAtom);
                            heteroAtoms.push_back(newHeteroAtom);
                        }
                        multiplier->detach();
                        multiplier = nullptr;
                    }
                } else if (heteroatomOrMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                    if (multiplier != nullptr) {
                        break;
                    } else {
                        multiplier = heteroatomOrMultiplier;
                    }
                } else {
                    break;
                }
                heteroatomOrMultiplier = OpsinTools::getNextSibling(heteroatomOrMultiplier);
            }
        }

        for (int i = 0; i <
                        lambdaValues.size(); i++) { //assign all the lambdas to heteroatoms or to newly created lambdaConvention elements
            std::wstring lambdaValue = lambdaValues[i];
            Matcher *m = matchLambdaConvention->matcher(lambdaValue);
            if (m->matches()) { //a lambda
                Attribute *valencyChange = new Attribute(XmlDeclarations::LAMBDA_ATR, m->group(2));
                Attribute *locantAtr = nullptr;
                if (m->group(1) != nullptr) {
                    locantAtr = new Attribute(XmlDeclarations::LOCANT_ATR, m->group(1));
                }
                if (frontLocantsExpected) {
                    if (m->group(1) == nullptr) {
                        throw ComponentGenerationException(
                                L"Locant not found for lambda convention before a benzo fused ring system");
                    }
                    lambdaValues[i] = m->group(1);
                }
                if (assignLambdasToHeteroAtoms) {
                    Element *heteroAtom = heteroAtoms[i];
                    heteroAtom->addAttribute(valencyChange);
                    if (locantAtr != nullptr) {
                        heteroAtom->addAttribute(locantAtr);
                    }
                } else {
                    Element *newLambda = new TokenEl(XmlDeclarations::LAMBDACONVENTION_EL);
                    newLambda->addAttribute(valencyChange);
                    if (locantAtr != nullptr) {
                        newLambda->addAttribute(locantAtr);
                    }
                    OpsinTools::insertBefore(lambdaConventionEl, newLambda);
                }
            } else { //just a locant e.g 1,3lambda5
                if (!assignLambdasToHeteroAtoms) {
                    if (!frontLocantsExpected) {
                        throw ComponentGenerationException(
                                std::wstring(L"Lambda convention not specified for locant: ") + lambdaValue);
                    }
                } else {
                    Element *heteroAtom = heteroAtoms[i];
                    Attribute tempVar(XmlDeclarations::LOCANT_ATR, lambdaValue);
                    heteroAtom->addAttribute(&tempVar);
                }
            }
        }
        if (!frontLocantsExpected) {
            lambdaConventionEl->detach();
        } else {
            lambdaConventionEl->setName(XmlDeclarations::LOCANT_EL);
            lambdaConventionEl->setValue(StringTools::arrayToString(lambdaValues, L","));
        }
    }
}

void ComponentGenerator::findAndStructureBrackets(
        std::vector<Element *> &substituentsAndRoot) throw(ComponentGenerationException) {
    int blevel = 0;
    Element *openBracket = nullptr;
    bool nestedBrackets = false;
    for (auto sub : substituentsAndRoot) {
        std::vector<Element *> children = sub->getChildElements();
        for (auto child : children) {
            std::wstring name = child->getName();
            if (name == XmlDeclarations::OPENBRACKET_EL) {
                blevel++;
                if (openBracket == nullptr) {
                    openBracket = child;
                } else {
                    nestedBrackets = true;
                }
            } else if (name == XmlDeclarations::CLOSEBRACKET_EL) {
                blevel--;
                if (blevel == 0) {
                    Element *bracket = structureBrackets(openBracket, child);
                    if (nestedBrackets) {
                        findAndStructureBrackets(OpsinTools::getDescendantElementsWithTagNames(bracket,
                                                                                               std::vector<std::wstring>{
                                                                                                       XmlDeclarations::SUBSTITUENT_EL,
                                                                                                       XmlDeclarations::ROOT_EL}));
                    }
                    openBracket = nullptr;
                    nestedBrackets = false;
                }
            }
        }
    }
    if (blevel != 0) {
        throw ComponentGenerationException(L"Brackets do not match!");
    }
}

Element *
ComponentGenerator::structureBrackets(Element *openBracket, Element *closeBracket) throw(ComponentGenerationException) {
    Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
    Element *currentEl = openBracket->getParent();
    OpsinTools::insertBefore(currentEl, bracket);
    /* Pick up everything in the substituent before the bracket*/
    Element *firstChild = currentEl->getChild(0);
    while (!firstChild->equals(openBracket)) {
        firstChild->detach();
        bracket->addChild(firstChild);
        firstChild = currentEl->getChild(0);
    }
    /* Pick up all elements from the one with the open bracket,
							 * to the one with the close bracket, inclusive.
							 */
    while (!currentEl->equals(closeBracket->getParent())) {
        Element *nextEl = OpsinTools::getNextSibling(currentEl);
        currentEl->detach();
        bracket->addChild(currentEl);
        currentEl = nextEl;
        if (currentEl == nullptr) {
            throw ComponentGenerationException(L"Brackets within a word do not match!");
        }
    }
    currentEl->detach();
    bracket->addChild(currentEl);
    /* Pick up elements after the close bracket */
    currentEl = OpsinTools::getNextSibling(closeBracket);
    while (currentEl != nullptr) {
        Element *nextEl = OpsinTools::getNextSibling(currentEl);
        currentEl->detach();
        bracket->addChild(currentEl);
        currentEl = nextEl;
    }
    openBracket->detach();
    closeBracket->detach();
    return bracket;
}

void ComponentGenerator::processHydroCarbonRings(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> annulens = subOrRoot->getChildElements(XmlDeclarations::ANNULEN_EL);
    for (auto annulen : annulens) {
        std::wstring annulenValue = annulen->getValue();
        Matcher *match = matchAnnulene->matcher(annulenValue);
        match->matches();
        if (match->groupCount() != 1) {
            throw ComponentGenerationException(L"Invalid annulen tag");
        }

        int annulenSize = static_cast<Integer>(match->group(1));
        if (annulenSize < 3) {
            throw ComponentGenerationException(L"Invalid annulen tag");
        }

        //build [annulenSize]annulene ring as SMILES
        std::wstring SMILES = std::wstring(L"c1") + StringTools::multiplyString(L"c", annulenSize - 1);
        SMILES += std::wstring(L"1");

        Element *group = new TokenEl(XmlDeclarations::GROUP_EL, annulenValue);
        Attribute tempVar(XmlDeclarations::VALUE_ATR, SMILES);
        group->addAttribute(&tempVar);
        Attribute tempVar2(XmlDeclarations::LABELS_ATR, XmlDeclarations::NUMERIC_LABELS_VAL);
        group->addAttribute(&tempVar2);
        Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::RING_TYPE_VAL);
        group->addAttribute(&tempVar3);
        Attribute tempVar4(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::RING_SUBTYPE_VAL);
        group->addAttribute(&tempVar4);
        annulen->getParent()->replaceChild(annulen, group);
    }

    std::vector<Element *> hydrocarbonFRSystems = subOrRoot->getChildElements(
            XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL);
    for (auto hydrocarbonFRSystem : hydrocarbonFRSystems) {
        Element *multiplier = OpsinTools::getPreviousSibling(hydrocarbonFRSystem);
        if (multiplier != nullptr && multiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
            int multiplierValue = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            std::wstring classOfHydrocarbonFRSystem = hydrocarbonFRSystem->getAttributeValue(
                    XmlDeclarations::VALUE_ATR);
            StringBuilder *smilesSB = new StringBuilder();
            if (classOfHydrocarbonFRSystem == L"polyacene") {
                if (multiplierValue <= 3) {
                    throw ComponentGenerationException(L"Invalid polyacene");
                }
                smilesSB->append(L"c1ccc");
                for (int j = 2; j <= multiplierValue; j++) {
                    smilesSB->append(L"c");
                    smilesSB->append(ringClosure(j));
                    smilesSB->append(L"c");
                }
                smilesSB->append(L"ccc");
                for (int j = multiplierValue; j > 2; j--) {
                    smilesSB->append(L"c");
                    smilesSB->append(ringClosure(j));
                    smilesSB->append(L"c");
                }
                smilesSB->append(L"c12");
            } else if (classOfHydrocarbonFRSystem == L"polyaphene") {
                if (multiplierValue <= 3) {
                    throw ComponentGenerationException(L"Invalid polyaphene");
                }
                smilesSB->append(L"c1ccc");

                int ringsAbovePlane;
                int ringsOnPlane;
                int ringOpeningCounter = 2;
                if (multiplierValue % 2 == 0) {
                    ringsAbovePlane = (multiplierValue - 2) / 2;
                    ringsOnPlane = ringsAbovePlane + 1;
                } else {
                    ringsAbovePlane = (multiplierValue - 1) / 2;
                    ringsOnPlane = ringsAbovePlane;
                }

                for (int j = 1; j <= ringsAbovePlane; j++) {
                    smilesSB->append(L"c");
                    smilesSB->append(ringClosure(ringOpeningCounter++));
                    smilesSB->append(L"c");
                }

                for (int j = 1; j <= ringsOnPlane; j++) {
                    smilesSB->append(L"cc");
                    smilesSB->append(ringClosure(ringOpeningCounter++));
                }
                smilesSB->append(L"ccc");
                ringOpeningCounter--;
                for (int j = 1; j <= ringsOnPlane; j++) {
                    smilesSB->append(L"cc");
                    smilesSB->append(ringClosure(ringOpeningCounter--));
                }
                for (int j = 1; j < ringsAbovePlane; j++) {
                    smilesSB->append(L"c");
                    smilesSB->append(ringClosure(ringOpeningCounter--));
                    smilesSB->append(L"c");
                }

                smilesSB->append(L"c12");
            } else if (classOfHydrocarbonFRSystem == L"polyalene") {
                if (multiplierValue < 5) {
                    throw ComponentGenerationException(L"Invalid polyalene");
                }
                smilesSB->append(L"c1");
                for (int j = 3; j < multiplierValue; j++) {
                    smilesSB->append(L"c");
                }
                smilesSB->append(L"c2");
                for (int j = 3; j <= multiplierValue; j++) {
                    smilesSB->append(L"c");
                }
                smilesSB->append(L"c12");
            } else if (classOfHydrocarbonFRSystem == L"polyphenylene") {
                if (multiplierValue < 2) {
                    throw ComponentGenerationException(L"Invalid polyphenylene");
                }
                smilesSB->append(L"c1cccc2");
                for (int j = 1; j < multiplierValue; j++) {
                    smilesSB->append(L"c3ccccc3");
                }
                smilesSB->append(L"c12");
            } else if (classOfHydrocarbonFRSystem == L"polynaphthylene") {
                if (multiplierValue < 3) {
                    throw ComponentGenerationException(L"Invalid polynaphthylene");
                }
                smilesSB->append(L"c1cccc2cc3");
                for (int j = 1; j < multiplierValue; j++) {
                    smilesSB->append(L"c4cc5ccccc5cc4");
                }
                smilesSB->append(L"c3cc12");
            } else if (classOfHydrocarbonFRSystem == L"polyhelicene") {
                if (multiplierValue < 4) {
                    throw ComponentGenerationException(L"Invalid polyhelicene");
                }
                smilesSB->append(L"c1c");
                int ringOpeningCounter = 2;
                for (int j = 1; j < multiplierValue; j++) {
                    smilesSB->append(L"ccc");
                    smilesSB->append(ringClosure(ringOpeningCounter++));
                }
                smilesSB->append(L"cccc");
                ringOpeningCounter--;
                for (int j = 2; j < multiplierValue; j++) {
                    smilesSB->append(L"c");
                    smilesSB->append(ringClosure(ringOpeningCounter--));
                }
                smilesSB->append(L"c12");
            } else {
                throw ComponentGenerationException(L"Unknown semi-trivially named hydrocarbon fused ring system");
            }
            Element *newGroup = new TokenEl(XmlDeclarations::GROUP_EL,
                                            multiplier->getValue() + hydrocarbonFRSystem->getValue());
            Attribute tempVar5(XmlDeclarations::VALUE_ATR, smilesSB->toString());
            newGroup->addAttribute(&tempVar5);
            Attribute tempVar6(XmlDeclarations::LABELS_ATR, XmlDeclarations::FUSEDRING_LABELS_VAL);
            newGroup->addAttribute(&tempVar6);
            Attribute tempVar7(XmlDeclarations::TYPE_ATR, XmlDeclarations::RING_TYPE_VAL);
            newGroup->addAttribute(&tempVar7);
            Attribute tempVar8(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL);
            newGroup->addAttribute(&tempVar8);
            hydrocarbonFRSystem->getParent()->replaceChild(hydrocarbonFRSystem, newGroup);
            multiplier->detach();
        } else {
            throw ComponentGenerationException(L"Invalid semi-trivially named hydrocarbon fused ring system");
        }
    }
}

void ComponentGenerator::handleSuffixIrregularities(Element *subOrRoot) throw(ComponentGenerationException) {
    std::vector<Element *> suffixes = subOrRoot->getChildElements(XmlDeclarations::SUFFIX_EL);
    for (auto suffix : suffixes) {
        std::wstring suffixValue = suffix->getValue();
        if (suffixValue == L"ic" || suffixValue == L"ous") {
            if (!n2sConfig->allowInterpretationOfAcidsWithoutTheWordAcid()) {
                Element *next = OpsinTools::getNext(suffix);
                if (next == nullptr) {
                    throw ComponentGenerationException(std::wstring(L"\"acid\" not found after ") + suffixValue);
                }
            }
        }
            // convert quinone to dione
        else if (suffixValue == L"quinone" || suffixValue == L"quinon") {
            suffix->removeAttribute(suffix->getAttribute(XmlDeclarations::ADDITIONALVALUE_ATR));
            suffix->setValue(L"one");
            Element *multiplier = OpsinTools::getPreviousSibling(suffix);
            if (multiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                Attribute *multVal = multiplier->getAttribute(XmlDeclarations::VALUE_ATR);
                int newMultiplier = std::stoi(multVal->getValue()) * 2;
                multVal->setValue(std::to_wstring(newMultiplier));
            } else {
                multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL, L"di");
                Attribute tempVar(XmlDeclarations::VALUE_ATR, L"2");
                multiplier->addAttribute(&tempVar);
                OpsinTools::insertBefore(suffix, multiplier);
            }
        } else if (suffixValue == L"ylene" || suffixValue == L"ylen") {
            suffix->removeAttribute(suffix->getAttribute(XmlDeclarations::ADDITIONALVALUE_ATR));
            suffix->setValue(L"yl");
            Element *alk = OpsinTools::getPreviousSibling(suffix, XmlDeclarations::GROUP_EL);
            if (alk->getAttribute(XmlDeclarations::USABLEASJOINER_ATR) != nullptr) {
                alk->removeAttribute(alk->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
            }
            Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL, L"di");
            Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"2");
            multiplier->addAttribute(&tempVar2);
            OpsinTools::insertBefore(suffix, multiplier);
        } else if (suffixValue == L"ylium" &&
                   (std::wstring(L"acylium")) == suffix->getAttributeValue(XmlDeclarations::VALUE_ATR) &&
                   suffix->getAttribute(XmlDeclarations::SUFFIXPREFIX_ATR) == nullptr &&
                   suffix->getAttribute(XmlDeclarations::INFIX_ATR) ==
                   nullptr) { //disambiguate between ylium the charge modifying suffix and ylium the acylium suffix
            Element *group = OpsinTools::getPreviousSibling(suffix, XmlDeclarations::GROUP_EL);
            if (group == nullptr ||
                (XmlDeclarations::ACIDSTEM_TYPE_VAL != group->getAttributeValue(XmlDeclarations::TYPE_ATR) &&
                 XmlDeclarations::CHALCOGENACIDSTEM_TYPE_VAL != group->getAttributeValue(XmlDeclarations::TYPE_ATR) &&
                 XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL != group->getAttributeValue(XmlDeclarations::TYPE_ATR))) {
                Element *beforeSuffix = OpsinTools::getPreviousSibling(suffix);
                std::wstring o = beforeSuffix->getAttributeValue(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR);
                if (o == L"" || !StringTools::endsWithCaseInsensitive(o, L"o")) {
                    if (group != nullptr && XmlDeclarations::ARYLSUBSTITUENT_SUBTYPE_VAL ==
                                            group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                        //contracted form for removal of hydride e.g. 9-Anthrylium
                        suffix->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(L"ylium");
                        suffix->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::CHARGE_TYPE_VAL);
                        suffix->removeAttribute(suffix->getAttribute(XmlDeclarations::SUBTYPE_ATR));
                    } else {
                        throw ComponentGenerationException(
                                L"ylium is intended to be the removal of H- in this context not the formation of an acylium ion");
                    }
                }
            }
        } else if (suffixValue == L"nitrolic acid" || suffixValue == L"nitrolicacid") {
            Element *precedingGroup = OpsinTools::getPreviousSibling(suffix, XmlDeclarations::GROUP_EL);
            if (precedingGroup == nullptr) {
                if (subOrRoot->getChildCount() != 1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN Bug: nitrolic acid not expected to have sibilings");
                    throw std::exception();
                }
                Element *precedingSubstituent = OpsinTools::getPreviousSibling(subOrRoot);
                if (precedingSubstituent == nullptr ||
                    precedingSubstituent->getName() != XmlDeclarations::SUBSTITUENT_EL) {
                    throw ComponentGenerationException(L"Expected substituent before nitrolic acid");
                }

                std::vector<Element *> existingSuffixes = precedingSubstituent->getChildElements(
                        XmlDeclarations::SUFFIX_EL);
                if (existingSuffixes.size() == 1) {
                    if (existingSuffixes[0]->getValue() != L"yl") {
                        throw ComponentGenerationException(L"Unexpected suffix found before nitrolic acid");
                    }
                    existingSuffixes[0]->detach();
                    for (auto child : precedingSubstituent->getChildElements()) {
                        child->detach();
                        OpsinTools::insertBefore(suffix, child);
                    }
                    precedingSubstituent->detach();
                } else {
                    throw ComponentGenerationException(
                            L"Only the nitrolic acid case where it is preceded by an yl suffix is supported");
                }
            }
        }
    }
}

void ComponentGenerator::detectAlkaneFusedRingBridges(Element *group) {
    if (XmlDeclarations::ALKANESTEM_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
        Element *unsaturator = OpsinTools::getNextSibling(group);
        if (unsaturator != nullptr && unsaturator->getName() == XmlDeclarations::UNSATURATOR_EL) {
            Element *possibleBridgeFormer = OpsinTools::getNextSiblingIgnoringCertainElements(group,
                                                                                              std::vector<std::wstring>{
                                                                                                      XmlDeclarations::UNSATURATOR_EL});
            if (possibleBridgeFormer != nullptr &&
                possibleBridgeFormer->getName() == XmlDeclarations::BRIDGEFORMINGO_EL) {
                group->setName(XmlDeclarations::FUSEDRINGBRIDGE_EL);
                possibleBridgeFormer->detach();
                unsaturator->detach();
            }
        }
    }
}

void ComponentGenerator::processRings(Element *group) throw(ComponentGenerationException) {
    Element *previous = OpsinTools::getPreviousSiblingIgnoringCertainElements(group, std::vector<std::wstring>{
            XmlDeclarations::LOCANT_EL});
    if (previous != nullptr) {
        std::wstring previousElType = previous->getName();
        if (previousElType == XmlDeclarations::SPIRO_EL) {
            processSpiroSystem(group, previous);
        } else if (previousElType == XmlDeclarations::VONBAEYER_EL) {
            processVonBaeyerSystem(group, previous);
        } else if (previousElType == XmlDeclarations::CYCLO_EL) {
            processCyclisedChain(group, previous);
        }
    }
}

void ComponentGenerator::processSpiroSystem(Element *chainGroup,
                                            Element *spiroEl) throw(NumberFormatException, ComponentGenerationException) {
    std::vector<std::vector<int>> spiroDescriptors = getSpiroDescriptors(
            StringTools::removeDashIfPresent(spiroEl->getValue()));

    Element *multiplier = OpsinTools::getPreviousSibling(spiroEl);
    int numberOfSpiros = 1;
    if (multiplier != nullptr && multiplier->getName() == XmlDeclarations::MULTIPLIER_EL &&
        XmlDeclarations::BASIC_TYPE_VAL == multiplier->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
        numberOfSpiros = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
        multiplier->detach();
    }
    int numberOfCarbonInDescriptors = 0;
    for (auto spiroDescriptor : spiroDescriptors) {
        numberOfCarbonInDescriptors += spiroDescriptor[0];
    }
    numberOfCarbonInDescriptors += numberOfSpiros;
    if (numberOfCarbonInDescriptors != chainGroup->getAttributeValue(XmlDeclarations::VALUE_ATR).length()) {
        throw ComponentGenerationException(std::wstring(L"Disagreement between number of atoms in spiro descriptor: ") +
                                           numberOfCarbonInDescriptors +
                                           std::wstring(L" and number of atoms in chain: ") + std::to_wstring(
                chainGroup->getAttributeValue(XmlDeclarations::VALUE_ATR).length()));
    }

    int numOfOpenedBrackets = 1;
    int curIndex = 2;
    std::wstring smiles =
            std::wstring(L"C0") + StringTools::multiplyString(L"C", spiroDescriptors[0][0]) + std::wstring(L"10(");

    // for those molecules where no superstrings compare prefix number with curIndex.
    for (int i = 1; i < spiroDescriptors.size(); i++) {
        if (spiroDescriptors[i][1] >= 0) {
            int ringOpeningPos = findIndexOfRingOpenings(smiles, spiroDescriptors[i][1]);
            std::wstring ringOpeningLabel = StringHelper::toString(smiles[ringOpeningPos]);
            ringOpeningPos++;
            if (ringOpeningLabel == L"%") {
                while (smiles[ringOpeningPos] >= L'0' && smiles[ringOpeningPos] <= L'9' &&
                       ringOpeningPos < smiles.length()) {
                    ringOpeningLabel += smiles[ringOpeningPos];
                    ringOpeningPos++;
                }
            }
            if (smiles.find(std::wstring(L"C") + ringOpeningLabel, ringOpeningPos) != std::wstring::npos) {
                // this ring opening has already been closed
                // i.e. this atom connects more than one ring in a spiro fusion

                // insert extra ring opening
                smiles = smiles.substr(0, ringOpeningPos) + ringClosure(curIndex) + smiles.substr(ringOpeningPos);

                // add ring in new brackets
                smiles += std::wstring(L"(") + StringTools::multiplyString(L"C", spiroDescriptors[i][0]) +
                          ringClosure(curIndex) + std::wstring(L")");
                curIndex++;
            } else {
                smiles += StringTools::multiplyString(L"C", spiroDescriptors[i][0]) + ringOpeningLabel +
                          std::wstring(L")");
            }
        } else if (numOfOpenedBrackets >= numberOfSpiros) {
            smiles += StringTools::multiplyString(L"C", spiroDescriptors[i][0]);

            // take the number before bracket as index for smiles
            // we can open more brackets, this considered in prev if
            curIndex--;
            smiles += ringClosure(curIndex) + std::wstring(L")");

            // from here start to decrease index for the following
        } else {
            smiles += StringTools::multiplyString(L"C", spiroDescriptors[i][0]);
            smiles += std::wstring(L"C") + ringClosure(curIndex++) + std::wstring(L"(");
            numOfOpenedBrackets++;
        }
    }
    chainGroup->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles);
    chainGroup->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::RING_TYPE_VAL);
    if (chainGroup->getAttribute(XmlDeclarations::USABLEASJOINER_ATR) != nullptr) {
        chainGroup->removeAttribute(chainGroup->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
    }
    spiroEl->detach();
}

std::wstring ComponentGenerator::ringClosure(int ringClosure) {
    if (ringClosure > 9) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        return std::wstring(L"%") + Integer::toString(ringClosure);
    } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        return Integer::toString(ringClosure);
    }
}

std::vector<std::vector<int>> ComponentGenerator::getSpiroDescriptors(const std::wstring &text) {
    if ((int) text.find(L"-") == 5) {
        text = text.substr(7, (text.length() - 1) - 7); //cut off spiro-[ and terminal ]
    } else {
        text = text.substr(6, (text.length() - 1) - 6); //cut off spiro[ and terminal ]
    }

//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> spiroDescriptorStrings = matchCommaOrDot->split(text);

//JAVA TO C++ CONVERTER NOTE: The following call to the 'RectangularVectors' helper class reproduces the rectangular array initialization that is automatic in Java:
//ORIGINAL LINE: int[][] spiroDescriptors = new int[spiroDescriptorStrings.length][2];
    std::vector<std::vector<int>> spiroDescriptors = RectangularVectors::ReturnRectangularVector(
            spiroDescriptorStrings.size(), 2); // array of descriptors where number of elements and super string present

    for (int i = 0; i < spiroDescriptorStrings.size(); i++) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> elements = matchNonDigit->split(spiroDescriptorStrings[i]);
        if (elements.size() > 1) { //a "superscripted" number is present
            spiroDescriptors[i][0] = std::stoi(elements[0]);
            StringBuilder *superScriptedNumber = new StringBuilder();
            for (int j = 1; j <
                            elements.size(); j++) { //may be more than one non digit as there are many ways of indicating superscripts
                superScriptedNumber->append(elements[j]);
            }
            spiroDescriptors[i][1] = static_cast<Integer>(superScriptedNumber->toString());
        } else {
            spiroDescriptors[i][0] = std::stoi(spiroDescriptorStrings[i]);
            spiroDescriptors[i][1] = -1;
        }
    }

    return spiroDescriptors;
}

boost::optional<int> ComponentGenerator::findIndexOfRingOpenings(const std::wstring &smiles,
                                                                 int locant) throw(ComponentGenerationException) {
    int count = 0;
    int pos = -1;
    for (int i = 0; i < smiles.length(); i++) {
        if (smiles[i] == L'C') {
            count++;
        }
        if (count == locant) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        throw ComponentGenerationException(
                L"Unable to find atom corresponding to number indicated by superscript in spiro descriptor");
    }
    pos++;
    return pos;
}

void ComponentGenerator::processVonBaeyerSystem(Element *chainEl,
                                                Element *vonBaeyerBracketEl) throw(ComponentGenerationException) {
    std::wstring vonBaeyerBracket = StringTools::removeDashIfPresent(vonBaeyerBracketEl->getValue());
    Element *multiplier = OpsinTools::getPreviousSibling(vonBaeyerBracketEl);
    int numberOfRings = std::stoi(multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
    multiplier->detach();

    int alkylChainLength;
    Deque <std::wstring> *elementSymbolArray = new ArrayDeque<std::wstring>();
    std::wstring smiles = chainEl->getAttributeValue(XmlDeclarations::VALUE_ATR);
    std::vector<wchar_t> smilesArray = smiles.toCharArray();
    for (int i = 0; i <
                    smilesArray.size(); i++) { //only able to interpret the SMILES that should be in an unmodified unbranched chain
        wchar_t currentChar = smilesArray[i];
        if (currentChar == L'[') {
            if (smilesArray[i + 2] == L']') {
                elementSymbolArray->add(
                        std::wstring(L"[") + StringHelper::toString(smilesArray[i + 1]) + std::wstring(L"]"));
                i = i + 2;
            } else {
                elementSymbolArray->add(std::wstring(L"[") + StringHelper::toString(smilesArray[i + 1]) +
                                        StringHelper::toString(smilesArray[i + 2]) + std::wstring(L"]"));
                i = i + 3;
            }
        } else {
            elementSymbolArray->add(StringHelper::toString(currentChar));
        }
    }
    alkylChainLength = elementSymbolArray->size();


    int totalLengthOfBridges = 0;
    int bridgeLabelsUsed = 3; //start labelling from 3 upwards
    //3 and 4 will be the atoms on each end of one secondary bridge, 5 and 6 for the next etc.

    std::vector<std::unordered_map<std::wstring, int>> bridges;
    std::unordered_map<int, std::vector<int>> bridgeLocations(alkylChainLength);
    if ((int) vonBaeyerBracket.find(L"-") == 5) {
        vonBaeyerBracket = vonBaeyerBracket.substr(7, (vonBaeyerBracket.length() - 1) -
                                                      7); //cut off cyclo-[ and terminal ]
    } else {
        vonBaeyerBracket = vonBaeyerBracket.substr(6,
                                                   (vonBaeyerBracket.length() - 1) - 6); //cut off cyclo[ and terminal ]
    }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> bridgeDescriptors = matchCommaOrDot->split(
            vonBaeyerBracket); //the bridgelengths and positions for secondary bridges
    //all bridges from past the first 3 are secondary bridges and require specification of bridge position which will be partially in the subsequent position in the array
    for (int i = 0; i < bridgeDescriptors.size(); i++) {
        std::wstring bridgeDescriptor = bridgeDescriptors[i];
        std::unordered_map<std::wstring, int> bridge;
        int bridgeLength = 0;
        if (i > 2) { //this is a secondary bridge (chain start/end locations should have been specified)
            i++;
            std::wstring coordinatesStr1;
            std::wstring coordinatesStr2 = matchNonDigit->matcher(bridgeDescriptors[i]).replaceAll(L"");
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            std::vector<std::wstring> tempArray = matchNonDigit->split(bridgeDescriptor);

            if (tempArray.size() == 1) {
                //there is some ambiguity as it has not been made obvious which number/s are supposed to be the superscripted locant
                //so we assume that it is more likely that it will be referring to an atom of label >10
                //rather than a secondary bridge of length > 10
                std::vector<wchar_t> tempCharArray = bridgeDescriptor.toCharArray();
                if (tempCharArray.size() == 2) {
                    bridgeLength = Character::getNumericValue(tempCharArray[0]);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    coordinatesStr1 = Character::toString(tempCharArray[1]);
                } else if (tempCharArray.size() == 3) {
                    bridgeLength = Character::getNumericValue(tempCharArray[0]);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    coordinatesStr1 = Character::toString(tempCharArray[1]) + Character::toString(tempCharArray[2]);
                } else if (tempCharArray.size() == 4) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    bridgeLength = static_cast<Integer>(Character::toString(tempCharArray[0]) +
                                                        Character::toString(tempCharArray[1]));
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    coordinatesStr1 = Character::toString(tempCharArray[2]) + Character::toString(tempCharArray[3]);
                } else {
                    throw ComponentGenerationException(
                            std::wstring(L"Unsupported Von Baeyer locant description: ") + bridgeDescriptor);
                }
            } else { //bracket or other delimiter detected, no ambiguity!
                bridgeLength = std::stoi(tempArray[0]);
                coordinatesStr1 = tempArray[1];
            }

            bridge[L"Bridge Length"] = bridgeLength;
            int coordinates1 = std::stoi(coordinatesStr1);
            int coordinates2 = std::stoi(coordinatesStr2);
            if (coordinates1 > alkylChainLength || coordinates2 > alkylChainLength) {
                throw ComponentGenerationException(
                        std::wstring(L"Indicated bridge position is not on chain: ") + coordinates1 +
                        std::wstring(L",") + std::to_wstring(coordinates2));
            }
            if (coordinates2 > coordinates1) { //makes sure that bridges are built from highest coord to lowest
                int swap = coordinates1;
                coordinates1 = coordinates2;
                coordinates2 = swap;
            }
            if (bridgeLocations[coordinates1].empty()) {
                bridgeLocations[coordinates1] = std::vector<int>();
            }
            if (bridgeLocations[coordinates2].empty()) {
                bridgeLocations[coordinates2] = std::vector<int>();
            }
            bridgeLocations[coordinates1].push_back(bridgeLabelsUsed);
            bridge[L"AtomId_Larger_Label"] = bridgeLabelsUsed;
            bridgeLabelsUsed++;
            if (bridgeLength ==
                0) { //0 length bridge, hence want atoms with the same labels so they can join together without a bridge
                bridgeLocations[coordinates2].push_back(bridgeLabelsUsed - 1);
                bridge[L"AtomId_Smaller_Label"] = bridgeLabelsUsed - 1;
            } else {
                bridgeLocations[coordinates2].push_back(bridgeLabelsUsed);
                bridge[L"AtomId_Smaller_Label"] = bridgeLabelsUsed;
            }
            bridgeLabelsUsed++;

            bridge[L"AtomId_Larger"] = coordinates1;
            bridge[L"AtomId_Smaller"] = coordinates2;
        } else {
            bridgeLength = std::stoi(bridgeDescriptor);
            bridge[L"Bridge Length"] = bridgeLength;
        }
        totalLengthOfBridges += bridgeLength;
        bridges.push_back(bridge);
    }
    if (totalLengthOfBridges + 2 != alkylChainLength) {
        throw ComponentGenerationException(L"Disagreement between lengths of bridges and alkyl chain length");
    }
    if (numberOfRings + 1 != bridges.size()) {
        throw ComponentGenerationException(L"Disagreement between number of rings and number of bridges");
    }

    StringBuilder *smilesSB = new StringBuilder();
    int atomCounter = 1;
    int bridgeCounter = 1;
    //add standard bridges
    for (auto bridge : bridges) {
        if (bridgeCounter == 1) {
            smilesSB->append(elementSymbolArray->removeFirst());
            smilesSB->append(L"1");
            if (bridgeLocations[atomCounter].size() > 0) {
                for (* : :optional < int > bridgeAtomLabel : bridgeLocations[atomCounter])
                {
                    smilesSB->append(ringClosure(bridgeAtomLabel));
                }
            }
            smilesSB->append(L"(");
        }
        int bridgeLength = bridge.get(L"Bridge Length");

        for (int i = 0; i < bridgeLength; i++) {
            atomCounter++;
            smilesSB->append(elementSymbolArray->removeFirst());
            if (bridgeLocations[atomCounter].size() > 0) {
                for (* : :optional < int > bridgeAtomLabel : bridgeLocations[atomCounter])
                {
                    smilesSB->append(ringClosure(bridgeAtomLabel));
                }
            }
        }
        if (bridgeCounter == 1) {
            atomCounter++;
            smilesSB->append(elementSymbolArray->removeFirst());
            smilesSB->append(L"2");
            if (bridgeLocations[atomCounter].size() > 0) {
                for (* : :optional < int > bridgeAtomLabel : bridgeLocations[atomCounter])
                {
                    smilesSB->append(ringClosure(bridgeAtomLabel));
                }
            }
        }
        if (bridgeCounter == 2) {
            smilesSB->append(L"1)");
        }
        if (bridgeCounter == 3) {
            smilesSB->append(L"2");
        }
        bridgeCounter++;
        if (bridgeCounter > 3) {
            break;
        }
    }

    //create list of secondary bridges that need to be added
    //0 length bridges and the 3 main bridges are dropped
    std::vector<std::unordered_map<std::wstring, int>> secondaryBridges;
    for (auto bridge : bridges) {
        if (bridge.get(L"AtomId_Larger") != nullptr && bridge.get(L"Bridge Length") != 0) {
            secondaryBridges.push_back(bridge);
        }
    }

    Comparator <std::unordered_map<std::wstring, int>> *sortBridges = new VonBaeyerSecondaryBridgeSort();
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(secondaryBridges, sortBridges);
    std::sort(secondaryBridges.begin(), secondaryBridges.end(), sortBridges);

    std::vector<std::unordered_map<std::wstring, int>> dependantSecondaryBridges;
    //add secondary bridges, recursively add dependent secondary bridges
    do {
        dependantSecondaryBridges = std::vector<std::unordered_map<std::wstring, int>>();
        for (auto bridge : secondaryBridges) {
            int bridgeLength = bridge.get(L"Bridge Length");
            if (bridge.get(L"AtomId_Larger") > atomCounter) {
                dependantSecondaryBridges.push_back(bridge);
                continue;
            }
            smilesSB->append(L".");
            for (int i = 0; i < bridgeLength; i++) {
                atomCounter++;
                smilesSB->append(elementSymbolArray->removeFirst());
                if (i == 0) {
                    smilesSB->append(ringClosure(bridge.get(L"AtomId_Larger_Label")));
                }
                if (bridgeLocations[atomCounter].size() > 0) {
                    for (* : :optional < int > bridgeAtomLabel : bridgeLocations[atomCounter])
                    {
                        smilesSB->append(ringClosure(bridgeAtomLabel));
                    }
                }
            }
            smilesSB->append(ringClosure(bridge.get(L"AtomId_Smaller_Label")));
        }
        if (dependantSecondaryBridges.size() > 0 && dependantSecondaryBridges.size() == secondaryBridges.size()) {
            throw ComponentGenerationException(L"Unable to resolve all dependant bridges!!!");
        }
        secondaryBridges = dependantSecondaryBridges;
    } while (dependantSecondaryBridges.size() > 0);

    chainEl->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smilesSB->toString());
    chainEl->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::RING_TYPE_VAL);
    if (chainEl->getAttribute(XmlDeclarations::USABLEASJOINER_ATR) != nullptr) {
        chainEl->removeAttribute(chainEl->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
    }
    vonBaeyerBracketEl->detach();
}

void
ComponentGenerator::processCyclisedChain(Element *chainGroup, Element *cycloEl) throw(ComponentGenerationException) {
    std::wstring smiles = chainGroup->getAttributeValue(XmlDeclarations::VALUE_ATR);
    int chainlen = 0;
    for (int i = smiles.length() - 1; i >= 0; i--) {
        if (std::isupper(smiles[i]) && smiles[i] != L'H') {
            chainlen++;
        }
    }
    if (chainlen < 3) {
        throw ComponentGenerationException(
                std::wstring(L"Heteroatom chain too small to create a ring: ") + std::to_wstring(chainlen));
    }
    smiles += std::wstring(L"1");
    if (smiles[0] == L'[') {
        int closeBracketIndex = (int) smiles.find(L']');
        smiles = smiles.substr(0, closeBracketIndex + 1) + std::wstring(L"1") + smiles.substr(closeBracketIndex + 1);
    } else {
        if (Character::getType(smiles[1]) == Character::LOWERCASE_LETTER) { //element is 2 letters long
            smiles = smiles.substr(0, 2) + std::wstring(L"1") + smiles.substr(2);
        } else {
            smiles = smiles.substr(0, 1) + std::wstring(L"1") + smiles.substr(1);
        }
    }
    chainGroup->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles);
    if (chainlen == 6) { //6 membered rings have ortho/meta/para positions
        if (chainGroup->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr) {
            chainGroup->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(L"1/2,ortho/3,meta/4,para/5/6");
        } else {
            Attribute tempVar(XmlDeclarations::LABELS_ATR, L"1/2,ortho/3,meta/4,para/5/6");
            chainGroup->addAttribute(&tempVar);
        }
    }
    chainGroup->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::RING_TYPE_VAL);
    if (chainGroup->getAttribute(XmlDeclarations::USABLEASJOINER_ATR) != nullptr) {
        chainGroup->removeAttribute(chainGroup->getAttribute(XmlDeclarations::USABLEASJOINER_ATR));
    }
    cycloEl->detach();
}

void ComponentGenerator::handleGroupIrregularities(Element *group) throw(ComponentGenerationException) {
    std::wstring groupValue = group->getValue();

    if (!n2sConfig->allowInterpretationOfAcidsWithoutTheWordAcid()) {
        if (group->getAttribute(XmlDeclarations::FUNCTIONALIDS_ATR) != nullptr &&
            (boost::ends_with(groupValue, L"ic") || boost::ends_with(groupValue, L"ous"))) {
            Element *next = OpsinTools::getNext(group);
            if (next == nullptr) {
                throw ComponentGenerationException(std::wstring(L"\"acid\" not found after ") + groupValue);
            }
        }
    }

    if (groupValue == L"thiophen" || groupValue == L"selenophen" || groupValue ==
                                                                    L"tellurophen") { //thiophenol is generally phenol with an O replaced with S not thiophene with a hydroxy
        Element *possibleSuffix = OpsinTools::getNextSibling(group);
        if ((std::wstring(L"e")) != group->getAttributeValue(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) &&
            possibleSuffix != nullptr && possibleSuffix->getName() == XmlDeclarations::SUFFIX_EL) {
            if (boost::starts_with(possibleSuffix->getValue(), L"ol")) {
                Element *isThisALocant = OpsinTools::getPreviousSibling(group);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                if (isThisALocant == nullptr || isThisALocant->getName() != XmlDeclarations::LOCANT_EL ||
                    isThisALocant->getValue().split(L",")->length != 1) {
                    throw ComponentGenerationException(
                            groupValue + std::wstring(L"ol has been incorrectly interpreted as ") + groupValue +
                            std::wstring(L", ol instead of phenol with the oxgen replaced"));
                }
            }
        }
    } else if (groupValue ==
               L"chromen") { //chromene in IUPAC nomenclature is fully unsaturated, but sometimes is instead considered to be chromane with a front locanted double bond
        Element *possibleLocant = OpsinTools::getPreviousSibling(group);
        if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL &&
            (possibleLocant->getValue() == L"2" || possibleLocant->getValue() == L"3")) {
            Element *possibleSuffix = OpsinTools::getNextSibling(group);
            if (possibleSuffix == nullptr || possibleSuffix->getName() ==
                                             XmlDeclarations::LOCANT_EL) { //if there is a suffix assume the locant refers to that rather than the double bond
                group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(L"O1CCCc2ccccc12");
                group->addAttribute(XmlDeclarations::ADDBOND_ATR, L"2 locant required");
                group->addAttribute(XmlDeclarations::FRONTLOCANTSEXPECTED_ATR, L"2,3");
            }
        }
    } else if (groupValue == L"methylene" || groupValue == L"methylen") { //e.g. 3,4-methylenedioxyphenyl
        Element *nextSub = OpsinTools::getNextSibling(group->getParent());
        if (nextSub != nullptr && nextSub->getName() == XmlDeclarations::SUBSTITUENT_EL &&
            OpsinTools::getNextSibling(group) == nullptr && (OpsinTools::getPreviousSibling(group) == nullptr ||
                                                             OpsinTools::getPreviousSibling(group)->getName() !=
                                                             XmlDeclarations::MULTIPLIER_EL)) { //not trimethylenedioxy
            std::vector<Element *> children = nextSub->getChildElements();
            if (children.size() >= 2 && children[0]->getValue() == L"di" && children[1]->getValue() == L"oxy") {
                group->setValue(groupValue + std::wstring(L"dioxy"));
                group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(L"C(O)O");
                group->getAttribute(XmlDeclarations::OUTIDS_ATR)->setValue(L"2,3");
                group->getAttribute(XmlDeclarations::SUBTYPE_ATR)->setValue(XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL);
                if (group->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr) {
                    group->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(XmlDeclarations::NONE_LABELS_VAL);
                } else {
                    Attribute tempVar(XmlDeclarations::LABELS_ATR, XmlDeclarations::NONE_SUBTYPE_VAL);
                    group->addAttribute(&tempVar);
                }
                nextSub->detach();
                for (int i = children.size() - 1; i >= 2; i--) {
                    children[i]->detach();
                    OpsinTools::insertAfter(group, children[i]);
                }
            }
        }
    } else if (groupValue == L"ethylene" || groupValue == L"ethylen") {
        Element *previous = OpsinTools::getPreviousSibling(group);
        if (previous != nullptr && previous->getName() == XmlDeclarations::MULTIPLIER_EL) {
            int multiplierValue = std::stoi(previous->getAttributeValue(XmlDeclarations::VALUE_ATR));
            Element *possibleRoot = OpsinTools::getNextSibling(group->getParent());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            if (possibleRoot == nullptr &&
                OpsinTools::getParentWordRule(group)->getAttributeValue(XmlDeclarations::WORDRULE_ATR) ==
                WordRule::glycol->toString()) { //e.g. dodecaethylene glycol
                StringBuilder *smiles = new StringBuilder(L"CC");
                for (int i = 1; i < multiplierValue; i++) {
                    smiles->append(L"OCC");
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                group->getAttribute(XmlDeclarations::OUTIDS_ATR)->setValue(
                        std::wstring(L"1,") + Integer::toString(3 * (multiplierValue - 1) + 2));
                group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles->toString());
                previous->detach();
                if (group->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr) { //use numeric numbering
                    group->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(XmlDeclarations::NUMERIC_LABELS_VAL);
                } else {
                    Attribute tempVar2(XmlDeclarations::LABELS_ATR, XmlDeclarations::NUMERIC_LABELS_VAL);
                    group->addAttribute(&tempVar2);
                }
            } else if (possibleRoot != nullptr && possibleRoot->getName() == XmlDeclarations::ROOT_EL) {
                std::vector<Element *> children = possibleRoot->getChildElements();
                if (children.size() == 2) {
                    Element *amineMultiplier = children[0];
                    Element *amine = children[1];
                    if (amineMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL &&
                        (amine->getValue() == L"amine" || amine->getValue() == L"amin")) { //e.g. Triethylenetetramine
                        if (std::stoi(amineMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR)) !=
                            multiplierValue + 1) {
                            throw ComponentGenerationException(L"Invalid polyethylene amine!");
                        }
                        StringBuilder *smiles = new StringBuilder();
                        for (int i = 0; i < multiplierValue; i++) {
                            smiles->append(L"NCC");
                        }
                        smiles->append(L"N");
                        group->removeAttribute(group->getAttribute(XmlDeclarations::OUTIDS_ATR));
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles->toString());
                        previous->detach();
                        possibleRoot->detach();
                        group->getParent()->setName(XmlDeclarations::ROOT_EL);
                        if (group->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr) { //use numeric numbering
                            group->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(
                                    XmlDeclarations::NUMERIC_LABELS_VAL);
                        } else {
                            Attribute tempVar3(XmlDeclarations::LABELS_ATR, XmlDeclarations::NUMERIC_LABELS_VAL);
                            group->addAttribute(&tempVar3);
                        }
                    }
                }
            }
        } else {
            Element *nextSub = OpsinTools::getNextSibling(group->getParent());
            if (nextSub != nullptr && nextSub->getName() == XmlDeclarations::SUBSTITUENT_EL &&
                OpsinTools::getNextSibling(group) == nullptr) {
                std::vector<Element *> children = nextSub->getChildElements();
                if (children.size() >= 2 && children[0]->getValue() == L"di" && children[1]->getValue() == L"oxy") {
                    group->setValue(groupValue + std::wstring(L"dioxy"));
                    group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(L"C(O)CO");
                    group->getAttribute(XmlDeclarations::OUTIDS_ATR)->setValue(L"2,4");
                    group->getAttribute(XmlDeclarations::SUBTYPE_ATR)->setValue(XmlDeclarations::EPOXYLIKE_SUBTYPE_VAL);
                    if (group->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr) {
                        group->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(XmlDeclarations::NONE_LABELS_VAL);
                    } else {
                        Attribute tempVar4(XmlDeclarations::LABELS_ATR, XmlDeclarations::NONE_SUBTYPE_VAL);
                        group->addAttribute(&tempVar4);
                    }
                    nextSub->detach();
                    for (int i = children.size() - 1; i >= 2; i--) {
                        children[i]->detach();
                        OpsinTools::insertAfter(group, children[i]);
                    }
                }
            }
        }
    } else if (groupValue == L"propylene" || groupValue == L"propylen") {
        Element *previous = OpsinTools::getPreviousSibling(group);
        if (previous != nullptr && previous->getName() == XmlDeclarations::MULTIPLIER_EL) {
            int multiplierValue = std::stoi(previous->getAttributeValue(XmlDeclarations::VALUE_ATR));
            Element *possibleRoot = OpsinTools::getNextSibling(group->getParent());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            if (possibleRoot == nullptr &&
                OpsinTools::getParentWordRule(group)->getAttributeValue(XmlDeclarations::WORDRULE_ATR) ==
                WordRule::glycol->toString()) { //e.g. dodecaethylene glycol
                StringBuilder *smiles = new StringBuilder(L"CCC");
                for (int i = 1; i < multiplierValue; i++) {
                    smiles->append(L"OC(C)C");
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                group->getAttribute(XmlDeclarations::OUTIDS_ATR)->setValue(
                        std::wstring(L"2,") + Integer::toString(4 * (multiplierValue - 1) + 3));
                group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(smiles->toString());
                if (group->getAttribute(XmlDeclarations::LABELS_ATR) != nullptr) {
                    group->getAttribute(XmlDeclarations::LABELS_ATR)->setValue(XmlDeclarations::NONE_LABELS_VAL);
                } else {
                    Attribute tempVar5(XmlDeclarations::LABELS_ATR, XmlDeclarations::NONE_LABELS_VAL);
                    group->addAttribute(&tempVar5);
                }
                previous->detach();
            }
        }
    }

        //acridone (not codified), anthrone, phenanthrone and xanthone have the one at position 9 by default
    else if (groupValue == L"anthr" || groupValue == L"phenanthr" || groupValue == L"acrid" || groupValue == L"xanth" ||
             groupValue == L"thioxanth" || groupValue == L"selenoxanth" || groupValue == L"telluroxanth" ||
             groupValue == L"xanthen") {
        Element *possibleLocant = OpsinTools::getPreviousSibling(group);
        if (possibleLocant == nullptr || possibleLocant->getName() !=
                                         XmlDeclarations::LOCANT_EL) { //only need to give one a locant of 9 if no locant currently present
            Element *possibleSuffix = OpsinTools::getNextSibling(group);
            if (possibleSuffix != nullptr &&
                (std::wstring(L"one")) == possibleSuffix->getAttributeValue(XmlDeclarations::VALUE_ATR)) {
                //Rule C-315.2
                Element *newLocant = new TokenEl(XmlDeclarations::LOCANT_EL, L"9");
                OpsinTools::insertBefore(possibleSuffix, newLocant);
                Element *newAddedHydrogen = new TokenEl(XmlDeclarations::ADDEDHYDROGEN_EL);
                Attribute tempVar6(XmlDeclarations::LOCANT_ATR, L"10");
                newAddedHydrogen->addAttribute(&tempVar6);
                OpsinTools::insertBefore(newLocant, newAddedHydrogen);
            } else if (possibleSuffix != nullptr && possibleSuffix->getName() == XmlDeclarations::SUFFIX_EL &&
                       groupValue == L"xanth" || groupValue == L"thioxanth" || groupValue == L"selenoxanth" ||
                       groupValue == L"telluroxanth") {
                //diasambiguate between xanthate/xanthic acid and xanthene
                std::wstring suffixVal = possibleSuffix->getAttributeValue(XmlDeclarations::VALUE_ATR);
                if (suffixVal == L"ic" || suffixVal == L"ate") {
                    throw ComponentGenerationException(groupValue + possibleSuffix->getValue() +
                                                       std::wstring(L" is not a derivative of xanthene"));
                }
            }
        }
    } else if (groupValue == L"phospho") { //is this the organic meaning (P(=O)=O) or biochemical meaning (P(=O)(O)O)
        Element *wordRule = OpsinTools::getParentWordRule(group);
        for (auto otherGroup : OpsinTools::getDescendantElementsWithTagName(wordRule, XmlDeclarations::GROUP_EL)) {
            std::wstring type = otherGroup->getAttributeValue(XmlDeclarations::TYPE_ATR);
            std::wstring subType = otherGroup->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
            if (type == XmlDeclarations::AMINOACID_TYPE_VAL || type == XmlDeclarations::CARBOHYDRATE_TYPE_VAL ||
                XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL == subType ||
                (XmlDeclarations::YLFORACYL_SUBTYPE_VAL == subType &&
                 ((std::wstring(L"glycol")) == otherGroup->getValue() ||
                  (std::wstring(L"diglycol")) == otherGroup->getValue()))) {
                group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(L"-P(=O)(O)O");
                Attribute tempVar7(XmlDeclarations::USABLEASJOINER_ATR, L"yes");
                group->addAttribute(&tempVar7);
                break;
            }
        }
    } else if (groupValue == L"hydrogen") {
        Element *hydrogenParentEl = group->getParent();
        Element *nextSubOrRoot = OpsinTools::getNextSibling(hydrogenParentEl);
        if (nextSubOrRoot != nullptr) {
            Element *possibleSuitableAteGroup = nextSubOrRoot->getChild(0);
            if (possibleSuitableAteGroup->getName() != XmlDeclarations::GROUP_EL ||
                XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL !=
                possibleSuitableAteGroup->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
                throw ComponentGenerationException(L"Hydrogen is not meant as a substituent in this context!");
            }
            Element *possibleMultiplier = OpsinTools::getPreviousSibling(group);
            std::wstring multiplier = L"1";
            if (possibleMultiplier != nullptr && possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                multiplier = possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR);
                possibleMultiplier->detach();
            }
            Attribute tempVar8(XmlDeclarations::NUMBEROFFUNCTIONALATOMSTOREMOVE_ATR, multiplier);
            possibleSuitableAteGroup->addAttribute(&tempVar8);
            group->detach();
            std::vector<Element *> childrenToMove = hydrogenParentEl->getChildElements();
            for (int i = childrenToMove.size() - 1; i >= 0; i--) {
                childrenToMove[i]->detach();
                nextSubOrRoot->insertChild(childrenToMove[i], 0);
            }
            hydrogenParentEl->detach();
        }
    } else if (groupValue == L"acryl") {
        if (XmlDeclarations::SIMPLESUBSTITUENT_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
            Element *nextEl = OpsinTools::getNext(group);
            if (nextEl != nullptr && nextEl->getValue() == L"amid") {
                throw ComponentGenerationException(L"amide in acrylamide is not [NH2-]");
            }
        }
    } else if (groupValue == L"azo" || groupValue == L"azoxy" || groupValue == L"nno-azoxy" ||
               groupValue == L"non-azoxy" || groupValue == L"onn-azoxy" || groupValue == L"diazoamino" ||
               groupValue == L"hydrazo") {
        Element *enclosingSub = group->getParent();
        Element *next = OpsinTools::getNextSiblingIgnoringCertainElements(enclosingSub, std::vector<std::wstring>{
                XmlDeclarations::HYPHEN_EL});
        if (next == nullptr && OpsinTools::getPreviousSibling(enclosingSub) == nullptr) { //e.g. [(E)-NNO-azoxy]benzene
            next = OpsinTools::getNextSiblingIgnoringCertainElements(enclosingSub->getParent(),
                                                                     std::vector<std::wstring>{
                                                                             XmlDeclarations::HYPHEN_EL});
        }
        if (next != nullptr && next->getName() == XmlDeclarations::ROOT_EL) {
            if (!(next->getChild(0)->getName() == XmlDeclarations::MULTIPLIER_EL)) {
                std::vector<Element *> suffixes = next->getChildElements(XmlDeclarations::SUFFIX_EL);
                if (suffixes.empty()) { //only case without locants is handled so far. suffixes only apply to one of the fragments rather than both!!!
                    Element *newMultiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
                    Attribute tempVar9(XmlDeclarations::VALUE_ATR, L"2");
                    newMultiplier->addAttribute(&tempVar9);
                    next->insertChild(newMultiplier, 0);
                    Element *interSubstituentHyphen = OpsinTools::getPrevious(group);
                    if (interSubstituentHyphen != nullptr && interSubstituentHyphen->getName() !=
                                                             XmlDeclarations::HYPHEN_EL) { //prevent implicit bracketting
                        TokenEl tempVar10(XmlDeclarations::HYPHEN_EL);
                        OpsinTools::insertAfter(interSubstituentHyphen, &tempVar10);
                    }
                }
            }
        }
    } else if (groupValue == L"coenzyme a" || groupValue == L"coa") {
        Element *enclosingSubOrRoot = group->getParent();
        Element *previous = OpsinTools::getPreviousSibling(enclosingSubOrRoot);
        if (previous != nullptr) {
            std::vector<Element *> groups = OpsinTools::getDescendantElementsWithTagName(previous,
                                                                                         XmlDeclarations::GROUP_EL);
            if (groups.size() > 0) {
                Element *possibleAcid = groups[groups.size() - 1];
                if (XmlDeclarations::ACIDSTEM_TYPE_VAL == possibleAcid->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
                    if (possibleAcid->getAttribute(XmlDeclarations::SUFFIXAPPLIESTO_ATR) !=
                        nullptr) { //multi acid. yl should be one oyl and the rest carboxylic acids
                        Element *suffix = OpsinTools::getNextSibling(possibleAcid, XmlDeclarations::SUFFIX_EL);
                        if (suffix->getAttribute(XmlDeclarations::ADDITIONALVALUE_ATR) == nullptr) {
                            Attribute tempVar11(XmlDeclarations::ADDITIONALVALUE_ATR, L"ic");
                            suffix->addAttribute(&tempVar11);
                        }
                    }
                    std::wstring subType = possibleAcid->getAttributeValue(XmlDeclarations::SUBTYPE_ATR);
                    if (subType == XmlDeclarations::YLFORYL_SUBTYPE_VAL ||
                        subType == XmlDeclarations::YLFORNOTHING_SUBTYPE_VAL) {
                        possibleAcid->getAttribute(XmlDeclarations::SUBTYPE_ATR)->setValue(
                                XmlDeclarations::YLFORACYL_SUBTYPE_VAL); //yl always means an acyl when next to coenzyme A
                    }
                }
            }
        }
        //locanted substitution onto Coenzyme A is rarely intended, so put prior content into a bracket to disfavour it
        Element *enclosingBracketOrWord = enclosingSubOrRoot->getParent();
        int indexOfCoa = enclosingBracketOrWord->find(enclosingSubOrRoot);
        if (indexOfCoa > 0) {
            Element *newBracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
            std::vector<Element *> precedingElements = enclosingBracketOrWord->getChildElements();
            for (int i = 0; i < indexOfCoa; i++) {
                Element *precedingElement = precedingElements[i];
                precedingElement->detach();
                newBracket->addChild(precedingElement);
            }
            OpsinTools::insertBefore(enclosingSubOrRoot, newBracket);
        }
    } else if (groupValue == L"sphinganine" || groupValue == L"icosasphinganine" ||
               groupValue == L"eicosasphinganine" || groupValue == L"phytosphingosine" ||
               groupValue == L"sphingosine" || groupValue == L"sphinganin" || groupValue == L"icosasphinganin" ||
               groupValue == L"eicosasphinganin" || groupValue == L"phytosphingosin" || groupValue == L"sphingosin") {
        Element *enclosingSubOrRoot = group->getParent();
        Element *previous = OpsinTools::getPreviousSibling(enclosingSubOrRoot);
        if (previous != nullptr) {
            std::vector<Element *> groups = OpsinTools::getDescendantElementsWithTagName(previous,
                                                                                         XmlDeclarations::GROUP_EL);
            if (groups.size() > 0) {
                Element *possibleAcid = groups[groups.size() - 1];
                if (XmlDeclarations::ALKANESTEM_SUBTYPE_VAL ==
                    possibleAcid->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                    std::vector<Element *> inlineSuffixes = OpsinTools::getChildElementsWithTagNameAndAttribute(
                            possibleAcid->getParent(), XmlDeclarations::SUFFIX_EL, XmlDeclarations::TYPE_ATR,
                            XmlDeclarations::INLINE_TYPE_VAL);
                    if (inlineSuffixes.size() == 1 &&
                        inlineSuffixes[0]->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"yl") {
                        inlineSuffixes[0]->getAttribute(XmlDeclarations::VALUE_ATR).setValue(
                                L"oyl"); //yl on a systematic acid next to a fatty acid means acyl
                        //c.f. Nomenclature of Lipids 1976, Appendix A, note a
                    }
                }
            }
        }
    } else if (groupValue == L"sel") {
        //check that it is not "selenium"
        if (XmlDeclarations::HETEROSTEM_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) &&
            group->getAttribute(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) == nullptr) {
            Element *unsaturator = OpsinTools::getNextSibling(group);
            if (unsaturator != nullptr && unsaturator->getName() == XmlDeclarations::UNSATURATOR_EL &&
                unsaturator->getValue() == L"en" &&
                group->getAttribute(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) == nullptr) {
                Element *ium = OpsinTools::getNextSibling(unsaturator);
                if (ium != nullptr && ium->getName() == XmlDeclarations::SUFFIX_EL && ium->getValue() == L"ium") {
                    throw ComponentGenerationException(
                            L"<multiplier>selenium does not indicate a chain of selenium atoms with a double bond and a positive charge");
                }
            }
        }
    } else if ((groupValue == L"keto" || groupValue == L"aldehydo") && XmlDeclarations::SIMPLESUBSTITUENT_SUBTYPE_VAL ==
                                                                       group->getAttributeValue(
                                                                               XmlDeclarations::SUBTYPE_ATR)) {
        //check for case where this is specifying the open chain form of a ketose/aldose
        Element *previousEl = OpsinTools::getPreviousSibling(group);
        if (previousEl == nullptr || previousEl->getName() != XmlDeclarations::LOCANT_EL || groupValue == L"aldehydo") {
            Element *parentSubstituent = group->getParent();
            Element *nextSubOrRoot = OpsinTools::getNextSibling(parentSubstituent);
            Element *parentOfCarbohydate = nextSubOrRoot;
            Element *carbohydrate = nullptr;
            while (parentOfCarbohydate != nullptr) {
                Element *possibleCarbohydrate = parentOfCarbohydate->getFirstChildElement(XmlDeclarations::GROUP_EL);
                if (possibleCarbohydrate != nullptr &&
                    possibleCarbohydrate->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
                    XmlDeclarations::CARBOHYDRATE_TYPE_VAL) {
                    carbohydrate = possibleCarbohydrate;
                    break;
                }
                parentOfCarbohydate = OpsinTools::getNextSibling(parentOfCarbohydate);
            }
            if (carbohydrate != nullptr) {
                if (parentOfCarbohydate->getChildElements(XmlDeclarations::CARBOHYDRATERINGSIZE_EL).size() > 0) {
                    throw ComponentGenerationException(
                            std::wstring(L"Carbohydrate has a specified ring size but ") + groupValue +
                            std::wstring(L" indicates the open chain form!"));
                }
                for (auto suffix : parentOfCarbohydate->getChildElements(XmlDeclarations::SUFFIX_EL)) {
                    if ((std::wstring(L"yl")) == suffix->getAttributeValue(XmlDeclarations::VALUE_ATR)) {
                        throw ComponentGenerationException(
                                std::wstring(L"Carbohydrate appears to be a glycosyl, but ") + groupValue +
                                std::wstring(L" indicates the open chain form!"));
                    }
                }
                Element *alphaOrBetaLocantEl = OpsinTools::getPreviousSiblingIgnoringCertainElements(carbohydrate,
                                                                                                     std::vector<std::wstring>{
                                                                                                             XmlDeclarations::STEREOCHEMISTRY_EL});
                if (alphaOrBetaLocantEl != nullptr && alphaOrBetaLocantEl->getName() == XmlDeclarations::LOCANT_EL) {
                    std::wstring value = alphaOrBetaLocantEl->getValue();
                    if (value == L"alpha" || value == L"beta" || value == L"alpha,beta" || value == L"beta,alpha") {
                        throw ComponentGenerationException(
                                std::wstring(L"Carbohydrate has alpha/beta anomeric form but ") + groupValue +
                                std::wstring(L" indicates the open chain form!"));
                    }
                }
                group->detach();
                std::vector<Element *> childrenToMove = parentSubstituent->getChildElements();
                for (int i = childrenToMove.size() - 1; i >= 0; i--) {
                    Element *el = childrenToMove[i];
                    if (el->getName() != XmlDeclarations::HYPHEN_EL) {
                        el->detach();
                        nextSubOrRoot->insertChild(el, 0);
                    }
                }
                parentSubstituent->detach();
                if (XmlDeclarations::RING_SUBTYPE_VAL ==
                    carbohydrate->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
                    std::wstring carbohydrateAdditionValue = carbohydrate->getAttributeValue(
                            XmlDeclarations::ADDITIONALVALUE_ATR);
                    //OPSIN assumes a few trivial names are more likely to describe the cyclic form. additionalValue contains the SMILES for the acyclic form
                    if (carbohydrateAdditionValue == L"") {
                        throw ComponentGenerationException(
                                carbohydrate->getValue() + std::wstring(L" can only describe the cyclic form but ") +
                                groupValue + std::wstring(L" indicates the open chain form!"));
                    }
                    carbohydrate->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(carbohydrateAdditionValue);
                }
            } else if (groupValue == L"aldehydo") {
                throw ComponentGenerationException(L"aldehydo is only a valid prefix when it precedes a carbohydrate!");
            }
        }
    } else if (groupValue == L"bor" || groupValue == L"antimon" || groupValue == L"arsen" ||
               groupValue == L"phosphor" || groupValue == L"phosphate" || groupValue == L"phosphat" ||
               groupValue == L"silicicacid" || groupValue == L"silicic acid" || groupValue == L"silicate" ||
               groupValue ==
               L"silicat") { //fluoroboric acid/fluoroborate are trivial rather than systematic; tetra(fooyl)borate is inorganic
        Element *suffix = nullptr;
        boost::optional<bool> isAcid = boost::none;
        if (boost::ends_with(groupValue, L"acid")) {
            if (OpsinTools::getNext(group) == nullptr) {
                isAcid = true;
            }
        } else if (boost::ends_with(groupValue, L"ate") || boost::ends_with(groupValue, L"at")) {
            if (OpsinTools::getNext(group) == nullptr) {
                isAcid = false;
            }
        } else {
            suffix = OpsinTools::getNextSibling(group);
            if (suffix != nullptr && suffix->getName() == XmlDeclarations::SUFFIX_EL &&
                suffix->getAttribute(XmlDeclarations::INFIX_ATR) == nullptr && OpsinTools::getNext(suffix) == nullptr) {
                std::wstring suffixValue = suffix->getAttributeValue(XmlDeclarations::VALUE_ATR);
                if (suffixValue == L"ic") {
                    isAcid = true;
                } else if (suffixValue == L"ate") {
                    isAcid = false;
                }
            }
        }
        if (isAcid) { //check for inorganic interpretation
            Element *substituent = OpsinTools::getPreviousSibling(group->getParent());
            if (substituent != nullptr && (substituent->getName() == XmlDeclarations::SUBSTITUENT_EL ||
                                           substituent->getName() == XmlDeclarations::BRACKET_EL)) {
                std::vector<Element *> children = substituent->getChildElements();
                Element *firstChild = children[0];
                bool matched = false;
                if (children.size() == 1 && firstChild->getName() == XmlDeclarations::GROUP_EL &&
                    (firstChild->getValue() == L"fluoro" || firstChild->getValue() == L"fluor")) {
                    if (groupValue == L"bor") {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(
                                isAcid ? L"F[B-](F)(F)F.[H+]" : L"F[B-](F)(F)F");
                        matched = true;
                    } else if (groupValue == L"antimon") {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(
                                isAcid ? L"F[Sb-](F)(F)(F)(F)F.[H+]" : L"F[Sb-](F)(F)(F)(F)F");
                        matched = true;
                    } else if (boost::starts_with(groupValue, L"silicic")) {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(
                                isAcid ? L"F[Si|6-2](F)(F)(F)(F)F.[H+].[H+]" : L"F[Si|6-2](F)(F)(F)(F)F");
                        matched = true;
                    }
                    if (matched) {
                        substituent->detach();
                    }
                } else if (firstChild->getName() == XmlDeclarations::MULTIPLIER_EL) {
                    std::wstring multiplierVal = firstChild->getAttributeValue(XmlDeclarations::VALUE_ATR);

                    if (groupValue == L"bor") {
                        if (multiplierVal == L"4" ||
                            (multiplierVal == L"3" && OpsinTools::getPreviousSibling(substituent) != nullptr)) {
                            //tri case allows organotrifluoroborates
                            group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(isAcid ? L"[B-].[H+]" : L"[B-]");
                            matched = true;
                        }
                    } else if (groupValue == L"antimon" && multiplierVal == L"6") {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(isAcid ? L"[Sb-].[H+]" : L"[Sb-]");
                        matched = true;
                    } else if (groupValue == L"arsen" && multiplierVal == L"6") {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(isAcid ? L"[As-].[H+]" : L"[As-]");
                        matched = true;
                    } else if (boost::starts_with(groupValue, L"phosph") && multiplierVal == L"6") {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(isAcid ? L"[P-].[H+]" : L"[P-]");
                        matched = true;
                    } else if (boost::starts_with(groupValue, L"silic") && multiplierVal == L"6") {
                        group->getAttribute(XmlDeclarations::VALUE_ATR)->setValue(
                                isAcid ? L"[Si|6-2].[H+].[H+]" : L"[Si|6-2]");
                        matched = true;
                    }
                }
                if (matched) {
                    group->getAttribute(XmlDeclarations::TYPE_ATR)->setValue(XmlDeclarations::SIMPLEGROUP_TYPE_VAL);
                    group->getAttribute(XmlDeclarations::SUBTYPE_ATR)->setValue(
                            XmlDeclarations::SIMPLEGROUP_SUBTYPE_VAL);

                    Attribute *usableAsJoiner = group->getAttribute(XmlDeclarations::USABLEASJOINER_ATR);
                    if (usableAsJoiner != nullptr) {
                        group->removeAttribute(usableAsJoiner);
                    }
                    Attribute *acceptsAdditiveBonds = group->getAttribute(XmlDeclarations::ACCEPTSADDITIVEBONDS_ATR);
                    if (acceptsAdditiveBonds != nullptr) {
                        group->removeAttribute(acceptsAdditiveBonds);
                    }
                    Attribute *functionalIds = group->getAttribute(XmlDeclarations::FUNCTIONALIDS_ATR);
                    if (functionalIds != nullptr) {
                        group->removeAttribute(functionalIds);
                    }

                    if (suffix != nullptr) {
                        suffix->detach();
                    }
                }
            }
        }
    } else if (XmlDeclarations::ENDINIC_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR) &&
               XmlDeclarations::AMINOACID_TYPE_VAL == group->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
        //aspartyl and glutamyl typically mean alpha-aspartyl/alpha-glutamyl
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> suffixAppliesTo = group->getAttributeValue(
                XmlDeclarations::SUFFIXAPPLIESTO_ATR).split(L",");
        if (suffixAppliesTo.size() == 2) {
            Element *yl = OpsinTools::getNextSibling(group);
            if (yl->getAttributeValue(XmlDeclarations::VALUE_ATR) == L"yl") {
                if (yl->getAttribute(XmlDeclarations::ADDITIONALVALUE_ATR) == nullptr) {
                    Attribute tempVar12(XmlDeclarations::ADDITIONALVALUE_ATR, L"ic");
                    yl->addAttribute(&tempVar12);
                }
            }
        }
    } else if (XmlDeclarations::SALTCOMPONENT_SUBTYPE_VAL == group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
        Element *parse = nullptr;
        Element *tempParent = group->getParent();
        while (tempParent != nullptr) {
            parse = tempParent;
            tempParent = tempParent->getParent();
        }
        if (parse->getChildCount() <= 1) {
            throw ComponentGenerationException(std::wstring(
                    L"Group expected to be part of a salt but only one component found. Could be a class of compound: ") +
                                               groupValue);
        }
        if (groupValue.length() > 0) {
            //e.g. 2HCl
            wchar_t firstChar = groupValue[0];
            if (firstChar >= L'1' && firstChar <= L'9') {
                Element *shouldntBeAmultiplier = OpsinTools::getPreviousSibling(group);
                if (shouldntBeAmultiplier != nullptr &&
                    shouldntBeAmultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                    throw ComponentGenerationException(
                            std::wstring(L"Unepxected multiplier found before: ") + groupValue);
                }
                Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL, StringHelper::toString(firstChar));
                multiplier->addAttribute(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
                multiplier->addAttribute(XmlDeclarations::VALUE_ATR, StringHelper::toString(firstChar));
                OpsinTools::insertBefore(group, multiplier);
                group->setValue(groupValue.substr(1));
            }
        }
    }
}
