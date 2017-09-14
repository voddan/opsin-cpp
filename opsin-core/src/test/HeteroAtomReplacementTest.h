#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class FragmentManager;
class Atom;
class StructureBuildingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.mockito.Mockito.mock;


class HeteroAtomReplacementTest {

public:
    FragmentManager *fragManager;
    Atom *a;

    virtual ~HeteroAtomReplacementTest() {
        delete fragManager;
        delete a;
    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
    virtual void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thia() throws StructureBuildingException
    virtual void thia() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thionia() throws StructureBuildingException
    virtual void thionia() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sulfanylia() throws StructureBuildingException
    virtual void sulfanylia() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sulfanida() throws StructureBuildingException
    virtual void sulfanida() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sulfanuida() throws StructureBuildingException
    virtual void sulfanuida() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void replaceNeutralWithCharged() throws StructureBuildingException
    virtual void replaceNeutralWithCharged() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void replaceChargedWithEquallyCharged() throws StructureBuildingException
    virtual void replaceChargedWithEquallyCharged() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void replaceChargedWithUnEquallyCharged() throws StructureBuildingException
    virtual void replaceChargedWithUnEquallyCharged() throw(StructureBuildingException);
};

