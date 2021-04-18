#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

// the subdirectory for configuration
#define NAME "heb12"

// TODO: consider a more generic find-and-replace scheme to clean up the code
// and merging the body of heb12_config_dir and heb12_data_dir, and making the
// directories more directly configurable with macros

// Return the configuration directory with an optional subdirectory or file (can
// accept NULL or "" to ignore)
char *heb12_config_dir(const char *subdir, size_t buf_len, char *buf)
{
#ifdef HEB12_CONFIG_DIR
	if (HEB12_DATA_DIR[0] != '~') {
		strncpy(buf, HEB12_DATA_DIR, buf_len);
	} else {
		strncpy(buf, getenv("HOME"), buf_len);
		strncat(buf, HEB12_DATA_DIR, buf_len);
	}
#else
#ifdef _WIN32
	strncpy(buf, getenv("\%ProgramFiles\%"), buf_len);
	strncat(buf, "\\", buf_len);
	strncat(buf, NAME, buf_len);
#elif __unix__
	strncpy(buf, getenv("HOME"), buf_len);
	strncat(buf, "/.config/", buf_len);
	strncat(buf, NAME, buf_len);
#else
#error "Unsupported OS"
#endif
#endif
	if (subdir != NULL && strcmp(subdir, "") != 0) {
		strncat(buf, SEPARATOR, buf_len);
		strncat(buf, subdir, buf_len);
	}
	return buf;
}

// Return the data directory (for Bibles and similar) with an optional
// subdirectory or file (can accept NULL or "" to ignore)
char *heb12_data_dir(const char *subdir, size_t buf_len, char *buf)
{
#ifdef HEB12_DATA_DIR
	if (HEB12_DATA_DIR[0] != '~') {
		strncpy(buf, HEB12_DATA_DIR, buf_len);
	} else {
		strncpy(buf, getenv("HOME"), buf_len);
		strncat(buf, HEB12_DATA_DIR, buf_len);
	}
#else
#ifdef _WIN32
	// should be C:\User\Name\AppData\Roaming
	strncpy(buf, getenv("\%APPDATA\%"), buf_len);
	strncat(buf, "\\", buf_len);
	strncat(buf, NAME, buf_len);
#elif __unix__
	char *dir;
	if ((dir = getenv("XDG_DATA_HOME")) == NULL) {
		strncpy(buf, getenv("HOME"), buf_len);
		strncat(buf, "/.local/share/", buf_len);
		strncat(buf, NAME, buf_len);
	} else {
		strncpy(buf, dir, buf_len);
	};
#else
#error "Unsupported OS"
#endif
#endif
	if (subdir != NULL && strcmp(subdir, "") != 0) {
		strncat(buf, SEPARATOR, buf_len);
		strncat(buf, subdir, buf_len);
	}
	return buf;
}
