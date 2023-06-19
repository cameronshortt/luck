/* cc -o # %
 * luck - fortune generalised
 * usage: luck [-n] [-d delim] [files]
 *   e.g: luck -nd %% /lib/fortune
 */

/* these need to persist between randln calls */
lines=0;
char *save;

die (n,s)
char *s;
{
  write (2,s,strlen (s));
  exit (n);
}

usage (n)
{
  die (n,"usage: luck [-n] [-d delim] [files]\n");
}

rd (fd,dst,end)
char *dst,*end;
{
  char c;
  int s,r,i;

  r=i=0;

  while (i<strlen (end) && (s=read (fd,&c,1))) {
    r += s;

check:
    if (c == end[i]) {
      ++i;
      continue;
    } else if (i>0) {
      memcpy (dst,end,i);
      dst+=i;
      i=0;

      goto check;  /* the check needs to be run again just in case c==*end */
    }

    *dst++=c;
  }
  *dst++=0;

  return (r);
}

randln (fd,delim)
char *delim;
{
  char *buf;
  buf=malloc (4096);

  while (rd (fd,buf,delim))
    if (!arc4random_uniform (++lines))
      strcpy (save,buf);

  free (buf);
}

main (c,v)
char **v;
{
  int nl,fd;
  char *delim;

  /* luck    : delim="\n"   */
  /* luck -n : delim="\n\n" */
  nl=fd=0;
  delim=malloc (1);
  *delim='\n';

  save=malloc (4096);

  while (++v,--c) {
    if (**v == '-') {
      if (*(*v+1)) {  /* check for '-' (stdin filename) */
        while (*++*v) {
          switch (**v) {
          case 'n': nl=1; break;  /* delim\n */
          case 'd':  /* -ddelim or -d delim */
            if (*++*v || (++v,--c)) {
              delim=realloc (delim,strlen (*v)+1);
              strcpy (delim,*v);
            } else {
              usage (2);
            }
            goto arg;  /* get next argument (break switch, then break loop) */
          case 'f':  /* bsd-style fortune delim */
            nl=0;

            delim=realloc (delim,3);
            strcpy (delim,"%%\n");

            break;
          default:
            usage (1);
          }
        }
      } else {
        if (nl) {
          delim=realloc (delim,strlen (delim)+1);
          strcat (delim,"\n");
        }

        randln (0,delim);  /* use stdin as specified by '-' */

        fd=1;  /* don't trip no-arg case */
      }
    } else {
      if (nl) {
        delim=realloc (delim,strlen (delim)+1);
        strcat (delim,"\n");
      }

      if ((fd=open (*v,0))<0)
        die (-1,"luck: could not open file\n");

      randln (fd,delim);

      close (fd);
    }
arg:
  }

  if (!fd)  /* no args, so use stdin with default delim */
    randln (0,delim);

  write (1,save,strlen (save));
  if (!strcmp (delim,"\n"))
    write (1,"\n",1);

  free (save);
  free (delim);
  exit (0);
}
