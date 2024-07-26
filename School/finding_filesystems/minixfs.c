/**
 * finding_filesystems
 * CS 341 - Fall 2023
 */
#include "minixfs.h"
#include "minixfs_utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * Virtual paths:
 *  Add your new virtual endpoint to minixfs_virtual_path_names
 */
char *minixfs_virtual_path_names[] = {"info", /* add your paths here*/};

/**
 * Forward declaring block_info_string so that we can attach unused on it
 * This prevents a compiler warning if you haven't used it yet.
 *
 * This function generates the info string that the virtual endpoint info should
 * emit when read
 */
static char *block_info_string(ssize_t num_used_blocks) __attribute__((unused));
static char *block_info_string(ssize_t num_used_blocks) {
    char *block_string = NULL;
    ssize_t curr_free_blocks = DATA_NUMBER - num_used_blocks;
    asprintf(&block_string,
             "Free blocks: %zd\n"
             "Used blocks: %zd\n",
             curr_free_blocks, num_used_blocks);
    return block_string;
}

// Don't modify this line unless you know what you're doing
int minixfs_virtual_path_count =
    sizeof(minixfs_virtual_path_names) / sizeof(minixfs_virtual_path_names[0]);

int minixfs_chmod(file_system *fs, char *path, int new_permissions) {
    // Thar she blows!
    int output = 0;
    inode * n = get_inode(fs, path);
    if (n == NULL) {
        errno = ENOENT;
        output = -1;
    } 
    else {
        uint16_t t = n->mode >> RWX_BITS_NUMBER;
        t = t << RWX_BITS_NUMBER;
        n->mode = new_permissions | t;
        clock_gettime(CLOCK_REALTIME, &(n->ctim));
    }
    return output;
}

int minixfs_chown(file_system *fs, char *path, uid_t owner, gid_t group) {
    int output = -1;
    inode *n = get_inode(fs, path);
    if (n != NULL) {
        if (n->uid - 1 != owner) {
            n->uid = owner;
        }
        if (n->gid - 1 != group) {
            n->gid = group;
        }
        clock_gettime(CLOCK_REALTIME, &n->ctim);
        output = 0;
        return output;
    }
     else if (access(path,F_OK) == -1) {
        errno = ENOENT;
    }
    return output;
}

int divor(inode* n, int lev) {
    int output = 0;
    size_t d = sizeof(data_block);
    if (lev == 0) {
        output = n->size/d;
    } else if (lev == 1) {
        output = n->size % d;
    }
    return output;
}

inode *minixfs_create_inode_for_path(file_system *fs, const char *path) {
    FILE* file = NULL;
    file = fopen(path, "a");
    if (get_inode(fs, path) || !valid_filename(path)) {
        return NULL;
    }
    const char *f;
    inode *pn = parent_directory(fs, path, &f);
    int div[2];
    div[0] = divor(pn, 0);
    div[1] = divor(pn, 1);
    printf("test 2.2 %s\n", "");
    inode_number in = first_unused_inode(fs);
    init_inode(pn, fs->inode_root + in);
    minixfs_dirent sc;
    sc.name = strdup(f);
    sc.inode_num = in;
    data_block * d_b = fs->data_root;
    if (div[0] >= NUM_DIRECT_BLOCKS) {
        data_block_number d_b_num = pn->indirect;
        d_b += d_b_num;
        size_t dbns = sizeof(data_block_number);
        div[0] -= NUM_DIRECT_BLOCKS;
        make_string_from_dirent((*(fs->data_root + (*((data_block_number *) (*d_b).data + div[0] * dbns)))).data+div[1], sc);
    } else {  
      data_block_number d_b_num = pn->direct[div[0]];
      d_b += d_b_num;
      make_string_from_dirent((*d_b).data+div[1], sc);
    }
    fclose(file);
    return fs->inode_root+in;
}

ssize_t minixfs_virtual_read(file_system *fs, const char *path, void *buf,
                             size_t count, off_t *off) {
    ssize_t output = -1;
    if (!strcmp(path, "info")) {
        // TODO implement the "info" virtual file here
        ssize_t occupied = 0;
        superblock* fm = fs->meta;
        uint16_t fmc = fm->dblock_count;
        char *map = GET_DATA_MAP(fm);
        for (uint64_t i = 0; i < fmc; i++) {
            if (map[i] == 1) {
                occupied++;
            }
        }

        char *ist = block_info_string(occupied);
        size_t len = strlen(ist);
        if ((size_t)*off > len){
            output = 0;
        } else {
            len -= *off;
            if (count > len) {
                count = len;
            }
            ist+= *off;
            memmove(buf, ist, count);
            *off = *off + count;
            output = count;
        }
    }
    errno = ENOENT;
    return output;
}

data_block_number fillnum(int idx, file_system* fs, inode* node) {
    data_block_number output = 0;
    if (idx >= NUM_DIRECT_BLOCKS) {
        size_t dbns = sizeof(data_block_number);
        idx -= NUM_DIRECT_BLOCKS;
        output = *((data_block_number *) (*(fs->data_root+node->indirect)).data+idx*dbns);
    } else {
        output = node->direct[idx];
    }
    return output;
}

int divor2(off_t* off, int lev) {
    int output = 0;
    size_t d = sizeof(data_block);
    if (lev == 0) {
        output = *off/d;
    } else if (lev == 1) {
        output = *off % d;
    }
    return output;
}

size_t fillbytes(size_t count, size_t sub, int set) {
    size_t output = 0;
    size_t cb = count - sub;
    size_t dbs = sizeof(data_block);
    dbs -= set;
    output = cb;
    if (cb > dbs) {
        output = dbs;
    }
    return output;
}

ssize_t minixfs_write(file_system *fs, const char *path, const void *buf,
                      size_t count, off_t *off) {
    // X marks the spot
    size_t dbs = sizeof(data_block);
    size_t co = count + *off;
    if (co > (NUM_DIRECT_BLOCKS+NUM_INDIRECT_BLOCKS)*dbs) {
      errno = ENOSPC;
      return -1;
    }
    inode *node = get_inode(fs, path);
    if (!node) {
      node = minixfs_create_inode_for_path(fs, path);
      if (node == NULL) {
        errno = ENOSPC;
        return -1;
      }
    }
    int bc = co / dbs;
    if (co % dbs != 0) {
        bc++;
    }
    if (minixfs_min_blockcount(fs, path, bc) == -1) {
      errno = ENOSPC;
      return -1;
    }
    int div[2];
    div[0] = divor2(off, 0);
    div[1] = divor2(off, 1);

    data_block_number num;
    size_t bytes_write = 0;
    while (bytes_write < count) {
        num = fillnum(div[0], fs, node);
        size_t ret = fillbytes(count, bytes_write, div[1]);
        memcpy((*(fs->data_root+num)).data+div[1], buf+bytes_write, ret);
        bytes_write += ret;
        div[1] = 0;
        div[0]++;
    }
    *off += count;
    node->size = *off;
    clock_gettime(CLOCK_REALTIME, &(node->mtim));
    clock_gettime(CLOCK_REALTIME, &(node->atim));
    return count;
}

ssize_t minixfs_read(file_system *fs, const char *path, void *buf, size_t count,
                     off_t *off) {
    const char *virtual_path = is_virtual_path(path);
    if (virtual_path)
        return minixfs_virtual_read(fs, virtual_path, buf, count, off);
    inode *node = get_inode(fs, path);
    if (node == NULL) {
        errno = ENOENT;
        return -1;
    }
    if (*off >= (off_t)(node->size)) {
        return 0;
    }
    int div[2];
    div[0] = divor2(off, 0);
    div[1] = divor2(off, 1);
    if (count > node->size - *off){
        count = node->size - *off;
    }
    data_block_number num;
    size_t byte_read = 0;
    while (byte_read < count) {
        num = fillnum(div[0], fs, node);
        size_t byte_to_read = fillbytes(count, *off, div[1]);
        memcpy(buf + byte_read, (*(fs->data_root+num)).data+div[1], byte_to_read);
        *off += byte_to_read;
        byte_read += byte_to_read;
        div[1] = 0;
        div[0]++;
    }
    clock_gettime(CLOCK_REALTIME, &node->atim);
    return byte_read;
}
