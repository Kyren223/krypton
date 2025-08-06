#include <stdio.h>
#include <stdlib.h>

typedef struct Pad
{
	int first;
	int second;
} Pad;

int Max(int a, int b) { return a > b ? a : b; }

char* ReadFileContents(char* path, int* len) {
	if (len != 0) *len = 0;

	FILE* file = fopen(path, "rb");
	if (!file) return 0;

	int position = ftell(file);
	fseek(file, 0, SEEK_END);
	int fileLen = ftell(file);
	fseek(file, position, SEEK_SET);

	char *contents = malloc(fileLen + 1);
	fread(contents, 1, fileLen, file);
	fclose(file);

	if (len != 0) *len = fileLen;
	contents[fileLen] = 0;

	return contents;
}

void GenFunctionPrototypes(char* srcPath, char* destPath, char* define, int append) {
	int fileLen = 0;
	char* src = ReadFileContents(srcPath, &fileLen);
	if (src == 0) return;

	FILE *file = append ? fopen(destPath, "a") : fopen(destPath, "w");
	if (!append) {
		fprintf(file, "// This header is generated, DO NOT EDIT!\n");
	}
	fprintf(file, "\n#ifdef %s", define);

	struct Keyword { char* str; int len; char end; int track; char* append; int endOffset; } keywords[] = {
		{ .str = "fn", .len = 2, .end = ')', .append = ";" },
		{ .str = "pub fn", .len = 6, .end = ')', .append = ";" },
		{ .str = "/// ---", .len = 7, .end = '\n', .append = "", .endOffset = -2 },
	};

	int padCount = 0;
	Pad pads[1024] = {0};

	int count = -1;
	while (++count < fileLen) {
		for (int i = 0; i < 3; ++i) {
			struct Keyword* kw = keywords + i;
			if (src[count] != kw->str[kw->track++]) kw->track = 0;
			if (kw->track == kw->len) {
				int copyStart = (count - kw->track);
				if (copyStart == 0 || src[copyStart - 1] == '\r' || src[copyStart - 1] == '\n') {
					if (i == 2) {
						++padCount;
					} else {
						int spaceCount = 0;
						while (spaceCount < 2) {
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
				kw->track = 0;
			}
		}
	}

	int padAt = 0;
	count = -1;
	while (++count < fileLen) {
		for (int i = 0; i < 3; ++i) {
			struct Keyword* kw = keywords + i;
			if (src[count] != kw->str[kw->track++]) kw->track = 0;
			if (kw->track == kw->len) {
				int copyStart = (count - kw->track);
				if (copyStart == 0 || src[copyStart - 1] == '\r' || src[copyStart - 1] == '\n') {
					if (i == 2) {
						fprintf(file, "\n");
						while (src[count++] != kw->end);
						fprintf(file, "%.*s%s", (count - copyStart + kw->endOffset), (src + copyStart), kw->append);
						++padAt;
					} else {
						Pad pad = pads[padAt];

						int spaceCount = 0;
						while (spaceCount < 2) {
							if (src[count] == ' ' || src[count] == '\t') ++spaceCount;
							++count;
						}
						int len = (count - copyStart);
						fprintf(file, "%.*s", len, (src + copyStart));
						int extra1 = (pad.first - len);
						for (int j = 0; j < extra1; ++j) {
							fprintf(file, " ");
						}
						copyStart += len;

						// --

						while (src[count++] != '(');
						len = (count - copyStart - 1);
						fprintf(file, "%.*s", len, (src + copyStart));
						int extra2 = (pad.second - len - 1);
						for (int j = 0; j < extra2; ++j) {
							fprintf(file, " ");
						}
						copyStart += len;

						// --

						int loop = 1;
						while (loop) {
							if (src[count] == '\n') {
								// Pad function arguments that go into new line
								++count;
								fprintf(file, "%.*s", (count - copyStart), (src + copyStart));
								len = (count - copyStart);
								copyStart += len;

								int extra = extra1 + extra2;
								while (extra-- > 0) {
									fprintf(file, " ");
								}
							} else if (src[count] == kw->end) {
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

	fprintf(file, "\n\n#endif\n");

	free(src);
	fclose(file);
}

void GenDefsPrototypes(char* srcPath, char* destPath, char* define) {
	int fileLen = 0;
	char* src = ReadFileContents(srcPath, &fileLen);
	if (src == 0) return;

	FILE* file = fopen(destPath, "w");
	fprintf(file, "// This header is generated, DO NOT EDIT!\n");
	fprintf(file, "\n#ifdef %s\n", define);

	struct Keyword { char* str; int len; int track; char* prepend; char* append; int endOffset; } keywords[] = {
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
			struct Keyword* kw = keywords + i;
			if (src[count] != kw->str[kw->track++]) kw->track = 0;
			if (kw->track == kw->len) {
				int copyStart = count;
				while (src[++count] != '\n');
				if (i <= 2) {
          // ignore existing prototypes
					if (src[copyStart - kw->len] == '\n' && src[count + 1] == '{') {
						fprintf(file, "%s%s%.*s%.*s%s\n",
              kw->prepend,
              kw->str,
              (count - copyStart + kw->endOffset), (src + copyStart),
              (count - copyStart + kw->endOffset), (src + copyStart),
              kw->append
            );
					}
				} else {
					fprintf(file, "\n%s%.*s%s\n",
            kw->str,
            (count - copyStart + kw->endOffset), (src + copyStart),
            kw->append
          );
				}
				kw->track = 0;
			}
		}
	}

	fprintf(file, "\n#endif\n");

	free(src);
	fclose(file);
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
}

