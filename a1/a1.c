#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct sectiune{
    int sect_type;
    int sect_offset;
    int sect_size;
    char sect_name[9];
}sectiune;

int verif_size(char s[]){
    int fd = -1, header_size = 0, version = 0, numar_sectiuni = 0, ok = 0;
    fd = open(s, O_RDONLY);
    char magic[2];
    magic[2] = '\0';
    read(fd, magic, 2);
    read(fd, &header_size, 2);
    read(fd, &version, 4);
    read(fd, &numar_sectiuni, 1);
    if(strcmp(magic, "QP") != 0) ok = 1;
    if(version < 112 || version > 140) ok = 1;
    if(numar_sectiuni < 3 || numar_sectiuni > 17) ok = 1;
    struct sectiune sect[numar_sectiuni];
    for(int i = 0; i < numar_sectiuni; i++){
        read(fd, sect[i].sect_name, 9);
        sect[i].sect_name[9] = '\0';
        sect[i].sect_type = 0;
        sect[i].sect_offset = 0;
        sect[i].sect_size = 0;
        read(fd, &sect[i].sect_type, 4);
        read(fd, &sect[i].sect_offset, 4);
        read(fd, &sect[i].sect_size, 4);
        if(sect[i].sect_type != 88 && sect[i].sect_type !=75 && sect[i].sect_type != 36 && sect[i].sect_type != 85)
            ok = 1;
        if(sect[i].sect_size > 1444)
            ok = 1;
        
    }
    close(fd);
    return ok;
}

void listare_rec(char path[], int smaller, char start[], int ok){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }
    else if(ok == 0){
        printf("SUCCESS\n");
        ok++;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(S_ISDIR(statbuf.st_mode) == 0){
                    if(smaller != 0 && statbuf.st_size < smaller){
                        printf("%s\n", fullPath);
                    }
                    else if(strcmp(start, "") != 0){
                        if(strstr(entry->d_name, start) != NULL)
                            if(strcmp(strstr(entry->d_name, start), entry->d_name) == 0)
                                printf("%s\n", fullPath); 
                    }
                    else if(strcmp(start, "") == 0 && smaller == 0) printf("%s\n", fullPath);
                }
                else{
                    if(strcmp(start, "") == 0 && smaller == 0) printf("%s\n", fullPath);
                    listare_rec(fullPath, smaller, start, ok);
                }
            }
        }
    }
}

void listare_nerec(char path[], int smaller, char start[]){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(smaller != 0 && statbuf.st_size < smaller){
                    printf("%s\n", fullPath);
                }
                else if(strcmp(start, "") != 0){
                    if(strstr(entry->d_name, start) != NULL)
                        if(strcmp(strstr(entry->d_name, start), entry->d_name) == 0)
                            printf("%s\n", fullPath); 
                }
                else if(strcmp(start, "") == 0 && smaller == 0) printf("%s\n", fullPath);
            }
        }
    }
}

void findall(char s[], int ok){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    dir = opendir(s);
    if(dir == NULL) {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }
    else if(ok == 0){
        printf("SUCCESS\n");
        ok++;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", s, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(S_ISDIR(statbuf.st_mode) == 0){
                    int size_ok = 0;
                    size_ok = verif_size(fullPath);
                    if(size_ok == 0) printf("%s\n", fullPath); 
                }
                else{
                    findall(fullPath, ok);
                }
            }
        }
    }
}

void parse_fct(char s[]){
    int fd = -1, ok_m = 0, header_size = 0, version = 0, numar_sectiuni = 0, ok_v = 0, ok_nr = 0, ok_tip = 0;
    fd = open(s, O_RDONLY);
    char magic[2];
    magic[2] = '\0';
    read(fd, magic, 2);
    if(strcmp(magic, "QP") != 0) ok_m = 1;
    read(fd, &header_size, 2);
    read(fd, &version, 4);
    if(version < 112 || version > 140) ok_v = 1;
    read(fd, &numar_sectiuni, 1);
    if(numar_sectiuni < 3 || numar_sectiuni > 17) ok_nr = 1;
    struct sectiune sect[numar_sectiuni];
    for(int i = 0; i < numar_sectiuni; i++){
        read(fd, sect[i].sect_name, 9);
        sect[i].sect_name[9] = '\0';
        sect[i].sect_type = 0;
        sect[i].sect_offset = 0;
        sect[i].sect_size = 0;
        read(fd, &sect[i].sect_type, 4);
        read(fd, &sect[i].sect_offset, 4);
        read(fd, &sect[i].sect_size, 4);
        if(sect[i].sect_type != 88 && sect[i].sect_type !=75 && sect[i].sect_type != 36 && sect[i].sect_type != 85)
            ok_tip = 1;
    }
    if(ok_v == 1 || ok_m == 1 || ok_nr == 1 || ok_tip == 1){
        int ok = 0;
        printf("ERROR\n");
        printf("wrong ");
        if(ok_m == 1 && ok == 0){ 
            printf("magic\n");
            ok++;
        }
        if(ok_v == 1 && ok == 0){ 
            printf("version\n");
            ok++;
        }
        if(ok_nr == 1 && ok == 0){ 
            printf("sect_nr\n");
            ok++;
        }
        if(ok_tip == 1 && ok == 0){ 
            printf("sect_types\n");
            ok = 1;
        }
    }
    else{
        printf("SUCCESS\n");
        printf("version=%d\n", version);
        printf("nr_sections=%d\n", numar_sectiuni);
        for(int i = 0; i < numar_sectiuni; i++){
            printf("section%d: %s %d %d\n", i+1, sect[i].sect_name, sect[i].sect_type, sect[i].sect_size);
        }
    }
    close(fd);
}

int main(int argc, char **argv){
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("98075\n");
        }
        else if(strcmp(argv[1], "parse") == 0){
            char s[200] = "";
            sscanf(argv[2], "path=%s", s);
            parse_fct(s);
        }
        else if(strcmp(argv[1], "findall") == 0){
            char s[200] = "";
            int ok = 0;
            sscanf(argv[2], "path=%s", s);
            findall(s, ok);
        }
        else if(strcmp(argv[1], "list") == 0){
            char path[200] = "", start[50] = "";
            int smaller = 0, recursiv = 0;
            for(int i = 2; i < argc; i++){
                if(strcmp(argv[i], "recursive") == 0) recursiv++;
                else if(strstr(argv[i], "path") != NULL) sscanf(argv[i], "path=%s", path);
                else if(strstr(argv[i], "size_smaller") != NULL) sscanf(argv[i], "size_smaller=%d", &smaller);
                else if(strstr(argv[i], "name_starts_with") != NULL) sscanf(argv[i], "name_starts_with=%s", start);
            }
            if(recursiv == 0){
                listare_nerec(path, smaller, start);
            }
            else{
                int ok = 0;
                listare_rec(path, smaller, start, ok);
            }
        }
    }
    return 0;
}