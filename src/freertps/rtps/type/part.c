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

#include "freertps/rtps/type/part.h"

#include "freertps/config.h"
#include "freertps/log.h"
#include "freertps/utility.h"
#include "freertps/psm/udp.h"
#include "freertps/psm/bswap.h"

#include "freertps/rtps/constant/vendor.h"
#include "freertps/rtps/type/config.h"
#include "freertps/rtps/type/guid_prefix.h"
#include "freertps/rtps/discovery/disco.h"

#include <stdio.h>
#include <stdlib.h>

static bool g_frudp_participant_init_complete = false;

frudp_part_t *frudp_part_find(const frudp_guid_prefix_t *guid_prefix)
{
  for (int i = 0; i < g_frudp_disco_num_parts; i++)
  {
    frudp_part_t *p = &g_frudp_disco_parts[i]; // shorter
    bool match = true;
    for (int j = 0; match && j < FRUDP_GUID_PREFIX_LEN; j++)
    {
      if (guid_prefix->prefix[j] != p->guid_prefix.prefix[j])
        match = false;
    }
    if (match)
      return p;
  }
  return NULL; // couldn't find it. sorry.
}

bool frudp_part_create(void)
{
  FREERTPS_DEBUG("frudp_part_create()\r\n");

  // Single call check
  if (g_frudp_participant_init_complete)
  {
    FREERTPS_ERROR("woah there partner. freertps currently only allows one participant.\r\n");
    return false;
  }

  // Set Vendor ID
  g_frudp_config.guid_prefix.prefix[0] = FREERTPS_VID_FREERTPS >> 8;
  g_frudp_config.guid_prefix.prefix[1] = FREERTPS_VID_FREERTPS & 0xff;

  // Set Domain ID
  g_frudp_config.domain_id = FRUDP_DOMAIN_ID;
  FREERTPS_INFO("Create domain id %d\r\n", (int)g_frudp_config.domain_id);

  // Set Participant ID
  if (!frudp_init_participant_id())
  {
    FREERTPS_ERROR("unable to initialize participant ID\r\n");
    return false;
  }

  // Set Single call check
  g_frudp_participant_init_complete = true;
  FREERTPS_INFO("prefix: %s \r\n", frudp_print_guid_prefix(&g_frudp_config.guid_prefix));

  return true;
}

void frudp_part_fini(void)
{

}

//#ifdef VERBOSE_SPDP
void frudp_debug_participants(void)
{
  fr_time_t time = fr_time_now();
  FREERTPS_INFO("\r\n");
  FREERTPS_INFO("Current time : %d \r\n", fr_time_now().seconds);
  FREERTPS_INFO("PARTICIPANT\r\n");
  FREERTPS_INFO("| ID     | NAME       | IP              | GUID                       | Bail | Last Time  | End |\r\n");
  for (unsigned i = 0; i < g_frudp_disco_num_parts; i++) {
    frudp_part_t *match = &g_frudp_disco_parts[i];
    frudp_duration_t *duration = &match->lease_duration;
    int32_t bail = (duration->sec); // + duration->nanosec); // In second
    FREERTPS_INFO("| %d\t| %s | %s | %s | %d | %d | %d |\r\n",
                  i,
                  match->name,
                  frudp_print_ip(freertps_htonl(match->default_unicast_locator.address.udp4.address)),
                  frudp_print_guid_prefix(&match->guid_prefix),
                  bail,
                  match->last_spdp.seconds,
                  (time.seconds - (match->last_spdp.seconds + bail)));
  }
  FREERTPS_INFO("\r\n");
}
//#endif
