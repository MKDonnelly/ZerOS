#pragma once

#include <lib/types.h>
#include <fs/vfs/vnode.h>

void initrd_init(size_t *start);
int initrd_len(fs_node_t *node);
int initrd_read(fs_node_t *file, int offset, int len, char *buffer);
int initrd_write(fs_node_t *file, int offset, int len, char *buffer);
int initrd_open(fs_node_t *node, int flags);
int initrd_close(fs_node_t *node);
dirent_t *initrd_readdir(fs_node_t *dir, int index);
fs_node_t *initrd_finddir(fs_node_t *dir, char *name);
