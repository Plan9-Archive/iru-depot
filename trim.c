/* clear string from escape sequences
 * Iruatã Souza
 * Public Domain
 */

#include <u.h>
#include <libc.h>

int
trim(const char *t, char *s)
{
	int n = 0;
	
	while (*t) {
		switch (*t) {
		case '\n': /* FALLTHROUGH */
		case '\r': /* FALLTHROUGH */
		case '\t': /* FALLTHROUGH */
		case '\v': /* FALLTHROUGH */
		case '\b': /* FALLTHROUGH */
		case '\f': /* FALLTHROUGH */
		case '\a': /* FALLTHROUGH */
			break;
		default:
			*s++ = *t;
			n++;
			break;
		}
		*t++;
	}
	*s = '\0';
	return n;
}


int
main(void)
{
	char *s = "i\tr\bu\na\vt\fa\a.", s1[16], n;

	n = trim(s, s1);
	
	print("%s -> %s (%d chars copied)\n", s, s1, n);
	return 0;
}
