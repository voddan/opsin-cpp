#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ComponentGenerationException;
class Element;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.assertEquals;

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


class ComponentGeneration_MiscTest {

public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testRejectSingleComponentSaltComponent() throws ComponentGenerationException
    virtual void testRejectSingleComponentSaltComponent() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNumericallyMultipliedSaltComponent() throws ComponentGenerationException
    virtual void testNumericallyMultipliedSaltComponent() throw(ComponentGenerationException);

private:
    void processComponents(Element *parse) throw(ComponentGenerationException);

};

