#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class StructureBuildingException;


using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.mockito.Mockito.mock;

class StructureBuildingMethodsTest {

public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedPrimeNotSpecialCase()
    virtual void bracketedPrimeNotSpecialCase();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedPrimeSpecialCase1()
    virtual void bracketedPrimeSpecialCase1();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedPrimeSpecialCase2()
    virtual void bracketedPrimeSpecialCase2();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void notPhosphoSubstitution() throws StructureBuildingException
    virtual void notPhosphoSubstitution() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphoUnlocantedSubstitution() throws StructureBuildingException
    virtual void phosphoUnlocantedSubstitution() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphoLocantedSubstitution() throws StructureBuildingException
    virtual void phosphoLocantedSubstitution() throw(StructureBuildingException);
};

