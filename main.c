#include <fcntl.h>
#include <libelf/libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void failure(void);

void main(int argc, char** argv) {
    Elf32_Shdr* shdr;
    Elf32_Ehdr* ehdr;
    Elf* elf;
    Elf_Scn* scn;
    Elf_Data* data;
    int fd;
    unsigned int cnt;

    /* Open the input file */
    if ((fd = open(argv[1], O_RDONLY)) == -1)
        if ((fd = fopen(argv[1], "r")) == -1)
            exit(1);

    /* Obtain the ELF descriptor */
    (void)elf_version(EV_CURRENT);
    if ((elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
        failure();

    /* Obtain the .shstrtab data buffer */
    if (((ehdr = elf32_getehdr(elf)) == NULL) ||
        ((scn = elf_getscn(elf, ehdr->e_shstrndx)) == NULL) ||
        ((data = elf_getdata(scn, NULL)) == NULL))
        failure();

    /* Traverse input filename, printing each section */
    for (cnt = 1, scn = NULL; scn = elf_nextscn(elf, scn); cnt++) {
        if ((shdr = elf32_getshdr(scn)) == NULL)
            failure();
        (void)printf("[%02d] %20s addr:0x%08x, offset:0x%08x, size:0x%08x\n",
                     cnt, (char*)data->d_buf + shdr->sh_name,
                     (uint32_t)(shdr->sh_addr), (uint32_t)(shdr->sh_offset),
                     (uint32_t)(shdr->sh_size));
        Elf_Data* d2 = elf_getdata(scn, NULL);
        if (d2 != NULL && d2->d_buf != NULL) {
            uint8_t* addr = d2->d_buf;
            printf("addr: 0x%08x\n", addr);
            for (int i = 0; i < d2->d_size; i++) {
                printf("%02x ", *addr);
                addr++;
            }
            printf("\n");
        }
    }
} /* end main */

static void failure() {
    (void)fprintf(stderr, "%s\n", elf_errmsg(elf_errno()));
    exit(1);
}
