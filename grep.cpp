typedef int va_list;
typedef int size_t;
typedef int __builtin_va_list;
typedef int __gnuc_va_list;
typedef int __int8_t;
typedef int __uint8_t;
typedef int __int16_t;
typedef int __uint16_t;
typedef int __int_least16_t;
typedef int __uint_least16_t;
typedef int __int32_t;
typedef int __uint32_t;
typedef int __int64_t;
typedef int __uint64_t;
typedef int __int_least32_t;
typedef int __uint_least32_t;
typedef int __s8;
typedef int __u8;
typedef int __s16;
typedef int __u16;
typedef int __s32;
typedef int __u32;
typedef int __s64;
typedef int __u64;
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
typedef int _off_t;
typedef int __dev_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int _off64_t;
typedef int _fpos_t;
typedef int _ssize_t;
typedef int wint_t;
typedef int _mbstate_t;
typedef int _flock_t;
typedef int _iconv_t;
typedef int __ULong;
typedef int __FILE;
typedef int ptrdiff_t;
typedef int wchar_t;
typedef int __off_t;
typedef int __pid_t;
typedef int __loff_t;
typedef int u_char;
typedef int u_short;
typedef int u_int;
typedef int u_long;
typedef int ushort;
typedef int uint;
typedef int clock_t;
typedef int time_t;
typedef int daddr_t;
typedef int caddr_t;
typedef int ino_t;
typedef int off_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef int mode_t;
typedef int nlink_t;
typedef int fd_mask;
typedef int _types_fd_set;
typedef int clockid_t;
typedef int timer_t;
typedef int useconds_t;
typedef int suseconds_t;
typedef int FILE;
typedef int fpos_t;
typedef int cookie_read_function_t;
typedef int cookie_write_function_t;
typedef int cookie_seek_function_t;
typedef int cookie_close_function_t;
typedef int cookie_io_functions_t;
typedef int div_t;
typedef int ldiv_t;
typedef int lldiv_t;
typedef int sigset_t;
typedef int __sigset_t;
typedef int _sig_func_ptr;
typedef int sig_atomic_t;
typedef int __tzrule_type;
typedef int __tzinfo_type;
typedef int mbstate_t;
typedef int sem_t;
typedef int pthread_t;
typedef int pthread_attr_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int jmp_buf;
typedef int rlim_t;
typedef int sa_family_t;
typedef int sigjmp_buf;
typedef int stack_t;
typedef int siginfo_t;
typedef int z_stream;
typedef int int8_t;
typedef int uint8_t;
typedef int int16_t;
typedef int uint16_t;
typedef int int32_t;
typedef int uint32_t;
typedef int int64_t;
typedef int uint64_t;
typedef int int_least8_t;
typedef int uint_least8_t;
typedef int int_least16_t;
typedef int uint_least16_t;
typedef int int_least32_t;
typedef int uint_least32_t;
typedef int int_least64_t;
typedef int uint_least64_t;
typedef int int_fast8_t;
typedef int uint_fast8_t;
typedef int int_fast16_t;
typedef int uint_fast16_t;
typedef int int_fast32_t;
typedef int uint_fast32_t;
typedef int int_fast64_t;
typedef int uint_fast64_t;
typedef int intptr_t;
typedef int uintptr_t;
typedef int intmax_t;
typedef int uintmax_t;
typedef _Bool bool;
extern int errno;
extern void fatal(const char *, int);
extern struct matcher
{
  char *name;
  void (*compile)(char *, size_t);
  char *(*execute)(char *, size_t, char **);
} matchers[];
extern char *matcher;
extern int match_icase;
extern int match_words;
extern int match_lines;
char *matcher;
int match_icase;
int match_words;
int match_lines;
static void (*compile)();
static char *(*execute)();
static char *prog;
static char *filename;
static int errseen;
static void
error(mesg, errnum)
     const
     char *mesg;
     int errnum;
{
  if (errnum)
    fprintf(stdout, "%s: %s: %s\n", prog, mesg, strerror(errnum));
  else
    fprintf(stdout, "%s: %s\n", prog, mesg);
  errseen = 1;
}
void
fatal(mesg, errnum)
     const
     char *mesg;
     int errnum;
{
  error(mesg, errnum);
  exit(2);
}
char *
xmalloc(size)
     size_t size;
{
  char *result;
  result = malloc(size);
  if (size && !result)
    fatal("memory exhausted", 0);
  return result;
}
char *
xrealloc(ptr, size)
     char *ptr;
     size_t size;
{
  char *result;
  if (ptr)
    result = realloc(ptr, size);
  else
    result = malloc(size);
  if (size && !result)
    fatal("memory exhausted", 0);
  return result;
}
extern void *valloc(size_t);
static char *buffer;
static size_t bufsalloc;
static size_t bufalloc;
static int bufdesc;
static char *bufbeg;
static char *buflim;
static int bufmapped;
static struct stat bufstat;
static off_t bufoffset;
void
reset(fd)
     int fd;
{
  static int initialized;
  if (!initialized)
    {
      initialized = 1;
      bufsalloc = ((8192) > (getpagesize()) ? (8192) : (getpagesize()));
      bufalloc = 5 * bufsalloc;
      buffer = valloc(bufalloc + 1);
      if (!buffer)
 fatal("memory exhausted", 0);
      bufbeg = buffer;
      buflim = buffer;
    }
  bufdesc = fd;
  if (fstat(fd, &bufstat) < 0 || !S_ISREG(bufstat.st_mode))
    bufmapped = 0;
  else
    {
      bufmapped = 1;
      bufoffset = lseek(fd, 0, 1);
    }
}
static int
fillbuf(save)
     size_t save;
{
  char *nbuffer, *dp, *sp;
  int cc;
  caddr_t maddr;
  static int pagesize;
  if (pagesize == 0 && (pagesize = getpagesize()) == 0)
    abort();
  if (save > bufsalloc)
    {
      while (save > bufsalloc)
 bufsalloc *= 2;
      bufalloc = 5 * bufsalloc;
      nbuffer = valloc(bufalloc + 1);
      if (!nbuffer)
 fatal("memory exhausted", 0);
    }
  else
    nbuffer = buffer;
  sp = buflim - save;
  dp = nbuffer + bufsalloc - save;
  bufbeg = dp;
  while (save--)
    *dp++ = *sp++;
  buffer = nbuffer;
  if (bufmapped && bufoffset % pagesize == 0
      && bufstat.st_size - bufoffset >= bufalloc - bufsalloc)
    {
      maddr = buffer + bufsalloc;
      maddr = mmap(maddr, bufalloc - bufsalloc, PROT_READ | PROT_WRITE,
     MAP_PRIVATE | MAP_FIXED, bufdesc, bufoffset);
      if (maddr == (caddr_t) -1)
 {
   fprintf(stdout, "%s: warning: %s: %s\n", filename,
    strerror(errno));
   goto tryread;
 }
      cc = bufalloc - bufsalloc;
      bufoffset += cc;
    }
  else
    {
    tryread:
      if (bufmapped)
 {
   bufmapped = 0;
   lseek(bufdesc, bufoffset, 0);
 }
      cc = read(bufdesc, buffer + bufsalloc, bufalloc - bufsalloc);
    }
  if (cc > 0)
    buflim = buffer + bufsalloc + cc;
  else
    buflim = buffer + bufsalloc;
  return cc;
}
static int out_quiet;
static int out_invert;
static int out_file;
static int out_line;
static int out_byte;
static int out_before;
static int out_after;
static size_t totalcc;
static char *lastnl;
static char *lastout;
static size_t totalnl;
static int pending;
static void
nlscan(lim)
     char *lim;
{
  char *beg;
  for (beg = lastnl; beg < lim; ++beg)
    if (*beg == '\n')
      ++totalnl;
  lastnl = beg;
}
static void
prline(beg, lim, sep)
     char *beg;
     char *lim;
     char sep;
{
  if (out_file)
    printf("%s%c", filename, sep);
  if (out_line)
    {
      nlscan(beg);
      printf("%d%c", ++totalnl, sep);
      lastnl = lim;
    }
  if (out_byte)
    printf("%lu%c", totalcc + (beg - bufbeg), sep);
  fwrite(beg, 1, lim - beg, stdout);
  if (ferror(stdout))
    error("writing output", errno);
  lastout = lim;
}
static void
prpending(lim)
     char *lim;
{
  char *nl;
  if (!lastout)
    lastout = bufbeg;
  while (pending > 0 && lastout < lim)
    {
      --pending;
      if ((nl = memchr(lastout, '\n', lim - lastout)) != 0)
 ++nl;
      else
 nl = lim;
      prline(lastout, nl, '-');
    }
}
static void
prtext(beg, lim, nlinesp)
     char *beg;
     char *lim;
     int *nlinesp;
{
  static int used;
  char *bp, *p, *nl;
  int i, n;
  if (!out_quiet && pending > 0)
    prpending(beg);
  p = beg;
  if (!out_quiet)
    {
      bp = lastout ? lastout : bufbeg;
      for (i = 0; i < out_before; ++i)
 if (p > bp)
   do
     --p;
   while (p > bp && p[-1] != '\n');
      if ((out_before || out_after) && used && p != lastout)
 puts("--");
      while (p < beg)
 {
   nl = memchr(p, '\n', beg - p);
   prline(p, nl + 1, '-');
   p = nl + 1;
 }
    }
  if (nlinesp)
    {
      for (n = 0; p < lim; ++n)
 {
   if ((nl = memchr(p, '\n', lim - p)) != 0)
     ++nl;
   else
     nl = lim;
   if (!out_quiet)
     prline(p, nl, ':');
   p = nl;
 }
      *nlinesp = n;
    }
  else
    if (!out_quiet)
      prline(beg, lim, ':');
  pending = out_after;
  used = 1;
}
static int
grepbuf(beg, lim)
     char *beg;
     char *lim;
{
  int nlines, n;
  register char *p, *b;
  char *endp;
  nlines = 0;
  p = beg;
  while ((b = (*execute)(p, lim - p, &endp)) != 0)
    {
      if (b == lim && ((b > beg && b[-1] == '\n') || b == beg))
 break;
      if (!out_invert)
 {
   prtext(b, endp, (int *) 0);
   nlines += 1;
 }
      else if (p < b)
 {
   prtext(p, b, &n);
   nlines += n;
 }
      p = endp;
    }
  if (out_invert && p < lim)
    {
      prtext(p, lim, &n);
      nlines += n;
    }
  return nlines;
}
static int
grep(fd)
     int fd;
{
  int nlines, i;
  size_t residue, save;
  char *beg, *lim;
  reset(fd);
  totalcc = 0;
  lastout = 0;
  totalnl = 0;
  pending = 0;
  nlines = 0;
  residue = 0;
  save = 0;
  for (;;)
    {
      if (fillbuf(save) < 0)
 {
   error(filename, errno);
   return nlines;
 }
      lastnl = bufbeg;
      if (lastout)
 lastout = bufbeg;
      if (buflim - bufbeg == save)
 break;
      beg = bufbeg + save - residue;
      for (lim = buflim; lim > beg && lim[-1] != '\n'; --lim)
 ;
      residue = buflim - lim;
      if (beg < lim)
 {
   nlines += grepbuf(beg, lim);
   if (pending)
     prpending(lim);
 }
      i = 0;
      beg = lim;
      while (i < out_before && beg > bufbeg && beg != lastout)
 {
   ++i;
   do
     --beg;
   while (beg > bufbeg && beg[-1] != '\n');
 }
      if (beg != lastout)
 lastout = 0;
      save = residue + lim - beg;
      totalcc += buflim - bufbeg - save;
      if (out_line)
 nlscan(beg);
    }
  if (residue)
    {
      nlines += grepbuf(bufbeg + save - residue, buflim);
      if (pending)
 prpending(buflim);
    }
  return nlines;
}
static char version[] = "GNU grep version 2.0";
static void
usage()
{
  fprintf(stdout, "usage: %s [-[[AB] ]<num>] [-[CEFGVchilnqsvwx]] [-[ef]] <expr> [<files...>]\n", prog);
  exit(2);
}
int
setmatcher(name)
     char *name;
{
  int i;
  for (i = 0; matchers[i].name; ++i)
    if (strcmp(name, matchers[i].name) == 0)
      {
 compile = matchers[i].compile;
 execute = matchers[i].execute;
 return 1;
      }
  return 0;
}
int
main(argc, argv)
     int argc;
     char *argv[];
{
  char *keys;
  size_t keycc, oldcc, keyalloc;
  int keyfound, count_matches, no_filenames, list_files, suppress_errors;
  int opt, cc, desc, count, status;
  FILE *fp;
  extern char *optarg;
  extern int optind;
  argv[0] = "target";
  prog = argv[0];
  if (prog && strrchr(prog, '/'))
    prog = strrchr(prog, '/') + 1;
  keys = 0;
  keycc = 0;
  keyfound = 0;
  count_matches = 0;
  no_filenames = 0;
  list_files = 0;
  suppress_errors = 0;
  matcher = 0;
  while ((opt = getopt(argc, argv, "0123456789A:B:CEFGVX:bce:f:hiLlnqsvwxy"))
  != EOF)
    switch (opt)
      {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
 out_before = 10 * out_before + opt - '0';
 out_after = 10 * out_after + opt - '0';
 break;
      case 'A':
 out_after = atoi(optarg);
 if (out_after < 0)
   usage();
 break;
      case 'B':
 out_before = atoi(optarg);
 if (out_before < 0)
   usage();
 break;
      case 'C':
 out_before = out_after = 2;
 break;
      case 'E':
 if (matcher && strcmp(matcher, "egrep") != 0)
   fatal("you may specify only one of -E, -F, or -G", 0);
 matcher = "posix-egrep";
 break;
      case 'F':
 if (matcher && strcmp(matcher, "fgrep") != 0)
   fatal("you may specify only one of -E, -F, or -G", 0);;
 matcher = "fgrep";
 break;
      case 'G':
 if (matcher && strcmp(matcher, "grep") != 0)
   fatal("you may specify only one of -E, -F, or -G", 0);
 matcher = "grep";
 break;
      case 'V':
 fprintf(stdout, "%s\n", version);
 break;
      case 'X':
 if (matcher)
   fatal("matcher already specified", 0);
 matcher = optarg;
 break;
      case 'b':
 out_byte = 1;
 break;
      case 'c':
 out_quiet = 1;
 count_matches = 1;
 break;
      case 'e':
 cc = strlen(optarg);
 keys = xrealloc(keys, keycc + cc + 1);
 if (keyfound)
   keys[keycc++] = '\n';
 strcpy(&keys[keycc], optarg);
 keycc += cc;
 keyfound = 1;
 break;
      case 'f':
 fp = strcmp(optarg, "-") != 0 ? fopen(optarg, "r") : stdin;
 if (!fp)
   fatal(optarg, errno);
 for (keyalloc = 1; keyalloc <= keycc; keyalloc *= 2)
   ;
 keys = xrealloc(keys, keyalloc);
 oldcc = keycc;
 if (keyfound)
   keys[keycc++] = '\n';
 while (!feof(fp)
        && (cc = fread(keys + keycc, 1, keyalloc - keycc, fp)) > 0)
   {
     keycc += cc;
     if (keycc == keyalloc)
       keys = xrealloc(keys, keyalloc *= 2);
   }
 if (fp != stdin)
   fclose(fp);
 if (keycc - oldcc > 0 && keys[keycc - 1] == '\n')
   --keycc;
 keyfound = 1;
 break;
      case 'h':
 no_filenames = 1;
 break;
      case 'i':
      case 'y':
 match_icase = 1;
 break;
      case 'L':
 out_quiet = 1;
 list_files = -1;
 break;
      case 'l':
 out_quiet = 1;
 list_files = 1;
 break;
      case 'n':
 out_line = 1;
 break;
      case 'q':
 out_quiet = 1;
 break;
      case 's':
 suppress_errors = 1;
 break;
      case 'v':
 out_invert = 1;
 break;
      case 'w':
 match_words = 1;
 break;
      case 'x':
 match_lines = 1;
 break;
      default:
 usage();
 break;
      }
  if (!keyfound)
    if (optind < argc)
      {
 keys = argv[optind++];
 keycc = strlen(keys);
      }
    else
      usage();
  if (!matcher)
    matcher = prog;
  if (!setmatcher(matcher) && !setmatcher("default"))
    abort();
  (*compile)(keys, keycc);
  if (argc - optind > 1 && !no_filenames)
    out_file = 1;
  status = 1;
  if (optind < argc)
    while (optind < argc)
      {
 desc = strcmp(argv[optind], "-") ? open(argv[optind], O_RDONLY) : 0;
 if (desc < 0)
   {
     if (!suppress_errors)
       error(argv[optind], errno);
   }
 else
   {
     filename = desc == 0 ? "(standard input)" : argv[optind];
     count = grep(desc);
     if (count_matches)
       {
  if (out_file)
    printf("%s:", filename);
  printf("%d\n", count);
       }
     if (count)
       {
  status = 0;
  if (list_files == 1)
    printf("%s\n", filename);
       }
     else if (list_files == -1)
       printf("%s\n", filename);
   }
 if (desc != 0)
   close(desc);
 ++optind;
      }
  else
    {
      filename = "(standard input)";
      count = grep(0);
      if (count_matches)
 printf("%d\n", count);
      if (count)
 {
   status = 0;
   if (list_files == 1)
     printf("(standard input)\n");
 }
      else if (list_files == -1)
 printf("(standard input)\n");
    }
  exit(errseen ? 2 : status);
}
extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;
struct option
{
  const char *name;
  int has_arg;
  int *flag;
  int val;
};
extern int getopt ();
extern int getopt_long (int argc, char *const *argv, const char *shortopts,
          const struct option *longopts, int *longind);
extern int getopt_long_only (int argc, char *const *argv,
        const char *shortopts,
               const struct option *longopts, int *longind);
extern int _getopt_internal (int argc, char *const *argv,
        const char *shortopts,
               const struct option *longopts, int *longind,
        int long_only);
char *optarg = 0;
int optind = 0;
static char *nextchar;
int opterr = 1;
int optopt = '?';
static enum
{
  REQUIRE_ORDER, PERMUTE, RETURN_IN_ORDER
} ordering;
char *getenv ();
static char *
my_index (str, chr)
     const char *str;
     int chr;
{
  while (*str)
    {
      if (*str == chr)
 return (char *) str;
      str++;
    }
  return 0;
}
static void
my_bcopy (from, to, size)
     const char *from;
     char *to;
     int size;
{
  int i;
  for (i = 0; i < size; i++)
    to[i] = from[i];
}
static int first_nonopt;
static int last_nonopt;
static void
exchange (argv)
     char **argv;
{
  int nonopts_size = (last_nonopt - first_nonopt) * sizeof (char *);
  char **temp = (char **) __builtin_alloca (nonopts_size);
  my_bcopy ((char *) &argv[first_nonopt], (char *) temp, nonopts_size);
  my_bcopy ((char *) &argv[last_nonopt], (char *) &argv[first_nonopt],
     (optind - last_nonopt) * sizeof (char *));
  my_bcopy ((char *) temp,
     (char *) &argv[first_nonopt + optind - last_nonopt],
     nonopts_size);
  first_nonopt += (optind - last_nonopt);
  last_nonopt = optind;
}
int
_getopt_internal (argc, argv, optstring, longopts, longind, long_only)
     int argc;
     char *const *argv;
     const char *optstring;
     const struct option *longopts;
     int *longind;
     int long_only;
{
  int option_index;
  optarg = 0;
  if (optind == 0)
    {
      first_nonopt = last_nonopt = optind = 1;
      nextchar = 0;
      if (optstring[0] == '-')
 {
   ordering = RETURN_IN_ORDER;
   ++optstring;
 }
      else if (optstring[0] == '+')
 {
   ordering = REQUIRE_ORDER;
   ++optstring;
 }
      else if (getenv ("POSIXLY_CORRECT") != 0)
 ordering = REQUIRE_ORDER;
      else
 ordering = PERMUTE;
    }
  if (nextchar == 0 || *nextchar == '\0')
    {
      if (ordering == PERMUTE)
 {
   if (first_nonopt != last_nonopt && last_nonopt != optind)
     exchange ((char **) argv);
   else if (last_nonopt != optind)
     first_nonopt = optind;
   while (optind < argc
   && (argv[optind][0] != '-' || argv[optind][1] == '\0')
   )
     optind++;
   last_nonopt = optind;
 }
      if (optind != argc && !strcmp (argv[optind], "--"))
 {
   optind++;
   if (first_nonopt != last_nonopt && last_nonopt != optind)
     exchange ((char **) argv);
   else if (first_nonopt == last_nonopt)
     first_nonopt = optind;
   last_nonopt = argc;
   optind = argc;
 }
      if (optind == argc)
 {
   if (first_nonopt != last_nonopt)
     optind = first_nonopt;
   return EOF;
 }
      if ((argv[optind][0] != '-' || argv[optind][1] == '\0')
   )
 {
   if (ordering == REQUIRE_ORDER)
     return EOF;
   optarg = argv[optind++];
   return 1;
 }
      nextchar = (argv[optind] + 1
    + (longopts != 0 && argv[optind][1] == '-'));
    }
  if (longopts != 0
      && ((argv[optind][0] == '-'
    && (argv[optind][1] == '-' || long_only))
   ))
    {
      const struct option *p;
      char *s = nextchar;
      int exact = 0;
      int ambig = 0;
      const struct option *pfound = 0;
      int indfound;
      while (*s && *s != '=')
 s++;
      for (p = longopts, option_index = 0; p->name;
    p++, option_index++)
 if (!strncmp (p->name, nextchar, s - nextchar))
   {
     if (s - nextchar == strlen (p->name))
       {
  pfound = p;
  indfound = option_index;
  exact = 1;
  break;
       }
     else if (pfound == 0)
       {
  pfound = p;
  indfound = option_index;
       }
     else
       ambig = 1;
   }
      if (ambig && !exact)
 {
   if (opterr)
     fprintf (stdout, "%s: option `%s' is ambiguous\n",
       argv[0], argv[optind]);
   nextchar += strlen (nextchar);
   optind++;
   return '?';
 }
      if (pfound != 0)
 {
   option_index = indfound;
   optind++;
   if (*s)
     {
       if (pfound->has_arg)
  optarg = s + 1;
       else
  {
    if (opterr)
      {
        if (argv[optind - 1][1] == '-')
   fprintf (stdout,
     "%s: option `--%s' doesn't allow an argument\n",
     argv[0], pfound->name);
        else
   fprintf (stdout,
        "%s: option `%c%s' doesn't allow an argument\n",
        argv[0], argv[optind - 1][0], pfound->name);
      }
    nextchar += strlen (nextchar);
    return '?';
  }
     }
   else if (pfound->has_arg == 1)
     {
       if (optind < argc)
  optarg = argv[optind++];
       else
  {
    if (opterr)
      fprintf (stdout, "%s: option `%s' requires an argument\n",
        argv[0], argv[optind - 1]);
    nextchar += strlen (nextchar);
    return optstring[0] == ':' ? ':' : '?';
  }
     }
   nextchar += strlen (nextchar);
   if (longind != 0)
     *longind = option_index;
   if (pfound->flag)
     {
       *(pfound->flag) = pfound->val;
       return 0;
     }
   return pfound->val;
 }
      if (!long_only || argv[optind][1] == '-'
   || my_index (optstring, *nextchar) == 0)
 {
   if (opterr)
     {
       if (argv[optind][1] == '-')
  fprintf (stdout, "%s: unrecognized option `--%s'\n",
    argv[0], nextchar);
       else
  fprintf (stdout, "%s: unrecognized option `%c%s'\n",
    argv[0], argv[optind][0], nextchar);
     }
   nextchar = (char *) "";
   optind++;
   return '?';
 }
    }
  {
    char c = *nextchar++;
    char *temp = my_index (optstring, c);
    if (*nextchar == '\0')
      ++optind;
    if (temp == 0 || c == ':')
      {
 if (opterr)
   {
     fprintf (stdout, "%s: illegal option -- %c\n", argv[0], c);
   }
 optopt = c;
 return '?';
      }
    if (temp[1] == ':')
      {
 if (temp[2] == ':')
   {
     if (*nextchar != '\0')
       {
  optarg = nextchar;
  optind++;
       }
     else
       optarg = 0;
     nextchar = 0;
   }
 else
   {
     if (*nextchar != '\0')
       {
  optarg = nextchar;
  optind++;
       }
     else if (optind == argc)
       {
  if (opterr)
    {
      fprintf (stdout, "%s: option requires an argument -- %c\n",
        argv[0], c);
    }
  optopt = c;
  if (optstring[0] == ':')
    c = ':';
  else
    c = '?';
       }
     else
       optarg = argv[optind++];
     nextchar = 0;
   }
      }
    return c;
  }
}
int
getopt (argc, argv, optstring)
     int argc;
     char *const *argv;
     const char *optstring;
{
  return _getopt_internal (argc, argv, optstring,
      (const struct option *) 0,
      (int *) 0,
      0);
}
static char re_syntax_table[256];
static void
init_syntax_once ()
{
   register int c;
   static int done = 0;
   if (done)
     return;
   memset ((re_syntax_table), 0, (sizeof re_syntax_table));
   for (c = 'a'; c <= 'z'; c++)
     re_syntax_table[c] = 1;
   for (c = 'A'; c <= 'Z'; c++)
     re_syntax_table[c] = 1;
   for (c = '0'; c <= '9'; c++)
     re_syntax_table[c] = 1;
   re_syntax_table['_'] = 1;
   done = 1;
}
typedef unsigned reg_syntax_t;
extern reg_syntax_t re_syntax_options;
typedef enum
{
  REG_NOERROR = 0,
  REG_NOMATCH,
  REG_BADPAT,
  REG_ECOLLATE,
  REG_ECTYPE,
  REG_EESCAPE,
  REG_ESUBREG,
  REG_EBRACK,
  REG_EPAREN,
  REG_EBRACE,
  REG_BADBR,
  REG_ERANGE,
  REG_ESPACE,
  REG_BADRPT,
  REG_EEND,
  REG_ESIZE,
  REG_ERPAREN
} reg_errcode_t;
struct re_pattern_buffer
{
  unsigned char *buffer;
  unsigned long allocated;
  unsigned long used;
  reg_syntax_t syntax;
  char *fastmap;
  char *translate;
  size_t re_nsub;
  unsigned can_be_null : 1;
  unsigned regs_allocated : 2;
  unsigned fastmap_accurate : 1;
  unsigned no_sub : 1;
  unsigned not_bol : 1;
  unsigned not_eol : 1;
  unsigned newline_anchor : 1;
};
typedef struct re_pattern_buffer regex_t;
typedef int regoff_t;
struct re_registers
{
  unsigned num_regs;
  regoff_t *start;
  regoff_t *end;
};
typedef struct
{
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;
extern reg_syntax_t re_set_syntax (reg_syntax_t syntax);
extern const char *re_compile_pattern
  (const char *pattern, int length, struct re_pattern_buffer *buffer)
                                               ;
extern int re_compile_fastmap (struct re_pattern_buffer *buffer);
extern int re_search
  (struct re_pattern_buffer *buffer, const char *string, int length, int start, int range, struct re_registers *regs)
                                                                         ;
extern int re_search_2
  (struct re_pattern_buffer *buffer, const char *string1, int length1, const char *string2, int length2, int start, int range, struct re_registers *regs, int stop)
                                                                        ;
extern int re_match
  (struct re_pattern_buffer *buffer, const char *string, int length, int start, struct re_registers *regs)
                                                               ;
extern int re_match_2
  (struct re_pattern_buffer *buffer, const char *string1, int length1, const char *string2, int length2, int start, struct re_registers *regs, int stop)
                                                             ;
extern void re_set_registers
  (struct re_pattern_buffer *buffer, struct re_registers *regs, unsigned num_regs, regoff_t *starts, regoff_t *ends)
                                                                  ;
extern char *re_comp (const char *);
extern int re_exec (const char *);
extern int regcomp (regex_t *preg, const char *pattern, int cflags);
extern int regexec
  (const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags)
                                              ;
extern size_t regerror
  (int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size)
                                 ;
extern void regfree (regex_t *preg);
typedef char boolean;
typedef enum
{
  no_op = 0,
  exactn = 1,
  anychar,
  charset,
  charset_not,
  start_memory,
  stop_memory,
  duplicate,
  begline,
  endline,
  begbuf,
  endbuf,
  jump,
  jump_past_alt,
  on_failure_jump,
  on_failure_keep_string_jump,
  pop_failure_jump,
  maybe_pop_jump,
  dummy_failure_jump,
  push_dummy_failure,
  succeed_n,
  jump_n,
  set_number_at,
  wordchar,
  notwordchar,
  wordbeg,
  wordend,
  wordbound,
  notwordbound
} re_opcode_t;
reg_syntax_t re_syntax_options = 0;
reg_syntax_t
re_set_syntax (syntax)
    reg_syntax_t syntax;
{
  reg_syntax_t ret = re_syntax_options;
  re_syntax_options = syntax;
  return ret;
}
static const char *re_error_msg[] =
  { 0,
    "No match",
    "Invalid regular expression",
    "Invalid collation character",
    "Invalid character class name",
    "Trailing backslash",
    "Invalid back reference",
    "Unmatched [ or [^",
    "Unmatched ( or \\(",
    "Unmatched \\{",
    "Invalid content of \\{\\}",
    "Invalid range end",
    "Memory exhausted",
    "Invalid preceding regular expression",
    "Premature end of regular expression",
    "Regular expression too big",
    "Unmatched ) or \\)",
  };
static void store_op1 (), store_op2 ();
static void insert_op1 (), insert_op2 ();
static boolean at_begline_loc_p (), at_endline_loc_p ();
static boolean group_in_compile_stack ();
static reg_errcode_t compile_range ();
typedef unsigned regnum_t;
typedef int pattern_offset_t;
typedef struct
{
  pattern_offset_t begalt_offset;
  pattern_offset_t fixup_alt_jump;
  pattern_offset_t inner_group_offset;
  pattern_offset_t laststart_offset;
  regnum_t regnum;
} compile_stack_elt_t;
typedef struct
{
  compile_stack_elt_t *stack;
  unsigned size;
  unsigned avail;
} compile_stack_type;
static reg_errcode_t
regex_compile (pattern, size, syntax, bufp)
     const char *pattern;
     int size;
     reg_syntax_t syntax;
     struct re_pattern_buffer *bufp;
{
  register unsigned char c, c1;
  const char *p1;
  register unsigned char *b;
  compile_stack_type compile_stack;
  const char *p = pattern;
  const char *pend = pattern + size;
  char *translate = bufp->translate;
  unsigned char *pending_exact = 0;
  unsigned char *laststart = 0;
  unsigned char *begalt;
  const char *beg_interval;
  unsigned char *fixup_alt_jump = 0;
  regnum_t regnum = 0;
  compile_stack.stack = ((compile_stack_elt_t *) malloc ((32) * sizeof (compile_stack_elt_t)));
  if (compile_stack.stack == 0)
    return REG_ESPACE;
  compile_stack.size = 32;
  compile_stack.avail = 0;
  bufp->syntax = syntax;
  bufp->fastmap_accurate = 0;
  bufp->not_bol = bufp->not_eol = 0;
  bufp->used = 0;
  bufp->re_nsub = 0;
   init_syntax_once ();
  if (bufp->allocated == 0)
    {
      if (bufp->buffer)
 {
          ((bufp->buffer) = (unsigned char *) realloc (bufp->buffer, (32) * sizeof (unsigned char)));
        }
      else
        {
          bufp->buffer = ((unsigned char *) malloc ((32) * sizeof (unsigned char)));
        }
      if (!bufp->buffer) return REG_ESPACE;
      bufp->allocated = 32;
    }
  begalt = b = bufp->buffer;
  while (p != pend)
    {
      do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
      switch (c)
        {
        case '^':
          {
            if (
                   p == pattern + 1
                || syntax & ((((1) << 1) << 1) << 1)
                || at_begline_loc_p (pattern, p, syntax))
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (begline); } while (0);
            else
              goto normal_char;
          }
          break;
        case '$':
          {
            if (
                   p == pend
                || syntax & ((((1) << 1) << 1) << 1)
                || at_endline_loc_p (p, pend, syntax))
               do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (endline); } while (0);
             else
               goto normal_char;
           }
           break;
 case '+':
        case '?':
          if ((syntax & ((1) << 1))
              || (syntax & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
            goto normal_char;
        handle_plus:
        case '*':
          if (!laststart)
            {
              if (syntax & ((((((1) << 1) << 1) << 1) << 1) << 1))
                return REG_BADRPT;
              else if (!(syntax & (((((1) << 1) << 1) << 1) << 1)))
                goto normal_char;
            }
          {
            boolean keep_string_p = 0;
            char zero_times_ok = 0, many_times_ok = 0;
            for (;;)
              {
                zero_times_ok |= c != '+';
                many_times_ok |= c != '?';
                if (p == pend)
                  break;
                do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
                if (c == '*'
                    || (!(syntax & ((1) << 1)) && (c == '+' || c == '?')))
                  ;
                else if (syntax & ((1) << 1) && c == '\\')
                  {
                    if (p == pend) return REG_EESCAPE;
                    do {if (p == pend) return REG_EEND; c1 = (unsigned char) *p++; if (translate) c1 = translate[c1]; } while (0);
                    if (!(c1 == '+' || c1 == '?'))
                      {
                        p--;
                        p--;
                        break;
                      }
                    c = c1;
                  }
                else
                  {
                    p--;
                    break;
                  }
               }
            if (!laststart)
              break;
            if (many_times_ok)
              {
                ;
                while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
                if ((translate ? translate[(unsigned char) (*(p - 2))] : (*(p - 2))) == (translate ? translate[(unsigned char) ('.')] : ('.'))
      && zero_times_ok
                    && p < pend && (translate ? translate[(unsigned char) (*p)] : (*p)) == (translate ? translate[(unsigned char) ('\n')] : ('\n'))
                    && !(syntax & (((((((1) << 1) << 1) << 1) << 1) << 1) << 1)))
                  {
                    store_op1 (jump, b, (laststart) - (b) - 3);
                    keep_string_p = 1;
                  }
                else
                  store_op1 (maybe_pop_jump, b, (laststart - 3) - (b) - 3);
                b += 3;
              }
            while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
            insert_op1 (keep_string_p ? on_failure_keep_string_jump : on_failure_jump, laststart, (b + 3) - (laststart) - 3, b)
                                          ;
            pending_exact = 0;
            b += 3;
            if (!zero_times_ok)
              {
                while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
                insert_op1 (dummy_failure_jump, laststart, (laststart + 6) - (laststart) - 3, b);
                b += 3;
              }
            }
   break;
 case '.':
          laststart = b;
          do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (anychar); } while (0);
          break;
        case '[':
          {
            boolean had_char_class = 0;
            if (p == pend) return REG_EBRACK;
     while (b - bufp->buffer + (34) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
            laststart = b;
            do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (*p == '^' ? charset_not : charset); } while (0);
            if (*p == '^')
              p++;
            p1 = p;
            do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) ((1 << 8) / 8); } while (0);
            memset ((b), 0, ((1 << 8) / 8));
            if ((re_opcode_t) b[-2] == charset_not
                && (syntax & (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
              (b[((unsigned char) ('\n')) / 8] |= 1 << (((unsigned char) '\n') % 8));
            for (;;)
              {
                if (p == pend) return REG_EBRACK;
                do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
                if ((syntax & (1)) && c == '\\')
                  {
                    if (p == pend) return REG_EESCAPE;
                    do {if (p == pend) return REG_EEND; c1 = (unsigned char) *p++; if (translate) c1 = translate[c1]; } while (0);
                    (b[((unsigned char) (c1)) / 8] |= 1 << (((unsigned char) c1) % 8));
                    continue;
                  }
                if (c == ']' && p != p1 + 1)
                  break;
                if (had_char_class && c == '-' && *p != ']')
                  return REG_ERANGE;
                if (c == '-'
                    && !(p - 2 >= pattern && p[-2] == '[')
                    && !(p - 3 >= pattern && p[-3] == '[' && p[-2] == '^')
                    && *p != ']')
                  {
                    reg_errcode_t ret
                      = compile_range (&p, pend, translate, syntax, b);
                    if (ret != REG_NOERROR) return ret;
                  }
                else if (p[0] == '-' && p[1] != ']')
                  {
                    reg_errcode_t ret;
                    do {if (p == pend) return REG_EEND; c1 = (unsigned char) *p++; if (translate) c1 = translate[c1]; } while (0);
                    ret = compile_range (&p, pend, translate, syntax, b);
                    if (ret != REG_NOERROR) return ret;
                  }
                else if (syntax & (((1) << 1) << 1) && c == '[' && *p == ':')
                  {
                    char str[6 + 1];
                    do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
                    c1 = 0;
                    if (p == pend) return REG_EBRACK;
                    for (;;)
                      {
                        do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
                        if (c == ':' || c == ']' || p == pend
                            || c1 == 6)
                          break;
                        str[c1++] = c;
                      }
                    str[c1] = '\0';
                    if (c == ':' && *p == ']')
                      {
                        int ch;
                        boolean is_alnum = ((strcmp (str, "alnum") == 0));
                        boolean is_alpha = ((strcmp (str, "alpha") == 0));
                        boolean is_blank = ((strcmp (str, "blank") == 0));
                        boolean is_cntrl = ((strcmp (str, "cntrl") == 0));
                        boolean is_digit = ((strcmp (str, "digit") == 0));
                        boolean is_graph = ((strcmp (str, "graph") == 0));
                        boolean is_lower = ((strcmp (str, "lower") == 0));
                        boolean is_print = ((strcmp (str, "print") == 0));
                        boolean is_punct = ((strcmp (str, "punct") == 0));
                        boolean is_space = ((strcmp (str, "space") == 0));
                        boolean is_upper = ((strcmp (str, "upper") == 0));
                        boolean is_xdigit = ((strcmp (str, "xdigit") == 0));
                        if (!(((strcmp (str, "alpha") == 0)) || ((strcmp (str, "upper") == 0)) || ((strcmp (str, "lower") == 0)) || ((strcmp (str, "digit") == 0)) || ((strcmp (str, "alnum") == 0)) || ((strcmp (str, "xdigit") == 0)) || ((strcmp (str, "space") == 0)) || ((strcmp (str, "print") == 0)) || ((strcmp (str, "punct") == 0)) || ((strcmp (str, "graph") == 0)) || ((strcmp (str, "cntrl") == 0)) || ((strcmp (str, "blank") == 0)))) return REG_ECTYPE;
                        do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
                        if (p == pend) return REG_EBRACK;
                        for (ch = 0; ch < 1 << 8; ch++)
                          {
                            if ( (is_alnum && (1 && isalnum (ch)))
                                || (is_alpha && (1 && isalpha (ch)))
                                || (is_blank && ((ch) == ' ' || (ch) == '\t'))
                                || (is_cntrl && (1 && iscntrl (ch)))
                                || (is_digit && (1 && isdigit (ch)))
                                || (is_graph && (1 && isprint (ch) && !isspace (ch)))
                                || (is_lower && (1 && islower (ch)))
                                || (is_print && (1 && isprint (ch)))
                                || (is_punct && (1 && ispunct (ch)))
                                || (is_space && (1 && isspace (ch)))
                                || (is_upper && (1 && isupper (ch)))
                                || (is_xdigit && (1 && isxdigit (ch))))
                            (b[((unsigned char) (ch)) / 8] |= 1 << (((unsigned char) ch) % 8));
                          }
                        had_char_class = 1;
                      }
                    else
                      {
                        c1++;
                        while (c1--)
                          p--;
                        (b[((unsigned char) ('[')) / 8] |= 1 << (((unsigned char) '[') % 8));
                        (b[((unsigned char) (':')) / 8] |= 1 << (((unsigned char) ':') % 8));
                        had_char_class = 0;
                      }
                  }
                else
                  {
                    had_char_class = 0;
                    (b[((unsigned char) (c)) / 8] |= 1 << (((unsigned char) c) % 8));
                  }
              }
            while ((int) b[-1] > 0 && b[b[-1] - 1] == 0)
              b[-1]--;
            b += b[-1];
          }
          break;
 case '(':
          if (syntax & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
            goto handle_open;
          else
            goto normal_char;
        case ')':
          if (syntax & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
            goto handle_close;
          else
            goto normal_char;
        case '\n':
          if (syntax & ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
            goto handle_alt;
          else
            goto normal_char;
 case '|':
          if (syntax & ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
            goto handle_alt;
          else
            goto normal_char;
        case '{':
           if (syntax & ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) && syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
             goto handle_interval;
           else
             goto normal_char;
        case '\\':
          if (p == pend) return REG_EESCAPE;
          do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; } while (0);
          switch (c)
            {
            case '(':
              if (syntax & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                goto normal_backslash;
            handle_open:
              bufp->re_nsub++;
              regnum++;
              if ((compile_stack.avail == compile_stack.size))
                {
                  ((compile_stack.stack) = (compile_stack_elt_t *) realloc (compile_stack.stack, (compile_stack.size << 1) * sizeof (compile_stack_elt_t)))
                                                ;
                  if (compile_stack.stack == 0) return REG_ESPACE;
                  compile_stack.size <<= 1;
                }
              (compile_stack.stack[compile_stack.avail]).begalt_offset = begalt - bufp->buffer;
              (compile_stack.stack[compile_stack.avail]).fixup_alt_jump
                = fixup_alt_jump ? fixup_alt_jump - bufp->buffer + 1 : 0;
              (compile_stack.stack[compile_stack.avail]).laststart_offset = b - bufp->buffer;
              (compile_stack.stack[compile_stack.avail]).regnum = regnum;
              if (regnum <= 255)
                {
                  (compile_stack.stack[compile_stack.avail]).inner_group_offset = b - bufp->buffer + 2;
                  do { while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (start_memory); *b++ = (unsigned char) (regnum); *b++ = (unsigned char) (0); } while (0);
                }
              compile_stack.avail++;
              fixup_alt_jump = 0;
              laststart = 0;
              begalt = b;
       pending_exact = 0;
              break;
            case ')':
              if (syntax & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) goto normal_backslash;
              if ((compile_stack.avail == 0))
                if (syntax & ((((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                  goto normal_backslash;
                else
                  return REG_ERPAREN;
            handle_close:
              if (fixup_alt_jump)
                {
                  do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (push_dummy_failure); } while (0);
                  store_op1 (jump_past_alt, fixup_alt_jump, (b - 1) - (fixup_alt_jump) - 3);
                }
              if ((compile_stack.avail == 0))
                if (syntax & ((((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                  goto normal_char;
                else
                  return REG_ERPAREN;
              ;
              {
                regnum_t this_group_regnum;
                compile_stack.avail--;
                begalt = bufp->buffer + (compile_stack.stack[compile_stack.avail]).begalt_offset;
                fixup_alt_jump
                  = (compile_stack.stack[compile_stack.avail]).fixup_alt_jump
                    ? bufp->buffer + (compile_stack.stack[compile_stack.avail]).fixup_alt_jump - 1
                    : 0;
                laststart = bufp->buffer + (compile_stack.stack[compile_stack.avail]).laststart_offset;
                this_group_regnum = (compile_stack.stack[compile_stack.avail]).regnum;
  pending_exact = 0;
                if (this_group_regnum <= 255)
                  {
                    unsigned char *inner_group_loc
                      = bufp->buffer + (compile_stack.stack[compile_stack.avail]).inner_group_offset;
                    *inner_group_loc = regnum - this_group_regnum;
                    do { while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (stop_memory); *b++ = (unsigned char) (this_group_regnum); *b++ = (unsigned char) (regnum - this_group_regnum); } while (0)
                                                           ;
                  }
              }
              break;
            case '|':
              if (syntax & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) || syntax & ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                goto normal_backslash;
            handle_alt:
              if (syntax & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                goto normal_char;
              while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
              insert_op1 (on_failure_jump, begalt, (b + 6) - (begalt) - 3, b);
              pending_exact = 0;
              b += 3;
              if (fixup_alt_jump)
                store_op1 (jump_past_alt, fixup_alt_jump, (b) - (fixup_alt_jump) - 3);
              fixup_alt_jump = b;
              while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
              b += 3;
              laststart = 0;
              begalt = b;
              break;
            case '{':
              if (!(syntax & ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                  || ((syntax & ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) && (syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
                  || (p - 2 == pattern && p == pend))
                goto normal_backslash;
            handle_interval:
              {
                int lower_bound = -1, upper_bound = -1;
                beg_interval = p - 1;
                if (p == pend)
                  {
                    if (syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                      goto unfetch_interval;
                    else
                      return REG_EBRACE;
                  }
                { if (p != pend) { do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0); while ((1 && isdigit (c))) { if (lower_bound < 0) lower_bound = 0; lower_bound = lower_bound * 10 + c - '0'; if (p == pend) break; do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0); } } };
                if (c == ',')
                  {
                    { if (p != pend) { do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0); while ((1 && isdigit (c))) { if (upper_bound < 0) upper_bound = 0; upper_bound = upper_bound * 10 + c - '0'; if (p == pend) break; do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0); } } };
                    if (upper_bound < 0) upper_bound = ((1 << 15) - 1);
                  }
                else
                  upper_bound = lower_bound;
                if (lower_bound < 0 || upper_bound > ((1 << 15) - 1)
                    || lower_bound > upper_bound)
                  {
                    if (syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                      goto unfetch_interval;
                    else
                      return REG_BADBR;
                  }
                if (!(syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
                  {
                    if (c != '\\') return REG_EBRACE;
                    do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
                  }
                if (c != '}')
                  {
                    if (syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                      goto unfetch_interval;
                    else
                      return REG_BADBR;
                  }
                if (!laststart)
                  {
                    if (syntax & ((((((1) << 1) << 1) << 1) << 1) << 1))
                      return REG_BADRPT;
                    else if (syntax & (((((1) << 1) << 1) << 1) << 1))
                      laststart = b;
                    else
                      goto unfetch_interval;
                  }
                 if (upper_bound == 0)
                   {
                     while (b - bufp->buffer + (3) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
                     insert_op1 (jump, laststart, (b + 3) - (laststart) - 3, b);
                     b += 3;
                   }
                 else
                   {
                     unsigned nbytes = 10 + (upper_bound > 1) * 10;
                     while (b - bufp->buffer + (nbytes) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0);
                     insert_op2 (succeed_n, laststart, (b + 5 + (upper_bound > 1) * 5) - (laststart) - 3, lower_bound, b)
                                               ;
                     b += 5;
                     insert_op2 (set_number_at, laststart, 5, lower_bound, b);
                     b += 5;
                     if (upper_bound > 1)
                       {
                         store_op2 (jump_n, b, (laststart + 5) - (b) - 3, upper_bound - 1)
                                                      ;
                         b += 5;
                         insert_op2 (set_number_at, laststart, b - laststart,
                                     upper_bound - 1, b);
                         b += 5;
                       }
                   }
                pending_exact = 0;
                beg_interval = 0;
              }
              break;
            unfetch_interval:
               ;
               p = beg_interval;
               beg_interval = 0;
               do {if (p == pend) return REG_EEND; c = (unsigned char) *p++; if (translate) c = translate[c]; } while (0);
               if (!(syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
                 {
                   if (p > pattern && p[-1] == '\\')
                     goto normal_backslash;
                 }
               goto normal_char;
            case 'w':
              laststart = b;
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (wordchar); } while (0);
              break;
            case 'W':
              laststart = b;
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (notwordchar); } while (0);
              break;
            case '<':
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (wordbeg); } while (0);
              break;
            case '>':
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (wordend); } while (0);
              break;
            case 'b':
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (wordbound); } while (0);
              break;
            case 'B':
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (notwordbound); } while (0);
              break;
            case '`':
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (begbuf); } while (0);
              break;
            case '\'':
              do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (endbuf); } while (0);
              break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
              if (syntax & (((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                goto normal_char;
              c1 = c - '0';
              if (c1 > regnum)
                return REG_ESUBREG;
              if (group_in_compile_stack (compile_stack, c1))
                goto normal_char;
              laststart = b;
              do { while (b - bufp->buffer + (2) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (duplicate); *b++ = (unsigned char) (c1); } while (0);
              break;
            case '+':
            case '?':
              if (syntax & ((1) << 1))
                goto handle_plus;
              else
                goto normal_backslash;
            default:
            normal_backslash:
              c = (translate ? translate[(unsigned char) (c)] : (c));
              goto normal_char;
            }
          break;
 default:
 normal_char:
          if (!pending_exact
              || pending_exact + *pending_exact + 1 != b
       || *pending_exact == (1 << 8) - 1
              || *p == '*' || *p == '^'
       || ((syntax & ((1) << 1))
    ? *p == '\\' && (p[1] == '+' || p[1] == '?')
    : (*p == '+' || *p == '?'))
       || ((syntax & ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
                  && ((syntax & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
        ? *p == '{'
                      : (p[0] == '\\' && p[1] == '{'))))
     {
              laststart = b;
       do { while (b - bufp->buffer + (2) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (exactn); *b++ = (unsigned char) (0); } while (0);
       pending_exact = b - 1;
            }
   do { while (b - bufp->buffer + (1) > bufp->allocated) do { unsigned char *old_buffer = bufp->buffer; if (bufp->allocated == (1L << 16)) return REG_ESIZE; bufp->allocated <<= 1; if (bufp->allocated > (1L << 16)) bufp->allocated = (1L << 16); bufp->buffer = (unsigned char *) realloc (bufp->buffer, bufp->allocated); if (bufp->buffer == 0) return REG_ESPACE; if (old_buffer != bufp->buffer) { b = (b - old_buffer) + bufp->buffer; begalt = (begalt - old_buffer) + bufp->buffer; if (fixup_alt_jump) fixup_alt_jump = (fixup_alt_jump - old_buffer) + bufp->buffer; if (laststart) laststart = (laststart - old_buffer) + bufp->buffer; if (pending_exact) pending_exact = (pending_exact - old_buffer) + bufp->buffer; } } while (0); *b++ = (unsigned char) (c); } while (0);
          (*pending_exact)++;
   break;
        }
    }
  if (fixup_alt_jump)
    store_op1 (jump_past_alt, fixup_alt_jump, (b) - (fixup_alt_jump) - 3);
  if (!(compile_stack.avail == 0))
    return REG_EPAREN;
  free (compile_stack.stack);
  bufp->used = b - bufp->buffer;
  return REG_NOERROR;
}
static void
store_op1 (op, loc, arg)
    re_opcode_t op;
    unsigned char *loc;
    int arg;
{
  *loc = (unsigned char) op;
  do { (loc + 1)[0] = (arg) & 0377; (loc + 1)[1] = (arg) >> 8; } while (0);
}
static void
store_op2 (op, loc, arg1, arg2)
    re_opcode_t op;
    unsigned char *loc;
    int arg1, arg2;
{
  *loc = (unsigned char) op;
  do { (loc + 1)[0] = (arg1) & 0377; (loc + 1)[1] = (arg1) >> 8; } while (0);
  do { (loc + 3)[0] = (arg2) & 0377; (loc + 3)[1] = (arg2) >> 8; } while (0);
}
static void
insert_op1 (op, loc, arg, end)
    re_opcode_t op;
    unsigned char *loc;
    int arg;
    unsigned char *end;
{
  register unsigned char *pfrom = end;
  register unsigned char *pto = end + 3;
  while (pfrom != loc)
    *--pto = *--pfrom;
  store_op1 (op, loc, arg);
}
static void
insert_op2 (op, loc, arg1, arg2, end)
    re_opcode_t op;
    unsigned char *loc;
    int arg1, arg2;
    unsigned char *end;
{
  register unsigned char *pfrom = end;
  register unsigned char *pto = end + 5;
  while (pfrom != loc)
    *--pto = *--pfrom;
  store_op2 (op, loc, arg1, arg2);
}
static boolean
at_begline_loc_p (pattern, p, syntax)
    const char *pattern, *p;
    reg_syntax_t syntax;
{
  const char *prev = p - 2;
  boolean prev_prev_backslash = prev > pattern && prev[-1] == '\\';
  return
       (*prev == '(' && (syntax & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) || prev_prev_backslash))
    || (*prev == '|' && (syntax & ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) || prev_prev_backslash));
}
static boolean
at_endline_loc_p (p, pend, syntax)
    const char *p, *pend;
    int syntax;
{
  const char *next = p;
  boolean next_backslash = *next == '\\';
  const char *next_next = p + 1 < pend ? p + 1 : 0;
  return
       (syntax & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) ? *next == ')'
        : next_backslash && next_next && *next_next == ')')
    || (syntax & ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) ? *next == '|'
        : next_backslash && next_next && *next_next == '|');
}
static boolean
group_in_compile_stack (compile_stack, regnum)
    compile_stack_type compile_stack;
    regnum_t regnum;
{
  int this_element;
  for (this_element = compile_stack.avail - 1;
       this_element >= 0;
       this_element--)
    if (compile_stack.stack[this_element].regnum == regnum)
      return 1;
  return 0;
}
static reg_errcode_t
compile_range (p_ptr, pend, translate, syntax, b)
    const char **p_ptr, *pend;
    char *translate;
    reg_syntax_t syntax;
    unsigned char *b;
{
  unsigned this_char;
  const char *p = *p_ptr;
  int range_start, range_end;
  if (p == pend)
    return REG_ERANGE;
  range_start = ((unsigned char *) p)[-2];
  range_end = ((unsigned char *) p)[0];
  (*p_ptr)++;
  if (range_start > range_end)
    return syntax & (((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) ? REG_ERANGE : REG_NOERROR;
  for (this_char = range_start; this_char <= range_end; this_char++)
    {
      (b[((unsigned char) ((translate ? translate[(unsigned char) (this_char)] : (this_char)))) / 8] |= 1 << (((unsigned char) (translate ? translate[(unsigned char) (this_char)] : (this_char))) % 8));
    }
  return REG_NOERROR;
}
int re_max_failures = 2000;
typedef const unsigned char *fail_stack_elt_t;
typedef struct
{
  fail_stack_elt_t *stack;
  unsigned size;
  unsigned avail;
} fail_stack_type;
int
re_compile_fastmap (bufp)
     struct re_pattern_buffer *bufp;
{
  int j, k;
  fail_stack_type fail_stack;
  char *destination;
  unsigned num_regs = 0;
  register char *fastmap = bufp->fastmap;
  unsigned char *pattern = bufp->buffer;
  unsigned long size = bufp->used;
  const unsigned char *p = pattern;
  register unsigned char *pend = pattern + size;
  boolean path_can_be_null = 1;
  boolean succeed_n_p = 0;
  ;
  do { fail_stack.stack = (fail_stack_elt_t *) __builtin_alloca (5 * sizeof (fail_stack_elt_t)); if (fail_stack.stack == 0) return -2; fail_stack.size = 5; fail_stack.avail = 0; } while (0);
  memset ((fastmap), 0, (1 << 8));
  bufp->fastmap_accurate = 1;
  bufp->can_be_null = 0;
  while (p != pend || !(fail_stack.avail == 0))
    {
      if (p == pend)
        {
          bufp->can_be_null |= path_can_be_null;
          path_can_be_null = 1;
          p = fail_stack.stack[--fail_stack.avail];
 }
      ;
      switch ((re_opcode_t) *p++)
 {
 case duplicate:
   bufp->can_be_null = 1;
          return 0;
 case exactn:
          fastmap[p[1]] = 1;
   break;
        case charset:
          for (j = *p++ * 8 - 1; j >= 0; j--)
     if (p[j / 8] & (1 << (j % 8)))
              fastmap[j] = 1;
   break;
 case charset_not:
   for (j = *p * 8; j < (1 << 8); j++)
            fastmap[j] = 1;
   for (j = *p++ * 8 - 1; j >= 0; j--)
     if (!(p[j / 8] & (1 << (j % 8))))
              fastmap[j] = 1;
          break;
 case wordchar:
   for (j = 0; j < (1 << 8); j++)
     if (re_syntax_table[j] == 1)
       fastmap[j] = 1;
   break;
 case notwordchar:
   for (j = 0; j < (1 << 8); j++)
     if (re_syntax_table[j] != 1)
       fastmap[j] = 1;
   break;
        case anychar:
   for (j = 0; j < (1 << 8); j++)
            fastmap[j] = 1;
          if (!(bufp->syntax & (((((((1) << 1) << 1) << 1) << 1) << 1) << 1)))
            fastmap['\n'] = 0;
   else if (bufp->can_be_null)
     return 0;
   break;
        case no_op:
        case begline:
        case endline:
 case begbuf:
 case endbuf:
 case wordbound:
 case notwordbound:
 case wordbeg:
 case wordend:
        case push_dummy_failure:
          continue;
 case jump_n:
        case pop_failure_jump:
 case maybe_pop_jump:
 case jump:
        case jump_past_alt:
 case dummy_failure_jump:
          do { do { (j) = *(p) & 0377; (j) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
   p += j;
   if (j > 0)
     continue;
          if ((re_opcode_t) *p != on_failure_jump
       && (re_opcode_t) *p != succeed_n)
     continue;
          p++;
          do { do { (j) = *(p) & 0377; (j) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          p += j;
          if (!(fail_stack.avail == 0)
       && fail_stack.stack[fail_stack.avail - 1] == p)
            fail_stack.avail--;
          continue;
        case on_failure_jump:
        case on_failure_keep_string_jump:
 handle_on_failure_jump:
          do { do { (j) = *(p) & 0377; (j) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          if (p + j < pend)
            {
              if (!(((fail_stack.avail == fail_stack.size) && !((fail_stack).size > re_max_failures * ((num_regs - 1) * 3 + 4) ? 0 : ((fail_stack).stack = (fail_stack_elt_t *) (destination = (char *) __builtin_alloca (((fail_stack).size << 1) * sizeof (fail_stack_elt_t)), memcpy ((destination), ((fail_stack).stack), ((fail_stack).size * sizeof (fail_stack_elt_t))), destination), (fail_stack).stack == 0 ? 0 : ((fail_stack).size <<= 1, 1)))) ? 0 : ((fail_stack).stack[(fail_stack).avail++] = p + j, 1)))
                return -2;
            }
          else
            bufp->can_be_null = 1;
          if (succeed_n_p)
            {
              do { do { (k) = *(p) & 0377; (k) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
              succeed_n_p = 0;
     }
          continue;
 case succeed_n:
          p += 2;
          do { do { (k) = *(p) & 0377; (k) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          if (k == 0)
     {
              p -= 4;
         succeed_n_p = 1;
              goto handle_on_failure_jump;
            }
          continue;
 case set_number_at:
          p += 4;
          continue;
 case start_memory:
        case stop_memory:
   p += 2;
   continue;
 default:
          abort ();
        }
      path_can_be_null = 0;
      p = pend;
    }
  bufp->can_be_null |= path_can_be_null;
  return 0;
}
void
re_set_registers (bufp, regs, num_regs, starts, ends)
    struct re_pattern_buffer *bufp;
    struct re_registers *regs;
    unsigned num_regs;
    regoff_t *starts, *ends;
{
  if (num_regs)
    {
      bufp->regs_allocated = 1;
      regs->num_regs = num_regs;
      regs->start = starts;
      regs->end = ends;
    }
  else
    {
      bufp->regs_allocated = 0;
      regs->num_regs = 0;
      regs->start = regs->end = (regoff_t) 0;
    }
}
int
re_search (bufp, string, size, startpos, range, regs)
     struct re_pattern_buffer *bufp;
     const char *string;
     int size, startpos, range;
     struct re_registers *regs;
{
  return re_search_2 (bufp, 0, 0, string, size, startpos, range,
        regs, size);
}
int
re_search_2 (bufp, string1, size1, string2, size2, startpos, range, regs, stop)
     struct re_pattern_buffer *bufp;
     const char *string1, *string2;
     int size1, size2;
     int startpos;
     int range;
     struct re_registers *regs;
     int stop;
{
  int val;
  register char *fastmap = bufp->fastmap;
  register char *translate = bufp->translate;
  int total_size = size1 + size2;
  int endpos = startpos + range;
  if (startpos < 0 || startpos > total_size)
    return -1;
  if (endpos < -1)
    range = -1 - startpos;
  else if (endpos > total_size)
    range = total_size - startpos;
  if (bufp->used > 0 && (re_opcode_t) bufp->buffer[0] == begbuf && range > 0)
    {
      if (startpos > 0)
 return -1;
      else
 range = 1;
    }
  if (fastmap && !bufp->fastmap_accurate)
    if (re_compile_fastmap (bufp) == -2)
      return -2;
  for (;;)
    {
      if (fastmap && startpos < total_size && !bufp->can_be_null)
 {
   if (range > 0)
     {
       register const char *d;
       register int lim = 0;
       int irange = range;
              if (startpos < size1 && startpos + range >= size1)
                lim = range - (size1 - startpos);
       d = (startpos >= size1 ? string2 - size1 : string1) + startpos;
       if (translate)
                while (range > lim
                       && !fastmap[(unsigned char)
       translate[(unsigned char) *d++]])
                  range--;
       else
                while (range > lim && !fastmap[(unsigned char) *d++])
                  range--;
       startpos += irange - range;
     }
   else
     {
       register char c = (size1 == 0 || startpos >= size1
                                 ? string2[startpos - size1]
                                 : string1[startpos]);
       if (!fastmap[(unsigned char) (translate ? translate[(unsigned char) (c)] : (c))])
  goto advance;
     }
 }
      if (range >= 0 && startpos == total_size && fastmap
          && !bufp->can_be_null)
 return -1;
      val = re_match_2 (bufp, string1, size1, string2, size2,
                 startpos, regs, stop);
      if (val >= 0)
 return startpos;
      if (val == -2)
 return -2;
    advance:
      if (!range)
        break;
      else if (range > 0)
        {
          range--;
          startpos++;
        }
      else
        {
          range++;
          startpos--;
        }
    }
  return -1;
}
static int bcmp_translate ();
static boolean alt_match_null_string_p (),
               common_op_match_null_string_p (),
               group_match_null_string_p ();
typedef union
{
  fail_stack_elt_t word;
  struct
  {
    unsigned match_null_string_p : 2;
    unsigned is_active : 1;
    unsigned matched_something : 1;
    unsigned ever_matched_something : 1;
  } bits;
} register_info_type;
int
re_match (bufp, string, size, pos, regs)
     struct re_pattern_buffer *bufp;
     const char *string;
     int size, pos;
     struct re_registers *regs;
 {
  return re_match_2 (bufp, 0, 0, string, size, pos, regs, size);
}
int
re_match_2 (bufp, string1, size1, string2, size2, pos, regs, stop)
     struct re_pattern_buffer *bufp;
     const char *string1, *string2;
     int size1, size2;
     int pos;
     struct re_registers *regs;
     int stop;
{
  int mcnt;
  unsigned char *p1;
  const char *end1, *end2;
  const char *end_match_1, *end_match_2;
  const char *d, *dend;
  unsigned char *p = bufp->buffer;
  register unsigned char *pend = p + bufp->used;
  char *translate = bufp->translate;
  fail_stack_type fail_stack;
  unsigned num_regs = bufp->re_nsub + 1;
  unsigned lowest_active_reg = ((1 << 8) + 1);
  unsigned highest_active_reg = (1 << 8);
  const char **regstart, **regend;
  const char **old_regstart, **old_regend;
  register_info_type *reg_info;
  unsigned best_regs_set = 0;
  const char **best_regstart, **best_regend;
  const char *match_end = 0;
  const char **reg_dummy;
  register_info_type *reg_info_dummy;
  ;
  do { fail_stack.stack = (fail_stack_elt_t *) __builtin_alloca (5 * sizeof (fail_stack_elt_t)); if (fail_stack.stack == 0) return -2; fail_stack.size = 5; fail_stack.avail = 0; } while (0);
  if (bufp->re_nsub)
    {
      regstart = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      regend = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      old_regstart = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      old_regend = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      best_regstart = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      best_regend = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      reg_info = ((register_info_type *) __builtin_alloca ((num_regs) * sizeof (register_info_type)));
      reg_dummy = ((const char * *) __builtin_alloca ((num_regs) * sizeof (const char *)));
      reg_info_dummy = ((register_info_type *) __builtin_alloca ((num_regs) * sizeof (register_info_type)));
      if (!(regstart && regend && old_regstart && old_regend && reg_info
            && best_regstart && best_regend && reg_dummy && reg_info_dummy))
        {
          __builtin_alloca (0);
          return -2;
        }
    }
  if (pos < 0 || pos > size1 + size2)
    {
      __builtin_alloca (0);
      return -1;
    }
  for (mcnt = 1; mcnt < num_regs; mcnt++)
    {
      regstart[mcnt] = regend[mcnt]
        = old_regstart[mcnt] = old_regend[mcnt] = ((char *) -1);
      ((reg_info[mcnt]).bits.match_null_string_p) = 3;
      ((reg_info[mcnt]).bits.is_active) = 0;
      ((reg_info[mcnt]).bits.matched_something) = 0;
      ((reg_info[mcnt]).bits.ever_matched_something) = 0;
    }
  if (size2 == 0 && string1 != 0)
    {
      string2 = string1;
      size2 = size1;
      string1 = 0;
      size1 = 0;
    }
  end1 = string1 + size1;
  end2 = string2 + size2;
  if (stop <= size1)
    {
      end_match_1 = string1 + stop;
      end_match_2 = string2;
    }
  else
    {
      end_match_1 = end1;
      end_match_2 = string2 + stop - size1;
    }
  if (size1 > 0 && pos <= size1)
    {
      d = string1 + pos;
      dend = end_match_1;
    }
  else
    {
      d = string2 + pos - size1;
      dend = end_match_2;
    }
  ;
  ;
  ;
  ;
  ;
  for (;;)
    {
      ;
      if (p == pend)
 {
          ;
          if (d != end_match_2)
     {
              ;
              if (!(fail_stack.avail == 0))
                {
                  boolean same_str_p = ((size1 && string1 <= (match_end) && (match_end) <= string1 + size1)
                          == (dend == end_match_1));
                  if (!best_regs_set
                      || (same_str_p && d > match_end)
                      || (!same_str_p && !(dend == end_match_1)))
                    {
                      best_regs_set = 1;
                      match_end = d;
                      ;
                      for (mcnt = 1; mcnt < num_regs; mcnt++)
                        {
                          best_regstart[mcnt] = regstart[mcnt];
                          best_regend[mcnt] = regend[mcnt];
                        }
                    }
                  goto fail;
                }
              else if (best_regs_set)
                {
           restore_best_regs:
                  ;
                  d = match_end;
                  dend = ((d >= string1 && d <= end1)
             ? end_match_1 : end_match_2);
    for (mcnt = 1; mcnt < num_regs; mcnt++)
      {
        regstart[mcnt] = best_regstart[mcnt];
        regend[mcnt] = best_regend[mcnt];
      }
                }
            }
          ;
          if (regs && !bufp->no_sub)
     {
              if (bufp->regs_allocated == 0)
                {
                  regs->num_regs = ((30) > (num_regs + 1) ? (30) : (num_regs + 1));
                  regs->start = ((regoff_t *) malloc ((regs->num_regs) * sizeof (regoff_t)));
                  regs->end = ((regoff_t *) malloc ((regs->num_regs) * sizeof (regoff_t)));
                  if (regs->start == 0 || regs->end == 0)
                    return -2;
                  bufp->regs_allocated = 1;
                }
              else if (bufp->regs_allocated == 1)
                {
                  if (regs->num_regs < num_regs + 1)
                    {
                      regs->num_regs = num_regs + 1;
                      ((regs->start) = (regoff_t *) realloc (regs->start, (regs->num_regs) * sizeof (regoff_t)));
                      ((regs->end) = (regoff_t *) realloc (regs->end, (regs->num_regs) * sizeof (regoff_t)));
                      if (regs->start == 0 || regs->end == 0)
                        return -2;
                    }
                }
              else
  {
    ;
  }
              if (regs->num_regs > 0)
                {
                  regs->start[0] = pos;
                  regs->end[0] = ((dend == end_match_1) ? d - string1
             : d - string2 + size1);
                }
       for (mcnt = 1; mcnt < ((num_regs) < (regs->num_regs) ? (num_regs) : (regs->num_regs)); mcnt++)
  {
                  if (((regstart[mcnt]) == ((char *) -1)) || ((regend[mcnt]) == ((char *) -1)))
                    regs->start[mcnt] = regs->end[mcnt] = -1;
                  else
                    {
        regs->start[mcnt] = ((size1 && string1 <= (regstart[mcnt]) && (regstart[mcnt]) <= string1 + size1) ? (regstart[mcnt]) - string1 : (regstart[mcnt]) - string2 + size1);
                      regs->end[mcnt] = ((size1 && string1 <= (regend[mcnt]) && (regend[mcnt]) <= string1 + size1) ? (regend[mcnt]) - string1 : (regend[mcnt]) - string2 + size1);
                    }
  }
              for (mcnt = num_regs; mcnt < regs->num_regs; mcnt++)
                regs->start[mcnt] = regs->end[mcnt] = -1;
     }
          __builtin_alloca (0);

                                                                        ;
          ;
          mcnt = d - pos - ((dend == end_match_1)
       ? string1
       : string2 - size1);
          ;
          return mcnt;
        }
      switch ((re_opcode_t) *p++)
 {
        case no_op:
          ;
          break;
 case exactn:
   mcnt = *p++;
          ;
          if (translate)
     {
       do
  {
    while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
    if (translate[(unsigned char) *d++] != (char) *p++)
                    goto fail;
  }
       while (--mcnt);
     }
   else
     {
       do
  {
    while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
    if (*d++ != (char) *p++) goto fail;
  }
       while (--mcnt);
     }
   do { unsigned r; for (r = lowest_active_reg; r <= highest_active_reg; r++) { ((reg_info[r]).bits.matched_something) = ((reg_info[r]).bits.ever_matched_something) = 1; } } while (0);
          break;
 case anychar:
          ;
          while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
          if ((!(bufp->syntax & (((((((1) << 1) << 1) << 1) << 1) << 1) << 1)) && (translate ? translate[(unsigned char) (*d)] : (*d)) == '\n')
              || (bufp->syntax & ((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) && (translate ? translate[(unsigned char) (*d)] : (*d)) == '\000'))
     goto fail;
          do { unsigned r; for (r = lowest_active_reg; r <= highest_active_reg; r++) { ((reg_info[r]).bits.matched_something) = ((reg_info[r]).bits.ever_matched_something) = 1; } } while (0);
          ;
          d++;
   break;
 case charset:
 case charset_not:
   {
     register unsigned char c;
     boolean not = (re_opcode_t) *(p - 1) == charset_not;
            ;
     while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
     c = (translate ? translate[(unsigned char) (*d)] : (*d));
     if (c < (unsigned) (*p * 8)
  && p[1 + c / 8] & (1 << (c % 8)))
       not = !not;
     p += 1 + *p;
     if (!not) goto fail;
     do { unsigned r; for (r = lowest_active_reg; r <= highest_active_reg; r++) { ((reg_info[r]).bits.matched_something) = ((reg_info[r]).bits.ever_matched_something) = 1; } } while (0);
            d++;
     break;
   }
        case start_memory:
   ;
   p1 = p;
          if (((reg_info[*p]).bits.match_null_string_p) == 3)
            ((reg_info[*p]).bits.match_null_string_p)
              = group_match_null_string_p (&p1, pend, reg_info);
          old_regstart[*p] = ((reg_info[*p]).bits.match_null_string_p)
                             ? ((regstart[*p]) == ((char *) -1)) ? d : regstart[*p]
                             : regstart[*p];

                                         ;
          regstart[*p] = d;
   ;
          ((reg_info[*p]).bits.is_active) = 1;
          ((reg_info[*p]).bits.matched_something) = 0;
          highest_active_reg = *p;
          if (lowest_active_reg == ((1 << 8) + 1))
            lowest_active_reg = *p;
          p += 2;
          break;
 case stop_memory:
   ;
          old_regend[*p] = ((reg_info[*p]).bits.match_null_string_p)
                           ? ((regend[*p]) == ((char *) -1)) ? d : regend[*p]
      : regend[*p];

                                       ;
          regend[*p] = d;
   ;
          ((reg_info[*p]).bits.is_active) = 0;
          if (lowest_active_reg == highest_active_reg)
            {
              lowest_active_reg = ((1 << 8) + 1);
              highest_active_reg = (1 << 8);
            }
          else
            {
              unsigned char r = *p - 1;
              while (r > 0 && !((reg_info[r]).bits.is_active))
                r--;
       if (r == 0)
                {
                  lowest_active_reg = ((1 << 8) + 1);
                  highest_active_reg = (1 << 8);
                }
              else
                highest_active_reg = r;
            }
          if ((!((reg_info[*p]).bits.matched_something)
               || (re_opcode_t) p[-3] == start_memory)
       && (p + 2) < pend)
            {
              boolean is_a_jump_n = 0;
              p1 = p + 2;
              mcnt = 0;
              switch ((re_opcode_t) *p1++)
                {
                  case jump_n:
      is_a_jump_n = 1;
                  case pop_failure_jump:
    case maybe_pop_jump:
    case jump:
    case dummy_failure_jump:
                    do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
      if (is_a_jump_n)
        p1 += 2;
                    break;
                  default:
                                     ;
                }
       p1 += mcnt;
              if (mcnt < 0 && (re_opcode_t) *p1 == on_failure_jump
                  && (re_opcode_t) p1[3] == start_memory && p1[4] == *p)
  {
                  if (((reg_info[*p]).bits.ever_matched_something))
      {
        unsigned r;
                      ((reg_info[*p]).bits.ever_matched_something) = 0;
                      for (r = *p; r < *p + *(p + 1); r++)
                        {
                          regstart[r] = old_regstart[r];
                          if ((int) old_regend[r] >= (int) regstart[r])
                            regend[r] = old_regend[r];
                        }
                    }
    p1++;
                  do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
                  do { char *destination; int this_reg; ; ; ; ; ; ; ; while (((fail_stack).size - (fail_stack).avail) < ((highest_active_reg - lowest_active_reg + 1) * 3 + 4)) { if (!((fail_stack).size > re_max_failures * ((num_regs - 1) * 3 + 4) ? 0 : ((fail_stack).stack = (fail_stack_elt_t *) (destination = (char *) __builtin_alloca (((fail_stack).size << 1) * sizeof (fail_stack_elt_t)), memcpy ((destination), ((fail_stack).stack), ((fail_stack).size * sizeof (fail_stack_elt_t))), destination), (fail_stack).stack == 0 ? 0 : ((fail_stack).size <<= 1, 1)))) return -2; ; ; } ; for (this_reg = lowest_active_reg; this_reg <= highest_active_reg; this_reg++) { ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regstart[this_reg]; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regend[this_reg]; ; ; ; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) reg_info[this_reg].word; } ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) lowest_active_reg; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) highest_active_reg; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) p1 + mcnt; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) d; ; ; } while (0);
                  goto fail;
                }
            }
          p += 2;
          break;
        case duplicate:
   {
     register const char *d2, *dend2;
     int regno = *p++;
     ;
            if (((regstart[regno]) == ((char *) -1)) || ((regend[regno]) == ((char *) -1)))
              goto fail;
            d2 = regstart[regno];
            dend2 = (((size1 && string1 <= (regstart[regno]) && (regstart[regno]) <= string1 + size1)
        == (size1 && string1 <= (regend[regno]) && (regend[regno]) <= string1 + size1))
       ? regend[regno] : end_match_1);
     for (;;)
       {
  while (d2 == dend2)
    {
      if (dend2 == end_match_2) break;
      if (dend2 == regend[regno]) break;
                    d2 = string2;
                    dend2 = regend[regno];
    }
  if (d2 == dend2) break;
  while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
  mcnt = dend - d;
                if (mcnt > dend2 - d2)
    mcnt = dend2 - d2;
  if (translate
                    ? bcmp_translate (d, d2, mcnt, translate)
                    : memcmp ((d), (d2), (mcnt)))
    goto fail;
  d += mcnt, d2 += mcnt;
       }
   }
   break;
 case begline:
          ;
          if (((d) == (size1 ? string1 : string2) || !size2))
            {
              if (!bufp->not_bol) break;
            }
          else if (d[-1] == '\n' && bufp->newline_anchor)
            {
              break;
            }
          goto fail;
 case endline:
          ;
          if (((d) == end2))
            {
              if (!bufp->not_eol) break;
            }
          else if ((d == end1 ? *string2 : *d) == '\n'
                   && bufp->newline_anchor)
            {
              break;
            }
          goto fail;
        case begbuf:
          ;
          if (((d) == (size1 ? string1 : string2) || !size2))
            break;
          goto fail;
        case endbuf:
          ;
   if (((d) == end2))
     break;
          goto fail;
        case on_failure_keep_string_jump:
          ;
          do { do { (mcnt) = *(p) & 0377; (mcnt) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          ;
          do { char *destination; int this_reg; ; ; ; ; ; ; ; while (((fail_stack).size - (fail_stack).avail) < ((highest_active_reg - lowest_active_reg + 1) * 3 + 4)) { if (!((fail_stack).size > re_max_failures * ((num_regs - 1) * 3 + 4) ? 0 : ((fail_stack).stack = (fail_stack_elt_t *) (destination = (char *) __builtin_alloca (((fail_stack).size << 1) * sizeof (fail_stack_elt_t)), memcpy ((destination), ((fail_stack).stack), ((fail_stack).size * sizeof (fail_stack_elt_t))), destination), (fail_stack).stack == 0 ? 0 : ((fail_stack).size <<= 1, 1)))) return -2; ; ; } ; for (this_reg = lowest_active_reg; this_reg <= highest_active_reg; this_reg++) { ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regstart[this_reg]; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regend[this_reg]; ; ; ; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) reg_info[this_reg].word; } ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) lowest_active_reg; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) highest_active_reg; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) p + mcnt; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) 0; ; ; } while (0);
          break;
 case on_failure_jump:
        on_failure:
          ;
          do { do { (mcnt) = *(p) & 0377; (mcnt) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          ;
          p1 = p;
          while (p1 < pend && (re_opcode_t) *p1 == no_op)
            p1++;
          if (p1 < pend && (re_opcode_t) *p1 == start_memory)
            {
              highest_active_reg = *(p1 + 1) + *(p1 + 2);
              if (lowest_active_reg == ((1 << 8) + 1))
                lowest_active_reg = *(p1 + 1);
            }
          ;
          do { char *destination; int this_reg; ; ; ; ; ; ; ; while (((fail_stack).size - (fail_stack).avail) < ((highest_active_reg - lowest_active_reg + 1) * 3 + 4)) { if (!((fail_stack).size > re_max_failures * ((num_regs - 1) * 3 + 4) ? 0 : ((fail_stack).stack = (fail_stack_elt_t *) (destination = (char *) __builtin_alloca (((fail_stack).size << 1) * sizeof (fail_stack_elt_t)), memcpy ((destination), ((fail_stack).stack), ((fail_stack).size * sizeof (fail_stack_elt_t))), destination), (fail_stack).stack == 0 ? 0 : ((fail_stack).size <<= 1, 1)))) return -2; ; ; } ; for (this_reg = lowest_active_reg; this_reg <= highest_active_reg; this_reg++) { ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regstart[this_reg]; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regend[this_reg]; ; ; ; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) reg_info[this_reg].word; } ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) lowest_active_reg; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) highest_active_reg; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) p + mcnt; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) d; ; ; } while (0);
          break;
        case maybe_pop_jump:
          do { do { (mcnt) = *(p) & 0377; (mcnt) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          ;
          {
     register unsigned char *p2 = p;
     while (p2 + 2 < pend
     && ((re_opcode_t) *p2 == stop_memory
         || (re_opcode_t) *p2 == start_memory))
       p2 += 3;
            if (p2 == pend)
       {
           p[-3] = (unsigned char) pop_failure_jump;

                                                                       ;
              }
            else if ((re_opcode_t) *p2 == exactn
       || (bufp->newline_anchor && (re_opcode_t) *p2 == endline))
       {
  register unsigned char c
                  = *p2 == (unsigned char) endline ? '\n' : p2[2];
  p1 = p + mcnt;
                if ((re_opcode_t) p1[3] == exactn && p1[5] != c)
                  {
        p[-3] = (unsigned char) pop_failure_jump;

                                           ;
                  }
  else if ((re_opcode_t) p1[3] == charset
    || (re_opcode_t) p1[3] == charset_not)
    {
      int not = (re_opcode_t) p1[3] == charset_not;
      if (c < (unsigned char) (p1[4] * 8)
   && p1[5 + c / 8] & (1 << (c % 8)))
        not = !not;
      if (!not)
                      {
            p[-3] = (unsigned char) pop_failure_jump;
                        ;
                      }
    }
       }
   }
   p -= 2;
   if ((re_opcode_t) p[-1] != pop_failure_jump)
     {
       p[-1] = (unsigned char) jump;
              ;
       goto unconditional_jump;
     }
        case pop_failure_jump:
          {
            unsigned dummy_low_reg, dummy_high_reg;
            unsigned char *pdummy;
            const char *sdummy;
            ;
            { int this_reg; const unsigned char *string_temp; ; ; ; ; ; ; ; string_temp = fail_stack.stack[--fail_stack.avail]; if (string_temp != 0) sdummy = (const char *) string_temp; ; ; ; pdummy = (unsigned char *) fail_stack.stack[--fail_stack.avail]; ; ; dummy_high_reg = (unsigned) fail_stack.stack[--fail_stack.avail]; ; dummy_low_reg = (unsigned) fail_stack.stack[--fail_stack.avail]; ; for (this_reg = dummy_high_reg; this_reg >= dummy_low_reg; this_reg--) { ; reg_info_dummy[this_reg].word = fail_stack.stack[--fail_stack.avail]; ; reg_dummy[this_reg] = (const char *) fail_stack.stack[--fail_stack.avail]; ; reg_dummy[this_reg] = (const char *) fail_stack.stack[--fail_stack.avail]; ; } ; }
                                                                    ;
          }
        case jump:
 unconditional_jump:
   do { do { (mcnt) = *(p) & 0377; (mcnt) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
          ;
   p += mcnt;
          ;
   break;
        case jump_past_alt:
          ;
          goto unconditional_jump;
        case dummy_failure_jump:
          ;
          do { char *destination; int this_reg; ; ; ; ; ; ; ; while (((fail_stack).size - (fail_stack).avail) < ((highest_active_reg - lowest_active_reg + 1) * 3 + 4)) { if (!((fail_stack).size > re_max_failures * ((num_regs - 1) * 3 + 4) ? 0 : ((fail_stack).stack = (fail_stack_elt_t *) (destination = (char *) __builtin_alloca (((fail_stack).size << 1) * sizeof (fail_stack_elt_t)), memcpy ((destination), ((fail_stack).stack), ((fail_stack).size * sizeof (fail_stack_elt_t))), destination), (fail_stack).stack == 0 ? 0 : ((fail_stack).size <<= 1, 1)))) return -2; ; ; } ; for (this_reg = lowest_active_reg; this_reg <= highest_active_reg; this_reg++) { ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regstart[this_reg]; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regend[this_reg]; ; ; ; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) reg_info[this_reg].word; } ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) lowest_active_reg; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) highest_active_reg; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) 0; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) 0; ; ; } while (0);
          goto unconditional_jump;
        case push_dummy_failure:
          ;
          do { char *destination; int this_reg; ; ; ; ; ; ; ; while (((fail_stack).size - (fail_stack).avail) < ((highest_active_reg - lowest_active_reg + 1) * 3 + 4)) { if (!((fail_stack).size > re_max_failures * ((num_regs - 1) * 3 + 4) ? 0 : ((fail_stack).stack = (fail_stack_elt_t *) (destination = (char *) __builtin_alloca (((fail_stack).size << 1) * sizeof (fail_stack_elt_t)), memcpy ((destination), ((fail_stack).stack), ((fail_stack).size * sizeof (fail_stack_elt_t))), destination), (fail_stack).stack == 0 ? 0 : ((fail_stack).size <<= 1, 1)))) return -2; ; ; } ; for (this_reg = lowest_active_reg; this_reg <= highest_active_reg; this_reg++) { ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regstart[this_reg]; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) regend[this_reg]; ; ; ; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) reg_info[this_reg].word; } ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) lowest_active_reg; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) highest_active_reg; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) 0; ; ; ; fail_stack.stack[fail_stack.avail++] = (fail_stack_elt_t) 0; ; ; } while (0);
          break;
        case succeed_n:
          do { (mcnt) = *(p + 2) & 0377; (mcnt) += ((signed char) (*((p + 2) + 1))) << 8; } while (0);
          ;
          ;
          if (mcnt > 0)
            {
               mcnt--;
        p += 2;
               do { do { (p)[0] = (mcnt) & 0377; (p)[1] = (mcnt) >> 8; } while (0); (p) += 2; } while (0);
               ;
            }
   else if (mcnt == 0)
            {
              ;
       p[2] = (unsigned char) no_op;
              p[3] = (unsigned char) no_op;
              goto on_failure;
            }
          break;
        case jump_n:
          do { (mcnt) = *(p + 2) & 0377; (mcnt) += ((signed char) (*((p + 2) + 1))) << 8; } while (0);
          ;
          if (mcnt)
            {
               mcnt--;
               do { (p + 2)[0] = (mcnt) & 0377; (p + 2)[1] = (mcnt) >> 8; } while (0);
        goto unconditional_jump;
            }
   else
     p += 4;
          break;
 case set_number_at:
   {
            ;
            do { do { (mcnt) = *(p) & 0377; (mcnt) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
            p1 = p + mcnt;
            do { do { (mcnt) = *(p) & 0377; (mcnt) += ((signed char) (*((p) + 1))) << 8; } while (0); (p) += 2; } while (0);
            ;
     do { (p1)[0] = (mcnt) & 0377; (p1)[1] = (mcnt) >> 8; } while (0);
            break;
          }
        case wordbound:
          ;
          if ((((d) == (size1 ? string1 : string2) || !size2) || ((d) == end2) || (re_syntax_table[(d - 1) == end1 ? *string2 : (d - 1) == string2 - 1 ? *(end1 - 1) : *(d - 1)] == 1) != (re_syntax_table[(d) == end1 ? *string2 : (d) == string2 - 1 ? *(end1 - 1) : *(d)] == 1)))
     break;
          goto fail;
 case notwordbound:
          ;
   if ((((d) == (size1 ? string1 : string2) || !size2) || ((d) == end2) || (re_syntax_table[(d - 1) == end1 ? *string2 : (d - 1) == string2 - 1 ? *(end1 - 1) : *(d - 1)] == 1) != (re_syntax_table[(d) == end1 ? *string2 : (d) == string2 - 1 ? *(end1 - 1) : *(d)] == 1)))
     goto fail;
          break;
 case wordbeg:
          ;
   if ((re_syntax_table[(d) == end1 ? *string2 : (d) == string2 - 1 ? *(end1 - 1) : *(d)] == 1) && (((d) == (size1 ? string1 : string2) || !size2) || !(re_syntax_table[(d - 1) == end1 ? *string2 : (d - 1) == string2 - 1 ? *(end1 - 1) : *(d - 1)] == 1)))
     break;
          goto fail;
 case wordend:
          ;
   if (!((d) == (size1 ? string1 : string2) || !size2) && (re_syntax_table[(d - 1) == end1 ? *string2 : (d - 1) == string2 - 1 ? *(end1 - 1) : *(d - 1)] == 1)
              && (!(re_syntax_table[(d) == end1 ? *string2 : (d) == string2 - 1 ? *(end1 - 1) : *(d)] == 1) || ((d) == end2)))
     break;
          goto fail;
 case wordchar:
          ;
   while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
          if (!(re_syntax_table[(d) == end1 ? *string2 : (d) == string2 - 1 ? *(end1 - 1) : *(d)] == 1))
            goto fail;
   do { unsigned r; for (r = lowest_active_reg; r <= highest_active_reg; r++) { ((reg_info[r]).bits.matched_something) = ((reg_info[r]).bits.ever_matched_something) = 1; } } while (0);
          d++;
   break;
 case notwordchar:
          ;
   while (d == dend) { if (dend == end_match_2) goto fail; d = string2; dend = end_match_2; };
   if ((re_syntax_table[(d) == end1 ? *string2 : (d) == string2 - 1 ? *(end1 - 1) : *(d)] == 1))
            goto fail;
          do { unsigned r; for (r = lowest_active_reg; r <= highest_active_reg; r++) { ((reg_info[r]).bits.matched_something) = ((reg_info[r]).bits.ever_matched_something) = 1; } } while (0);
          d++;
   break;
        default:
          abort ();
 }
      continue;
    fail:
      if (!(fail_stack.avail == 0))
 {
          ;
          { int this_reg; const unsigned char *string_temp; ; ; ; ; ; ; ; string_temp = fail_stack.stack[--fail_stack.avail]; if (string_temp != 0) d = (const char *) string_temp; ; ; ; p = (unsigned char *) fail_stack.stack[--fail_stack.avail]; ; ; highest_active_reg = (unsigned) fail_stack.stack[--fail_stack.avail]; ; lowest_active_reg = (unsigned) fail_stack.stack[--fail_stack.avail]; ; for (this_reg = highest_active_reg; this_reg >= lowest_active_reg; this_reg--) { ; reg_info[this_reg].word = fail_stack.stack[--fail_stack.avail]; ; regend[this_reg] = (const char *) fail_stack.stack[--fail_stack.avail]; ; regstart[this_reg] = (const char *) fail_stack.stack[--fail_stack.avail]; ; } ; }
                                                        ;
          if (!p)
     goto fail;
   ;
          if (p < pend)
            {
              boolean is_a_jump_n = 0;
              switch ((re_opcode_t) *p)
                {
                case jump_n:
                  is_a_jump_n = 1;
                case maybe_pop_jump:
                case pop_failure_jump:
                case jump:
                  p1 = p + 1;
                  do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
                  p1 += mcnt;
                  if ((is_a_jump_n && (re_opcode_t) *p1 == succeed_n)
                      || (!is_a_jump_n
                          && (re_opcode_t) *p1 == on_failure_jump))
                    goto fail;
                  break;
                default:
                                   ;
                }
            }
          if (d >= string1 && d <= end1)
     dend = end_match_1;
        }
      else
        break;
    }
  if (best_regs_set)
    goto restore_best_regs;
  __builtin_alloca (0);
  return -1;
}
static boolean
group_match_null_string_p (p, end, reg_info)
    unsigned char **p, *end;
    register_info_type *reg_info;
{
  int mcnt;
  unsigned char *p1 = *p + 2;
  while (p1 < end)
    {
      switch ((re_opcode_t) *p1)
        {
        case on_failure_jump:
          p1++;
          do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
   if (mcnt >= 0)
     {
              while ((re_opcode_t) p1[mcnt-3] == jump_past_alt)
                {
                  if (!alt_match_null_string_p (p1, p1 + mcnt - 3,
                          reg_info))
                    return 0;
                  p1 += mcnt;
                  if ((re_opcode_t) *p1 != on_failure_jump)
                    break;
    p1++;
                  do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
                  if ((re_opcode_t) p1[mcnt-3] != jump_past_alt)
                    {
                      p1 -= 3;
                      break;
                    }
                }
              do { (mcnt) = *(p1 - 2) & 0377; (mcnt) += ((signed char) (*((p1 - 2) + 1))) << 8; } while (0);
              if (!alt_match_null_string_p (p1, p1 + mcnt, reg_info))
                return 0;
              p1 += mcnt;
            }
          break;
        case stop_memory:
   ;
          *p = p1 + 2;
          return 1;
        default:
          if (!common_op_match_null_string_p (&p1, end, reg_info))
            return 0;
        }
    }
  return 0;
}
static boolean
alt_match_null_string_p (p, end, reg_info)
    unsigned char *p, *end;
    register_info_type *reg_info;
{
  int mcnt;
  unsigned char *p1 = p;
  while (p1 < end)
    {
      switch ((re_opcode_t) *p1)
        {
        case on_failure_jump:
          p1++;
          do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
          p1 += mcnt;
          break;
 default:
          if (!common_op_match_null_string_p (&p1, end, reg_info))
            return 0;
        }
    }
  return 1;
}
static boolean
common_op_match_null_string_p (p, end, reg_info)
    unsigned char **p, *end;
    register_info_type *reg_info;
{
  int mcnt;
  boolean ret;
  int reg_no;
  unsigned char *p1 = *p;
  switch ((re_opcode_t) *p1++)
    {
    case no_op:
    case begline:
    case endline:
    case begbuf:
    case endbuf:
    case wordbeg:
    case wordend:
    case wordbound:
    case notwordbound:
      break;
    case start_memory:
      reg_no = *p1;
      ;
      ret = group_match_null_string_p (&p1, end, reg_info);
      if (((reg_info[reg_no]).bits.match_null_string_p) == 3)
        ((reg_info[reg_no]).bits.match_null_string_p) = ret;
      if (!ret)
        return 0;
      break;
    case jump:
      do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
      if (mcnt >= 0)
        p1 += mcnt;
      else
        return 0;
      break;
    case succeed_n:
      p1 += 2;
      do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
      if (mcnt == 0)
        {
          p1 -= 4;
          do { do { (mcnt) = *(p1) & 0377; (mcnt) += ((signed char) (*((p1) + 1))) << 8; } while (0); (p1) += 2; } while (0);
          p1 += mcnt;
        }
      else
        return 0;
      break;
    case duplicate:
      if (!((reg_info[*p1]).bits.match_null_string_p))
        return 0;
      break;
    case set_number_at:
      p1 += 4;
    default:
      return 0;
  }
  *p = p1;
  return 1;
}
static int
bcmp_translate (s1, s2, len, translate)
     unsigned char *s1, *s2;
     register int len;
     char *translate;
{
  register unsigned char *p1 = s1, *p2 = s2;
  while (len)
    {
      if (translate[*p1++] != translate[*p2++]) return 1;
      len--;
    }
  return 0;
}
const char *
re_compile_pattern (pattern, length, bufp)
     const char *pattern;
     int length;
     struct re_pattern_buffer *bufp;
{
  reg_errcode_t ret;
  bufp->regs_allocated = 0;
  bufp->no_sub = 0;
  bufp->newline_anchor = 1;
  ret = regex_compile (pattern, length, re_syntax_options, bufp);
  return re_error_msg[(int) ret];
}
static struct re_pattern_buffer re_comp_buf;
char *
re_comp (s)
    const char *s;
{
  reg_errcode_t ret;
  if (!s)
    {
      if (!re_comp_buf.buffer)
 return "No previous regular expression";
      return 0;
    }
  if (!re_comp_buf.buffer)
    {
      re_comp_buf.buffer = (unsigned char *) malloc (200);
      if (re_comp_buf.buffer == 0)
        return "Memory exhausted";
      re_comp_buf.allocated = 200;
      re_comp_buf.fastmap = (char *) malloc (1 << 8);
      if (re_comp_buf.fastmap == 0)
 return "Memory exhausted";
    }
  re_comp_buf.newline_anchor = 1;
  ret = regex_compile (s, strlen (s), re_syntax_options, &re_comp_buf);
  return (char *) re_error_msg[(int) ret];
}
int
re_exec (s)
    const char *s;
{
  const int len = strlen (s);
  return
    0 <= re_search (&re_comp_buf, s, len, 0, len, (struct re_registers *) 0);
}
int
regcomp (preg, pattern, cflags)
    regex_t *preg;
    const char *pattern;
    int cflags;
{
  reg_errcode_t ret;
  unsigned syntax
    = (cflags & 1) ?
      (((((1) << 1) << 1) | (((((((1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | (((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) | ((((1) << 1) << 1) << 1) | (((((1) << 1) << 1) << 1) << 1) | (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) : (((((1) << 1) << 1) | (((((((1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | (((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) | ((1) << 1));
  preg->buffer = 0;
  preg->allocated = 0;
  preg->used = 0;
  preg->fastmap = 0;
  if (cflags & (1 << 1))
    {
      unsigned i;
      preg->translate = (char *) malloc (256);
      if (preg->translate == 0)
        return (int) REG_ESPACE;
      for (i = 0; i < 256; i++)
        preg->translate[i] = (1 && isupper (i)) ? tolower (i) : i;
    }
  else
    preg->translate = 0;
  if (cflags & ((1 << 1) << 1))
    {
      syntax &= ~(((((((1) << 1) << 1) << 1) << 1) << 1) << 1);
      syntax |= (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1);
      preg->newline_anchor = 1;
    }
  else
    preg->newline_anchor = 0;
  preg->no_sub = !!(cflags & (((1 << 1) << 1) << 1));
  ret = regex_compile (pattern, strlen (pattern), syntax, preg);
  if (ret == REG_ERPAREN) ret = REG_EPAREN;
  return (int) ret;
}
int
regexec (preg, string, nmatch, pmatch, eflags)
    const regex_t *preg;
    const char *string;
    size_t nmatch;
    regmatch_t pmatch[];
    int eflags;
{
  int ret;
  struct re_registers regs;
  regex_t private_preg;
  int len = strlen (string);
  boolean want_reg_info = !preg->no_sub && nmatch > 0;
  private_preg = *preg;
  private_preg.not_bol = !!(eflags & 1);
  private_preg.not_eol = !!(eflags & (1 << 1));
  private_preg.regs_allocated = 2;
  if (want_reg_info)
    {
      regs.num_regs = nmatch;
      regs.start = ((regoff_t *) malloc ((nmatch) * sizeof (regoff_t)));
      regs.end = ((regoff_t *) malloc ((nmatch) * sizeof (regoff_t)));
      if (regs.start == 0 || regs.end == 0)
        return (int) REG_NOMATCH;
    }
  ret = re_search (&private_preg, string, len,
                                0, len,
                   want_reg_info ? &regs : (struct re_registers *) 0);
  if (want_reg_info)
    {
      if (ret >= 0)
        {
          unsigned r;
          for (r = 0; r < nmatch; r++)
            {
              pmatch[r].rm_so = regs.start[r];
              pmatch[r].rm_eo = regs.end[r];
            }
        }
      free (regs.start);
      free (regs.end);
    }
  return ret >= 0 ? (int) REG_NOERROR : (int) REG_NOMATCH;
}
size_t
regerror (errcode, preg, errbuf, errbuf_size)
    int errcode;
    const regex_t *preg;
    char *errbuf;
    size_t errbuf_size;
{
  const char *msg;
  size_t msg_size;
  if (errcode < 0
      || errcode >= (sizeof (re_error_msg) / sizeof (re_error_msg[0])))
    abort ();
  msg = re_error_msg[errcode];
  if (! msg)
    msg = "Success";
  msg_size = strlen (msg) + 1;
  if (errbuf_size != 0)
    {
      if (msg_size > errbuf_size)
        {
          strncpy (errbuf, msg, errbuf_size - 1);
          errbuf[errbuf_size - 1] = 0;
        }
      else
        strcpy (errbuf, msg);
    }
  return msg_size;
}
void
regfree (preg)
    regex_t *preg;
{
  if (preg->buffer != 0)
    free (preg->buffer);
  preg->buffer = 0;
  preg->allocated = 0;
  preg->used = 0;
  if (preg->fastmap != 0)
    free (preg->fastmap);
  preg->fastmap = 0;
  preg->fastmap_accurate = 0;
  if (preg->translate != 0)
    free (preg->translate);
  preg->translate = 0;
}
typedef int charclass[(((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int)))];
typedef enum
{
  END = -1,
  EMPTY = (1 << 8),
  BACKREF,
  BEGLINE,
  ENDLINE,
  BEGWORD,
  ENDWORD,
  LIMWORD,
  NOTLIMWORD,
  QMARK,
  STAR,
  PLUS,
  REPMN,
  CAT,
  OR,
  ORTOP,
  LPAREN,
  RPAREN,
  CSET
} token;
typedef struct
{
  unsigned strchr;
  unsigned constraint;
} position;
typedef struct
{
  position *elems;
  int nelem;
} position_set;
typedef struct
{
  int hash;
  position_set elems;
  char newline;
  char letter;
  char backref;
  unsigned char constraint;
  int first_end;
} dfa_state;
struct dfamust
{
  int exact;
  char *must;
  struct dfamust *next;
};
struct dfa
{
  charclass *charclasses;
  int cindex;
  int calloc;
  token *tokens;
  int tindex;
  int talloc;
  int depth;
  int nleaves;
  int nregexps;
  dfa_state *states;
  int sindex;
  int salloc;
  position_set *follows;
  int searchflag;
  int tralloc;
  int trcount;
  int **trans;
  int **realtrans;
  int **fails;
  int *success;
  int *newlines;
  struct dfamust *musts;
};
extern void dfasyntax(int, int);
extern void dfacomp(char *, size_t, struct dfa *, int);
extern char *dfaexec(struct dfa *, char *, char *, int, int *, int *);
extern void dfafree(struct dfa *);
extern void dfainit(struct dfa *);
extern void dfaparse(char *, size_t, struct dfa *);
extern void dfaanalyze(struct dfa *, int);
extern void dfastate(int, struct dfa *, int []);
extern void dfaerror(char *);
typedef void *ptr_t;
static void dfamust();
static ptr_t
xcalloc(n, s)
     int n;
     size_t s;
{
  ptr_t r = calloc(n, s);
  if (!r)
    dfaerror("Memory exhausted");
  return r;
}
static ptr_t
xmalloc_1(n)
     size_t n;
{
  ptr_t r = malloc(n);
  ;
  if (!r)
    dfaerror("Memory exhausted");
  return r;
}
static ptr_t
xrealloc_1(p, n)
     ptr_t p;
     size_t n;
{
  ptr_t r = realloc(p, n);
  ;
  if (!r)
    dfaerror("Memory exhausted");
  return r;
}
static int
tstbit(b, c)
     int b;
     charclass c;
{
  return c[b / (8 * sizeof (int))] & 1 << b % (8 * sizeof (int));
}
static void
setbit(b, c)
     int b;
     charclass c;
{
  c[b / (8 * sizeof (int))] |= 1 << b % (8 * sizeof (int));
}
static void
clrbit(b, c)
     int b;
     charclass c;
{
  c[b / (8 * sizeof (int))] &= ~(1 << b % (8 * sizeof (int)));
}
static void
copyset(src, dst)
     charclass src;
     charclass dst;
{
  int i;
  for (i = 0; i < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++i)
    dst[i] = src[i];
}
static void
zeroset(s)
     charclass s;
{
  int i;
  for (i = 0; i < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++i)
    s[i] = 0;
}
static void
notset(s)
     charclass s;
{
  int i;
  for (i = 0; i < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++i)
    s[i] = ~s[i];
}
static int
equal(s1, s2)
     charclass s1;
     charclass s2;
{
  int i;
  for (i = 0; i < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++i)
    if (s1[i] != s2[i])
      return 0;
  return 1;
}
static struct dfa *dfa;
static int
charclass_index(s)
     charclass s;
{
  int i;
  for (i = 0; i < dfa->cindex; ++i)
    if (equal(s, dfa->charclasses[i]))
      return i;
  if ((dfa->cindex) >= (dfa->calloc)) { while ((dfa->cindex) >= (dfa->calloc)) (dfa->calloc) *= 2; ((dfa->charclasses) = (charclass *) xrealloc_1((ptr_t) (dfa->charclasses), (dfa->calloc) * sizeof (charclass))); };
  ++dfa->cindex;
  copyset(s, dfa->charclasses[i]);
  return i;
}
static int syntax_bits, syntax_bits_set;
static int case_fold;
void
dfasyntax(bits, fold)
     int bits;
     int fold;
{
  syntax_bits_set = 1;
  syntax_bits = bits;
  case_fold = fold;
}
static char *lexstart;
static char *lexptr;
static lexleft;
static token lasttok;
static int laststart;
static int parens;
static int minrep, maxrep;
static int is_alpha(c) int c; { return (1 && isalpha(c)); }
static int is_upper(c) int c; { return (1 && isupper(c)); }
static int is_lower(c) int c; { return (1 && islower(c)); }
static int is_digit(c) int c; { return (1 && isdigit(c)); }
static int is_xdigit(c) int c; { return (1 && isxdigit(c)); }
static int is_space(c) int c; { return (1 && isspace(c)); }
static int is_punct(c) int c; { return (1 && ispunct(c)); }
static int is_alnum(c) int c; { return (1 && isalnum(c)); }
static int is_print(c) int c; { return (1 && isprint(c)); }
static int is_graph(c) int c; { return (1 && (isprint(c) && !isspace(c))); }
static int is_cntrl(c) int c; { return (1 && iscntrl(c)); }
static struct {
  char *name;
  int (*pred)();
} prednames[] = {
  ":alpha:]", is_alpha,
  ":upper:]", is_upper,
  ":lower:]", is_lower,
  ":digit:]", is_digit,
  ":xdigit:]", is_xdigit,
  ":space:]", is_space,
  ":punct:]", is_punct,
  ":alnum:]", is_alnum,
  ":print:]", is_print,
  ":graph:]", is_graph,
  ":cntrl:]", is_cntrl,
  0
};
static int
looking_at(s)
     char *s;
{
  int len;
  len = strlen(s);
  if (lexleft < len)
    return 0;
  return strncmp(s, lexptr, len) == 0;
}
static token
lex()
{
  token c, c1, c2;
  int backslash = 0, invert;
  charclass ccl;
  int i;
  for (i = 0; i < 2; ++i)
    {
      { if (! lexleft) if (0 != 0) dfaerror(0); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
      switch (c)
 {
 case '\\':
   if (backslash)
     goto normal_char;
   if (lexleft == 0)
     dfaerror("Unfinished \\ escape");
   backslash = 1;
   break;
 case '^':
   if (backslash)
     goto normal_char;
   if (syntax_bits & ((((1) << 1) << 1) << 1)
       || lasttok == END
       || lasttok == LPAREN
       || lasttok == OR)
     return lasttok = BEGLINE;
   goto normal_char;
 case '$':
   if (backslash)
     goto normal_char;
   if (syntax_bits & ((((1) << 1) << 1) << 1)
       || lexleft == 0
       || (syntax_bits & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)
    ? lexleft > 0 && *lexptr == ')'
    : lexleft > 1 && lexptr[0] == '\\' && lexptr[1] == ')')
       || (syntax_bits & ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)
    ? lexleft > 0 && *lexptr == '|'
    : lexleft > 1 && lexptr[0] == '\\' && lexptr[1] == '|')
       || ((syntax_bits & ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
           && lexleft > 0 && *lexptr == '\n'))
     return lasttok = ENDLINE;
   goto normal_char;
 case '1':
 case '2':
 case '3':
 case '4':
 case '5':
 case '6':
 case '7':
 case '8':
 case '9':
   if (backslash && !(syntax_bits & (((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
     {
       laststart = 0;
       return lasttok = BACKREF;
     }
   goto normal_char;
 case '<':
   if (backslash)
     return lasttok = BEGWORD;
   goto normal_char;
 case '>':
   if (backslash)
     return lasttok = ENDWORD;
   goto normal_char;
 case 'b':
   if (backslash)
     return lasttok = LIMWORD;
   goto normal_char;
 case 'B':
   if (backslash)
     return lasttok = NOTLIMWORD;
   goto normal_char;
 case '?':
   if (syntax_bits & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
     goto normal_char;
   if (backslash != ((syntax_bits & ((1) << 1)) != 0))
     goto normal_char;
   if (!(syntax_bits & (((((1) << 1) << 1) << 1) << 1)) && laststart)
     goto normal_char;
   return lasttok = QMARK;
 case '*':
   if (backslash)
     goto normal_char;
   if (!(syntax_bits & (((((1) << 1) << 1) << 1) << 1)) && laststart)
     goto normal_char;
   return lasttok = STAR;
 case '+':
   if (syntax_bits & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
     goto normal_char;
   if (backslash != ((syntax_bits & ((1) << 1)) != 0))
     goto normal_char;
   if (!(syntax_bits & (((((1) << 1) << 1) << 1) << 1)) && laststart)
     goto normal_char;
   return lasttok = PLUS;
 case '{':
   if (!(syntax_bits & ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
     goto normal_char;
   if (backslash != ((syntax_bits & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) == 0))
     goto normal_char;
   minrep = maxrep = 0;
   { if (! lexleft) if ("unfinished repeat count" != 0) dfaerror("unfinished repeat count"); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
   if ((1 && isdigit(c)))
     {
       minrep = c - '0';
       for (;;)
  {
    { if (! lexleft) if ("unfinished repeat count" != 0) dfaerror("unfinished repeat count"); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
    if (!(1 && isdigit(c)))
      break;
    minrep = 10 * minrep + c - '0';
  }
     }
   else if (c != ',')
     dfaerror("malformed repeat count");
   if (c == ',')
     for (;;)
       {
  { if (! lexleft) if ("unfinished repeat count" != 0) dfaerror("unfinished repeat count"); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
  if (!(1 && isdigit(c)))
    break;
  maxrep = 10 * maxrep + c - '0';
       }
   else
     maxrep = minrep;
   if (!(syntax_bits & (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
     {
       if (c != '\\')
  dfaerror("malformed repeat count");
       { if (! lexleft) if ("unfinished repeat count" != 0) dfaerror("unfinished repeat count"); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
     }
   if (c != '}')
     dfaerror("malformed repeat count");
   laststart = 0;
   return lasttok = REPMN;
 case '|':
   if (syntax_bits & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
     goto normal_char;
   if (backslash != ((syntax_bits & ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) == 0))
     goto normal_char;
   laststart = 1;
   return lasttok = OR;
 case '\n':
   if (syntax_bits & (((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)
       || backslash
       || !(syntax_bits & ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)))
     goto normal_char;
   laststart = 1;
   return lasttok = OR;
 case '(':
   if (backslash != ((syntax_bits & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) == 0))
     goto normal_char;
   ++parens;
   laststart = 1;
   return lasttok = LPAREN;
 case ')':
   if (backslash != ((syntax_bits & ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) == 0))
     goto normal_char;
   if (parens == 0 && syntax_bits & ((((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
     goto normal_char;
   --parens;
   laststart = 0;
   return lasttok = RPAREN;
 case '.':
   if (backslash)
     goto normal_char;
   zeroset(ccl);
   notset(ccl);
   if (!(syntax_bits & (((((((1) << 1) << 1) << 1) << 1) << 1) << 1)))
     clrbit('\n', ccl);
   if (syntax_bits & ((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
     clrbit('\0', ccl);
   laststart = 0;
   return lasttok = CSET + charclass_index(ccl);
 case 'w':
 case 'W':
   if (!backslash)
     goto normal_char;
   zeroset(ccl);
   for (c2 = 0; c2 < (1 << 8); ++c2)
     if ((1 && isalnum(c2)))
       setbit(c2, ccl);
   if (c == 'W')
     notset(ccl);
   laststart = 0;
   return lasttok = CSET + charclass_index(ccl);
 case '[':
   if (backslash)
     goto normal_char;
   zeroset(ccl);
   { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
   if (c == '^')
     {
       { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
       invert = 1;
     }
   else
     invert = 0;
   do
     {
       if (c == '[' && (syntax_bits & (((1) << 1) << 1)))
  for (c1 = 0; prednames[c1].name; ++c1)
    if (looking_at(prednames[c1].name))
      {
        for (c2 = 0; c2 < (1 << 8); ++c2)
   if ((*prednames[c1].pred)(c2))
     setbit(c2, ccl);
        lexptr += strlen(prednames[c1].name);
        lexleft -= strlen(prednames[c1].name);
        { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c1) = (unsigned char) *lexptr++; --lexleft; };
        goto skip;
      }
       if (c == '\\' && (syntax_bits & (1)))
  { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c) = (unsigned char) *lexptr++; --lexleft; };
       { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c1) = (unsigned char) *lexptr++; --lexleft; };
       if (c1 == '-')
  {
    { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c2) = (unsigned char) *lexptr++; --lexleft; };
    if (c2 == ']')
      {
        --lexptr;
        ++lexleft;
        c2 = c;
      }
    else
      {
        if (c2 == '\\'
     && (syntax_bits & (1)))
   { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c2) = (unsigned char) *lexptr++; --lexleft; };
        { if (! lexleft) if ("Unbalanced [" != 0) dfaerror("Unbalanced ["); else return END; (c1) = (unsigned char) *lexptr++; --lexleft; };
      }
  }
       else
  c2 = c;
       while (c <= c2)
  {
    setbit(c, ccl);
    if (case_fold)
      if ((1 && isupper(c)))
        setbit(tolower(c), ccl);
      else if ((1 && islower(c)))
        setbit(toupper(c), ccl);
    ++c;
  }
     skip:
       ;
     }
   while ((c = c1) != ']');
   if (invert)
     {
       notset(ccl);
       if (syntax_bits & (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1))
  clrbit('\n', ccl);
     }
   laststart = 0;
   return lasttok = CSET + charclass_index(ccl);
 default:
 normal_char:
   laststart = 0;
   if (case_fold && (1 && isalpha(c)))
     {
       zeroset(ccl);
       setbit(c, ccl);
       if (isupper(c))
  setbit(tolower(c), ccl);
       else
  setbit(toupper(c), ccl);
       return lasttok = CSET + charclass_index(ccl);
     }
   return c;
 }
    }
  abort();
}
static token tok;
static depth;
static void
addtok(t)
     token t;
{
  if ((dfa->tindex) >= (dfa->talloc)) { while ((dfa->tindex) >= (dfa->talloc)) (dfa->talloc) *= 2; ((dfa->tokens) = (token *) xrealloc_1((ptr_t) (dfa->tokens), (dfa->talloc) * sizeof (token))); };
  dfa->tokens[dfa->tindex++] = t;
  switch (t)
    {
    case QMARK:
    case STAR:
    case PLUS:
      break;
    case CAT:
    case OR:
    case ORTOP:
      --depth;
      break;
    default:
      ++dfa->nleaves;
    case EMPTY:
      ++depth;
      break;
    }
  if (depth > dfa->depth)
    dfa->depth = depth;
}
static void regexp(int);
static void
atom()
{
  if ((tok >= 0 && tok < (1 << 8)) || tok >= CSET || tok == BACKREF
      || tok == BEGLINE || tok == ENDLINE || tok == BEGWORD
      || tok == ENDWORD || tok == LIMWORD || tok == NOTLIMWORD)
    {
      addtok(tok);
      tok = lex();
    }
  else if (tok == LPAREN)
    {
      tok = lex();
      regexp(0);
      if (tok != RPAREN)
 dfaerror("Unbalanced (");
      tok = lex();
    }
  else
    addtok(EMPTY);
}
static int
nsubtoks(tindex)
{
  int ntoks1;
  switch (dfa->tokens[tindex - 1])
    {
    default:
      return 1;
    case QMARK:
    case STAR:
    case PLUS:
      return 1 + nsubtoks(tindex - 1);
    case CAT:
    case OR:
    case ORTOP:
      ntoks1 = nsubtoks(tindex - 1);
      return 1 + ntoks1 + nsubtoks(tindex - 1 - ntoks1);
    }
}
static void
copytoks(tindex, ntokens)
     int tindex, ntokens;
{
  int i;
  for (i = 0; i < ntokens; ++i)
    addtok(dfa->tokens[tindex + i]);
}
static void
closure()
{
  int tindex, ntokens, i;
  atom();
  while (tok == QMARK || tok == STAR || tok == PLUS || tok == REPMN)
    if (tok == REPMN)
      {
 ntokens = nsubtoks(dfa->tindex);
 tindex = dfa->tindex - ntokens;
 if (maxrep == 0)
   addtok(PLUS);
 if (minrep == 0)
   addtok(QMARK);
 for (i = 1; i < minrep; ++i)
   {
     copytoks(tindex, ntokens);
     addtok(CAT);
   }
 for (; i < maxrep; ++i)
   {
     copytoks(tindex, ntokens);
     addtok(QMARK);
     addtok(CAT);
   }
 tok = lex();
      }
    else
      {
 addtok(tok);
 tok = lex();
      }
}
static void
branch()
{
  closure();
  while (tok != RPAREN && tok != OR && tok >= 0)
    {
      closure();
      addtok(CAT);
    }
}
static void
regexp(toplevel)
     int toplevel;
{
  branch();
  while (tok == OR)
    {
      tok = lex();
      branch();
      if (toplevel)
 addtok(ORTOP);
      else
 addtok(OR);
    }
}
void
dfaparse(s, len, d)
     char *s;
     size_t len;
     struct dfa *d;
{
  dfa = d;
  lexstart = lexptr = s;
  lexleft = len;
  lasttok = END;
  laststart = 1;
  parens = 0;
  if (! syntax_bits_set)
    dfaerror("No syntax specified");
  tok = lex();
  depth = d->depth;
  regexp(1);
  if (tok != END)
    dfaerror("Unbalanced )");
  addtok(END - d->nregexps);
  addtok(CAT);
  if (d->nregexps)
    addtok(ORTOP);
  ++d->nregexps;
}
static void
copy(src, dst)
     position_set *src;
     position_set *dst;
{
  int i;
  for (i = 0; i < src->nelem; ++i)
    dst->elems[i] = src->elems[i];
  dst->nelem = src->nelem;
}
static void
insert(p, s)
     position p;
     position_set *s;
{
  int i;
  position t1, t2;
  for (i = 0; i < s->nelem && p.strchr < s->elems[i].strchr; ++i)
    ;
  if (i < s->nelem && p.strchr == s->elems[i].strchr)
    s->elems[i].constraint |= p.constraint;
  else
    {
      t1 = p;
      ++s->nelem;
      while (i < s->nelem)
 {
   t2 = s->elems[i];
   s->elems[i++] = t1;
   t1 = t2;
 }
    }
}
static void
merge(s1, s2, m)
     position_set *s1;
     position_set *s2;
     position_set *m;
{
  int i = 0, j = 0;
  m->nelem = 0;
  while (i < s1->nelem && j < s2->nelem)
    if (s1->elems[i].strchr > s2->elems[j].strchr)
      m->elems[m->nelem++] = s1->elems[i++];
    else if (s1->elems[i].strchr < s2->elems[j].strchr)
      m->elems[m->nelem++] = s2->elems[j++];
    else
      {
 m->elems[m->nelem] = s1->elems[i++];
 m->elems[m->nelem++].constraint |= s2->elems[j++].constraint;
      }
  while (i < s1->nelem)
    m->elems[m->nelem++] = s1->elems[i++];
  while (j < s2->nelem)
    m->elems[m->nelem++] = s2->elems[j++];
}
static void
delete(p, s)
     position p;
     position_set *s;
{
  int i;
  for (i = 0; i < s->nelem; ++i)
    if (p.strchr == s->elems[i].strchr)
      break;
  if (i < s->nelem)
    for (--s->nelem; i < s->nelem; ++i)
      s->elems[i] = s->elems[i + 1];
}
static int
state_index(d, s, newline, letter)
     struct dfa *d;
     position_set *s;
     int newline;
     int letter;
{
  int hash = 0;
  int constraint;
  int i, j;
  newline = newline ? 1 : 0;
  letter = letter ? 1 : 0;
  for (i = 0; i < s->nelem; ++i)
    hash ^= s->elems[i].strchr + s->elems[i].constraint;
  for (i = 0; i < d->sindex; ++i)
    {
      if (hash != d->states[i].hash || s->nelem != d->states[i].elems.nelem
   || newline != d->states[i].newline || letter != d->states[i].letter)
 continue;
      for (j = 0; j < s->nelem; ++j)
 if (s->elems[j].constraint
     != d->states[i].elems.elems[j].constraint
     || s->elems[j].strchr != d->states[i].elems.elems[j].strchr)
   break;
      if (j == s->nelem)
 return i;
    }
  if ((d->sindex) >= (d->salloc)) { while ((d->sindex) >= (d->salloc)) (d->salloc) *= 2; ((d->states) = (dfa_state *) xrealloc_1((ptr_t) (d->states), (d->salloc) * sizeof (dfa_state))); };
  d->states[i].hash = hash;
  ((d->states[i].elems.elems) = (position *) xmalloc_1((s->nelem) * sizeof (position)));
  copy(s, &d->states[i].elems);
  d->states[i].newline = newline;
  d->states[i].letter = letter;
  d->states[i].backref = 0;
  d->states[i].constraint = 0;
  d->states[i].first_end = 0;
  for (j = 0; j < s->nelem; ++j)
    if (d->tokens[s->elems[j].strchr] < 0)
      {
 constraint = s->elems[j].constraint;
 if ((((constraint) & 1 << (((newline) ? 2 : 0) + ((0) ? 1 : 0) + 4)) && ((constraint) & 1 << (((letter) ? 2 : 0) + ((0) ? 1 : 0))))
     || (((constraint) & 1 << (((newline) ? 2 : 0) + ((0) ? 1 : 0) + 4)) && ((constraint) & 1 << (((letter) ? 2 : 0) + ((1) ? 1 : 0))))
     || (((constraint) & 1 << (((newline) ? 2 : 0) + ((1) ? 1 : 0) + 4)) && ((constraint) & 1 << (((letter) ? 2 : 0) + ((0) ? 1 : 0))))
     || (((constraint) & 1 << (((newline) ? 2 : 0) + ((1) ? 1 : 0) + 4)) && ((constraint) & 1 << (((letter) ? 2 : 0) + ((1) ? 1 : 0)))))
   d->states[i].constraint |= constraint;
 if (! d->states[i].first_end)
   d->states[i].first_end = d->tokens[s->elems[j].strchr];
      }
    else if (d->tokens[s->elems[j].strchr] == BACKREF)
      {
 d->states[i].constraint = 0xff;
 d->states[i].backref = 1;
      }
  ++d->sindex;
  return i;
}
void
epsclosure(s, d)
     position_set *s;
     struct dfa *d;
{
  int i, j;
  int *visited;
  position p, old;
  ((visited) = (int *) xmalloc_1((d->tindex) * sizeof (int)));
  for (i = 0; i < d->tindex; ++i)
    visited[i] = 0;
  for (i = 0; i < s->nelem; ++i)
    if (d->tokens[s->elems[i].strchr] >= (1 << 8)
 && d->tokens[s->elems[i].strchr] != BACKREF
 && d->tokens[s->elems[i].strchr] < CSET)
      {
 old = s->elems[i];
 p.constraint = old.constraint;
 delete(s->elems[i], s);
 if (visited[old.strchr])
   {
     --i;
     continue;
   }
 visited[old.strchr] = 1;
 switch (d->tokens[old.strchr])
   {
   case BEGLINE:
     p.constraint &= 0xcf;
     break;
   case ENDLINE:
     p.constraint &= 0xaf;
     break;
   case BEGWORD:
     p.constraint &= 0xf2;
     break;
   case ENDWORD:
     p.constraint &= 0xf4;
     break;
   case LIMWORD:
     p.constraint &= 0xf6;
     break;
   case NOTLIMWORD:
     p.constraint &= 0xf9;
     break;
   default:
     break;
   }
 for (j = 0; j < d->follows[old.strchr].nelem; ++j)
   {
     p.strchr = d->follows[old.strchr].elems[j].strchr;
     insert(p, s);
   }
 i = -1;
      }
  free(visited);
}
void
dfaanalyze(d, searchflag)
     struct dfa *d;
     int searchflag;
{
  int *nullable;
  int *nfirstpos;
  position *firstpos;
  int *nlastpos;
  position *lastpos;
  int *nalloc;
  position_set tmp;
  position_set merged;
  int wants_newline;
  int *o_nullable;
  int *o_nfirst, *o_nlast;
  position *o_firstpos, *o_lastpos;
  int i, j;
  position *pos;
  d->searchflag = searchflag;
  ((nullable) = (int *) xmalloc_1((d->depth) * sizeof (int)));
  o_nullable = nullable;
  ((nfirstpos) = (int *) xmalloc_1((d->depth) * sizeof (int)));
  o_nfirst = nfirstpos;
  ((firstpos) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
  o_firstpos = firstpos, firstpos += d->nleaves;
  ((nlastpos) = (int *) xmalloc_1((d->depth) * sizeof (int)));
  o_nlast = nlastpos;
  ((lastpos) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
  o_lastpos = lastpos, lastpos += d->nleaves;
  ((nalloc) = (int *) xmalloc_1((d->tindex) * sizeof (int)));
  for (i = 0; i < d->tindex; ++i)
    nalloc[i] = 0;
  ((merged.elems) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
  ((d->follows) = (position_set *) xcalloc((d->tindex), sizeof (position_set)));
  for (i = 0; i < d->tindex; ++i)
    switch (d->tokens[i])
      {
      case EMPTY:
 *nullable++ = 1;
 *nfirstpos++ = *nlastpos++ = 0;
 break;
      case STAR:
      case PLUS:
 tmp.nelem = nfirstpos[-1];
 tmp.elems = firstpos;
 pos = lastpos;
 for (j = 0; j < nlastpos[-1]; ++j)
   {
     merge(&tmp, &d->follows[pos[j].strchr], &merged);
     if ((merged.nelem - 1) >= (nalloc[pos[j].strchr])) { while ((merged.nelem - 1) >= (nalloc[pos[j].strchr])) (nalloc[pos[j].strchr]) *= 2; ((d->follows[pos[j].strchr].elems) = (position *) xrealloc_1((ptr_t) (d->follows[pos[j].strchr].elems), (nalloc[pos[j].strchr]) * sizeof (position))); }
                                            ;
     copy(&merged, &d->follows[pos[j].strchr]);
   }
      case QMARK:
 if (d->tokens[i] != PLUS)
   nullable[-1] = 1;
 break;
      case CAT:
 tmp.nelem = nfirstpos[-1];
 tmp.elems = firstpos;
 pos = lastpos + nlastpos[-1];
 for (j = 0; j < nlastpos[-2]; ++j)
   {
     merge(&tmp, &d->follows[pos[j].strchr], &merged);
     if ((merged.nelem - 1) >= (nalloc[pos[j].strchr])) { while ((merged.nelem - 1) >= (nalloc[pos[j].strchr])) (nalloc[pos[j].strchr]) *= 2; ((d->follows[pos[j].strchr].elems) = (position *) xrealloc_1((ptr_t) (d->follows[pos[j].strchr].elems), (nalloc[pos[j].strchr]) * sizeof (position))); }
                                            ;
     copy(&merged, &d->follows[pos[j].strchr]);
   }
 if (nullable[-2])
   nfirstpos[-2] += nfirstpos[-1];
 else
   firstpos += nfirstpos[-1];
 --nfirstpos;
 if (nullable[-1])
   nlastpos[-2] += nlastpos[-1];
 else
   {
     pos = lastpos + nlastpos[-2];
     for (j = nlastpos[-1] - 1; j >= 0; --j)
       pos[j] = lastpos[j];
     lastpos += nlastpos[-2];
     nlastpos[-2] = nlastpos[-1];
   }
 --nlastpos;
 nullable[-2] = nullable[-1] && nullable[-2];
 --nullable;
 break;
      case OR:
      case ORTOP:
 nfirstpos[-2] += nfirstpos[-1];
 --nfirstpos;
 nlastpos[-2] += nlastpos[-1];
 --nlastpos;
 nullable[-2] = nullable[-1] || nullable[-2];
 --nullable;
 break;
      default:
 *nullable++ = d->tokens[i] == BACKREF;
 *nfirstpos++ = *nlastpos++ = 1;
 --firstpos, --lastpos;
 firstpos->strchr = lastpos->strchr = i;
 firstpos->constraint = lastpos->constraint = 0xff;
 nalloc[i] = 1;
 ((d->follows[i].elems) = (position *) xmalloc_1((nalloc[i]) * sizeof (position)));
 break;
      }
  for (i = 0; i < d->tindex; ++i)
    if (d->tokens[i] < (1 << 8) || d->tokens[i] == BACKREF
 || d->tokens[i] >= CSET)
      {
 copy(&d->follows[i], &merged);
 epsclosure(&merged, d);
 if (d->follows[i].nelem < merged.nelem)
   ((d->follows[i].elems) = (position *) xrealloc_1((ptr_t) (d->follows[i].elems), (merged.nelem) * sizeof (position)));
 copy(&merged, &d->follows[i]);
      }
  merged.nelem = 0;
  for (i = 0; i < nfirstpos[-1]; ++i)
    insert(firstpos[i], &merged);
  epsclosure(&merged, d);
  wants_newline = 0;
  for (i = 0; i < merged.nelem; ++i)
    if ((((merged.elems[i].constraint) & 0xc0) >> 2 != ((merged.elems[i].constraint) & 0x30)))
      wants_newline = 1;
  d->salloc = 1;
  d->sindex = 0;
  ((d->states) = (dfa_state *) xmalloc_1((d->salloc) * sizeof (dfa_state)));
  state_index(d, &merged, wants_newline, 0);
  free(o_nullable);
  free(o_nfirst);
  free(o_firstpos);
  free(o_nlast);
  free(o_lastpos);
  free(nalloc);
  free(merged.elems);
}
void
dfastate(s, d, trans)
     int s;
     struct dfa *d;
     int trans[];
{
  position_set grps[(1 << 8)];
  charclass labels[(1 << 8)];
  int ngrps = 0;
  position pos;
  charclass matches;
  int matchesf;
  charclass intersect;
  int intersectf;
  charclass leftovers;
  int leftoversf;
  static charclass letters;
  static charclass newline;
  position_set follows;
  position_set tmp;
  int state;
  int wants_newline;
  int state_newline;
  int wants_letter;
  int state_letter;
  static initialized;
  int i, j, k;
  if (! initialized)
    {
      initialized = 1;
      for (i = 0; i < (1 << 8); ++i)
 if ((1 && isalnum(i)))
   setbit(i, letters);
      setbit('\n', newline);
    }
  zeroset(matches);
  for (i = 0; i < d->states[s].elems.nelem; ++i)
    {
      pos = d->states[s].elems.elems[i];
      if (d->tokens[pos.strchr] >= 0 && d->tokens[pos.strchr] < (1 << 8))
 setbit(d->tokens[pos.strchr], matches);
      else if (d->tokens[pos.strchr] >= CSET)
 copyset(d->charclasses[d->tokens[pos.strchr] - CSET], matches);
      else
 continue;
      if (pos.constraint != 0xFF)
 {
   if (! ((pos.constraint) & 1 << (((d->states[s].newline) ? 2 : 0) + ((1) ? 1 : 0) + 4))
                              )
     clrbit('\n', matches);
   if (! ((pos.constraint) & 1 << (((d->states[s].newline) ? 2 : 0) + ((0) ? 1 : 0) + 4))
                              )
     for (j = 0; j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++j)
       matches[j] &= newline[j];
   if (! ((pos.constraint) & 1 << (((d->states[s].letter) ? 2 : 0) + ((1) ? 1 : 0)))
                            )
     for (j = 0; j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++j)
       matches[j] &= ~letters[j];
   if (! ((pos.constraint) & 1 << (((d->states[s].letter) ? 2 : 0) + ((0) ? 1 : 0)))
                            )
     for (j = 0; j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++j)
       matches[j] &= letters[j];
   for (j = 0; j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))) && !matches[j]; ++j)
     ;
   if (j == (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))))
     continue;
 }
      for (j = 0; j < ngrps; ++j)
 {
   if (d->tokens[pos.strchr] >= 0 && d->tokens[pos.strchr] < (1 << 8)
       && !tstbit(d->tokens[pos.strchr], labels[j]))
     continue;
   intersectf = 0;
   for (k = 0; k < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++k)
     (intersect[k] = matches[k] & labels[j][k]) ? intersectf = 1 : 0;
   if (! intersectf)
     continue;
   leftoversf = matchesf = 0;
   for (k = 0; k < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++k)
     {
       int match = matches[k], label = labels[j][k];
       (leftovers[k] = ~match & label) ? leftoversf = 1 : 0;
       (matches[k] = match & ~label) ? matchesf = 1 : 0;
     }
   if (leftoversf)
     {
       copyset(leftovers, labels[ngrps]);
       copyset(intersect, labels[j]);
       ((grps[ngrps].elems) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
       copy(&grps[j], &grps[ngrps]);
       ++ngrps;
     }
   grps[j].elems[grps[j].nelem++] = pos;
   if (! matchesf)
     break;
 }
      if (j == ngrps)
 {
   copyset(matches, labels[ngrps]);
   zeroset(matches);
   ((grps[ngrps].elems) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
   grps[ngrps].nelem = 1;
   grps[ngrps].elems[0] = pos;
   ++ngrps;
 }
    }
  ((follows.elems) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
  ((tmp.elems) = (position *) xmalloc_1((d->nleaves) * sizeof (position)));
  if (d->searchflag)
    {
      wants_newline = 0;
      wants_letter = 0;
      for (i = 0; i < d->states[0].elems.nelem; ++i)
 {
   if ((((d->states[0].elems.elems[i].constraint) & 0xc0) >> 2 != ((d->states[0].elems.elems[i].constraint) & 0x30)))
     wants_newline = 1;
   if ((((d->states[0].elems.elems[i].constraint) & 0x0c) >> 2 != ((d->states[0].elems.elems[i].constraint) & 0x03)))
     wants_letter = 1;
 }
      copy(&d->states[0].elems, &follows);
      state = state_index(d, &follows, 0, 0);
      if (wants_newline)
 state_newline = state_index(d, &follows, 1, 0);
      else
 state_newline = state;
      if (wants_letter)
 state_letter = state_index(d, &follows, 0, 1);
      else
 state_letter = state;
      for (i = 0; i < (1 << 8); ++i)
 if (i == '\n')
   trans[i] = state_newline;
 else if ((1 && isalnum(i)))
   trans[i] = state_letter;
 else
   trans[i] = state;
    }
  else
    for (i = 0; i < (1 << 8); ++i)
      trans[i] = -1;
  for (i = 0; i < ngrps; ++i)
    {
      follows.nelem = 0;
      for (j = 0; j < grps[i].nelem; ++j)
 for (k = 0; k < d->follows[grps[i].elems[j].strchr].nelem; ++k)
   insert(d->follows[grps[i].elems[j].strchr].elems[k], &follows);
      if (d->searchflag)
 for (j = 0; j < d->states[0].elems.nelem; ++j)
   insert(d->states[0].elems.elems[j], &follows);
      wants_newline = 0;
      if (tstbit('\n', labels[i]))
 for (j = 0; j < follows.nelem; ++j)
   if ((((follows.elems[j].constraint) & 0xc0) >> 2 != ((follows.elems[j].constraint) & 0x30)))
     wants_newline = 1;
      wants_letter = 0;
      for (j = 0; j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++j)
 if (labels[i][j] & letters[j])
   break;
      if (j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))))
 for (j = 0; j < follows.nelem; ++j)
   if ((((follows.elems[j].constraint) & 0x0c) >> 2 != ((follows.elems[j].constraint) & 0x03)))
     wants_letter = 1;
      state = state_index(d, &follows, 0, 0);
      if (wants_newline)
 state_newline = state_index(d, &follows, 1, 0);
      else
 state_newline = state;
      if (wants_letter)
 state_letter = state_index(d, &follows, 0, 1);
      else
 state_letter = state;
      for (j = 0; j < (((1 << 8) + (8 * sizeof (int)) - 1) / (8 * sizeof (int))); ++j)
 for (k = 0; k < (8 * sizeof (int)); ++k)
   if (labels[i][j] & 1 << k)
     {
       int c = j * (8 * sizeof (int)) + k;
       if (c == '\n')
  trans[c] = state_newline;
       else if ((1 && isalnum(c)))
  trans[c] = state_letter;
       else if (c < (1 << 8))
  trans[c] = state;
     }
    }
  for (i = 0; i < ngrps; ++i)
    free(grps[i].elems);
  free(follows.elems);
  free(tmp.elems);
}
static void
build_state(s, d)
     int s;
     struct dfa *d;
{
  int *trans;
  int i;
  if (d->trcount >= 1024)
    {
      for (i = 0; i < d->tralloc; ++i)
 if (d->trans[i])
   {
     free((ptr_t) d->trans[i]);
     d->trans[i] = 0;
   }
 else if (d->fails[i])
   {
     free((ptr_t) d->fails[i]);
     d->fails[i] = 0;
   }
      d->trcount = 0;
    }
  ++d->trcount;
  d->success[s] = 0;
  if (((((*d).states[s].constraint) & 1 << (((d->states[s].newline) ? 2 : 0) + ((1) ? 1 : 0) + 4)) && (((*d).states[s].constraint) & 1 << (((d->states[s].letter) ? 2 : 0) + ((0) ? 1 : 0))))
            )
    d->success[s] |= 4;
  if (((((*d).states[s].constraint) & 1 << (((d->states[s].newline) ? 2 : 0) + ((0) ? 1 : 0) + 4)) && (((*d).states[s].constraint) & 1 << (((d->states[s].letter) ? 2 : 0) + ((1) ? 1 : 0))))
            )
    d->success[s] |= 2;
  if (((((*d).states[s].constraint) & 1 << (((d->states[s].newline) ? 2 : 0) + ((0) ? 1 : 0) + 4)) && (((*d).states[s].constraint) & 1 << (((d->states[s].letter) ? 2 : 0) + ((0) ? 1 : 0))))
            )
    d->success[s] |= 1;
  ((trans) = (int *) xmalloc_1(((1 << 8)) * sizeof (int)));
  dfastate(s, d, trans);
  for (i = 0; i < (1 << 8); ++i)
    if (trans[i] >= d->tralloc)
      {
 int oldalloc = d->tralloc;
 while (trans[i] >= d->tralloc)
   d->tralloc *= 2;
 ((d->realtrans) = (int * *) xrealloc_1((ptr_t) (d->realtrans), (d->tralloc + 1) * sizeof (int *)));
 d->trans = d->realtrans + 1;
 ((d->fails) = (int * *) xrealloc_1((ptr_t) (d->fails), (d->tralloc) * sizeof (int *)));
 ((d->success) = (int *) xrealloc_1((ptr_t) (d->success), (d->tralloc) * sizeof (int)));
 ((d->newlines) = (int *) xrealloc_1((ptr_t) (d->newlines), (d->tralloc) * sizeof (int)));
 while (oldalloc < d->tralloc)
   {
     d->trans[oldalloc] = 0;
     d->fails[oldalloc++] = 0;
   }
      }
  d->newlines[s] = trans['\n'];
  trans['\n'] = -1;
  if (((*d).states[s].constraint))
    d->fails[s] = trans;
  else
    d->trans[s] = trans;
}
static void
build_state_zero(d)
     struct dfa *d;
{
  d->tralloc = 1;
  d->trcount = 0;
  ((d->realtrans) = (int * *) xcalloc((d->tralloc + 1), sizeof (int *)));
  d->trans = d->realtrans + 1;
  ((d->fails) = (int * *) xcalloc((d->tralloc), sizeof (int *)));
  ((d->success) = (int *) xmalloc_1((d->tralloc) * sizeof (int)));
  ((d->newlines) = (int *) xmalloc_1((d->tralloc) * sizeof (int)));
  build_state(0, d);
}
char *
dfaexec(d, begin, end, newline, count, backref)
     struct dfa *d;
     char *begin;
     char *end;
     int newline;
     int *count;
     int *backref;
{
  register s, s1, tmp;
  register unsigned char *p;
  register **trans, *t;
  static sbit[(1 << 8)];
  static sbit_init;
  if (! sbit_init)
    {
      int i;
      sbit_init = 1;
      for (i = 0; i < (1 << 8); ++i)
 if (i == '\n')
   sbit[i] = 4;
 else if ((1 && isalnum(i)))
   sbit[i] = 2;
 else
   sbit[i] = 1;
    }
  if (! d->tralloc)
    build_state_zero(d);
  s = s1 = 0;
  p = (unsigned char *) begin;
  trans = d->trans;
  *end = '\n';
  for (;;)
    {
      if ((t = trans[s]) != 0)
 do
   {
     s1 = t[*p++];
     if (! (t = trans[s1]))
       goto last_was_s;
     s = t[*p++];
   }
        while ((t = trans[s]) != 0);
      goto last_was_s1;
    last_was_s:
      tmp = s, s = s1, s1 = tmp;
    last_was_s1:
      if (s >= 0 && p <= (unsigned char *) end && d->fails[s])
 {
   if (d->success[s] & sbit[*p])
     {
       if (backref)
  if (d->states[s].backref)
    *backref = 1;
  else
    *backref = 0;
       return (char *) p;
     }
   s1 = s;
   s = d->fails[s][*p++];
   continue;
 }
      if (count && (char *) p <= end && p[-1] == '\n')
 ++*count;
      if ((char *) p > end)
 return 0;
      if (s >= 0)
 {
   build_state(s, d);
   trans = d->trans;
   continue;
 }
      if (p[-1] == '\n' && newline)
 {
   s = d->newlines[s1];
   continue;
 }
      s = 0;
    }
}
void
dfainit(d)
     struct dfa *d;
{
  d->calloc = 1;
  ((d->charclasses) = (charclass *) xmalloc_1((d->calloc) * sizeof (charclass)));
  d->cindex = 0;
  d->talloc = 1;
  ((d->tokens) = (token *) xmalloc_1((d->talloc) * sizeof (token)));
  d->tindex = d->depth = d->nleaves = d->nregexps = 0;
  d->searchflag = 0;
  d->tralloc = 0;
  d->musts = 0;
}
void
dfacomp(s, len, d, searchflag)
     char *s;
     size_t len;
     struct dfa *d;
     int searchflag;
{
  if (case_fold)
    {
      char *copy;
      int i;
      copy = malloc(len);
      if (!copy)
 dfaerror("out of memory");
      case_fold = 0;
      for (i = 0; i < len; ++i)
 if ((1 && isupper(s[i])))
   copy[i] = tolower(s[i]);
 else
   copy[i] = s[i];
      dfainit(d);
      dfaparse(copy, len, d);
      free(copy);
      dfamust(d);
      d->cindex = d->tindex = d->depth = d->nleaves = d->nregexps = 0;
      case_fold = 1;
      dfaparse(s, len, d);
      dfaanalyze(d, searchflag);
    }
  else
    {
        dfainit(d);
        dfaparse(s, len, d);
 dfamust(d);
        dfaanalyze(d, searchflag);
    }
}
void
dfafree(d)
     struct dfa *d;
{
  int i;
  struct dfamust *dm, *ndm;
  free((ptr_t) d->charclasses);
  free((ptr_t) d->tokens);
  for (i = 0; i < d->sindex; ++i)
    free((ptr_t) d->states[i].elems.elems);
  free((ptr_t) d->states);
  for (i = 0; i < d->tindex; ++i)
    if (d->follows[i].elems)
      free((ptr_t) d->follows[i].elems);
  free((ptr_t) d->follows);
  for (i = 0; i < d->tralloc; ++i)
    if (d->trans[i])
      free((ptr_t) d->trans[i]);
    else if (d->fails[i])
      free((ptr_t) d->fails[i]);
  free((ptr_t) d->realtrans);
  free((ptr_t) d->fails);
  free((ptr_t) d->newlines);
  for (dm = d->musts; dm; dm = ndm)
    {
      ndm = dm->next;
      free(dm->must);
      free((ptr_t) dm);
    }
}
static char *
icatalloc(old, new)
     char *old;
     char *new;
{
  char *result;
  int oldsize, newsize;
  newsize = (new == 0) ? 0 : strlen(new);
  if (old == 0)
    oldsize = 0;
  else if (newsize == 0)
    return old;
  else oldsize = strlen(old);
  if (old == 0)
    result = (char *) malloc(newsize + 1);
  else
    result = (char *) realloc((void *) old, oldsize + newsize + 1);
  if (result != 0 && new != 0)
    (void) strcpy(result + oldsize, new);
  return result;
}
static char *
icpyalloc(string)
     char *string;
{
  return icatalloc((char *) 0, string);
}
static char *
istrstr(lookin, lookfor)
     char *lookin;
     char *lookfor;
{
  char *cp;
  int len;
  len = strlen(lookfor);
  for (cp = lookin; *cp != '\0'; ++cp)
    if (strncmp(cp, lookfor, len) == 0)
      return cp;
  return 0;
}
static void
ifree(cp)
     char *cp;
{
  if (cp != 0)
    free(cp);
}
static void
freelist(cpp)
     char **cpp;
{
  int i;
  if (cpp == 0)
    return;
  for (i = 0; cpp[i] != 0; ++i)
    {
      free(cpp[i]);
      cpp[i] = 0;
    }
}
static char **
enlist(cpp, new, len)
     char **cpp;
     char *new;
     int len;
{
  int i, j;
  if (cpp == 0)
    return 0;
  if ((new = icpyalloc(new)) == 0)
    {
      freelist(cpp);
      return 0;
    }
  new[len] = '\0';
  for (i = 0; cpp[i] != 0; ++i)
    if (istrstr(cpp[i], new) != 0)
      {
 free(new);
 return cpp;
      }
  j = 0;
  while (cpp[j] != 0)
    if (istrstr(new, cpp[j]) == 0)
      ++j;
    else
      {
 free(cpp[j]);
 if (--i == j)
   break;
 cpp[j] = cpp[i];
 cpp[i] = 0;
      }
  cpp = (char **) realloc((char *) cpp, (i + 2) * sizeof *cpp);
  if (cpp == 0)
    return 0;
  cpp[i] = new;
  cpp[i + 1] = 0;
  return cpp;
}
static char **
comsubs(left, right)
     char *left;
     char *right;
{
  char **cpp;
  char *lcp;
  char *rcp;
  int i, len;
  if (left == 0 || right == 0)
    return 0;
  cpp = (char **) malloc(sizeof *cpp);
  if (cpp == 0)
    return 0;
  cpp[0] = 0;
  for (lcp = left; *lcp != '\0'; ++lcp)
    {
      len = 0;
      rcp = strchr(right, *lcp);
      while (rcp != 0)
 {
   for (i = 1; lcp[i] != '\0' && lcp[i] == rcp[i]; ++i)
     ;
   if (i > len)
     len = i;
   rcp = strchr(rcp + 1, *lcp);
 }
      if (len == 0)
 continue;
      if ((cpp = enlist(cpp, lcp, len)) == 0)
 break;
    }
  return cpp;
}
static char **
addlists(old, new)
char **old;
char **new;
{
  int i;
  if (old == 0 || new == 0)
    return 0;
  for (i = 0; new[i] != 0; ++i)
    {
      old = enlist(old, new[i], strlen(new[i]));
      if (old == 0)
 break;
    }
  return old;
}
static char **
inboth(left, right)
     char **left;
     char **right;
{
  char **both;
  char **temp;
  int lnum, rnum;
  if (left == 0 || right == 0)
    return 0;
  both = (char **) malloc(sizeof *both);
  if (both == 0)
    return 0;
  both[0] = 0;
  for (lnum = 0; left[lnum] != 0; ++lnum)
    {
      for (rnum = 0; right[rnum] != 0; ++rnum)
 {
   temp = comsubs(left[lnum], right[rnum]);
   if (temp == 0)
     {
       freelist(both);
       return 0;
     }
   both = addlists(both, temp);
   freelist(temp);
   if (both == 0)
     return 0;
 }
    }
  return both;
}
typedef struct
{
  char **in;
  char *left;
  char *right;
  char *is;
} must;
static void
resetmust(mp)
must *mp;
{
  mp->left[0] = mp->right[0] = mp->is[0] = '\0';
  freelist(mp->in);
}
static void
dfamust(dfa)
struct dfa *dfa;
{
  must *musts;
  must *mp;
  char *result;
  int ri;
  int i;
  int exact;
  token t;
  static must must0;
  struct dfamust *dm;
  result = "";
  exact = 0;
  musts = (must *) malloc((dfa->tindex + 1) * sizeof *musts);
  if (musts == 0)
    return;
  mp = musts;
  for (i = 0; i <= dfa->tindex; ++i)
    mp[i] = must0;
  for (i = 0; i <= dfa->tindex; ++i)
    {
      mp[i].in = (char **) malloc(sizeof *mp[i].in);
      mp[i].left = malloc(2);
      mp[i].right = malloc(2);
      mp[i].is = malloc(2);
      if (mp[i].in == 0 || mp[i].left == 0 ||
   mp[i].right == 0 || mp[i].is == 0)
 goto done;
      mp[i].left[0] = mp[i].right[0] = mp[i].is[0] = '\0';
      mp[i].in[0] = 0;
    }
  for (ri = 0; ri < dfa->tindex; ++ri)
    {
      switch (t = dfa->tokens[ri])
 {
 case LPAREN:
 case RPAREN:
   goto done;
 case EMPTY:
 case BEGLINE:
 case ENDLINE:
 case BEGWORD:
 case ENDWORD:
 case LIMWORD:
 case NOTLIMWORD:
 case BACKREF:
   resetmust(mp);
   break;
 case STAR:
 case QMARK:
   if (mp <= musts)
     goto done;
   --mp;
   resetmust(mp);
   break;
 case OR:
 case ORTOP:
   if (mp < &musts[2])
     goto done;
   {
     char **new;
     must *lmp;
     must *rmp;
     int j, ln, rn, n;
     rmp = --mp;
     lmp = --mp;
     if (strcmp(lmp->is, rmp->is) != 0)
       lmp->is[0] = '\0';
     i = 0;
     while (lmp->left[i] != '\0' && lmp->left[i] == rmp->left[i])
       ++i;
     lmp->left[i] = '\0';
     ln = strlen(lmp->right);
     rn = strlen(rmp->right);
     n = ln;
     if (n > rn)
       n = rn;
     for (i = 0; i < n; ++i)
       if (lmp->right[ln - i - 1] != rmp->right[rn - i - 1])
  break;
     for (j = 0; j < i; ++j)
       lmp->right[j] = lmp->right[(ln - i) + j];
     lmp->right[j] = '\0';
     new = inboth(lmp->in, rmp->in);
     if (new == 0)
       goto done;
     freelist(lmp->in);
     free((char *) lmp->in);
     lmp->in = new;
   }
   break;
 case PLUS:
   if (mp <= musts)
     goto done;
   --mp;
   mp->is[0] = '\0';
   break;
 case END:
   if (mp != &musts[1])
     goto done;
   for (i = 0; musts[0].in[i] != 0; ++i)
     if (strlen(musts[0].in[i]) > strlen(result))
       result = musts[0].in[i];
   if (strcmp(result, musts[0].is) == 0)
     exact = 1;
   goto done;
 case CAT:
   if (mp < &musts[2])
     goto done;
   {
     must *lmp;
     must *rmp;
     rmp = --mp;
     lmp = --mp;
     lmp->in = addlists(lmp->in, rmp->in);
     if (lmp->in == 0)
       goto done;
     if (lmp->right[0] != '\0' &&
  rmp->left[0] != '\0')
       {
  char *tp;
  tp = icpyalloc(lmp->right);
  if (tp == 0)
    goto done;
  tp = icatalloc(tp, rmp->left);
  if (tp == 0)
    goto done;
  lmp->in = enlist(lmp->in, tp,
     strlen(tp));
  free(tp);
  if (lmp->in == 0)
    goto done;
       }
     if (lmp->is[0] != '\0')
       {
  lmp->left = icatalloc(lmp->left,
          rmp->left);
  if (lmp->left == 0)
    goto done;
       }
     if (rmp->is[0] == '\0')
       lmp->right[0] = '\0';
     lmp->right = icatalloc(lmp->right, rmp->right);
     if (lmp->right == 0)
       goto done;
     if (lmp->is[0] != '\0' && rmp->is[0] != '\0')
       {
  lmp->is = icatalloc(lmp->is, rmp->is);
  if (lmp->is == 0)
    goto done;
       }
     else
       lmp->is[0] = '\0';
   }
   break;
 default:
   if (t < END)
     {
       goto done;
     }
   else if (t == '\0')
     {
       goto done;
     }
   else if (t >= CSET)
     {
       resetmust(mp);
     }
   else
     {
       resetmust(mp);
       mp->is[0] = mp->left[0] = mp->right[0] = t;
       mp->is[1] = mp->left[1] = mp->right[1] = '\0';
       mp->in = enlist(mp->in, mp->is, 1);
       if (mp->in == 0)
  goto done;
     }
   break;
 }
      ++mp;
    }
 done:
  if (strlen(result))
    {
      dm = (struct dfamust *) malloc(sizeof (struct dfamust));
      dm->exact = exact;
      dm->must = malloc(strlen(result) + 1);
      strcpy(dm->must, result);
      dm->next = dfa->musts;
      dfa->musts = dm;
    }
  mp = musts;
  for (i = 0; i <= dfa->tindex; ++i)
    {
      freelist(mp[i].in);
      ifree((char *) mp[i].in);
      ifree(mp[i].left);
      ifree(mp[i].right);
      ifree(mp[i].is);
    }
  free((char *) mp);
}
extern char *xmalloc();
struct kwsmatch
{
  int strchr;
  char *beg[1];
  size_t size[1];
};
typedef void *kwset_t;
extern kwset_t kwsalloc(char *);
extern char *kwsincr(kwset_t, char *, size_t);
extern char *kwsprep(kwset_t);
extern char *kwsexec(kwset_t, char *, size_t, struct kwsmatch *);
extern void kwsfree(kwset_t);
struct _obstack_chunk
{
  char *limit;
  struct _obstack_chunk *prev;
  char contents[4];
};
struct obstack
{
  long chunk_size;
  struct _obstack_chunk* chunk;
  char *object_base;
  char *next_free;
  char *chunk_limit;
  ptrdiff_t temp;
  int alignment_mask;
  struct _obstack_chunk *(*chunkfun) ();
  void (*freefun) ();
  char *extra_arg;
  unsigned use_extra_arg:1;
  unsigned maybe_empty_object:1;
};
extern void _obstack_newchunk (struct obstack *, int);
extern void _obstack_free (struct obstack *, void *);
extern void _obstack_begin (struct obstack *, int, int,
       void *(*) (), void (*) ());
extern void _obstack_begin_1 (struct obstack *, int, int,
         void *(*) (), void (*) (), void *);
void obstack_init (struct obstack *obstack);
void * obstack_alloc (struct obstack *obstack, int size);
void * obstack_copy (struct obstack *obstack, void *address, int size);
void * obstack_copy0 (struct obstack *obstack, void *address, int size);
void obstack_free (struct obstack *obstack, void *block);
void obstack_blank (struct obstack *obstack, int size);
void obstack_grow (struct obstack *obstack, void *data, int size);
void obstack_grow0 (struct obstack *obstack, void *data, int size);
void obstack_1grow (struct obstack *obstack, int data_char);
void obstack_ptr_grow (struct obstack *obstack, void *data);
void obstack_int_grow (struct obstack *obstack, int data);
void * obstack_finish (struct obstack *obstack);
int obstack_object_size (struct obstack *obstack);
int obstack_room (struct obstack *obstack);
void obstack_1grow_fast (struct obstack *obstack, int data_char);
void obstack_ptr_grow_fast (struct obstack *obstack, void *data);
void obstack_int_grow_fast (struct obstack *obstack, int data);
void obstack_blank_fast (struct obstack *obstack, int size);
void * obstack_base (struct obstack *obstack);
void * obstack_next_free (struct obstack *obstack);
int obstack_alignment_mask (struct obstack *obstack);
int obstack_chunk_size (struct obstack *obstack);
struct tree
{
  struct tree *llink;
  struct tree *rlink;
  struct trie *trie;
  unsigned char label;
  char balance;
};
struct trie
{
  unsigned int accepting;
  struct tree *links;
  struct trie *parent;
  struct trie *next;
  struct trie *fail;
  int depth;
  int shift;
  int maxshift;
};
struct kwset
{
  struct obstack obstack;
  int words;
  struct trie *trie;
  int mind;
  int maxd;
  unsigned char delta[(255 + 1)];
  struct trie *next[(255 + 1)];
  char *target;
  int mind2;
  char *trans;
};
kwset_t
kwsalloc(trans)
     char *trans;
{
  struct kwset *kwset;
  kwset = (struct kwset *) xmalloc(sizeof (struct kwset));
  if (!kwset)
    return 0;
  _obstack_begin ((&kwset->obstack), 0, 0, (void *(*) ()) xmalloc, (void (*) ()) free);
  kwset->words = 0;
  kwset->trie
    = (struct trie *) ;
  if (!kwset->trie)
    {
      kwsfree((kwset_t) kwset);
      return 0;
    }
  kwset->trie->accepting = 0;
  kwset->trie->links = 0;
  kwset->trie->parent = 0;
  kwset->trie->next = 0;
  kwset->trie->fail = 0;
  kwset->trie->depth = 0;
  kwset->trie->shift = 0;
  kwset->mind = 32767;
  kwset->maxd = -1;
  kwset->target = 0;
  kwset->trans = trans;
  return (kwset_t) kwset;
}
char *
kwsincr(kws, text, len)
     kwset_t kws;
     char *text;
     size_t len;
{
  struct kwset *kwset;
  register struct trie *trie;
  register unsigned char label;
  register struct tree *link;
  register int depth;
  struct tree *links[12];
  enum { L, R } dirs[12];
  struct tree *t, *r, *l, *rl, *lr;
  kwset = (struct kwset *) kws;
  trie = kwset->trie;
  text += len;
  while (len--)
    {
      label = kwset->trans ? kwset->trans[(unsigned char) *--text] : *--text;
      link = trie->links;
      links[0] = (struct tree *) &trie->links;
      dirs[0] = L;
      depth = 1;
      while (link && label != link->label)
 {
   links[depth] = link;
   if (label < link->label)
     dirs[depth++] = L, link = link->llink;
   else
     dirs[depth++] = R, link = link->rlink;
 }
      if (!link)
 {
   link = (struct tree *)
                                 ;
   if (!link)
     return "memory exhausted";
   link->llink = 0;
   link->rlink = 0;
   link->trie = (struct trie *)
                                ;
   if (!link->trie)
     return "memory exhausted";
   link->trie->accepting = 0;
   link->trie->links = 0;
   link->trie->parent = trie;
   link->trie->next = 0;
   link->trie->fail = 0;
   link->trie->depth = trie->depth + 1;
   link->trie->shift = 0;
   link->label = label;
   link->balance = 0;
   if (dirs[--depth] == L)
     links[depth]->llink = link;
   else
     links[depth]->rlink = link;
   while (depth && !links[depth]->balance)
     {
       if (dirs[depth] == L)
  --links[depth]->balance;
       else
  ++links[depth]->balance;
       --depth;
     }
   if (depth && ((dirs[depth] == L && --links[depth]->balance)
   || (dirs[depth] == R && ++links[depth]->balance)))
     {
       switch (links[depth]->balance)
  {
  case (char) -2:
    switch (dirs[depth + 1])
      {
      case L:
        r = links[depth], t = r->llink, rl = t->rlink;
        t->rlink = r, r->llink = rl;
        t->balance = r->balance = 0;
        break;
      case R:
        r = links[depth], l = r->llink, t = l->rlink;
        rl = t->rlink, lr = t->llink;
        t->llink = l, l->rlink = lr, t->rlink = r, r->llink = rl;
        l->balance = t->balance != 1 ? 0 : -1;
        r->balance = t->balance != (char) -1 ? 0 : 1;
        t->balance = 0;
        break;
      }
    break;
  case 2:
    switch (dirs[depth + 1])
      {
      case R:
        l = links[depth], t = l->rlink, lr = t->llink;
        t->llink = l, l->rlink = lr;
        t->balance = l->balance = 0;
        break;
      case L:
        l = links[depth], r = l->rlink, t = r->llink;
        lr = t->llink, rl = t->rlink;
        t->llink = l, l->rlink = lr, t->rlink = r, r->llink = rl;
        l->balance = t->balance != 1 ? 0 : -1;
        r->balance = t->balance != (char) -1 ? 0 : 1;
        t->balance = 0;
        break;
      }
    break;
  }
       if (dirs[depth - 1] == L)
  links[depth - 1]->llink = t;
       else
  links[depth - 1]->rlink = t;
     }
 }
      trie = link->trie;
    }
  if (!trie->accepting)
    trie->accepting = 1 + 2 * kwset->words;
  ++kwset->words;
  if (trie->depth < kwset->mind)
    kwset->mind = trie->depth;
  if (trie->depth > kwset->maxd)
    kwset->maxd = trie->depth;
  return 0;
}
static void
enqueue(tree, last)
     struct tree *tree;
     struct trie **last;
{
  if (!tree)
    return;
  enqueue(tree->llink, last);
  enqueue(tree->rlink, last);
  (*last) = (*last)->next = tree->trie;
}
static void
treefails(tree, fail, recourse)
     register struct tree *tree;
     struct trie *fail;
     struct trie *recourse;
{
  register struct tree *link;
  if (!tree)
    return;
  treefails(tree->llink, fail, recourse);
  treefails(tree->rlink, fail, recourse);
  while (fail)
    {
      link = fail->links;
      while (link && tree->label != link->label)
 if (tree->label < link->label)
   link = link->llink;
 else
   link = link->rlink;
      if (link)
 {
   tree->trie->fail = link->trie;
   return;
 }
      fail = fail->fail;
    }
  tree->trie->fail = recourse;
}
static void
treedelta(tree, depth, delta)
     register struct tree *tree;
     register unsigned int depth;
     unsigned char delta[];
{
  if (!tree)
    return;
  treedelta(tree->llink, depth, delta);
  treedelta(tree->rlink, depth, delta);
  if (depth < delta[tree->label])
    delta[tree->label] = depth;
}
static int
hasevery(a, b)
     register struct tree *a;
     register struct tree *b;
{
  if (!b)
    return 1;
  if (!hasevery(a, b->llink))
    return 0;
  if (!hasevery(a, b->rlink))
    return 0;
  while (a && b->label != a->label)
    if (b->label < a->label)
      a = a->llink;
    else
      a = a->rlink;
  return !!a;
}
static void
treenext(tree, next)
     struct tree *tree;
     struct trie *next[];
{
  if (!tree)
    return;
  treenext(tree->llink, next);
  treenext(tree->rlink, next);
  next[tree->label] = tree->trie;
}
char *
kwsprep(kws)
     kwset_t kws;
{
  register struct kwset *kwset;
  register int i;
  register struct trie *curr, *fail;
  register char *trans;
  unsigned char delta[(255 + 1)];
  struct trie *last, *next[(255 + 1)];
  kwset = (struct kwset *) kws;
  if (kwset->mind < 256)
    for (i = 0; i < (255 + 1); ++i)
      delta[i] = kwset->mind;
  else
    for (i = 0; i < (255 + 1); ++i)
      delta[i] = 255;
  if (kwset->words == 1 && kwset->trans == 0)
    {
      kwset->target = ;
      for (i = kwset->mind - 1, curr = kwset->trie; i >= 0; --i)
 {
   kwset->target[i] = curr->links->label;
   curr = curr->links->trie;
 }
      for (i = 0; i < kwset->mind; ++i)
 delta[(unsigned char) kwset->target[i]] = kwset->mind - (i + 1);
      kwset->mind2 = kwset->mind;
      for (i = 0; i < kwset->mind - 1; ++i)
 if (kwset->target[i] == kwset->target[kwset->mind - 1])
   kwset->mind2 = kwset->mind - (i + 1);
    }
  else
    {
      for (curr = last = kwset->trie; curr; curr = curr->next)
 {
   enqueue(curr->links, &last);
   curr->shift = kwset->mind;
   curr->maxshift = kwset->mind;
   treedelta(curr->links, curr->depth, delta);
   treefails(curr->links, curr->fail, kwset->trie);
   for (fail = curr->fail; fail; fail = fail->fail)
     {
       if (!hasevery(fail->links, curr->links))
  if (curr->depth - fail->depth < fail->shift)
    fail->shift = curr->depth - fail->depth;
       if (curr->accepting && fail->maxshift > curr->depth - fail->depth)
  fail->maxshift = curr->depth - fail->depth;
     }
 }
      for (curr = kwset->trie->next; curr; curr = curr->next)
 {
   if (curr->maxshift > curr->parent->maxshift)
     curr->maxshift = curr->parent->maxshift;
   if (curr->shift > curr->maxshift)
     curr->shift = curr->maxshift;
 }
      for (i = 0; i < (255 + 1); ++i)
 next[i] = 0;
      treenext(kwset->trie->links, next);
      if ((trans = kwset->trans) != 0)
 for (i = 0; i < (255 + 1); ++i)
   kwset->next[i] = next[(unsigned char) trans[i]];
      else
 for (i = 0; i < (255 + 1); ++i)
   kwset->next[i] = next[i];
    }
  if ((trans = kwset->trans) != 0)
    for (i = 0; i < (255 + 1); ++i)
      kwset->delta[i] = delta[(unsigned char) trans[i]];
  else
    for (i = 0; i < (255 + 1); ++i)
      kwset->delta[i] = delta[i];
  return 0;
}
static char *
bmexec(kws, text, size)
     kwset_t kws;
     char *text;
     size_t size;
{
  struct kwset *kwset;
  register unsigned char *d1;
  register char *ep, *sp, *tp;
  register int d, gc, i, len, md2;
  kwset = (struct kwset *) kws;
  len = kwset->mind;
  if (len == 0)
    return text;
  if (len > size)
    return 0;
  if (len == 1)
    return memchr(text, kwset->target[0], size);
  d1 = kwset->delta;
  sp = kwset->target + len;
  gc = ((unsigned char) (sp[-2]));
  md2 = kwset->mind2;
  tp = text + len;
  if (size > 12 * len)
    for (ep = text + size - 11 * len;;)
      {
 while (tp <= ep)
   {
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     if (d == 0)
       goto found;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     if (d == 0)
       goto found;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     if (d == 0)
       goto found;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
     d = d1[((unsigned char) (tp[-1]))], tp += d;
   }
 break;
      found:
 if (((unsigned char) (tp[-2])) == gc)
   {
     for (i = 3; i <= len && ((unsigned char) (tp[-i])) == ((unsigned char) (sp[-i])); ++i)
       ;
     if (i > len)
       return tp - len;
   }
 tp += md2;
      }
  ep = text + size;
  d = d1[((unsigned char) (tp[-1]))];
  while (d <= ep - tp)
    {
      d = d1[((unsigned char) ((tp += d)[-1]))];
      if (d != 0)
 continue;
      if (tp[-2] == gc)
 {
   for (i = 3; i <= len && ((unsigned char) (tp[-i])) == ((unsigned char) (sp[-i])); ++i)
     ;
   if (i > len)
     return tp - len;
 }
      d = md2;
    }
  return 0;
}
static char *
cwexec(kws, text, len, kwsmatch)
     kwset_t kws;
     char *text;
     size_t len;
     struct kwsmatch *kwsmatch;
{
  struct kwset *kwset;
  struct trie **next, *trie, *accept;
  char *beg, *lim, *mch, *lmch;
  register unsigned char c, *delta;
  register int d;
  register char *end, *qlim;
  register struct tree *tree;
  register char *trans;
  kwset = (struct kwset *) kws;
  if (len < kwset->mind)
    return 0;
  next = kwset->next;
  delta = kwset->delta;
  trans = kwset->trans;
  lim = text + len;
  end = text;
  if ((d = kwset->mind) != 0)
    mch = 0;
  else
    {
      mch = text, accept = kwset->trie;
      goto match;
    }
  if (len >= 4 * kwset->mind)
    qlim = lim - 4 * kwset->mind;
  else
    qlim = 0;
  while (lim - end >= d)
    {
      if (qlim && end <= qlim)
 {
   end += d - 1;
   while ((d = delta[c = *end]) && end < qlim)
     {
       end += d;
       end += delta[(unsigned char) *end];
       end += delta[(unsigned char) *end];
     }
   ++end;
 }
      else
 d = delta[c = (end += d)[-1]];
      if (d)
 continue;
      beg = end - 1;
      trie = next[c];
      if (trie->accepting)
 {
   mch = beg;
   accept = trie;
 }
      d = trie->shift;
      while (beg > text)
 {
   c = trans ? trans[(unsigned char) *--beg] : *--beg;
   tree = trie->links;
   while (tree && c != tree->label)
     if (c < tree->label)
       tree = tree->llink;
     else
       tree = tree->rlink;
   if (tree)
     {
       trie = tree->trie;
       if (trie->accepting)
  {
    mch = beg;
    accept = trie;
  }
     }
   else
     break;
   d = trie->shift;
 }
      if (mch)
 goto match;
    }
  return 0;
 match:
  if (lim - mch > kwset->maxd)
    lim = mch + kwset->maxd;
  lmch = 0;
  d = 1;
  while (lim - end >= d)
    {
      if ((d = delta[c = (end += d)[-1]]) != 0)
 continue;
      beg = end - 1;
      if (!(trie = next[c]))
 {
   d = 1;
   continue;
 }
      if (trie->accepting && beg <= mch)
 {
   lmch = beg;
   accept = trie;
 }
      d = trie->shift;
      while (beg > text)
 {
   c = trans ? trans[(unsigned char) *--beg] : *--beg;
   tree = trie->links;
   while (tree && c != tree->label)
     if (c < tree->label)
       tree = tree->llink;
     else
       tree = tree->rlink;
   if (tree)
     {
       trie = tree->trie;
       if (trie->accepting && beg <= mch)
  {
    lmch = beg;
    accept = trie;
  }
     }
   else
     break;
   d = trie->shift;
 }
      if (lmch)
 {
   mch = lmch;
   goto match;
 }
      if (!d)
 d = 1;
    }
  if (kwsmatch)
    {
      kwsmatch->strchr = accept->accepting / 2;
      kwsmatch->beg[0] = mch;
      kwsmatch->size[0] = accept->depth;
    }
  return mch;
}
char *
kwsexec(kws, text, size, kwsmatch)
     kwset_t kws;
     char *text;
     size_t size;
     struct kwsmatch *kwsmatch;
{
  struct kwset *kwset;
  char *ret;
  kwset = (struct kwset *) kws;
  if (kwset->words == 1 && kwset->trans == 0)
    {
      ret = bmexec(kws, text, size);
      if (kwsmatch != 0 && ret != 0)
 {
   kwsmatch->strchr = 0;
   kwsmatch->beg[0] = ret;
   kwsmatch->size[0] = kwset->mind;
 }
      return ret;
    }
  else
    return cwexec(kws, text, size, kwsmatch);
}
void
kwsfree(kws)
     kwset_t kws;
{
  struct kwset *kwset;
  kwset = (struct kwset *) kws;
  ;
  free(kws);
}
struct fooalign {char x; double d;};
union fooround {long x; double d;};
struct obstack *_obstack;
void
_obstack_begin (h, size, alignment, chunkfun, freefun)
     struct obstack *h;
     int size;
     int alignment;
     void * (*chunkfun) ();
     void (*freefun) ();
{
  register struct _obstack_chunk* chunk;
  if (alignment == 0)
    alignment = ((ptrdiff_t) ((char *)&((struct fooalign *) 0)->d - (char *)0));
  if (size == 0)
    {
      int extra = ((((12 + (sizeof (union fooround)) - 1) & ~((sizeof (union fooround)) - 1))
      + 4 + (sizeof (union fooround)) - 1)
     & ~((sizeof (union fooround)) - 1));
      size = 4096 - extra;
    }
  h->chunkfun = (struct _obstack_chunk * (*)()) chunkfun;
  h->freefun = freefun;
  h->chunk_size = size;
  h->alignment_mask = alignment - 1;
  h->use_extra_arg = 0;
  chunk = h->chunk = (((h) -> use_extra_arg) ? (*(h)->chunkfun) ((h)->extra_arg, (h -> chunk_size)) : (*(h)->chunkfun) ((h -> chunk_size)));
  h->next_free = h->object_base = chunk->contents;
  h->chunk_limit = chunk->limit
    = (char *) chunk + h->chunk_size;
  chunk->prev = 0;
  h->maybe_empty_object = 0;
}
void
_obstack_begin_1 (h, size, alignment, chunkfun, freefun, arg)
     struct obstack *h;
     int size;
     int alignment;
     void * (*chunkfun) ();
     void (*freefun) ();
     void * arg;
{
  register struct _obstack_chunk* chunk;
  if (alignment == 0)
    alignment = ((ptrdiff_t) ((char *)&((struct fooalign *) 0)->d - (char *)0));
  if (size == 0)
    {
      int extra = ((((12 + (sizeof (union fooround)) - 1) & ~((sizeof (union fooround)) - 1))
      + 4 + (sizeof (union fooround)) - 1)
     & ~((sizeof (union fooround)) - 1));
      size = 4096 - extra;
    }
  h->chunkfun = (struct _obstack_chunk * (*)()) chunkfun;
  h->freefun = freefun;
  h->chunk_size = size;
  h->alignment_mask = alignment - 1;
  h->extra_arg = arg;
  h->use_extra_arg = 1;
  chunk = h->chunk = (((h) -> use_extra_arg) ? (*(h)->chunkfun) ((h)->extra_arg, (h -> chunk_size)) : (*(h)->chunkfun) ((h -> chunk_size)));
  h->next_free = h->object_base = chunk->contents;
  h->chunk_limit = chunk->limit
    = (char *) chunk + h->chunk_size;
  chunk->prev = 0;
  h->maybe_empty_object = 0;
}
void
_obstack_newchunk (h, length)
     struct obstack *h;
     int length;
{
  register struct _obstack_chunk* old_chunk = h->chunk;
  register struct _obstack_chunk* new_chunk;
  register long new_size;
  register int obj_size = h->next_free - h->object_base;
  register int i;
  int already;
  new_size = (obj_size + length) + (obj_size >> 3) + 100;
  if (new_size < h->chunk_size)
    new_size = h->chunk_size;
  new_chunk = h->chunk = (((h) -> use_extra_arg) ? (*(h)->chunkfun) ((h)->extra_arg, (new_size)) : (*(h)->chunkfun) ((new_size)));
  new_chunk->prev = old_chunk;
  new_chunk->limit = h->chunk_limit = (char *) new_chunk + new_size;
  if (h->alignment_mask + 1 >= ((ptrdiff_t) ((char *)&((struct fooalign *) 0)->d - (char *)0)))
    {
      for (i = obj_size / sizeof (int) - 1;
    i >= 0; i--)
 ((int *)new_chunk->contents)[i]
   = ((int *)h->object_base)[i];
      already = obj_size / sizeof (int) * sizeof (int);
    }
  else
    already = 0;
  for (i = already; i < obj_size; i++)
    new_chunk->contents[i] = h->object_base[i];
  if (h->object_base == old_chunk->contents && ! h->maybe_empty_object)
    {
      new_chunk->prev = old_chunk->prev;
      do { free ((old_chunk)); } while (0);
    }
  h->object_base = new_chunk->contents;
  h->next_free = h->object_base + obj_size;
  h->maybe_empty_object = 0;
}
int
_obstack_allocated_p (h, obj)
     struct obstack *h;
     void * obj;
{
  register struct _obstack_chunk* lp;
  register struct _obstack_chunk* plp;
  lp = (h)->chunk;
  while (lp != 0 && ((void *)lp >= obj || (void *)(lp)->limit < obj))
    {
      plp = lp->prev;
      lp = plp;
    }
  return lp != 0;
}
void
_obstack_free (h, obj)
     struct obstack *h;
     void * obj;
{
  register struct _obstack_chunk* lp;
  register struct _obstack_chunk* plp;
  lp = h->chunk;
  while (lp != 0 && ((void *)lp >= obj || (void *)(lp)->limit < obj))
    {
      plp = lp->prev;
      do { free ((lp)); } while (0);
      lp = plp;
      h->maybe_empty_object = 1;
    }
  if (lp)
    {
      h->object_base = h->next_free = (char *)(obj);
      h->chunk_limit = lp->limit;
      h->chunk = lp;
    }
  else if (obj != 0)
    abort ();
}
void
obstack_free (h, obj)
     struct obstack *h;
     void * obj;
{
  register struct _obstack_chunk* lp;
  register struct _obstack_chunk* plp;
  lp = h->chunk;
  while (lp != 0 && ((void *)lp >= obj || (void *)(lp)->limit < obj))
    {
      plp = lp->prev;
      do { free ((lp)); } while (0);
      lp = plp;
      h->maybe_empty_object = 1;
    }
  if (lp)
    {
      h->object_base = h->next_free = (char *)(obj);
      h->chunk_limit = lp->limit;
      h->chunk = lp;
    }
  else if (obj != 0)
    abort ();
}
static void Gcompile(char *, size_t);
static void Ecompile(char *, size_t);
static char *EGexecute(char *, size_t, char **);
static void Fcompile(char *, size_t);
static char *Fexecute(char *, size_t, char **);
struct matcher matchers[] = {
  { "default", Gcompile, EGexecute },
  { "grep", Gcompile, EGexecute },
  { "ggrep", Gcompile, EGexecute },
  { "egrep", Ecompile, EGexecute },
  { "posix-egrep", Ecompile, EGexecute },
  { "gegrep", Ecompile, EGexecute },
  { "fgrep", Fcompile, Fexecute },
  { "gfgrep", Fcompile, Fexecute },
  { 0, 0, 0 },
};
static struct dfa dfa_1;
static struct re_pattern_buffer regex;
static kwset_t kwset;
static int lastexact;
void
dfaerror(mesg)
     char *mesg;
{
  fatal(mesg, 0);
}
static void
kwsinit()
{
  static char trans[(255 + 1)];
  int i;
  if (match_icase)
    for (i = 0; i < (255 + 1); ++i)
      trans[i] = ((1 && isupper(i)) ? tolower(i) : (i));
  if (!(kwset = kwsalloc(match_icase ? trans : (char *) 0)))
    fatal("memory exhausted", 0);
}
static void
kwsmusts()
{
  struct dfamust *dm;
  char *err;
  if (dfa_1.musts)
    {
      kwsinit();
      for (dm = dfa_1.musts; dm; dm = dm->next)
 {
   if (!dm->exact)
     continue;
   ++lastexact;
   if ((err = kwsincr(kwset, dm->must, strlen(dm->must))) != 0)
     fatal(err, 0);
 }
      for (dm = dfa_1.musts; dm; dm = dm->next)
 {
   if (dm->exact)
     continue;
   if ((err = kwsincr(kwset, dm->must, strlen(dm->must))) != 0)
     fatal(err, 0);
 }
      if ((err = kwsprep(kwset)) != 0)
 fatal(err, 0);
    }
}
static void
Gcompile(pattern, size)
     char *pattern;
     size_t size;
{
  const
  char *err;
  re_set_syntax((((1) << 1) | (((1) << 1) << 1) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1));
  dfasyntax((((1) << 1) | (((1) << 1) << 1) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1), match_icase);
  if ((err = re_compile_pattern(pattern, size, &regex)) != 0)
    fatal(err, 0);
  dfainit(&dfa_1);
  if (match_words || match_lines)
    {
      char *n = xmalloc(size + 50);
      int i = 0;
      strcpy(n, "");
      if (match_lines)
 strcpy(n, "^\\(");
      if (match_words)
 strcpy(n, "\\(^\\|[^0-9A-Za-z_]\\)\\(");
      i = strlen(n);
      memcpy((n + i), (pattern), (size));
      i += size;
      if (match_words)
 strcpy(n + i, "\\)\\([^0-9A-Za-z_]\\|$\\)");
      if (match_lines)
 strcpy(n + i, "\\)$");
      i += strlen(n + i);
      dfacomp(n, i, &dfa_1, 1);
    }
  else
    dfacomp(pattern, size, &dfa_1, 1);
  kwsmusts();
}
static void
Ecompile(pattern, size)
     char *pattern;
     size_t size;
{
  const
  char *err;
  if (strcmp(matcher, "posix-egrep") == 0)
    {
      re_set_syntax((((((1) << 1) << 1) | ((((1) << 1) << 1) << 1) | (((((1) << 1) << 1) << 1) << 1) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) | ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)));
      dfasyntax((((((1) << 1) << 1) | ((((1) << 1) << 1) << 1) | (((((1) << 1) << 1) << 1) << 1) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)) | ((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | (((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)), match_icase);
    }
  else
    {
      re_set_syntax(((((1) << 1) << 1) | ((((1) << 1) << 1) << 1) | (((((1) << 1) << 1) << 1) << 1) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)));
      dfasyntax(((((1) << 1) << 1) | ((((1) << 1) << 1) << 1) | (((((1) << 1) << 1) << 1) << 1) | (((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) | ((((((((((((((((1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1) << 1)), match_icase);
    }
  if ((err = re_compile_pattern(pattern, size, &regex)) != 0)
    fatal(err, 0);
  dfainit(&dfa_1);
  if (match_words || match_lines)
    {
      char *n = xmalloc(size + 50);
      int i = 0;
      strcpy(n, "");
      if (match_lines)
 strcpy(n, "^(");
      if (match_words)
 strcpy(n, "(^|[^0-9A-Za-z_])(");
      i = strlen(n);
      memcpy((n + i), (pattern), (size));
      i += size;
      if (match_words)
 strcpy(n + i, ")([^0-9A-Za-z_]|$)");
      if (match_lines)
 strcpy(n + i, ")$");
      i += strlen(n + i);
      dfacomp(n, i, &dfa_1, 1);
    }
  else
    dfacomp(pattern, size, &dfa_1, 1);
  kwsmusts();
}
static char *
EGexecute(buf, size, endp)
     char *buf;
     size_t size;
     char **endp;
{
  register char *buflim, *beg, *end, save;
  int backref, start, len;
  struct kwsmatch kwsm;
  static struct re_registers regs;
  buflim = buf + size;
  for (beg = end = buf; end < buflim; beg = end + 1)
    {
      if (kwset)
 {
   beg = kwsexec(kwset, beg, buflim - beg, &kwsm);
   if (!beg)
     goto failure;
   end = memchr(beg, '\n', buflim - beg);
   if (!end)
     end = buflim;
   while (beg > buf && beg[-1] != '\n')
     --beg;
   save = *end;
   if (kwsm.strchr < lastexact)
     goto success;
   if (!dfaexec(&dfa_1, beg, end, 0, (int *) 0, &backref))
     {
       *end = save;
       continue;
     }
   *end = save;
   if (!backref)
     goto success;
 }
      else
 {
   save = *buflim;
   beg = dfaexec(&dfa_1, beg, buflim, 0, (int *) 0, &backref);
   *buflim = save;
   if (!beg)
     goto failure;
   end = memchr(beg, '\n', buflim - beg);
   if (!end)
     end = buflim;
   while (beg > buf && beg[-1] != '\n')
     --beg;
   if (!backref)
     goto success;
 }
      regex.not_eol = 0;
      if ((start = re_search(&regex, beg, end - beg, 0, end - beg, &regs)) >= 0)
 {
   len = regs.end[0] - start;
   if (!match_lines && !match_words || match_lines && len == end - beg)
     goto success;
   if (match_words)
     while (start >= 0)
       {
  if ((start == 0 || !((1 && isalnum(beg[start - 1])) || (beg[start - 1]) == '_'))
      && (len == end - beg || !((1 && isalnum(beg[start + len])) || (beg[start + len]) == '_')))
    goto success;
  if (len > 0)
    {
      --len;
      regex.not_eol = 1;
      len = re_match(&regex, beg, start + len, start, &regs);
    }
  if (len <= 0)
    {
      if (start == end - beg)
        break;
      ++start;
      regex.not_eol = 0;
      start = re_search(&regex, beg, end - beg,
          start, end - beg - start, &regs);
      len = regs.end[0] - start;
    }
       }
 }
    }
 failure:
  return 0;
 success:
  *endp = end < buflim ? end + 1 : end;
  return beg;
}
static void
Fcompile(pattern, size)
     char *pattern;
     size_t size;
{
  char *beg, *lim, *err;
  kwsinit();
  beg = pattern;
  do
    {
      for (lim = beg; lim < pattern + size && *lim != '\n'; ++lim)
 ;
      if ((err = kwsincr(kwset, beg, lim - beg)) != 0)
 fatal(err, 0);
      if (lim < pattern + size)
 ++lim;
      beg = lim;
    }
  while (beg < pattern + size);
  if ((err = kwsprep(kwset)) != 0)
    fatal(err, 0);
}
static char *
Fexecute(buf, size, endp)
     char *buf;
     size_t size;
     char **endp;
{
  register char *beg, *try, *end;
  register size_t len;
  struct kwsmatch kwsmatch;
  for (beg = buf; beg <= buf + size; ++beg)
    {
      if (!(beg = kwsexec(kwset, beg, buf + size - beg, &kwsmatch)))
 return 0;
      len = kwsmatch.size[0];
      if (match_lines)
 {
   if (beg > buf && beg[-1] != '\n')
     continue;
   if (beg + len < buf + size && beg[len] != '\n')
     continue;
   goto success;
 }
      else if (match_words)
 for (try = beg; len && try;)
   {
     if (try > buf && ((1 && isalnum((unsigned char) try[-1])) || ((unsigned char) try[-1]) == '_'))
       break;
     if (try + len < buf + size && ((1 && isalnum((unsigned char) try[len])) || ((unsigned char) try[len]) == '_'))
       {
  try = kwsexec(kwset, beg, --len, &kwsmatch);
  len = kwsmatch.size[0];
       }
     else
       goto success;
   }
      else
 goto success;
    }
  return 0;
 success:
  if ((end = memchr(beg + len, '\n', (buf + size) - (beg + len))) != 0)
    ++end;
  else
    end = buf + size;
  *endp = end;
  while (beg > buf && beg[-1] != '\n')
    --beg;
  return beg;
}
