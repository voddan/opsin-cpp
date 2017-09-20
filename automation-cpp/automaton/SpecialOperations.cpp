#include "SpecialOperations.h"
#include "Automaton.h"
#include "State.h"
#include "Transition.h"
#include "BasicAutomata.h"
#include "StatePair.h"
#include "Datatypes.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

			SpecialOperations::SpecialOperations()
			{
			}

			Set<State*> *SpecialOperations::reverse(Automaton *a)
			{
				// reverse all edges
				std::unordered_map<State*, std::unordered_set<Transition*>> m;
				Set<State*> *states = a->getStates();
				Set<State*> *accept = a->getAcceptStates();
				for (auto r : states)
				{
					m[r] = std::unordered_set<Transition*>();
					r->accept = false;
				}
				for (auto r : states)
				{
					for (auto t : r->getTransitions())
					{
						Transition tempVar(t->min, t->max, r);
						m[t->to].insert(&tempVar);
					}
				}
				for (auto r : states)
				{
					r->transitions = m[r];
				}
				// make new initial+final states
				a->initial->accept = true;
				a->initial = new State();
				for (auto r : accept)
				{
					a->initial->addEpsilon(r); // ensures that all initial states are reachable
				}
				a->deterministic = false;
				return accept;
			}

			Automaton *SpecialOperations::overlap(Automaton *a1, Automaton *a2)
			{
				Automaton *b1 = a1->cloneExpanded();
				b1->determinize();
				acceptToAccept(b1);
				Automaton *b2 = a2->cloneExpanded();
				reverse(b2);
				b2->determinize();
				acceptToAccept(b2);
				reverse(b2);
				b2->determinize();
				return b1->intersection(b2)->minus(BasicAutomata::makeEmptyString());
			}

			void SpecialOperations::acceptToAccept(Automaton *a)
			{
				State *s = new State();
				for (auto r : a->getAcceptStates())
				{
					s->addEpsilon(r);
				}
				a->initial = s;
				a->deterministic = false;
			}

			Automaton *SpecialOperations::singleChars(Automaton *a)
			{
				Automaton *b = new Automaton();
				State *s = new State();
				b->initial = s;
				State *q = new State();
				q->accept = true;
				if (a->isSingleton())
				{
					for (int i = 0; i < a->singleton.length(); i++)
					{
						Transition tempVar(a->singleton[i], q);
						s->transitions->add(&tempVar);
					}
				}
				else
				{
					for (auto p : a->getStates())
					{
						for (auto t : p->transitions)
						{
							Transition tempVar2(t->min, t->max, q);
							s->transitions->add(&tempVar2);
						}
					}
				}
				b->deterministic = true;
				b->removeDeadTransitions();
				return b;
			}

			Automaton *SpecialOperations::trim(Automaton *a, const std::wstring &set, wchar_t c)
			{
				a = a->cloneExpandedIfRequired();
				State *f = new State();
				addSetTransitions(f, set, f);
				f->accept = true;
				for (auto s : a->getStates())
				{
					State *r = s->step(c);
					if (r != nullptr)
					{
						// add inner
						State *q = new State();
						addSetTransitions(q, set, q);
						addSetTransitions(s, set, q);
						q->addEpsilon(r);
					}
					// add postfix
					if (s->accept)
					{
						s->addEpsilon(f);
					}
				}
				// add prefix
				State *p = new State();
				addSetTransitions(p, set, p);
				p->addEpsilon(a->initial);
				a->initial = p;
				a->deterministic = false;
				a->removeDeadTransitions();
				a->checkMinimizeAlways();
				return a;
			}

			void SpecialOperations::addSetTransitions(State *s, const std::wstring &set, State *p)
			{
				for (int n = 0; n < set.length(); n++)
				{
					Transition tempVar(set[n], p);
					s->transitions->add(&tempVar);
				}
			}

			Automaton *SpecialOperations::compress(Automaton *a, const std::wstring &set, wchar_t c)
			{
				a = a->cloneExpandedIfRequired();
				for (auto s : a->getStates())
				{
					State *r = s->step(c);
					if (r != nullptr)
					{
						// add inner
						State *q = new State();
						addSetTransitions(q, set, q);
						addSetTransitions(s, set, q);
						q->addEpsilon(r);
					}
				}
				// add prefix
				a->deterministic = false;
				a->removeDeadTransitions();
				a->checkMinimizeAlways();
				return a;
			}

			Automaton *SpecialOperations::subst(Automaton *a, std::unordered_map<wchar_t, Set<wchar_t>*> &map)
			{
				if (map.empty())
				{
					return a->cloneIfRequired();
				}
				Set<wchar_t> *ckeys = std::set<wchar_t>(map.keySet());
				std::vector<wchar_t> keys(ckeys->size());
				int j = 0;
				for (* : :optional<wchar_t> c : ckeys)
				{
					keys[j++] = c;
				}
				a = a->cloneExpandedIfRequired();
				for (auto s : a->getStates())
				{
					Set<Transition*> *st = s->transitions;
					s->resetTransitions();
					for (auto t : st)
					{
						int index = findIndex(t->min, keys);
						while (t->min <= t->max)
						{
							if (keys[index] > t->min)
							{
								wchar_t m = static_cast<wchar_t>(keys[index] - 1);
								if (t->max < m)
								{
									m = t->max;
								}
								Transition tempVar(t->min, m, t->to);
								s->transitions->add(&tempVar);
								if (m + 1 > std::numeric_limits<wchar_t>::max())
								{
									break;
								}
								t->min = static_cast<wchar_t>(m + 1);
							}
							else if (keys[index] < t->min)
							{
								wchar_t m;
								if (index + 1 < keys.size())
								{
									m = static_cast<wchar_t>(keys[++index] - 1);
								}
								else
								{
									m = std::numeric_limits<wchar_t>::max();
								}
								if (t->max < m)
								{
									m = t->max;
								}
								Transition tempVar2(t->min, m, t->to);
								s->transitions->add(&tempVar2);
								if (m + 1 > std::numeric_limits<wchar_t>::max())
								{
									break;
								}
								t->min = static_cast<wchar_t>(m + 1);
							}
							else
							{ // found t.min in substitution map
								for (* : :optional<wchar_t> c : map[t->min])
								{
									Transition tempVar3(c, t->to);
									s->transitions->add(&tempVar3);
								}
								if (t->min + 1 > std::numeric_limits<wchar_t>::max())
								{
									break;
								}
								t->min++;
								if (index + 1 < keys.size() && keys[index + 1] == t->min)
								{
									index++;
								}
							}
						}
					}
				}
				a->deterministic = false;
				a->removeDeadTransitions();
				a->checkMinimizeAlways();
				return a;
			}

			int SpecialOperations::findIndex(wchar_t c, std::vector<wchar_t> &points)
			{
				int a = 0;
				int b = points.size();
				while (b - a > 1)
				{
					int d = static_cast<int>(static_cast<unsigned int>((a + b)) >> 1);
					if (points[d] > c)
					{
						b = d;
					}
					else if (points[d] < c)
					{
						a = d;
					}
					else
					{
						return d;
					}
				}
				return a;
			}

			Automaton *SpecialOperations::subst(Automaton *a, wchar_t c, const std::wstring &s)
			{
				a = a->cloneExpandedIfRequired();
				Set<StatePair*> *epsilons = std::unordered_set<StatePair*>();
				for (auto p : a->getStates())
				{
					Set<Transition*> *st = p->transitions;
					p->resetTransitions();
					for (auto t : st)
					{
						if (t->max < c || t->min > c)
						{
							p->transitions->add(t);
						}
						else
						{
							if (t->min < c)
							{
								Transition tempVar(t->min, static_cast<wchar_t>(c - 1), t->to);
								p->transitions->add(&tempVar);
							}
							if (t->max > c)
							{
								Transition tempVar2(static_cast<wchar_t>(c + 1), t->max, t->to);
								p->transitions->add(&tempVar2);
							}
							if (s.length() == 0)
							{
								StatePair tempVar3(p, t->to);
								epsilons->add(&tempVar3);
							}
							else
							{
								State *q = p;
								for (int i = 0; i < s.length(); i++)
								{
									State *r;
									if (i + 1 == s.length())
									{
										r = t->to;
									}
									else
									{
										r = new State();
									}
									Transition tempVar4(s[i], r);
									q->transitions->add(&tempVar4);
									q = r;
								}
							}
						}
					}
				}
				a->addEpsilons(epsilons);
				a->deterministic = false;
				a->removeDeadTransitions();
				a->checkMinimizeAlways();
				return a;
			}

			Automaton *SpecialOperations::homomorph(Automaton *a, std::vector<wchar_t> &source, std::vector<wchar_t> &dest)
			{
				a = a->cloneExpandedIfRequired();
				for (auto s : a->getStates())
				{
					Set<Transition*> *st = s->transitions;
					s->resetTransitions();
					for (auto t : st)
					{
						int min = t->min;
						while (min <= t->max)
						{
							int n = findIndex(static_cast<wchar_t>(min), source);
							wchar_t nmin = static_cast<wchar_t>(dest[n] + min - source[n]);
							int end = (n + 1 == source.size()) ? std::numeric_limits<wchar_t>::max() : source[n + 1] - 1;
							int length;
							if (end < t->max)
							{
								length = end + 1 - min;
							}
							else
							{
								length = t->max + 1 - min;
							}
							Transition tempVar(nmin, static_cast<wchar_t>(nmin + length - 1), t->to);
							s->transitions->add(&tempVar);
							min += length;
						}
					}
				}
				a->deterministic = false;
				a->removeDeadTransitions();
				a->checkMinimizeAlways();
				return a;
			}

			Automaton *SpecialOperations::projectChars(Automaton *a, Set<wchar_t> *chars)
			{
				std::vector<boost::optional<wchar_t>> c = chars->toArray(std::vector<boost::optional<wchar_t>*>(chars->size()));
				std::vector<wchar_t> cc(c.size());
				bool normalchars = false;
				for (int i = 0; i < c.size(); i++)
				{
					if (!c[i])
					{
						normalchars = true;
					}
					else
					{
						cc[i] = c[i];
					}
				}
				Arrays::sort(cc);
				if (a->isSingleton())
				{
					for (int i = 0; i < a->singleton.length(); i++)
					{
						wchar_t sc = a->singleton[i];
						if (!(normalchars && (sc <= L'\udfff' || sc >= L'\uf900') || Arrays::binarySearch(cc, sc) >= 0))
						{
							return BasicAutomata::makeEmpty();
						}
					}
					return a->cloneIfRequired();
				}
				else
				{
					std::unordered_set<StatePair*> epsilons;
					a = a->cloneExpandedIfRequired();
					for (auto s : a->getStates())
					{
						std::unordered_set<Transition*> new_transitions;
						for (auto t : s->transitions)
						{
							bool addepsilon = false;
							if (t->min < L'\uf900' && t->max > L'\udfff')
							{
								int w1 = Arrays::binarySearch(cc, t->min > L'\ue000' ? t->min : L'\ue000');
								if (w1 < 0)
								{
									w1 = -w1 - 1;
									addepsilon = true;
								}
								int w2 = Arrays::binarySearch(cc, t->max < L'\uf8ff' ? t->max : L'\uf8ff');
								if (w2 < 0)
								{
									w2 = -w2 - 2;
									addepsilon = true;
								}
								for (int w = w1; w <= w2; w++)
								{
									Transition tempVar(cc[w], t->to);
									new_transitions.insert(&tempVar);
									if (w > w1 && cc[w - 1] + 1 != cc[w])
									{
										addepsilon = true;
									}
								}
							}
							if (normalchars)
							{
								if (t->min <= L'\udfff')
								{
									Transition tempVar2(t->min, t->max < L'\udfff' ? t->max : L'\udfff', t->to);
									new_transitions.insert(&tempVar2);
								}
								if (t->max >= L'\uf900')
								{
									Transition tempVar3(t->min > L'\uf900' ? t->min : L'\uf900', t->max, t->to);
									new_transitions.insert(&tempVar3);
								}
							}
							else if (t->min <= L'\udfff' || t->max >= L'\uf900')
							{
								addepsilon = true;
							}
							if (addepsilon)
							{
								StatePair tempVar4(s, t->to);
								epsilons.insert(&tempVar4);
							}
						}
						s->transitions = new_transitions;
					}
					a->reduce();
					a->addEpsilons(epsilons);
					a->removeDeadTransitions();
					a->checkMinimizeAlways();
					return a;
				}
			}

			bool SpecialOperations::isFinite(Automaton *a)
			{
				if (a->isSingleton())
				{
					return true;
				}
				return isFinite(a->initial, std::unordered_set<State*>(), std::unordered_set<State*>());
			}

			bool SpecialOperations::isFinite(State *s, std::unordered_set<State*> &path, std::unordered_set<State*> &visited)
			{
				path.insert(s);
				for (auto t : s->transitions)
				{
					if (std::find(path.begin(), path.end(), t->to) != path.end() || (!std::find(visited.begin(), visited.end(), t->to) != visited.end() && !isFinite(t->to, path, visited)))
					{
						return false;
					}
				}
				path.remove(s);
				visited.insert(s);
				return true;
			}

			Set<std::wstring> *SpecialOperations::getStrings(Automaton *a, int length)
			{
				std::unordered_set<std::wstring> strings;
				if (a->isSingleton() && a->singleton.length() == length)
				{
					strings.insert(a->singleton);
				}
				else if (length >= 0)
				{
					StringBuilder tempVar();
					getStrings(a->initial, strings, &tempVar, length);
				}
				return strings;
			}

			void SpecialOperations::getStrings(State *s, Set<std::wstring> *strings, StringBuilder *path, int length)
			{
				if (length == 0)
				{
					if (s->accept)
					{
						strings->add(path->toString());
					}
				}
				else
				{
					for (auto t : s->transitions)
					{
						for (int n = t->min; n <= t->max; n++)
						{
							path->append(static_cast<wchar_t>(n));
							getStrings(t->to, strings, path, length - 1);
							path->deleteCharAt(path->length() - 1);
						}
					}
				}
			}

			Set<std::wstring> *SpecialOperations::getFiniteStrings(Automaton *a)
			{
				std::unordered_set<std::wstring> strings;
				if (a->isSingleton())
				{
					strings.insert(a->singleton);
				}
				else if (!getFiniteStrings(a->initial, std::unordered_set<State*>(), strings, new StringBuilder(), -1))
				{
					return nullptr;
				}
				return strings;
			}

			Set<std::wstring> *SpecialOperations::getFiniteStrings(Automaton *a, int limit)
			{
				std::unordered_set<std::wstring> strings;
				if (a->isSingleton())
				{
					if (limit > 0)
					{
						strings.insert(a->singleton);
					}
					else
					{
						return nullptr;
					}
				}
				else if (!getFiniteStrings(a->initial, std::unordered_set<State*>(), strings, new StringBuilder(), limit))
				{
					return nullptr;
				}
				return strings;
			}

			bool SpecialOperations::getFiniteStrings(State *s, std::unordered_set<State*> &pathstates, std::unordered_set<std::wstring> &strings, StringBuilder *path, int limit)
			{
				pathstates.insert(s);
				for (auto t : s->transitions)
				{
					if (std::find(pathstates.begin(), pathstates.end(), t->to) != pathstates.end())
					{
						return false;
					}
					for (int n = t->min; n <= t->max; n++)
					{
						path->append(static_cast<wchar_t>(n));
						if (t->to->accept)
						{
							strings.insert(path->toString());
							if (limit >= 0 && strings.size() > limit)
							{
								return false;
							}
						}
						if (!getFiniteStrings(t->to, pathstates, strings, path, limit))
						{
							return false;
						}
						path->deleteCharAt(path->length() - 1);
					}
				}
				pathstates.remove(s);
				return true;
			}

			std::wstring SpecialOperations::getCommonPrefix(Automaton *a)
			{
				if (a->isSingleton())
				{
					return a->singleton;
				}
				StringBuilder *b = new StringBuilder();
				std::unordered_set<State*> visited;
				State *s = a->initial;
				bool done;
				do
				{
					done = true;
					visited.insert(s);
					if (!s->accept && s->transitions->size() == 1)
					{
						Transition *t = s->transitions->begin()->next();
						if (t->min == t->max && !std::find(visited.begin(), visited.end(), t->to) != visited.end())
						{
							b->append(t->min);
							s = t->to;
							done = false;
						}
					}
				} while (!done);
				return b->toString();
			}

			void SpecialOperations::prefixClose(Automaton *a)
			{
				for (auto s : a->getStates())
				{
					s->setAccept(true);
				}
				a->clearHashCode();
				a->checkMinimizeAlways();
			}

			Automaton *SpecialOperations::hexCases(Automaton *a)
			{
				std::unordered_map<wchar_t, Set<wchar_t>*> map;
				for (wchar_t c1 = L'a', c2 = L'A'; c1 <= L'f'; c1++, c2++)
				{
					Set<wchar_t> *ws = std::unordered_set<wchar_t>();
					ws->add(c1);
					ws->add(c2);
					map[c1] = ws;
					map[c2] = ws;
				}
				Automaton *ws = Datatypes::getWhitespaceAutomaton();
				return ws->concatenate(a->subst(map))->concatenate(ws);
			}

			Automaton *SpecialOperations::replaceWhitespace(Automaton *a)
			{
				std::unordered_map<wchar_t, Set<wchar_t>*> map;
				Set<wchar_t> *ws = std::unordered_set<wchar_t>();
				ws->add(L' ');
				ws->add(L'\t');
				ws->add(L'\n');
				ws->add(L'\r');
				map[L' '] = ws;
				return a->subst(map);
			}
		}
	}
}
