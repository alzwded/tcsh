/* $Header: /home/hyperion/mu/christos/src/sys/tcsh-6.00/RCS/sh.h,v 3.21 1991/11/11 01:56:34 christos Exp $ */
/*
 * sh.h: Catch it all globals and includes file!
 */
/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _h_sh
#define _h_sh

/* This is separated instead of a #else of above because makedepend is
easily confused. */

#ifndef CONFIGH
# define CONFIGH "config.h"
#endif

/*
 * Avoid cpp bugs (CONFIGH is always defined at this point)
 */
#ifdef CONFIGH
# include CONFIGH
#endif

#ifndef EXTERN
# define EXTERN extern
#endif /* EXTERN */
/*
 * Sanity
 */
#if defined(_POSIX_SOURCE) && !defined(POSIX)
# define POSIX
#endif 

#if defined(POSIXJOBS) && !defined(BSDJOBS)
# define BSDJOBS
#endif 

#ifdef SHORT_STRINGS
typedef short Char;
# define SAVE(a) (Strsave(str2short(a)))
#else
typedef char Char;
# define SAVE(a) (strsave(a))
#endif 

/*
 * If your compiler complains, then you can either
 * throw it away and get gcc or, use the following define
 * and get rid of the typedef.
 * [The 4.2/3BSD vax compiler does not like that]
 */
#ifdef SIGVOID
# if (defined(vax) || defined(uts)) && !defined(__GNUC__)
#  define sigret_t void
# else
typedef void sigret_t;
# endif 
#else
typedef int sigret_t;
#endif /* SIGVOID */


/*
 * Fundamental definitions which may vary from system to system.
 *
 *	BUFSIZ		The i/o buffering size; also limits word size
 *	MAILINTVL	How often to mailcheck; more often is more expensive
 */
#ifndef BUFSIZ
#define	BUFSIZ	1024		/* default buffer size */
#endif /* BUFSIZ */

#define FORKSLEEP	10	/* delay loop on non-interactive fork failure */
#define	MAILINTVL	600	/* 10 minutes */

#ifndef INBUFSIZ
# define INBUFSIZ	1024	/* Num input characters on the command line */
#endif /* INBUFSIZ */
/*
 * The shell moves std in/out/diag and the old std input away from units
 * 0, 1, and 2 so that it is easy to set up these standards for invoked
 * commands.
 */
#define	FSHTTY	15		/* /dev/tty when manip pgrps */
#define	FSHIN	16		/* Preferred desc for shell input */
#define	FSHOUT	17		/* ... shell output */
#define	FSHDIAG	18		/* ... shell diagnostics */
#define	FOLDSTD	19		/* ... old std input */

#ifdef PROF
#define	xexit(n)	done(n)
#endif 

#ifdef cray
# define word word_t           /* sys/types.h defines word.. bad move! */
#endif

#include <sys/types.h>

#ifdef cray
# undef word
#endif 

/*
 * This macro compares the st_dev field of struct stat. On aix on ibmESA
 * st_dev is a structure, so comparison does not work. 
 */
#ifndef DEV_DEV_COMPARE
# define DEV_DEV_COMPARE(x,y)   ((x) == (y))
#endif /* DEV_DEV_COMPARE */

#ifdef _SEQUENT_
# include <sys/procstats.h>
#endif /* _SEQUENT_ */
#if defined(POSIX) || SVID > 0
# include <sys/times.h>
#endif /* POSIX || SVID > 0 */

#ifdef NLS
# include <locale.h>
#endif 

#include <sys/param.h>
#include <sys/stat.h>

#ifdef BSDTIMES
# include <sys/time.h>
# if SVID>3
#  include "/usr/ucbinclude/sys/resource.h"
# else
#  include <sys/resource.h>
# endif /* SVID>3 */
#endif /* BSDTIMES */

#ifndef POSIX
# ifdef TERMIO
#  include <termio.h>
# else /* SGTTY */
#  include <sgtty.h>
# endif /* TERMIO */
#else /* POSIX */
# include <termios.h>
# if SVID > 3
#  undef TIOCGLTC	/* we don't need those, since POSIX has them */
#  undef TIOCSLTC
#  undef CSWTCH
#  define CSWTCH _POSIX_VDISABLE	/* So job control works */
# endif /* SVID > 3 */
#endif /* POSIX */

#ifdef POSIX
/*
 * We should be using setpgid and setpgid
 * by now, but in some systems we use the
 * old routines...
 */
# define getpgrp __getpgrp
# define setpgrp __setpgrp
# include <unistd.h>
# undef getpgrp
# undef setpgrp
extern int getpgrp();
extern int setpgrp();

/*
 * the gcc+protoize version of <stdlib.h>
 * redefines malloc(), so we define the following
 * to avoid it.
 */
# define _GNU_STDLIB_H
# define malloc __malloc
# define free __free
# define calloc __calloc
# define realloc __realloc
# include <stdlib.h>
# undef malloc
# undef free
# undef calloc
# undef realloc
# include <limits.h>
#endif /* POSIX */

#if SVID > 0 || defined(_IBMR2)
# if !defined(pyr) || !defined(aiws)
#  include <time.h>
# endif /* !aiws || !pyr */
#endif /* SVID > 0 ||  _IBMR2 */

#if !(defined(sun) && defined(TERMIO))
# include <sys/ioctl.h>
#endif 

#if !defined(FIOCLEX) && defined(sun)
# include <sys/filio.h>
#endif /* !FIOCLEX && sun */


#include <sys/file.h>

#if !defined(O_RDONLY) || !defined(O_NDELAY)
# include <fcntl.h>
#endif 

#include <errno.h>

#include <setjmp.h>

#if __STDC__
# include <stdarg.h>
#else
# include <varargs.h>
#endif 

#ifdef DIRENT
# include <dirent.h>
#else
# ifdef hp9000s500
#  include <ndir.h>
# else
#  include <sys/dir.h>
# endif
# define dirent direct
#endif /* DIRENT */
#ifdef hpux
# include <stdio.h>	/* So the fgetpwent() prototypes work */
#endif 
#include <pwd.h>
#ifdef BSD
# include <strings.h>
# define strchr(a, b) index(a, b)
# define strrchr(a, b) rindex(a, b)
#else
# include <string.h>
#endif /* BSD */


/*
 * ANSIisms... These must be *after* the system include and 
 * *before* our includes, so that BSDreno has time to define __P
 */
#ifndef __P
# if __STDC__
#  define __P(a) a
# else
#  define __P(a) ()
#  define const
#  ifndef apollo
#   define volatile	/* Apollo 'c' extensions need this */
#  endif /* apollo */
# endif 
#endif 


typedef int bool;

#include "sh.types.h"

#ifndef lint
typedef ptr_t memalign_t;
#else
typedef union {
    char    am_char, *am_char_p;
    short   am_short, *am_short_p;
    int     am_int, *am_int_p;
    long    am_long, *am_long_p;
    float   am_float, *am_float_p;
    double  am_double, *am_double_p;
}      *memalign_t;

# define malloc		lint_malloc
# define free		lint_free
# define realloc	lint_realloc
# define calloc		lint_calloc
#endif 

#ifdef MDEBUG
extern memalign_t	DebugMalloc	__P((unsigned, char *, int));
extern memalign_t	DebugRealloc	__P((ptr_t, unsigned, char *, int));
extern memalign_t	DebugCalloc	__P((unsigned, unsigned, char *, int));
extern void		DebugFree	__P((ptr_t, char *, int));
# define xmalloc(i)  	DebugMalloc(i, __FILE__, __LINE__)
# define xrealloc(p, i)((p) ? DebugRealloc(p, i, __FILE__, __LINE__) : \
			      DebugMalloc(i, __FILE__, __LINE__))
# define xcalloc(n, s)	DebugCalloc(n, s, __FILE__, __LINE__)
# define xfree(p)    	if (p) DebugFree(p, __FILE__, __LINE__); else
#else
# ifdef SYSMALLOC
#  define xmalloc(i)  	Malloc(i)
#  define xrealloc(p, i)Realloc(p, i)
#  define xcalloc(n, s)	Calloc(n, s)
#  define xfree(p)    	Free(p)
# else
# define xmalloc(i)  	malloc(i)
# define xrealloc(p, i)	realloc(p, i)
# define xcalloc(n, s)	calloc(n, s)
# define xfree(p)    	free(p)
# endif /* SYSMALLOC */
#endif /* MDEBUG */
#include "sh.char.h"
#include "sh.err.h"
#include "sh.dir.h"
#include "sh.proc.h"

#include "pathnames.h"


/*
 * C shell
 *
 * Bill Joy, UC Berkeley
 * October, 1978; May 1980
 *
 * Jim Kulp, IIASA, Laxenburg Austria
 * April, 1980
 */

#define SIGN_EXTEND_CHAR(a) \
	((int) ((a) & 0x80 ? ((int) (a)) | 0xffffff00 : ((int) a) & 0x000000ff))



#if !defined(MAXNAMLEN) && defined(_D_NAME_MAX)
# define MAXNAMLEN _D_NAME_MAX
#endif /* MAXNAMLEN */

#ifndef MAXHOSTNAMELEN
# define MAXHOSTNAMELEN	255
#endif /* MAXHOSTNAMELEN */



#define	eq(a, b)	(Strcmp(a, b) == 0)

/* globone() flags */
#define G_ERROR		0	/* default action: error if multiple words */
#define G_IGNORE	1	/* ignore the rest of the words		   */
#define G_APPEND	2	/* make a sentence by cat'ing the words    */

/*
 * Global flags
 */
EXTERN bool    chkstop;		/* Warned of stopped jobs... allow exit */

#ifndef FIOCLEX
EXTERN bool    didcch;		/* Have closed unused fd's for child */
#endif 

EXTERN bool    didfds;		/* Have setup i/o fd's for child */
EXTERN bool    doneinp;		/* EOF indicator after reset from readc */
EXTERN bool    exiterr;		/* Exit if error or non-zero exit status */
EXTERN bool    child;		/* Child shell ... errors cause exit */
EXTERN bool    haderr;		/* Reset was because of an error */
EXTERN bool    intty;		/* Input is a tty */
EXTERN bool    intact;		/* We are interactive... therefore prompt */
EXTERN bool    justpr;		/* Just print because of :p hist mod */
EXTERN bool    loginsh;		/* We are a loginsh -> .login/.logout */
EXTERN bool    neednote;	/* Need to pnotify() */
EXTERN bool    noexec;		/* Don't execute, just syntax check */
EXTERN bool    pjobs;		/* want to print jobs if interrupted */
EXTERN bool    setintr;		/* Set interrupts on/off -> Wait intr... */
EXTERN bool    timflg;		/* Time the next waited for command */
EXTERN bool    havhash;		/* path hashing is available */
EXTERN bool    editing;		/* doing filename expansion and line editing */
EXTERN bool    bslash_quote;	/* PWP: tcsh-style quoting?  (in sh.c) */
EXTERN bool    isoutatty;	/* is SHOUT a tty */
EXTERN bool    isdiagatty;	/* is SHDIAG a tty */
EXTERN bool    is1atty;		/* is file descriptor 1 a tty (didfds mode) */
EXTERN bool    is2atty;		/* is file descriptor 2 a tty (didfds mode) */

/*
 * Global i/o info
 */
EXTERN Char   *arginp;		/* Argument input for sh -c and internal `xx` */
EXTERN int     onelflg;		/* 2 -> need line for -t, 1 -> exit on read */
EXTERN Char   *ffile;		/* Name of shell file for $0 */

extern char *seterr;		/* Error message from scanner/parser */
extern int errno;		/* Error from C library routines */
EXTERN Char   *shtemp;		/* Temp name for << shell files in /tmp */

#ifdef BSDTIMES
EXTERN struct timeval time0;	/* Time at which the shell started */
EXTERN struct rusage ru0;
#else
# ifdef _SEQUENT_
EXTERN timeval_t time0;		/* Time at which the shell started */
EXTERN struct process_stats ru0;
# else /* _SEQUENT_ */
#  ifndef POSIX
EXTERN time_t  time0;		/* time at which shell started */
#  else	/* POSIX */
EXTERN clock_t time0;		/* time at which shell started */
#  endif /* POSIX */
EXTERN struct tms shtimes;	/* shell and child times for process timing */
# endif /* _SEQUENT_ */
#endif /* BSDTIMES */

/*
 * Miscellany
 */
EXTERN Char   *doldol;		/* Character pid for $$ */
EXTERN int     backpid;		/* pid of the last background job */
EXTERN time_t  chktim;		/* Time mail last checked */

/*
 * Ideally these should be uid_t, gid_t, pid_t. I cannot do that right now
 * cause pid's could be unsigned and that would break our -1 flag, and 
 * uid_t and gid_t are not defined in all the systems so I would have to
 * make special cases for them. In the future...
 */
EXTERN int     uid;		/* Invokers uid */
EXTERN int     gid;		/* Invokers gid */
EXTERN int     opgrp,		/* Initial pgrp and tty pgrp */
               shpgrp,		/* Pgrp of shell */
               tpgrp;		/* Terminal process group */
				/* If tpgrp is -1, leave tty alone! */

EXTERN Char    PromptBuf[INBUFSIZ*2]; /* buffer for the actual printed prompt.
				       * this must be large enough to contain
				       * the input line and the prompt, in
				       * case a correction occured...
				       */

/*
 * To be able to redirect i/o for builtins easily, the shell moves the i/o
 * descriptors it uses away from 0,1,2.
 * Ideally these should be in units which are closed across exec's
 * (this saves work) but for version 6, this is not usually possible.
 * The desired initial values for these descriptors are defined in
 * sh.local.h.
 */
EXTERN int   SHIN;		/* Current shell input (script) */
EXTERN int   SHOUT;		/* Shell output */
EXTERN int   SHDIAG;		/* Diagnostic output... shell errs go here */
EXTERN int   OLDSTD;		/* Old standard input (def for cmds) */

/*
 * Error control
 *
 * Errors in scanning and parsing set up an error message to be printed
 * at the end and complete.  Other errors always cause a reset.
 * Because of source commands and .cshrc we need nested error catches.
 */

extern jmp_buf reslab;

/* bugfix by Jak Kirman @ Brown U.: remove the (void) cast here, see sh.c */
#define	setexit()	(setjmp(reslab))
#define	reset()		longjmp(reslab, 1)
 /* Should use structure assignment here */
#define	getexit(a)	copy((char *)(a), (char *)reslab, sizeof reslab)
#define	resexit(a)	copy((char *)reslab, ((char *)(a)), sizeof reslab)

EXTERN Char   *gointr;		/* Label for an onintr transfer */

EXTERN sigret_t (*parintr) ();	/* Parents interrupt catch */
EXTERN sigret_t (*parterm) ();	/* Parents terminate catch */

/*
 * Lexical definitions.
 *
 * All lexical space is allocated dynamically.
 * The eighth/sixteenth bit of characters is used to prevent recognition,
 * and eventually stripped.
 */
#define		META		0200
#define		ASCII		0177
#ifdef SHORT_STRINGS
# define	QUOTE 	((Char)	0100000)/* 16nth char bit used for 'ing */
# define	TRIM		0077777	/* Mask to strip quote bit */
# define	UNDER		0040000	/* Underline flag */
# define	BOLD		0020000	/* Bold flag */
# define	STANDOUT	0010000	/* Standout flag */
# define	LITERAL		0004000	/* Literal character flag */
# define	ATTRIBUTES	0074000	/* The bits used for attributes */
# define	CHAR		0000377	/* Mask to mask out the character */
#else
# define	QUOTE 	((Char)	0200)	/* Eighth char bit used for 'ing */
# define	TRIM		0177	/* Mask to strip quote bit */
# define	UNDER		0000000	/* No extra bits to do both */
# define	BOLD		0000000	/* Bold flag */
# define	STANDOUT	META	/* Standout flag */
# define	LITERAL		0000000	/* Literal character flag */
# define	ATTRIBUTES	0200	/* The bits used for attributes */
# define	CHAR		0000177	/* Mask to mask out the character */
#endif 

EXTERN int     AsciiOnly;	/* If set only 7 bits expected in characters */

/*
 * Each level of input has a buffered input structure.
 * There are one or more blocks of buffered input for each level,
 * exactly one if the input is seekable and tell is available.
 * In other cases, the shell buffers enough blocks to keep all loops
 * in the buffer.
 */
EXTERN struct Bin {
    off_t   Bfseekp;		/* Seek pointer */
    off_t   Bfbobp;		/* Seekp of beginning of buffers */
    off_t   Bfeobp;		/* Seekp of end of buffers */
    int     Bfblocks;		/* Number of buffer blocks */
    Char  **Bfbuf;		/* The array of buffer blocks */
}       B;

/*
 * This structure allows us to seek inside aliases
 */
struct Ain {
    int type;
#define I_SEEK -1		/* Invalid seek */
#define A_SEEK	0		/* Alias seek */
#define F_SEEK	1		/* File seek */
#define E_SEEK	2		/* Eval seek */
    off_t f_seek;
    Char **a_seek;
} ;

extern int aret;		/* Type of last char returned */
#define SEEKEQ(a, b) ((a)->type == (b)->type && \
		      (a)->f_seek == (b)->f_seek && \
		      (a)->a_seek == (b)->a_seek)

#define	fseekp	B.Bfseekp
#define	fbobp	B.Bfbobp
#define	feobp	B.Bfeobp
#define	fblocks	B.Bfblocks
#define	fbuf	B.Bfbuf

/*
 * The shell finds commands in loops by reseeking the input
 * For whiles, in particular, it reseeks to the beginning of the
 * line the while was on; hence the while placement restrictions.
 */
EXTERN struct Ain lineloc;

EXTERN bool    cantell;		/* Is current source tellable ? */

/*
 * Input lines are parsed into doubly linked circular
 * lists of words of the following form.
 */
struct wordent {
    Char   *word;
    struct wordent *prev;
    struct wordent *next;
};

/*
 * During word building, both in the initial lexical phase and
 * when expanding $ variable substitutions, expansion by `!' and `$'
 * must be inhibited when reading ahead in routines which are themselves
 * processing `!' and `$' expansion or after characters such as `\' or in
 * quotations.  The following flags are passed to the getC routines
 * telling them which of these substitutions are appropriate for the
 * next character to be returned.
 */
#define	DODOL	1
#define	DOEXCL	2
#define	DOALL	DODOL|DOEXCL

/*
 * Labuf implements a general buffer for lookahead during lexical operations.
 * Text which is to be placed in the input stream can be stuck here.
 * We stick parsed ahead $ constructs during initial input,
 * process id's from `$$', and modified variable values (from qualifiers
 * during expansion in sh.dol.c) here.
 */
EXTERN Char   *lap;

/*
 * Parser structure
 *
 * Each command is parsed to a tree of command structures and
 * flags are set bottom up during this process, to be propagated down
 * as needed during the semantics/exeuction pass (sh.sem.c).
 */
struct command {
    short   t_dtyp;		/* Type of node 		 */
#define	NODE_COMMAND	1	/* t_dcom <t_dlef >t_drit	 */
#define	NODE_PAREN	2	/* ( t_dspr ) <t_dlef >t_drit	 */
#define	NODE_PIPE	3	/* t_dlef | t_drit		 */
#define	NODE_LIST	4	/* t_dlef ; t_drit		 */
#define	NODE_OR		5	/* t_dlef || t_drit		 */
#define	NODE_AND	6	/* t_dlef && t_drit		 */
    short   t_dflg;		/* Flags, e.g. F_AMPERSAND|... 	 */
/* save these when re-doing 	 */
#ifndef apollo
#define	F_SAVE	(F_NICE|F_TIME|F_NOHUP)	
#else
#define	F_SAVE	(F_NICE|F_TIME|F_NOHUP|F_VER)
#endif 
#define	F_AMPERSAND	(1<<0)	/* executes in background	 */
#define	F_APPEND	(1<<1)	/* output is redirected >>	 */
#define	F_PIPEIN	(1<<2)	/* input is a pipe		 */
#define	F_PIPEOUT	(1<<3)	/* output is a pipe		 */
#define	F_NOFORK	(1<<4)	/* don't fork, last ()ized cmd	 */
#define	F_NOINTERRUPT	(1<<5)	/* should be immune from intr's */
/* spare */
#define	F_STDERR	(1<<7)	/* redirect unit 2 with unit 1	 */
#define	F_OVERWRITE	(1<<8)	/* output was !			 */
#define	F_READ		(1<<9)	/* input redirection is <<	 */
#define	F_REPEAT	(1<<10)	/* reexec aft if, repeat,...	 */
#define	F_NICE		(1<<11)	/* t_nice is meaningful 	 */
#define	F_NOHUP		(1<<12)	/* nohup this command 		 */
#define	F_TIME		(1<<13)	/* time this command 		 */
#ifdef apollo
#define F_VER		(1<<14)	/* execute command under SYSTYPE */
#endif 
    union {
	Char   *T_dlef;		/* Input redirect word 		 */
	struct command *T_dcar;	/* Left part of list/pipe 	 */
    }       L;
    union {
	Char   *T_drit;		/* Output redirect word 	 */
	struct command *T_dcdr;	/* Right part of list/pipe 	 */
    }       R;
#define	t_dlef	L.T_dlef
#define	t_dcar	L.T_dcar
#define	t_drit	R.T_drit
#define	t_dcdr	R.T_dcdr
    Char  **t_dcom;		/* Command/argument vector 	 */
    struct command *t_dspr;	/* Pointer to ()'d subtree 	 */
    short   t_nice;
#ifdef F_VER
    short   t_systype;
#endif 
};


/*
 * The keywords for the parser
 */
#define	T_BREAK		0
#define	T_BRKSW		1
#define	T_CASE		2
#define	T_DEFAULT 	3
#define	T_ELSE		4
#define	T_END		5
#define	T_ENDIF		6
#define	T_ENDSW		7
#define	T_EXIT		8
#define	T_FOREACH	9
#define	T_GOTO		10
#define	T_IF		11
#define	T_LABEL		12
#define	T_LET		13
#define	T_SET		14
#define	T_SWITCH	15
#define	T_TEST		16
#define	T_THEN		17
#define	T_WHILE		18

/*
 * These are declared here because they want to be
 * initialized in sh.init.c (to allow them to be made readonly)
 */

extern struct biltins {
    char   *bname;
#if defined(hpux) && defined(__STDC__) && !defined(__GNUC__)
    /* Avoid hpux ansi mode spurious warnings */
    void    (*bfunct) ();
#else
    void    (*bfunct) __P((Char **, struct command *));
#endif /* hpux && __STDC__ && !__GNUC__ */
    int     minargs, maxargs;
}       bfunc[];
extern int nbfunc;

extern struct srch {
    char   *s_name;
    int     s_value;
}       srchn[];
extern int nsrchn;

/*
 * Structure defining the existing while/foreach loops at this
 * source level.  Loops are implemented by seeking back in the
 * input.  For foreach (fe), the word list is attached here.
 */
EXTERN struct whyle {
    struct Ain   w_start;	/* Point to restart loop */
    struct Ain   w_end;		/* End of loop (0 if unknown) */
    Char  **w_fe, **w_fe0;	/* Current/initial wordlist for fe */
    Char   *w_fename;		/* Name for fe */
    struct whyle *w_next;	/* Next (more outer) loop */
}      *whyles;

/*
 * Variable structure
 *
 * Aliases and variables are stored in AVL balanced binary trees.
 */
EXTERN struct varent {
    Char  **vec;		/* Array of words which is the value */
    Char   *v_name;		/* Name of variable/alias */
    struct varent *v_link[3];	/* The links, see below */
    int     v_bal;		/* Balance factor */
}       shvhed, aliases;

#define v_left		v_link[0]
#define v_right		v_link[1]
#define v_parent	v_link[2]

extern struct varent *adrof1();

#define adrof(v)	adrof1(v, &shvhed)
#define value(v)	value1(v, &shvhed)

/*
 * The following are for interfacing redo substitution in
 * aliases to the lexical routines.
 */
EXTERN struct wordent *alhistp;	/* Argument list (first) */
EXTERN struct wordent *alhistt;	/* Node after last in arg list */
EXTERN Char  **alvec, *alvecp;	/* The (remnants of) alias vector */

/*
 * Filename/command name expansion variables
 */
EXTERN int   gflag;		/* After tglob -> is globbing needed? */

#define MAXVARLEN 30		/* Maximum number of char in a variable name */

#ifndef MAXPATHLEN
# define MAXPATHLEN 2048
#endif /* MAXPATHLEN */

#ifndef MAXNAMLEN
# define MAXNAMLEN 512
#endif /* MAXNAMLEN */

/*
 * Variables for filename expansion
 */
extern Char **gargv;		/* Pointer to the (stack) arglist */
extern long gargc;		/* Number args in gargv */

/*
 * Variables for command expansion.
 */
extern Char **pargv;		/* Pointer to the argv list space */
extern long pargc;		/* Count of arguments in pargv */
EXTERN Char   *pargs;		/* Pointer to start current word */
EXTERN long    pnleft;		/* Number of chars left in pargs */
EXTERN Char   *pargcp;		/* Current index into pargs */

/*
 * History list
 *
 * Each history list entry contains an embedded wordlist
 * from the scanner, a number for the event, and a reference count
 * to aid in discarding old entries.
 *
 * Essentially "invisible" entries are put on the history list
 * when history substitution includes modifiers, and thrown away
 * at the next discarding since their event numbers are very negative.
 */
EXTERN struct Hist {
    struct wordent Hlex;
    int     Hnum;
    int     Href;
    time_t  Htime;
    Char   *histline;
    struct Hist *Hnext;
}       Histlist;

EXTERN struct wordent paraml;	/* Current lexical word list */
EXTERN int     eventno;		/* Next events number */
EXTERN int     lastev;		/* Last event reference (default) */

EXTERN Char    HIST;		/* history invocation character */
EXTERN Char    HISTSUB;		/* auto-substitute character */

/*
 * To print system call errors...
 */
extern char *sys_errlist[];
extern int errno, sys_nerr;

/*
 * strings.h:
 */
#ifndef SHORT_STRINGS
#define Strchr(a, b)  		strchr(a, b)
#define Strrchr(a, b)  	strrchr(a, b)
#define Strcat(a, b)  		strcat(a, b)
#define Strncat(a, b, c) 	strncat(a, b, c)
#define Strcpy(a, b)  		strcpy(a, b)
#define Strncpy(a, b, c) 	strncpy(a, b, c)
#define Strlen(a)		strlen(a)
#define Strcmp(a, b)		strcmp(a, b)
#define Strncmp(a, b, c)	strncmp(a, b, c)

#define Strspl(a, b)		strspl(a, b)
#define Strsave(a)		strsave(a)
#define Strend(a)		strend(a)
#define Strstr(a, b)		strstr(a, b)

#define str2short(a) 		(a)
#define blk2short(a) 		saveblk(a)
#define short2blk(a) 		saveblk(a)
#define short2str(a) 		strip(a)
#else
#define Strchr(a, b)   	s_strchr(a, b)
#define Strrchr(a, b) 		s_strrchr(a, b)
#define Strcat(a, b)  		s_strcat(a, b)
#define Strncat(a, b, c) 	s_strncat(a, b, c)
#define Strcpy(a, b)  		s_strcpy(a, b)
#define Strncpy(a, b, c)	s_strncpy(a, b, c)
#define Strlen(a)		s_strlen(a)
#define Strcmp(a, b)		s_strcmp(a, b)
#define Strncmp(a, b, c)	s_strncmp(a, b, c)

#define Strspl(a, b)		s_strspl(a, b)
#define Strsave(a)		s_strsave(a)
#define Strend(a)		s_strend(a)
#define Strstr(a, b)		s_strstr(a, b)
#endif 

/*
 * setname is a macro to save space (see sh.err.c)
 */
EXTERN char   *bname;

#define	setname(a)	(bname = (a))

#ifdef VFORK
EXTERN Char   *Vsav;
EXTERN Char   *Vdp;
EXTERN Char   *Vexpath;
EXTERN char  **Vt;
#endif /* VFORK */

EXTERN Char  **evalvec;
EXTERN Char   *evalp;

extern struct mesg {
    char   *iname;		/* name from /usr/include */
    char   *pname;		/* print name */
}       mesg[];

/* word_chars is set by default to WORD_CHARS but can be overridden by
   the worchars variable--if unset, reverts to WORD_CHARS */

EXTERN Char   *word_chars;

#define WORD_CHARS "*?_-.[]~="	/* default chars besides alnums in words */

EXTERN Char   *STR_SHELLPATH;

#ifdef _PATH_BSHELL
EXTERN Char   *STR_BSHELL;
#endif 
EXTERN Char   *STR_WORD_CHARS;
EXTERN Char  **STR_environ;

#include "tc.h"
#include "sh.decls.h"

#endif /* _h_sh */
