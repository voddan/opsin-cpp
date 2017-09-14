#pragma once



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;


class FusedRingNumbererFunctionsTest {


public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetOppositeDirection()
    virtual void testGetOppositeDirection();
    //
    //	@Test
    //	public void testDetermineAbsoluteDirectionFromPreviousDirection3Membered(){
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 0, 3));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 0, 3));
    //
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 1, 3));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 1, 3));
    //
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -1, 3));
    //		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -1, 3));
    //	}
    //
    //	@Test
    //	public void testDetermineAbsoluteDirectionFromPreviousDirection4Membered(){
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 0, 4));
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, 0, 4));
    //		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, 0, 4));
    //
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 2, 4));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, 2, 4));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, 2, 4));
    //
    //		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -2, 4));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, -2, 4));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, -2, 4));
    //	}
    //
    //	@Test
    //	public void testDetermineAbsoluteDirectionFromPreviousDirection5Membered(){
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 0, 5));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 0, 5));
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, 0, 5));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 0, 5));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 0, 5));
    //		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, 0, 5));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 0, 5));
    //
    //		//assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 1, 5));
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 1, 5));
    //		//assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, 1, 5));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 1, 5));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 1, 5));
    //		//assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, 1, 5));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 1, 5));
    //
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 2, 5));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 2, 5));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, 2, 5));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 2, 5));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 2, 5));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, 2, 5));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 2, 5));
    //
    //		//assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 3, 5));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 3, 5));
    //		//assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, 3, 5));
    //		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 3, 5));
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 3, 5));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, 3, 5));
    //		//assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 3, 5));
    ////
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -1, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -1, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, -1, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, -1, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -1, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, -1, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, -1, 5));
    ////
    ////		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -2, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -2, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, -2, 5));
    ////		assertEquals(.FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, -2, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -2, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, -2, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, -2, 5));
    ////
    ////		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -3, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -3, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(2, -3, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, -3, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -3, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-2, -3, 5));
    ////		assertEquals(,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, -3, 5));
    //	}
    //
    //	@Test
    //	public void testDetermineAbsoluteDirectionFromPreviousDirection8Membered(){
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 0, 8));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 0, 8));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 0, 8));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 0, 8));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 0, 8));
    //
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 1, 8));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 1, 8));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 1, 8));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 1, 8));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 1, 8));
    //
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 1, 8));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 1, 8));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 1, 8));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 1, 8));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 1, 8));
    //
    //		assertEquals(2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 2, 8));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 2, 8));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 2, 8));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 2, 8));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 2, 8));
    //
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, 3, 8));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, 3, 8));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, 3, 8));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 3, 8));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, 3, 8));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, 3, 8));
    //
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -1, 8));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -1, 8));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, -1, 8));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -1, 8));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, -1, 8));
    //
    //		assertEquals(-2,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -2, 8));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -2, 8));
    //		assertEquals(1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, -2, 8));
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -2, 8));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, -2, 8));
    //
    //		assertEquals(-3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(0, -3, 8));
    //		assertEquals(-1,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(1, -3, 8));
    //		assertEquals(0,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(3, -3, 8));
    //		assertEquals(4,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-1, -3, 8));
    //		assertEquals(3,FusedRingNumberer.determineAbsoluteDirectionFromPreviousDirection(-3, -3, 8));
    //	}
};

