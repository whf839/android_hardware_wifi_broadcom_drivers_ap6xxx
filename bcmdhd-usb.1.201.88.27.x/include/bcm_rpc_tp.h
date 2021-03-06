/*
 * RPC - Transport layer
 *   HOST may glue it to DBUS layer
 *   CLIENT may glue it to its bus driver
 *
 * Copyright (C) 1999-2015, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
 * $Id: bcm_rpc_tp.h 538701 2015-03-04 18:12:10Z $
 */

#ifndef _bcm_rpc_tp_h_
#define _bcm_rpc_tp_h_
#include <bcm_rpc.h>

#define BCM_RPC_TP_ENCAP_LEN	4	/* TP header is 4 bytes */
#if defined(BCMSDIODEV_ENABLED)
#define BCM_RPC_BUS_HDR_LEN	(BCMDONGLEHDRSZ + SDALIGN)
#else
#define BCM_RPC_BUS_HDR_LEN	0
#endif

#define BCM_FDAGGR_D2H_ENABLED	0x01
#define BCM_FDAGGR_H2D_ENABLED	0x02

#define BCM_RPC_TP_HOST_AGG_MASK	0xffff0000
#define BCM_RPC_TP_HOST_AGG_SHIFT	16
#define BCM_RPC_TP_HOST_AGG_AMPDU	0x00010000	/* HOST->DNGL ampdu aggregation */
#define BCM_RPC_TP_HOST_AGG_TEST	0x00100000	/* HOST->DNGL test aggregation */
#define BCM_RPC_TP_DNGL_AGG_MASK	0x0000ffff
#define BCM_RPC_TP_DNGL_AGG_DPC		0x00000001	/* DNGL->HOST data aggregation */
#define BCM_RPC_TP_DNGL_AGG_FLOWCTL	0x00000002	/* DNGL->HOST tx flowcontrol agg */
#define BCM_RPC_TP_DNGL_AGG_TEST	0x00000010	/* DNGL->HOST test agg */
#define BCM_RPC_TP_AGG_SF_SHIFT		16
#define BCM_RPC_TP_AGG_SF_MASK		0xffff0000
#define BCM_RPC_TP_AGG_BYTES_MASK	0x0000ffff

#define BCM_RPC_TP_DNGL_AGG_MAX_SFRAME	3       /* max agg subframes, must be <= USB_NTXD */
#if defined(BCM_RPC_NOCOPY) || defined(BCM_RPC_RXNOCOPY)
#define BCM_RPC_TP_DNGL_AGG_MAX_BYTE	2100    /* max agg bytes, we only do either agg or nocopy */
#else
#define BCM_RPC_TP_DNGL_AGG_MAX_BYTE	4000    /* max agg bytes */
#endif /* BCM_RPC_NOCOPY || BCM_RPC_RXNOCOPY */

#define BCM_RPC_TP_DNGL_AGG_MAX_BYTE_4360	16000

#define BCM_RPC_TP_HOST_AGG_MAX_SFRAME  3	/* max agg subframes, AMPDU only, 3 is enough */
#define BCM_RPC_TP_HOST_AGG_MAX_BYTE    3400    /* max agg bytes; to fit 2+ tcp/udp pkts. Each one:
						 * 802.3pkt + 802.11 hdr + rpc hdr + tp hdr < 1700B
						 * Need to be in sync with dongle usb rx dma
						 *  rxbufsize(USBBULK_RXBUF_GIANT in usbdev_sb.c)
						 */

/* chip specific AMPDU_MPDU should be defined in chipid makefile.
 * if not defined in chipid makefile defaults defined below will be used
 */
#ifndef BCM_AMPDU_MPDU
#define BCM_AMPDU_MPDU			16
#endif
/* chip specific HOST/DNGL_DEFAULT_SFRAME/BYTE constants should be defined in chipid makefile
 * if not defined in chipid makefile defaults defined below will be used
 */
#ifndef BCM_RPC_TP_HOST_AGG_DEFAULT_SFRAME
#define BCM_RPC_TP_HOST_AGG_DEFAULT_SFRAME	3
#endif
#ifndef BCM_RPC_TP_HOST_AGG_DEFAULT_BYTE
#define BCM_RPC_TP_HOST_AGG_DEFAULT_BYTE	3400
#endif
#ifndef BCM_RPC_TP_DNGL_AGG_DEFAULT_SFRAME
#define BCM_RPC_TP_DNGL_AGG_DEFAULT_SFRAME	3
#endif
#ifndef BCM_RPC_TP_DNGL_AGG_DEFAULT_BYTE
#define BCM_RPC_TP_DNGL_AGG_DEFAULT_BYTE	3400
#endif
#define BCM_RPC_TP_HOST_AGG_DEFAULT	(((BCM_RPC_TP_HOST_AGG_DEFAULT_SFRAME)\
	<< BCM_RPC_TP_HOST_AGG_SHIFT) | BCM_RPC_TP_HOST_AGG_DEFAULT_BYTE)
#define BCM_RPC_TP_DNGL_AGG_DEFAULT	(((BCM_RPC_TP_DNGL_AGG_DEFAULT_SFRAME)\
	<< BCM_RPC_TP_HOST_AGG_SHIFT) | BCM_RPC_TP_DNGL_AGG_DEFAULT_BYTE)

#ifndef DBUS_RX_BUFFER_SIZE_RPC
#define DBUS_RX_BUFFER_SIZE_RPC    (BCM_RPC_TP_DNGL_AGG_MAX_BYTE)
#endif /* DBUS_RX_BUFFER_SIZE_RPC */

#if (0 && (0>= 0x0600) && 0)
/* TP-DBUS pkts flowcontrol */
#ifndef BCM_RPC_TP_DBUS_NTXQ
#define BCM_RPC_TP_DBUS_NTXQ	512	/* queue size for TX on bulk OUT, aggregation possible */
#endif
#ifndef BCM_RPC_TP_DBUS_NRXQ
#define BCM_RPC_TP_DBUS_NRXQ	512	/* queue size for RX on bulk IN, aggregation possible */
#endif
#else
#ifndef BCM_RPC_TP_DBUS_NTXQ
#define BCM_RPC_TP_DBUS_NTXQ	50	/* queue size for TX on bulk OUT, aggregation possible */
#endif
#ifndef BCM_RPC_TP_DBUS_NRXQ
#define BCM_RPC_TP_DBUS_NRXQ	50	/* queue size for RX on bulk IN, aggregation possible */
#endif
#endif 
#define BCM_RPC_TP_DBUS_NRXQ_CTRL	1	/* queue size for RX on ctl EP0 */

#define BCM_RPC_TP_DBUS_NRXQ_PKT	(BCM_RPC_TP_DBUS_NRXQ * BCM_RPC_TP_DNGL_AGG_MAX_SFRAME)
#define BCM_RPC_TP_DBUS_NTXQ_PKT	(BCM_RPC_TP_DBUS_NTXQ * BCM_RPC_TP_HOST_AGG_MAX_SFRAME)

#define BCM_RPC_TP_PADHEAD_SHIFT 24
#define BCM_RPC_TP_PADEND_SHIFT 16
#define BCM_RPC_TP_PADHEAD_MASK 0xff
#define BCM_RPC_TP_PADEND_MASK 0xff
#define BCM_RPC_TP_LEN_MASK 0xffff
#define BCM_RPC_TP_HOST_TMOUT 10 /* in ms */
#define BCM_RPC_TP_DNGL_TMOUT 5 /* in ms */

typedef struct rpc_transport_info rpc_tp_info_t;

typedef void (*rpc_tx_complete_fn_t)(void*, rpc_buf_t *, int status);
typedef void (*rpc_rx_fn_t)(void*, rpc_buf_t*);


extern void bcm_rpc_tp_sleep(rpc_tp_info_t * rpcb);
extern int  bcm_rpc_tp_resume(rpc_tp_info_t * rpcb, int *fw_reload);
extern int bcm_rpc_tp_shutdown(rpc_tp_info_t * rpcb);
extern rpc_tp_info_t *bcm_rpc_tp_attach(osl_t * osh, void *bus);
extern void bcm_rpc_tp_detach(rpc_tp_info_t * rpcb);
extern void bcm_rpc_tp_down(rpc_tp_info_t *rpcb);
extern void bcm_rpc_tp_watchdog(rpc_tp_info_t *rpcb);

extern int bcm_rpc_tp_buf_send(rpc_tp_info_t * rpcb, rpc_buf_t *buf);

/* callback for tx_complete, rx_pkt */
extern void bcm_rpc_tp_register_cb(rpc_tp_info_t * rpcb, rpc_tx_complete_fn_t txcmplt,
	void* tx_context, rpc_rx_fn_t rxpkt, void* rx_context, rpc_osl_t *rpc_osh);
extern void bcm_rpc_tp_deregister_cb(rpc_tp_info_t * rpcb);

/* Buffer manipulation */
extern uint bcm_rpc_buf_tp_header_len(rpc_tp_info_t * rpcb);
extern rpc_buf_t *bcm_rpc_tp_buf_alloc(rpc_tp_info_t * rpcb, int len);
extern void bcm_rpc_tp_buf_free(rpc_tp_info_t * rpcb, rpc_buf_t *buf);
extern int bcm_rpc_buf_len_get(rpc_tp_info_t * rpcb, rpc_buf_t* b);
extern int bcm_rpc_buf_totlen_get(rpc_tp_info_t * rpcb, rpc_buf_t* b);
extern int bcm_rpc_buf_len_set(rpc_tp_info_t * rpcb, rpc_buf_t* b, uint len);
extern rpc_buf_t *bcm_rpc_buf_next_get(rpc_tp_info_t * rpcb, rpc_buf_t* b);
extern void bcm_rpc_buf_next_set(rpc_tp_info_t * rpcb, rpc_buf_t* b, rpc_buf_t *nextb);
extern unsigned char* bcm_rpc_buf_data(rpc_tp_info_t * rpcb, rpc_buf_t* b);
#ifdef __FreeBSD__
extern unsigned char* bcm_rpc_buf_push(rpc_tp_info_t * rpcb, rpc_buf_t** b, uint delta);
#else
extern unsigned char* bcm_rpc_buf_push(rpc_tp_info_t * rpcb, rpc_buf_t* b, uint delta);
#endif
extern unsigned char* bcm_rpc_buf_pull(rpc_tp_info_t * rpcb, rpc_buf_t* b, uint delta);
extern rpc_buf_t * bcm_rpc_buf_pktdup(rpc_tp_info_t * rpcb, rpc_buf_t* b);
extern void bcm_rpc_tp_buf_release(rpc_tp_info_t * rpcb, rpc_buf_t *buf);
extern void bcm_rpc_tp_buf_cnt_adjust(rpc_tp_info_t * rpcb, int adjust);
#if defined(BCM_FD_AGGR)
extern int bcm_rpc_tp_set_config(rpc_tp_info_t *rpc_th, void *config);
#endif	


extern void bcm_rpc_tp_agg_set(rpc_tp_info_t *rpcb, uint32 reason, bool set);
extern void bcm_rpc_tp_agg_limit_set(rpc_tp_info_t *rpc_th, uint8 sf, uint16 bytes);
extern void bcm_rpc_tp_agg_limit_get(rpc_tp_info_t *rpc_th, uint8 *sf, uint16 *bytes);


#define BCM_RPC_TP_MSG_LEVEL_MASK	0x00ff
/* dongle msg level */
#define RPC_TP_MSG_DNGL_ERR_VAL		0x0001	/* DNGL TP error msg */
#define RPC_TP_MSG_DNGL_DBG_VAL		0x0002	/* DNGL TP dbg msg */
#define RPC_TP_MSG_DNGL_AGG_VAL		0x0004	/* DNGL TP agg msg */
#define RPC_TP_MSG_DNGL_DEA_VAL		0x0008	/* DNGL TP deag msg */

/* host msg level */
#define RPC_TP_MSG_HOST_ERR_VAL		0x0001	/* DNGL TP error msg */
#define RPC_TP_MSG_HOST_DBG_VAL		0x0002	/* DNGL TP dbg msg */
#define RPC_TP_MSG_HOST_AGG_VAL		0x0004	/* DNGL TP agg msg */
#define RPC_TP_MSG_HOST_DEA_VAL		0x0008	/* DNGL TP deag msg */

extern void bcm_rpc_tp_msglevel_set(rpc_tp_info_t *rpc_th, uint8 msglevel, bool high_low);
#ifdef BCM_FD_AGGR
extern void bcm_rpc_dbus_recv_aggrpkt(void *handle, void *pkt, int len);
extern void bcm_rpc_dbus_recv_aggrbuf(void *handle, uint8 *buf, int len);
#endif
extern uint32 bcm_rpc_tp_agg_get(rpc_tp_info_t *rpcb);

#endif /* _bcm_rpc_tp_h_ */
