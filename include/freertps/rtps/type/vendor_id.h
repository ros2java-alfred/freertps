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

#ifndef FREERTPS_VENDOR_ID_H
#define FREERTPS_VENDOR_ID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t frudp_vid_t;

const char *frudp_vendor(const frudp_vid_t vid);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_VENDOR_ID_H
