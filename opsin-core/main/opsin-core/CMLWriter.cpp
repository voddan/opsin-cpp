#include "CMLWriter.h"
#include "Fragment.h"
#include "IndentingXMLStreamWriter.h"
#include "Atom.h"
#include "ChemEl.h"
#include "AtomParity.h"
#include "Bond.h"
#include "BondStereo.h"


using com::ctc::wstx::api::WstxOutputProperties;
using com::ctc::wstx::stax::WstxOutputFactory;
const std::wstring CMLWriter::CML_NAMESPACE = L"http://www.xml-cml.org/schema";

CMLWriter::StaticConstructor::StaticConstructor() {
    factory->setProperty(WstxOutputProperties::P_OUTPUT_ESCAPE_CR, false);
}

CMLWriter::StaticConstructor CMLWriter::staticConstructor;

CMLWriter::CMLWriter(XMLStreamWriter *writer) : writer(writer) {
}

std::wstring CMLWriter::generateCml(Fragment *structure, const std::wstring &chemicalName) {
    return generateCml(structure, chemicalName, false);
}

std::wstring CMLWriter::generateIndentedCml(Fragment *structure, const std::wstring &chemicalName) {
    return generateCml(structure, chemicalName, true);
}

std::wstring CMLWriter::generateCml(Fragment *structure, const std::wstring &chemicalName, bool indent) {
    ByteArrayOutputStream *out = new ByteArrayOutputStream();
    try {
        XMLStreamWriter *xmlWriter = factory->createXMLStreamWriter(out, L"UTF-8");
        if (indent) {
            xmlWriter = new IndentingXMLStreamWriter(xmlWriter, 2);
        }
        CMLWriter *cmlWriter = new CMLWriter(xmlWriter);
        cmlWriter->writeCmlStart();
        cmlWriter->writeMolecule(structure, chemicalName, 1);
        cmlWriter->writeCmlEnd();
        delete xmlWriter;
    }
    catch (const XMLStreamException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
        throw std::exception();
    }
    try {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        return out->toString(L"UTF-8");
    }
    catch (const UnsupportedEncodingException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("JVM doesn't support UTF-8...but it should do!");
        throw std::exception();
    }
}

void CMLWriter::writeCmlStart() {
    try {
        writer->writeStartElement(L"cml");
        writer->writeDefaultNamespace(CML_NAMESPACE);
        writer->writeAttribute(L"convention", L"conventions:molecular");
        writer->writeNamespace(L"conventions", L"http://www.xml-cml.org/convention/");
        writer->writeNamespace(L"cmlDict", L"http://www.xml-cml.org/dictionary/cml/");
        writer->writeNamespace(L"nameDict", L"http://www.xml-cml.org/dictionary/cml/name/");
    }
    catch (const XMLStreamException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
        throw std::exception();
    }
}

void CMLWriter::writeCmlEnd() {
    try {
        writer->writeEndElement();
        writer->flush();
    }
    catch (const XMLStreamException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
        throw std::exception();
    }
}

void CMLWriter::writeMolecule(Fragment *structure, const std::wstring &chemicalName, int id) throw(XMLStreamException) {
    writer->writeStartElement(L"molecule");
    writer->writeAttribute(L"id", std::wstring(L"m") + std::to_wstring(id));

    writer->writeStartElement(L"name");
    writer->writeAttribute(L"dictRef", L"nameDict:unknown");
    writer->writeCharacters(chemicalName);
    writer->writeEndElement();

    if (structure != nullptr) {
        writer->writeStartElement(L"atomArray");
        for (auto atom : structure->getAtomList()) {
            writeAtom(atom);
        }
        writer->writeEndElement();

        writer->writeStartElement(L"bondArray");
        for (auto bond : structure->getBondSet()) {
            writeBond(bond);
        }
        writer->writeEndElement();
    }

    writer->writeEndElement();
}

void CMLWriter::writeAtom(Atom *atom) throw(XMLStreamException) {
    writer->writeStartElement(L"atom");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    writer->writeAttribute(L"id", std::wstring(L"a") + Integer::toString(atom->getID()));
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    writer->writeAttribute(L"elementType", atom->getElement()
    ::toString());
    if (atom->getCharge() != 0) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        writer->writeAttribute(L"formalCharge", Integer::toString(atom->getCharge()));
    }
    if (atom->getIsotope()) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        writer->writeAttribute(L"isotopeNumber", Integer::toString(atom->getIsotope()));
    }
    if (atom->getElement() != ChemEl::H) {
        int hydrogenCount = 0;
        std::vector<Atom *> neighbours = atom->getAtomNeighbours();
        for (auto neighbour : neighbours) {
            if (neighbour->getElement() == ChemEl::H) {
                hydrogenCount++;
            }
        }
        if (hydrogenCount == 0) { //prevent adding of implicit hydrogen
            writer->writeAttribute(L"hydrogenCount", L"0");
        }
    }
    AtomParity *atomParity = atom->getAtomParity();
    if (atomParity != nullptr) {
        writeAtomParity(atomParity);
    }
    for (auto locant : atom->getLocants()) {
        writer->writeStartElement(L"label");
        writer->writeAttribute(L"value", locant);
        writer->writeAttribute(L"dictRef", L"cmlDict:locant");
        writer->writeEndElement();
    }
    writer->writeEndElement();
}

void CMLWriter::writeAtomParity(AtomParity *atomParity) throw(XMLStreamException) {
    writer->writeStartElement(L"atomParity");
    writeAtomRefs4(atomParity->getAtomRefs4());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    writer->writeCharacters(Integer::toString(atomParity->getParity()));
    writer->writeEndElement();
}

void CMLWriter::writeBond(Bond *bond) throw(XMLStreamException) {
    writer->writeStartElement(L"bond");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    writer->writeAttribute(L"id", std::wstring(L"a") + Integer::toString(bond->getFrom()) + std::wstring(L"_a") +
                                  Integer::toString(bond->getTo()));
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    writer->writeAttribute(L"atomRefs2", std::wstring(L"a") + Integer::toString(bond->getFrom()) + std::wstring(L" a") +
                                         Integer::toString(bond->getTo()));
    switch (bond->getOrder()) {
        case 1:
            writer->writeAttribute(L"order", L"S");
            break;
        case 2:
            writer->writeAttribute(L"order", L"D");
            break;
        case 3:
            writer->writeAttribute(L"order", L"T");
            break;
        default:
            writer->writeAttribute(L"order", L"unknown");
            break;
    }
    BondStereo *bondStereo = bond->getBondStereo();
    if (bondStereo != nullptr) {
        writeBondStereo(bondStereo);
    }
    writer->writeEndElement();
}

void CMLWriter::writeBondStereo(BondStereo *bondStereo) throw(XMLStreamException) {
    writer->writeStartElement(L"bondStereo");
    writeAtomRefs4(bondStereo->getAtomRefs4());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    writer->writeCharacters(bondStereo->getBondStereoValue()
    ::toString());
    writer->writeEndElement();
}

void CMLWriter::writeAtomRefs4(std::vector<Atom *> &atomRefs4) throw(XMLStreamException) {
    StringBuilder *atomRefsSb = new StringBuilder();
    for (int i = 0; i < atomRefs4.size() - 1; i++) {
        atomRefsSb->append(L'a');
        atomRefsSb->append(atomRefs4[i]->getID());
        atomRefsSb->append(L' ');
    }
    atomRefsSb->append(L'a');
    atomRefsSb->append(atomRefs4[atomRefs4.size() - 1]->getID());
    writer->writeAttribute(L"atomRefs4", atomRefsSb->toString());
}
