#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L
#define STR_LENGTH 32

enum {
	TIMEFMT_ISO, TIMEFMT_HUMAN, NTIMEFMT
};

const char *time_fmts[NTIMEFMT] = {
	"%FT%T%z", "%F  %T  %z"
};

const char *file = "/twc/tz.conf";

static void usage(const char *s) {
	fprintf(stderr,
		"usage: %s [-h] [-f path] [-s format] [-t timezone] ...\n\n"
		"Options:\n"
		"  -f path\n"
		"        Read config from path (default \"$HOME/.config/twc/tz.conf\")\n"
		"  -h    Print in human-readable format\n"
		"  -s format\n"
		"        Set desired time format (e.g. \"%%Y-%%m-%%d\")\n"
		"  -t timezone\n"
		"        Set a specific timezone (e.g. \"Asia/Tokyo\")\n", s);

	exit(EXIT_FAILURE);
}

static void err(const char *msg) {
	fprintf(stderr, "error: %s\n", msg);
	exit(EXIT_FAILURE);
}

static void ptz(const char *tz, const char *fmt, time_t t, const char *s) {
	char timestr[STR_LENGTH];

	setenv("TZ", tz, 1);
	strftime(timestr, sizeof(timestr), fmt, localtime(&t));
	printf("%-*s%s\n", (int)strlen(tz) + 2, tz, timestr);
}

static void fparse(const char *s, char *fpath, const char *fmt) {
	char *line = NULL, *path = NULL;
	const char *env;
	size_t len = 0;
	FILE *fp;
	time_t t;
	time(&t);

	if (!fpath && !fmt) {
		if ((env = getenv("XDG_CONFIG_HOME"))) {
			asprintf(&fpath, "%s/twc", env);
		} else if ((env = getenv("HOME"))) {
			asprintf(&fpath, "%s/.config/twc", env);
		} else {
			err("could not get value of $HOME or $XDG_CONFIG_HOME");
		}
	}

	if (fmt) {
		ptz(fmt, s, t, s);
		free(path);
		return;
	}

	fp = fopen(fpath, "r");
	if (!fp) {
		ptz("UTC", s, t, s);
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

		ptz(line, s, t, s);
	}

	free(line);
	free(path);
	fclose(fp);
}

int main(int argc, char **argv) {
	int opt;
	char *fpath = NULL;
	const char *progname = basename(argv[0]),
		*timefmt = time_fmts[TIMEFMT_ISO], *fmt = NULL,
		*tz_env = getenv("TZ");

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
}
