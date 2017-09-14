#include "IndentingXMLStreamWriter.h"
#include "OpsinTools.h"
#include "StringTools.h"


using org::codehaus::stax2::util::StreamWriterDelegate;

IndentingXMLStreamWriter::IndentingXMLStreamWriter(XMLStreamWriter *writer, int indentSize)
        : org::codehaus::stax2::util::StreamWriterDelegate(writer), indentSize(indentSize) {
}

void IndentingXMLStreamWriter::writeStartElement(const std::wstring &arg0) throw(XMLStreamException) {
    if (!atStartOfNewline) {
        StreamWriterDelegate::writeCharacters(OpsinTools::NEWLINE);
    }
    StreamWriterDelegate::writeCharacters(StringTools::multiplyString(L" ", depth * indentSize));
    StreamWriterDelegate::writeStartElement(arg0);
    atStartOfNewline = false;
    depth++;
}

void IndentingXMLStreamWriter::writeEndElement() throw(XMLStreamException) {
    depth--;
    if (atStartOfNewline) {
        StreamWriterDelegate::writeCharacters(StringTools::multiplyString(L" ", depth * indentSize));
    }
    StreamWriterDelegate::writeEndElement();
    StreamWriterDelegate::writeCharacters(OpsinTools::NEWLINE);
    atStartOfNewline = true;
}

void IndentingXMLStreamWriter::writeCharacters(const std::wstring &arg0) throw(XMLStreamException) {
    StreamWriterDelegate::writeCharacters(arg0);
    atStartOfNewline = false;
}
