#include "edenpch.h"
#include "Helpers.h"

namespace Eden {

	void msleep(long milliseconds)
	{
		struct timespec req, rem;

		if(milliseconds > 999)
		{
			req.tv_sec = (int)(milliseconds / 1000);
			req.tv_nsec = (long long int)((milliseconds - ((long)req.tv_sec * 1000)) * 1000000);
		}
		else
		{
			req.tv_sec = 0;
			req.tv_nsec = (long long int)(milliseconds * 1000000);
		}

		nanosleep(&req , &rem);
	}

}