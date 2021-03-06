#include "freertps/freertps.h"
#include "freertps/psm/udp.h"
#include "freertps/psm/bswap.h"
#include "freertps/rtps/discovery/disco.h"
#include "freertps/rtps/constant/vendor.h"
#include "freertps/rtps/type/config.h"

#include "metal/enet_config.h"
#include "metal/enet.h"
#include "metal/systime.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

bool frudp_init(void)
{
  enet_init();

  FREERTPS_INFO("metal udp init()\r\n");
  FREERTPS_INFO("using address %s for unicast\r\n (new code)", frudp_print_ip(FRUDP_IP4_ADDR));
  FREERTPS_INFO("using address %d.%d.%d.%d for unicast (original code)\r\n",
                (FRUDP_IP4_ADDR >> 24) & 0xff,
                (FRUDP_IP4_ADDR >> 16) & 0xff,
                (FRUDP_IP4_ADDR >>  8) & 0xff,
                (FRUDP_IP4_ADDR      ) & 0xff);

  g_frudp_config.unicast_addr = freertps_htonl(FRUDP_IP4_ADDR);
  memcpy(&g_frudp_config.guid_prefix.prefix[2], g_enet_mac, 6);

  frudp_generic_init();
  // not sure about endianness here.
  // 4 bytes left. let's use the system time in microseconds since power-up
  // todo: init ethernet PHY. after PHY link is up,
  // store system time in the guid_prefix.
  //memcpy(&g_frudp_config.guid_prefix.prefix[8], &pid, 4);
  //frudp_disco_init();
  return true;
}

void frudp_fini(void)
{
  frudp_disco_fini();
  FREERTPS_INFO("metal udp fini\r\n");
}

bool frudp_listen(const uint32_t max_usec)
{
  // just busy-wait here for the requested amount of time
  volatile uint32_t t_start = systime_usecs();
  while (1)
  {
    enet_process_rx_ring();
    volatile uint32_t t = systime_usecs();
    if (t - t_start >= max_usec)
      break;
  }
  return true;
}

bool frudp_tx(const uint32_t dst_addr,
              const uint16_t dst_port,
              const uint8_t *tx_data,
              const uint16_t tx_len)
{
  if ((dst_addr & 0xe0000000) == 0xe0000000)
  {
    // ipv4 multicast
    enet_send_udp_mcast(dst_addr, dst_port, tx_data, tx_len);
  }
  else
  {
    uint8_t dst_mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // todo: not this
    enet_send_udp_ucast(dst_mac,
                        dst_addr, dst_port,
                        FRUDP_IP4_ADDR, dst_port,
                        tx_data, tx_len);
  }
  return true;
}

bool frudp_add_mcast_rx(uint32_t group, uint16_t port)
{
  // todo: multicast group filtering
  return enet_allow_udp_port(port);
}

bool frudp_add_ucast_rx(const uint16_t port)
{
  return enet_allow_udp_port(port);
}

long getIp() {
    return FRUDP_IP4_ADDR;
}

long getNetwork() {
    return 0;
}
