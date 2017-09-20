#include "FunctionalReplacement.h"
#include "BuildState.h"
#include "ComponentGenerationException.h"
#include "Element.h"
#include "StructureBuildingException.h"
#include "OpsinTools.h"
#include "WordRule.h"
#include "XmlDeclarations.h"
#include "WordType.h"
#include "Fragment.h"
#include "StringTools.h"
#include "Atom.h"
#include "StructureBuildingMethods.h"
#include "ChemEl.h"
#include "Bond.h"
#include "Attribute.h"
#include "FunctionalAtom.h"
#include "OutAtom.h"



//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

int FunctionalReplacement::SortInfixTransformations::compare(std::wstring *infixTransformation1,
                                                             std::wstring *infixTransformation2) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    int allowedInputs1 = infixTransformation1.split(L",")->length;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    int allowedInputs2 = infixTransformation2.split(L",")->length;
    if (allowedInputs1 < allowedInputs2) { //infixTransformation1 preferred
        return -1;
    }
    if (allowedInputs1 > allowedInputs2) { //infixTransformation2 preferred
        return 1;
    } else {
        return 0;
    }
}

FunctionalReplacement::FunctionalReplacement(BuildState *state) : state(state) {
}

void FunctionalReplacement::processAcidReplacingFunctionalClassNomenclature(Element *finalSubOrRootInWord,
                                                                            Element *word) throw(ComponentGenerationException, StructureBuildingException) {
    Element *wordRule = OpsinTools::getParentWordRule(word);
    if (StringHelper::fromString<WordRule>(wordRule->getAttributeValue(XmlDeclarations::WORDRULE_ATR)) ==
        WordRule::acidReplacingFunctionalGroup) {
        Element *parentWordRule = word->getParent();
        if (parentWordRule->find(word) == 0) {
            for (int i = 1, l = parentWordRule->getChildCount(); i < l; i++) {
                Element *acidReplacingWord = parentWordRule->getChild(i);
                if (acidReplacingWord->getName() != XmlDeclarations::WORD_EL) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: problem with acidReplacingFunctionalGroup word rule");
                    throw std::exception();
                }
                std::wstring type = acidReplacingWord->getAttributeValue(XmlDeclarations::TYPE_ATR);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                if (type == WordType::full->toString()) {
                    //case where functionalTerm is substituted
                    //as words are processed from right to left in cases like phosphoric acid tri(ethylamide) this will be phosphoric acid ethylamide ethylamide ethylamide
                    processAcidReplacingFunctionalClassNomenclatureFullWord(finalSubOrRootInWord, acidReplacingWord);
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                else if (type == WordType::functionalTerm->toString()) {
                    processAcidReplacingFunctionalClassNomenclatureFunctionalWord(finalSubOrRootInWord,
                                                                                  acidReplacingWord);
                } else {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("OPSIN bug: problem with acidReplacingFunctionalGroup word rule");
                    throw std::exception();
                }
            }
        }
    }
}

bool FunctionalReplacement::processPrefixFunctionalReplacementNomenclature(std::vector<Element *> &groups,
                                                                           std::vector<Element *> &substituents) throw(StructureBuildingException, ComponentGenerationException) {
    int originalNumberOfGroups = groups.size();
    for (int i = originalNumberOfGroups - 1; i >= 0; i--) {
        Element *group = groups[i];
        std::wstring groupValue = group->getValue();
        PREFIX_REPLACEMENT_TYPE replacementType = nullptr;
        if (matchChalcogenReplacement->matcher(groupValue).matches()) {
            replacementType = PREFIX_REPLACEMENT_TYPE::chalcogen;
        } else if (XmlDeclarations::HALIDEORPSEUDOHALIDE_SUBTYPE_VAL ==
                   group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
            replacementType = PREFIX_REPLACEMENT_TYPE::halideOrPseudoHalide;
        } else if (XmlDeclarations::DEDICATEDFUNCTIONALREPLACEMENTPREFIX_SUBTYPE_VAL ==
                   group->getAttributeValue(XmlDeclarations::SUBTYPE_ATR)) {
            replacementType = PREFIX_REPLACEMENT_TYPE::dedicatedFunctionalReplacementPrefix;
        } else if (groupValue == L"hydrazono") {
            replacementType = PREFIX_REPLACEMENT_TYPE::hydrazono;
        } else if (groupValue == L"peroxy") {
            replacementType = PREFIX_REPLACEMENT_TYPE::peroxy;
        }
        if (replacementType != nullptr) {
            //need to check whether this is an instance of functional replacement by checking the substituent/root it is applying to
            Element *substituent = group->getParent();
            Element *nextSubOrBracket = OpsinTools::getNextSibling(substituent);
            if (nextSubOrBracket != nullptr && (nextSubOrBracket->getName() == XmlDeclarations::ROOT_EL ||
                                                nextSubOrBracket->getName() == XmlDeclarations::SUBSTITUENT_EL)) {
                Element *groupToBeModified = nextSubOrBracket->getFirstChildElement(XmlDeclarations::GROUP_EL);
                if (groupPrecededByElementThatBlocksPrefixReplacementInterpetation(groupToBeModified)) {
                    if (replacementType == PREFIX_REPLACEMENT_TYPE::dedicatedFunctionalReplacementPrefix) {
                        throw ComponentGenerationException(std::wstring(
                                L"dedicated Functional Replacement Prefix used in an inappropriate position :") +
                                                           groupValue);
                    }
                    continue; //not 2,2'-thiodipyran
                }
                Element *locantEl = nullptr; //null unless a locant that agrees with the multiplier is present
                Element *multiplierEl = nullptr;
                int numberOfAtomsToReplace = 1; //the number of atoms to be functionally replaced, modified by a multiplier e.g. dithio
                Element *possibleMultiplier = OpsinTools::getPreviousSibling(group);
                if (possibleMultiplier != nullptr) {
                    Element *possibleLocant;
                    if (possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                        numberOfAtomsToReplace = std::stoi(
                                possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
                        possibleLocant = OpsinTools::getPreviousSibling(possibleMultiplier);
                        multiplierEl = possibleMultiplier;
                    } else {
                        possibleLocant = possibleMultiplier;
                    }
                    if (possibleLocant != nullptr && possibleLocant->getName() == XmlDeclarations::LOCANT_EL &&
                        possibleLocant->getAttribute(XmlDeclarations::TYPE_ATR) == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                        int numberOfLocants = possibleLocant->getValue().split(L",")->length;
                        if (numberOfLocants == numberOfAtomsToReplace) { //locants and number of replacements agree
                            locantEl = possibleLocant;
                        } else if (numberOfAtomsToReplace > 1) { //doesn't look like prefix functional replacement
                            if (replacementType == PREFIX_REPLACEMENT_TYPE::dedicatedFunctionalReplacementPrefix) {
                                throw ComponentGenerationException(std::wstring(
                                        L"dedicated Functional Replacement Prefix used in an inappropriate position :") +
                                                                   groupValue);
                            }
                            continue;
                        }
                    }
                }

                int oxygenReplaced;
                if (replacementType == PREFIX_REPLACEMENT_TYPE::chalcogen) {
                    oxygenReplaced = performChalcogenFunctionalReplacement(groupToBeModified, locantEl,
                                                                           numberOfAtomsToReplace,
                                                                           group->getAttributeValue(
                                                                                   XmlDeclarations::VALUE_ATR));
                } else if (replacementType == PREFIX_REPLACEMENT_TYPE::peroxy) {
                    if (nextSubOrBracket->getName() == XmlDeclarations::SUBSTITUENT_EL) {
                        continue;
                    }
                    oxygenReplaced = performPeroxyFunctionalReplacement(groupToBeModified, locantEl,
                                                                        numberOfAtomsToReplace);
                } else if (replacementType == PREFIX_REPLACEMENT_TYPE::dedicatedFunctionalReplacementPrefix) {
                    if (groupToBeModified->getAttributeValue(XmlDeclarations::TYPE_ATR) !=
                        XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL &&
                        !(groupToBeModified->getValue() == L"form" && groupValue == L"imido")) {
                        throw ComponentGenerationException(std::wstring(
                                L"dedicated Functional Replacement Prefix used in an inappropriate position :") +
                                                           groupValue);
                    }
                    oxygenReplaced = performFunctionalReplacementOnAcid(groupToBeModified, locantEl,
                                                                        numberOfAtomsToReplace,
                                                                        group->getAttributeValue(
                                                                                XmlDeclarations::VALUE_ATR));
                    if (oxygenReplaced == 0) {
                        throw ComponentGenerationException(std::wstring(
                                L"dedicated Functional Replacement Prefix used in an inappropriate position :") +
                                                           groupValue);
                    }
                } else if (replacementType == PREFIX_REPLACEMENT_TYPE::hydrazono ||
                           replacementType == PREFIX_REPLACEMENT_TYPE::halideOrPseudoHalide) {
                    Fragment *acidFrag = groupToBeModified->getFrag();
                    if (groupToBeModified->getAttributeValue(XmlDeclarations::TYPE_ATR) !=
                        XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL ||
                        acidHasSufficientHydrogenForSubstitutionInterpretation(acidFrag, group->getFrag()->getOutAtom(
                                0)->getValency(), locantEl)) {
                        //hydrazono replacement only applies to non carboxylic acids e.g. hydrazonooxalic acid
                        //need to be careful to note that something like chlorophosphonic acid isn't functional replacement
                        continue;
                    }
                    oxygenReplaced = performFunctionalReplacementOnAcid(groupToBeModified, locantEl,
                                                                        numberOfAtomsToReplace,
                                                                        group->getAttributeValue(
                                                                                XmlDeclarations::VALUE_ATR));
                } else {
                    throw StructureBuildingException(L"OPSIN bug: Unexpected prefix replacement type");
                }
                if (oxygenReplaced > 0) {
                    state->fragManager->removeFragment(group->getFrag());
                    substituent->removeChild(group);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                    groups.remove(group);
                    std::vector<Element *> remainingChildren = substituent->getChildElements(); //there may be a locant that should be moved
                    for (int j = remainingChildren.size() - 1; j >= 0; j--) {
                        Element *child = substituent->getChild(j);
                        child->detach();
                        nextSubOrBracket->insertChild(child, 0);
                    }
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
                    substituents.remove(substituent);
                    substituent->detach();
                    if (oxygenReplaced > 1) {
                        multiplierEl->detach();
                    }
                }
            } else if (replacementType == PREFIX_REPLACEMENT_TYPE::dedicatedFunctionalReplacementPrefix) {
                throw ComponentGenerationException(
                        std::wstring(L"dedicated Functional Replacement Prefix used in an inappropriate position :") +
                        groupValue);
            }
        }
    }
    return groups.size() != originalNumberOfGroups;
}

bool FunctionalReplacement::groupPrecededByElementThatBlocksPrefixReplacementInterpetation(Element *groupToBeModified) {
    Element *previous = OpsinTools::getPreviousSibling(groupToBeModified);
    while (previous != nullptr && (previous->getName() == XmlDeclarations::SUBTRACTIVEPREFIX_EL ||
                                   (previous->getName() == XmlDeclarations::STEREOCHEMISTRY_EL &&
                                    previous->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
                                    XmlDeclarations::CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL))) {
        previous = OpsinTools::getPreviousSibling(previous);
    }
    return previous != nullptr;
}

void FunctionalReplacement::processInfixFunctionalReplacementNomenclature(std::vector<Element *> &suffixes,
                                                                          std::vector<Fragment *> &suffixFragments) throw(StructureBuildingException, ComponentGenerationException) {
    for (int i = 0; i < suffixes.size(); i++) {
        Element *suffix = suffixes[i];
        if (suffix->getAttribute(XmlDeclarations::INFIX_ATR) != nullptr) {
            Fragment *fragToApplyInfixTo = suffix->getFrag();
            Element *possibleAcidGroup = OpsinTools::getPreviousSiblingIgnoringCertainElements(suffix,
                                                                                               std::vector<std::wstring>{
                                                                                                       XmlDeclarations::MULTIPLIER_EL,
                                                                                                       XmlDeclarations::INFIX_EL,
                                                                                                       XmlDeclarations::SUFFIX_EL});
            if (possibleAcidGroup != nullptr && possibleAcidGroup->getName() == XmlDeclarations::GROUP_EL &&
                (possibleAcidGroup->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
                 XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL ||
                 possibleAcidGroup->getAttributeValue(XmlDeclarations::TYPE_ATR) ==
                 XmlDeclarations::CHALCOGENACIDSTEM_TYPE_VAL)) {
                fragToApplyInfixTo = possibleAcidGroup->getFrag();
            }
            if (fragToApplyInfixTo == nullptr) {
                throw ComponentGenerationException(std::wstring(
                        L"infix has erroneously been assigned to a suffix which does not correspond to a suffix fragment. suffix: ") +
                                                   suffix->getValue());
            }
            //e.g. =O:S,-O:S (which indicates replacing either a double or single bonded oxygen with S)
            //This is semicolon delimited for each infix
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            std::vector<std::wstring> infixTransformations = StringTools::arrayToList(
                    suffix->getAttributeValue(XmlDeclarations::INFIX_ATR).split(L";"));

            std::vector<Atom *> atomList = fragToApplyInfixTo->getAtomList();
            std::list<Atom *> singleBondedOxygen;
            std::list<Atom *> doubleBondedOxygen;
            populateTerminalSingleAndDoubleBondedOxygen(atomList, singleBondedOxygen, doubleBondedOxygen);
            int oxygenAvailable = singleBondedOxygen.size() + doubleBondedOxygen.size();

            /*
             * Modifies suffixes, suffixFragments, suffix and infixTransformations as appropriate
             */
            disambiguateMultipliedInfixMeaning(suffixes, suffixFragments, suffix, infixTransformations,
                                               oxygenAvailable);

            /*
             * Sort infixTransformations so more specific transformations are performed first
             * e.g. ethanthioimidic acid-->ethanimidthioic acid as imid can only apply to the double bonded oxygen
             */
//JAVA TO C++ CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the Java Comparator parameter produces a tri-state result:
//ORIGINAL LINE: java.util.Collections.sort(infixTransformations, new SortInfixTransformations());
            SortInfixTransformations tempVar{};
            std::sort(infixTransformations.begin(), infixTransformations.end(), tempVar.cmp);

            for (auto infixTransformation : infixTransformations) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                std::vector<std::wstring> transformationArray = infixTransformation.split(L":");
                if (transformationArray.size() != 2) {
                    throw StructureBuildingException(
                            std::wstring(L"Atom to be replaced and replacement not specified correctly in infix: ") +
                            infixTransformation);
                }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                std::vector<std::wstring> transformations = transformationArray[0].split(L",");
                std::wstring replacementSMILES = transformationArray[1];
                bool acceptDoubleBondedOxygen = false;
                bool acceptSingleBondedOxygen = false;
                bool nitrido = false;
                for (auto transformation : transformations) {
                    if (transformation.startsWith(L"=")) {
                        acceptDoubleBondedOxygen = true;
                    } else if (transformation.startsWith(L"-")) {
                        acceptSingleBondedOxygen = true;
                    } else if (transformation.startsWith(L"#")) {
                        nitrido = true;
                    } else {
                        throw StructureBuildingException(std::wstring(
                                L"Malformed infix transformation. Expected to start with either - or =. Transformation was: ") +
                                                         transformation);
                    }
                    if (transformation.length() < 2 || transformation.charAt(1) != L'O') {
                        throw StructureBuildingException(
                                L"Only replacement by oxygen is supported. Check infix defintions");
                    }
                }
                bool infixAssignmentAmbiguous = false;
                if ((acceptSingleBondedOxygen || nitrido) && !acceptDoubleBondedOxygen) {
                    if (singleBondedOxygen.empty()) {
                        throw StructureBuildingException(
                                std::wstring(L"Cannot find single bonded oxygen for infix with SMILES: ") +
                                replacementSMILES + std::wstring(L" to modify!"));
                    }
                    if (singleBondedOxygen.size() != 1) {
                        infixAssignmentAmbiguous = true;
                    }
                }
                if (!acceptSingleBondedOxygen && (acceptDoubleBondedOxygen | nitrido)) {
                    if (doubleBondedOxygen.empty()) {
                        throw StructureBuildingException(
                                std::wstring(L"Cannot find double bonded oxygen for infix with SMILES: ") +
                                replacementSMILES + std::wstring(L" to modify!"));
                    }
                    if (doubleBondedOxygen.size() != 1) {
                        infixAssignmentAmbiguous = true;
                    }
                }
                if (acceptSingleBondedOxygen && acceptDoubleBondedOxygen) {
                    if (oxygenAvailable == 0) {
                        throw StructureBuildingException(
                                std::wstring(L"Cannot find oxygen for infix with SMILES: ") + replacementSMILES +
                                std::wstring(L" to modify!"));
                    }
                    if (oxygenAvailable != 1) {
                        infixAssignmentAmbiguous = true;
                    }
                }

                Set < Atom * > *ambiguousElementAtoms = new LinkedHashSet<Atom *>();
                Atom *atomToUse = nullptr;
                if ((acceptDoubleBondedOxygen || nitrido) && doubleBondedOxygen.size() > 0) {
                    atomToUse = doubleBondedOxygen.pop_front();
                } else if (acceptSingleBondedOxygen && singleBondedOxygen.size() > 0) {
                    atomToUse = singleBondedOxygen.pop_front();
                } else {
                    throw StructureBuildingException(
                            std::wstring(L"Cannot find oxygen for infix with SMILES: ") + replacementSMILES +
                            std::wstring(L" to modify!")); //this would be a bug
                }
                Fragment *replacementFrag = state->fragManager->buildSMILES(replacementSMILES,
                                                                            XmlDeclarations::SUFFIX_TYPE_VAL,
                                                                            XmlDeclarations::NONE_LABELS_VAL);
                if (replacementFrag->getOutAtomCount() >
                    0) { //SMILES include an indication of the bond order the replacement fragment will have, this is not intended to be an outatom
                    replacementFrag->removeOutAtom(0);
                }
                Atom *atomThatWillReplaceOxygen = replacementFrag->getFirstAtom();
                if (replacementFrag->getAtomCount() == 1 && atomThatWillReplaceOxygen->getElement()::isChalcogen())
                {
                    atomThatWillReplaceOxygen->setCharge(atomToUse->getCharge());
                    atomThatWillReplaceOxygen->setProtonsExplicitlyAddedOrRemoved(
                            atomToUse->getProtonsExplicitlyAddedOrRemoved());
                }
                removeOrMoveObsoleteFunctionalAtoms(atomToUse, replacementFrag); //also will move charge if necessary
                moveObsoleteOutAtoms(atomToUse,
                                     replacementFrag); //if the replaced atom was an outatom the fragments outatom list need to be corrected
                if (nitrido) {
                    atomToUse->getFirstBond()->setOrder(3);
                    Atom *removedHydroxy = singleBondedOxygen.pop_front();
                    state->fragManager->removeAtomAndAssociatedBonds(removedHydroxy);
                    removeAssociatedFunctionalAtom(removedHydroxy);
                }
                state->fragManager->incorporateFragment(replacementFrag, atomToUse->getFrag());
                state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToUse,
                                                                                     atomThatWillReplaceOxygen);
                if (infixAssignmentAmbiguous) {
                    ambiguousElementAtoms->add(atomThatWillReplaceOxygen);
                    if (atomThatWillReplaceOxygen->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT) != nullptr) {
                        ambiguousElementAtoms->addAll(
                                atomThatWillReplaceOxygen->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT));
                    }
                }
                if (infixAssignmentAmbiguous) { //record what atoms could have been replaced. Often this ambiguity is resolved later e.g. S-methyl ethanthioate
                    for (auto a : doubleBondedOxygen) {
                        ambiguousElementAtoms->add(a);
                        if (a->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT) != nullptr) {
                            ambiguousElementAtoms->addAll(a->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT));
                        }
                    }
                    for (auto a : singleBondedOxygen) {
                        ambiguousElementAtoms->add(a);
                        if (a->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT) != nullptr) {
                            ambiguousElementAtoms->addAll(a->getProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT));
                        }
                    }
                    for (auto atom : ambiguousElementAtoms) {
                        atom->setProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT, ambiguousElementAtoms);
                    }
                }
            }
        }
    }
}

void FunctionalReplacement::processAcidReplacingFunctionalClassNomenclatureFullWord(Element *acidContainingRoot,
                                                                                    Element *acidReplacingWord) throw(ComponentGenerationException, StructureBuildingException) {
    std::wstring locant = acidReplacingWord->getAttributeValue(XmlDeclarations::LOCANT_ATR);
    Element *acidReplacingGroup = StructureBuildingMethods::findRightMostGroupInBracket(acidReplacingWord);
    if (acidReplacingGroup == nullptr) {
        throw ComponentGenerationException(
                L"OPSIN bug: acid replacing group not found where one was expected for acidReplacingFunctionalGroup wordRule");
    }
    std::wstring functionalGroupName = acidReplacingGroup->getValue();
    Fragment *acidReplacingFrag = acidReplacingGroup->getFrag();
    if (acidReplacingGroup->getParent()->getChildCount() != 1) {
        throw ComponentGenerationException(std::wstring(L"Unexpected qualifier to: ") + functionalGroupName);
    }

    Element *groupToBeModified = acidContainingRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
    std::vector<Atom *> oxygenAtoms = findFunctionalOxygenAtomsInApplicableSuffixes(groupToBeModified);
    if (oxygenAtoms.empty()) {
        oxygenAtoms = findFunctionalOxygenAtomsInGroup(groupToBeModified);
    }
    if (oxygenAtoms.empty()) {
        std::vector<Element *> conjunctiveSuffixElements = OpsinTools::getNextSiblingsOfType(groupToBeModified,
                                                                                             XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
        for (auto conjunctiveSuffixElement : conjunctiveSuffixElements) {
            oxygenAtoms.addAll(findFunctionalOxygenAtomsInGroup(conjunctiveSuffixElement));
        }
    }
    if (oxygenAtoms.size() < 1) {
        throw ComponentGenerationException(
                std::wstring(L"Insufficient oxygen to replace with ") + functionalGroupName + std::wstring(L"s in ") +
                acidContainingRoot->getFirstChildElement(XmlDeclarations::GROUP_EL)->getValue());
    }

    bool isAmide = functionalGroupName == L"amide" || functionalGroupName == L"amid";
    if (isAmide) {
        if (acidReplacingFrag->getAtomCount() != 1) {
            throw ComponentGenerationException(
                    std::wstring(L"OPSIN bug: ") + functionalGroupName + std::wstring(L" not found where expected"));
        }
        Atom *amideNitrogen = acidReplacingFrag->getFirstAtom();
        amideNitrogen->neutraliseCharge();
        amideNitrogen->clearLocants();
        acidReplacingFrag->addMappingToAtomLocantMap(L"N", amideNitrogen);
    }
    Atom *chosenOxygen = locant != L"" ? removeOxygenWithAppropriateLocant(oxygenAtoms, locant) : oxygenAtoms[0];
    state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(chosenOxygen,
                                                                         acidReplacingFrag->getFirstAtom());
    removeAssociatedFunctionalAtom(chosenOxygen);
}

void FunctionalReplacement::processAcidReplacingFunctionalClassNomenclatureFunctionalWord(Element *acidContainingRoot,
                                                                                          Element *functionalWord) throw(ComponentGenerationException, StructureBuildingException) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    if (functionalWord != nullptr &&
        functionalWord->getAttributeValue(XmlDeclarations::TYPE_ATR) == WordType::functionalTerm->toString()) {
        Element *functionalTerm = functionalWord->getFirstChildElement(XmlDeclarations::FUNCTIONALTERM_EL);
        if (functionalTerm == nullptr) {
            throw ComponentGenerationException(
                    L"OPSIN bug: functionalTerm word not found where one was expected for acidReplacingFunctionalGroup wordRule");
        }
        Element *acidReplacingGroup = functionalTerm->getFirstChildElement(XmlDeclarations::FUNCTIONALGROUP_EL);
        std::wstring functionalGroupName = acidReplacingGroup->getValue();
        Element *possibleLocantOrMultiplier = OpsinTools::getPreviousSibling(acidReplacingGroup);
        int numberOfAcidicHydroxysToReplace = 1;
        std::vector<std::wstring> locants;
        if (possibleLocantOrMultiplier != nullptr) {
            if (possibleLocantOrMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
                numberOfAcidicHydroxysToReplace = std::stoi(
                        possibleLocantOrMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
                possibleLocantOrMultiplier->detach();
                possibleLocantOrMultiplier = OpsinTools::getPreviousSibling(acidReplacingGroup);
            }
            if (possibleLocantOrMultiplier != nullptr) {
                if (possibleLocantOrMultiplier->getName() == XmlDeclarations::LOCANT_EL) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                    locants = StringTools::removeDashIfPresent(possibleLocantOrMultiplier->getValue()).split(L",");
                    possibleLocantOrMultiplier->detach();
                } else {
                    throw ComponentGenerationException(
                            L"Unexpected qualifier to acidReplacingFunctionalGroup functionalTerm");
                }
            }
        }
        if (functionalTerm->getChildCount() != 1) {
            throw ComponentGenerationException(L"Unexpected qualifier to acidReplacingFunctionalGroup functionalTerm");
        }

        Element *groupToBeModified = acidContainingRoot->getFirstChildElement(XmlDeclarations::GROUP_EL);
        std::vector<Atom *> oxygenAtoms = findFunctionalOxygenAtomsInApplicableSuffixes(groupToBeModified);
        if (oxygenAtoms.empty()) {
            oxygenAtoms = findFunctionalOxygenAtomsInGroup(groupToBeModified);
        }
        if (oxygenAtoms.empty()) {
            std::vector<Element *> conjunctiveSuffixElements = OpsinTools::getNextSiblingsOfType(groupToBeModified,
                                                                                                 XmlDeclarations::CONJUNCTIVESUFFIXGROUP_EL);
            for (auto conjunctiveSuffixElement : conjunctiveSuffixElements) {
                oxygenAtoms.addAll(findFunctionalOxygenAtomsInGroup(conjunctiveSuffixElement));
            }
        }
        if (numberOfAcidicHydroxysToReplace > oxygenAtoms.size()) {
            throw ComponentGenerationException(std::wstring(L"Insufficient oxygen to replace with nitrogen in ") +
                                               acidContainingRoot->getFirstChildElement(
                                                       XmlDeclarations::GROUP_EL)->getValue());
        }
        bool isAmide = functionalGroupName == L"amide" || functionalGroupName == L"amid";
        if (isAmide) {
            for (int i = 0; i < numberOfAcidicHydroxysToReplace; i++) {
                Atom *functionalOxygenToReplace =
                        locants.size() > 0 ? removeOxygenWithAppropriateLocant(oxygenAtoms, locants[i])
                                           : oxygenAtoms[i];
                removeAssociatedFunctionalAtom(functionalOxygenToReplace);
                functionalOxygenToReplace->setElement(ChemEl::N);
            }
        } else {
            std::wstring groupValue = acidReplacingGroup->getAttributeValue(XmlDeclarations::VALUE_ATR);
            std::wstring labelsValue = acidReplacingGroup->getAttributeValue(XmlDeclarations::LABELS_ATR);
            Fragment *acidReplacingFrag = state->fragManager->buildSMILES(groupValue, XmlDeclarations::SUFFIX_TYPE_VAL,
                                                                          labelsValue != L"" ? labelsValue
                                                                                             : XmlDeclarations::NONE_LABELS_VAL);
            Fragment *acidFragment = groupToBeModified->getFrag();
            if (acidFragment->hasLocant(L"2")) { //prefer numeric locants on group to those of replacing group
                for (auto atom : acidReplacingFrag->getAtomList()) {
                    atom->clearLocants();
                }
            }
            Atom *firstFunctionalOxygenToReplace =
                    locants.size() > 0 ? removeOxygenWithAppropriateLocant(oxygenAtoms, locants[0]) : oxygenAtoms[0];
            state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(firstFunctionalOxygenToReplace,
                                                                                 acidReplacingFrag->getFirstAtom());
            removeAssociatedFunctionalAtom(firstFunctionalOxygenToReplace);
            for (int i = 1; i < numberOfAcidicHydroxysToReplace; i++) {
                Fragment *clonedHydrazide = state->fragManager->copyAndRelabelFragment(acidReplacingFrag, i);
                Atom *functionalOxygenToReplace =
                        locants.size() > 0 ? removeOxygenWithAppropriateLocant(oxygenAtoms, locants[i])
                                           : oxygenAtoms[i];
                state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(functionalOxygenToReplace,
                                                                                     clonedHydrazide->getFirstAtom());
                state->fragManager->incorporateFragment(clonedHydrazide, functionalOxygenToReplace->getFrag());
                removeAssociatedFunctionalAtom(functionalOxygenToReplace);
            }
            state->fragManager->incorporateFragment(acidReplacingFrag, firstFunctionalOxygenToReplace->getFrag());
        }
    } else {
        throw ComponentGenerationException(L"amide word not found where expected, bug?");
    }
}

Atom *FunctionalReplacement::removeOxygenWithAppropriateLocant(std::vector<Atom *> &oxygenAtoms,
                                                               const std::wstring &locant) throw(ComponentGenerationException) {
    for (std::vector<Atom *>::const_iterator iterator = oxygenAtoms.begin();
         iterator != oxygenAtoms.end(); ++iterator) {
        Atom *atom = *iterator;
        if (atom->hasLocant(locant)) {
            (*iterator)->remove();
            return atom;
        }
    }
    //Look for the case whether the locant refers to the backbone
    for (std::vector<Atom *>::const_iterator iterator = oxygenAtoms.begin();
         iterator != oxygenAtoms.end(); ++iterator) {
        Atom *atom = *iterator;
        if (OpsinTools::depthFirstSearchForNonSuffixAtomWithLocant(atom, locant) != nullptr) {
            (*iterator)->remove();
            return atom;
        }
    }
    throw ComponentGenerationException(std::wstring(L"Failed to find acid group at locant: ") + locant);
}

bool FunctionalReplacement::acidHasSufficientHydrogenForSubstitutionInterpretation(Fragment *acidFrag,
                                                                                   int hydrogenRequiredForSubstitutionInterpretation,
                                                                                   Element *locantEl) {
    std::vector<Atom *> atomsThatWouldBeSubstituted;
    if (locantEl != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> possibleLocants = locantEl->getValue().split(L",");
        for (auto locant : possibleLocants) {
            Atom *atomToBeSubstituted = acidFrag->getAtomByLocant(locant);
            if (atomToBeSubstituted != nullptr) {
                atomsThatWouldBeSubstituted.push_back(atomToBeSubstituted);
            } else {
                atomsThatWouldBeSubstituted.clear();
                atomsThatWouldBeSubstituted.push_back(acidFrag->getDefaultInAtomOrFirstAtom());
                break;
            }
        }
    } else {
        atomsThatWouldBeSubstituted.push_back(acidFrag->getDefaultInAtomOrFirstAtom());
    }
    for (auto atom : atomsThatWouldBeSubstituted) {
        if (StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(atom) <
            hydrogenRequiredForSubstitutionInterpretation) {
            return false; //insufficient hydrogens for substitution interpretation
        }
    }
    return true;
}

int FunctionalReplacement::performChalcogenFunctionalReplacement(Element *groupToBeModified, Element *locantEl,
                                                                 int numberOfAtomsToReplace,
                                                                 const std::wstring &replacementSmiles) throw(StructureBuildingException) {
    std::vector<Atom *> oxygenAtoms = findOxygenAtomsInApplicableSuffixes(groupToBeModified);
    if (oxygenAtoms.empty()) {
        oxygenAtoms = findOxygenAtomsInGroup(groupToBeModified);
    }
    if (locantEl != nullptr) { //locants are used to indicate replacement on trivial groups
        std::vector<Atom *> oxygenWithAppropriateLocants = pickOxygensWithAppropriateLocants(locantEl, oxygenAtoms);
        if (oxygenWithAppropriateLocants.size() < numberOfAtomsToReplace) {
            numberOfAtomsToReplace = 1;
            //e.g. -1-thioureidomethyl
        } else {
            locantEl->detach();
            oxygenAtoms = oxygenWithAppropriateLocants;
        }
    }

    std::vector<Atom *> doubleBondedOxygen;
    std::vector<Atom *> singleBondedOxygen;
    std::vector<Atom *> ethericOxygen;
    for (auto oxygen : oxygenAtoms) {
        int incomingValency = oxygen->getIncomingValency();
        int bondCount = oxygen->getBondCount();
        if (bondCount == 1 && incomingValency == 2) {
            doubleBondedOxygen.push_back(oxygen);
        } else if (bondCount == 1 && incomingValency == 1) {
            singleBondedOxygen.push_back(oxygen);
        } else if (bondCount == 2 && incomingValency == 2) {
            ethericOxygen.push_back(oxygen);
        }
    }
    std::vector<Atom *> replaceableAtoms;
    replaceableAtoms.insert(replaceableAtoms.end(), doubleBondedOxygen.begin(), doubleBondedOxygen.end());
    replaceableAtoms.insert(replaceableAtoms.end(), singleBondedOxygen.begin(), singleBondedOxygen.end());
    replaceableAtoms.insert(replaceableAtoms.end(), ethericOxygen.begin(), ethericOxygen.end());
    int totalOxygen = replaceableAtoms.size();
    if (numberOfAtomsToReplace > 1) {
        if (totalOxygen < numberOfAtomsToReplace) {
            numberOfAtomsToReplace = 1;
        }
    }

    int atomsReplaced = 0;
    if (totalOxygen >= numberOfAtomsToReplace) { //check that there atleast as many oxygens as requested replacements
        bool prefixAssignmentAmbiguous = false;
        Set < Atom * > *ambiguousElementAtoms = new LinkedHashSet<Atom *>();
        if (totalOxygen != numberOfAtomsToReplace) {
            prefixAssignmentAmbiguous = true;
        }

        for (auto atomToReplace : replaceableAtoms) {
            if (atomsReplaced == numberOfAtomsToReplace) {
                ambiguousElementAtoms->add(atomToReplace);
                continue;
            } else {
                state->fragManager->replaceAtomWithSmiles(atomToReplace, replacementSmiles);
                if (prefixAssignmentAmbiguous) {
                    ambiguousElementAtoms->add(atomToReplace);
                }
            }
            atomsReplaced++;
        }

        if (prefixAssignmentAmbiguous) { //record what atoms could have been replaced. Often this ambiguity is resolved later e.g. S-methyl thioacetate
            for (auto atom : ambiguousElementAtoms) {
                atom->setProperty(Atom::AMBIGUOUS_ELEMENT_ASSIGNMENT, ambiguousElementAtoms);
            }
        }
    }
    return atomsReplaced;
}

int FunctionalReplacement::performPeroxyFunctionalReplacement(Element *groupToBeModified, Element *locantEl,
                                                              int numberOfAtomsToReplace) throw(StructureBuildingException) {
    std::vector<Atom *> oxygenAtoms = findFunctionalOxygenAtomsInApplicableSuffixes(groupToBeModified);
    if (oxygenAtoms.empty()) {
        oxygenAtoms = findEthericOxygenAtomsInGroup(groupToBeModified);
        oxygenAtoms.addAll(findFunctionalOxygenAtomsInGroup(groupToBeModified));
    }
    if (locantEl != nullptr) {
        std::vector<Atom *> oxygenWithAppropriateLocants = pickOxygensWithAppropriateLocants(locantEl, oxygenAtoms);
        if (oxygenWithAppropriateLocants.size() < numberOfAtomsToReplace) {
            numberOfAtomsToReplace = 1;
        } else {
            locantEl->detach();
            oxygenAtoms = oxygenWithAppropriateLocants;
        }
    }
    if (numberOfAtomsToReplace > 1 && oxygenAtoms.size() < numberOfAtomsToReplace) {
        numberOfAtomsToReplace = 1;
    }
    int atomsReplaced = 0;
    if (oxygenAtoms.size() >=
        numberOfAtomsToReplace) { //check that there atleast as many oxygens as requested replacements
        atomsReplaced = numberOfAtomsToReplace;
        for (int j = 0; j < numberOfAtomsToReplace; j++) {
            Atom *oxygenToReplace = oxygenAtoms[j];
            if (oxygenToReplace->getBondCount() == 2) { //etheric oxygen
                Fragment *newOxygen = state->fragManager->buildSMILES(L"O", XmlDeclarations::SUFFIX_TYPE_VAL,
                                                                      XmlDeclarations::NONE_LABELS_VAL);
                Bond *bondToRemove = oxygenToReplace->getFirstBond();
                Atom *atomToAttachTo = bondToRemove->getFromAtom() == oxygenToReplace ? bondToRemove->getToAtom()
                                                                                      : bondToRemove->getFromAtom();
                state->fragManager->createBond(atomToAttachTo, newOxygen->getFirstAtom(), 1);
                state->fragManager->createBond(newOxygen->getFirstAtom(), oxygenToReplace, 1);
                state->fragManager->removeBond(bondToRemove);
                state->fragManager->incorporateFragment(newOxygen, groupToBeModified->getFrag());
            } else {
                Fragment *replacementFrag = state->fragManager->buildSMILES(L"OO", XmlDeclarations::SUFFIX_TYPE_VAL,
                                                                            XmlDeclarations::NONE_LABELS_VAL);
                removeOrMoveObsoleteFunctionalAtoms(oxygenToReplace, replacementFrag);
                state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(oxygenToReplace,
                                                                                     replacementFrag->getFirstAtom());
                state->fragManager->incorporateFragment(replacementFrag, groupToBeModified->getFrag());
            }
        }
    }
    return atomsReplaced;
}

int FunctionalReplacement::performFunctionalReplacementOnAcid(Element *groupToBeModified, Element *locantEl,
                                                              int numberOfAtomsToReplace,
                                                              const std::wstring &replacementSmiles) throw(StructureBuildingException) {
    int outValency;
    if (boost::starts_with(replacementSmiles, L"-")) {
        outValency = 1;
    } else if (boost::starts_with(replacementSmiles, L"=")) {
        outValency = 2;
    } else if (boost::starts_with(replacementSmiles, L"#")) {
        outValency = 3;
    } else {
        throw StructureBuildingException(
                L"OPSIN bug: Unexpected valency on fragment for prefix functional replacement");
    }
    replacementSmiles = replacementSmiles.substr(1);
    std::vector<Atom *> oxygenAtoms = findOxygenAtomsInApplicableSuffixes(groupToBeModified);
    if (oxygenAtoms.empty()) {
        oxygenAtoms = findOxygenAtomsInGroup(groupToBeModified);
    }
    if (locantEl != nullptr) { //locants are used to indicate replacement on trivial groups
        std::vector<Atom *> oxygenWithAppropriateLocants = pickOxygensWithAppropriateLocants(locantEl, oxygenAtoms);
        std::vector<Atom *> singleBondedOxygen;
        std::vector<Atom *> terminalDoubleBondedOxygen;
        populateTerminalSingleAndDoubleBondedOxygen(oxygenWithAppropriateLocants, singleBondedOxygen,
                                                    terminalDoubleBondedOxygen);
        if (outValency == 1) {
            oxygenWithAppropriateLocants.removeAll(terminalDoubleBondedOxygen);
        } else if (outValency == 2) {
            oxygenWithAppropriateLocants.removeAll(singleBondedOxygen);
        }
        if (oxygenWithAppropriateLocants.size() < numberOfAtomsToReplace) {
            numberOfAtomsToReplace = 1;
            //e.g. -1-thioureidomethyl
        } else {
            locantEl->detach();
            oxygenAtoms = oxygenWithAppropriateLocants;
        }
    }
    std::vector<Atom *> singleBondedOxygen;
    std::vector<Atom *> terminalDoubleBondedOxygen;
    populateTerminalSingleAndDoubleBondedOxygen(oxygenAtoms, singleBondedOxygen, terminalDoubleBondedOxygen);
    if (outValency == 1) {
        oxygenAtoms.removeAll(terminalDoubleBondedOxygen);
    } else if (outValency == 2) {
        oxygenAtoms.removeAll(singleBondedOxygen);
        //favour bridging oxygen over double bonded oxygen c.f. imidodicarbonate
        oxygenAtoms.removeAll(terminalDoubleBondedOxygen);
        oxygenAtoms.insert(oxygenAtoms.end(), terminalDoubleBondedOxygen.begin(), terminalDoubleBondedOxygen.end());
    } else {
        if (singleBondedOxygen.empty() || terminalDoubleBondedOxygen.empty()) {
            throw StructureBuildingException(
                    L"Both a -OH and =O are required for nitrido prefix functional replacement");
        }
        oxygenAtoms.removeAll(singleBondedOxygen);
    }
    if (numberOfAtomsToReplace > 1 && oxygenAtoms.size() < numberOfAtomsToReplace) {
        numberOfAtomsToReplace = 1;
    }

    int atomsReplaced = 0;
    if (oxygenAtoms.size() >=
        numberOfAtomsToReplace) { //check that there atleast as many oxygens as requested replacements
        for (auto atomToReplace : oxygenAtoms) {
            if (atomsReplaced == numberOfAtomsToReplace) {
                continue;
            } else {
                Fragment *replacementFrag = state->fragManager->buildSMILES(replacementSmiles,
                                                                            atomToReplace->getFrag()->getTokenEl(),
                                                                            XmlDeclarations::NONE_LABELS_VAL);
                if (outValency == 3) { //special case for nitrido
                    atomToReplace->getFirstBond()->setOrder(3);
                    Atom *removedHydroxy = singleBondedOxygen.erase(singleBondedOxygen.begin());
                    state->fragManager->removeAtomAndAssociatedBonds(removedHydroxy);
                    removeAssociatedFunctionalAtom(removedHydroxy);
                }
                state->fragManager->replaceAtomWithAnotherAtomPreservingConnectivity(atomToReplace,
                                                                                     replacementFrag->getFirstAtom());
                if (outValency == 1) {
                    removeOrMoveObsoleteFunctionalAtoms(atomToReplace, replacementFrag);
                }
                moveObsoleteOutAtoms(atomToReplace, replacementFrag);
                state->fragManager->incorporateFragment(replacementFrag, atomToReplace->getFrag());
            }
            atomsReplaced++;
        }
    }
    return atomsReplaced;
}

void FunctionalReplacement::disambiguateMultipliedInfixMeaning(std::vector<Element *> &suffixes,
                                                               std::vector<Fragment *> &suffixFragments,
                                                               Element *suffix,
                                                               std::vector<std::wstring> &infixTransformations,
                                                               int oxygenAvailable) throw(ComponentGenerationException, StructureBuildingException) {
    Element *possibleInfix = OpsinTools::getPreviousSibling(suffix);
    if (possibleInfix->getName() == XmlDeclarations::INFIX_EL) { //the infix is only left when there was ambiguity
        Element *possibleMultiplier = OpsinTools::getPreviousSibling(possibleInfix);
        if (possibleMultiplier->getName() == XmlDeclarations::MULTIPLIER_EL) {
            int multiplierValue = std::stoi(possibleMultiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
            if (infixTransformations.size() + multiplierValue - 1 <=
                oxygenAvailable) { //multiplier means multiply the infix e.g. butandithiate
                for (int j = 1; j < multiplierValue; j++) {
                    infixTransformations.push_back(0, infixTransformations[0]);
                }
            } else {
                Element *possibleLocant = OpsinTools::getPreviousSibling(possibleMultiplier);
                std::vector<std::wstring> locants;
                if (possibleLocant->getName() == XmlDeclarations::LOCANT_EL) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
                    locants = possibleLocant->getValue().split(L",");
                }
                if (locants.size() > 0) {
                    if (locants.size() != multiplierValue) {
                        throw ComponentGenerationException(
                                L"Multiplier/locant disagreement when multiplying infixed suffix");
                    }
                    Attribute tempVar(XmlDeclarations::LOCANT_ATR, locants[0]);
                    suffix->addAttribute(&tempVar);
                }
                Attribute tempVar2(XmlDeclarations::MULTIPLIED_ATR, L"multiplied");
                suffix->addAttribute(&tempVar2);
                for (int j = 1;
                     j < multiplierValue; j++) { //multiplier means multiply the infixed suffix e.g. butandithione
                    Element *newSuffix = suffix->copy();
                    Fragment *newSuffixFrag = state->fragManager->copyFragment(suffix->getFrag());
                    newSuffix->setFrag(newSuffixFrag);
                    suffixFragments.push_back(newSuffixFrag);
                    OpsinTools::insertAfter(suffix, newSuffix);
                    suffixes.push_back(newSuffix);
                    if (locants.size() > 0) { //assign locants if available
                        newSuffix->getAttribute(XmlDeclarations::LOCANT_ATR)->setValue(locants[j]);
                    }
                }
                if (locants.size() > 0) {
                    possibleLocant->detach();
                }
            }
            possibleMultiplier->detach();
            possibleInfix->detach();
        } else {
            throw ComponentGenerationException(L"Multiplier expected in front of ambiguous infix");
        }
    }
}

void FunctionalReplacement::removeOrMoveObsoleteFunctionalAtoms(Atom *atomToBeReplaced, Fragment *replacementFrag) {
    std::vector<Atom *> replacementAtomList = replacementFrag->getAtomList();
    Fragment *origFrag = atomToBeReplaced->getFrag();
    for (int i = origFrag->getFunctionalAtomCount() - 1; i >= 0; i--) {
        FunctionalAtom *functionalAtom = origFrag->getFunctionalAtom(i);
        if (atomToBeReplaced->equals(functionalAtom->getAtom())) {
            atomToBeReplaced->getFrag()->removeFunctionalAtom(i);
            Atom *terminalAtomOfReplacementFrag = replacementAtomList[replacementAtomList.size() - 1];
            if ((terminalAtomOfReplacementFrag->getIncomingValency() == 1 || replacementAtomList.size() == 1) &&
                terminalAtomOfReplacementFrag->getElement()
                ::isChalcogen())
            {
                replacementFrag->addFunctionalAtom(terminalAtomOfReplacementFrag);
                terminalAtomOfReplacementFrag->setCharge(atomToBeReplaced->getCharge());
                terminalAtomOfReplacementFrag->setProtonsExplicitlyAddedOrRemoved(
                        atomToBeReplaced->getProtonsExplicitlyAddedOrRemoved());
            }
            atomToBeReplaced->neutraliseCharge();
        }
    }
}

void FunctionalReplacement::moveObsoleteOutAtoms(Atom *atomToBeReplaced, Fragment *replacementFrag) {
    if (atomToBeReplaced->getOutValency() >
        0) { //this is not known to occur in well formed IUPAC names but would occur in thioxy (as a suffix)
        std::vector<Atom *> replacementAtomList = replacementFrag->getAtomList();
        Fragment *origFrag = atomToBeReplaced->getFrag();
        for (int i = origFrag->getOutAtomCount() - 1; i >= 0; i--) {
            OutAtom *outAtom = origFrag->getOutAtom(i);
            if (atomToBeReplaced->equals(outAtom->getAtom())) {
                atomToBeReplaced->getFrag()->removeOutAtom(i);
                Atom *terminalAtomOfReplacementFrag = replacementAtomList[replacementAtomList.size() - 1];
                replacementFrag->addOutAtom(terminalAtomOfReplacementFrag, outAtom->getValency(),
                                            outAtom->isSetExplicitly());
            }
        }
    }
}

void
FunctionalReplacement::removeAssociatedFunctionalAtom(Atom *atomWithFunctionalAtom) throw(StructureBuildingException) {
    Fragment *frag = atomWithFunctionalAtom->getFrag();
    for (int i = frag->getFunctionalAtomCount() - 1; i >= 0; i--) {
        FunctionalAtom *functionalAtom = frag->getFunctionalAtom(i);
        if (atomWithFunctionalAtom->equals(functionalAtom->getAtom())) {
            atomWithFunctionalAtom->getFrag()->removeFunctionalAtom(i);
            return;
        }
    }
    throw StructureBuildingException(L"OPSIN bug: Unable to find associated functionalAtom");
}

std::vector<Atom *>
FunctionalReplacement::pickOxygensWithAppropriateLocants(Element *locantEl, std::vector<Atom *> &oxygenAtoms) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> possibleLocants = locantEl->getValue().split(L",");

    bool pLocantSpecialCase = (possibleLocants.size() == 1 && possibleLocants[0] == L"P");
    std::vector<Atom *> oxygenWithAppropriateLocants;
    for (auto atom : oxygenAtoms) {
        std::vector<std::wstring> atomlocants = atom->getLocants();
        if (atomlocants.size() > 0) {
            for (auto locantVal : possibleLocants) {
                if (std::find(atomlocants.begin(), atomlocants.end(), locantVal) != atomlocants.end()) {
                    oxygenWithAppropriateLocants.push_back(atom);
                    break;
                }
            }
        } else if (pLocantSpecialCase) {
            for (auto neighbour : atom->getAtomNeighbours()) {
                if (neighbour->getElement() == ChemEl::P) {
                    oxygenWithAppropriateLocants.push_back(atom);
                    break;
                }
            }
        } else {
            Atom *atomWithNumericLocant = OpsinTools::depthFirstSearchForAtomWithNumericLocant(atom);
            if (atomWithNumericLocant != nullptr) {
                std::vector<std::wstring> atomWithNumericLocantLocants = atomWithNumericLocant->getLocants();
                for (auto locantVal : possibleLocants) {
                    if (std::find(atomWithNumericLocantLocants.begin(), atomWithNumericLocantLocants.end(),
                                  locantVal) != atomWithNumericLocantLocants.end()) {
                        oxygenWithAppropriateLocants.push_back(atom);
                        break;
                    }
                }
            }
        }
    }
    return oxygenWithAppropriateLocants;
}

std::vector<Atom *> FunctionalReplacement::findFunctionalOxygenAtomsInApplicableSuffixes(Element *groupToBeModified) {
    std::vector<Element *> suffixElements = OpsinTools::getNextSiblingsOfType(groupToBeModified,
                                                                              XmlDeclarations::SUFFIX_EL);
    std::vector<Atom *> oxygenAtoms;
    for (auto suffix : suffixElements) {
        Fragment *suffixFrag = suffix->getFrag();
        if (suffixFrag != nullptr) { //null for non carboxylic acids
            for (int i = 0, l = suffixFrag->getFunctionalAtomCount(); i < l; i++) {
                Atom *a = suffixFrag->getFunctionalAtom(i)->getAtom();
                if (a->getElement() == ChemEl::O) {
                    oxygenAtoms.push_back(a);
                }
            }
        }
    }
    return oxygenAtoms;
}

std::vector<Atom *> FunctionalReplacement::findFunctionalOxygenAtomsInGroup(Element *groupToBeModified) {
    std::vector<Atom *> oxygenAtoms;
    Fragment *frag = groupToBeModified->getFrag();
    for (int i = 0, l = frag->getFunctionalAtomCount(); i < l; i++) {
        Atom *a = frag->getFunctionalAtom(i)->getAtom();
        if (a->getElement() == ChemEl::O) {
            oxygenAtoms.push_back(a);
        }
    }
    return oxygenAtoms;
}

std::vector<Atom *> FunctionalReplacement::findEthericOxygenAtomsInGroup(Element *groupToBeModified) {
    std::vector<Atom *> oxygenAtoms;
    std::vector<Atom *> atomList = groupToBeModified->getFrag()->getAtomList();
    for (auto a : atomList) {
        if (a->getElement() == ChemEl::O && a->getBondCount() == 2 && a->getCharge() == 0 &&
            a->getIncomingValency() == 2) {
            oxygenAtoms.push_back(a);
        }
    }
    return oxygenAtoms;
}

std::vector<Atom *> FunctionalReplacement::findOxygenAtomsInApplicableSuffixes(Element *groupToBeModified) {
    std::vector<Element *> suffixElements = OpsinTools::getNextSiblingsOfType(groupToBeModified,
                                                                              XmlDeclarations::SUFFIX_EL);
    std::vector<Atom *> oxygenAtoms;
    for (auto suffix : suffixElements) {
        Fragment *suffixFrag = suffix->getFrag();
        if (suffixFrag != nullptr) { //null for non carboxylic acids
            if (suffixFrag->getFunctionalAtomCount() > 0 ||
                groupToBeModified->getAttributeValue(XmlDeclarations::TYPE_ATR) == XmlDeclarations::ACIDSTEM_TYPE_VAL ||
                suffix->getAttributeValue(XmlDeclarations::VALUE_ATR).equals(L"aldehyde")) {
                std::vector<Atom *> atomList = suffixFrag->getAtomList();
                for (auto a : atomList) {
                    if (a->getElement() == ChemEl::O) {
                        oxygenAtoms.push_back(a);
                    }
                }
            }
        }
    }
    return oxygenAtoms;
}

std::vector<Atom *> FunctionalReplacement::findOxygenAtomsInGroup(Element *groupToBeModified) {
    std::vector<Atom *> oxygenAtoms;
    std::vector<Atom *> atomList = groupToBeModified->getFrag()->getAtomList();
    for (auto a : atomList) {
        if (a->getElement() == ChemEl::O) {
            oxygenAtoms.push_back(a);
        }
    }
    return oxygenAtoms;
}

void FunctionalReplacement::populateTerminalSingleAndDoubleBondedOxygen(std::vector<Atom *> &atomList,
                                                                        std::vector<Atom *> &singleBondedOxygen,
                                                                        std::vector<Atom *> &doubleBondedOxygen) throw(StructureBuildingException) {
    for (auto a : atomList) {
        if (a->getElement() == ChemEl::O) { //find terminal oxygens
            if (a->getBondCount() == 1) {
                int incomingValency = a->getIncomingValency();
                if (incomingValency == 2) {
                    doubleBondedOxygen.push_back(a);
                } else if (incomingValency == 1) {
                    singleBondedOxygen.push_back(a);
                } else {
                    throw StructureBuildingException(
                            std::wstring(L"Unexpected bond order to oxygen; excepted 1 or 2 found: ") +
                            std::to_wstring(incomingValency));
                }

            }
        }
    }
}
