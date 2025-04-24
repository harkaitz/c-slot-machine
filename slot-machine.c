#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_STEPS 100
#define MAX_COLS  20

char const HELP[] =
    "Usage: slot-machine [OPTS...] < INPUT"                       "\n"
    ""                                                            "\n"
    "Read lines from the standard input and print a slot machine style output" "\n"
    "to the standard error. Then print the pointed word."                      "\n"
    ""                                                                 "\n"
    "  -m MS[:NUM],...  Set the delays between updates."               "\n"
    "  -s SEL_COL       Set the pointer position (default: 0)"         "\n"
    "  -c COLS          Set the column count (default: 5)"             "\n"
    "  -C               Enable color output (default: false)"          "\n"
    ""                                                                 "\n"
    "Example: slot-machine -C -s 3 < examples/words"                   "\n"
    "Example: shuf exaples/words | slot-machine -C -s 3"               "\n"
    ""                                                                 "\n"
    "Copyright (c) 2025, Harkaitz Agirre Ezama, All rights reserved."  "\n";
    
char  *lines[MAX_STEPS] = {0};
size_t linesz = 0;
size_t linepos = 0;
char const *columns[MAX_COLS] = {0};
size_t column_count = 5;
int delay_ms[MAX_STEPS] = {
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	200, 200, 200, 200, 200, 200, 200, 200,
	400, 400, 400, 400,
	800, 800,
	1200,
	0
};
size_t pointed_column = 0;
size_t column_width = 10;
bool   enable_color = false;

static char const *getword(FILE *_fp);
static void escape(FILE *_fp, char const _esc[]);
static void print_word(FILE *_fp, size_t _len, char const _str[], bool _bold, bool _underline);
static void print_words(FILE *_fp, bool _bold, bool _underline);
static int  parse_options(int _argc, char *_argv[]);

int
main(int _argc, char *_argv[])
{
	if (_argc > 1 && (!strcmp(_argv[1], "-h") || !strcmp(_argv[1], "--help"))) {
		fputs(HELP, stdout);
		return 0;
	}

	if (parse_options(_argc, _argv)) {
		return 1;
	}

	for (size_t i=0; i<column_count; i++) {
		print_word(stderr, column_width, (i==pointed_column)?"V":"", true, false);
	}
	fprintf(stderr, "\n");
	for (size_t d=0; delay_ms[d]; d++) {
		for (size_t i=(column_count-1); i>0; i--) {
			columns[i] = columns[i-1];
		}
		columns[0] = getword(stdin);
		print_words(stderr, true, false);
		usleep(delay_ms[d]*1000);
	}

	print_words(stderr, true, true);
	fprintf(stderr, "\n");
	if (columns[pointed_column]) {
		printf("%s\n", columns[pointed_column]);
	}
	return 0;
}

static int
parse_options(int _argc, char *_argv[])
{
	char *a,*ar,*br,*num,*ms; int opt, i_ms, i_num, delay_count = 0;

	while((opt = getopt (_argc, _argv, "m:s:c:C")) != -1) {
		switch (opt) {
		case 'm':
			delay_count = 0;
			for (a = strtok_r(optarg, ",", &ar); a; a = strtok_r(NULL, ",", &ar)) {
				ms  = strtok_r(a, ":", &br);
				if (!ms) continue;
				num = strtok_r(NULL, ":", &br);
				num = (num)?num:"1";
				i_ms = atoi(ms);
				i_num = atoi(num);
				while (i_num-->0) {
					if (delay_count == MAX_STEPS) {
						fprintf(stderr, "error: Too much delays.\n");
						return 1;
					}
					delay_ms[delay_count++] = i_ms;
				}
			}
			delay_ms[delay_count] = 0;
			break;
		case 's':
			pointed_column = atoi(optarg)-1;
			break;
		case 'c':
			column_count = atoi(optarg);
			break;
		case 'C':
			enable_color = true;
			break;
		case '?':
		default:
			return 1;
		}
	}
	if (column_count>=MAX_COLS || column_count < 0) {
		fprintf(stderr, "error: -c: Invalid column number, max=%i\n", (int)column_count);
		return 1;
	}
	if (pointed_column>=column_count || pointed_column < 0) {
		fprintf(stderr, "error: -s: Invalid column number\n");
		return 1;
	}
	return 0;
}

static char const *
getword(FILE *_fp)
{
	char *line = NULL, *ch;
	if (linesz < MAX_STEPS) {
		line = malloc(1024);
		if (fgets(line, 1023, _fp)) {
			line[1023] = '\0';
			if ((ch = strchr(line, '\n'))) {
				*ch = '\0';
			}
			line = realloc(line, strlen(line)+1);
			lines[linesz++] = line;
		}
	}
	line = lines[linepos%linesz];
	linepos = (linepos + 1) % linesz;
	return line;
}

static void
escape(FILE *_fp, char const _esc[])
{
	if (enable_color) {
		fprintf(stderr, _esc);
	}
}

static void
print_word(FILE *_fp, size_t _len, char const _str[], bool _bold, bool _underline)
{
	size_t l = (_str)?strlen(_str):0;
	if (_bold) escape(stderr, "\033[1m");
	if (_underline) escape(stderr, "\033[4m");
	for (size_t i=0; i<_len; i++) {
		if (i<l) {
			fputc(_str[i], _fp);
		} else if (i == l) {
			escape(stderr, "\033[0m");
			fputc(' ', _fp);
		} else {
			fputc(' ', _fp);
		}
	}
	escape(stderr, "\033[0m");
	fputc(' ', _fp);
}

static void
print_words(FILE *_fp, bool _bold, bool _underline)
{
	fprintf(_fp, "\r");
	for (size_t i=0; i<column_count; i++) {
		print_word(
		    stderr,
		    column_width,
		    columns[i],
		    (i==pointed_column) && _bold,
		    (i==pointed_column) && _underline
		);
	}
}
