#include "PlatformHelper.h"

inline void BCats::Helper::open_href(const char* url)
{
#ifdef WIN32
	ShellExecute(NULL, "open", url, 0, 0, SW_SHOWNORMAL);
#endif // WIN32
}
