#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 202405L
#define _XOPEN_SOURCE 800
#define STR_LENGTH 32

enum {
	TIMEFMT_ISO, TIMEFMT_HUMAN, NTIMEFMT
};

const char *time_fmts[NTIMEFMT] = {
	"%FT%T%z", "%F  %T  %z"
};

const char *conf = "twc/tz.conf";

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

static char *confpath(char *fpath) {
	const char *env;

	if (!fpath) {
		char *npath = NULL;

		if ((env = getenv("XDG_CONFIG_HOME"))) {
			asprintf(&npath, "%s/%s", env, conf);
		} else if ((env = getenv("HOME"))) {
			asprintf(&npath, "%s/.config/%s", env, conf);
		} else {
			err("could not get value of $HOME or $XDG_CONFIG_HOME");
		}

		return npath;
	}

	return fpath;
}

static size_t calcwidth(FILE *fp) {
	size_t max_width = 0;
	char *line = NULL;
	size_t len = 0;

	while (getline(&line, &len, fp) != -1) {
		if (*line == '#' || *line == '\n')
			continue;

		char *nl = strchr(line, '\n');

		if (nl)
			*nl = '\0';

		size_t width = strlen(line);
		if (width > max_width)
			max_width = width;
	}

	free(line);
	return max_width + 1;
}

static void tztime(const char *tz, const char *fmt, time_t t, size_t max_width) {
	char timestr[STR_LENGTH];

	setenv("TZ", tz, 1);

	if (strftime(timestr, sizeof(timestr), fmt, localtime(&t)) == 0) {
		fprintf(stderr, "error: failed to format time for timezone: %s\n", tz);
		return;
	}

	printf("%-*s %s\n", (int)max_width, tz, timestr);
}

static void parsetz(const char *fmt, const char *s, const char *fpath) {
	FILE *fp = fopen(fpath, "r");
	if (!fp) {
		time_t t;
		time(&t);
		tztime("UTC", s, t, 0);
		return;
	}

	size_t max_width = calcwidth(fp);
	rewind(fp);

	char *line = NULL;
	size_t len = 0;
	time_t t;
	time(&t);

	while (getline(&line, &len, fp) != -1) {
		if (*line == '#' || *line == '\n')
			continue;

		char *nl = strchr(line, '\n');
		if (nl) *nl = '\0';

		tztime(line, s, t, max_width);
	}

	free(line);
	fclose(fp);
}

static void fparse(const char *s, char *fpath, const char *fmt) {
	if (fmt) {
		time_t t;
		time(&t);
		tztime(fmt, s, t, 0);
		return;
	}

	fpath = confpath(fpath);
	parsetz(fmt, s, fpath);
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
