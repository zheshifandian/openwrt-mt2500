#include "iwinfo.h"
#include "iwinfo_wext.h"

typedef union _MACHTTRANSMIT_SETTING {
	struct  {
		unsigned short  MCS:6;  // MCS
		unsigned short  rsv:1;
		unsigned short  BW:2;   //channel bandwidth 20MHz or 40 MHz
		unsigned short  ShortGI:1;
		unsigned short  STBC:1; //SPACE
		unsigned short  eTxBF:1;
		unsigned short  iTxBF:1;
		unsigned short  MODE:3; // Use definition MODE_xxx.
	} field;
	unsigned short      word;
} MACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	unsigned char           ApIdx;
	unsigned char           Addr[6];
	unsigned char           Aid;
	unsigned char           Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
	unsigned char           MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
	signed char             AvgRssi0;
	signed char             AvgRssi1;
	signed char             AvgRssi2;
	signed char             AvgRssi3;
	unsigned int            ConnectedTime;
	MACHTTRANSMIT_SETTING   TxRate;
	unsigned int            LastRxRate;
	short                   StreamSnr[3];
	short                   SoundingRespSnr[3];
#if 0
	short                   TxPER;
	short                   reserved;
#endif
} RT_802_11_MAC_ENTRY;

#define MAX_NUMBER_OF_MAC               554

typedef struct _RT_802_11_MAC_TABLE {
	unsigned long            Num;
	RT_802_11_MAC_ENTRY      Entry[MAX_NUMBER_OF_MAC]; //MAX_LEN_OF_MAC_TABLE = 32
} RT_802_11_MAC_TABLE;

#define SIOCIWFIRSTPRIV	0x8BE0
#define RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT	(SIOCIWFIRSTPRIV + 0x1F)
#define RTPRIV_IOCTL_SET                 	(SIOCIWFIRSTPRIV + 0x02)
#define RTPRIV_IOCTL_GSITESURVEY            (SIOCIWFIRSTPRIV + 0x0D)

static inline int wext_ioctl(const char *ifname, int cmd, struct iwreq *wrq)
{
	strncpy(wrq->ifr_name, ifname, IFNAMSIZ);
	return iwinfo_ioctl(cmd, wrq);
}

static int mtk_isap(const char *ifname)
{
    return !strncmp(ifname, "ra", 2);
}

static int mtk_iscli(const char *ifname)
{
    return !strncmp(ifname, "apcli", 5);
}

static int mtk_iswds(const char *ifname)
{
    return !strncmp(ifname, "wds", 3);
}

static int mtk_probe(const char *ifname)
{
	return mtk_isap(ifname) || mtk_iscli(ifname) || mtk_iswds(ifname);
}

static void mtk_close(void)
{
	/* Nop */
}

static int mtk_get_mode(const char *ifname, int *buf)
{
	return wext_ops.mode(ifname, buf);
}

static int mtk_get_ssid(const char *ifname, char *buf)
{
	return wext_ops.ssid(ifname, buf);
}

static int mtk_get_bssid(const char *ifname, char *buf)
{
	return wext_ops.bssid(ifname, buf);
}

static int mtk_get_bitrate(const char *ifname, int *buf)
{
	return wext_ops.bitrate(ifname, buf);
}

static int mtk_get_channel(const char *ifname, int *buf)
{
	return wext_ops.channel(ifname, buf);
}

static int mtk_get_center_chan1(const char *ifname, int *buf)
{
	/* Not Supported */
	return -1;
}

static int mtk_get_center_chan2(const char *ifname, int *buf)
{
	/* Not Supported */
	return -1;
}

static int mtk_get_frequency(const char *ifname, int *buf)
{
	return wext_ops.frequency(ifname, buf);
}

static int mtk_get_txpower(const char *ifname, int *buf)
{
	return wext_ops.txpower(ifname, buf);
}

static int mtk_get_signal(const char *ifname, int *buf)
{
	return wext_ops.signal(ifname, buf);
}

static int mtk_get_noise(const char *ifname, int *buf)
{
	return wext_ops.noise(ifname, buf);
}

static int mtk_get_quality(const char *ifname, int *buf)
{
	return wext_ops.quality(ifname, buf);
}

static int mtk_get_quality_max(const char *ifname, int *buf)
{
	return wext_ops.quality_max(ifname, buf);
}

static int mtk_get_assoclist(const char *ifname, char *buf, int *len)
{
	struct iwreq wrq = {};
	RT_802_11_MAC_TABLE *table;
	int i;

	table = calloc(1, sizeof(RT_802_11_MAC_TABLE));
	if (!table)
		return -1;

	wrq.u.data.pointer = (caddr_t)table;
	wrq.u.data.length  = sizeof(RT_802_11_MAC_TABLE);

	if (wext_ioctl(ifname, RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT, &wrq) < 0) {
		free(table);
		return -1;
	}

	for (i = 0; i < table->Num; i++) {
		RT_802_11_MAC_ENTRY *pe = &(table->Entry[i]);
		struct iwinfo_assoclist_entry *e = (struct iwinfo_assoclist_entry *)buf + i;

		memcpy(e->mac, pe->Addr, 6);

		*len += sizeof(struct iwinfo_assoclist_entry);
	}

	free(table);
	return 0;
}

static int mtk_get_txpwrlist(const char *ifname, char *buf, int *len)
{
	return wext_ops.txpwrlist(ifname, buf, len);
}

static int mtk_get_scanlist(const char *ifname, char *buf, int *len)
{
	return -1;
}

static int mtk_get_freqlist(const char *ifname, char *buf, int *len)
{
	return wext_ops.freqlist(ifname, buf, len);
}

static int mtk_get_country(const char *ifname, char *buf)
{
	sprintf(buf, "00");
	return 0;
}

static int mtk_get_countrylist(const char *ifname, char *buf, int *len)
{
	/* Stub */
	return -1;
}

static int mtk_get_hwmodelist(const char *ifname, int *buf)
{
	return wext_ops.hwmodelist(ifname, buf);
}

static int mtk_get_htmodelist(const char *ifname, int *buf)
{
	/* Stub */
	return -1;
}

static int mtk_get_encryption(const char *ifname, char *buf)
{
	/* No reliable crypto info in wext */
	return -1;
}

static int mtk_get_phyname(const char *ifname, char *buf)
{
	if (strstr(ifname, "ra"))
		strcpy(buf, "ra0");
	else if (strstr(ifname, "rax"))
		strcpy(buf, "rax0");
	else
		return -1;

	return 0;
}

static int mtk_get_mbssid_support(const char *ifname, int *buf)
{
	/* No multi bssid support atm */
	return -1;
}

static int mtk_get_hardware_id(const char *ifname, char *buf)
{
	return wext_ops.hardware_id(ifname, buf);
}

static int mtk_get_hardware_name(const char *ifname, char *buf)
{
	sprintf(buf, "MediaTek");
	return 0;
}

static int mtk_get_txpower_offset(const char *ifname, int *buf)
{
	/* Stub */
	*buf = 0;
	return -1;
}

static int mtk_get_frequency_offset(const char *ifname, int *buf)
{
	/* Stub */
	*buf = 0;
	return -1;
}

const struct iwinfo_ops mtk_ops = {
	.name             = "mtk",
	.probe            = mtk_probe,
	.channel          = mtk_get_channel,
	.center_chan1     = mtk_get_center_chan1,
	.center_chan2     = mtk_get_center_chan2,
	.frequency        = mtk_get_frequency,
	.frequency_offset = mtk_get_frequency_offset,
	.txpower          = mtk_get_txpower,
	.txpower_offset   = mtk_get_txpower_offset,
	.bitrate          = mtk_get_bitrate,
	.signal           = mtk_get_signal,
	.noise            = mtk_get_noise,
	.quality          = mtk_get_quality,
	.quality_max      = mtk_get_quality_max,
	.mbssid_support   = mtk_get_mbssid_support,
	.hwmodelist       = mtk_get_hwmodelist,
	.htmodelist       = mtk_get_htmodelist,
	.mode             = mtk_get_mode,
	.ssid             = mtk_get_ssid,
	.bssid            = mtk_get_bssid,
	.country          = mtk_get_country,
	.hardware_id      = mtk_get_hardware_id,
	.hardware_name    = mtk_get_hardware_name,
	.encryption       = mtk_get_encryption,
	.phyname          = mtk_get_phyname,
	.assoclist        = mtk_get_assoclist,
	.txpwrlist        = mtk_get_txpwrlist,
	.scanlist         = mtk_get_scanlist,
	.freqlist         = mtk_get_freqlist,
	.countrylist      = mtk_get_countrylist,
	.close            = mtk_close
};
