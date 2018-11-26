//
// Created by egguncle on 2018/11/26.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <unistd.h>
#include "include/bind_hook_utils.h"
#include "include/binder_hook.h"


void *get_libs_addr(pid_t pid, char *lib_name) {
    char mapsPath[32];
    long addr = 0;
    if (pid < 0) {
        sprintf(mapsPath, "/proc/self/maps");
    } else {
        sprintf(mapsPath, "/proc/%d/maps", pid);
    }
    FILE *maps = fopen(mapsPath, "r");
    char str_line[1024];
    //  printf("%s", mapsPath);
    while (!feof(maps)) {
        fgets(str_line, 1024, maps);
        if (strstr(str_line, lib_name) != NULL) {
            fclose(maps);
            addr = strtoul(strtok(str_line, "-"), NULL, 16);
            LOGI("%lx\n", addr);
            if (addr == 0x8000)
                addr = 0;
            break;
        }

    }
    fclose(maps);
    return (void *) addr;
}

void *
get_segment_base_address(int fd, void *base_addr, int phnum, size_t phsize, unsigned long phdr_addr) {
    if (phnum > 0) {
        Elf64_Phdr phdr;
        lseek(fd, phdr_addr, SEEK_SET);//将指针移至程序头表偏移地址
        for (Elf64_Half i = 0; i < phnum; i++) {
            read(fd, &phdr, phsize);
            if (phdr.p_type == PT_LOAD)
                break;
        }
        LOGD("offset %lx\n",phdr.p_offset);
        LOGD("p_vaddr %lx\n",phdr.p_vaddr);
        return base_addr + phdr.p_offset - phdr.p_vaddr;
    }
    return 0;
}
