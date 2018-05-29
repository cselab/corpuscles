int util_mkdir(const char *path);

/* string equal? */
int util_eq(const char *a, const char *b);

/* fgets without '\n' */
char *util_fgets(char *s, FILE *stream);
