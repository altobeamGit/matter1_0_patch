#ifndef __ATBM_GENERAL_H__
#define __ATBM_GENERAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "arch/cc.h"
#include "arch/sys_arch.h"

#define BIT(n) (1<<n)

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
//typedef unsigned int size_t;

typedef enum _SECURITY_TYPE{
	KEY_NONE = 0,
	KEY_WEP,
	KEY_WEP_SHARE,
	KEY_WPA,
	KEY_WPA2,
	KEY_WPA3,
	KEY_MAX,
}SECURITY_TYPE;

typedef enum _WIFI_IP_EVENT{
	WIFI_EVENT_SCAN_DONE,
	WIFI_EVENT_STA_START,
	WIFI_EVENT_STA_CONNECTED,
	WIFI_EVENT_STA_DISCONNECTED,
	WIFI_EVENT_STA_STOP,
	WIFI_EVENT_AP_START,
	WIFI_EVENT_AP_STOP,
	WIFI_EVENT_AP_STACONNECTED,
	IP_EVENT_STA_GOT_IP,
	IP_EVENT_STA_LOST_IP,
	IP_EVENT_GOT_IP6,
	WIFI_IP_EVENT_MAX,
}WIFI_IP_EVENT;

struct atbmwifi_scan_result_info{
	u8 ssid[33];
	u8 bssid[6];
	u8 ssidlen;
	u8 channel;
	u8 ht:1,
	   wpa:1,
	   rsn:1,
	   wps:1,
	   p2p:1,
	   b40M:1,
	   encrypt:1;
	s8 rssi;
#ifdef CONFIG_8266_AT_CMD
	short pairwise_cipher;
	short group_cipher;
#else
	short reserved[2];
#endif
};

struct atbmwifi_scan_result{
	int len;
	struct atbmwifi_scan_result_info *info;
};

typedef void (*atbm_wifi_scanRet_cb_t)(struct atbmwifi_scan_result *scan_result);
typedef int (*atbm_wifi_matter_cmd_cb_t)(int argc, char **argv);
typedef void (*atbm_wifi_matter_event_cb_t)(unsigned int eventId);

struct atbmwifi_configure{
	u8 ssid[32];
	u8 ssid_len;
	u8 bssid[6];
	u8 password_len;
	u8 password[64];
	u8 key_mgmt;
	u8 key_id;
	u16 auth_alg;
};

extern int printf(const char *fmt, ...);
bool hal_in_irq(void);
u32 sys_mem_total_size_get(void);
u32 sys_mem_free_size_get(void);
void sys_mutex_lock(sys_mutex_t *mutex);
void sys_mutex_unlock(sys_mutex_t *mutex);
void hal_sys_reboot(void);
void atbm_wifi_get_mac_addr(u8 *mac_addr);
void atbm_wifi_get_connected_destAddr(u8 *destMacAddr);
u8 atbm_wifi_get_channel(void);
int atbm_wifi_get_rssi(void);
int atbm_wifi_scan(void);
int atbm_wifi_scan_is_done(void);
struct atbmwifi_scan_result *atbm_wifi_scan_get_result(void);
int atbm_wifi_process_scan_result(atbm_wifi_scanRet_cb_t proc_scanResult);
int atbm_wifi_connect_ap(void);
int atbm_wifi_connect_ap_autoMgmt(void);
u8 atbm_wifi_is_connect_ok(void);
void atbm_wifi_set_config(u8 *essid, int essid_len, u8 *key, int key_len, int key_mgmt, int keyid, u8 *bssid);
void atbm_wifi_get_config(struct atbmwifi_configure *config);
void atbm_wifi_clear_config(void);
void atbm_wifi_startap_call_task(void);
void atbm_wifi_stopap_call_task(void);
void atbm_wifi_start_station_call_task(void);
void atbm_wifi_stop_station_call_task(void);
unsigned int hal_get_os_ms_time(void);
int random_get_bytes(u8 *buf, size_t len);
struct netif *atbm_wifi_get_sta_netif(void);
void atbm_console_matter_cmd_register(atbm_wifi_matter_cmd_cb_t matter_cmd_handler);
u8 atbm_wifi_is_iftype_ap(void);
u8 atbm_wifi_is_iftype_station(void);
void atbm_wifi_matter_event_handler_register(atbm_wifi_matter_event_cb_t event_cb);
void atbm_wifi_disconnect_call_task(void);
u16 atbm_wifi_get_disconn_reason_code(void);

#define iot_printf printf

#ifdef __cplusplus
}
#endif

#endif /* __ATBM_GENERAL_H__ */
