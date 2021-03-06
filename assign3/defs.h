#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <dirent.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>

typedef	int  boolean;
#define true    1
#define false   0

/*
 * Pointer to function returning boolean
 */
typedef boolean (*PFUNC)();

enum comparison_type
{
    COMP_GT,
    COMP_LT,
    COMP_EQ
};



struct size_val
{
    enum comparison_type kind;
    int blocksize;
    unsigned long size;
};

struct	predicate
{
    PFUNC pred_func;
    struct predicate *pred_next;
    union
    {
        char *str;  //fstype lname,name,path
        int time;   // atime ctime mtime
        struct size_val size;   // size
        uid_t uid;  //user
        gid_t gid;  //group
        unsigned long perm; //perm
        unsigned long type; //type
    }args;
};

struct dir{
    char *dir_name;
    struct dir *next_dir;
};


/*pred.c*/
boolean pred_name (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_type (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_size (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_atime (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_ctime (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_mtime (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_regex (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_perm (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_uid (struct predicate *pred_ptr,struct dirent *entry);
boolean pred_gid (struct predicate *pred_ptr,struct dirent *entry);

/*parser.c*/
PFUNC find_parser (char *search_name);
boolean parse_print (char *argv[], int arg_ptr);



extern struct predicate *pred_list;
extern struct predicate *last_pred;
extern int maxdepth;
extern int mindepth;
extern int curdepth;

