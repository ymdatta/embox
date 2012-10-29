/**
 * @file
 * @brief
 *
 * @date 03.11.10
 * @author Anton Bondarev
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <kernel/file.h>
#include <fs/file_desc.h>

#include <util/array.h>

typedef int (* device_module_init_ft)(void);
typedef struct device_module {
	const char * name;
	const struct file_operations *fops;
	const device_module_init_ft init;
} device_module_t;

extern const device_module_t __device_registry[];

#define EMBOX_DEVICE(name, file_op, init_func) \
	ARRAY_SPREAD_ADD(__device_registry, {name, file_op, init_func})

#endif /* DEVICE_H_ */
