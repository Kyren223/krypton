#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  const char *keyword;
} Keyword;

static Keyword func_keywords[] = {
  {"fn"},
  {NULL}
};

static Keyword type_keywords[] = {
  {"struct"},
  {"enum"},
  {"union"},
  {NULL}
};

static int starts_with(const char *s, const char *prefix) {
  while (*prefix) {
    if (*s++ != *prefix++) return 0;
  }
  return 1;
}

static void trim(char *s) {
  char *p = s;
  while (isspace((unsigned char)*p)) p++;
  if (p != s) memmove(s, p, strlen(p) + 1);
  for (char *end = s + strlen(s) - 1; end >= s && isspace((unsigned char)*end); *end-- = '\0');
}

static int match_keyword(const char *line, Keyword *kwlist) {
  for (int i = 0; kwlist[i].keyword; i++) {
    if (starts_with(line, kwlist[i].keyword) &&
      isspace((unsigned char)line[strlen(kwlist[i].keyword)])) {
      return 1;
    }
  }
  return 0;
}

static void GenDefsPrototypes(const char *infile, const char *outfile, const char *guard) {
  FILE *in = fopen(infile, "r");
  if (!in) { perror(infile); exit(1); }
  FILE *out = fopen(outfile, "w");
  if (!out) { perror(outfile); exit(1); }

  fprintf(out, "// This header is generated, DO NOT EDIT!\n\n");
  fprintf(out, "#ifdef %s\n\n", guard);

  char buf[4096];
  int last_was_decl = 0;
  while (fgets(buf, sizeof(buf), in)) {
    trim(buf);

    if (starts_with(buf, "/// ---")) {
      if (last_was_decl) fprintf(out, "\n");
      fprintf(out, "%s\n\n", buf);
      last_was_decl = 0;
      continue;
    }

    if (match_keyword(buf, type_keywords)) {
      char type[256];
      if (sscanf(buf, "%*s %255s", type) == 1) {
        char *brace = strchr(type, '{');
        if (brace) *brace = '\0';
        fprintf(out, "typedef %s %s %s;\n", strtok(buf, " "), type, type);
        last_was_decl = 1;
      }
    }
  }

  if (last_was_decl) {
    fprintf(out, "\n");
  }
  fprintf(out, "#endif\n");

  fclose(in);
  fclose(out);
}

static void GenFunctionPrototypes(const char *infile, const char *outfile, const char *guard, int append) {
  FILE *in = fopen(infile, "r");
  if (!in) { perror(infile); exit(1); }
  FILE *out = fopen(outfile, append ? "a" : "w");
  if (!out) { perror(outfile); exit(1); }

  fprintf(out, "\n#ifdef %s\n\n", guard);

  char buf[4096];
  int last_was_decl = 0;
  while (fgets(buf, sizeof(buf), in)) {
    trim(buf);

    if (starts_with(buf, "/// ---")) {
      if (last_was_decl) fprintf(out, "\n");
      fprintf(out, "%s\n\n", buf);
      last_was_decl = 0;
      continue;
    }

    if (match_keyword(buf, func_keywords)) {
      char funcbuf[8192];
      strcpy(funcbuf, buf);
      while (!strchr(funcbuf, ')') && !strchr(funcbuf, '{')) {
        if (!fgets(buf, sizeof(buf), in)) break;
        strcat(funcbuf, buf);
      }
      char *brace = strchr(funcbuf, '{');
      if (brace) *brace = '\0';
      trim(funcbuf);
      fprintf(out, "%s;\n", funcbuf);
      last_was_decl = 1;
    }
  }

  if (last_was_decl) {
    fprintf(out, "\n");
  }
  fprintf(out, "#endif\n");

  fclose(in);
  fclose(out);
}

int main(void)
{
	// Base
	GenDefsPrototypes("../src/krypton/base.h", "../src/krypton/generated/base.meta.h", "BASE_DEFS");
	GenFunctionPrototypes("../src/krypton/base.c", "../src/krypton/generated/base.meta.h", "BASE_FUNCTIONS", 1);

  // Main
	GenDefsPrototypes("../src/krypton/krypton_main.h", "../src/krypton/generated/krypton_main.meta.h", "KRYPTON_MAIN_DEFS");
	GenFunctionPrototypes("../src/krypton/krypton_main.c", "../src/krypton/generated/krypton_main.meta.h", "KRYPTON_MAIN_FUNCTIONS", 1);

	// Platform Linux
	GenDefsPrototypes("../src/krypton/platform_linux.h", "../src/krypton/generated/platform_linux.meta.h", "PLATFORM_LINUX_DEFS");
	GenFunctionPrototypes("../src/krypton/platform_linux.c", "../src/krypton/generated/platform_linux.meta.h", "PLATFORM_LINUX_FUNCTIONS", 1);

  // libkrypton
	GenDefsPrototypes("../src/krypton/libkrypton.h", "../src/krypton/generated/libkrypton.meta.h", "LIBKRYPTON_DEFS");
	GenFunctionPrototypes("../src/krypton/libkrypton.c", "../src/krypton/generated/libkrypton.meta.h", "LIBKRYPTON_FUNCTIONS", 1);
}

