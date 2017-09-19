#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ComponentGenerationException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


class ComponentGeneration_AmbiguitiesAndIrregularitiesTest {

public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCorrectlyTokenisedAlkane()
    virtual void testCorrectlyTokenisedAlkane();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCorrectlyTokenisedAlkane2()
    virtual void testCorrectlyTokenisedAlkane2();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCorrectlyTokenisedAlkane3()
    virtual void testCorrectlyTokenisedAlkane3();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testMisTokenisedAlkane() throws ComponentGenerationException
    virtual void testMisTokenisedAlkane() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantsIndicatingTokenizationIsCorrect()
    virtual void testLocantsIndicatingTokenizationIsCorrect();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testLocantsIndicatingTokenizationIsIncorrect() throws ComponentGenerationException
    virtual void testLocantsIndicatingTokenizationIsIncorrect() throw(ComponentGenerationException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testTetraphenShouldBeTetra_Phen1() throws ComponentGenerationException
    virtual void testTetraphenShouldBeTetra_Phen1() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testTetraphenShouldBeTetra_Phen2() throws ComponentGenerationException
    virtual void testTetraphenShouldBeTetra_Phen2() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetraphenShouldBeTetraphen1()
    virtual void testTetraphenShouldBeTetraphen1();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetraphenShouldBeTetraphen2()
    virtual void testTetraphenShouldBeTetraphen2();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetraphenShouldBeTetraphen3()
    virtual void testTetraphenShouldBeTetraphen3();

    //TODO multiplier oxy tests, fusion vs Hw locants,  and handleGroupIrregularities tests
};

