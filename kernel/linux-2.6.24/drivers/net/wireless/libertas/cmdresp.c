/**
  * This file contains the handling of command
  * responses as well as events generated by firmware.
  */
#include <linux/delay.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>

#include <net/iw_handler.h>

#include "host.h"
#include "decl.h"
#include "defs.h"
#include "dev.h"
#include "join.h"
#include "wext.h"

/**
 *  @brief This function handles disconnect event. it
 *  reports disconnect to upper layer, clean tx/rx packets,
 *  reset link state etc.
 *
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   n/a
 */
void libertas_mac_event_disconnected(wlan_private * priv)
{
	wlan_adapter *adapter = priv->adapter;
	union iwreq_data wrqu;

	if (adapter->connect_status != LIBERTAS_CONNECTED)
		return;

	lbs_deb_enter(LBS_DEB_CMD);

	memset(wrqu.ap_addr.sa_data, 0x00, ETH_ALEN);
	wrqu.ap_addr.sa_family = ARPHRD_ETHER;

	/*
	 * Cisco AP sends EAP failure and de-auth in less than 0.5 ms.
	 * It causes problem in the Supplicant
	 */

	msleep_interruptible(1000);
	wireless_send_event(priv->dev, SIOCGIWAP, &wrqu, NULL);

	/* Free Tx and Rx packets */
	kfree_skb(priv->adapter->currenttxskb);
	priv->adapter->currenttxskb = NULL;

	/* report disconnect to upper layer */
	netif_stop_queue(priv->dev);
	netif_carrier_off(priv->dev);

	/* reset SNR/NF/RSSI values */
	memset(adapter->SNR, 0x00, sizeof(adapter->SNR));
	memset(adapter->NF, 0x00, sizeof(adapter->NF));
	memset(adapter->RSSI, 0x00, sizeof(adapter->RSSI));
	memset(adapter->rawSNR, 0x00, sizeof(adapter->rawSNR));
	memset(adapter->rawNF, 0x00, sizeof(adapter->rawNF));
	adapter->nextSNRNF = 0;
	adapter->numSNRNF = 0;
	lbs_deb_cmd("current SSID '%s', length %u\n",
	            escape_essid(adapter->curbssparams.ssid,
	                         adapter->curbssparams.ssid_len),
	            adapter->curbssparams.ssid_len);

	adapter->connect_status = LIBERTAS_DISCONNECTED;

	/* Clear out associated SSID and BSSID since connection is
	 * no longer valid.
	 */
	memset(&adapter->curbssparams.bssid, 0, ETH_ALEN);
	memset(&adapter->curbssparams.ssid, 0, IW_ESSID_MAX_SIZE);
	adapter->curbssparams.ssid_len = 0;

	if (adapter->psstate != PS_STATE_FULL_POWER) {
		/* make firmware to exit PS mode */
		lbs_deb_cmd("disconnected, so exit PS mode\n");
		libertas_ps_wakeup(priv, 0);
	}
	lbs_deb_leave(LBS_DEB_CMD);
}

/**
 *  @brief This function handles MIC failure event.
 *
 *  @param priv    A pointer to wlan_private structure
 *  @para  event   the event id
 *  @return 	   n/a
 */
static void handle_mic_failureevent(wlan_private * priv, u32 event)
{
	char buf[50];

	lbs_deb_enter(LBS_DEB_CMD);
	memset(buf, 0, sizeof(buf));

	sprintf(buf, "%s", "MLME-MICHAELMICFAILURE.indication ");

	if (event == MACREG_INT_CODE_MIC_ERR_UNICAST) {
		strcat(buf, "unicast ");
	} else {
		strcat(buf, "multicast ");
	}

	libertas_send_iwevcustom_event(priv, buf);
	lbs_deb_leave(LBS_DEB_CMD);
}

static int wlan_ret_reg_access(wlan_private * priv,
			       u16 type, struct cmd_ds_command *resp)
{
	int ret = 0;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	switch (type) {
	case CMD_RET(CMD_MAC_REG_ACCESS):
		{
			struct cmd_ds_mac_reg_access *reg = &resp->params.macreg;

			adapter->offsetvalue.offset = (u32)le16_to_cpu(reg->offset);
			adapter->offsetvalue.value = le32_to_cpu(reg->value);
			break;
		}

	case CMD_RET(CMD_BBP_REG_ACCESS):
		{
			struct cmd_ds_bbp_reg_access *reg = &resp->params.bbpreg;

			adapter->offsetvalue.offset = (u32)le16_to_cpu(reg->offset);
			adapter->offsetvalue.value = reg->value;
			break;
		}

	case CMD_RET(CMD_RF_REG_ACCESS):
		{
			struct cmd_ds_rf_reg_access *reg = &resp->params.rfreg;

			adapter->offsetvalue.offset = (u32)le16_to_cpu(reg->offset);
			adapter->offsetvalue.value = reg->value;
			break;
		}

	default:
		ret = -1;
	}

	lbs_deb_leave_args(LBS_DEB_CMD, "ret %d", ret);
	return ret;
}

static int wlan_ret_get_hw_spec(wlan_private * priv,
				struct cmd_ds_command *resp)
{
	u32 i;
	struct cmd_ds_get_hw_spec *hwspec = &resp->params.hwspec;
	wlan_adapter *adapter = priv->adapter;
	int ret = 0;
	DECLARE_MAC_BUF(mac);

	lbs_deb_enter(LBS_DEB_CMD);

	adapter->fwcapinfo = le32_to_cpu(hwspec->fwcapinfo);

	memcpy(adapter->fwreleasenumber, hwspec->fwreleasenumber, 4);

	lbs_deb_cmd("GET_HW_SPEC: firmware release %u.%u.%up%u\n",
		    adapter->fwreleasenumber[2], adapter->fwreleasenumber[1],
		    adapter->fwreleasenumber[0], adapter->fwreleasenumber[3]);
	lbs_deb_cmd("GET_HW_SPEC: MAC addr %s\n",
		    print_mac(mac, hwspec->permanentaddr));
	lbs_deb_cmd("GET_HW_SPEC: hardware interface 0x%x, hardware spec 0x%04x\n",
	       hwspec->hwifversion, hwspec->version);

	/* Clamp region code to 8-bit since FW spec indicates that it should
	 * only ever be 8-bit, even though the field size is 16-bit.  Some firmware
	 * returns non-zero high 8 bits here.
	 */
	adapter->regioncode = le16_to_cpu(hwspec->regioncode) & 0xFF;

	for (i = 0; i < MRVDRV_MAX_REGION_CODE; i++) {
		/* use the region code to search for the index */
		if (adapter->regioncode == libertas_region_code_to_index[i]) {
			break;
		}
	}

	/* if it's unidentified region code, use the default (USA) */
	if (i >= MRVDRV_MAX_REGION_CODE) {
		adapter->regioncode = 0x10;
		lbs_pr_info("unidentified region code; using the default (USA)\n");
	}

	if (adapter->current_addr[0] == 0xff)
		memmove(adapter->current_addr, hwspec->permanentaddr, ETH_ALEN);

	memcpy(priv->dev->dev_addr, adapter->current_addr, ETH_ALEN);
	if (priv->mesh_dev)
		memcpy(priv->mesh_dev->dev_addr, adapter->current_addr, ETH_ALEN);

	if (libertas_set_regiontable(priv, adapter->regioncode, 0)) {
		ret = -1;
		goto done;
	}

	if (libertas_set_universaltable(priv, 0)) {
		ret = -1;
		goto done;
	}

done:
	lbs_deb_enter_args(LBS_DEB_CMD, "ret %d", ret);
	return ret;
}

static int wlan_ret_802_11_sleep_params(wlan_private * priv,
					struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_sleep_params *sp = &resp->params.sleep_params;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	lbs_deb_cmd("error 0x%x, offset 0x%x, stabletime 0x%x, calcontrol 0x%x "
		    "extsleepclk 0x%x\n", le16_to_cpu(sp->error),
		    le16_to_cpu(sp->offset), le16_to_cpu(sp->stabletime),
		    sp->calcontrol, sp->externalsleepclk);

	adapter->sp.sp_error = le16_to_cpu(sp->error);
	adapter->sp.sp_offset = le16_to_cpu(sp->offset);
	adapter->sp.sp_stabletime = le16_to_cpu(sp->stabletime);
	adapter->sp.sp_calcontrol = sp->calcontrol;
	adapter->sp.sp_extsleepclk = sp->externalsleepclk;
	adapter->sp.sp_reserved = le16_to_cpu(sp->reserved);

	lbs_deb_enter(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_stat(wlan_private * priv,
				struct cmd_ds_command *resp)
{
	lbs_deb_enter(LBS_DEB_CMD);
/*	currently adapter->wlan802_11Stat is unused

	struct cmd_ds_802_11_get_stat *p11Stat = &resp->params.gstat;
	wlan_adapter *adapter = priv->adapter;

	// TODO Convert it to Big endian befor copy
	memcpy(&adapter->wlan802_11Stat,
	       p11Stat, sizeof(struct cmd_ds_802_11_get_stat));
*/
	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_snmp_mib(wlan_private * priv,
				    struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_snmp_mib *smib = &resp->params.smib;
	u16 oid = le16_to_cpu(smib->oid);
	u16 querytype = le16_to_cpu(smib->querytype);

	lbs_deb_enter(LBS_DEB_CMD);

	lbs_deb_cmd("SNMP_RESP: oid 0x%x, querytype 0x%x\n", oid,
	       querytype);
	lbs_deb_cmd("SNMP_RESP: Buf size %d\n", le16_to_cpu(smib->bufsize));

	if (querytype == CMD_ACT_GET) {
		switch (oid) {
		case FRAGTHRESH_I:
			priv->adapter->fragthsd =
				le16_to_cpu(*((__le16 *)(smib->value)));
			lbs_deb_cmd("SNMP_RESP: frag threshold %u\n",
				    priv->adapter->fragthsd);
			break;
		case RTSTHRESH_I:
			priv->adapter->rtsthsd =
				le16_to_cpu(*((__le16 *)(smib->value)));
			lbs_deb_cmd("SNMP_RESP: rts threshold %u\n",
				    priv->adapter->rtsthsd);
			break;
		case SHORT_RETRYLIM_I:
			priv->adapter->txretrycount =
				le16_to_cpu(*((__le16 *)(smib->value)));
			lbs_deb_cmd("SNMP_RESP: tx retry count %u\n",
				    priv->adapter->rtsthsd);
			break;
		default:
			break;
		}
	}

	lbs_deb_enter(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_key_material(wlan_private * priv,
					struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_key_material *pkeymaterial =
	    &resp->params.keymaterial;
	wlan_adapter *adapter = priv->adapter;
	u16 action = le16_to_cpu(pkeymaterial->action);

	lbs_deb_enter(LBS_DEB_CMD);

	/* Copy the returned key to driver private data */
	if (action == CMD_ACT_GET) {
		u8 * buf_ptr = (u8 *) &pkeymaterial->keyParamSet;
		u8 * resp_end = (u8 *) (resp + le16_to_cpu(resp->size));

		while (buf_ptr < resp_end) {
			struct MrvlIEtype_keyParamSet * pkeyparamset =
			    (struct MrvlIEtype_keyParamSet *) buf_ptr;
			struct enc_key * pkey;
			u16 param_set_len = le16_to_cpu(pkeyparamset->length);
			u16 key_len = le16_to_cpu(pkeyparamset->keylen);
			u16 key_flags = le16_to_cpu(pkeyparamset->keyinfo);
			u16 key_type = le16_to_cpu(pkeyparamset->keytypeid);
			u8 * end;

			end = (u8 *) pkeyparamset + sizeof (pkeyparamset->type)
			                          + sizeof (pkeyparamset->length)
			                          + param_set_len;
			/* Make sure we don't access past the end of the IEs */
			if (end > resp_end)
				break;

			if (key_flags & KEY_INFO_WPA_UNICAST)
				pkey = &adapter->wpa_unicast_key;
			else if (key_flags & KEY_INFO_WPA_MCAST)
				pkey = &adapter->wpa_mcast_key;
			else
				break;

			/* Copy returned key into driver */
			memset(pkey, 0, sizeof(struct enc_key));
			if (key_len > sizeof(pkey->key))
				break;
			pkey->type = key_type;
			pkey->flags = key_flags;
			pkey->len = key_len;
			memcpy(pkey->key, pkeyparamset->key, pkey->len);

			buf_ptr = end + 1;
		}
	}

	lbs_deb_enter(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_mac_address(wlan_private * priv,
				       struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_mac_address *macadd = &resp->params.macadd;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	memcpy(adapter->current_addr, macadd->macadd, ETH_ALEN);

	lbs_deb_enter(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_rf_tx_power(wlan_private * priv,
				       struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_rf_tx_power *rtp = &resp->params.txp;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	adapter->txpowerlevel = le16_to_cpu(rtp->currentlevel);

	lbs_deb_cmd("TX power currently %d\n", adapter->txpowerlevel);

	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_rate_adapt_rateset(wlan_private * priv,
					      struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_rate_adapt_rateset *rates = &resp->params.rateset;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	if (rates->action == CMD_ACT_GET) {
		adapter->enablehwauto = le16_to_cpu(rates->enablehwauto);
		adapter->ratebitmap = le16_to_cpu(rates->bitmap);
	}

	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_data_rate(wlan_private * priv,
				     struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_data_rate *pdatarate = &resp->params.drate;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	lbs_deb_hex(LBS_DEB_CMD, "DATA_RATE_RESP", (u8 *) pdatarate,
		sizeof(struct cmd_ds_802_11_data_rate));

	/* FIXME: get actual rates FW can do if this command actually returns
	 * all data rates supported.
	 */
	adapter->cur_rate = libertas_fw_index_to_data_rate(pdatarate->rates[0]);
	lbs_deb_cmd("DATA_RATE: current rate 0x%02x\n", adapter->cur_rate);

	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_rf_channel(wlan_private * priv,
				      struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_rf_channel *rfchannel = &resp->params.rfchannel;
	wlan_adapter *adapter = priv->adapter;
	u16 action = le16_to_cpu(rfchannel->action);
	u16 newchannel = le16_to_cpu(rfchannel->currentchannel);

	lbs_deb_enter(LBS_DEB_CMD);

	if (action == CMD_OPT_802_11_RF_CHANNEL_GET
	    && adapter->curbssparams.channel != newchannel) {
		lbs_deb_cmd("channel switch from %d to %d\n",
		       adapter->curbssparams.channel, newchannel);

		/* Update the channel again */
		adapter->curbssparams.channel = newchannel;
	}

	lbs_deb_enter(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_rssi(wlan_private * priv,
				struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_rssi_rsp *rssirsp = &resp->params.rssirsp;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	/* store the non average value */
	adapter->SNR[TYPE_BEACON][TYPE_NOAVG] = le16_to_cpu(rssirsp->SNR);
	adapter->NF[TYPE_BEACON][TYPE_NOAVG] = le16_to_cpu(rssirsp->noisefloor);

	adapter->SNR[TYPE_BEACON][TYPE_AVG] = le16_to_cpu(rssirsp->avgSNR);
	adapter->NF[TYPE_BEACON][TYPE_AVG] = le16_to_cpu(rssirsp->avgnoisefloor);

	adapter->RSSI[TYPE_BEACON][TYPE_NOAVG] =
	    CAL_RSSI(adapter->SNR[TYPE_BEACON][TYPE_NOAVG],
		     adapter->NF[TYPE_BEACON][TYPE_NOAVG]);

	adapter->RSSI[TYPE_BEACON][TYPE_AVG] =
	    CAL_RSSI(adapter->SNR[TYPE_BEACON][TYPE_AVG] / AVG_SCALE,
		     adapter->NF[TYPE_BEACON][TYPE_AVG] / AVG_SCALE);

	lbs_deb_cmd("RSSI: beacon %d, avg %d\n",
	       adapter->RSSI[TYPE_BEACON][TYPE_NOAVG],
	       adapter->RSSI[TYPE_BEACON][TYPE_AVG]);

	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_802_11_eeprom_access(wlan_private * priv,
				  struct cmd_ds_command *resp)
{
	wlan_adapter *adapter = priv->adapter;
	struct wlan_ioctl_regrdwr *pbuf;
	pbuf = (struct wlan_ioctl_regrdwr *) adapter->prdeeprom;

	lbs_deb_enter_args(LBS_DEB_CMD, "len %d",
	       le16_to_cpu(resp->params.rdeeprom.bytecount));
	if (pbuf->NOB < le16_to_cpu(resp->params.rdeeprom.bytecount)) {
		pbuf->NOB = 0;
		lbs_deb_cmd("EEPROM read length too big\n");
		return -1;
	}
	pbuf->NOB = le16_to_cpu(resp->params.rdeeprom.bytecount);
	if (pbuf->NOB > 0) {

		memcpy(&pbuf->value, (u8 *) & resp->params.rdeeprom.value,
		       le16_to_cpu(resp->params.rdeeprom.bytecount));
		lbs_deb_hex(LBS_DEB_CMD, "EEPROM", (char *)&pbuf->value,
			le16_to_cpu(resp->params.rdeeprom.bytecount));
	}
	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int wlan_ret_get_log(wlan_private * priv,
			    struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_get_log *logmessage = &resp->params.glog;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_CMD);

	/* Stored little-endian */
	memcpy(&adapter->logmsg, logmessage, sizeof(struct cmd_ds_802_11_get_log));

	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static int libertas_ret_802_11_enable_rsn(wlan_private * priv,
                                          struct cmd_ds_command *resp)
{
	struct cmd_ds_802_11_enable_rsn *enable_rsn = &resp->params.enbrsn;
	wlan_adapter *adapter = priv->adapter;
	u32 * pdata_buf = adapter->cur_cmd->pdata_buf;

	lbs_deb_enter(LBS_DEB_CMD);

	if (enable_rsn->action == cpu_to_le16(CMD_ACT_GET)) {
		if (pdata_buf)
			*pdata_buf = (u32) le16_to_cpu(enable_rsn->enable);
	}

	lbs_deb_leave(LBS_DEB_CMD);
	return 0;
}

static inline int handle_cmd_response(u16 respcmd,
				      struct cmd_ds_command *resp,
				      wlan_private *priv)
{
	int ret = 0;
	unsigned long flags;
	wlan_adapter *adapter = priv->adapter;

	lbs_deb_enter(LBS_DEB_HOST);

	switch (respcmd) {
	case CMD_RET(CMD_MAC_REG_ACCESS):
	case CMD_RET(CMD_BBP_REG_ACCESS):
	case CMD_RET(CMD_RF_REG_ACCESS):
		ret = wlan_ret_reg_access(priv, respcmd, resp);
		break;

	case CMD_RET(CMD_GET_HW_SPEC):
		ret = wlan_ret_get_hw_spec(priv, resp);
		break;

	case CMD_RET(CMD_802_11_SCAN):
		ret = libertas_ret_80211_scan(priv, resp);
		break;

	case CMD_RET(CMD_802_11_GET_LOG):
		ret = wlan_ret_get_log(priv, resp);
		break;

	case CMD_RET_802_11_ASSOCIATE:
	case CMD_RET(CMD_802_11_ASSOCIATE):
	case CMD_RET(CMD_802_11_REASSOCIATE):
		ret = libertas_ret_80211_associate(priv, resp);
		break;

	case CMD_RET(CMD_802_11_DISASSOCIATE):
	case CMD_RET(CMD_802_11_DEAUTHENTICATE):
		ret = libertas_ret_80211_disassociate(priv, resp);
		break;

	case CMD_RET(CMD_802_11_AD_HOC_START):
	case CMD_RET(CMD_802_11_AD_HOC_JOIN):
		ret = libertas_ret_80211_ad_hoc_start(priv, resp);
		break;

	case CMD_RET(CMD_802_11_GET_STAT):
		ret = wlan_ret_802_11_stat(priv, resp);
		break;

	case CMD_RET(CMD_802_11_SNMP_MIB):
		ret = wlan_ret_802_11_snmp_mib(priv, resp);
		break;

	case CMD_RET(CMD_802_11_RF_TX_POWER):
		ret = wlan_ret_802_11_rf_tx_power(priv, resp);
		break;

	case CMD_RET(CMD_802_11_SET_AFC):
	case CMD_RET(CMD_802_11_GET_AFC):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		memmove(adapter->cur_cmd->pdata_buf, &resp->params.afc,
			sizeof(struct cmd_ds_802_11_afc));
		spin_unlock_irqrestore(&adapter->driver_lock, flags);

		break;

	case CMD_RET(CMD_MAC_MULTICAST_ADR):
	case CMD_RET(CMD_MAC_CONTROL):
	case CMD_RET(CMD_802_11_SET_WEP):
	case CMD_RET(CMD_802_11_RESET):
	case CMD_RET(CMD_802_11_AUTHENTICATE):
	case CMD_RET(CMD_802_11_RADIO_CONTROL):
	case CMD_RET(CMD_802_11_BEACON_STOP):
		break;

	case CMD_RET(CMD_802_11_ENABLE_RSN):
		ret = libertas_ret_802_11_enable_rsn(priv, resp);
		break;

	case CMD_RET(CMD_802_11_DATA_RATE):
		ret = wlan_ret_802_11_data_rate(priv, resp);
		break;
	case CMD_RET(CMD_802_11_RATE_ADAPT_RATESET):
		ret = wlan_ret_802_11_rate_adapt_rateset(priv, resp);
		break;
	case CMD_RET(CMD_802_11_RF_CHANNEL):
		ret = wlan_ret_802_11_rf_channel(priv, resp);
		break;

	case CMD_RET(CMD_802_11_RSSI):
		ret = wlan_ret_802_11_rssi(priv, resp);
		break;

	case CMD_RET(CMD_802_11_MAC_ADDRESS):
		ret = wlan_ret_802_11_mac_address(priv, resp);
		break;

	case CMD_RET(CMD_802_11_AD_HOC_STOP):
		ret = libertas_ret_80211_ad_hoc_stop(priv, resp);
		break;

	case CMD_RET(CMD_802_11_KEY_MATERIAL):
		ret = wlan_ret_802_11_key_material(priv, resp);
		break;

	case CMD_RET(CMD_802_11_EEPROM_ACCESS):
		ret = wlan_ret_802_11_eeprom_access(priv, resp);
		break;

	case CMD_RET(CMD_802_11D_DOMAIN_INFO):
		ret = libertas_ret_802_11d_domain_info(priv, resp);
		break;

	case CMD_RET(CMD_802_11_SLEEP_PARAMS):
		ret = wlan_ret_802_11_sleep_params(priv, resp);
		break;
	case CMD_RET(CMD_802_11_INACTIVITY_TIMEOUT):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		*((u16 *) adapter->cur_cmd->pdata_buf) =
		    le16_to_cpu(resp->params.inactivity_timeout.timeout);
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		break;

	case CMD_RET(CMD_802_11_TPC_CFG):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		memmove(adapter->cur_cmd->pdata_buf, &resp->params.tpccfg,
			sizeof(struct cmd_ds_802_11_tpc_cfg));
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		break;
	case CMD_RET(CMD_802_11_LED_GPIO_CTRL):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		memmove(adapter->cur_cmd->pdata_buf, &resp->params.ledgpio,
			sizeof(struct cmd_ds_802_11_led_ctrl));
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		break;
	case CMD_RET(CMD_802_11_PWR_CFG):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		memmove(adapter->cur_cmd->pdata_buf, &resp->params.pwrcfg,
			sizeof(struct cmd_ds_802_11_pwr_cfg));
		spin_unlock_irqrestore(&adapter->driver_lock, flags);

		break;

	case CMD_RET(CMD_GET_TSF):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		memcpy(priv->adapter->cur_cmd->pdata_buf,
		       &resp->params.gettsf.tsfvalue, sizeof(u64));
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		break;
	case CMD_RET(CMD_BT_ACCESS):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		if (adapter->cur_cmd->pdata_buf)
			memcpy(adapter->cur_cmd->pdata_buf,
			       &resp->params.bt.addr1, 2 * ETH_ALEN);
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		break;
	case CMD_RET(CMD_FWT_ACCESS):
		spin_lock_irqsave(&adapter->driver_lock, flags);
		if (adapter->cur_cmd->pdata_buf)
			memcpy(adapter->cur_cmd->pdata_buf, &resp->params.fwt,
			       sizeof(resp->params.fwt));
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		break;
	case CMD_RET(CMD_MESH_ACCESS):
		if (adapter->cur_cmd->pdata_buf)
			memcpy(adapter->cur_cmd->pdata_buf, &resp->params.mesh,
			       sizeof(resp->params.mesh));
		break;
	default:
		lbs_deb_host("CMD_RESP: unknown cmd response 0x%04x\n",
			    resp->command);
		break;
	}
	lbs_deb_leave(LBS_DEB_HOST);
	return ret;
}

int libertas_process_rx_command(wlan_private * priv)
{
	u16 respcmd;
	struct cmd_ds_command *resp;
	wlan_adapter *adapter = priv->adapter;
	int ret = 0;
	ulong flags;
	u16 result;

	lbs_deb_enter(LBS_DEB_HOST);

	/* Now we got response from FW, cancel the command timer */
	del_timer(&adapter->command_timer);

	mutex_lock(&adapter->lock);
	spin_lock_irqsave(&adapter->driver_lock, flags);

	if (!adapter->cur_cmd) {
		lbs_deb_host("CMD_RESP: cur_cmd is NULL\n");
		ret = -1;
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		goto done;
	}
	resp = (struct cmd_ds_command *)(adapter->cur_cmd->bufvirtualaddr);

	respcmd = le16_to_cpu(resp->command);
	result = le16_to_cpu(resp->result);

	lbs_deb_host("CMD_RESP: response 0x%04x, size %d, jiffies %lu\n",
		respcmd, priv->upld_len, jiffies);
	lbs_deb_hex(LBS_DEB_HOST, "CMD_RESP", adapter->cur_cmd->bufvirtualaddr,
		    priv->upld_len);

	if (!(respcmd & 0x8000)) {
		lbs_deb_host("invalid response!\n");
		adapter->cur_cmd_retcode = -1;
		__libertas_cleanup_and_insert_cmd(priv, adapter->cur_cmd);
		adapter->nr_cmd_pending--;
		adapter->cur_cmd = NULL;
		spin_unlock_irqrestore(&adapter->driver_lock, flags);
		ret = -1;
		goto done;
	}

	/* Store the response code to cur_cmd_retcode. */
	adapter->cur_cmd_retcode = result;;

	if (respcmd == CMD_RET(CMD_802_11_PS_MODE)) {
		struct cmd_ds_802_11_ps_mode *psmode = &resp->params.psmode;
		u16 action = le16_to_cpu(psmode->action);

		lbs_deb_host(
		       "CMD_RESP: PS_MODE cmd reply result 0x%x, action 0x%x\n",
		       result, action);

		if (result) {
			lbs_deb_host("CMD_RESP: PS command failed with 0x%x\n",
				    result);
			/*
			 * We should not re-try enter-ps command in
			 * ad-hoc mode. It takes place in
			 * libertas_execute_next_command().
			 */
			if (adapter->mode == IW_MODE_ADHOC &&
			    action == CMD_SUBCMD_ENTER_PS)
				adapter->psmode = WLAN802_11POWERMODECAM;
		} else if (action == CMD_SUBCMD_ENTER_PS) {
			adapter->needtowakeup = 0;
			adapter->psstate = PS_STATE_AWAKE;

			lbs_deb_host("CMD_RESP: ENTER_PS command response\n");
			if (adapter->connect_status != LIBERTAS_CONNECTED) {
				/*
				 * When Deauth Event received before Enter_PS command
				 * response, We need to wake up the firmware.
				 */
				lbs_deb_host(
				       "disconnected, invoking libertas_ps_wakeup\n");

				spin_unlock_irqrestore(&adapter->driver_lock, flags);
				mutex_unlock(&adapter->lock);
				libertas_ps_wakeup(priv, 0);
				mutex_lock(&adapter->lock);
				spin_lock_irqsave(&adapter->driver_lock, flags);
			}
		} else if (action == CMD_SUBCMD_EXIT_PS) {
			adapter->needtowakeup = 0;
			adapter->psstate = PS_STATE_FULL_POWER;
			lbs_deb_host("CMD_RESP: EXIT_PS command response\n");
		} else {
			lbs_deb_host("CMD_RESP: PS action 0x%X\n", action);
		}

		__libertas_cleanup_and_insert_cmd(priv, adapter->cur_cmd);
		adapter->nr_cmd_pending--;
		adapter->cur_cmd = NULL;
		spin_unlock_irqrestore(&adapter->driver_lock, flags);

		ret = 0;
		goto done;
	}

	if (adapter->cur_cmd->cmdflags & CMD_F_HOSTCMD) {
		/* Copy the response back to response buffer */
		memcpy(adapter->cur_cmd->pdata_buf, resp,
		       le16_to_cpu(resp->size));
		adapter->cur_cmd->cmdflags &= ~CMD_F_HOSTCMD;
	}

	/* If the command is not successful, cleanup and return failure */
	if ((result != 0 || !(respcmd & 0x8000))) {
		lbs_deb_host("CMD_RESP: error 0x%04x in command reply 0x%04x\n",
		       result, respcmd);
		/*
		 * Handling errors here
		 */
		switch (respcmd) {
		case CMD_RET(CMD_GET_HW_SPEC):
		case CMD_RET(CMD_802_11_RESET):
			lbs_deb_host("CMD_RESP: reset failed\n");
			break;

		}

		__libertas_cleanup_and_insert_cmd(priv, adapter->cur_cmd);
		adapter->nr_cmd_pending--;
		adapter->cur_cmd = NULL;
		spin_unlock_irqrestore(&adapter->driver_lock, flags);

		ret = -1;
		goto done;
	}

	spin_unlock_irqrestore(&adapter->driver_lock, flags);

	ret = handle_cmd_response(respcmd, resp, priv);

	spin_lock_irqsave(&adapter->driver_lock, flags);
	if (adapter->cur_cmd) {
		/* Clean up and Put current command back to cmdfreeq */
		__libertas_cleanup_and_insert_cmd(priv, adapter->cur_cmd);
		adapter->nr_cmd_pending--;
		WARN_ON(adapter->nr_cmd_pending > 128);
		adapter->cur_cmd = NULL;
	}
	spin_unlock_irqrestore(&adapter->driver_lock, flags);

done:
	mutex_unlock(&adapter->lock);
	lbs_deb_leave_args(LBS_DEB_HOST, "ret %d", ret);
	return ret;
}

int libertas_process_event(wlan_private * priv)
{
	int ret = 0;
	wlan_adapter *adapter = priv->adapter;
	u32 eventcause;

	lbs_deb_enter(LBS_DEB_CMD);

	spin_lock_irq(&adapter->driver_lock);
	eventcause = adapter->eventcause;
	spin_unlock_irq(&adapter->driver_lock);

	lbs_deb_cmd("event cause 0x%x\n", eventcause);

	switch (eventcause >> SBI_EVENT_CAUSE_SHIFT) {
	case MACREG_INT_CODE_LINK_SENSED:
		lbs_deb_cmd("EVENT: MACREG_INT_CODE_LINK_SENSED\n");
		break;

	case MACREG_INT_CODE_DEAUTHENTICATED:
		lbs_deb_cmd("EVENT: deauthenticated\n");
		libertas_mac_event_disconnected(priv);
		break;

	case MACREG_INT_CODE_DISASSOCIATED:
		lbs_deb_cmd("EVENT: disassociated\n");
		libertas_mac_event_disconnected(priv);
		break;

	case MACREG_INT_CODE_LINK_LOSE_NO_SCAN:
		lbs_deb_cmd("EVENT: link lost\n");
		libertas_mac_event_disconnected(priv);
		break;

	case MACREG_INT_CODE_PS_SLEEP:
		lbs_deb_cmd("EVENT: sleep\n");

		/* handle unexpected PS SLEEP event */
		if (adapter->psstate == PS_STATE_FULL_POWER) {
			lbs_deb_cmd(
			       "EVENT: in FULL POWER mode, ignoreing PS_SLEEP\n");
			break;
		}
		adapter->psstate = PS_STATE_PRE_SLEEP;

		libertas_ps_confirm_sleep(priv, (u16) adapter->psmode);

		break;

	case MACREG_INT_CODE_PS_AWAKE:
		lbs_deb_cmd("EVENT: awake\n");

		/* handle unexpected PS AWAKE event */
		if (adapter->psstate == PS_STATE_FULL_POWER) {
			lbs_deb_cmd(
			       "EVENT: In FULL POWER mode - ignore PS AWAKE\n");
			break;
		}

		adapter->psstate = PS_STATE_AWAKE;

		if (adapter->needtowakeup) {
			/*
			 * wait for the command processing to finish
			 * before resuming sending
			 * adapter->needtowakeup will be set to FALSE
			 * in libertas_ps_wakeup()
			 */
			lbs_deb_cmd("waking up ...\n");
			libertas_ps_wakeup(priv, 0);
		}
		break;

	case MACREG_INT_CODE_MIC_ERR_UNICAST:
		lbs_deb_cmd("EVENT: UNICAST MIC ERROR\n");
		handle_mic_failureevent(priv, MACREG_INT_CODE_MIC_ERR_UNICAST);
		break;

	case MACREG_INT_CODE_MIC_ERR_MULTICAST:
		lbs_deb_cmd("EVENT: MULTICAST MIC ERROR\n");
		handle_mic_failureevent(priv, MACREG_INT_CODE_MIC_ERR_MULTICAST);
		break;
	case MACREG_INT_CODE_MIB_CHANGED:
	case MACREG_INT_CODE_INIT_DONE:
		break;

	case MACREG_INT_CODE_ADHOC_BCN_LOST:
		lbs_deb_cmd("EVENT: ADHOC beacon lost\n");
		break;

	case MACREG_INT_CODE_RSSI_LOW:
		lbs_pr_alert("EVENT: rssi low\n");
		break;
	case MACREG_INT_CODE_SNR_LOW:
		lbs_pr_alert("EVENT: snr low\n");
		break;
	case MACREG_INT_CODE_MAX_FAIL:
		lbs_pr_alert("EVENT: max fail\n");
		break;
	case MACREG_INT_CODE_RSSI_HIGH:
		lbs_pr_alert("EVENT: rssi high\n");
		break;
	case MACREG_INT_CODE_SNR_HIGH:
		lbs_pr_alert("EVENT: snr high\n");
		break;

	case MACREG_INT_CODE_MESH_AUTO_STARTED:
		/* Ignore spurious autostart events if autostart is disabled */
		if (!priv->mesh_autostart_enabled) {
			lbs_pr_info("EVENT: MESH_AUTO_STARTED (ignoring)\n");
			break;
		}
		lbs_pr_info("EVENT: MESH_AUTO_STARTED\n");
		adapter->connect_status = LIBERTAS_CONNECTED;
		if (priv->mesh_open == 1) {
			netif_wake_queue(priv->mesh_dev);
			netif_carrier_on(priv->mesh_dev);
		}
		adapter->mode = IW_MODE_ADHOC;
		schedule_work(&priv->sync_channel);
		break;

	default:
		lbs_pr_alert("EVENT: unknown event id 0x%04x\n",
		       eventcause >> SBI_EVENT_CAUSE_SHIFT);
		break;
	}

	spin_lock_irq(&adapter->driver_lock);
	adapter->eventcause = 0;
	spin_unlock_irq(&adapter->driver_lock);

	lbs_deb_leave_args(LBS_DEB_CMD, "ret %d", ret);
	return ret;
}
