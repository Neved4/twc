#define _POSIX_C_SOURCE 200809L
#define TIMEFMT_DEFAULT TIMEFMT_ISO
#define STR_LENGTH 32

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

enum {
	TIMEFMT_ISO,
	TIMEFMT_HUMAN,
	NTIMEFMT
};

const char *time_fmts[NTIMEFMT] = {
	"%FT%T%z",
	"%F  %T  %z"
};

static void usage(const char *s) {
	fprintf(stderr, "usage: %s [-h] ...\n", s);
	exit(EXIT_FAILURE);
}

static void err_env(void) {
	fprintf(stderr, "error: could not get the value of the $HOME variable");
	exit(EXIT_FAILURE);
}

static void err_file(const char *s) {
	fprintf(stderr, "error: configuration file not found in %s\n", s);
	exit(EXIT_FAILURE);
}

static void fparse(const char *s) {
	int hc = 0;
	char timestr[STR_LENGTH];
	char *nl = NULL;
	char *sep = NULL;
	char *line = NULL;
	char *path = NULL;
	char *conf = "/.config";
	char *home = getenv("XDG_CONFIG_HOME");
	char *file = "/twc/tz.conf";
	FILE *fp;

	if (home == NULL || *home == '\0') {
		hc = 1;
		home = getenv("HOME");

		if (home == NULL || *home == '\0') {
			err_env();
		}
	}

	size_t sizet = strlen(home) + strlen(file);
	path = (char *) malloc(sizet);
	strcpy(path, home);

	if (hc == 1) {
		sizet += strlen(conf);
		path = (char *) realloc(path, sizet);
		strcat(path, conf);
	}

	strcat(path, file);
	fp = fopen(path, "r");

	if (fp == NULL) {
		err_file(path);
	}

	size_t len = 0;
	ssize_t(read);
	time_t t;
	time(&t);

	while ((read = getline(&line, &len, fp)) != -1) {
		if (*line == '#' || *line == '\n') {
			continue;
		}

		nl = strchr(line, '\n');

		if (nl) {
			*nl = '\0';
		}

		setenv("TZ", line, 1);
		strftime(timestr, sizeof(timestr), s, localtime(&t));

		while ((sep = strchr(line, '_'))) {
			*sep = ' ';
		}

		printf("%-24s%s\n", line, timestr);
	}

	free(line);
	free(path);
	fclose(fp);
}

int main(int argc, char **argv) {
	int opt;
	const char *progname = basename(argv[0]);
	const char *timefmt = time_fmts[TIMEFMT_DEFAULT];
	opterr = 0;

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h':
				timefmt = time_fmts[TIMEFMT_HUMAN];
				break;
			case '?':
				fprintf(stderr, "%s: invalid option -%c\n", progname, optopt);
				break;
			default:
				usage(progname);
				return(1);
		}
	}

	fparse(timefmt);
	return(0);
}
