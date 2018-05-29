#define ERR(fmt, ...) tri_err(__FILE__, __LINE__, (fmt), ##__VA_ARGS__)
#define MSG(fmt, ...) tri_msg(__FILE__, __LINE__, (fmt), ##__VA_ARGS__)

void tri_err(const char *file, int line, const char *fmt, ...);
void tri_msg(const char *file, int line, const char *fmt, ...);
