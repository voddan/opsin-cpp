#include "TransitionComparator.h"
#include "Transition.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

			TransitionComparator::TransitionComparator(bool to_first)
			{
				this->to_first = to_first;
			}

			int TransitionComparator::compare(Transition *t1, Transition *t2)
			{
				if (to_first)
				{
					if (t1->to != t2->to)
					{
						if (t1->to == nullptr)
						{
							return -1;
						}
						else if (t2->to == nullptr)
						{
							return 1;
						}
						else if (t1->to->number < t2->to->number)
						{
							return -1;
						}
						else if (t1->to->number > t2->to->number)
						{
							return 1;
						}
					}
				}
				if (t1->min < t2->min)
				{
					return -1;
				}
				if (t1->min > t2->min)
				{
					return 1;
				}
				if (t1->max > t2->max)
				{
					return -1;
				}
				if (t1->max < t2->max)
				{
					return 1;
				}
				if (!to_first)
				{
					if (t1->to != t2->to)
					{
						if (t1->to == nullptr)
						{
							return -1;
						}
						else if (t2->to == nullptr)
						{
							return 1;
						}
						else if (t1->to->number < t2->to->number)
						{
							return -1;
						}
						else if (t1->to->number > t2->to->number)
						{
							return 1;
						}
					}
				}
				return 0;
			}
		}
	}
}
