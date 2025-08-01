// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

#define StrEq(s1, s2) !strcmp(s1, s2)
#define Strlen(str) sizeof("" str)-1

typedef struct Pad {
	int first;
	int second;
} Pad;

int Max(int a, int b) { return a > b ? a : b; }

char *ReadFileContents(char *path, int *filelen) {
	if (filelen != 0) *filelen = 0;

	FILE *file = fopen(path, "rb");
	if (!file) return 0;

	int position = ftell(file);
	fseek(file, 0, SEEK_END);
	int fileLen = ftell(file);
	fseek(file, position, SEEK_SET);

	char *contents = malloc(fileLen + 1);
	fread(contents, 1, fileLen, file);
	fclose(file);

	if (filelen != 0) *filelen = fileLen;
	contents[fileLen] = 0;

	return contents;
}

void GenDefsPrototypes(char *srcPath, char *destPath, char *define)
{
	int fileLen = 0;
	char *src = ReadFileContents(srcPath, &fileLen);
	if (src == 0) return;

	FILE *file = fopen(destPath, "w");
	fprintf(file, "// This header is generated, DO NOT EDIT!\n");
	fprintf(file, "\n#ifdef %s\n", define);

	struct Keyword { char *str; int len; int track; char *prepend; char *append; int endOffset; } keywords[] = {
		{ .str = "struct ", .len = 7, .prepend = "typedef ", .append = ";", .endOffset = -1 },
		{ .str = "enum ", .len = 5, .prepend = "typedef ", .append = ";", .endOffset = -1 },
		{ .str = "union ", .len = 6, .prepend = "typedef ", .append = ";", .endOffset = -1 },
		{ .str = "/// ---", .len = 7, .prepend = "", .append = "", .endOffset = -1 },
	};

	int count = -1;
	while (++count < fileLen)
	{
		for (int i = 0; i < 4; ++i)
		{
			struct Keyword *kw = keywords + i;
			if (src[count] != kw->str[kw->track++]) kw->track = 0;
			if (kw->track == kw->len)
			{
				int copyStart = count;
				while (src[++count] != '\n');
				if (i <= 2)
				{
					if (src[copyStart - kw->len] == '\n' &&
					    src[count + 1] == '{') // ignore existing prototypes
					{
						fprintf(file, "%s%s%.*s%.*s%s\n",
						        kw->prepend,
						        kw->str,
						        (count - copyStart + kw->endOffset), (src + copyStart),
						        (count - copyStart + kw->endOffset), (src + copyStart),
						        kw->append);
					}
				}
				else
				{
					fprintf(file, "\n%s%.*s%s\n", kw->str,
					        (count - copyStart + kw->endOffset), (src + copyStart),
					        kw->append);
				}
				kw->track = 0;
			}
		}
	}

	fprintf(file, "\n#endif");

	free(src);
	fclose(file);
}

void GenFunctionPrototypes(char *srcPath, char *destPath, char *define) {
	int filelen = 0;
	char *src = ReadFileContents(srcPath, &filelen);
	if (src == 0) return;

	FILE *file = fopen(destPath, "w");
  fprintf(file, "// This header is generated, DO NOT EDIT!\n");
	fprintf(file, "\n#ifndef %s\n", define);
	fprintf(file, "#define %s\n", define);
	// fprintf(file, "\n#include \"base.h\"\n");

	struct Keyword { char *str; int len; char end; int track; char *append; int endOffset; } keywords[] = {
		{ .str = "pub fn", .len = Strlen("pub fn"), .end = ')', .append = ";" },
		{ .str = "external fn", .len = Strlen("external fn"), .end = ')', .append = ";" },
		// { .str = "/// ---", .len = Strlen("/// ---"), .end = '\n', .append = "", .endOffset = -2 },
	};
  const int keylen = sizeof(keywords)/sizeof(struct Keyword);

	int padCount = 0;
	Pad pads[1024] = {0};

	int count = -1;
	while (++count < filelen)
	{
		for (int i = 0; i < keylen; ++i)
		{
			struct Keyword *kw = keywords + i;
			if (src[count] != kw->str[kw->track++]) kw->track = 0;
      if (kw->track > 0) {
        // printf("src[count] %c | %c kw->str | kw->track\n", src[count], kw->str[kw->track], kw->track);
      }
			if (kw->track == kw->len)
			{
        if (DEBUG) {
          printf("pre-gen keyword: %s\n", kw->str);
        }
				int copyStart = (count - kw->track);
				if (copyStart == 0 || src[copyStart - 1] == '\r' || src[copyStart - 1] == '\n')
				{
					if (i == 2)
					{
						++padCount;
					}
					else
					{
						int spaceCount = 0;
						while (spaceCount < 2)
						{
							if (src[count] == ' ' || src[count] == '\t') ++spaceCount;
							++count;
						}
						int len = (count - copyStart);
						pads[padCount].first = Max(len, pads[padCount].first);

						while (src[count++] != '(');
						len = (count - copyStart - len);
						pads[padCount].second = Max(len, pads[padCount].second);
					}
					while (src[count++] != kw->end);
				}
        if (DEBUG) {
          printf("post-gen keyword: %s\n", kw->str);
        }
				kw->track = 0;
			}
		}
	}

	int padAt = 0;
	count = -1;
	while (++count < filelen)
	{
		for (int i = 0; i < keylen; ++i)
		{
			struct Keyword *kw = keywords + i;
			if (src[count] != kw->str[kw->track++]) kw->track = 0;
			if (kw->track == kw->len)
			{
				int copyStart = (count - kw->track);
				if (copyStart == 0 || src[copyStart - 1] == '\r' || src[copyStart - 1] == '\n')
				{
					if (i == 2)
					{
						fprintf(file, "\n");
						while (src[count++] != kw->end);
						fprintf(file, "%.*s%s", (count - copyStart + kw->endOffset), (src + copyStart), kw->append);
						++padAt;
					}
					else
					{
						Pad pad = pads[padAt];

						int spaceCount = 0;
						while (spaceCount < 2)
						{
							if (src[count] == ' ' || src[count] == '\t') ++spaceCount;
							++count;
						}
						int len = (count - copyStart);
						fprintf(file, "%.*s", len, (src + copyStart));
						int extra1 = (pad.first - len);
						for (int j = 0; j < extra1; ++j)
						{
							fprintf(file, " ");
						}
						copyStart += len;

						// --

						while (src[count++] != '(');
						len = (count - copyStart - 1);
						fprintf(file, "%.*s", len, (src + copyStart));
						int extra2 = (pad.second - len - 1);
						for (int j = 0; j < extra2; ++j)
						{
							fprintf(file, " ");
						}
						copyStart += len;

						// --

						int loop = 1;
						while (loop)
						{
							if (src[count] == '\n')
							{
								// Pad function arguments that go into new line
								++count;
								fprintf(file, "%.*s", (count - copyStart), (src + copyStart));
								len = (count - copyStart);
								copyStart += len;

								int extra = extra1 + extra2;
								while (extra-- > 0)
								{
									fprintf(file, " ");
								}
							}
							else if (src[count] == kw->end)
							{
								loop = 0;
							}
							++count;
						}
						fprintf(file, "%.*s%s", (count - copyStart + kw->endOffset), (src + copyStart), kw->append);
					}
				}
				kw->track = 0;
			}
		}
	}

	fprintf(file, "\n#endif");

	free(src);
	fclose(file);
}

// -1 if no extension
int ExtensionIndex(const char *path, int length) {
  for (int i = length - 1; i >= 0; i--) {
    if (path[i] == '.') {
      return i;
    }
    if (path[i] == '/') {
      return -1;
    }
  }
  return -1;
}

void GenerateHeader(char *path) {
  const char *extmeta = ".meta.h";

  int length = strlen(path);
  if (DEBUG) {
    printf("path: '%s' (%d)\n", path, length);
  }

  int index = ExtensionIndex(path, length);
  if (DEBUG) {
    printf("extension: '%s' (%d)\n", path + index, index);
  }

  if (!StrEq(path+index, ".c")) {
    return; // only metagen for .c files
  }

  int subIndex = ExtensionIndex(path, index);
  if (DEBUG) {
    printf("subext: '%s' (%d)\n", path + subIndex, subIndex);
  }
  if (subIndex != -1) {
    int matching = 1;
    for (int i = 0; i < strlen(extmeta); i++) {
      if (path[subIndex+i] != extmeta[i]) {
        matching = 0;
        break;
      }
    }
    if (matching) {
      if (DEBUG) {
        printf("skipping: '%s'\n", path);
      }
      return;
    }
  }

  // ../filename.c -> ../filename.meta.h
  char metapath[length+strlen(extmeta)];
  strcpy(metapath, path);
  for (int i = 0; i < strlen(extmeta); i++) {
    metapath[index + i] = extmeta[i];
    // printf("index: '%d' | i: %d\n", index, i);
  }
  int metalen = index + strlen(extmeta);
  metapath[metalen] = 0;

  if (DEBUG) {
    printf("metapath: '%s' (%d)\n", metapath, metalen);
    if (strlen(metapath) != metalen) {
      exit(1);
    }
  }

  // ../flename.meta.h -> FILENAME_META_H
  int lastSlash = -1;
  for (int i = 0; i < metalen; i++) {
    if (path[i] == '/') {
      lastSlash = i;
    }
  }
  int defineLen = metalen - (lastSlash + 1);
  char define[defineLen+1];
  strcpy(define, metapath+(lastSlash+1));
  define[defineLen] = 0;

  for (int i = 0; i < defineLen; i++) {
    char c = define[i];
    if ('a' <= c && c <= 'z') {
      c -= 'a' - 'A';
    } else if (c == '.') {
      c = '_';
    }
    define[i] = c;
  }

  GenFunctionPrototypes(path, metapath, define);
  GenDefsPrototypes(path, metapath, define);
  if (DEBUG) {
    printf("generated '%s'\n", metapath);
  }
}

int main() {
  const char *dirname = "../src/krypton/";

  DIR *dir = opendir(dirname);
  struct dirent *entry;

  if (!dir)
    return 1;

  while ((entry = readdir(dir))) {
    if (StrEq(entry->d_name, ".") || StrEq(entry->d_name, "..")) {
      continue;
    }

    char path[strlen(dirname) + 255];
    strcpy(path, dirname);
    strcat(path, entry->d_name);

    GenerateHeader(path);
  }

  closedir(dir);
  return 0;
}
