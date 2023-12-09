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
	fprintf(stderr, "usage: %s [-h] ...\n", s);
	exit(EXIT_FAILURE);
}

static void errmsg(const char *msg) {
	fprintf(stderr, "error: %s\n", msg);
	exit(EXIT_FAILURE);
}

static void fparse(const char *s) {
	char timestr[STR_LENGTH];
	char *line = NULL, *path = NULL, *sep = NULL;
	size_t sizet;
	FILE *fp;

	char *home = getenv("XDG_CONFIG_HOME");
	home = (!home || !*home) ? getenv("HOME") : home;
	if (!home || !*home)
		errmsg("could not get the value of the $HOME variable");

	char *conf = home == getenv("HOME") ? "/.config" : "";
	char *file = "/twc/tz.conf";
	sizet = strlen(home) + strlen(conf) + strlen(file) + 1;
	path = malloc(sizet);
	snprintf(path, sizet, "%s%s%s", home, conf, file);

	fp = fopen(path, "r");
	if (!fp)
		errmsg(strcat("configuration file not found in ", path));

	size_t len = 0;
	ssize_t read;
	time_t t;
	time(&t);

	while ((read = getline(&line, &len, fp)) != -1) {
		if (*line == '#' || *line == '\n')
			continue;

		char *nl = strchr(line, '\n');
		if (nl) *nl = '\0';

		setenv("TZ", line, 1);
		strftime(timestr, sizeof(timestr), s, localtime(&t));

		while ((sep = strchr(line, '_')))
			*sep = ' ';

		printf("%-24s%s\n", line, timestr);
	}

	free(line);
	free(path);
	fclose(fp);
}

int main(int argc, char **argv) {
	const char *progname = basename(argv[0]);
	const char *timefmt = time_fmts[TIMEFMT_ISO];
	int opt;
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
				return 1;
		}
	}

	fparse(timefmt);
	return 0;
}
