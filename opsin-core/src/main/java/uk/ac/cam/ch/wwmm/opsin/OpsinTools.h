#pragma once

#include "WordType.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ParsingException; } } } } } }

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


						using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

						/// <summary>
						/// A set of useful methods and constants to assist OPSIN
						/// @author dl387
						/// 
						/// </summary>
						class OpsinTools
						{

						public:
							static Pattern *const MATCH_COLONORSEMICOLON = Pattern::compile(L"[:;]");

							static Pattern *const MATCH_AMINOACID_STYLE_LOCANT = Pattern::compile(L"([A-Z][a-z]?)('*)((\\d+[a-z]?|alpha|beta|gamma|delta|epsilon|zeta|eta|omega)'*)");
							static Pattern *const MATCH_ELEMENT_SYMBOL = Pattern::compile(L"[A-Z][a-z]?");
							static Pattern *const MATCH_ELEMENT_SYMBOL_LOCANT = Pattern::compile(L"[A-Z][a-z]?'*");
							static Pattern *const MATCH_NUMERIC_LOCANT = Pattern::compile(L"(\\d+)[a-z]?'*");
							static constexpr wchar_t END_OF_SUBSTITUENT = L'\u00e9';
							static constexpr wchar_t END_OF_MAINGROUP = L'\u00e2';
							static constexpr wchar_t END_OF_FUNCTIONALTERM = L'\u00FB';

							static const std::wstring NEWLINE;

							/// <summary>
							/// Returns the next sibling suffix node which is not related to altering charge (ium/ide/id) </summary>
							/// <param name="currentEl"> </param>
							static Element *getNextNonChargeSuffix(Element *currentEl);

							/// <summary>
							/// Returns a new list containing the elements of list1 followed by list2 </summary>
							/// <param name="list1"> </param>
							/// <param name="list2"> </param>
							/// <returns> The new list </returns>
							static std::vector<Element*> combineElementLists(std::vector<Element*> &list1, std::vector<Element*> &list2);

							/// <summary>
							/// Returns the previous group. This group element need not be a sibling </summary>
							/// <param name="current">: starting element
							/// @return </param>
							static Element *getPreviousGroup(Element *current);

							/// <summary>
							/// Returns the next group. This group element need not be a sibling </summary>
							/// <param name="current">: starting element
							/// @return </param>
							static Element *getNextGroup(Element *current);

							/// <summary>
							/// Finds the wordRule element that encloses the given element.
							/// Returns the wordRule element or throws an exception </summary>
							/// <param name="el"> </param>
							/// <returns> wordRule Element </returns>
							static Element *getParentWordRule(Element *el);

							/// <summary>
							/// Searches in a depth-first manner for a non-suffix atom that has the target non element symbol locant
							/// Returns either that atom or null if one cannot be found </summary>
							/// <param name="startingAtom"> </param>
							/// <param name="targetLocant"> </param>
							/// <returns> the matching atom or null </returns>
							static Atom *depthFirstSearchForNonSuffixAtomWithLocant(Atom *startingAtom, const std::wstring &targetLocant);

							/// <summary>
							/// Searches in a depth-first manner for an atom with a numeric locant
							/// Returns either that atom or null if one cannot be found </summary>
							/// <param name="startingAtom"> </param>
							/// <returns> the matching atom or null </returns>
							static Atom *depthFirstSearchForAtomWithNumericLocant(Atom *startingAtom);

							/// <summary>
							/// Given a list of annotations returns the word type as indicated by the final annotation of the list </summary>
							/// <param name="annotations"> </param>
							/// <returns> WordType </returns>
							/// <exception cref="ParsingException">  </exception>
							static WordType determineWordType(std::vector<wchar_t> &annotations) throw(ParsingException);

							/// <summary>
							///Gets the next sibling of a given element.
							/// </summary>
							/// <param name="element"> The reference element. </param>
							/// <returns> The next Sibling, or null. </returns>
							static Element *getNextSibling(Element *element);

							/// <summary>
							///Gets the first next sibling of a given element whose element name matches the given string.
							/// </summary>
							/// <param name="current"> The reference element. </param>
							/// <param name="elName"> The element name to look for </param>
							/// <returns> The matched next Sibling, or null. </returns>
							static Element *getNextSibling(Element *current, const std::wstring &elName);

							/// <summary>
							///Gets the previous sibling of a given element.
							/// </summary>
							/// <param name="element"> The reference element. </param>
							/// <returns> The previous Sibling, or null. </returns>
							static Element *getPreviousSibling(Element *element);

							/// <summary>
							///Gets the first previous sibling of a given element whose element name matches the given string.
							/// </summary>
							/// <param name="current"> The reference element. </param>
							/// <param name="elName"> The element name of a element to look for </param>
							/// <returns> The matched previous Sibling, or null. </returns>
							static Element *getPreviousSibling(Element *current, const std::wstring &elName);

							/// <summary>
							///Inserts a element so that it occurs before a reference element. The new element
							/// must not currently have a parent.
							/// </summary>
							/// <param name="element"> The reference element. </param>
							/// <param name="newElement"> The new element to insert. </param>
							static void insertBefore(Element *element, Element *newElement);

							/// <summary>
							///Inserts an element so that it occurs after a reference element. The new element
							/// must not currently have a parent.
							/// </summary>
							/// <param name="element"> The reference element. </param>
							/// <param name="neweElement"> The new element to insert. </param>
							static void insertAfter(Element *element, Element *neweElement);

							/// <summary>
							/// Gets the next element. This element need not be a sibling </summary>
							/// <param name="element">: starting element
							/// @return </param>
							static Element *getNext(Element *element);

							/// <summary>
							/// Gets the previous element. This element need not be a sibling </summary>
							/// <param name="element">: starting element
							/// @return </param>
							static Element *getPrevious(Element *element);

							/// <summary>
							/// Returns a list containing sibling elements with the given element name after the given element.
							/// These elements need not be continuous </summary>
							/// <param name="currentElem">: the element to look for following siblings of </param>
							/// <param name="elName">: the name of the elements desired
							/// @return </param>
							static std::vector<Element*> getNextSiblingsOfType(Element *currentElem, const std::wstring &elName);

							/// <summary>
							/// Returns a list containing sibling elements with the given element name after the given element. </summary>
							/// <param name="currentElem">: the element to look for following siblings of </param>
							/// <param name="elName">: the name of the elements desired
							/// @return </param>
							static std::vector<Element*> getNextAdjacentSiblingsOfType(Element *currentElem, const std::wstring &elName);

							/// <summary>
							/// Returns a list containing sibling elements with the given element names after the given element.
							/// These elements need not be continuous and are returned in the order encountered </summary>
							/// <param name="currentElem">: the element to look for following siblings of </param>
							/// <param name="elNames">: An array of the names of the elements desired
							/// @return </param>
							static std::vector<Element*> getNextSiblingsOfTypes(Element *currentElem, std::vector<std::wstring> &elNames);

							/// <summary>
							/// Returns a list containing sibling elements with the given element name before the given element.
							/// These elements need not be continuous </summary>
							/// <param name="currentElem">: the element to look for previous siblings of </param>
							/// <param name="elName">: the name of the elements desired
							/// @return </param>
							static std::vector<Element*> getPreviousSiblingsOfType(Element *currentElem, const std::wstring &elName);

							/// <summary>
							/// Gets the next sibling element of the given element. If this element's name is within the elementsToIgnore array this is repeated
							/// If no appropriate element can be found null is returned </summary>
							/// <param name="startingEl"> </param>
							/// <param name="elNamesToIgnore">
							/// @return </param>
							static Element *getNextSiblingIgnoringCertainElements(Element *startingEl, std::vector<std::wstring> &elNamesToIgnore);

							/// <summary>
							/// Gets the previous sibling element of the given element. If this element's name is within the elementsToIgnore array this is repeated
							/// If no appropriate element can be found null is returned </summary>
							/// <param name="startingEl"> </param>
							/// <param name="elNamesToIgnore">
							/// @return </param>
							static Element *getPreviousSiblingIgnoringCertainElements(Element *startingEl, std::vector<std::wstring> &elNamesToIgnore);

							/// <summary>
							/// Finds all descendant elements whose name  matches the given element name </summary>
							/// <param name="startingElement"> </param>
							/// <param name="elementName">
							/// @return </param>
							static std::vector<Element*> getDescendantElementsWithTagName(Element *startingElement, const std::wstring &elementName);

							/// <summary>
							/// Finds all descendant elements whose element name matches one of the strings in elementNames </summary>
							/// <param name="startingElement"> </param>
							/// <param name="elementNames">
							/// @return </param>
							static std::vector<Element*> getDescendantElementsWithTagNames(Element *startingElement, std::vector<std::wstring> &elementNames);

							/// <summary>
							/// Finds all child elements whose element name matches one of the strings in elementNames </summary>
							/// <param name="startingElement"> </param>
							/// <param name="elementNames">
							/// @return </param>
							static std::vector<Element*> getChildElementsWithTagNames(Element *startingElement, std::vector<std::wstring> &elementNames);

							/// <summary>
							/// Finds all descendant elements whose element name matches the given elementName
							/// Additionally the element must have the specified attribute and the value of the attribute must be as specified </summary>
							/// <param name="startingElement"> </param>
							/// <param name="elementName"> </param>
							/// <param name="attributeName"> </param>
							/// <param name="attributeValue">
							/// @return </param>
							static std::vector<Element*> getDescendantElementsWithTagNameAndAttribute(Element *startingElement, const std::wstring &elementName, const std::wstring &attributeName, const std::wstring &attributeValue);

							/// <summary>
							/// Finds all child elements whose element name matches the given elementName
							/// Additionally the element must have the specified attribute and the value of the attribute must be as specified </summary>
							/// <param name="startingElement"> </param>
							/// <param name="elementName">
							/// @return </param>
							static std::vector<Element*> getChildElementsWithTagNameAndAttribute(Element *startingElement, const std::wstring &elementName, const std::wstring &attributeName, const std::wstring &attributeValue);

							/// <summary>
							/// Finds and returns the number of elements and the number of elements with no children, that are descendants of the startingElement
							/// The 0th position of the returned array is the total number of elements
							/// The 1st position is the number of child less elements </summary>
							/// <param name="startingElement">
							/// @return </param>
							static std::vector<int> countNumberOfElementsAndNumberOfChildLessElements(Element *startingElement);

							/// <summary>
							/// Find all the later siblings of startingElement up until there is no more siblings or an
							/// element with the given element name  is reached (exclusive of that element) </summary>
							/// <param name="startingEl"> </param>
							/// <param name="elName">
							/// @return </param>
							static std::vector<Element*> getSiblingsUpToElementWithTagName(Element *startingEl, const std::wstring &elName);
						};

					}
				}
			}
		}
	}
}
