#define _POSIX_C_SOURCE 200809L
#define STR_LENGTH 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
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
	fprintf(stderr, "usage: %s [-h] [-f FILE]\n", s);
	exit(EXIT_FAILURE);
}

static void errmsg(const char *msg) {
	fprintf(stderr, "error: %s\n", msg);
	exit(EXIT_FAILURE);
}

static void fparse(const char *s, const char *fpath) {
	char timestr[STR_LENGTH];
	char *line = NULL, *path = NULL, *sep = NULL;
	size_t len = 0;
	FILE *fp;

	if (!fpath) {
		char *home = getenv("XDG_CONFIG_HOME");
		home = (!home || !*home) ? getenv("HOME") : home;
		if (!home || !*home)
			errmsg("could not get the value of the $HOME variable");

		char *conf = home == getenv("HOME") ? "/.config" : "";
		char *file = "/twc/tz.conf";
		size_t sizet = strlen(home) + strlen(conf) + strlen(file) + 1;
		char *path = malloc(sizet);
		snprintf(path, sizet, "%s%s%s", home, conf, file);
		fpath = path;
	}

	time_t t;
	time(&t);

	fp = fopen(fpath, "r");
	if (!fp) {
		line = "UTC";
		setenv("TZ", line, 1);
		strftime(timestr, sizeof(timestr), s, gmtime(&t));
		int width = strlen(line) + 2;
		printf("%-*s%s\n", width, line, timestr);
		return;
	}

	size_t max_width = 0;

	while (getline(&line, &len, fp) != -1) {
		if (*line == '#' || *line == '\n')
			continue;

		size_t width = strlen(line);
		if (width > max_width) {
			max_width = width;
		}
	}

	max_width = max_width + 1;
	rewind(fp);

	while (getline(&line, &len, fp) != -1) {
		if (*line == '#' || *line == '\n')
			continue;

		char *nl = strchr(line, '\n');
		if (nl)
			*nl = '\0';

		setenv("TZ", line, 1);
		strftime(timestr, sizeof(timestr), s, localtime(&t));

		while ((sep = strchr(line, '_')))
			*sep = ' ';

		printf("%-*s%s\n", (int)max_width, line, timestr);
	}

	free(line);
	free(path);
	fclose(fp);
}

int main(int argc, char **argv) {
	int opt;
	const char *progname = basename(argv[0]);
	const char *timefmt = time_fmts[TIMEFMT_ISO];
	const char *fpath = NULL;

	while ((opt = getopt(argc, argv, "hf:")) != -1) {
		switch (opt) {
			case 'h':
				timefmt = time_fmts[TIMEFMT_HUMAN];
				break;
			case 'f':
				fpath = optarg;
				break;
			default:
				usage(progname);
				return 1;
		}
	}

	fparse(timefmt, fpath);

	return 0;
}
