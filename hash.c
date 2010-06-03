/* pjw hash */
typedef unsigned long ulong;

ulong
pjw(char *s)
{
	ulong n, g, h, i;

	g = h = i = 0;		
	n = strlen(s);

	while(n > 0){
		n--;
		h = (h<<4)+s[i];
		g = h & 0xf00000000;
		if(g != 0)
			h ^= (g>>24);
		h ^= g;
		i++;
	}
	return h;
}
