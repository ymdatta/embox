/**
 * @file
 * @brief Useful functions for manipulations with a hostent structure
 *
 * @date 20.08.12
 * @author Ilia Vaprol
 */

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <net/netdb.h>
#include "hostent_api.h"

#include <framework/mod/options.h>
#define MODOPS_MAX_NAME_LEN OPTION_GET(NUMBER, max_name_len)
#define MODOPS_MAX_ALIASES_NUM OPTION_GET(NUMBER, max_aliases_num)
#define MODOPS_MAX_ADDR_LEN OPTION_GET(NUMBER, max_addr_len)
#define MODOPS_MAX_ADDRS_NUM OPTION_GET(NUMBER, max_addrs_num)

static struct hostent hentry_storage;
static char hentry_name_storage[MODOPS_MAX_NAME_LEN];
static char *hentry_aliases[MODOPS_MAX_ALIASES_NUM + 1]; /* for null-termanated */
static size_t hentry_aliases_sz;
static char hentry_aliases_storage[MODOPS_MAX_ALIASES_NUM][MODOPS_MAX_NAME_LEN];
static char *hentry_addrs[MODOPS_MAX_ADDRS_NUM + 1]; /* for null-terminated */
static size_t hentry_addrs_sz;
static char hentry_addrs_storage[MODOPS_MAX_ADDRS_NUM][MODOPS_MAX_ADDR_LEN];

struct hostent * hostent_create(void) {
	memset(&hentry_storage, 0, sizeof hentry_storage);
	hentry_storage.h_aliases = &hentry_aliases[0];
	hentry_storage.h_addr_list = &hentry_addrs[0];
	hentry_aliases[0] = hentry_addrs[0] = NULL;
	hentry_aliases_sz = hentry_addrs_sz = 0;

	return &hentry_storage;
}

int hostent_set_name(struct hostent *he, const char *name) {
	size_t name_sz;

	assert((he != NULL) && (name != NULL));

	assert(he->h_name == NULL); /* EINVAL */

	name_sz = strlen(name) + 1;
	if (name_sz > sizeof hentry_name_storage) {
		return -ENOMEM;
	}

	memcpy(&hentry_name_storage[0], name, name_sz);

	he->h_name = &hentry_name_storage[0];

	return 0;
}

int hostent_add_alias(struct hostent *he, const char *alias) {
	size_t alias_sz;

	assert((he != NULL) && (alias != NULL));

	if (hentry_aliases_sz >= sizeof hentry_aliases_storage / sizeof hentry_aliases_storage[0]) {
		return -ENOMEM;
	}

	alias_sz = strlen(alias) + 1;
	if (alias_sz > sizeof hentry_aliases_storage[0]) {
		return -ENOMEM;
	}

	memcpy(&hentry_aliases_storage[hentry_aliases_sz][0], alias, alias_sz);

	hentry_aliases[hentry_aliases_sz] = &hentry_aliases_storage[hentry_aliases_sz][0];
	hentry_aliases[++hentry_aliases_sz] = NULL;

	return 0;
}

int hostent_set_addr_info(struct hostent *he, int addrtype, int addrlen) {
	assert(he != NULL);

	assert(he->h_addr_list != NULL); /* EINVAL */

	if ((addrlen <= 0) || (addrlen > sizeof hentry_addrs_storage[0])) {
		return -EINVAL;
	}

	he->h_addrtype = addrtype;
	he->h_length = addrlen;

	return 0;
}

int hostent_add_addr(struct hostent *he, const void *addr) {
	assert((he != NULL) && (addr != NULL));

	assert(he->h_length != 0); /* EINVAL */

	if (hentry_addrs_sz >= sizeof hentry_addrs_storage / sizeof hentry_addrs_storage[0]) {
		return -ENOMEM;
	}

	memcpy(&hentry_addrs_storage[hentry_addrs_sz][0], addr, he->h_length);

	hentry_addrs[hentry_addrs_sz] = &hentry_addrs_storage[hentry_addrs_sz][0];
	hentry_addrs[++hentry_addrs_sz] = NULL;

	return 0;
}
