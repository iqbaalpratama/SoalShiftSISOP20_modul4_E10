#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif


static const char *dirpath = "/home/syubban/Documents";
static const char *log_path = "/home/syubban/fs.log";
/*
    Fungsi ini akan dipanggil ketika sistem meminta SSFS
    untuk atribu-atribut dari file spesifik.  
*/

int print_info_command(char *command, char *desc){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char mains[1000] = "INFO::";
    sprintf(mains,"INFO::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n",
        tm.tm_year + 1900, tm.tm_mon + 1, 
        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, command, desc);
    printf("%s", mains);
    FILE *foutput = fopen(log_path, "a+");
    fputs(mains, foutput);
    fclose(foutput);
    return 1;
}

int print_warning_command(char *command, char *desc){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char mains[1000] = "INFO::";
    sprintf(mains,"WARNING::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n",
        tm.tm_year + 1900, tm.tm_mon + 1, 
        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, command, desc);
    printf("%s", mains);
    FILE *foutput = fopen(log_path, "a+");
    fputs(mains, foutput);
    fclose(foutput);
    return 1;
}

//info level command
static int do_getattr(const char *path, struct stat *st){
    printf("getattr\n");
    
    int res;
    char fpath[1000];
    sprintf(fpath, "%s/%s", dirpath, path);
    res = lstat(fpath, st);

    if(res == -1){
        return -errno;
    };
    return 0;
}

static int do_readdir(const char *path, void *buf, fuse_fill_dir_t filler, 
    off_t offset, struct fuse_file_info *fi){
    printf("readdir\n");
    char fpath[1000];
    if(strcmp(path, "/") == 0){
        path = dirpath;
        sprintf(fpath, "%s", path);
    }else{
        sprintf(fpath, "%s%s", dirpath, path);
    }

    int res = 0;

    DIR *dp;
    struct dirent *de;

    // (void) offset;
    // (void) fi;

    dp = opendir(fpath);
    if(dp == NULL){
        return -errno;
    }

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) {
            break;
        }
    }

    closedir(dp);
    return 0;
}

static int do_mkdir(const char *path, mode_t mode){
    printf("enter => mkdir\n");
    // printf("%s", path);
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_info_command("MKDIR", paths)){
        mkdir(fpath, 0777);
        printf("penulisan dan mkdir sukses\n");        
    }
    return 0;
}

static int do_rename(const char *from, const char *to){
    printf("enter => rename\n");
    // rename the file, directory, or other object "from" to the target "to"
    // source and target don't have to be in the same directory
    int res;
    char fpath_from[500] = "/home/syubban/Documents", fpath_to[500] = "/home/syubban/Documents";
    strcat(fpath_from, from);
    strcat(fpath_to, to);
    char joins[1000];
    sprintf(joins, "%s::%s", from, to);
    
    if(print_info_command("RENAME", joins)){
        res = rename(fpath_from, fpath_to);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan rename sukses");
        }
    }
    return 0;
}

static void *do_init(struct fuse_conn_info *conn){
    printf("enter => init");
    /*Initialize the filesystem. This function can often be left unimplemented, 
    but it can be a handy way to perform one-time setup such as allocating variable-sized 
    data structures or initializing a new filesystem. The fuse_conn_info structure gives 
    information about what features are supported by FUSE, and can be used to request 
    certain capabilities*/
    FILE *fpointer = fopen(log_path, "rb+");
    if(fpointer == NULL){
        fpointer = fopen(log_path, "wb");
        fclose(fpointer);
    }else{
        fclose(fpointer);
    }
    return 0;
}

// static int do_access(const char *path, int mode){
//     printf("enter => access\n");
//     //Same as access(2) sytstem call
// }

static int do_readlink(const char *path, char *buf, size_t size){
    printf("enter => readlink\n");
    //If path is a symbolic link, fill buf with its target, up to size
    int res;
    char fpath[1000] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_info_command("READLINK", paths)){
        res = readlink(fpath, buf, size-1);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan readlink success\n");
        }
    }
    return 0;
}

static int do_opendir(const char *path, struct fuse_file_info *fi){
    printf("enter => opendir\n");
    //open directory for reading
    return 0;
}

static int do_mknod(const char *path, mode_t mode, dev_t rdev){
    printf("enter => mknod\n");
    //make special (device) file, FIFO, or socket
    int res;
    char fpath[1000] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_info_command("MKNOD", paths)){
        if(S_ISREG(mode)){
        res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0){
            res = close(res);
        }
        }else if(S_ISFIFO(mode)){
            res = mkfifo(fpath, mode);
        }else{
            res = mknod(fpath, mode, rdev);
        }

        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan mknod success\n");
        }
    }

    return 0;
}

static int do_symlink(const char *from, const char *to){
    printf("enter => symlink");
    // create symbolic link named "from" which, when evaluated will lead to "to"
    int res;
    char fpath_from[500] = "/home/syubban/Documents", fpath_to[500] = "/home/syubban/Documents";
    strcat(fpath_from, from);
    strcat(fpath_to, to);
    char joins[1000];
    sprintf(joins, "%s::%s", from, to);
    
    if(print_info_command("SYMLINK", joins)){
        res = rename(fpath_from, fpath_to);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan symlink sukses");
        }
    }
    return 0;
}

static int do_link(const char *from, const char *to){
    printf("enter => link");
    int res;
    char fpath_from[500] = "/home/syubban/Documents", fpath_to[500] = "/home/syubban/Documents";
    strcat(fpath_from, from);
    strcat(fpath_to, to);
    char joins[1000];
    sprintf(joins, "%s::%s", from, to);
    
    if(print_info_command("LINK", joins)){
        res = rename(fpath_from, fpath_to);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan link sukses");
        }
    }
    return 0;
}

static int do_chmod(const char *path, uid_t uid, gid_t gid, mode_t mode){
    printf("enter => chmod");
    // Change the given object's owner and group to the provided values.
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("CHMOD", paths)){
        res = chmod(fpath, mode);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan chmod sukses\n");
        }        
    }
    return 0;
}

static int do_chown(const char *path, uid_t uid, gid_t gid){
    printf("enter => chown");
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("CHOWN", paths)){
        res = chown(fpath, uid, gid);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan chmown sukses\n");
        }        
    }
    return 0;
}

static int do_truncate(const char *path, off_t size){
    printf("enter => truncate");
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("TRUNCATE", paths)){
        res = truncate(fpath, size);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan truncate sukses\n");
        }        
    }
    return 0;
}

#ifdef HAVE_UTIMENSAT
static int do_utimens(const char *path, off_t size){
    printf("enter => ultimens");
    return 0;
}
#endif

static int do_open(const char *path, struct fuse_file_info* fi){
    printf("enter => open");
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("OPEN", paths)){
        res = open(fpath, fi->flags);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan open sukses\n");
        }
        close(res);        
    }
    return 0;
}

static int do_read(const char* path, char *buf, size_t size, off_t offset, 
    struct fuse_file_info* fi){
    printf("enter => read");
    int fd;
	int res;

    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);

    if(print_info_command("READ", paths)){
        (void) fi;
        fd = open(fpath, O_RDONLY);
        if (fd == -1){
            return -errno;
        }else{
            res = pread(fd, buf, size, offset);
            if (res == -1){
                res = -errno;
            }else{
                printf("penulisan dan read sukses\n");
                return res;
            }
        }
        close(fd);
    }
    return 0;
}

static int do_write(const char* path, char *buf, size_t size, off_t offset, 
    struct fuse_file_info* fi){
    printf("enter => write");
    int fd;
	int res;

    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);

    if(print_info_command("WRITE", paths)){
        (void) fi;
        fd = open(fpath, O_WRONLY);
        if (fd == -1){
            return -errno;
        }else{
            res = pwrite(fd, buf, size, offset);
            if (res == -1){
                res = -errno;
            }else{
                printf("penulisan dan write sukses\n");
                return res;
            }
        }
        close(fd);
    }
    return 0;
}

static int do_statfs(const char* path, struct statvfs* stbuf){
    printf("enter => statfs");
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("STATFS", paths)){
        res = statvfs(fpath, stbuf);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan statfs sukses\n");
        }        
    }
    return 0;
}

static int do_create(){
    printf("enter => create");
    return 0;
}

static int do_release(const char* path, struct fuse_file_info *fi){
    printf("enter => release");
    (void) path;
	(void) fi;
    return 0;
}

static int do_fsync(const char* path, int isdatasync, struct fuse_file_info* fi){
    printf("enter => fsync");
    (void) path;
	(void) isdatasync;
	(void) fi;
    return 0;
}

#ifdef HAVE_SETXATTR
static int do_setxattr(const char* path, const char* name, const char* value, size_t size, int flags){
    printf("enter => setxattr");
    return 0;
}
static int do_getxattr(const char* path, const char* name, char* value, size_t size){
    printf("enter => getxattr");
    return 0;
}

static int do_listxattr(const char* path, const char* list, size_t size){
    printf("enter => listxattr");
    return 0;
}

static int do_removexattr(){
    printf("enter => removexattr");
    return 0;
}
#endif

/*<====================================================================>*/
/*<====================================================================>*/
/*<====================================================================>*/
// Warning level command
static int do_rmdir(const char *path){
    printf("Enter => rmdir\n");
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("RMDIR", paths)){
        res = rmdir(fpath);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan rmdir sukses\n");
        }        
    }
    return 0;
}
static int do_unlink(const char *path, mode_t mode){
    printf("Enter => rmdir\n");
    int res;
    char fpath[500] = "/home/syubban/Documents";
    strcat(fpath, path);
    char paths[100];
    strcpy(paths, path);
    if(print_warning_command("UNLINK", paths)){
        res = unlink(fpath);
        if(res == -1){
            return -errno;
        }else{
            printf("penulisan dan unlink sukses\n");
        }        
    }
    return 0;
}

static struct fuse_operations do_operation = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
    .rmdir = do_rmdir,
    .unlink = do_unlink,
    .rename = do_rename,
    .mkdir = do_mkdir,
    .init = do_init,
    // .access = do_access,
    .readlink = do_readlink,
    .mknod = do_mknod,
    .symlink = do_symlink,
    .link = do_link,
    .chmod = do_chmod,
    .chown = do_chown,
    .truncate = do_truncate,
    
    #ifdef HAVE_UTIMENSAT
    .utimens = do_utimens,
    #endif
    
    .open = do_open,
    .read = do_read,
    .write = do_write,
    .statfs = do_statfs,
    .create = do_create,
    .release = do_release,
    .fsync = do_fsync,
    
    #ifdef HAVE_SETXATTR
    .setxattr = do_setxattr,
    .getxattr = do_getxattr,
    .listxattr = do_listxattr,
    .removexattr = do_removexattr,
    #endif
};


int main(int argc, char *argv[]){
    umask(0);
    return fuse_main(argc, argv, &do_operation, NULL);
}