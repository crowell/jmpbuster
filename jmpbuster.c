//lolgplv3 or some shit

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define NORMAL "\033[0m"
#define BOLD           "\033[1m"
#define GREEN  "\033[92m"
#define YELLOW "\033[01;93m"

typedef unsigned short int u16;
typedef unsigned long int u32;

int main(int argc, char**argv)
{
    char* file;
    unsigned char* code;
    int i, fd, text_offset, code_size;

    u16 buff;
    u32 addr;
    Elf32_Ehdr elfhdr;
    Elf32_Phdr p_hdr;
    Elf32_Shdr s_hdr;

    printf("** JMP buster - Tool for searching registers JMP/CALL inside an executable file\n"
            "by BlackLight, released under GNU GPL license v.3, 2009\n\n");
    if(!argv[1])
    {
        printf("Usage: %s <executable_file>\n", argv[1]);
        exit(1);
    }

    file = argv[1];

    if((fd=open(file, O_RDONLY)) < 0)
    {
        error(errno, 2, argv[0]);
    }

    read(fd, &elfhdr, sizeof(Elf32_Ehdr));
    code_size = elfhdr.e_shoff;

    lseek(fd, elfhdr.e_phoff, SEEK_SET);
    addr = elfhdr.e_entry;
    text_offset = addr - 0x08048000;

    lseek(fd, text_offset, SEEK_SET);
    
    code = (char*) malloc(code_size);
    memset(code, 0x0, code_size);
    read(fd, code, code_size);

    for(i=0; i<code_size; i++, addr = elfhdr.e_entry+i)
    {
        memcpy(&buff, &(code[i]), sizeof(buff));

        switch(buff)
        {
            case 0xe0ff:
                printf("%s-> [jmp *%%eax] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe1ff:
                printf("%s-> [jmp *%%ecx] found at addr %x0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
                
            case 0xe2ff:
                printf("%s-> [jmp *%%edx] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
                
            case 0xe3ff:
                printf("%s-> [jmp *%%ebx] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
                
            case 0xe4ff:
                printf("%s-> [%sjmp *%%esp%s] found at addr %s0x%.8x%s\n", BOLD, YELLOW, NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe5ff:
                printf("%s-> [jmp *%%ebp] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xd0ff:
                printf("%s-> [call *%%eax] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xd1ff:
                printf("%s-> [call *%%ecx] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xd2ff:
                printf("%s-> [call *%%edx] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xd3ff:
                printf("%s-> [call *%%ebx] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xd4ff:
                printf("%s-> [%scall *%%esp%s] found at addr %s0x%.8x%s\n", BOLD, YELLOW, BOLD, GREEN, addr, NORMAL);
                break;

            case 0xd5ff:
                printf("%s-> [call *%%ebp] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
        }
    }
    close(fd);
    return 0;
}

