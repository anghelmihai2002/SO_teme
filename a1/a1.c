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

void parse_fct(char s[]){
    int fd = -1, ok_m = 0, header_size = 0, version = 0, numar_sectiuni = 0, ok_v = 0, ok_nr = 0, ok_tip = 0;
    fd = open(s, O_RDONLY);
    char magic[2];
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
    }
    return 0;
}