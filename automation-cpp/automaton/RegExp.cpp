#include "RegExp.h"
#include "Automaton.h"
#include "AutomatonProvider.h"
#include "BasicOperations.h"
#include "BasicAutomata.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

bool RegExp::allow_mutation = false;

			RegExp::RegExp()
			{
			}

			RegExp::RegExp(const std::wstring &s) throw(std::invalid_argument) : RegExp(s, ALL)
			{
			}

			RegExp::RegExp(const std::wstring &s, int syntax_flags) throw(std::invalid_argument)
			{
				b = s;
				flags = syntax_flags;
				RegExp *e;
				if (s.length() == 0)
				{
					e = makeString(L"");
				}
				else
				{
					e = parseUnionExp();
					if (pos < b.length())
					{
						throw std::invalid_argument(std::wstring(L"end-of-string expected at position ") + std::to_wstring(pos));
					}
				}
				kind = e->kind;
				exp1 = e->exp1;
				exp2 = e->exp2;
				this->s = e->s;
				c = e->c;
				min = e->min;
				max = e->max;
				digits = e->digits;
				from = e->from;
				to = e->to;
				b = L"";
			}

			Automaton *RegExp::toAutomaton()
			{
				return toAutomatonAllowMutate(nullptr, nullptr, true);
			}

			Automaton *RegExp::toAutomaton(bool minimize)
			{
				return toAutomatonAllowMutate(nullptr, nullptr, minimize);
			}

			Automaton *RegExp::toAutomaton(AutomatonProvider *automaton_provider) throw(std::invalid_argument)
			{
				return toAutomatonAllowMutate(nullptr, automaton_provider, true);
			}

			Automaton *RegExp::toAutomaton(AutomatonProvider *automaton_provider, bool minimize) throw(std::invalid_argument)
			{
				return toAutomatonAllowMutate(nullptr, automaton_provider, minimize);
			}

			Automaton *RegExp::toAutomaton(std::unordered_map<std::wstring, Automaton*> &automata) throw(std::invalid_argument)
			{
				return toAutomatonAllowMutate(automata, nullptr, true);
			}

			Automaton *RegExp::toAutomaton(std::unordered_map<std::wstring, Automaton*> &automata, bool minimize) throw(std::invalid_argument)
			{
				return toAutomatonAllowMutate(automata, nullptr, minimize);
			}

			bool RegExp::setAllowMutate(bool flag)
			{
				bool b = allow_mutation;
				allow_mutation = flag;
				return b;
			}

			Automaton *RegExp::toAutomatonAllowMutate(std::unordered_map<std::wstring, Automaton*> &automata, AutomatonProvider *automaton_provider, bool minimize) throw(std::invalid_argument)
			{
				bool b = false;
				if (allow_mutation)
				{
					b = Automaton::setAllowMutate(true); // thread unsafe
				}
				Automaton *a = toAutomaton(automata, automaton_provider, minimize);
				if (allow_mutation)
				{
					Automaton::setAllowMutate(b);
				}
				return a;
			}

			Automaton *RegExp::toAutomaton(std::unordered_map<std::wstring, Automaton*> &automata, AutomatonProvider *automaton_provider, bool minimize) throw(std::invalid_argument)
			{
				std::vector<Automaton*> list;
				Automaton *a = nullptr;
				switch (kind)
				{
				case REGEXP_UNION:
					list = std::vector<Automaton*>();
					findLeaves(exp1, Kind::REGEXP_UNION, list, automata, automaton_provider, minimize);
					findLeaves(exp2, Kind::REGEXP_UNION, list, automata, automaton_provider, minimize);
					a = BasicOperations::union_Renamed(list);
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_CONCATENATION:
					list = std::vector<Automaton*>();
					findLeaves(exp1, Kind::REGEXP_CONCATENATION, list, automata, automaton_provider, minimize);
					findLeaves(exp2, Kind::REGEXP_CONCATENATION, list, automata, automaton_provider, minimize);
					a = BasicOperations::concatenate(list);
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_INTERSECTION:
					a = exp1->toAutomaton(automata, automaton_provider, minimize)->intersection(exp2->toAutomaton(automata, automaton_provider, minimize));
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_OPTIONAL:
					a = exp1->toAutomaton(automata, automaton_provider, minimize)->optional();
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_REPEAT:
					a = exp1->toAutomaton(automata, automaton_provider, minimize)->repeat();
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_REPEAT_MIN:
					a = exp1->toAutomaton(automata, automaton_provider, minimize)->repeat(min);
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_REPEAT_MINMAX:
					a = exp1->toAutomaton(automata, automaton_provider, minimize)->repeat(min, max);
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_COMPLEMENT:
					a = exp1->toAutomaton(automata, automaton_provider, minimize)->complement();
					if (minimize)
					{
						a->minimize();
					}
					break;
				case REGEXP_CHAR:
					a = BasicAutomata::makeChar(c);
					break;
				case REGEXP_CHAR_RANGE:
					a = BasicAutomata::makeCharRange(from, to);
					break;
				case REGEXP_ANYCHAR:
					a = BasicAutomata::makeAnyChar();
					break;
				case REGEXP_EMPTY:
					a = BasicAutomata::makeEmpty();
					break;
				case REGEXP_STRING:
					a = BasicAutomata::makeString(s);
					break;
				case REGEXP_ANYSTRING:
					a = BasicAutomata::makeAnyString();
					break;
				case REGEXP_AUTOMATON:
				{
					Automaton *aa = nullptr;
					if (automata.size() > 0)
					{
						aa = automata[s];
					}
					if (aa == nullptr && automaton_provider != nullptr)
					{
						try
						{
							aa = automaton_provider->getAutomaton(s);
						}
						catch (const IOException &e)
						{
							throw std::invalid_argument(e);
						}
					}
					if (aa == nullptr)
					{
						throw std::invalid_argument(std::wstring(L"'") + s + std::wstring(L"' not found"));
					}
					a = aa->clone(); // always clone here (ignore allow_mutate)
					break;
				}
				case REGEXP_INTERVAL:
					a = BasicAutomata::makeInterval(min, max, digits);
					break;
				}
				return a;
			}

			void RegExp::findLeaves(RegExp *exp, Kind kind, std::vector<Automaton*> &list, std::unordered_map<std::wstring, Automaton*> &automata, AutomatonProvider *automaton_provider, bool minimize)
			{
				if (exp->kind == kind)
				{
					findLeaves(exp->exp1, kind, list, automata, automaton_provider, minimize);
					findLeaves(exp->exp2, kind, list, automata, automaton_provider, minimize);
				}
				else
				{
					list.push_back(exp->toAutomaton(automata, automaton_provider, minimize));
				}
			}

			std::wstring RegExp::toString()
			{
				StringBuilder tempVar();
				return toStringBuilder(&tempVar)->toString();
			}

			StringBuilder *RegExp::toStringBuilder(StringBuilder *b)
			{
				switch (kind)
				{
				case REGEXP_UNION:
					b->append(L"(");
					exp1->toStringBuilder(b);
					b->append(L"|");
					exp2->toStringBuilder(b);
					b->append(L")");
					break;
				case REGEXP_CONCATENATION:
					exp1->toStringBuilder(b);
					exp2->toStringBuilder(b);
					break;
				case REGEXP_INTERSECTION:
					b->append(L"(");
					exp1->toStringBuilder(b);
					b->append(L"&");
					exp2->toStringBuilder(b);
					b->append(L")");
					break;
				case REGEXP_OPTIONAL:
					b->append(L"(");
					exp1->toStringBuilder(b);
					b->append(L")?");
					break;
				case REGEXP_REPEAT:
					b->append(L"(");
					exp1->toStringBuilder(b);
					b->append(L")*");
					break;
				case REGEXP_REPEAT_MIN:
					b->append(L"(");
					exp1->toStringBuilder(b);
					b->append(L"){")->append(min)->append(L",}");
					break;
				case REGEXP_REPEAT_MINMAX:
					b->append(L"(");
					exp1->toStringBuilder(b);
					b->append(L"){")->append(min)->append(L",")->append(max)->append(L"}");
					break;
				case REGEXP_COMPLEMENT:
					b->append(L"~(");
					exp1->toStringBuilder(b);
					b->append(L")");
					break;
				case REGEXP_CHAR:
					appendChar(c, b);
					break;
				case REGEXP_CHAR_RANGE:
					b->append(L"[\\")->append(from)->append(L"-\\")->append(to)->append(L"]");
					break;
				case REGEXP_ANYCHAR:
					b->append(L".");
					break;
				case REGEXP_EMPTY:
					b->append(L"#");
					break;
				case REGEXP_STRING:
					if (s.find(L'"') == std::wstring::npos)
					{
						b->append(L"\"")->append(s)->append(L"\"");
					}
					else
					{
						for (int i = 0; i < s.length(); i++)
						{
							appendChar(s[i], b);
						}
					}
					break;
				case REGEXP_ANYSTRING:
					b->append(L"@");
					break;
				case REGEXP_AUTOMATON:
					b->append(L"<")->append(s)->append(L">");
					break;
				case REGEXP_INTERVAL:
				{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
					std::wstring s1 = Integer::toString(min);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
					std::wstring s2 = Integer::toString(max);
					b->append(L"<");
					if (digits > 0)
					{
						for (int i = s1.length(); i < digits; i++)
						{
							b->append(L'0');
						}
					}
					b->append(s1)->append(L"-");
					if (digits > 0)
					{
						for (int i = s2.length(); i < digits; i++)
						{
							b->append(L'0');
						}
					}
					b->append(s2)->append(L">");
					break;
				}
				}
				return b;
			}

			void RegExp::appendChar(wchar_t c, StringBuilder *b)
			{
				if ((std::wstring(L"|&?*+{},![]^-.#@\"()<>\\")).find(c) != std::wstring::npos)
				{
					b->append(L"\\");
				}
				b->append(c);
			}

			Set<std::wstring> *RegExp::getIdentifiers()
			{
				std::unordered_set<std::wstring> set;
				getIdentifiers(set);
				return set;
			}

			void RegExp::getIdentifiers(Set<std::wstring> *set)
			{
				switch (kind)
				{
				case REGEXP_UNION:
				case REGEXP_CONCATENATION:
				case REGEXP_INTERSECTION:
					exp1->getIdentifiers(set);
					exp2->getIdentifiers(set);
					break;
				case REGEXP_OPTIONAL:
				case REGEXP_REPEAT:
				case REGEXP_REPEAT_MIN:
				case REGEXP_REPEAT_MINMAX:
				case REGEXP_COMPLEMENT:
					exp1->getIdentifiers(set);
					break;
				case REGEXP_AUTOMATON:
					set->add(s);
					break;
				default:
				}
			}

			RegExp *RegExp::makeUnion(RegExp *exp1, RegExp *exp2)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_UNION;
				r->exp1 = exp1;
				r->exp2 = exp2;
				return r;
			}

			RegExp *RegExp::makeConcatenation(RegExp *exp1, RegExp *exp2)
			{
				if ((exp1->kind == Kind::REGEXP_CHAR || exp1->kind == Kind::REGEXP_STRING) && (exp2->kind == Kind::REGEXP_CHAR || exp2->kind == Kind::REGEXP_STRING))
				{
					return makeString(exp1, exp2);
				}
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_CONCATENATION;
				if (exp1->kind == Kind::REGEXP_CONCATENATION && (exp1->exp2->kind == Kind::REGEXP_CHAR || exp1->exp2->kind == Kind::REGEXP_STRING) && (exp2->kind == Kind::REGEXP_CHAR || exp2->kind == Kind::REGEXP_STRING))
				{
					r->exp1 = exp1->exp1;
					r->exp2 = makeString(exp1->exp2, exp2);
				}
				else if ((exp1->kind == Kind::REGEXP_CHAR || exp1->kind == Kind::REGEXP_STRING) && exp2->kind == Kind::REGEXP_CONCATENATION && (exp2->exp1->kind == Kind::REGEXP_CHAR || exp2->exp1->kind == Kind::REGEXP_STRING))
				{
					r->exp1 = makeString(exp1, exp2->exp1);
					r->exp2 = exp2->exp2;
				}
				else
				{
					r->exp1 = exp1;
					r->exp2 = exp2;
				}
				return r;
			}

			RegExp *RegExp::makeString(RegExp *exp1, RegExp *exp2)
			{
				StringBuilder *b = new StringBuilder();
				if (exp1->kind == Kind::REGEXP_STRING)
				{
					b->append(exp1->s);
				}
				else
				{
					b->append(exp1->c);
				}
				if (exp2->kind == Kind::REGEXP_STRING)
				{
					b->append(exp2->s);
				}
				else
				{
					b->append(exp2->c);
				}
				return makeString(b->toString());
			}

			RegExp *RegExp::makeIntersection(RegExp *exp1, RegExp *exp2)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_INTERSECTION;
				r->exp1 = exp1;
				r->exp2 = exp2;
				return r;
			}

			RegExp *RegExp::makeOptional(RegExp *exp)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_OPTIONAL;
				r->exp1 = exp;
				return r;
			}

			RegExp *RegExp::makeRepeat(RegExp *exp)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_REPEAT;
				r->exp1 = exp;
				return r;
			}

			RegExp *RegExp::makeRepeat(RegExp *exp, int min)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_REPEAT_MIN;
				r->exp1 = exp;
				r->min = min;
				return r;
			}

			RegExp *RegExp::makeRepeat(RegExp *exp, int min, int max)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_REPEAT_MINMAX;
				r->exp1 = exp;
				r->min = min;
				r->max = max;
				return r;
			}

			RegExp *RegExp::makeComplement(RegExp *exp)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_COMPLEMENT;
				r->exp1 = exp;
				return r;
			}

			RegExp *RegExp::makeChar(wchar_t c)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_CHAR;
				r->c = c;
				return r;
			}

			RegExp *RegExp::makeCharRange(wchar_t from, wchar_t to)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_CHAR_RANGE;
				r->from = from;
				r->to = to;
				return r;
			}

			RegExp *RegExp::makeAnyChar()
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_ANYCHAR;
				return r;
			}

			RegExp *RegExp::makeEmpty()
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_EMPTY;
				return r;
			}

			RegExp *RegExp::makeString(const std::wstring &s)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_STRING;
				r->s = s;
				return r;
			}

			RegExp *RegExp::makeAnyString()
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_ANYSTRING;
				return r;
			}

			RegExp *RegExp::makeAutomaton(const std::wstring &s)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_AUTOMATON;
				r->s = s;
				return r;
			}

			RegExp *RegExp::makeInterval(int min, int max, int digits)
			{
				RegExp *r = new RegExp();
				r->kind = Kind::REGEXP_INTERVAL;
				r->min = min;
				r->max = max;
				r->digits = digits;
				return r;
			}

			bool RegExp::peek(const std::wstring &s)
			{
				return more() && s.find(b[pos]) != std::wstring::npos;
			}

			bool RegExp::match(wchar_t c)
			{
				if (pos >= b.length())
				{
					return false;
				}
				if (b[pos] == c)
				{
					pos++;
					return true;
				}
				return false;
			}

			bool RegExp::more()
			{
				return pos < b.length();
			}

			wchar_t RegExp::next() throw(std::invalid_argument)
			{
				if (!more())
				{
					throw std::invalid_argument("unexpected end-of-string");
				}
				return b[pos++];
			}

			bool RegExp::check(int flag)
			{
				return (flags & flag) != 0;
			}

			RegExp *RegExp::parseUnionExp() throw(std::invalid_argument)
			{
				RegExp *e = parseInterExp();
				if (match(L'|'))
				{
					e = makeUnion(e, parseUnionExp());
				}
				return e;
			}

			RegExp *RegExp::parseInterExp() throw(std::invalid_argument)
			{
				RegExp *e = parseConcatExp();
				if (check(INTERSECTION) && match(L'&'))
				{
					e = makeIntersection(e, parseInterExp());
				}
				return e;
			}

			RegExp *RegExp::parseConcatExp() throw(std::invalid_argument)
			{
				RegExp *e = parseRepeatExp();
				if (more() && !peek(L")|") && (!check(INTERSECTION) || !peek(L"&")))
				{
					e = makeConcatenation(e, parseConcatExp());
				}
				return e;
			}

			RegExp *RegExp::parseRepeatExp() throw(std::invalid_argument)
			{
				RegExp *e = parseComplExp();
				while (peek(L"?*+{"))
				{
					if (match(L'?'))
					{
						e = makeOptional(e);
					}
					else if (match(L'*'))
					{
						e = makeRepeat(e);
					}
					else if (match(L'+'))
					{
						e = makeRepeat(e, 1);
					}
					else if (match(L'{'))
					{
						int start = pos;
						while (peek(L"0123456789"))
						{
							next();
						}
						if (start == pos)
						{
							throw std::invalid_argument(std::wstring(L"integer expected at position ") + std::to_wstring(pos));
						}
						int n = std::stoi(b.substr(start, pos - start));
						int m = -1;
						if (match(L','))
						{
							start = pos;
							while (peek(L"0123456789"))
							{
								next();
							}
							if (start != pos)
							{
								m = std::stoi(b.substr(start, pos - start));
							}
						}
						else
						{
							m = n;
						}
						if (!match(L'}'))
						{
							throw std::invalid_argument(std::wstring(L"expected '}' at position ") + std::to_wstring(pos));
						}
						if (m == -1)
						{
							e = makeRepeat(e, n);
						}
						else
						{
							e = makeRepeat(e, n, m);
						}
					}
				}
				return e;
			}

			RegExp *RegExp::parseComplExp() throw(std::invalid_argument)
			{
				if (check(COMPLEMENT) && match(L'~'))
				{
					return makeComplement(parseComplExp());
				}
				else
				{
					return parseCharClassExp();
				}
			}

			RegExp *RegExp::parseCharClassExp() throw(std::invalid_argument)
			{
				if (match(L'['))
				{
					bool negate = false;
					if (match(L'^'))
					{
						negate = true;
					}
					RegExp *e = parseCharClasses();
					if (negate)
					{
						e = makeIntersection(makeAnyChar(), makeComplement(e));
					}
					if (!match(L']'))
					{
						throw std::invalid_argument(std::wstring(L"expected ']' at position ") + std::to_wstring(pos));
					}
					return e;
				}
				else
				{
					return parseSimpleExp();
				}
			}

			RegExp *RegExp::parseCharClasses() throw(std::invalid_argument)
			{
				RegExp *e = parseCharClass();
				while (more() && !peek(L"]"))
				{
					e = makeUnion(e, parseCharClass());
				}
				return e;
			}

			RegExp *RegExp::parseCharClass() throw(std::invalid_argument)
			{
				wchar_t c = parseCharExp();
				if (match(L'-'))
				{
					if (peek(L"]"))
					{
						return makeUnion(makeChar(c), makeChar(L'-'));
					}
					else
					{
						return makeCharRange(c, parseCharExp());
					}
				}
				else
				{
					return makeChar(c);
				}
			}

			RegExp *RegExp::parseSimpleExp() throw(std::invalid_argument)
			{
				if (match(L'.'))
				{
					return makeAnyChar();
				}
				else if (check(EMPTY) && match(L'#'))
				{
					return makeEmpty();
				}
				else if (check(ANYSTRING) && match(L'@'))
				{
					return makeAnyString();
				}
				else if (match(L'"'))
				{
					int start = pos;
					while (more() && !peek(L"\""))
					{
						next();
					}
					if (!match(L'"'))
					{
						throw std::invalid_argument(std::wstring(L"expected '\"' at position ") + std::to_wstring(pos));
					}
					return makeString(b.substr(start, (pos - 1) - start));
				}
				else if (match(L'('))
				{
					if (match(L')'))
					{
						return makeString(L"");
					}
					RegExp *e = parseUnionExp();
					if (!match(L')'))
					{
						throw std::invalid_argument(std::wstring(L"expected ')' at position ") + std::to_wstring(pos));
					}
					return e;
				}
				else if ((check(AUTOMATON) || check(INTERVAL)) && match(L'<'))
				{
					int start = pos;
					while (more() && !peek(L">"))
					{
						next();
					}
					if (!match(L'>'))
					{
						throw std::invalid_argument(std::wstring(L"expected '>' at position ") + std::to_wstring(pos));
					}
					std::wstring s = b.substr(start, (pos - 1) - start);
					int i = (int)s.find(L'-');
					if (i == -1)
					{
						if (!check(AUTOMATON))
						{
							throw std::invalid_argument(std::wstring(L"interval syntax error at position ") + std::to_wstring(pos - 1));
						}
						return makeAutomaton(s);
					}
					else
					{
						if (!check(INTERVAL))
						{
							throw std::invalid_argument(std::wstring(L"illegal identifier at position ") + std::to_wstring(pos - 1));
						}
						try
						{
							if (i == 0 || i == s.length() - 1 || i != (int)s.rfind(L'-'))
							{
								throw NumberFormatException();
							}
							std::wstring smin = s.substr(0, i);
							std::wstring smax = s.substr(i + 1, s.length() - (i + 1));
							int imin = std::stoi(smin);
							int imax = std::stoi(smax);
							int digits;
							if (smin.length() == smax.length())
							{
								digits = smin.length();
							}
							else
							{
								digits = 0;
							}
							if (imin > imax)
							{
								int t = imin;
								imin = imax;
								imax = t;
							}
							return makeInterval(imin, imax, digits);
						}
						catch (const NumberFormatException &e)
						{
							throw std::invalid_argument(std::wstring(L"interval syntax error at position ") + std::to_wstring(pos - 1));
						}
					}
				}
				else
				{
					return makeChar(parseCharExp());
				}
			}

			wchar_t RegExp::parseCharExp() throw(std::invalid_argument)
			{
				match(L'\\');
				return next();
			}
		}
	}
}
