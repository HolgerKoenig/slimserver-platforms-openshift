//=========================================================================
// FILENAME	: textutils.c
// DESCRIPTION	: Misc. text utilities
//=========================================================================
// Copyright (c) 2008- NETGEAR, Inc. All Rights Reserved.
//=========================================================================

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "misc.h"
#include "textutils.h"
#include "prefs.h"
#include "log.h"

static unsigned int
_char_htoi(char h)
{
  if (h<'0')
    return 0;
  if (h<='9')
    return h-'0';
  if (h<'A')
    return 0;
  if (h<='F')
    return h-'A'+10;
  if (h<'a')
    return 0;
  if (h<='f')
    return h-'a'+10;
  return 0;
}

void
urldecode(char *src)
{
  char c, *s, *d;

  for (d=s=src; *s; s++, d++) {
    c = *s;
    if (c=='%') {
      c = *++s;
      if (c=='%')
	c = '%';
      else {
	c = _char_htoi(c)<<4 | _char_htoi(*++s);
      }
      *d = c;
    }
    else {
      *d = c;
    }
  }
  *d = '\0';
}

static int
is_ignoredword(const char *str)
{
  int i;

  if (!prefs.ignoredwords)
    return 0;

  for (i=0; prefs.ignoredwords[i].n; i++) {
    if (!(strncasecmp(prefs.ignoredwords[i].word, str, prefs.ignoredwords[i].n))) {
      char next_char = str[prefs.ignoredwords[i].n];
      if (isalnum(next_char))
	continue;
      return prefs.ignoredwords[i].n;
    }
  }
  return 0;
}

char *
skipspaces(const char *str)
{
  while (isspace(*str)) str++;
  return (char*) str;
}

/*
U+0040 (40): @ A B C  D E F G  H I J K  L M N O
U+0050 (50): P Q R S  T U V W  X Y Z [  \ ] ^ _
U+0060 (60): ` a b c  d e f g  h i j k  l m n o
U+0070 (70): p q r s  t u v w  x y z {  | } ~

U+00c0 (c3 80):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+00d0 (c3 90):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+00e0 (c3 a0):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+00f0 (c3 b0):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0100 (c4 80):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0110 (c4 90):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0120 (c4 a0):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0130 (c4 b0):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0140 (c5 80):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0150 (c5 90):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0160 (c5 a0):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
U+0170 (c5 b0):  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??  ?? ?? ?? ??
 */

// conversion table for latin diacritical char to ascii one char or two chars.
unsigned short UtoAscii[] = {
  // U+00c0
  0x0041,0x0041,0x0041,0x0041, 0x0041,0x0041,0x4145,0x0043, 0x0045,0x0045,0x0045,0x0045, 0x0049,0x0049,0x0049,0x0049,
  0x0044,0x004e,0x004f,0x004f, 0x004f,0x004f,0x004f,0xc397, 0xc398,0x0055,0x0055,0x0055, 0x0055,0x0059,0x0050,0x5353,
  // U+00e0
  0x0041,0x0041,0x0041,0x0041, 0x0041,0x0041,0x4145,0x0043, 0x0045,0x0045,0x0045,0x0045, 0x0049,0x0049,0x0049,0x0049,
  0x0044,0x004e,0x004f,0x004f, 0x004f,0x004f,0x004f,0xc397, 0xc398,0x0055,0x0055,0x0055, 0x0055,0x0059,0x0050,0x5353,
  // U+0100
  0x0041,0x0041,0x0041,0x0041, 0x0041,0x0041,0x0043,0x0043, 0x0043,0x0043,0x0043,0x0043, 0x0043,0x0043,0x0044,0x0044,
  0x0044,0x0044,0x0045,0x0045, 0x0045,0x0045,0x0045,0x0045, 0x0045,0x0045,0x0045,0x0045, 0x0047,0x0047,0x0047,0x0047,
  // U+0120
  0x0047,0x0047,0x0047,0x0047, 0x0048,0x0048,0x0048,0x0048, 0x0049,0x0049,0x0049,0x0049, 0x0049,0x0049,0x0049,0x0049,
  0x0049,0x0049,0x494a,0x494a, 0x004a,0x004a,0x004b,0x004b, 0x004b,0x004c,0x004c,0x004c, 0x004c,0x004c,0x004c,0x004c,
  // U+0140
  0x004c,0x004c,0x004c,0x004e, 0x004e,0x004e,0x004e,0x004e, 0x004e,0x004e,0x004e,0x004e, 0x004f,0x004f,0x004f,0x004f,
  0x004f,0x004f,0x4f45,0x4f45, 0x0052,0x0052,0x0052,0x0052, 0x0052,0x0052,0x0053,0x0053, 0x0053,0x0053,0x0053,0x0053,
  // U+0160
  0x0053,0x0053,0x0054,0x0054, 0x0054,0x0054,0x0054,0x0054, 0x0055,0x0055,0x0055,0x0055, 0x0055,0x0055,0x0055,0x0055,
  0x0055,0x0055,0x0055,0x0055, 0x0057,0x0057,0x0059,0x0059, 0x0059,0x005a,0x005a,0x005a, 0x005a,0x005a,0x005a,0xc5bf
};

// conversion table for toupper() function for latin diacritical char
unsigned short UtoUpper[] = {
  // U+00c0
  0xc380,0xc381,0xc382,0xc383, 0xc384,0xc385,0xc386,0xc387, 0xc388,0xc389,0xc38a,0xc38b, 0xc38c,0xc38d,0xc38e,0xc38f,
  0xc390,0xc391,0xc392,0xc393, 0xc394,0xc395,0xc396,0xc397, 0xc398,0xc399,0xc39a,0xc39b, 0xc39c,0xc39d,0xc39e,0x5353,
  // U+00e0
  0xc380,0xc381,0xc382,0xc383, 0xc384,0xc385,0xc386,0xc387, 0xc388,0xc389,0xc38a,0xc38b, 0xc38c,0xc38d,0xc38e,0xc38f,
  0xc390,0xc391,0xc392,0xc393, 0xc394,0xc395,0xc396,0xc397, 0xc398,0xc399,0xc39a,0xc39b, 0xc39c,0xc39d,0xc39e,0xc39f,
  // U+0100
  0xc480,0xc480,0xc482,0xc482, 0xc484,0xc484,0xc486,0xc486, 0xc488,0xc488,0xc48a,0xc48a, 0xc48c,0xc48c,0xc48e,0xc48e,
  0xc490,0xc490,0xc492,0xc492, 0xc494,0xc494,0xc496,0xc496, 0xc498,0xc498,0xc49a,0xc49a, 0xc49c,0xc49c,0xc49e,0xc49e,
  // U+0120
  0xc4a0,0xc4a0,0xc4a2,0xc4a2, 0xc4a4,0xc4a4,0xc4a6,0xc4a6, 0xc4a8,0xc4a8,0xc4aa,0xc4aa, 0xc4ac,0xc4ac,0xc4ae,0xc4ae,
  0xc4b0,0xc4b0,0xc4b2,0xc4b2, 0xc4b4,0xc4b4,0xc4b6,0xc4b6, 0xc4b8,0xc4b9,0xc4b9,0xc4bb, 0xc4bb,0xc4bd,0xc4bd,0xc4bf,
  // U+0140
  0xc4bf,0xc581,0xc581,0xc583, 0xc583,0xc585,0xc585,0xc587, 0xc587,0xc589,0xc58a,0xc58a, 0xc58c,0xc58c,0xc58e,0xc58e,
  0xc590,0xc591,0xc592,0xc593, 0xc594,0xc595,0xc596,0xc597, 0xc598,0xc599,0xc59a,0xc59b, 0xc59c,0xc59d,0xc59e,0xc59f,
  // U+0160
  0xc5a0,0xc5a0,0xc5a2,0xc5a2, 0xc5a4,0xc5a4,0xc5a6,0xc5a6, 0xc5a8,0xc5a8,0xc5aa,0xc5aa, 0xc5ac,0xc5ac,0xc5ae,0xc5ae,
  0xc5b0,0xc5b1,0xc5b2,0xc5b3, 0xc5b4,0xc5b5,0xc5b6,0xc5b7, 0xc5b8,0xc5b9,0xc5b9,0xc5bb, 0xc5bc,0xc5bd,0xc5bd,0xc5bf,
};


char *
canonicalize_name(const char *src)
{
  unsigned char *new;
  unsigned char *s, *p;
  size_t src_len = strlen(src);
  unsigned char c, lastc = ' ';
  int n;
  int pos0;
  int converted = 0;
  const int ignoredword_only_at_1st_word = 1;

  p = new = malloc(src_len + 1);

  pos0 = 1;
  for (s = (unsigned char *) src; (c = *s); s++) {
    pos0 = 0;
    if (ispunct(c) || isspace(c)) {
      if (lastc != ' ') {
	*p++ = lastc = ' ';
      }
      converted = 1;
    }
    else if (islower(c)) {
      if ((!converted || !ignoredword_only_at_1st_word) &&
	  lastc==' ' && (n = is_ignoredword((char*)s))) {
	s += n;
      }
      else {
	*p++ = lastc = c & ~0x20;
      }
      converted = 1;
    }
    else if (!(c&0x80)) {
      if ((!converted || !ignoredword_only_at_1st_word) &&
	  lastc==' ' && (n = is_ignoredword((char*)s))) {
	s += n;
	converted = 1;
      }
      else {
	*p++ = lastc = c;
      }
    }
    else if (0xc3<=c && c<=0xc5 && s[1]>=0x80 && s[1]<=0xbf) {
      // U+0300-017f: unicode for latin diacritical chars
      int x = ((c-0xc3)<<6) | ((*++s)&0x3f);
      unsigned short u = UtoUpper[x];
      if (u & 0xff00)
	*p++ = (u>>8)&0xff;
      *p++ = lastc = u&0xff;
      converted = 1;
    }
    else if (c==0xe2 && ((s[1]==0x80 && 0x81<=s[2] && s[2]<=0xbf) ||
			 (s[1]==0x81 && 0x81<=s[2] && s[2]<=0xaf))) {
      // General Punctuation (except U+2000~U+206F) -- 3 bytes sequence
      s += 2;
      if (lastc!=' ')
	*p++ = lastc = ' ';
      converted = 1;
    }
    else if (c==0xe3 && ((s[1]==0x80 && 0x81<=s[2] && s[2]<=0xbf) ||
	     (s[1]==0x83 && s[2]==0xbb))) {
      // CJK Symbol (except U+3000) -- 3 bytes sequence
      s += 2;
      if (lastc!=' ')
	*p++ = lastc = ' ';
      converted = 1;
    }
    else if (c==0xef && ((s[1]==0xbc && 0x80<=s[2] && s[2]<=0xbf) ||
			 (s[1]==0xbd && 0x80<=s[2] && s[2]<=0x9f))) {
      // full-width -- 3 bytes sequence
      char ascii = s[1]==0xbc ? 0x20 + (s[2] & 0x3f) : 0x60 + (s[2] & 0x1f);
      if (ispunct(ascii) && ascii!='~' && ascii!='|') {
	if (lastc!=' ')
	  *p++ = lastc = ' ';
	converted = 1;
      }
      else if (islower(ascii)) {
	char xx[4];
	xx[0] = c; xx[1] = 0xbc; xx[2] = s[2] | 0x20; xx[3] = '\0';
	*p++ = c;
	*p++ = 0xbc;
	*p++ = lastc = s[2] | 0x20;
	converted = 1;
      }
      else {
	*p++ = c;
	*p++ = s[1];
	*p++ = lastc = s[2];
      }
      s += 2;
    }
    else {
      *p++ = lastc = c;
    }
  }

  if (converted) {
    if (lastc == ' ') {
      --p;
      while (p >= new && *p == ' ') --p;
      ++p;
      *p = '\0';
    }
    *p = '\0';
    return (char*) new;
  }

  free(new);
  return (char *) src;
}

int
safe_atoi(char *s)
{
  if (!s)
    return 0;
  if ((s[0]>='0' && s[0]<='9') || s[0]=='-' || s[0]=='+')
    return atoi(s);
  return 0;
}

// NOTE: support U+0000 ~ U+FFFF only.
int
utf16le_to_utf8(char *dst, int n, __u16 utf16le)
{
  __u16 wc = le16_to_cpu(utf16le);
  if (wc < 0x80) {
    if (n<1) return 0;
    *dst++ = wc & 0xff;
    return 1;
  }
  else if (wc < 0x800) {
    if (n<2) return 0;
    *dst++ = 0xc0 | (wc>>6);
    *dst++ = 0x80 | (wc & 0x3f);
    return 2;
  }
  else {
    if (n<3) return 0;
    *dst++ = 0xe0 | (wc>>12);
    *dst++ = 0x80 | ((wc>>6) & 0x3f);
    *dst++ = 0x80 | (wc & 0x3f);
    return 3;
  }
}

void
fetch_string_txt(char *fname, char *lang, int n, ...)
{
  va_list args;
  char **keys;
  char ***strs;
  char **defstr;
  int i;
  FILE *fp;
  char buf[4096];
  int state;
  char *p;
  char *langid;
  const char *lang_en = "EN";

  if (!(keys = malloc(sizeof(keys) * n))) {
    DPRINTF(E_FATAL, L_SCAN, "Out of memory\n");
  }
  if (!(strs = malloc(sizeof(strs) * n))) {
    DPRINTF(E_FATAL, L_SCAN, "Out of memory\n");
  }
  if (!(defstr = malloc(sizeof(defstr) * n))) {
    DPRINTF(E_FATAL, L_SCAN, "Out of memory\n");
  }

  va_start(args, n);
  for (i=0; i<n; i++) {
    keys[i] = va_arg(args, char *);
    strs[i] = va_arg(args, char **);
    defstr[i] = va_arg(args, char *);
  }
  va_end(args);

  if (!(fp = fopen(fname, "rb"))) {
    DPRINTF(E_ERROR, L_SCAN, "Cannot open <%s>\n", fname);
    goto _exit;
  }

  state = -1;
  while (fgets(buf, sizeof(buf), fp)) {
    int len = strlen(buf);

    if (buf[len-1]=='\n') buf[len-1] = '\0';

    if (state<0) {
      if (isalpha(buf[0])) {
	for (i=0; i<n; i++) {
	  if (!(strcmp(keys[i], buf))) {
	    state = i;
	    break;
	  }
	}
      }
    }
    else {
      int found = 0;

      if (isalpha(buf[0]) || buf[0]=='\0') {
	state = -1;
	continue;
      }

      p = buf;
      while (isspace(*p)) p++;
      if (*p == '\0') {
	state = -1;
	continue;
      }
      langid = p;
      while (!isspace(*p)) p++;
      *p++ = '\0';

      if (!strcmp(lang, langid))
	found = 1;
      else if (strcmp(lang_en, langid))
	continue;

      while (isspace(*p)) p++;
      if (*strs[state])
	free(*strs[state]);
      *strs[state] = strdup(p);

      if (found)
	state = -1;
    }
  }

  for (i=0; i<n; i++) {
    if (!*strs[i])
      *strs[i] = defstr[i];
  }

 _exit:
  free(keys);
  free(strs);
  free(defstr);
}
