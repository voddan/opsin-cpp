#pragma once

#include <string>


using org::codehaus::stax2::util::StreamWriterDelegate;

/// <summary>
/// This only overrides the commands actually used by the CmlWriter i.e. it isn't general
/// </summary>
class IndentingXMLStreamWriter : public StreamWriterDelegate {

private:
    const int indentSize;
    int depth = 0;
    bool atStartOfNewline = false;

public:
    IndentingXMLStreamWriter(XMLStreamWriter *writer, int indentSize);

    virtual void writeStartElement(const std::wstring &arg0) throw(XMLStreamException) override;

    virtual void writeEndElement() throw(XMLStreamException) override;

    virtual void writeCharacters(const std::wstring &arg0) throw(XMLStreamException) override;

};

