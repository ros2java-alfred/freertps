// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "freertps/rtps/subscriber.h"
#include "freertps/rtps/reader.h"
#include "freertps/rtps/discovery/sedp.h"
#include "freertps/psm/bswap.h"
#include "freertps/psm/udp.h"
#include "freertps/log.h"

frudp_sub_t g_frudp_subs[FRUDP_MAX_SUBS];
uint32_t g_frudp_num_subs = 0;

frudp_reader_t g_frudp_readers[FRUDP_MAX_READERS];
uint32_t g_frudp_num_readers = 0;

///////////////////////////////////////////////////////////////////////////

void frudp_add_reader(const frudp_reader_t *match)
{
  FREERTPS_DEBUG("frudp_add_reader(0x%08x) for %s\r\n",
                 freertps_htonl(match->reader_eid.u),
                 frudp_print_guid(&match->writer_guid));

  if (g_frudp_num_readers >= FRUDP_MAX_READERS)
    return;

  // make sure that in the meantime, we haven't already added this
  bool found = false;
  for (unsigned j = 0; !found && j < g_frudp_num_readers; j++)
  {
    frudp_reader_t *r = &g_frudp_readers[j];
    if (frudp_guid_identical(&r->writer_guid, &match->writer_guid))
      found = true;
  }

  if (found)
  {
    FREERTPS_INFO("found reader already; skipping duplicate add\r\n");
    return;
  }

  g_frudp_readers[g_frudp_num_readers] = *match;
  g_frudp_num_readers++;
}

frudp_sub_t *frudp_add_user_sub(const char *topic_name,
                                const char *type_name,
                                freertps_msg_cb_t msg_cb)
{
  frudp_eid_t sub_eid = frudp_create_user_id(FRUDP_ENTITY_KIND_USER_READER_NO_KEY);
  FREERTPS_DEBUG("frudp_add_user_sub(%s, %s) on EID 0x%08x\r\n",
                 topic_name, type_name,
                 (unsigned)freertps_htonl(sub_eid.u));

  // for now, just copy the pointers. maybe in the future we can/should have
  // an option for storage of various kind (static, malloc, etc.) for copies.
  frudp_sub_t *sub = frudp_create_sub(topic_name,
                   type_name,
                   sub_eid,
                   msg_cb,
                   NULL,
                   false);

  frudp_sedp_publish_sub(sub); // can't do this yet; spdp hasn't started bcast
  return sub;
}

frudp_sub_t * frudp_create_sub(const char *topic_name,
        const char *type_name,
        frudp_eid_t sub_eid,
        freertps_msg_cb_t msg_cb,
        frudp_rx_data_cb_t data_cb,
        bool reliable)
{
  if (g_frudp_num_subs >= FRUDP_MAX_SUBS - 1) {
    return NULL; // no room. sorry.
  }

  frudp_sub_t *s = &g_frudp_subs[g_frudp_num_subs];
  g_frudp_num_subs++;

  s->topic_name = topic_name;
  s->type_name = type_name;
  s->reader_eid = sub_eid;
  s->msg_cb = msg_cb;
  s->data_cb = data_cb;
  s->reliable = reliable;

  FREERTPS_INFO("Create sub (%d) : 0x%08x\r\n", g_frudp_num_subs, freertps_htonl((uint32_t)s->reader_eid.u));
  //frudp_subscribe(s->entity_id, g_frudp_entity_id_unknown, NULL, s->msg_cb);
  return s;
}

void frudp_debug_readers(void)
{
  for (unsigned i = 0; i < g_frudp_num_readers; i++)
  {
    frudp_reader_t *match = &g_frudp_readers[i];
    FREERTPS_INFO("\tsub\t%d: writer = %s  => %08x\r\n",
                  (int)i,
                  frudp_print_guid(&match->writer_guid),
                  (unsigned)freertps_htonl(match->reader_eid.u));
  }
}

///////////////////////////////////////////////////////////////////////////
/*
static void frudp_add_userland_subscription(
                                frudp_userland_subscription_request_t *s);
frudp_userland_subscription_request_t g_frudp_userland_subs[FRUDP_MAX_USERLAND_SUBS];
uint32_t g_frudp_num_userland_subs = 0;
*/
///////////////////////////////////////////////////////////////////////////

/*
NEED TO START RE-STRUCTURING STUFF HERE. THE SUBSCRIPTION OBJECTS SHOULD
HOLD ONLY THE "DESIRE" FOR A SUBSCRIPTION, AND FEED INTO A MATCHED-READER
OBJECT AT SOME POINT WHEN WE ACTUALLY HEAR ABOUT OTHER ENDPOINTS.
*/

/*
bool frudp_subscribe(const frudp_entity_id_t reader_id,
                     const frudp_entity_id_t writer_id,
                     const frudp_rx_data_cb_t data_cb,
                     const freertps_msg_cb_t msg_cb)
{
  if (g_frudp_subs_used >= FRUDP_MAX_SUBSCRIPTIONS)
    return false;
  frudp_subscription_t *sub = &g_frudp_subs[g_frudp_subs_used];
  sub->reader_id = reader_id;
  sub->writer_id = writer_id;
  sub->data_cb = data_cb;
  sub->msg_cb = msg_cb;
  sub->max_rx_sn.low = sub->max_rx_sn.high = 0;
  g_frudp_subs_used++;
  return true;
}
*/
