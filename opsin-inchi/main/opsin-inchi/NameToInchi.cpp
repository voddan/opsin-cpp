#include "NameToInchi.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructure.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/OpsinResult.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/Atom.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/Bond.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/AtomParity.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/BondStereo.h"


using org::apache::log4j::Logger;
using BondStereoValue = uk::ac::cam::ch::wwmm::opsin::BondStereo::BondStereoValue;
using net::sf::jniinchi::INCHI_BOND_TYPE;
using net::sf::jniinchi::INCHI_OPTION;
using net::sf::jniinchi::INCHI_PARITY;
using net::sf::jniinchi::INCHI_RET;
using net::sf::jniinchi::JniInchiAtom;
using net::sf::jniinchi::JniInchiBond;
using net::sf::jniinchi::JniInchiException;
using net::sf::jniinchi::JniInchiInput;
using net::sf::jniinchi::JniInchiOutput;
using net::sf::jniinchi::JniInchiOutputKey;
using net::sf::jniinchi::JniInchiStereo0D;
using net::sf::jniinchi::JniInchiWrapper;

NameToInchi::NameToInchi() {
    n2s = NameToStructure::getInstance();
}

std::wstring NameToInchi::parseToInchi(const std::wstring &name) {
    OpsinResult *result = n2s->parseChemicalName(name);
    return convertResultToInChI(result);
}

std::wstring NameToInchi::parseToStdInchi(const std::wstring &name) {
    OpsinResult *result = n2s->parseChemicalName(name);
    return convertResultToStdInChI(result);
}

std::wstring NameToInchi::parseToStdInchiKey(const std::wstring &name) {
    OpsinResult *result = n2s->parseChemicalName(name);
    return convertResultToStdInChIKey(result);
}

std::wstring NameToInchi::convertResultToInChI(OpsinResult *result) {
    return convertResultToInChI(result, false);
}

std::wstring NameToInchi::convertResultToStdInChI(OpsinResult *result) {
    return convertResultToInChI(result, true);
}

std::wstring NameToInchi::convertResultToStdInChIKey(OpsinResult *result) {
    std::wstring stdInchi = convertResultToInChI(result, true);
    if (stdInchi != L"") {
        try {
            JniInchiOutputKey *key = JniInchiWrapper::getInchiKey(stdInchi);
            return key->getKey();
        }
        catch (const std::exception &e) {
            if (LOG->isDebugEnabled()) {
                LOG->debug(e.what(), e);
            }
            return L"";
        }
    }
    return L"";
}

std::wstring NameToInchi::convertResultToInChI(OpsinResult *result, bool produceStdInChI) {
    if (result->getStructure() != nullptr) {
        std::wstring inchi = L"";
        try {
            inchi = opsinFragmentToInchi(result->getStructure(), produceStdInChI);
        }
        catch (const std::exception &e) {
            if (LOG->isDebugEnabled()) {
                LOG->debug(e.what(), e);
            }
            return L"";
        }
        if (inchi == L"") {
            //inchi generation failed
            return L"";
        }
        if (LOG->isDebugEnabled()) {
            LOG->debug(inchi);
        }
        return inchi;
    }
    return L"";
}

std::wstring NameToInchi::opsinFragmentToInchi(Fragment *frag, bool produceStdInChI) throw(JniInchiException) {
    std::unordered_map<int, JniInchiAtom *> opsinIdAtomMap;
    JniInchiInput *input;
    std::vector<INCHI_OPTION *> options;
    options.push_back(INCHI_OPTION::AuxNone);
    if (!produceStdInChI) {
        options.push_back(INCHI_OPTION::FixedH);
    }
    input = new JniInchiInput(options);

    std::vector<Atom *> atomList = frag->getAtomList();
    // Generate atoms
    for (auto atom : atomList) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        JniInchiAtom
        tempVar(0.0, 0.0, 0.0, atom->getElement()
        ::toString());
        JniInchiAtom *jAtom = input->addAtom(&tempVar);
        jAtom->setCharge(atom->getCharge());
        boost::optional<int> isotope = atom->getIsotope();
        if (isotope) {
            jAtom->setIsotopicMass(isotope);
        }
        jAtom->setImplicitH(0);
        opsinIdAtomMap[atom->getID()] = jAtom;
    }
    Set < Bond * > *bondList = frag->getBondSet();
    for (auto bond : bondList) {
        JniInchiBond tempVar2(opsinIdAtomMap[bond->getFrom()], opsinIdAtomMap[bond->getTo()],
                              INCHI_BOND_TYPE::getValue(bond->getOrder()));
        input->addBond(&tempVar2);
    }

    for (auto atom : atomList) { //add atomParities
        AtomParity *atomParity = atom->getAtomParity();
        if (atomParity != nullptr) {
            std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
            std::vector<int> atomRefs4AsInt(4);
            for (int i = 0; i < atomRefs4.size(); i++) {
                atomRefs4AsInt[i] = atomRefs4[i]->getID();
            }
            INCHI_PARITY *parity = INCHI_PARITY::UNKNOWN;
            if (atomParity->getParity() > 0) {
                parity = INCHI_PARITY::EVEN;
            } else if (atomParity->getParity() < 0) {
                parity = INCHI_PARITY::ODD;
            }
            input->addStereo0D(JniInchiStereo0D::createNewTetrahedralStereo0D(opsinIdAtomMap[atom->getID()],
                                                                              opsinIdAtomMap[atomRefs4AsInt[0]],
                                                                              opsinIdAtomMap[atomRefs4AsInt[1]],
                                                                              opsinIdAtomMap[atomRefs4AsInt[2]],
                                                                              opsinIdAtomMap[atomRefs4AsInt[3]],
                                                                              parity));
        }
    }

    for (auto bond : bondList) { //add bondStereos
        BondStereo *bondStereo = bond->getBondStereo();
        if (bondStereo != nullptr) {
            std::vector<Atom *> atomRefs4 = bondStereo->getAtomRefs4();
            std::vector<int> atomRefs4Ids(4);
            for (int i = 0; i < atomRefs4.size(); i++) {
                atomRefs4Ids[i] = atomRefs4[i]->getID();
            }
            if (BondStereoValue::CIS::equals(bondStereo->getBondStereoValue())) {
                input->addStereo0D(JniInchiStereo0D::createNewDoublebondStereo0D(opsinIdAtomMap[atomRefs4Ids[0]],
                                                                                 opsinIdAtomMap[atomRefs4Ids[1]],
                                                                                 opsinIdAtomMap[atomRefs4Ids[2]],
                                                                                 opsinIdAtomMap[atomRefs4Ids[3]],
                                                                                 INCHI_PARITY::ODD));
            } else if (BondStereoValue::TRANS::equals(bondStereo->getBondStereoValue())) {
                input->addStereo0D(JniInchiStereo0D::createNewDoublebondStereo0D(opsinIdAtomMap[atomRefs4Ids[0]],
                                                                                 opsinIdAtomMap[atomRefs4Ids[1]],
                                                                                 opsinIdAtomMap[atomRefs4Ids[2]],
                                                                                 opsinIdAtomMap[atomRefs4Ids[3]],
                                                                                 INCHI_PARITY::EVEN));
            }
        }
    }
    JniInchiOutput *output = JniInchiWrapper::getInchi(input);
    if (output == nullptr) {
        return L"";
    }
    INCHI_RET *ret = output->getReturnStatus();
    if (LOG->isDebugEnabled()) {
        LOG->debug(std::wstring(L"Inchi generation status: ") + ret);
        if (!INCHI_RET::OKAY::equals(ret)) {
            LOG->debug(output->getMessage());
        }
    }
    if (!INCHI_RET::OKAY::equals(ret) && !INCHI_RET::WARNING::equals(ret)) {
        return L"";
    }
    return output->getInchi();
}
