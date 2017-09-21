#include "RadixTrieTest.h"
#include "opsin-core/OpsinRadixTrie.h"


//						import static org.junit.Assert.*;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimpleAddSimpleGet()
void RadixTrieTest::testSimpleAddSimpleGet() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L"benzene");
    std::vector<int> matches = trie->findMatches(L"benzene", 0);
    assertNotNull(matches);
    assertEquals(1, matches.size());
    assertEquals(7, matches[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimpleAddFindPrefix()
void RadixTrieTest::testSimpleAddFindPrefix() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L"phenyl");
    std::vector<int> matches = trie->findMatches(L"phenylbenzene", 0);
    assertNotNull(matches);
    assertEquals(1, matches.size());
    assertEquals(6, matches[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddWithBranchFindPrefix()
void RadixTrieTest::testAddWithBranchFindPrefix() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L"pyridinyl");
    trie->addToken(L"phenyl");
    std::vector<int> matches = trie->findMatches(L"phenylbenzene", 0);
    assertNotNull(matches);
    assertEquals(1, matches.size());
    assertEquals(6, matches[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testZeroLengthToken()
void RadixTrieTest::testZeroLengthToken() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L""); //e.g. end of substituent
    std::vector<int> matches = trie->findMatches(L"phenylbenzene", 0);
    assertNotNull(matches);
    assertEquals(1, matches.size());
    assertEquals(0, matches[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleHits()
void RadixTrieTest::testMultipleHits() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L"methyl");
    trie->addToken(L"methylidene");
    std::vector<int> matches = trie->findMatches(L"methylidene", 0);
    assertNotNull(matches);
    assertEquals(2, matches.size());
    assertEquals(6, matches[0]);
    assertEquals(11, matches[1]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleHits2()
void RadixTrieTest::testMultipleHits2() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L"abcdef");
    trie->addToken(L"a");
    trie->addToken(L"");
    trie->addToken(L"acd");
    trie->addToken(L"ab");
    trie->addToken(L"abcf");
    std::vector<int> matches = trie->findMatches(L"abc", 0);
    assertNotNull(matches);
    assertEquals(3, matches.size());
    assertEquals(0, matches[0]);
    assertEquals(1, matches[1]);
    assertEquals(2, matches[2]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testReverseMatching()
void RadixTrieTest::testReverseMatching() {
    OpsinRadixTrie *trie = new OpsinRadixTrie();
    trie->addToken(L"enedilyhte");
    trie->addToken(L"lyhte");
    trie->addToken(L"");
    trie->addToken(L"ly");
    trie->addToken(L"lyhtem");
    std::vector<int> matches = trie->findMatchesReadingStringRightToLeft(L"ethyl", 5);
    assertNotNull(matches);

    assertEquals(3, matches.size());
    assertEquals(5, matches[0]);
    assertEquals(3, matches[1]);
    assertEquals(0, matches[2]);
}
