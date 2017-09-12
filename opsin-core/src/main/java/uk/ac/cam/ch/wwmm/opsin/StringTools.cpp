#include "StringTools.h"
#include "PreProcessingException.h"

namespace uk
{
	namespace ac
	{
		namespace cam
		{
			namespace ch
			{
				namespace wwmm
				{
					namespace opsin
					{

						std::wstring StringTools::stringListToString(std::vector<std::wstring> &list, const std::wstring &separator)
						{
							StringBuilder *sb = new StringBuilder();
							int lastIndexOfList = list.size() - 1;
							for (int i = 0; i < lastIndexOfList; i++)
							{
								sb->append(list[i]);
								sb->append(separator);
							}
							if (lastIndexOfList >= 0)
							{
								sb->append(list[lastIndexOfList]);
							}
							return sb->toString();
						}

						std::wstring StringTools::multiplyString(const std::wstring &s, int n)
						{
							StringBuilder *sb = new StringBuilder();
							for (int i = 0; i < n; i++)
							{
								sb->append(s);
							}
							return sb->toString();
						}

						std::wstring StringTools::arrayToString(std::vector<std::wstring> &stringArray, const std::wstring &separator)
						{
							StringBuilder *sb = new StringBuilder();
							int lastIndexOfArray = stringArray.size() - 1;
							for (int i = 0; i < lastIndexOfArray; i++)
							{
								sb->append(stringArray[i]);
								sb->append(separator);
							}
							if (lastIndexOfArray >= 0)
							{
								sb->append(stringArray[lastIndexOfArray]);
							}
							return sb->toString();
						}

						std::wstring StringTools::convertNonAsciiAndNormaliseRepresentation(const std::wstring &s) throw(PreProcessingException)
						{
							StringBuilder *sb = new StringBuilder(s.length());
							for (int i = 0, l = s.length(); i < l; i++)
							{
								wchar_t c = s[i];
								switch (c)
								{
								case L'\t':
								case L'\n':
								case L'\u000B': //vertical tab
								case L'\f':
								case L'\r':
									//normalise white space
									sb->append(L" ");
									break;
								case L'`':
									sb->append(L"'"); //replace back ticks with apostrophe
									break;
								case L'"':
									sb->append(L"''"); //replace quotation mark with two primes
									break;
								default:
									if (c >= 128)
									{
										sb->append(getReplacementForNonASCIIChar(c)); //replace non ascii characters with hard coded ascii strings
									}
									else if (c > 31)
									{ //ignore control characters
										sb->append(c);
									}
								}
							}
							return sb->toString();
						}

						std::wstring StringTools::getReplacementForNonASCIIChar(wchar_t c) throw(PreProcessingException)
						{
							switch (c)
							{
								case L'\u03b1':
									return L"alpha"; //greeks
								case L'\u03b2':
									return L"beta";
								case L'\u03b3':
									return L"gamma";
								case L'\u03b4':
									return L"delta";
								case L'\u03b5':
									return L"epsilon";
								case L'\u03b6':
									return L"zeta";
								case L'\u03b7':
									return L"eta";
								case L'\u03b8':
									return L"theta";
								case L'\u03b9':
									return L"iota";
								case L'\u03ba':
									return L"kappa";
								case L'\u03bb':
									return L"lambda";
								case L'\u03bc':
									return L"mu";
								case L'\u03bd':
									return L"nu";
								case L'\u03be':
									return L"xi";
								case L'\u03bf':
									return L"omicron";
								case L'\u03c0':
									return L"pi";
								case L'\u03c1':
									return L"rho";
								case L'\u03c2':
									return L"stigma";
								case L'\u03c3':
									return L"sigma";
								case L'\u03c4':
									return L"tau";
								case L'\u03c5':
									return L"upsilon";
								case L'\u03c6':
									return L"phi";
								case L'\u03c7':
									return L"chi";
								case L'\u03c8':
									return L"psi";
								case L'\u03c9':
									return L"omega";

								case L'\u1D05':
									return L"D"; //small capitals
								case L'\u029F':
									return L"L";

								case L'\u00B1':
									return L"+-"; //plus minus symbol
								case L'\u2213':
									return L"-+";

								case L'\u2192': //right arrows
								case L'\u2794':
								case L'\u2799':
								case L'\u279C':
									return L"->";

								case L'\u00C6':
									return L"AE"; //common ligatures
								case L'\u00E6':
									return L"ae";
								case L'\u0152':
									return L"OE";
								case L'\u0153':
									return L"oe";
								case L'\u0132':
									return L"IJ";
								case L'\u0133':
									return L"ij";
								case L'\u1D6B':
									return L"ue";
								case L'\uFB00':
									return L"ff";
								case L'\uFB01':
									return L"fi";
								case L'\uFB02':
									return L"fl";
								case L'\uFB03':
									return L"ffi";
								case L'\uFB04':
									return L"ffl";
								case L'\uFB06':
									return L"st";

								case L'\u00E0':
									return L"a"; //diacritics
								case L'\u00C0':
									return L"A";
								case L'\u00E1':
									return L"a";
								case L'\u00C1':
									return L"A";
								case L'\u00E2':
									return L"a";
								case L'\u00C2':
									return L"A";
								case L'\u00E3':
									return L"a";
								case L'\u00C3':
									return L"A";
								case L'\u00E4':
									return L"a";
								case L'\u00C4':
									return L"A";
								case L'\u00E5':
									return L"a";
								case L'\u00C5':
									return L"A";
								case L'\u00E7':
									return L"c";
								case L'\u00C7':
									return L"C";
								case L'\u00E8':
									return L"e";
								case L'\u00C8':
									return L"E";
								case L'\u00E9':
									return L"e";
								case L'\u00C9':
									return L"E";
								case L'\u00EA':
									return L"e";
								case L'\u00CA':
									return L"E";
								case L'\u00EB':
									return L"e";
								case L'\u00CB':
									return L"E";
								case L'\u00EC':
									return L"i";
								case L'\u00CC':
									return L"I";
								case L'\u00ED':
									return L"i";
								case L'\u00CD':
									return L"I";
								case L'\u00EE':
									return L"i";
								case L'\u00CE':
									return L"I";
								case L'\u00EF':
									return L"i";
								case L'\u00CF':
									return L"I";
								case L'\u00F2':
									return L"o";
								case L'\u00D2':
									return L"O";
								case L'\u00F3':
									return L"o";
								case L'\u00D3':
									return L"O";
								case L'\u00F4':
									return L"o";
								case L'\u00D4':
									return L"O";
								case L'\u00F5':
									return L"o";
								case L'\u00D5':
									return L"O";
								case L'\u00F6':
									return L"o";
								case L'\u00D6':
									return L"O";
								case L'\u00F9':
									return L"u";
								case L'\u00D9':
									return L"U";
								case L'\u00FA':
									return L"u";
								case L'\u00DA':
									return L"U";
								case L'\u00FB':
									return L"u";
								case L'\u00DB':
									return L"U";
								case L'\u00FC':
									return L"u";
								case L'\u00DC':
									return L"U";
								case L'\u00FD':
									return L"y";
								case L'\u00DD':
									return L"Y";

								case L'\u0115':
									return L"e";
								case L'\u0114':
									return L"E";
								case L'\u0117':
									return L"e";
								case L'\u0116':
									return L"E";

								case L'\u2070':
									return L"0"; //superscripts
								case L'\u00B9':
									return L"1";
								case L'\u00B2':
									return L"2";
								case L'\u00B3':
									return L"3";
								case L'\u2074':
									return L"4";
								case L'\u2075':
									return L"5";
								case L'\u2076':
									return L"6";
								case L'\u2077':
									return L"7";
								case L'\u2078':
									return L"8";
								case L'\u2079':
									return L"9";

								case L'\u2080':
									return L"0"; //subscripts
								case L'\u2081':
									return L"1";
								case L'\u2082':
									return L"2";
								case L'\u2083':
									return L"3";
								case L'\u2084':
									return L"4";
								case L'\u2085':
									return L"5";
								case L'\u2086':
									return L"6";
								case L'\u2087':
									return L"7";
								case L'\u2088':
									return L"8";
								case L'\u2089':
									return L"9";

								case L'\u2018':
									return L"'"; //quotation marks and primes (map to apostrophe/s)
								case L'\u2019':
									return L"'";
								case L'\u201B':
									return L"'";
								case L'\u02BC':
									return L"'";
								case L'\u201C':
									return L"''";
								case L'\u201D':
									return L"''";
								case L'\u2032':
									return L"'"; //primes
								case L'\u2033':
									return L"''";
								case L'\u2034':
									return L"'''";
								case L'\u2057':
									return L"''''";
								case L'\u02B9':
									return L"'"; //modifier primes
								case L'\u02BA':
									return L"''";
								case L'\u2035':
									return L"'"; //back primes
								case L'\u2036':
									return L"''";
								case L'\u2037':
									return L"'''";
								case L'\u00B4':
									return L"'"; //accents
								case L'\u02CA':
									return L"'";
								case L'\u0301':
									return L"'";
								case L'\u02DD':
									return L"''";
								case L'\u030B':
									return L"''";

								case L'\u2010': //dashes, hyphens and the minus sign
								case L'\u2011':
								case L'\u2012':
								case L'\u2013':
								case L'\u2014':
								case L'\u2015':
								case L'\u2212':
									return L"-";

								case L'\u02DC': //small tilde
								case L'\u223C': //tilde operator
								case L'\u301C':
									return L"~"; //wave dash

								case L'\uff0c':
									return L","; //full width punctuation
								case L'\uFF1A':
									return L":";
								case L'\uFF1B':
									return L";";
								case L'\uFF08':
									return L"(";
								case L'\uFF09':
									return L")";
								case L'\uFF3B':
									return L"[";
								case L'\uFF3D':
									return L"]";
								case L'\u3010':
									return L"[";
								case L'\u3011':
									return L"]";
								case L'\uFF5B':
									return L"{";
								case L'\uFF5D':
									return L"}";

								case L'\u00DF':
									return L"beta"; //similar glyph

								case L'\u2000': //different sized spaces
								case L'\u2001':
								case L'\u2002':
								case L'\u2003':
								case L'\u2004':
								case L'\u2005':
								case L'\u2006':
								case L'\u2008':
								case L'\u2009':
								case L'\u200A':
								case L'\u205F':
								case L'\u00A0': //Non-breaking spaces
								case L'\u2007':
								case L'\u202F':
								case L'\u3000':
									return L" "; //ideographic space

								case L'\u00AD': //soft hyphen
								case L'\u200b': //zero width space
								case L'\u200d': //zero width joiner
								case L'\uFEFF':
									return L""; //BOM-found at the start of some UTF files

								default:
									throw PreProcessingException(std::wstring(L"Unrecognised unicode character: ") + StringHelper::toString(c));
							}
						}

						std::vector<std::wstring> StringTools::arrayToList(std::vector<std::wstring> &array_Renamed)
						{
							std::vector<std::wstring> list;
							list.addAll(Arrays::asList(array_Renamed));
							return list;
						}

						std::wstring StringTools::removeDashIfPresent(const std::wstring &locantText)
						{
							if (boost::ends_with(locantText, L"-"))
							{
								locantText = locantText.substr(0, locantText.length() - 1);
							}
							return locantText;
						}

						int StringTools::countTerminalPrimes(const std::wstring &locantText)
						{
							int numberOfPrimes = 0;
							for (int i = locantText.length() - 1; i > 0; i--)
							{
								if (locantText[i] == L'\'')
								{
									numberOfPrimes++;
								}
								else
								{
									break;
								}
							}
							return numberOfPrimes;
						}

						bool StringTools::startsWithCaseInsensitive(const std::wstring &str, const std::wstring &prefix)
						{
							return str.regionMatches(true, 0, prefix, 0, prefix.length());
						}

						bool StringTools::endsWithCaseInsensitive(const std::wstring &str, const std::wstring &suffix)
						{
							if (suffix.length() > str.length())
							{
								return false;
							}
							int strOffset = str.length() - suffix.length();
							return str.regionMatches(true, strOffset, suffix, 0, suffix.length());
						}

						std::wstring StringTools::lowerCaseAsciiString(const std::wstring &str)
						{
							StringBuilder *sb = new StringBuilder(str.length());
							for (int i = 0, l = str.length(); i < l; i++)
							{
								wchar_t c = str[i];
								if (c >= L'A' && c <= L'Z')
								{
									c = static_cast<wchar_t>(c + 32);
								}
								sb->append(c);
							}
							return sb->toString();
						}
					}
				}
			}
		}
	}
}
