#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class NameToInchi;
class StructureBuildingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;



class InchiOutputTest {

private:
    static NameToInchi *n2i;
public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
    static void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
    static void cleanUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStaticToInChI() throws StructureBuildingException
    virtual void testStaticToInChI() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStaticToStdInChI() throws StructureBuildingException
    virtual void testStaticToStdInChI() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStaticToStdInChIKey() throws StructureBuildingException
    virtual void testStaticToStdInChIKey() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToInChI()
    virtual void testParseToInChI();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToStdInChI()
    virtual void testParseToStdInChI();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToStdInChIKey()
    virtual void testParseToStdInChIKey();
};

