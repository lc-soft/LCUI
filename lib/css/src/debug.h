
#ifdef DEBUG
#define DEBUG_MSG _DEBUG_MSG
#else
#define DEBUG_MSG(format, ...)
#endif

#define _DEBUG_MSG(format, ...)                                       \
	logger_log(LOGGER_LEVEL_DEBUG, __FILE__ ":%d: %s(): " format, \
		   __LINE__, __FUNCTION__, ##__VA_ARGS__)
