#ifndef FREERTPS_SEQUENCE_NUMBER_H
#define FREERTPS_SEQUENCE_NUMBER_H

#include <stdint.h>

typedef struct fr_sequence_number
{
  int32_t high;
  uint32_t low;
} fr_seq_num_t;

typedef struct
{
  fr_seq_num_t bitmap_base;
  uint32_t num_bits;
  uint32_t bitmap[];
} fr_seq_num_set_t;

typedef struct
{
  fr_seq_num_t bitmap_base;
  uint32_t num_bits;
  uint32_t bitmap;
} fr_seq_num_set_32bits_t;

extern const fr_seq_num_t g_fr_seq_num_unknown;

#endif
