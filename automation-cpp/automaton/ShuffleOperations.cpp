#include "ShuffleOperations.h"
#include "Automaton.h"
#include "Transition.h"
#include "StatePair.h"
#include "State.h"
#include "TransitionComparator.h"
#include "BasicOperations.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

			ShuffleOperations::ShuffleOperations()
			{
			}

			Automaton *ShuffleOperations::shuffle(Automaton *a1, Automaton *a2)
			{
				a1->determinize();
				a2->determinize();
				std::vector<std::vector<Transition*>> transitions1 = Automaton::getSortedTransitions(a1->getStates());
				std::vector<std::vector<Transition*>> transitions2 = Automaton::getSortedTransitions(a2->getStates());
				Automaton *c = new Automaton();
				std::list<StatePair*> worklist;
				std::unordered_map<StatePair*, StatePair*> newstates;
				State *s = new State();
				c->initial = s;
				StatePair *p = new StatePair(s, a1->initial, a2->initial);
				worklist.push_back(p);
				newstates[p] = p;
				while (worklist.size() > 0)
				{
					p = worklist.pop_front();
					p->s->accept = p->s1->accept && p->s2->accept;
					std::vector<Transition*> t1 = transitions1[p->s1->number];
					for (int n1 = 0; n1 < t1.size(); n1++)
					{
						StatePair *q = new StatePair(t1[n1]->to, p->s2);
						StatePair *r = newstates[q];
						if (r == nullptr)
						{
							q->s = new State();
							worklist.push_back(q);
							newstates[q] = q;
							r = q;
						}
						Transition tempVar(t1[n1]->min, t1[n1]->max, r->s);
						p->s->transitions->add(&tempVar);
					}
					std::vector<Transition*> t2 = transitions2[p->s2->number];
					for (int n2 = 0; n2 < t2.size(); n2++)
					{
						StatePair *q = new StatePair(p->s1, t2[n2]->to);
						StatePair *r = newstates[q];
						if (r == nullptr)
						{
							q->s = new State();
							worklist.push_back(q);
							newstates[q] = q;
							r = q;
						}
						Transition tempVar2(t2[n2]->min, t2[n2]->max, r->s);
						p->s->transitions->add(&tempVar2);
					}
				}
				c->deterministic = false;
				c->removeDeadTransitions();
				c->checkMinimizeAlways();
				return c;
			}

			std::wstring ShuffleOperations::shuffleSubsetOf(Collection<Automaton*> *ca, Automaton *a, boost::optional<wchar_t> suspend_shuffle, boost::optional<wchar_t> resume_shuffle)
			{
				if (ca->size() == 0)
				{
					return L"";
				}
				if (ca->size() == 1)
				{
					Automaton *a1 = ca->begin()->next();
					if (a1->isSingleton())
					{
						if (a->run(a1->singleton))
						{
							return L"";
						}
						else
						{
							return a1->singleton;
						}
					}
					if (a1 == a)
					{
						return L"";
					}
				}
				a->determinize();
				std::vector<std::vector<std::vector<Transition*>>> ca_transitions(ca->size());
				int i = 0;
				for (auto a1 : ca)
				{
					ca_transitions[i++] = Automaton::getSortedTransitions(a1->getStates());
				}
				std::vector<std::vector<Transition*>> a_transitions = Automaton::getSortedTransitions(a->getStates());
				TransitionComparator *tc = new TransitionComparator(false);
				ShuffleConfiguration *init = new ShuffleConfiguration(ca, a);
				std::list<ShuffleConfiguration*> pending;
				Set<ShuffleConfiguration*> *visited = std::unordered_set<ShuffleConfiguration*>();
				pending.push_back(init);
				visited->add(init);
				while (!pending.empty())
				{
					ShuffleConfiguration *c = pending.pop_front();
					bool good = true;
					for (int i1 = 0; i1 < ca->size(); i1++)
					{
						if (!c->ca_states[i1]->accept)
						{
							good = false;
							break;
						}
					}
					if (c->a_state->accept)
					{
						good = false;
					}
					if (good)
					{
						StringBuilder *sb = new StringBuilder();
						while (c->prev != nullptr)
						{
							sb->append(c->min);
							c = c->prev;
						}
						StringBuilder *sb2 = new StringBuilder();
						for (int j = sb->length() - 1; j >= 0; j--)
						{
							sb2->append(sb->charAt(j));
						}
						return sb2->toString();
					}
					std::vector<Transition*> ta2 = a_transitions[c->a_state->number];
					for (int i1 = 0; i1 < ca->size(); i1++)
					{
						if (c->shuffle_suspended)
						{
							i1 = c->suspended1;
						}
						for (auto t1 : ca_transitions[i1][c->ca_states[i1]->number])
						{
							std::vector<Transition*> lt;
							int j = Arrays::binarySearch(ta2, t1, tc);
							if (j < 0)
							{
								j = -j - 1;
							}
							if (j > 0 && ta2[j - 1]->max >= t1->min)
							{
								j--;
							}
							while (j < ta2.size())
							{
								Transition *t2 = ta2[j++];
								wchar_t min = t1->min;
								wchar_t max = t1->max;
								if (t2->min > min)
								{
									min = t2->min;
								}
								if (t2->max < max)
								{
									max = t2->max;
								}
								if (min <= max)
								{
									add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, min, max);
									Transition tempVar(min, max, nullptr);
									lt.push_back(&tempVar);
								}
								else
								{
									break;
								}
							}
							std::vector<Transition*> at = lt.toArray(std::vector<Transition*>(lt.size()));
							Arrays::sort(at, tc);
							wchar_t min = t1->min;
							for (int k = 0; k < at.size(); k++)
							{
								if (at[k]->min > min)
								{
									break;
								}
								if (at[k]->max >= t1->max)
								{
									goto loopContinue;
								}
								min = static_cast<wchar_t>(at[k]->max + 1);
							}
							ShuffleConfiguration *nc = new ShuffleConfiguration(c, i1, t1->to, min);
							StringBuilder *sb = new StringBuilder();
							ShuffleConfiguration *b = nc;
							while (b->prev != nullptr)
							{
								sb->append(b->min);
								b = b->prev;
							}
							StringBuilder *sb2 = new StringBuilder();
							for (int m = sb->length() - 1; m >= 0; m--)
							{
								sb2->append(sb->charAt(m));
							}
							if (c->shuffle_suspended)
							{
								sb2->append(BasicOperations::getShortestExample(nc->ca_states[c->suspended1], true));
							}
							for (i1 = 0; i1 < ca->size(); i1++)
							{
								if (!c->shuffle_suspended || i1 != c->suspended1)
								{
									sb2->append(BasicOperations::getShortestExample(nc->ca_states[i1], true));
								}
							}
							return sb2->toString();
							loopContinue:
						}
						loopBreak:
						if (c->shuffle_suspended)
						{
							break;
						}
					}
				}
				return L"";
			}

			void ShuffleOperations::add(boost::optional<wchar_t> suspend_shuffle, boost::optional<wchar_t> resume_shuffle, std::list<ShuffleConfiguration*> &pending, Set<ShuffleConfiguration*> *visited, ShuffleConfiguration *c, int i1, Transition *t1, Transition *t2, wchar_t min, wchar_t max)
			{
				constexpr wchar_t HIGH_SURROGATE_BEGIN = L'\uD800';
				constexpr wchar_t HIGH_SURROGATE_END = L'\uDBFF';
				if (suspend_shuffle && min <= suspend_shuffle && suspend_shuffle <= max && min != max)
				{
					if (min < suspend_shuffle)
					{
						add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, min, static_cast<wchar_t>(suspend_shuffle - 1));
					}
					add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, suspend_shuffle, suspend_shuffle);
					if (suspend_shuffle < max)
					{
						add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, static_cast<wchar_t>(suspend_shuffle + 1), max);
					}
				}
				else if (resume_shuffle && min <= resume_shuffle && resume_shuffle <= max && min != max)
				{
					if (min < resume_shuffle)
					{
						add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, min, static_cast<wchar_t>(resume_shuffle - 1));
					}
					add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, resume_shuffle, resume_shuffle);
					if (resume_shuffle < max)
					{
						add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, static_cast<wchar_t>(resume_shuffle + 1), max);
					}
				}
				else if (min < HIGH_SURROGATE_BEGIN && max >= HIGH_SURROGATE_BEGIN)
				{
					add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, min, static_cast<wchar_t>(HIGH_SURROGATE_BEGIN - 1));
					add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, HIGH_SURROGATE_BEGIN, max);
				}
				else if (min <= HIGH_SURROGATE_END && max > HIGH_SURROGATE_END)
				{
					add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, min, HIGH_SURROGATE_END);
					add(suspend_shuffle, resume_shuffle, pending, visited, c, i1, t1, t2, static_cast<wchar_t>(HIGH_SURROGATE_END + 1), max);
				}
				else
				{
					ShuffleConfiguration *nc = new ShuffleConfiguration(c, i1, t1->to, t2->to, min);
					if (suspend_shuffle && min == suspend_shuffle)
					{
						nc->shuffle_suspended = true;
						nc->suspended1 = i1;
					}
					else if (resume_shuffle && min == resume_shuffle)
					{
						nc->shuffle_suspended = false;
					}
					if (min >= HIGH_SURROGATE_BEGIN && min <= HIGH_SURROGATE_BEGIN)
					{
						nc->shuffle_suspended = true;
						nc->suspended1 = i1;
						nc->surrogate = true;
					}
					if (!visited->contains(nc))
					{
						pending.push_back(nc);
						visited->add(nc);
					}
				}
			}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unused") private ShuffleConfiguration()
			ShuffleOperations::ShuffleConfiguration::ShuffleConfiguration()
			{
			}

			ShuffleOperations::ShuffleConfiguration::ShuffleConfiguration(Collection<Automaton*> *ca, Automaton *a)
			{
				ca_states = std::vector<State*>(ca->size());
				int i = 0;
				for (auto a1 : ca)
				{
					ca_states[i++] = a1->getInitialState();
				}
				a_state = a->getInitialState();
				computeHash();
			}

			ShuffleOperations::ShuffleConfiguration::ShuffleConfiguration(ShuffleConfiguration *c, int i1, State *s1, wchar_t min)
			{
				prev = c;
				ca_states = c->ca_states.clone();
				a_state = c->a_state;
				ca_states[i1] = s1;
				this->min = min;
				computeHash();
			}

			ShuffleOperations::ShuffleConfiguration::ShuffleConfiguration(ShuffleConfiguration *c, int i1, State *s1, State *s2, wchar_t min)
			{
				prev = c;
				ca_states = c->ca_states.clone();
				a_state = c->a_state;
				ca_states[i1] = s1;
				a_state = s2;
				this->min = min;
				if (!surrogate)
				{
					shuffle_suspended = c->shuffle_suspended;
					suspended1 = c->suspended1;
				}
				computeHash();
			}

			bool ShuffleOperations::ShuffleConfiguration::equals(void *obj)
			{
				if (dynamic_cast<ShuffleConfiguration*>(obj) != nullptr)
				{
					ShuffleConfiguration *c = static_cast<ShuffleConfiguration*>(obj);
					return shuffle_suspended == c->shuffle_suspended && surrogate == c->surrogate && suspended1 == c->suspended1 && Arrays::equals(ca_states, c->ca_states) && a_state == c->a_state;
				}
				return false;
			}

			int ShuffleOperations::ShuffleConfiguration::hashCode()
			{
				return hash;
			}

			void ShuffleOperations::ShuffleConfiguration::computeHash()
			{
				hash = 0;
				for (int i = 0; i < ca_states.size(); i++)
				{
					hash ^= ca_states[i]->hashCode();
				}
				hash ^= a_state->hashCode() * 100;
				if (shuffle_suspended || surrogate)
				{
					hash += suspended1;
				}
			}
		}
	}
}
