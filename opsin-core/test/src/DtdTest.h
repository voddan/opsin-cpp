#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ResourceGetter;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.assertTrue;



using org::xml::sax::ErrorHandler;
using org::xml::sax::SAXException;
using org::xml::sax::SAXParseException;

class DtdTest {
private:
    static const std::wstring RESOURCE_LOCATION;
    ResourceGetter *const resourceGetter = new ResourceGetter(RESOURCE_LOCATION);

public:
    virtual ~DtdTest() {
        delete resourceGetter;
    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTokenFiles() throws Exception
    virtual void testTokenFiles() throw(std::exception);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRegexes() throws Exception
    virtual void testRegexes() throw(std::exception);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRegexTokens() throws Exception
    virtual void testRegexTokens() throw(std::exception);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuffixApplicability() throws Exception
    virtual void testSuffixApplicability() throw(std::exception);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuffixRules() throws Exception
    virtual void testSuffixRules() throw(std::exception);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWordRules() throws Exception
    virtual void testWordRules() throw(std::exception);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTokenFilesValueValidity() throws Exception
    virtual void testTokenFilesValueValidity() throw(std::exception);

private:
    void validateTokenList(XMLStreamReader *reader) throw(XMLStreamException);

public:
    static void validate(URI *uri) throw(std::exception);

public:
    class MyErrorHandler : public ErrorHandler {

    private:
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
        SAXParseException *error_Renamed;

    public:
        virtual ~MyErrorHandler() {
            delete error;
        }

        virtual void error(SAXParseException *exception) throw(SAXException);

        virtual void fatalError(SAXParseException *exception) throw(SAXException);

        virtual void warning(SAXParseException *exception) throw(SAXException);

    };

private:
    URI *getUriForFile(const std::wstring &fileName) throw(URISyntaxException);

};

