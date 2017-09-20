#include "Datatypes.h"
#include "Automaton.h"
#include "RegExp.h"
#include <fstream>

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

const std::unordered_map<std::wstring, Automaton*> Datatypes::automata;
Automaton *const Datatypes::ws;
std::set<std::wstring> *const Datatypes::unicodeblock_names;
std::set<std::wstring> *const Datatypes::unicodecategory_names;
std::set<std::wstring> *const Datatypes::xml_names;
std::vector<std::wstring> const Datatypes::unicodeblock_names_array = {L"BasicLatin", L"Latin-1Supplement", L"LatinExtended-A", L"LatinExtended-B", L"IPAExtensions", L"SpacingModifierLetters", L"CombiningDiacriticalMarks", L"Greek", L"Cyrillic", L"Armenian", L"Hebrew", L"Arabic", L"Syriac", L"Thaana", L"Devanagari", L"Bengali", L"Gurmukhi", L"Gujarati", L"Oriya", L"Tamil", L"Telugu", L"Kannada", L"Malayalam", L"Sinhala", L"Thai", L"Lao", L"Tibetan", L"Myanmar", L"Georgian", L"HangulJamo", L"Ethiopic", L"Cherokee", L"UnifiedCanadianAboriginalSyllabics", L"Ogham", L"Runic", L"Khmer", L"Mongolian", L"LatinExtendedAdditional", L"GreekExtended", L"GeneralPunctuation", L"SuperscriptsandSubscripts", L"CurrencySymbols", L"CombiningMarksforSymbols", L"LetterlikeSymbols", L"NumberForms", L"Arrows", L"MathematicalOperators", L"MiscellaneousTechnical", L"ControlPictures", L"OpticalCharacterRecognition", L"EnclosedAlphanumerics", L"BoxDrawing", L"BlockElements", L"GeometricShapes", L"MiscellaneousSymbols", L"Dingbats", L"BraillePatterns", L"CJKRadicalsSupplement", L"KangxiRadicals", L"IdeographicDescriptionCharacters", L"CJKSymbolsandPunctuation", L"Hiragana", L"Katakana", L"Bopomofo", L"HangulCompatibilityJamo", L"Kanbun", L"BopomofoExtended", L"EnclosedCJKLettersandMonths", L"CJKCompatibility", L"CJKUnifiedIdeographsExtensionA", L"CJKUnifiedIdeographs", L"YiSyllables", L"YiRadicals", L"HangulSyllables", L"CJKCompatibilityIdeographs", L"AlphabeticPresentationForms", L"ArabicPresentationForms-A", L"CombiningHalfMarks", L"CJKCompatibilityForms", L"SmallFormVariants", L"ArabicPresentationForms-B", L"Specials", L"HalfwidthandFullwidthForms", L"Specials", L"OldItalic", L"Gothic", L"Deseret", L"ByzantineMusicalSymbols", L"MusicalSymbols", L"MathematicalAlphanumericSymbols", L"CJKUnifiedIdeographsExtensionB", L"CJKCompatibilityIdeographsSupplement", L"Tags"};
std::vector<std::wstring> const Datatypes::unicodecategory_names_array = {L"Lu", L"Ll", L"Lt", L"Lm", L"Lo", L"L", L"Mn", L"Mc", L"Me", L"M", L"Nd", L"Nl", L"No", L"N", L"Pc", L"Pd", L"Ps", L"Pe", L"Pi", L"Pf", L"Po", L"P", L"Zs", L"Zl", L"Zp", L"Z", L"Sm", L"Sc", L"Sk", L"So", L"S", L"Cc", L"Cf", L"Co", L"Cn", L"C"};
std::vector<std::wstring> const Datatypes::xml_names_array = {L"NCName", L"QName", L"Char", L"NameChar", L"URI", L"anyname", L"noap", L"whitespace", L"whitespacechar", L"string", L"boolean", L"decimal", L"float", L"integer", L"duration", L"dateTime", L"time", L"date", L"gYearMonth", L"gYear", L"gMonthDay", L"gDay", L"hexBinary", L"base64Binary", L"NCName2", L"NCNames", L"QName2", L"Nmtoken2", L"Nmtokens", L"Name2", L"Names", L"language"};

			Datatypes::StaticConstructor::StaticConstructor()
			{
									automata = std::unordered_map<std::wstring, Automaton*>();
									ws = Automaton::minimize(Automaton::makeCharSet(L" \t\n\r")->repeat());
									unicodeblock_names = std::unordered_set<std::wstring>(Arrays::asList(unicodeblock_names_array));
									unicodecategory_names = std::unordered_set<std::wstring>(Arrays::asList(unicodecategory_names_array));
									xml_names = std::unordered_set<std::wstring>(Arrays::asList(xml_names_array));
			}

Datatypes::StaticConstructor Datatypes::staticConstructor;

			Datatypes::Datatypes()
			{
			}

			void Datatypes::main(std::vector<std::wstring> &args)
			{
				long long t = System::currentTimeMillis();
				bool b = Automaton::setAllowMutate(true);
				buildAll();
				Automaton::setAllowMutate(b);
				std::wcout << std::wstring(L"Storing automata...") << std::endl;
				for (auto e : automata)
				{
					store(e->getKey(), e->getValue());
				}
				std::wcout << std::wstring(L"Time for building automata: ") << (System::currentTimeMillis() - t) << std::wstring(L"ms") << std::endl;
			}

			Automaton *Datatypes::get(const std::wstring &name)
			{
				Automaton *a = automata[name];
				if (a == nullptr)
				{
					a = load(name);
					automata[name] = a;
				}
				return a;
			}

			bool Datatypes::isUnicodeBlockName(const std::wstring &name)
			{
				return unicodeblock_names->contains(name);
			}

			bool Datatypes::isUnicodeCategoryName(const std::wstring &name)
			{
				return unicodecategory_names->contains(name);
			}

			bool Datatypes::isXMLName(const std::wstring &name)
			{
				return xml_names->contains(name);
			}

			bool Datatypes::exists(const std::wstring &name)
			{
				try
				{
					//noinspection ConstantConditions
					Datatypes::typeid->getClassLoader().getResource(name + std::wstring(L".aut")).openStream()->close();
				}
				catch (const IOException &e)
				{
					return false;
				}
				return true;
			}

			Automaton *Datatypes::load(const std::wstring &name)
			{
				try
				{
					std::ifstream resource{name + std::wstring(L".aut")};
					return Automaton::load(resource);
				}
				catch (const IOException &e)
				{
					e->printStackTrace();
					return nullptr;
				}
				catch (const ClassNotFoundException &e)
				{
					e->printStackTrace();
					return nullptr;
				}
			}

			void Datatypes::store(const std::wstring &name, Automaton *a)
			{
				std::wstring dir = System::getProperty(L"dk.brics.automaton.datatypes");
				if (dir == L"")
				{
					dir = L"build";
				}
				try
				{
					FileOutputStream tempVar(dir + std::wstring(L"/") + name + std::wstring(L".aut"));
					a->store((&tempVar));
				}
				catch (const IOException &e)
				{
					e->printStackTrace();
				}
			}

			void Datatypes::buildAll()
			{
				std::vector<std::wstring> xmlexps = {L"Extender", L"[\u3031-\u3035\u309D-\u309E\u30FC-\u30FE\u00B7\u02D0\u02D1\u0387\u0640\u0E46\u0EC6\u3005]", L"CombiningChar", std::wstring(L"[\u0300-\u0345\u0360-\u0361\u0483-\u0486\u0591-\u05A1\u05A3-\u05B9\u05BB-\u05BD\u05C1-\u05C2\u064B-\u0652") + std::wstring(L"\u06D6-\u06DC\u06DD-\u06DF\u06E0-\u06E4\u06E7-\u06E8\u06EA-\u06ED\u0901-\u0903\u093E-\u094C\u0951-\u0954") + std::wstring(L"\u0962-\u0963\u0981-\u0983\u09C0-\u09C4\u09C7-\u09C8\u09CB-\u09CD\u09E2-\u09E3\u0A40-\u0A42\u0A47-\u0A48") + std::wstring(L"\u0A4B-\u0A4D\u0A70-\u0A71\u0A81-\u0A83\u0ABE-\u0AC5\u0AC7-\u0AC9\u0ACB-\u0ACD\u0B01-\u0B03\u0B3E-\u0B43") + std::wstring(L"\u0B47-\u0B48\u0B4B-\u0B4D\u0B56-\u0B57\u0B82-\u0B83\u0BBE-\u0BC2\u0BC6-\u0BC8\u0BCA-\u0BCD\u0C01-\u0C03") + std::wstring(L"\u0C3E-\u0C44\u0C46-\u0C48\u0C4A-\u0C4D\u0C55-\u0C56\u0C82-\u0C83\u0CBE-\u0CC4\u0CC6-\u0CC8\u0CCA-\u0CCD") + std::wstring(L"\u0CD5-\u0CD6\u0D02-\u0D03\u0D3E-\u0D43\u0D46-\u0D48\u0D4A-\u0D4D\u0E34-\u0E3A\u0E47-\u0E4E\u0EB4-\u0EB9") + std::wstring(L"\u0EBB-\u0EBC\u0EC8-\u0ECD\u0F18-\u0F19\u0F71-\u0F84\u0F86-\u0F8B\u0F90-\u0F95\u0F99-\u0FAD\u0FB1-\u0FB7") + std::wstring(L"\u20D0-\u20DC\u302A-\u302F\u05BF\u05C4\u0670\u093C\u094D\u09BC\u09BE\u09BF\u09D7\u0A02\u0A3C\u0A3E\u0A3F") + std::wstring(L"\u0ABC\u0B3C\u0BD7\u0D57\u0E31\u0EB1\u0F35\u0F37\u0F39\u0F3E\u0F3F\u0F97\u0FB9\u20E1\u3099\u309A]"), L"Digit", std::wstring(L"[\u0030-\u0039\u0660-\u0669\u06F0-\u06F9\u0966-\u096F\u09E6-\u09EF\u0A66-\u0A6F\u0AE6-\u0AEF\u0B66-\u0B6F") + std::wstring(L"\u0BE7-\u0BEF\u0C66-\u0C6F\u0CE6-\u0CEF\u0D66-\u0D6F\u0E50-\u0E59\u0ED0-\u0ED9\u0F20-\u0F29]"), L"Ideographic", L"[\u4E00-\u9FA5\u3021-\u3029\u3007]", L"BaseChar", std::wstring(L"[\u0041-\u005A\u0061-\u007A\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u00FF\u0100-\u0131\u0134-\u013E\u0141-\u0148") + std::wstring(L"\u014A-\u017E\u0180-\u01C3\u01CD-\u01F0\u01F4-\u01F5\u01FA-\u0217\u0250-\u02A8\u02BB-\u02C1\u0388-\u038A") + std::wstring(L"\u038E-\u03A1\u03A3-\u03CE\u03D0-\u03D6\u03E2-\u03F3\u0401-\u040C\u040E-\u044F\u0451-\u045C\u045E-\u0481") + std::wstring(L"\u0490-\u04C4\u04C7-\u04C8\u04CB-\u04CC\u04D0-\u04EB\u04EE-\u04F5\u04F8-\u04F9\u0531-\u0556\u0561-\u0586") + std::wstring(L"\u05D0-\u05EA\u05F0-\u05F2\u0621-\u063A\u0641-\u064A\u0671-\u06B7\u06BA-\u06BE\u06C0-\u06CE\u06D0-\u06D3") + std::wstring(L"\u06E5-\u06E6\u0905-\u0939\u0958-\u0961\u0985-\u098C\u098F-\u0990\u0993-\u09A8\u09AA-\u09B0\u09B6-\u09B9") + std::wstring(L"\u09DC-\u09DD\u09DF-\u09E1\u09F0-\u09F1\u0A05-\u0A0A\u0A0F-\u0A10\u0A13-\u0A28\u0A2A-\u0A30\u0A32-\u0A33") + std::wstring(L"\u0A35-\u0A36\u0A38-\u0A39\u0A59-\u0A5C\u0A72-\u0A74\u0A85-\u0A8B\u0A8F-\u0A91\u0A93-\u0AA8\u0AAA-\u0AB0") + std::wstring(L"\u0AB2-\u0AB3\u0AB5-\u0AB9\u0B05-\u0B0C\u0B0F-\u0B10\u0B13-\u0B28\u0B2A-\u0B30\u0B32-\u0B33\u0B36-\u0B39") + std::wstring(L"\u0B5C-\u0B5D\u0B5F-\u0B61\u0B85-\u0B8A\u0B8E-\u0B90\u0B92-\u0B95\u0B99-\u0B9A\u0B9E-\u0B9F\u0BA3-\u0BA4") + std::wstring(L"\u0BA8-\u0BAA\u0BAE-\u0BB5\u0BB7-\u0BB9\u0C05-\u0C0C\u0C0E-\u0C10\u0C12-\u0C28\u0C2A-\u0C33\u0C35-\u0C39") + std::wstring(L"\u0C60-\u0C61\u0C85-\u0C8C\u0C8E-\u0C90\u0C92-\u0CA8\u0CAA-\u0CB3\u0CB5-\u0CB9\u0CE0-\u0CE1\u0D05-\u0D0C") + std::wstring(L"\u0D0E-\u0D10\u0D12-\u0D28\u0D2A-\u0D39\u0D60-\u0D61\u0E01-\u0E2E\u0E32-\u0E33\u0E40-\u0E45\u0E81-\u0E82") + std::wstring(L"\u0E87-\u0E88\u0E94-\u0E97\u0E99-\u0E9F\u0EA1-\u0EA3\u0EAA-\u0EAB\u0EAD-\u0EAE\u0EB2-\u0EB3\u0EC0-\u0EC4") + std::wstring(L"\u0F40-\u0F47\u0F49-\u0F69\u10A0-\u10C5\u10D0-\u10F6\u1102-\u1103\u1105-\u1107\u110B-\u110C\u110E-\u1112") + std::wstring(L"\u1154-\u1155\u115F-\u1161\u116D-\u116E\u1172-\u1173\u11AE-\u11AF\u11B7-\u11B8\u11BC-\u11C2\u1E00-\u1E9B") + std::wstring(L"\u1EA0-\u1EF9\u1F00-\u1F15\u1F18-\u1F1D\u1F20-\u1F45\u1F48-\u1F4D\u1F50-\u1F57\u1F5F-\u1F7D\u1F80-\u1FB4") + std::wstring(L"\u1FB6-\u1FBC\u1FC2-\u1FC4\u1FC6-\u1FCC\u1FD0-\u1FD3\u1FD6-\u1FDB\u1FE0-\u1FEC\u1FF2-\u1FF4\u1FF6-\u1FFC") + std::wstring(L"\u212A-\u212B\u2180-\u2182\u3041-\u3094\u30A1-\u30FA\u3105-\u312C\uAC00-\uD7A3") + std::wstring(L"\u0386\u038C\u03DA\u03DC\u03DE\u03E0\u0559\u06D5\u093D\u09B2\u0A5E\u0A8D\u0ABD\u0AE0\u0B3D\u0B9C\u0CDE\u0E30\u0E84\u0E8A") + std::wstring(L"\u0E8D\u0EA5\u0EA7\u0EB0\u0EBD\u1100\u1109\u113C\u113E\u1140\u114C\u114E\u1150\u1159\u1163\u1165\u1167\u1169\u1175\u119E") + std::wstring(L"\u11A8\u11AB\u11BA\u11EB\u11F0\u11F9\u1F59\u1F5B\u1F5D\u1FBE\u2126\u212E]"), L"Letter", L"<BaseChar>|<Ideographic>", L"NCNameChar", L"<Letter>|<Digit>|[-._]|<CombiningChar>|<Extender>", L"NameChar", L"<NCNameChar>|:", L"Nmtoken", L"<NameChar>+", L"NCName", L"(<Letter>|_)<NCNameChar>*", L"Name", L"(<Letter>|[_:])<NameChar>*", L"QName", L"(<NCName>:)?<NCName>", L"Char", L"[\t\n\r\u0020-\uD7FF\ue000-\ufffd]|[\uD800-\uDBFF][\uDC00-\uDFFF]", L"whitespacechar", L"[ \t\n\r]"};

				std::wcout << std::wstring(L"Building XML automata...") << std::endl;
				std::unordered_map<std::wstring, Automaton*> t = buildMap(xmlexps);
				putFrom(L"NCName", t);
				putFrom(L"QName", t);
				putFrom(L"Char", t);
				putFrom(L"NameChar", t);
				putFrom(L"Letter", t);
				putFrom(L"whitespacechar", t);

				put(automata, L"whitespace", ws);

				std::vector<std::wstring> uriexps = {L"digit", L"[0-9]", L"upalpha", L"[A-Z]", L"lowalpha", L"[a-z]", L"alpha", L"<lowalpha>|<upalpha>", L"alphanum", L"<alpha>|<digit>", L"hex", L"<digit>|[a-f]|[A-F]", L"escaped", L"%<hex><hex>", L"mark", L"[-_.!~*'()]", L"unreserved", L"<alphanum>|<mark>", L"reserved", L"[;/?:@&=+$,\\[\\]]", L"uric", L"<reserved>|<unreserved>|<escaped>", L"fragment", L"<uric>*", L"query", L"<uric>*", L"pchar", L"<unreserved>|<escaped>|[:@&=+$,]", L"param", L"<pchar>*", L"segment", L"<pchar>*(;<param>)*", L"path_segments", L"<segment>(/<segment>)*", L"abs_path", L"/<path_segments>", L"uric_no_slash", L"<unreserved>|<escaped>|[;?:@&=+$,]", L"opaque_part", L"<uric_no_slash><uric>*", L"port", L"<digit>*", L"IPv4address", L"(<digit>{1,3}\\.){3}<digit>{1,3}", L"hexseq", L"<hex>{1,4}(:<hex>{1,4})*", L"hexpart", L"<hexseq>|<hexseq>::<hexseq>?|::<hexseq>", L"IPv6address", L"<hexpart>(:<IPv4address>)?", L"toplabel", L"<alpha>|(<alpha>(<alphanum>|-)*<alphanum>)", L"domainlabel", L"<alphanum>|(<alphanum>(<alphanum>|-)*<alphanum>)", L"hostname", L"(<domainlabel>\\.)*<toplabel>\\.?", L"host", L"<hostname>|<IPv4address>|\\[<IPv6address>\\]", L"hostport", L"<host>(:<port>)?", L"userinfo", L"(<unreserved>|<escaped>|[;:&=+$,])*", L"server", L"((<userinfo>\\@)?<hostport>)?", L"reg_name", L"(<unreserved>|<escaped>|[$,;:@&=+])+", L"authority", L"<server>|<reg_name>", L"scheme", L"<alpha>(<alpha>|<digit>|[-+.])*", L"rel_segment", L"(<unreserved>|<escaped>|[;@&=+$,])+", L"rel_path", L"<rel_segment><abs_path>?", L"net_path", L"//<authority><abs_path>?", L"hier_part", L"(<net_path>|<abs_path>)(\\?<query>)?", L"relativeURI", L"(<net_path>|<abs_path>|<rel_path>)(\\?<query>)?", L"absoluteURI", L"<scheme>:(<hier_part>|<opaque_part>)", L"URI", L"(<absoluteURI>|<relativeURI>)?(\\#<fragment>)?"};
				std::wcout << std::wstring(L"Building URI automaton...") << std::endl;
				putFrom(L"URI", buildMap(uriexps));
				put(automata, L"anyname", Automaton::minimize(Automaton::makeChar(L'{')->concatenate(automata[L"URI"]->clone())->concatenate(Automaton::makeChar(L'}'))->optional()->concatenate(automata[L"NCName"]->clone())));

				RegExp tempVar(L"~(@[@%]@)");
				put(automata, L"noap", (&tempVar)->toAutomaton());

				std::vector<std::wstring> xsdmisc = {L"_", L"[ \t\n\r]*", L"d", L"[0-9]", L"Z", L"[-+](<00-13>:<00-59>|14:00)|Z", L"Y", L"(<d>{4,})&~(0000)", L"M", L"<01-12>", L"D", L"<01-31>", L"T", L"<00-23>:<00-59>:<00-59>|24:00:00", L"B64", L"[A-Za-z0-9+/]", L"B16", L"[AEIMQUYcgkosw048]", L"B04", L"[AQgw]", L"B04S", L"<B04> ?", L"B16S", L"<B16> ?", L"B64S", L"<B64> ?"};
				std::vector<std::wstring> xsdexps = {L"boolean", L"<_>(true|false|1|0)<_>", L"decimal", L"<_>([-+]?<d>+(\\.<d>+)?)<_>", L"float", L"<_>([-+]?<d>+(\\.<d>+)?([Ee][-+]?<d>+)?|INF|-INF|NaN)<_>", L"integer", L"<_>[-+]?[0-9]+<_>", L"duration", L"<_>(-?P(((<d>+Y)?(<d>+M)?(<d>+D)?(T(((<d>+H)?(<d>+M)?(<d>+(\\.<d>+)?S)?)&~()))?)&~()))<_>", L"dateTime", L"<_>(-?<Y>-<M>-<D>T<T>(\\.<d>+)?<Z>?)<_>", L"time", L"<_>(<T>(\\.<d>+)?<Z>?)<_>", L"date", L"<_>(-?<Y>-<M>-<D><Z>?)<_>", L"gYearMonth", L"<_>(-?<Y>-<M><Z>?)<_>", L"gYear", L"<_>(-?<Y><Z>?)<_>", L"gMonthDay", L"<_>(--<M>-<D><Z>?)<_>", L"gDay", L"<_>(--<D><Z>?)<_>", L"gMonth", L"<_>(--<M><Z>?)<_>", L"hexBinary", L"<_>([0-9a-fA-F]{2}*)<_>", L"base64Binary", L"<_>(((<B64S><B64S><B64S><B64S>)*((<B64S><B64S><B64S><B64>)|(<B64S><B64S><B16S>=)|(<B64S><B04S>= ?=)))?)<_>", L"language", L"<_>[a-zA-Z]{1,8}(-[a-zA-Z0-9]{1,8})*<_>", L"nonPositiveInteger", L"<_>(0+|-<d>+)<_>", L"negativeInteger", L"<_>(-[1-9]<d>*)<_>", L"nonNegativeInteger", L"<_>(<d>+)<_>", L"positiveInteger", L"<_>([1-9]<d>*)<_>"};
				std::wcout << std::wstring(L"Building XML Schema automata...") << std::endl;
				std::unordered_map<std::wstring, Automaton*> m = buildMap(xsdmisc);
				putWith(xsdexps, m);

				put(m, L"UNSIGNEDLONG", Automaton::makeMaxInteger(L"18446744073709551615"));
				put(m, L"UNSIGNEDINT", Automaton::makeMaxInteger(L"4294967295"));
				put(m, L"UNSIGNEDSHORT", Automaton::makeMaxInteger(L"65535"));
				put(m, L"UNSIGNEDBYTE", Automaton::makeMaxInteger(L"255"));
				put(m, L"LONG", Automaton::makeMaxInteger(L"9223372036854775807"));
				put(m, L"LONG_NEG", Automaton::makeMaxInteger(L"9223372036854775808"));
				put(m, L"INT", Automaton::makeMaxInteger(L"2147483647"));
				put(m, L"INT_NEG", Automaton::makeMaxInteger(L"2147483648"));
				put(m, L"SHORT", Automaton::makeMaxInteger(L"32767"));
				put(m, L"SHORT_NEG", Automaton::makeMaxInteger(L"32768"));
				put(m, L"BYTE", Automaton::makeMaxInteger(L"127"));
				put(m, L"BYTE_NEG", Automaton::makeMaxInteger(L"128"));

				std::unordered_map<std::wstring, Automaton*> u;
				u.putAll(t);
				u.putAll(m);
				std::vector<std::wstring> xsdexps2 = {L"Nmtoken2", L"<_><Nmtoken><_>", L"Name2", L"<_><Name><_>", L"NCName2", L"<_><NCName><_>", L"QName2", L"<_><QName><_>", L"Nmtokens", L"<_>(<Nmtoken><_>)+", L"NCNames", L"<_>(<NCName><_>)+", L"Names", L"<_>(<Name><_>)+", L"unsignedLong", L"<_><UNSIGNEDLONG><_>", L"unsignedInt", L"<_><UNSIGNEDINT><_>", L"unsignedShort", L"<_><UNSIGNEDSHORT><_>", L"unsignedByte", L"<_><UNSIGNEDBYTE><_>", L"long", L"<_>(<LONG>|-<LONG_NEG>)<_>", L"int", L"<_>(<INT>|-<INT_NEG>)<_>", L"short", L"<_>(<SHORT>|-<SHORT_NEG>)<_>", L"byte", L"<_>(<BYTE>|-<BYTE_NEG>)<_>", L"string", L"<Char>*"};
				putWith(xsdexps2, u);

				std::wcout << std::wstring(L"Building Unicode block automata...") << std::endl;
				put(automata, L"BasicLatin", Automaton::makeCharRange(L'\u0000', L'\u007F'));
				put(automata, L"Latin-1Supplement", Automaton::makeCharRange(L'\u0080', L'\u00FF'));
				put(automata, L"LatinExtended-A", Automaton::makeCharRange(L'\u0100', L'\u017F'));
				put(automata, L"LatinExtended-B", Automaton::makeCharRange(L'\u0180', L'\u024F'));
				put(automata, L"IPAExtensions", Automaton::makeCharRange(L'\u0250', L'\u02AF'));
				put(automata, L"SpacingModifierLetters", Automaton::makeCharRange(L'\u02B0', L'\u02FF'));
				put(automata, L"CombiningDiacriticalMarks", Automaton::makeCharRange(L'\u0300', L'\u036F'));
				put(automata, L"Greek", Automaton::makeCharRange(L'\u0370', L'\u03FF'));
				put(automata, L"Cyrillic", Automaton::makeCharRange(L'\u0400', L'\u04FF'));
				put(automata, L"Armenian", Automaton::makeCharRange(L'\u0530', L'\u058F'));
				put(automata, L"Hebrew", Automaton::makeCharRange(L'\u0590', L'\u05FF'));
				put(automata, L"Arabic", Automaton::makeCharRange(L'\u0600', L'\u06FF'));
				put(automata, L"Syriac", Automaton::makeCharRange(L'\u0700', L'\u074F'));
				put(automata, L"Thaana", Automaton::makeCharRange(L'\u0780', L'\u07BF'));
				put(automata, L"Devanagari", Automaton::makeCharRange(L'\u0900', L'\u097F'));
				put(automata, L"Bengali", Automaton::makeCharRange(L'\u0980', L'\u09FF'));
				put(automata, L"Gurmukhi", Automaton::makeCharRange(L'\u0A00', L'\u0A7F'));
				put(automata, L"Gujarati", Automaton::makeCharRange(L'\u0A80', L'\u0AFF'));
				put(automata, L"Oriya", Automaton::makeCharRange(L'\u0B00', L'\u0B7F'));
				put(automata, L"Tamil", Automaton::makeCharRange(L'\u0B80', L'\u0BFF'));
				put(automata, L"Telugu", Automaton::makeCharRange(L'\u0C00', L'\u0C7F'));
				put(automata, L"Kannada", Automaton::makeCharRange(L'\u0C80', L'\u0CFF'));
				put(automata, L"Malayalam", Automaton::makeCharRange(L'\u0D00', L'\u0D7F'));
				put(automata, L"Sinhala", Automaton::makeCharRange(L'\u0D80', L'\u0DFF'));
				put(automata, L"Thai", Automaton::makeCharRange(L'\u0E00', L'\u0E7F'));
				put(automata, L"Lao", Automaton::makeCharRange(L'\u0E80', L'\u0EFF'));
				put(automata, L"Tibetan", Automaton::makeCharRange(L'\u0F00', L'\u0FFF'));
				put(automata, L"Myanmar", Automaton::makeCharRange(L'\u1000', L'\u109F'));
				put(automata, L"Georgian", Automaton::makeCharRange(L'\u10A0', L'\u10FF'));
				put(automata, L"HangulJamo", Automaton::makeCharRange(L'\u1100', L'\u11FF'));
				put(automata, L"Ethiopic", Automaton::makeCharRange(L'\u1200', L'\u137F'));
				put(automata, L"Cherokee", Automaton::makeCharRange(L'\u13A0', L'\u13FF'));
				put(automata, L"UnifiedCanadianAboriginalSyllabics", Automaton::makeCharRange(L'\u1400', L'\u167F'));
				put(automata, L"Ogham", Automaton::makeCharRange(L'\u1680', L'\u169F'));
				put(automata, L"Runic", Automaton::makeCharRange(L'\u16A0', L'\u16FF'));
				put(automata, L"Khmer", Automaton::makeCharRange(L'\u1780', L'\u17FF'));
				put(automata, L"Mongolian", Automaton::makeCharRange(L'\u1800', L'\u18AF'));
				put(automata, L"LatinExtendedAdditional", Automaton::makeCharRange(L'\u1E00', L'\u1EFF'));
				put(automata, L"GreekExtended", Automaton::makeCharRange(L'\u1F00', L'\u1FFF'));
				put(automata, L"GeneralPunctuation", Automaton::makeCharRange(L'\u2000', L'\u206F'));
				put(automata, L"SuperscriptsandSubscripts", Automaton::makeCharRange(L'\u2070', L'\u209F'));
				put(automata, L"CurrencySymbols", Automaton::makeCharRange(L'\u20A0', L'\u20CF'));
				put(automata, L"CombiningMarksforSymbols", Automaton::makeCharRange(L'\u20D0', L'\u20FF'));
				put(automata, L"LetterlikeSymbols", Automaton::makeCharRange(L'\u2100', L'\u214F'));
				put(automata, L"NumberForms", Automaton::makeCharRange(L'\u2150', L'\u218F'));
				put(automata, L"Arrows", Automaton::makeCharRange(L'\u2190', L'\u21FF'));
				put(automata, L"MathematicalOperators", Automaton::makeCharRange(L'\u2200', L'\u22FF'));
				put(automata, L"MiscellaneousTechnical", Automaton::makeCharRange(L'\u2300', L'\u23FF'));
				put(automata, L"ControlPictures", Automaton::makeCharRange(L'\u2400', L'\u243F'));
				put(automata, L"OpticalCharacterRecognition", Automaton::makeCharRange(L'\u2440', L'\u245F'));
				put(automata, L"EnclosedAlphanumerics", Automaton::makeCharRange(L'\u2460', L'\u24FF'));
				put(automata, L"BoxDrawing", Automaton::makeCharRange(L'\u2500', L'\u257F'));
				put(automata, L"BlockElements", Automaton::makeCharRange(L'\u2580', L'\u259F'));
				put(automata, L"GeometricShapes", Automaton::makeCharRange(L'\u25A0', L'\u25FF'));
				put(automata, L"MiscellaneousSymbols", Automaton::makeCharRange(L'\u2600', L'\u26FF'));
				put(automata, L"Dingbats", Automaton::makeCharRange(L'\u2700', L'\u27BF'));
				put(automata, L"BraillePatterns", Automaton::makeCharRange(L'\u2800', L'\u28FF'));
				put(automata, L"CJKRadicalsSupplement", Automaton::makeCharRange(L'\u2E80', L'\u2EFF'));
				put(automata, L"KangxiRadicals", Automaton::makeCharRange(L'\u2F00', L'\u2FDF'));
				put(automata, L"IdeographicDescriptionCharacters", Automaton::makeCharRange(L'\u2FF0', L'\u2FFF'));
				put(automata, L"CJKSymbolsandPunctuation", Automaton::makeCharRange(L'\u3000', L'\u303F'));
				put(automata, L"Hiragana", Automaton::makeCharRange(L'\u3040', L'\u309F'));
				put(automata, L"Katakana", Automaton::makeCharRange(L'\u30A0', L'\u30FF'));
				put(automata, L"Bopomofo", Automaton::makeCharRange(L'\u3100', L'\u312F'));
				put(automata, L"HangulCompatibilityJamo", Automaton::makeCharRange(L'\u3130', L'\u318F'));
				put(automata, L"Kanbun", Automaton::makeCharRange(L'\u3190', L'\u319F'));
				put(automata, L"BopomofoExtended", Automaton::makeCharRange(L'\u31A0', L'\u31BF'));
				put(automata, L"EnclosedCJKLettersandMonths", Automaton::makeCharRange(L'\u3200', L'\u32FF'));
				put(automata, L"CJKCompatibility", Automaton::makeCharRange(L'\u3300', L'\u33FF'));
				put(automata, L"CJKUnifiedIdeographsExtensionA", Automaton::makeCharRange(L'\u3400', L'\u4DB5'));
				put(automata, L"CJKUnifiedIdeographs", Automaton::makeCharRange(L'\u4E00', L'\u9FFF'));
				put(automata, L"YiSyllables", Automaton::makeCharRange(L'\uA000', L'\uA48F'));
				put(automata, L"YiRadicals", Automaton::makeCharRange(L'\uA490', L'\uA4CF'));
				put(automata, L"HangulSyllables", Automaton::makeCharRange(L'\uAC00', L'\uD7A3'));
				put(automata, L"CJKCompatibilityIdeographs", Automaton::makeCharRange(L'\uF900', L'\uFAFF'));
				put(automata, L"AlphabeticPresentationForms", Automaton::makeCharRange(L'\uFB00', L'\uFB4F'));
				put(automata, L"ArabicPresentationForms-A", Automaton::makeCharRange(L'\uFB50', L'\uFDFF'));
				put(automata, L"CombiningHalfMarks", Automaton::makeCharRange(L'\uFE20', L'\uFE2F'));
				put(automata, L"CJKCompatibilityForms", Automaton::makeCharRange(L'\uFE30', L'\uFE4F'));
				put(automata, L"SmallFormVariants", Automaton::makeCharRange(L'\uFE50', L'\uFE6F'));
				put(automata, L"ArabicPresentationForms-B", Automaton::makeCharRange(L'\uFE70', L'\uFEFE'));
				put(automata, L"Specials", Automaton::makeCharRange(L'\uFEFF', L'\uFEFF'));
				put(automata, L"HalfwidthandFullwidthForms", Automaton::makeCharRange(L'\uFF00', L'\uFFEF'));
				put(automata, L"Specials", Automaton::makeCharRange(L'\uFFF0', L'\uFFFD'));

				put(automata, L"OldItalic", Automaton::makeChar(L'\ud800')->concatenate(Automaton::makeCharRange(L'\udf00', L'\udf2f')));
				put(automata, L"Gothic", Automaton::makeChar(L'\ud800')->concatenate(Automaton::makeCharRange(L'\udf30', L'\udf4f')));
				put(automata, L"Deseret", Automaton::makeChar(L'\ud801')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udc4f')));
				put(automata, L"ByzantineMusicalSymbols", Automaton::makeChar(L'\ud834')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udcff')));
				put(automata, L"MusicalSymbols", Automaton::makeChar(L'\ud834')->concatenate(Automaton::makeCharRange(L'\udd00', L'\uddff')));
				put(automata, L"MathematicalAlphanumericSymbols", Automaton::makeChar(L'\ud835')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udfff')));

				put(automata, L"CJKUnifiedIdeographsExtensionB", Automaton::makeCharRange(L'\ud840', L'\ud868')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udfff'))->union_Renamed(Automaton::makeChar(L'\ud869')->concatenate(Automaton::makeCharRange(L'\udc00', L'\uded6'))));

				put(automata, L"CJKCompatibilityIdeographsSupplement", Automaton::makeChar(L'\ud87e')->concatenate(Automaton::makeCharRange(L'\udc00', L'\ude1f')));
				put(automata, L"Tags", Automaton::makeChar(L'\udb40')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udc7f')));

				put(automata, L"PrivateUse", Automaton::makeCharRange(L'\uE000', L'\uF8FF')->union_Renamed(Automaton::makeCharRange(L'\udb80', L'\udbbe')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udfff'))->union_Renamed(Automaton::makeChar(L'\udbbf')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udffd'))))->union_Renamed(Automaton::makeCharRange(L'\udbc0', L'\udbfe')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udfff'))->union_Renamed(Automaton::makeChar(L'\udbff')->concatenate(Automaton::makeCharRange(L'\udc00', L'\udffd')))));

				std::wcout << std::wstring(L"Building Unicode category automata...") << std::endl;
				std::unordered_map<std::wstring, Set<int>*> categories;
				try
				{
					BufferedReader tempVar2(new FileReader(L"src/Unicode.txt"));
					StreamTokenizer *st = new StreamTokenizer(&tempVar2);
					st->resetSyntax();
					st->whitespaceChars(L';', L';');
					st->whitespaceChars(L'\n', L' ');
					st->wordChars(L'0', L'9');
					st->wordChars(L'a', L'z');
					st->wordChars(L'A', L'Z');
					while (st->nextToken() != StreamTokenizer::TT_EOF)
					{
						int cp = Integer::valueOf(st->sval, 16);
						st->nextToken();
						std::wstring cat = st->sval;
						Set<int> *c = categories[cat];
						if (c == nullptr)
						{
							c = std::set<int>();
							categories[cat] = c;
						}
						c->add(cp);
						std::wstring ccat = cat.substr(0, 1);
						c = categories[ccat];
						if (c == nullptr)
						{
							c = std::set<int>();
							categories[ccat] = c;
						}
						c->add(cp);
					}
				}
				catch (const IOException &e)
				{
					e->printStackTrace();
					exit(-1);
				}
				std::vector<Automaton*> assigned;
				for (auto me : categories)
				{
					std::vector<Automaton*> la1;
					std::vector<Automaton*> la2;
					for (* : :optional<int> cp : me->getValue())
					{
						la1.push_back(makeCodePoint(cp));
						if (la1.size() == 50)
						{
							la2.push_back(Automaton::minimize(Automaton::union_Renamed(la1)));
							la1.clear();
						}
					}
					la2.push_back(Automaton::union_Renamed(la1));
					Automaton *a = Automaton::minimize(Automaton::union_Renamed(la2));
					put(automata, me->getKey(), a);
					assigned.push_back(a);
				}
				Automaton *cn = Automaton::minimize(automata[L"Char"]->clone().intersection(Automaton::union_Renamed(assigned)->complement()));
				put(automata, L"Cn", cn);
				put(automata, L"C", automata[L"C"]->clone().union_Renamed(cn));
			}

			Automaton *Datatypes::makeCodePoint(int cp)
			{
				if (cp >= 0x10000)
				{
					cp -= 0x10000;
					std::vector<wchar_t> cu = {static_cast<wchar_t>(0xd800 + (cp >> 10)), static_cast<wchar_t>(0xdc00 + (cp & 0x3ff))};
					return Automaton::makeString(std::wstring(cu));
				}
				else
				{
					return Automaton::makeChar(static_cast<wchar_t>(cp));
				}
			}

			std::unordered_map<std::wstring, Automaton*> Datatypes::buildMap(std::vector<std::wstring> &exps)
			{
				std::unordered_map<std::wstring, Automaton*> map;
				int i = 0;
				while (i + 1 < exps.size())
				{
					RegExp tempVar(exps[i++]);
					put(map, exps[i++], (&tempVar)->toAutomaton(map));
				}
				return map;
			}

			void Datatypes::putWith(std::vector<std::wstring> &exps, std::unordered_map<std::wstring, Automaton*> &use)
			{
				int i = 0;
				while (i + 1 < exps.size())
				{
					RegExp tempVar(exps[i++]);
					put(automata, exps[i++], (&tempVar)->toAutomaton(use));
				}
			}

			void Datatypes::putFrom(const std::wstring &name, std::unordered_map<std::wstring, Automaton*> &from)
			{
				automata[name] = from[name];
			}

			void Datatypes::put(std::unordered_map<std::wstring, Automaton*> &map, const std::wstring &name, Automaton *a)
			{
				map[name] = a;
				std::wcout << std::wstring(L"  ") << name << std::wstring(L": ") << a->getNumberOfStates() << std::wstring(L" states, ") << a->getNumberOfTransitions() << std::wstring(L" transitions") << std::endl;
			}

			Automaton *Datatypes::getWhitespaceAutomaton()
			{
				return ws;
			}
		}
	}
}
