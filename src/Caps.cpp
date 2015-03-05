
#include <libclientserver.h>

int Caps::HasCap(int cap)
{
	cap_t caps;
	cap_flag_value_t value = CAP_CLEAR;

	caps = cap_get_proc();

	if (cap_get_flag(caps, cap, CAP_EFFECTIVE, &value) < 0)
	{
		int err = errno;
		cap_free(caps);
		return -err;
	}

	cap_free(caps);
	return value;
}

int Caps::SetCap(int cap)
{
	cap_t caps;
	cap_value_t cap_list[1] = { cap };

	caps = cap_get_proc();
	if (caps == NULL)
		return -errno;

	if (cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_list, CAP_SET) == -1)
	{
		int err = errno;
		cap_free(caps);
		return -err;
	}

	if (cap_set_flag(caps, CAP_PERMITTED, 1, cap_list, CAP_SET) == -1)
	{
		int err = errno;
		cap_free(caps);
		return -err;
	}

	if (cap_set_proc(caps) < 0)
	{
		int err = errno;
		cap_free(caps);
		return -err;
	}

	cap_free(caps);
	return 0;
}

int Caps::Keep()
{
	if (prctl(PR_SET_KEEPCAPS, 1, 0, 0) < 0)
		return -errno;
	return 0;
}

int Caps::UnKeep()
{
	if (prctl(PR_SET_KEEPCAPS, 0, 0, 0) < 0)
		return -errno;
	return 0;
}



