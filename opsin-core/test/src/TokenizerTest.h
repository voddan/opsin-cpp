#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Tokeniser;
class ReverseParseRules;
class ParsingException;



//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;



class TokenizerTest {

private:
    static Tokeniser *tokenizer;
    static ReverseParseRules *reverseParseRules;

public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp() throws java.io.IOException
    static void setUp() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
    static void cleanUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hexane() throws ParsingException
    virtual void hexane() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hexachlorohexane() throws ParsingException
    virtual void hexachlorohexane() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ethylChloride() throws ParsingException
    virtual void ethylChloride() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hexachlorohexaneeeeeee() throws ParsingException
    virtual void hexachlorohexaneeeeeee() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedHexachlorohexane() throws ParsingException
    virtual void bracketedHexachlorohexane() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void methyl() throws ParsingException
    virtual void methyl() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void aceticacid() throws ParsingException
    virtual void aceticacid() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acceptableInterWordBreaks() throws ParsingException
    virtual void acceptableInterWordBreaks() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void compoundWithValidUse() throws ParsingException
    virtual void compoundWithValidUse() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void compoundWithInvalidUse1() throws ParsingException
    virtual void compoundWithInvalidUse1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void compoundWithInvalidUse2() throws ParsingException
    virtual void compoundWithInvalidUse2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void CCCP() throws ParsingException
    virtual void CCCP() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void CCCP_RL() throws ParsingException
    virtual void CCCP_RL() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void partiallyInterpretatableLR() throws ParsingException
    virtual void partiallyInterpretatableLR() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void partiallyInterpretatableRL1() throws ParsingException
    virtual void partiallyInterpretatableRL1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void partiallyInterpretatableRL2() throws ParsingException
    virtual void partiallyInterpretatableRL2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizeDoesNotTokenizeUnTokenizableName() throws ParsingException
    virtual void tokenizeDoesNotTokenizeUnTokenizableName() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameLR1() throws ParsingException
    virtual void tokenizePreservesSpacesInUninterpretableNameLR1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameLR1() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsableNameLR1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameLR1() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsedNameLR1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameLR2() throws ParsingException
    virtual void tokenizePreservesSpacesInUninterpretableNameLR2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameLR2() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsableNameLR2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameLR2() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsedNameLR2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameRL1() throws ParsingException
    virtual void tokenizePreservesSpacesInUninterpretableNameRL1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameRL1() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsableNameRL1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameRL1() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsedNameRL1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameRL2() throws ParsingException
    virtual void tokenizePreservesSpacesInUninterpretableNameRL2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameRL2() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsableNameRL2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameRL2() throws ParsingException
    virtual void tokenizePreservesSpacesInUnparsedNameRL2() throw(ParsingException);
};

