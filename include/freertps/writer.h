#ifndef FREERTPS_WRITER_H
#define FREERTPS_WRITER_H

#include <stdbool.h>
#include "freertps/endpoint.h"
#include "freertps/history_cache.h"
#include "freertps/seq_num.h"
#include "freertps/reader_proxy.h"
#include "freertps/time.h"

typedef struct fr_writer
{
  struct fr_endpoint endpoint;
  bool push_mode;
  fr_duration_t heartbeat_period;
  fr_duration_t nack_response_delay;
  fr_duration_t nack_suppression_duration;
  fr_duration_t resend_data_period;
  fr_seq_num_t last_change_sequence_number;
  struct fr_container_t *matched_readers; // container of matched_readers
  struct fr_history_cache writer_cache;
} fr_writer_t;

fr_writer_t *fr_writer_create(const char *topic_name, const char *type_name);
void fr_writer_destroy(fr_writer_t *w);
void fr_writer_add_reader_proxy(fr_reader_proxy_t *rp);
bool fr_writer_new_change(struct fr_writer *w, struct fr_cache_change *cc);

#endif
