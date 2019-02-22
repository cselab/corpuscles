/* error codes */
enum {HE_OK, HE_IO, HE_MEMORY, HE_SYS, HE_USER, HE_INDEX, HE_NUM, HE_NOT};

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

/* for user code */
#define ER(fmt, ...) ERR(HE_USER, (fmt), ##__VA_ARGS__)
#define ERR(code, fmt, ...)                                             \
    do {                                                                \
        he_err(code, __FILE__, __LINE__, (fmt), ##__VA_ARGS__);         \
        return code;                                                    \
    } while(0)

#define MSG(fmt, ...) he_msg(__FILE__, __LINE__, (fmt), ##__VA_ARGS__)

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

void he_err(int code, const char *file, int line, const char *fmt, ...);
void he_msg(const char *file, int line, const char *fmt, ...);

int err_set(int);
int err_set_abort();
int err_set_ignore();
