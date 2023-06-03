
/*
    random useful functions
*/

#include "implant.h"


#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/*
   just a little utility function to read a generic text file.

   implant_t struct is required to have debug logging. i should maybe
   find a way to make this suck less, but we're not smart devs over here
*/
int read_file(char const *path, char **buffer, implant_t *instance) {

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        DEBUG_LOG(instance->debug_enabled, "Could not open %s: %s", path, strerror(errno));
        return ERROR;
    }

     // could use fseek but stat is posix so let's use that
    struct stat st;
    if (fstat(fd, &st) < 0) {
        DEBUG_LOG(instance->debug_enabled, "Could not call fstat on %s: %s", path, strerror(errno));
        return ERROR;
    }

    unsigned long filesize = (unsigned long)st.st_size;

    *buffer = malloc(sizeof(char) * (filesize + 1));
    if (!buffer || !(*buffer)) {
        DEBUG_LOG(instance->debug_enabled, "Could not allocate memory for %s: %s", path, strerror(errno));
        return ERROR;
    }

    ssize_t bytes = read(fd, *buffer, filesize);
    if (bytes != st.st_size) {
        DEBUG_LOG(
            instance->debug_enabled,
            "[!] Should have read %ld bytes but read %ld (filepath: %s)",
            st.st_size, bytes, path
        );

        if (bytes < 0) {
            DEBUG_LOG(instance->debug_enabled, "read: %s", strerror(errno));
            return ERROR;
        }
    }

    *buffer[bytes] = 0;
    close(fd);
    return SUCCESSFUL;
}

static size_t get_nb_cols(const char *str, int n, char separator)
{
    size_t i = (size_t)n;
    size_t res = 0;

    if (str == NULL || str[0] == '\0')
        return 0;
    for (; str[i] != '\0' && str[i] != separator; i++, res++);
    return (res);
}

static size_t get_nb_rows(const char *str, char separator)
{
    int i;
    size_t res = 0;

    if (str == NULL || str[0] == '\0')
        return 0;
    if (str == NULL || str[0] == '\0')
        return (0);
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == separator)
            res++;
    return (res + 1);
}

char **tabgen(const char *str, char separator)
{
    size_t i = 0;
    size_t malloc_size = 0;
    size_t index_str = 0;
    char **res = NULL;

    if (str == NULL || *str == 0)
        return NULL;

    res = malloc(sizeof(char *) * (get_nb_rows(str, separator) + 1));
    for (; i < get_nb_rows(str, separator); i++) {
        malloc_size = get_nb_cols(&str[index_str], 0, separator);

        res[i] = malloc(sizeof(char) * (malloc_size + 4));
        res[i] = strncpy(res[i], &str[index_str], malloc_size);
        res[i][malloc_size] = '\0';

        index_str = index_str + malloc_size + 1;
    }

    res[i] = NULL;
    return (res);
}

void free_tab(uint8_t **tab)
{
    for (size_t ctr = 0; tab[ctr] != NULL; ctr++)
        free(tab[ctr]);
    free(tab);
}

char *remove_repeating_whitespaces(char *s)
{
    size_t i = 0;
    size_t x = 0 ;

    if (!s || !(*s))
        return NULL;

    for(; s[i]; ++i)
        if(!isspace(s[i]) || (i > 0 && !isspace(s[i-1])))
            s[x++] = s[i];

    s[x] = '\0';
    return s;
}

size_t get_array_size(uint8_t **array)
{
    size_t ctr = 0;

    if (!array)
        return 0;
    while (array[ctr++]);
    return ctr;
}
