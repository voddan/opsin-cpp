#include "AmbiguityDetectionTest.h"
#include "opsin-core/NameToStructure.h"
#include "opsin-core/OpsinResult.h"


//						import static org.junit.Assert.assertEquals;
using org::apache::commons::io::IOUtils;
using org::junit::AfterClass;
using org::junit::BeforeClass;
using org::junit::Test;
NameToStructure *AmbiguityDetectionTest::n2s;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
void AmbiguityDetectionTest::setUp() {
    n2s = NameToStructure::getInstance();
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
void AmbiguityDetectionTest::cleanUp() {
    n2s = nullptr;
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNamesThatShouldBeDetectedAsAmbiguous() throws java.io.IOException
void AmbiguityDetectionTest::testNamesThatShouldBeDetectedAsAmbiguous() throw(IOException) {
    checkNames(L"ambiguous.txt", true);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUnAmbiguousCounterExamples() throws java.io.IOException
void AmbiguityDetectionTest::testUnAmbiguousCounterExamples() throw(IOException) {
    checkNames(L"unambiguous.txt", false);
}

void AmbiguityDetectionTest::checkNames(const std::wstring &file, bool isAmbiguous) throw(IOException) {
    InputStreamReader tempVar(this->getClass().getResourceAsStream(file));
    BufferedReader *input = new BufferedReader(&tempVar);
    try {
        std::wstring line = L"";
        while ((line = input->readLine()) != L"") {
            if (boost::starts_with(line, L"//")) {
                continue;
            }
            OpsinResult *result = n2s->parseChemicalName(line);
            assertEquals(line + std::wstring(L" gave unexpected result"), isAmbiguous,
                         result->nameAppearsToBeAmbiguous());
        }
    }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
    finally
    {
        IOUtils::closeQuietly(input);
    }
}
