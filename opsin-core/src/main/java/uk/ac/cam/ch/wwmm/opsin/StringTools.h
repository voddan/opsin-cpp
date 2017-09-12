#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class PreProcessingException; } } } } } }

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


						/// <summary>
						///Static routines for string manipulation.
						/// 
						/// @author ptc24
						/// @author dl387
						/// 
						/// </summary>
						class StringTools
						{

							/// <summary>
							/// Converts a list of strings into a single string delimited by the given separator
							/// </summary>
							/// <param name="list"> A list of strings. </param>
							/// <param name="separator"> </param>
							/// <returns> The corresponding string. </returns>
						public:
							static std::wstring stringListToString(std::vector<std::wstring> &list, const std::wstring &separator);

							/// <summary>
							///Produce repetitions of a string. Eg. HelloWorld * 2 = HelloWorldHelloWorld.
							/// </summary>
							/// <param name="s"> The string to multiply. </param>
							/// <param name="n"> The number of times to multiply it. </param>
							/// <returns> The multiplied string. </returns>
							static std::wstring multiplyString(const std::wstring &s, int n);

							/// <summary>
							///Joins an array of strings into a single string.
							/// </summary>
							/// <param name="stringArray"> The strings to join together. </param>
							/// <param name="separator"> The separator to use. </param>
							/// <returns> The resulting string. </returns>
							static std::wstring arrayToString(std::vector<std::wstring> &stringArray, const std::wstring &separator);

							/// <summary>
							///Converts a unicode string into ASCII
							/// e.g. converting Greek letters to their names (e.g. alpha)
							/// Unrecognised non-ASCII characters trigger an exception
							/// </summary>
							/// <param name="s"> The string to convert </param>
							/// <returns> The converted string </returns>
							/// <exception cref="PreProcessingException"> </exception>
							static std::wstring convertNonAsciiAndNormaliseRepresentation(const std::wstring &s) throw(PreProcessingException);

						private:
							static std::wstring getReplacementForNonASCIIChar(wchar_t c) throw(PreProcessingException);

							/// <summary>
							///Converts a string array to an ArrayList.
							/// </summary>
							/// <param name="array"> The array. </param>
							/// <returns> The ArrayList. </returns>
						public:
							static std::vector<std::wstring> arrayToList(std::vector<std::wstring> &array_Renamed);

							/// <summary>
							/// If a dash is the last character it is removed </summary>
							/// <param name="locantText">
							/// @return </param>
							static std::wstring removeDashIfPresent(const std::wstring &locantText);

							/// <summary>
							/// Counts the number of primes at the end of a locant </summary>
							/// <param name="locantText">
							/// @return </param>
							static int countTerminalPrimes(const std::wstring &locantText);

							/// <summary>
							/// Tests if this string start with the specified prefix ignoring case. </summary>
							/// <param name="str"> </param>
							/// <param name="prefix">
							/// @return </param>
							static bool startsWithCaseInsensitive(const std::wstring &str, const std::wstring &prefix);

							/// <summary>
							/// Tests if this string ends with the specified suffix ignoring case. </summary>
							/// <param name="str"> </param>
							/// <param name="suffix">
							/// @return </param>
							static bool endsWithCaseInsensitive(const std::wstring &str, const std::wstring &suffix);

							/// <summary>
							/// Lower cases a string (only converts A-Z to a-z) </summary>
							/// <param name="str"> </param>
							static std::wstring lowerCaseAsciiString(const std::wstring &str);
						};

					}
				}
			}
		}
	}
}
