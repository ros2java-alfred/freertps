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

#ifndef FREERTPS_TIME_H
#define FREERTPS_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define TIME_ZERO {0, 0}
#define TIME_INVALID {-1, 0xffffffff}
#define TIME_INFINITE {0x7fffffff, 0xffffffff}

typedef struct fr_time
{
  int32_t  seconds;    // time in seconds
  uint32_t fraction;   // time in sec/2^32
} __attribute__((packed)) fr_time_t;    // time = seconds + (fraction / 2^(32))

typedef struct
{
  int32_t sec;
  uint32_t nanosec;
} frudp_duration_t;

typedef fr_time_t fr_duration_t;

fr_time_t     fr_time_now(void);
fr_duration_t fr_time_diff(const fr_time_t *end, const fr_time_t *start);
double fr_time_double(const fr_time_t *t);
double fr_time_now_double(void); // convenience function
double fr_duration_double(const fr_duration_t *t);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_TIME_H
