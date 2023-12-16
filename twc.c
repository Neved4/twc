#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L
#define STR_LENGTH 32

enum {
	TIMEFMT_ISO,
	TIMEFMT_HUMAN,
	NTIMEFMT
};

const char *time_fmts[NTIMEFMT] = {
	"%FT%T%z",
	"%F  %T  %z"
};

const char *file = "/twc/tz.conf";

static void usage(const char *s) {
	fprintf(stderr, "usage: %s [-h] [-s FORMAT] [-f FILE | -t TIMEZONE]\n", s);
	exit(EXIT_FAILURE);
}

static void errmsg(const char *msg) {
	fprintf(stderr, "error: %s\n", msg);
	exit(EXIT_FAILURE);
}

static void fparse(const char *s, const char *fpath, const char *fmt) {
	char timestr[STR_LENGTH];
	char *line = NULL, *path = NULL, *sep = NULL;
	size_t len = 0;
	FILE *fp;

	if (!fpath && !fmt) {
		char *home = getenv("XDG_CONFIG_HOME");
		home = (!home || !*home) ? getenv("HOME") : home;
		if (!home || !*home)
			errmsg("could not get the value of the $HOME variable");

		char *conf = home == getenv("HOME") ? "/.config" : "";
		size_t sizet = strlen(home) + strlen(conf) + strlen(file) + 1;
		path = calloc(sizet, sizeof(char));
		snprintf(path, sizet, "%s%s%s", home, conf, file);
		fpath = path;
	}

	time_t t;
	time(&t);

	if (fmt) {
		setenv("TZ", fmt, 1);
		strftime(timestr, sizeof(timestr), s, localtime(&t));
		printf("%s\n", timestr);
		free(path);
		return;
	}

	fp = fopen(fpath, "r");
	if (!fp) {
		line = "UTC";
		setenv("TZ", line, 1);
		strftime(timestr, sizeof(timestr), s, gmtime(&t));
		int width = strlen(line) + 2;
		printf("%-*s\n", width, timestr);
		free(path);
		return;
	}

	size_t max_width = 0;

	while (getline(&line, &len, fp) != -1) {
		if (*line == '#' || *line == '\n')
			continue;

		size_t width = strlen(line);
		if (width > max_width)
			max_width = width;
	}

	max_width++;
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
	const char *progname = basename(argv[0]),
		*timefmt = NULL, *fpath = NULL, *fmt = NULL;

	timefmt = time_fmts[TIMEFMT_ISO];

	const char *tz_env = getenv("TZ");
	if (tz_env)
		fmt = tz_env;

	while ((opt = getopt(argc, argv, "hf:s:t:")) != -1) {
		switch (opt) {
		case 'h':
			timefmt = time_fmts[TIMEFMT_HUMAN];
			break;
		case 'f':
			fpath = optarg;
			break;
		case 's':
			timefmt = optarg;
			break;
		case 't':
			fmt = optarg;
			break;
		default:
			usage(progname);
		}
	}

	fparse(timefmt, fpath, fmt);

	return(EXIT_SUCCESS);
}
