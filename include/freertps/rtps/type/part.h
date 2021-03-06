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

#ifndef FRUDP_PARTICIPANT_H
#define FRUDP_PARTICIPANT_H

#include "freertps/rtps/type/protocol_version.h"
#include "freertps/rtps/type/locator.h"
#include "freertps/rtps/type/guid_prefix.h"
#include "freertps/rtps/type/time.h"
#include "freertps/rtps/type/vendor_id.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t frudp_builtin_endpoint_set_t;

//TODO Rename to participant (part != partition)
typedef struct frudp_part
{
  frudp_pver_t pver;
  frudp_vid_t vid;
  frudp_guid_prefix_t guid_prefix;
  bool expects_inline_qos;
  frudp_locator_t default_unicast_locator;
  frudp_locator_t default_multicast_locator;
  frudp_locator_t metatraffic_unicast_locator;
  frudp_locator_t metatraffic_multicast_locator;
  frudp_builtin_endpoint_set_t builtin_endpoints;
  frudp_duration_t lease_duration;
  long live_count;
  char name[256];
  // Internal
  fr_time_t last_spdp;
} __attribute__((packed)) frudp_part_t;

bool frudp_part_create(void);
frudp_part_t *frudp_part_find(const frudp_guid_prefix_t *guid_prefix);
void frudp_part_fini(void);
void frudp_debug_participants(void);

#ifdef __cplusplus
}
#endif
#endif // FRUDP_PARTICIPANT_H
