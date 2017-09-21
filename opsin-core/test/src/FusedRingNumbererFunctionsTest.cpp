#include "FusedRingNumbererFunctionsTest.h"
#include "opsin-core/FusedRingNumberer.h"


//						import static org.junit.Assert.*;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetOppositeDirection()
void FusedRingNumbererFunctionsTest::testGetOppositeDirection() {
    assertEquals(4, FusedRingNumberer::getOppositeDirection(0));
    assertEquals(-3, FusedRingNumberer::getOppositeDirection(1));
    assertEquals(-2, FusedRingNumberer::getOppositeDirection(2));
    assertEquals(-1, FusedRingNumberer::getOppositeDirection(3));
    assertEquals(0, FusedRingNumberer::getOppositeDirection(4));
    assertEquals(0, FusedRingNumberer::getOppositeDirection(-4));
    assertEquals(1, FusedRingNumberer::getOppositeDirection(-3));
    assertEquals(2, FusedRingNumberer::getOppositeDirection(-2));
    assertEquals(3, FusedRingNumberer::getOppositeDirection(-1));
}
