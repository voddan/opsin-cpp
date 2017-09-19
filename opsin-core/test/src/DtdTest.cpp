#include "DtdTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ResourceGetter.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/OpsinTools.h"


//						import static org.junit.Assert.assertTrue;
using org::junit::Test;
using org::xml::sax::ErrorHandler;
using org::xml::sax::SAXException;
using org::xml::sax::SAXParseException;
const std::wstring DtdTest::RESOURCE_LOCATION = L"uk/ac/cam/ch/wwmm/opsin/resources/";

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTokenFiles() throws Exception
void DtdTest::testTokenFiles() throw(std::exception) {
    XMLStreamReader *reader = resourceGetter->getXMLStreamReader(L"index.xml");
    while (reader->hasNext()) {
        if (reader->next() == XMLStreamConstants::START_ELEMENT && reader->getLocalName().equals(L"tokenFile")) {
            validate(getUriForFile(reader->getElementText()));
        }
        reader++;
    }
    delete reader;
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRegexes() throws Exception
void DtdTest::testRegexes() throw(std::exception) {
    validate(getUriForFile(L"regexes.xml"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRegexTokens() throws Exception
void DtdTest::testRegexTokens() throw(std::exception) {
    validate(getUriForFile(L"regexTokens.xml"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuffixApplicability() throws Exception
void DtdTest::testSuffixApplicability() throw(std::exception) {
    validate(getUriForFile(L"suffixApplicability.xml"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuffixRules() throws Exception
void DtdTest::testSuffixRules() throw(std::exception) {
    validate(getUriForFile(L"suffixRules.xml"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWordRules() throws Exception
void DtdTest::testWordRules() throw(std::exception) {
    validate(getUriForFile(L"wordRules.xml"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTokenFilesValueValidity() throws Exception
void DtdTest::testTokenFilesValueValidity() throw(std::exception) {
    XMLStreamReader *indexReader = resourceGetter->getXMLStreamReader(L"index.xml");
    while (indexReader->hasNext()) {
        if (indexReader->next() == XMLStreamConstants::START_ELEMENT &&
            indexReader->getLocalName().equals(L"tokenFile")) {
            XMLStreamReader *tokenReader = resourceGetter->getXMLStreamReader(indexReader->getElementText());
            while (tokenReader->hasNext()) {
                if (tokenReader->next() == XMLStreamConstants::START_ELEMENT) {
                    std::wstring tagName = tokenReader->getLocalName();
                    if (tagName == L"tokenLists") {
                        while (tokenReader->hasNext()) {
                            switch (tokenReader->next()) {
                                case XMLStreamConstants::START_ELEMENT:
                                    if (tokenReader->getLocalName().equals(L"tokenList")) {
                                        validateTokenList(tokenReader);
                                    }
                                    break;
                            }
                            tokenReader++;
                        }
                    } else if (tagName == L"tokenList") {
                        validateTokenList(tokenReader);
                    }
                }
                tokenReader++;
            }
        }
        indexReader++;
    }
    delete indexReader;
}

void DtdTest::validateTokenList(XMLStreamReader *reader) throw(XMLStreamException) {
    Set <std::wstring> *terms = std::unordered_set<std::wstring>();
    while (reader->hasNext()) {
        switch (reader->next()) {
            case XMLStreamConstants::START_ELEMENT:
                if (reader->getLocalName().equals(L"token")) {
                    std::wstring tokenString = reader->getElementText();
                    assertTrue(tokenString + std::wstring(L" occurred more than once in a tokenList"),
                               !terms->contains(tokenString));
                    terms->add(tokenString);
                    std::vector<wchar_t> characters = tokenString.toCharArray();
                    for (auto c : characters) {
                        assertTrue(std::wstring(L"Non ascii character found in token: ") + tokenString +
                                   OpsinTools::NEWLINE + std::wstring(L"An ASCII replacement should be used!"),
                                   static_cast<int>(c) < 128);
                        assertTrue(
                                std::wstring(L"Capital letter found in token: ") + tokenString + OpsinTools::NEWLINE +
                                std::wstring(L"Only lower case letters should be used!"), !(c >= L'A' && c <= L'Z'));
                    }
                }
                break;
            case XMLStreamConstants::END_ELEMENT:
                if (reader->getLocalName().equals(L"tokenList")) {
                    return;
                }
                break;
        }
        reader++;
    }
}

void DtdTest::validate(URI *uri) throw(std::exception) {
    std::wcout << std::wstring(L"Validating:") << uri << std::endl;
    DocumentBuilderFactory *f = DocumentBuilderFactory::newInstance();
    f->setValidating(true);
    DocumentBuilder *b = f->newDocumentBuilder();
    MyErrorHandler *h = new MyErrorHandler();
    b->setErrorHandler(h);
    try {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
        b->parse(uri->toString());
    }
    catch (const SAXException &e) {
        if (h->error != nullptr) {
            std::wcout << h->error << std::endl;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            AssertionError *ae = new AssertionError(std::wstring(L"XML Validation error: ") + uri->toString());
            ae->initCause(h->error);
            throw ae;
        }
    }
}

void DtdTest::MyErrorHandler::error(SAXParseException *exception) throw(SAXException) {
    this->error_Renamed = exception;
    throw SAXException(L"Error");
}

void DtdTest::MyErrorHandler::fatalError(SAXParseException *exception) throw(SAXException) {
    this->error_Renamed = exception;
    throw SAXException(L"Error");
}

void DtdTest::MyErrorHandler::warning(SAXParseException *exception) throw(SAXException) {
    this->error_Renamed = exception;
    throw SAXException(L"Error");
}

URI *DtdTest::getUriForFile(const std::wstring &fileName) throw(URISyntaxException) {
    ClassLoader *l = getClass().getClassLoader();
    URL *url = l->getResource(RESOURCE_LOCATION + fileName);
    if (url == nullptr) {
        return nullptr;
    }
    return url->toURI();
}
