#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <iostream>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; } } }

/*
 * dk.brics.automaton
 * 
 * Copyright (c) 2001-2017 Anders Moeller
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace dk
{
	namespace brics
	{
		namespace automaton
		{


			/// <summary>
			/// Basic automata for representing common datatypes
			/// related to Unicode, XML, and XML Schema.
			/// </summary>
			class Datatypes final
			{

			private:
				static const std::unordered_map<std::wstring, Automaton*> automata;

				static Automaton *const ws;

				static Set<std::wstring> *const unicodeblock_names;

				static Set<std::wstring> *const unicodecategory_names;

				static Set<std::wstring> *const xml_names;

				static std::vector<std::wstring> const unicodeblock_names_array;

				static std::vector<std::wstring> const unicodecategory_names_array;

				static std::vector<std::wstring> const xml_names_array;

						private:
							class StaticConstructor
							{
							public:
								StaticConstructor();
							};

						private:
							static Datatypes::StaticConstructor staticConstructor;


				Datatypes();

				/// <summary>
				/// Invoke during compilation to pre-build automata.
				/// Automata are stored in the directory specified by the system property <tt>dk.brics.automaton.datatypes</tt>.
				/// (Default: <tt>build</tt>, relative to the current working directory.)
				/// </summary>
				static void main(std::vector<std::wstring> &args);

				/// <summary>
				/// Returns pre-built automaton.
				/// Automata are loaded as resources from the class loader of the <tt>Datatypes</tt> class.
				/// (Typically, the pre-built automata are stored in the same jar as this class.)
				/// <para>
				/// The following automata are available:
				/// <table border=1>
				/// <tr><th>Name</th><th>Description</th></tr>
				/// <tr><td><tt>NCName</tt></td><td><a target="_top" href="http://www.w3.org/TR/REC-xml-names/#NT-NCName">NCName</a> from XML Namespaces 1.0</td></tr>
				/// <tr><td><tt>QName</tt></td><td><a target="_top" href="http://www.w3.org/TR/REC-xml-names/#NT-QName">QName</a> from XML Namespaces 1.0</td></tr>
				/// <tr><td><tt>Char</tt></td><td><a target="_top" href="http://www.w3.org/TR/REC-xml/#NT-Char">Char</a> from XML 1.0</td></tr>
				/// <tr><td><tt>NameChar</tt></td><td><a target="_top" href="http://www.w3.org/TR/REC-xml/#NT-NameChar">NameChar</a> from XML 1.0</td></tr>
				/// <tr><td><tt>URI</tt></td><td><a target="_top" href="http://rfc.net/rfc2396.html#sA%2e">URI</a> from RFC2396 with
				/// amendments from <a target="_top" href="http://www.faqs.org/rfcs/rfc2373.html">RFC2373</a></td></tr>
				/// <tr><td><tt>anyname</tt></td><td>optional URI enclosed by brackets, followed by NCName</td></tr>
				/// <tr><td><tt>noap</tt></td><td>strings not containing '@' and '%'</td></tr>
				/// <tr><td><tt>whitespace</tt></td><td>optional <a target="_top" href="http://www.w3.org/TR/REC-xml/#NT-S">S</a> from XML 1.0</td></tr>
				/// <tr><td><tt>whitespacechar</tt></td><td>a single <a target="_top" href="http://www.w3.org/TR/REC-xml/#NT-S">whitespace character</a> from XML 1.0</td></tr>
				/// <tr><td><tt>string</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#string">string</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>boolean</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#boolean">boolean</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>decimal</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#decimal">decimal</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>float</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#float">float</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>integer</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#integer">integer</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>duration</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#duration">duration</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>dateTime</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#dateTime">dateTime</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>time</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#time">time</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>date</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#date">date</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>gYearMonth</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#gYearMonth">gYearMonth</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>gYear</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#gYear">gYear</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>gMonthDay</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#gMonthDay">gMonthDay</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>gDay</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#gDay">gDay</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>hexBinary</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#hexBinary">hexBinary</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>base64Binary</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#base64Binary">base64Binary</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>NCName2</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#NCName">NCName</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>NCNames</tt></td><td>list of <a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#NCName">NCName</a>s from XML Schema Part 2</td></tr>
				/// <tr><td><tt>QName2</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#QName">QName</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>Nmtoken2</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#NMTOKEN">NMTOKEN</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>Nmtokens</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#NMTOKENS">NMTOKENS</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>Name2</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#Name">Name</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>Names</tt></td><td>list of <a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#Name">Name</a>s from XML Schema Part 2</td></tr>
				/// <tr><td><tt>language</tt></td><td><a target="_top" href="http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#language">language</a> from XML Schema Part 2</td></tr>
				/// <tr><td><tt>BasicLatin</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">BasicLatin</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Latin-1Supplement</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Latin-1Supplement</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>LatinExtended-A</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">LatinExtended-A</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>LatinExtended-B</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">LatinExtended-B</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>IPAExtensions</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">IPAExtensions</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>SpacingModifierLetters</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">SpacingModifierLetters</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CombiningDiacriticalMarks</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CombiningDiacriticalMarks</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Greek</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Greek</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Cyrillic</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Cyrillic</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Armenian</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Armenian</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Hebrew</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Hebrew</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Arabic</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Arabic</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Syriac</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Syriac</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Thaana</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Thaana</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Devanagari</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Devanagari</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Bengali</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Bengali</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Gurmukhi</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Gurmukhi</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Gujarati</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Gujarati</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Oriya</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Oriya</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Tamil</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Tamil</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Telugu</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Telugu</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Kannada</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Kannada</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Malayalam</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Malayalam</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Sinhala</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Sinhala</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Thai</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Thai</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Lao</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Lao</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Tibetan</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Tibetan</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Myanmar</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Myanmar</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Georgian</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Georgian</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>HangulJamo</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">HangulJamo</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Ethiopic</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Ethiopic</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Cherokee</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Cherokee</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>UnifiedCanadianAboriginalSyllabics</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">UnifiedCanadianAboriginalSyllabics</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Ogham</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Ogham</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Runic</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Runic</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Khmer</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Khmer</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Mongolian</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Mongolian</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>LatinExtendedAdditional</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">LatinExtendedAdditional</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>GreekExtended</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">GreekExtended</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>GeneralPunctuation</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">GeneralPunctuation</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>SuperscriptsandSubscripts</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">SuperscriptsandSubscripts</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CurrencySymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CurrencySymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CombiningMarksforSymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CombiningMarksforSymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>LetterlikeSymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">LetterlikeSymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>NumberForms</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">NumberForms</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Arrows</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Arrows</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>MathematicalOperators</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">MathematicalOperators</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>MiscellaneousTechnical</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">MiscellaneousTechnical</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>ControlPictures</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">ControlPictures</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>OpticalCharacterRecognition</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">OpticalCharacterRecognition</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>EnclosedAlphanumerics</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">EnclosedAlphanumerics</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>BoxDrawing</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">BoxDrawing</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>BlockElements</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">BlockElements</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>GeometricShapes</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">GeometricShapes</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>MiscellaneousSymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">MiscellaneousSymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Dingbats</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Dingbats</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>BraillePatterns</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">BraillePatterns</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKRadicalsSupplement</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKRadicalsSupplement</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>KangxiRadicals</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">KangxiRadicals</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>IdeographicDescriptionCharacters</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">IdeographicDescriptionCharacters</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKSymbolsandPunctuation</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKSymbolsandPunctuation</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Hiragana</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Hiragana</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Katakana</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Katakana</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Bopomofo</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Bopomofo</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>HangulCompatibilityJamo</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">HangulCompatibilityJamo</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Kanbun</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Kanbun</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>BopomofoExtended</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">BopomofoExtended</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>EnclosedCJKLettersandMonths</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">EnclosedCJKLettersandMonths</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKCompatibility</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKCompatibility</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKUnifiedIdeographsExtensionA</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKUnifiedIdeographsExtensionA</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKUnifiedIdeographs</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKUnifiedIdeographs</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>YiSyllables</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">YiSyllables</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>YiRadicals</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">YiRadicals</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>HangulSyllables</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">HangulSyllables</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKCompatibilityIdeographs</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKCompatibilityIdeographs</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>AlphabeticPresentationForms</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">AlphabeticPresentationForms</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>ArabicPresentationForms-A</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">ArabicPresentationForms-A</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CombiningHalfMarks</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CombiningHalfMarks</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKCompatibilityForms</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKCompatibilityForms</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>SmallFormVariants</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">SmallFormVariants</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>ArabicPresentationForms-B</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">ArabicPresentationForms-B</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Specials</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Specials</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>HalfwidthandFullwidthForms</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">HalfwidthandFullwidthForms</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Specials</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Specials</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>OldItalic</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">OldItalic</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Gothic</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Gothic</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Deseret</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Deseret</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>ByzantineMusicalSymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">ByzantineMusicalSymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>MusicalSymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">MusicalSymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>MathematicalAlphanumericSymbols</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">MathematicalAlphanumericSymbols</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKUnifiedIdeographsExtensionB</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKUnifiedIdeographsExtensionB</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>CJKCompatibilityIdeographsSupplement</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">CJKCompatibilityIdeographsSupplement</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Tags</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/Blocks-4.txt">Tags</a> block from Unicode 3.1</td></tr>
				/// <tr><td><tt>Lu</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Lu</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Ll</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Ll</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Lt</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Lt</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Lm</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Lm</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Lo</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Lo</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>L</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">L</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Mn</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Mn</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Mc</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Mc</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Me</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Me</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>M</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">M</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Nd</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Nd</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Nl</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Nl</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>No</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">No</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>N</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">N</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Pc</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Pc</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Pd</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Pd</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Ps</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Ps</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Pe</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Pe</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Pi</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Pi</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Pf</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Pf</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Po</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Po</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>P</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">P</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Zs</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Zs</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Zl</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Zl</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Zp</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Zp</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Z</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Z</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Sm</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Sm</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Sc</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Sc</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Sk</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Sk</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>So</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">So</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>S</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">S</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Cc</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Cc</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Cf</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Cf</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Co</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Co</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>Cn</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">Cn</a> category from Unicode 3.1</td></tr>
				/// <tr><td><tt>C</tt></td><td><a target="_top" href="http://www.unicode.org/Public/3.1-Update/UnicodeData-3.1.0.txt">C</a> category from Unicode 3.1</td></tr>
				/// </table>
				/// </para>
				/// <para>Loaded automata are cached in memory.
				/// </para>
				/// </summary>
				/// <param name="name"> name of automaton </param>
				/// <returns> automaton </returns>
			public:
				static Automaton *get(const std::wstring &name);

				/// <summary>
				/// Checks whether the given string is the name of a Unicode block (see <seealso cref="#get(String)"/>).
				/// </summary>
				static bool isUnicodeBlockName(const std::wstring &name);

				/// <summary>
				/// Checks whether the given string is the name of a Unicode category (see <seealso cref="#get(String)"/>).
				/// </summary>
				static bool isUnicodeCategoryName(const std::wstring &name);

				/// <summary>
				/// Checks whether the given string is the name of an XML / XML Schema automaton (see <seealso cref="#get(String)"/>).
				/// </summary>
				static bool isXMLName(const std::wstring &name);

				/// <summary>
				/// Checks whether a given automaton is available. </summary>
				/// <param name="name"> automaton name </param>
				/// <returns> true if the automaton is available </returns>
				static bool exists(const std::wstring &name);

			private:
				static Automaton *load(const std::wstring &name);

				static void store(const std::wstring &name, Automaton *a);

				static void buildAll();

				static Automaton *makeCodePoint(int cp);

				static std::unordered_map<std::wstring, Automaton*> buildMap(std::vector<std::wstring> &exps);

				static void putWith(std::vector<std::wstring> &exps, std::unordered_map<std::wstring, Automaton*> &use);

				static void putFrom(const std::wstring &name, std::unordered_map<std::wstring, Automaton*> &from);

				static void put(std::unordered_map<std::wstring, Automaton*> &map, const std::wstring &name, Automaton *a);

			public:
				static Automaton *getWhitespaceAutomaton();
			};

		}
	}
}
