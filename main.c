#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <wordexp.h>

#ifndef EXPLETIVE
	#define EXPLETIVE "fuck"
#endif /* EXPLETIVE */
#define FUCKFILENAME "~/." EXPLETIVE "s"

enum {
	MODE_APPEND,
	MODE_RESET,
	MODE_READ
};

int fuckOpen(char **fuckdata, FILE **fuckfile, char *fuckname, int mode) {
	int rc = 0;
	size_t fucksize = 0;
	char *mode_str = NULL;
	FILE *created = NULL;

	created = fopen(fuckname, "r+");

	if (!created) {
		created = fopen(fuckname, "w");
	}

	(void)fclose(created);

	switch (mode) {
	case MODE_APPEND:
		mode_str = "a";
		break;
	case MODE_RESET:
		mode_str = "w";
		break;
	case MODE_READ:
		mode_str = "r";
		break;
	default:
		break;
	}

	*fuckfile = fopen(fuckname, mode_str);

	if (!*fuckfile) {
		printf("Could not open %s\n", fuckname);
		printf("Mode: %d\n", mode);

		rc = 1;
		goto cleanup;
	}

	if (mode == MODE_READ) {
		(void)fseek(*fuckfile, 0, SEEK_END);
		fucksize = (size_t)ftell(*fuckfile);
		(void)fseek(*fuckfile, 0, SEEK_SET);

		*fuckdata = malloc(sizeof(char) * fucksize + 1);

		if (fread(*fuckdata, 1, fucksize, *fuckfile) != fucksize) {
			printf("Could not read %s\n", fuckname);

			rc = 1;
			goto cleanup;
		}
	}

cleanup:
	return rc;
}

void fuckClose(char **fuckdata, FILE **fuckfile, char *fuckname) {
	if (*fuckdata) {
		free(*fuckdata);
	}

	if (*fuckfile) {
		if (fclose(*fuckfile) != 0) {
			printf("Could not close %s\n", fuckname);
		}
	}
}

void fuckUsage(char *fuckexec) {
	printf("Usage: %s [options]\n\n", fuckexec);
	printf("  Options:\n");
	printf("    -h, --help\n");
	printf("    -r, --reset\n");
	printf("    -c, --count\n");
	printf("    -l, --list\n");
}

int fuckAdd(FILE **fuckfile, char *fuckname) {
	int rc = 0;
	char *timestamp = NULL;

	timestamp = malloc(sizeof(char) * 11);

	sprintf(timestamp, "%ld\n", time(NULL));

	if (fwrite(timestamp, 1, 11, *fuckfile) != 11) {
		printf("Could not write to %s\n", fuckname);
		rc = 1;
	}

	return rc;
}

void fuckCount(char *fuckdata) {
	int i = 0, l = 0, count = 0;
	size_t fucklength = 0;

	fucklength = strlen(fuckdata);

	for (i = 0, l = (int)fucklength; i < l; i++) {
		if (fuckdata[i] == '\n') {
			count++;
		}
	}

	printf("You have given %d %s%s\n", count, EXPLETIVE, count != 1 ? "s" : "");
}

void capitalize(char **dest, char *src) {
	size_t length = 0;

	length = strlen(src);

	if (!*dest) {
		*dest = malloc(sizeof(char) * length + 1);
	}

	strcpy(*dest, src);
	*dest[0] = toupper(*dest[0]);
}

void fuckList(char *fuckdata) {
	int i = 0, l = 0, count = 0;
	time_t timestamp = 0;
	size_t fucklength = 0;
	char *date = NULL, *line = NULL, *expletive_uc = NULL;
	struct tm *newtime;

	date = malloc(sizeof(char) * 30);
	line = malloc(sizeof(char) * 11);

	fucklength = strlen(fuckdata);

	if (fucklength >= 11) {
		printf("\n");
	}

	capitalize(&expletive_uc, EXPLETIVE);

	for (i = 0, l = (int)fucklength; i < l; i += 11) {
		strncpy(line, fuckdata + i, 10);
		sscanf(line, "%ld", &timestamp);

		newtime = localtime(&timestamp);

		strftime(date, 30, "%a, %d %b %Y %I:%M:%S GMT", newtime);
		printf("%s given on %s\n", expletive_uc, date);
	}

	if (expletive_uc) {
		free(expletive_uc);
	}
}

int main(int argc, char *argv[]) {
	int rc = 0;
	char *fuckname = NULL, **fuckpath = NULL, *fuckdata = NULL, *fuckexec = NULL, *fuckaction = NULL;
	FILE *fuckfile = NULL;
	wordexp_t path;

	wordexp(FUCKFILENAME, &path, 0);
	fuckpath = path.we_wordv;
	fuckname = *fuckpath;

	if (argc < 2) {
		if (fuckOpen(&fuckdata, &fuckfile, fuckname, MODE_APPEND) != 0) {
			rc = 1;
			goto cleanup;
		}

		if (fuckAdd(&fuckfile, fuckname) != 0) {
			rc = 1;
		}

		goto cleanup;
	}

	fuckexec = argv[0];
	fuckaction = argv[1];

	if (strcmp(fuckaction, "-h") == 0 || strcmp(fuckaction, "--help") == 0) {
		fuckUsage(fuckexec);
	} else if (strcmp(fuckaction, "-r") == 0 || strcmp(fuckaction, "--reset") == 0) {
		if (fuckOpen(&fuckdata, &fuckfile, fuckname, MODE_RESET) != 0) {
			rc = 1;
			goto cleanup;
		}
	} else if (strcmp(fuckaction, "-c") == 0 || strcmp(fuckaction, "--count") == 0) {
		if (fuckOpen(&fuckdata, &fuckfile, fuckname, MODE_READ) != 0) {
			rc = 1;
			goto cleanup;
		}

		fuckCount(fuckdata);
	} else if (strcmp(fuckaction, "-l") == 0 || strcmp(fuckaction, "--list") == 0) {
		if (fuckOpen(&fuckdata, &fuckfile, fuckname, MODE_READ) != 0) {
			rc = 1;
			goto cleanup;
		}

		fuckCount(fuckdata);
		fuckList(fuckdata);
	} else {
		printf("What the %s does `%s` mean?\n", EXPLETIVE, fuckaction);
	}

cleanup:
	fuckClose(&fuckdata, &fuckfile, fuckname);

	return rc;
}