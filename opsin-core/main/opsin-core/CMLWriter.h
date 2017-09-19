#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Fragment;
class Atom;
class AtomParity;
class Bond;
class BondStereo;


using com::ctc::wstx::stax::WstxOutputFactory;

class CMLWriter {
    /// <summary>
    /// CML Elements/Attributes/NameSpace
    /// </summary>
public:
    static const std::wstring CML_NAMESPACE;

private:
    static XMLOutputFactory *const factory = new WstxOutputFactory();
private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static CMLWriter::StaticConstructor staticConstructor;


    /// <summary>
    ///The XML writer </summary>
    XMLStreamWriter *const writer;

    /// <summary>
    /// Creates a CML writer for the given fragment </summary>
    /// <param name="writer">
    ///  </param>
public:
    virtual ~CMLWriter() {
        delete writer;
    }

    CMLWriter(XMLStreamWriter *writer);

    static std::wstring generateCml(Fragment *structure, const std::wstring &chemicalName);

    static std::wstring generateIndentedCml(Fragment *structure, const std::wstring &chemicalName);

private:
    static std::wstring generateCml(Fragment *structure, const std::wstring &chemicalName, bool indent);

public:
    virtual void writeCmlStart();

    virtual void writeCmlEnd();

    virtual void writeMolecule(Fragment *structure, const std::wstring &chemicalName, int id) throw(XMLStreamException);

private:
    void writeAtom(Atom *atom) throw(XMLStreamException);

    void writeAtomParity(AtomParity *atomParity) throw(XMLStreamException);

    void writeBond(Bond *bond) throw(XMLStreamException);

    void writeBondStereo(BondStereo *bondStereo) throw(XMLStreamException);

    void writeAtomRefs4(std::vector<Atom *> &atomRefs4) throw(XMLStreamException);

};

