#ifndef RAOP_H
#define RAOP_H

#include "dnssd.h"
#include "stream.h"
#include "raop_ntp.h"

#if defined (WIN32) && defined(DLL_EXPORT)
# define RAOP_API __declspec(dllexport)
#else
# define RAOP_API
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Define syslog style log levels */
#define RAOP_LOG_EMERG       0       /* system is unusable */
#define RAOP_LOG_ALERT       1       /* action must be taken immediately */
#define RAOP_LOG_CRIT        2       /* critical conditions */
#define RAOP_LOG_ERR         3       /* error conditions */
#define RAOP_LOG_WARNING     4       /* warning conditions */
#define RAOP_LOG_NOTICE      5       /* normal but significant condition */
#define RAOP_LOG_INFO        6       /* informational */
#define RAOP_LOG_DEBUG       7       /* debug-level messages */


typedef void (*raop_log_callback_t)(void *cls, int level, const char *msg);

struct raop_callbacks_s {
    void* cls;

    void  (*audio_process)(void *cls, raop_ntp_t *ntp, audio_decode_struct *data);
    void  (*video_process)(void *cls, raop_ntp_t *ntp, h264_decode_struct *data);

    /* Optional but recommended callback functions */
    void  (*conn_init)(void *cls);
    void  (*conn_destroy)(void *cls);
    void  (*conn_reset) (void *cls, int timeouts, bool reset_video);
    void  (*conn_teardown)(void *cls, bool *teardown_96, bool *teardown_110 );
    void  (*audio_flush)(void *cls);
    void  (*video_flush)(void *cls);
    void  (*audio_set_volume)(void *cls, float volume);
    void  (*audio_set_metadata)(void *cls, const void *buffer, int buflen);
    void  (*audio_set_coverart)(void *cls, const void *buffer, int buflen);
    void  (*audio_remote_control_id)(void *cls, const char *dacp_id, const char *active_remote_header);
    void  (*audio_set_progress)(void *cls, unsigned int start, unsigned int curr, unsigned int end);
    void  (*audio_get_format)(void *cls, unsigned char *ct, unsigned short *spf, bool *usingScreen, bool *isMedia, uint64_t *audioFormat);
    void  (*video_report_size)(void *cls, float *width_source, float *height_source, float *width, float *height);
};
typedef struct raop_callbacks_s raop_callbacks_t;
raop_ntp_t *raop_ntp_init(logger_t *logger, raop_callbacks_t *callbacks, const unsigned char *remote_addr, int remote_addr_len, unsigned short timing_rport);
  
RAOP_API struct raop_t *raop_init(int max_clients, raop_callbacks_t *callbacks);
RAOP_API void raop_set_log_level(struct raop_t *raop, int level);
RAOP_API void raop_set_log_callback(struct raop_t *raop, raop_log_callback_t callback, void *cls);
RAOP_API int raop_set_plist(struct raop_t *raop, const char *plist_item, const int value);
RAOP_API void raop_set_port(struct raop_t *raop, unsigned short port);
RAOP_API void raop_set_udp_ports(struct raop_t *raop, unsigned short port[3]);
RAOP_API void raop_set_tcp_ports(struct raop_t *raop, unsigned short port[2]);
RAOP_API unsigned short raop_get_port(struct raop_t *raop);
RAOP_API void *raop_get_callback_cls(struct raop_t *raop);
RAOP_API int raop_start(struct raop_t *raop, unsigned short *port);
RAOP_API int raop_is_running(struct raop_t *raop);
RAOP_API void raop_stop(struct raop_t *raop);
RAOP_API void raop_set_dnssd(struct raop_t *raop, dnssd_t *dnssd);
RAOP_API void raop_destroy(struct raop_t *raop);

#ifdef __cplusplus
}
#endif
#endif