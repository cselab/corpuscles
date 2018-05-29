#define ERR(fmt, ...) he_err(__FILE__, __LINE__, (fmt), ##__VA_ARGS__)
#define MSG(fmt, ...) he_msg(__FILE__, __LINE__, (fmt), ##__VA_ARGS__)

void he_err(const char *file, int line, const char *fmt, ...);
void he_msg(const char *file, int line, const char *fmt, ...);

/* error codes */
enum {HE_OK, HE_READ};
