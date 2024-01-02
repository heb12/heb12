#ifndef HEB12_CONFIG_H_
#define HEB12_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

char *heb12_config_dir(const char *, size_t, char *);
char *heb12_data_dir(const char *, size_t, char *);

#ifdef __cplusplus
}
#endif
#endif
