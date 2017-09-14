#pragma once

#include <string>
#include <vector>
#include <algorithm>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class FragmentManager;
class StructureBuildingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;




class SMILESWriterTest {

private:
    FragmentManager *fm;

public:
    virtual ~SMILESWriterTest() {
        delete fm;
    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setup()
    virtual void setup();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip1() throws StructureBuildingException
    virtual void testRoundTrip1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip2() throws StructureBuildingException
    virtual void testRoundTrip2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip3() throws StructureBuildingException
    virtual void testRoundTrip3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip4() throws StructureBuildingException
    virtual void testRoundTrip4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip5() throws StructureBuildingException
    virtual void testRoundTrip5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip6() throws StructureBuildingException
    virtual void testRoundTrip6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip7() throws StructureBuildingException
    virtual void testRoundTrip7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip8() throws StructureBuildingException
    virtual void testRoundTrip8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip9() throws StructureBuildingException
    virtual void testRoundTrip9() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip10() throws StructureBuildingException
    virtual void testRoundTrip10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip11() throws StructureBuildingException
    virtual void testRoundTrip11() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip12() throws StructureBuildingException
    virtual void testRoundTrip12() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic1() throws StructureBuildingException
    virtual void testOrganic1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic2() throws StructureBuildingException
    virtual void testOrganic2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic3() throws StructureBuildingException
    virtual void testOrganic3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic4() throws StructureBuildingException
    virtual void testOrganic4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic5() throws StructureBuildingException
    virtual void testOrganic5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic6() throws StructureBuildingException
    virtual void testOrganic6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic7() throws StructureBuildingException
    virtual void testOrganic7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic8() throws StructureBuildingException
    virtual void testOrganic8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic9() throws StructureBuildingException
    virtual void testOrganic9() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic10() throws StructureBuildingException
    virtual void testOrganic10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged1() throws StructureBuildingException
    virtual void testCharged1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged2() throws StructureBuildingException
    virtual void testCharged2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged3() throws StructureBuildingException
    virtual void testCharged3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged4() throws StructureBuildingException
    virtual void testCharged4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsotope() throws StructureBuildingException
    virtual void testIsotope() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRGroup1() throws StructureBuildingException
    virtual void testRGroup1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRGroup2() throws StructureBuildingException
    virtual void testRGroup2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRingOpeningsGreaterThan10() throws StructureBuildingException
    virtual void testRingOpeningsGreaterThan10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen1() throws StructureBuildingException
    virtual void testHydrogenNotBondedToAnyNonHydrogen1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen2() throws StructureBuildingException
    virtual void testHydrogenNotBondedToAnyNonHydrogen2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen3() throws StructureBuildingException
    virtual void testHydrogenNotBondedToAnyNonHydrogen3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen4() throws StructureBuildingException
    virtual void testHydrogenNotBondedToAnyNonHydrogen4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality1() throws StructureBuildingException
    virtual void testTetrahedralChirality1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality2() throws StructureBuildingException
    virtual void testTetrahedralChirality2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality3() throws StructureBuildingException
    virtual void testTetrahedralChirality3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality4() throws StructureBuildingException
    virtual void testTetrahedralChirality4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality5() throws StructureBuildingException
    virtual void testTetrahedralChirality5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality6() throws StructureBuildingException
    virtual void testTetrahedralChirality6() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality7() throws StructureBuildingException
    virtual void testTetrahedralChirality7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport1() throws StructureBuildingException
    virtual void testDoubleBondSupport1() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport2() throws StructureBuildingException
    virtual void testDoubleBondSupport2() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport3() throws StructureBuildingException
    virtual void testDoubleBondSupport3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport4() throws StructureBuildingException
    virtual void testDoubleBondSupport4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport5() throws StructureBuildingException
    virtual void testDoubleBondSupport5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport6() throws StructureBuildingException
    virtual void testDoubleBondSupport6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport7() throws StructureBuildingException
    virtual void testDoubleBondSupport7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport8() throws StructureBuildingException
    virtual void testDoubleBondSupport8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelling1() throws StructureBuildingException
    virtual void testLabelling1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelling2() throws StructureBuildingException
    virtual void testLabelling2() throw(StructureBuildingException);
};

