#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#define __USE_XOPEN
#include <time.h>

#define felder 56

#ifdef _WIN32
#define strtok_r strtok_s
__inline char *
stpcpy (char *a, const char *b)
{
  size_t n, m;

  n = strlen (a);
  m = strlen (b);
  a += n;
  memcpy (a, b, m + 1);
  return a + m;
}
#endif

size_t
getFilesize (const char *filename)
{
  struct stat st;

  stat (filename, &st);
  return st.st_size;
}

void parsedata (char **buffer, char **outpbuff, char **Kammer,
		char **Vorgang);
static int first = 0;
static unsigned long intsize = 2048000;
static unsigned int usedcols[58] =
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

int
main (int argc, char *argv[])
{
  int usedcolsinit[12] = { 1, 2, 9, 13, 14, 15, 20, 23, 24, 25, 26, 27 };
  for (int i = 0; i < 11; i++)
    usedcols[usedcolsinit[i] - 1] = 1;

  char *outbuff         = malloc (intsize);
  char *buffer          = malloc (intsize);
  char *filename        = argv[1];
  unsigned long cnt     = 0;
  unsigned long seekoff = 0;
  unsigned long tiles   = 0;
  unsigned long fsize   = (int) getFilesize (argv[1]);
  FILE *fp              = fopen (argv[1], "rb");
  FILE *fp2             = fopen ("courbesall.txt", "a");
  char *token   = strtok (filename, "_");
  char *Kammer  = strtok (NULL, "_");
  char *Vorgang = strtok (NULL, "_");
while(*Vorgang=='0')
Vorgang++;

//printf("%s-%s-%s\n",token,Kammer,Vorgang);
  unsigned long lastblock;
  int nlpos;



  buffer[intsize - 1] = '\0';

  setvbuf (fp2, NULL, _IOFBF, intsize);

  while (fread (buffer, intsize - 1, 1, fp))
    {
      cnt++;
      seekoff = ftell (fp);
      nlpos = intsize - 1UL;
      while (buffer[nlpos] != '\n')
	{
	  nlpos--;
	  seekoff = seekoff - 1UL;
	}
      fseek (fp, seekoff, SEEK_SET);
      buffer[nlpos - 1] = '\0';
      buffer[intsize] = '\0';
      parsedata (&buffer, &outbuff, &Kammer, &Vorgang);
      tiles = ftell (fp);
      fprintf (fp2, "%s", outbuff);
    }

  lastblock             = fsize - tiles;
  buffer[lastblock - 1] = '\0';
  fread (buffer, lastblock + 1, 1, fp);
  parsedata (&buffer, &outbuff, &Kammer, &Vorgang);
  fprintf (fp2, "%s", outbuff);
  fprintf (fp2, "%c", '\n');
  fflush (fp2);
  fclose (fp);
  fclose (fp2);
}

static int firstnl = 1;
void
checkdate (char **tmps)
{
  struct tm tm;
  char *tmp;
  tmp = tmps[0];
  if (firstnl != 1)
    tmp++;
  firstnl = 0;
  memset (&tm, 0, sizeof (tm));
  char *check = malloc (80);
  check[0] = '\0';

  if (tmp[0] == '\n')
    {
      tmp++;
    }
  if (tmp[0] == '\n')
    check = strptime (tmp, "\n%d/%m/%Y %H:%M:%S", &tm);
  else
    check = strptime (tmp, "%d/%m/%Y %H:%M:%S", &tm);

  if (check == NULL || *check != '\0')
    {
      if (tmp[0] == '\n')
	check = strptime (tmp, "\n%Y/%m/%d %H:%M:%S", &tm);
      else
	check = strptime (tmp, "%Y/%m/%d %H:%M:%S", &tm);

      if (check == NULL || *check != '\0')
	{
	}
      else if (tmp[0] == '\n')
	strftime (tmp, 21, "\n%d-%m-%Y %H:%M:%S", &tm);
      else
	strftime (tmp, 21, "%d-%m-%Y %H:%M:%S", &tm);
    }
  else if (tmp[0] == '\n')
    strftime (tmp, 21, "\n%d-%m-%Y %H:%M:%S", &tm);
  else
    strftime (tmp, 21, "%d-%m-%Y %H:%M:%S", &tm);
}


void
parsedata (char **buffer, char **outpbuff, char **Kammer, char **Vorgang)
{
  int ignore       = 3;
  char *backup     = NULL;
  int n            = 0;
  size_t seps      = 0;
  char *last       = NULL;
  char *lastconcat = NULL;
  char *p;
  int x;
  int first = 1;

  *outpbuff[0] = 0;
  lastconcat = *outpbuff;
  int firstnl = 1;
  for (p = strtok_r (*buffer, ",", &backup); p != NULL;
       p = strtok_r (NULL, ",", &backup))
    {
      if (n == 0)
	{
	  if (first != 1)
	    {
	      lastconcat = stpcpy (lastconcat, *Kammer);
	      lastconcat = stpcpy (lastconcat, ",");
	      lastconcat = stpcpy (lastconcat, *Vorgang);
	    }
	  checkdate (&p);
	}
      if (usedcols[n] != 0)
	{
	  if (first != 1)
	    lastconcat = stpcpy (lastconcat, ",");
	  first = 0;
	  lastconcat = stpcpy (lastconcat, p);
	}
      n++;
      if (n >= felder)
	{
	  n = 0;
	}
    }
}

