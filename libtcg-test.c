#include <qemu/libtcg/libtcg.h>

#include <elf.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define ARRAY_LEN(arr) \
    (sizeof(arr)/sizeof(arr[0]))

static bool read_elf_text_segment(const char *elffile,
                                  uint8_t **text_buffer,
                                  size_t *text_size);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: libtcg-test elf-file-to-read\n");
        return 1;
    }

    uint8_t *buffer = NULL;
    size_t size = 0;
    if (!read_elf_text_segment(argv[1], &buffer, &size)) {
        return 1;
    }

    /*
     * Create context with default initialization
     * (using malloc/free for alloc's)
     */
    LibTinyCodeContext *context = libtcg_context_create(&(LibTinyCodeDesc){0});

    uint64_t virtual_address = 0x4000000;
    LibTinyCodeInstructionList tcgops = libtcg_translate(context,
                                                         buffer,
                                                         size,
                                                         virtual_address);

    char dump_buf[256] = {0};
    for (size_t i = 0; i < tcgops.instruction_count; ++i) {
        libtcg_dump_instruction_to_buffer(&tcgops.list[i],
                                          dump_buf,
                                          ARRAY_LEN(dump_buf));
        puts(dump_buf);
    }

    libtcg_instruction_list_destroy(context, tcgops);

    libtcg_context_destroy(context);

    free((void *) buffer);

    return 0;
}

static bool read_elf_text_segment(const char *elffile,
                                  uint8_t **text_buffer,
                                  size_t *text_size) {
    /* Start by reading entire file into a buffer */
    FILE *fd = fopen(elffile, "r");
    if (!fd) {
        fprintf(stderr, "Failed to open file %s!\n", elffile);
        return false;
    }

    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    uint8_t *buffer = malloc(size);
    size_t bytes_read = fread(buffer, 1, size, fd);
    if (bytes_read != size) {
        fprintf(stderr, "Failed to read file %s (%ld/%zu bytes read)!\n", elffile, size, bytes_read);
        fclose(fd);
        return false;
    }
    fclose(fd);

    /* Verify ELF header */
    if (buffer[EI_MAG0] != ELFMAG0 ||
        buffer[EI_MAG1] != ELFMAG1 ||
        buffer[EI_MAG2] != ELFMAG2 ||
        buffer[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "Invalid ELF header for file %s!\n", elffile);
        return false;
    }
    /* TODO(anjo): Is there some nice way to deal with both 32/64-bit ELFs? */
    if (buffer[EI_CLASS] != ELFCLASS64) {
        fprintf(stderr, "Only support 64-bit ELFs!\n");
        return false;
    }

    Elf64_Ehdr *e_hdr = (Elf64_Ehdr *) buffer;
    /* Read string table header from ELF header */
    Elf64_Shdr *str_table_hdr = (Elf64_Shdr *) (buffer + e_hdr->e_shoff + e_hdr->e_shstrndx*e_hdr->e_shentsize);
    uint8_t *str_table = buffer + str_table_hdr->sh_offset;

    /* Loop over all segment headers and look for .text */
    bool found_text = false;
    Elf64_Shdr *s_hdr;
    for (uint32_t i = 0; i < e_hdr->e_shnum; ++i) {
        s_hdr = (Elf64_Shdr *) (buffer + e_hdr->e_shoff + i*e_hdr->e_shentsize);
        if (s_hdr->sh_name && strncmp(str_table + s_hdr->sh_name, ".text", 5) == 0) {
            found_text = true;
            break;
        }
    }

    if (!found_text) {
        fprintf(stderr, "Couldn't find .text segment in elf %s\n", elffile);
        free(buffer);
        return false;
    }

    /* Copy the actual .text bytes */
    *text_size = s_hdr->sh_size;
    *text_buffer = malloc(*text_size);
    memcpy(*text_buffer, buffer + s_hdr->sh_offset, *text_size);

    free(buffer);

    return true;
}
