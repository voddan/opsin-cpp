#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class NameToStructure;
class NameToStructureConfig;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;



class NomenclatureIntegrationTest {
private:
    static NameToStructure *n2s;

public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
    static void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
    static void cleanUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRadicals() throws java.io.IOException
    virtual void testRadicals() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAcetals() throws java.io.IOException
    virtual void testAcetals() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlcoholEsters() throws java.io.IOException
    virtual void testAlcoholEsters() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrates() throws java.io.IOException
    virtual void testCarbohydrates() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testChargeBalancing() throws java.io.IOException
    virtual void testChargeBalancing() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConjunctiveNomenclature() throws java.io.IOException
    virtual void testConjunctiveNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCyclicSuffixes() throws java.io.IOException
    virtual void testCyclicSuffixes() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testEpoxyLike() throws java.io.IOException
    virtual void testEpoxyLike() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFunctionalReplacement() throws java.io.IOException
    virtual void testFunctionalReplacement() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsotopes() throws java.io.IOException
    virtual void testIsotopes() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAdditiveNomenclature() throws java.io.IOException
    virtual void testAdditiveNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultiplicativeNomenclature() throws java.io.IOException
    virtual void testMultiplicativeNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOmittedSpaces() throws java.io.IOException
    virtual void testOmittedSpaces() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFunctionalClassNomenclature() throws java.io.IOException
    virtual void testFunctionalClassNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFusedRingNomenclature() throws java.io.IOException
    virtual void testFusedRingNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInorganicNomenclature() throws java.io.IOException
    virtual void testInorganicNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIonNomenclature() throws java.io.IOException
    virtual void testIonNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSpiroNomenclature() throws java.io.IOException
    virtual void testSpiroNomenclature() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganoMetallics() throws java.io.IOException
    virtual void testOrganoMetallics() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImplicitBracketting() throws java.io.IOException
    virtual void testImplicitBracketting() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistry() throws java.io.IOException
    virtual void testStereochemistry() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMiscellany() throws java.io.IOException
    virtual void testMiscellany() throw(IOException);

private:
    void checkNamesAgainstInChIs(const std::wstring &file, NameToStructureConfig *n2sConfig) throw(IOException);
};

