#include "ContactResolver.h"

void P6::ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
{
	this->current_iteration = 0;

	while (this->current_iteration < this->max_iterations)
	{
		unsigned current_index = 0;
		float curr_min = contacts[0]->GetSeperatingSpeed();
		float max_depth = contacts[0]->depth;
		
		for (int i = 1; i < contacts.size(); i++) {
			float ss = contacts[i]->GetSeperatingSpeed();

			if (ss < curr_min) {
				current_index = i;
				curr_min = ss;

				if (max_depth < contacts[i]->depth) {
					max_depth = contacts[i]->depth;
				}
			}
		}

		if (curr_min >= 0 && max_depth <= 0) {
			return;
		}

		contacts[current_index]->Resolve(time);
		this->current_iteration++;
	}
}
