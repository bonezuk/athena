#pragma once

#define MAX_FRAME_SIZE 4352
#define DEMUX_BUFFER_SIZE (65536 - MAX_FRAME_SIZE) // need some space as sand box

typedef struct mpc_demux_t {
    mpc_reader * r;
    mpc_decoder * d;
    mpc_streaminfo si;

    // buffer
    mpc_uint8_t buffer[DEMUX_BUFFER_SIZE + MAX_FRAME_SIZE];
    mpc_size_t bytes_total;
    mpc_bits_reader bits_reader;
    mpc_int32_t block_bits; /// bits remaining in current audio block
    mpc_uint_t block_frames; /// frames remaining in current audio block

    // seeking
    mpc_seek_t * seek_table;
    mpc_uint_t seek_pwr; /// distance between 2 frames in seek_table = 2^seek_pwr
    mpc_uint32_t seek_table_size; /// used size in seek_table

    // chapters
    mpc_seek_t chap_pos; /// supposed position of the first chapter block
    mpc_int_t chap_nb; /// number of chapters (-1 if unknown, 0 if no chapter)
    mpc_chap_info * chap; /// chapters position and tag
} mpc_demux;
