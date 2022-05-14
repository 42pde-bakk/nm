//
// Created by Peer De bakker on 5/13/22.
//

#ifndef NM_NM_H
#define NM_NM_H
#include <mach-o/loader.h>
#define DEFAULT_PATH "a.out"

/*
 * srcs/print_mach_header.c
 */
void	print_mach_header(const struct mach_header* hdr, int fd);
void	print_mach_header_64(const struct mach_header_64* hdr, int fd);

/*
 * srcs/load_commands.c
 */
void	print_load_commands(const struct mach_header* hdr);
void	print_load_commands_64(const struct mach_header_64* hdr);

/*
 * srcs/segments.c
 */
void	print_segment_command(const struct segment_command* segmentCommand, int fd);
void	print_segment_command_64(const struct segment_command_64* segmentCommand, int fd);

/*
 * srcs/macho.c
 */
int handle_macho(void *file);
int handle_macho_64(void *file);
#endif //NM_NM_H
