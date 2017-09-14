#pragma once

#include <string>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ResourceGetter;
class SMILESFragmentBuilder;





//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

class VerifyFragmentsTest {

private:
    static ResourceGetter *resourceGetter;
    static SMILESFragmentBuilder *sBuilder;

public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
    static void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
    static void cleanUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void verifySMILES() throws Exception
    virtual void verifySMILES() throw(std::exception);

private:
    void verifySmilesInTokenList(XMLStreamReader *reader) throw(XMLStreamException);
};

