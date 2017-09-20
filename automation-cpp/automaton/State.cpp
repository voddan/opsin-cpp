#include "State.h"
#include "Transition.h"
#include "TransitionComparator.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

int State::next_id = 0;

			State::State()
			{
				resetTransitions();
				id = next_id++;
			}

			void State::resetTransitions()
			{
				transitions = std::unordered_set<Transition*>();
			}

			Set<Transition*> *State::getTransitions()
			{
				return transitions;
			}

			void State::addTransition(Transition *t)
			{
				transitions->add(t);
			}

			void State::setAccept(bool accept)
			{
				this->accept = accept;
			}

			bool State::isAccept()
			{
				return accept;
			}

			State *State::step(wchar_t c)
			{
				for (auto t : transitions)
				{
					if (t->min <= c && c <= t->max)
					{
						return t->to;
					}
				}
				return nullptr;
			}

			void State::step(wchar_t c, Collection<State*> *dest)
			{
				for (auto t : transitions)
				{
					if (t->min <= c && c <= t->max)
					{
						dest->add(t->to);
					}
				}
			}

			void State::addEpsilon(State *to)
			{
				if (to->accept)
				{
					accept = true;
				}
				transitions->addAll(to->transitions);
			}

			std::vector<Transition*> State::getSortedTransitionArray(bool to_first)
			{
				std::vector<Transition*> e = transitions->toArray(std::vector<Transition*>(transitions->size()));
				TransitionComparator tempVar(to_first);
				Arrays::sort(e, &tempVar);
				return e;
			}

			std::vector<Transition*> State::getSortedTransitions(bool to_first)
			{
				return Arrays::asList(getSortedTransitionArray(to_first));
			}

			std::wstring State::toString()
			{
				StringBuilder *b = new StringBuilder();
				b->append(L"state ")->append(number);
				if (accept)
				{
					b->append(L" [accept]");
				}
				else
				{
					b->append(L" [reject]");
				}
				b->append(L":\n");
				for (auto t : transitions)
				{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
					b->append(L"  ")->append(t->toString())->append(L"\n");
				}
				return b->toString();
			}

			int State::compareTo(State *s)
			{
				return s->id - id;
			}

			bool State::equals(void *obj)
			{
				return __super::equals(obj);
			}

			int State::hashCode()
			{
				return __super::hashCode();
			}
		}
	}
}
