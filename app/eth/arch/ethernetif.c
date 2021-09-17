////////////////////////////////////////////////////////////////////////////////
/// @file     ethernetif.c
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  1.0.0
/// @date     2021-04-21
/// @brief    THIS FILE PROVIDES ALL THE ethernetif.c EXAMPLE.
/// ////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2021 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _ETHERNETIF_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "netif/ethernet.h"
#include "lwip/netif.h"
#include "lwip/etharp.h"

#include "hal_eth.h"
#include "bsp_eth.h"
#include "bsp_phy8720.h"
#include "ethernetif.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Application_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Application_Ethernet_Interface
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Ethernet_Interface_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Network interface low-level initialization function.
/// @param  netif: The structure of the network interface.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void low_level_init(struct netif* netif)
{
    // BSP_ETH_Configure();

    if (EthStatus & ETH_LINK_FLAG) {
        netif->flags |= NETIF_FLAG_LINK_UP;  // Set netif link flag
    }

    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TX_BUF_NUM);  // Initialize Tx Descriptors list: Chain Mode
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RX_BUF_NUM);  // Initialize Rx Descriptors list: Chain Mode

    netif->hwaddr_len = ETH_HWADDR_LEN;  // Set MAC hardware address length

    memcpy(netif->hwaddr, gMac, 6);  // Set MAC hardware address

    ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr);  // Initialize MAC address in ethernet MAC

    netif->mtu = NETIF_MTU;  // Network interface maximum transfer unit

    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    ETH_NVIC_Config();

    ETH_Start();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Network interface low-level output frame function.
/// @param  netif: The structure of the network interface.
/// @param  p: Pointer to a data buffer structure.
/// @retval err_t: Error code.
////////////////////////////////////////////////////////////////////////////////
static err_t low_level_output(struct netif* netif, struct pbuf* p)
{
    err_t        errval;
    struct pbuf* q;

    uint8_t* buffer = (uint8_t*)(DMATxDescToSet->BUF1ADDR);

    __IO ETH_DMADESCTypeDef* DmaTxDesc;
    uint32_t                 framelength     = 0;
    uint32_t                 bufferoffset    = 0;
    uint32_t                 byteslefttocopy = 0;
    uint32_t                 payloadoffset   = 0;
    DmaTxDesc                                = DMATxDescToSet;
    bufferoffset                             = 0;

    // copy frame from pbufs to driver buffers
    for (q = p; q != NULL; q = q->next) {
        if ((DmaTxDesc->CS & ETH_DMA_TDES_OWN)) {
            errval = ERR_USE;
            goto error;
        }

        // Get bytes in current lwIP buffer
        byteslefttocopy = q->len;
        payloadoffset   = 0;

        // Check if the length of data to copy is bigger than Tx buffer size
        while ((byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE) {
            // Copy data to Tx buffer
            memcpy((uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset),
                   (ETH_TX_BUF_SIZE - bufferoffset));

            // Point to next descriptor
            DmaTxDesc = (ETH_DMADESCTypeDef*)(DmaTxDesc->BUF2NDADDR);

            // Check if the buffer is available
            if ((DmaTxDesc->CS & ETH_DMA_TDES_OWN)) {
                errval = ERR_USE;
                goto error;
            }

            buffer = (uint8_t*)(DmaTxDesc->BUF1ADDR);

            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset   = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength     = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset    = 0;
        }

        // Copy the remaining bytes
        memcpy((uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy);
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength  = framelength + byteslefttocopy;
    }

    // Prepare transmit descriptors to give to DMA
    ETH_Prepare_Transmit_Descriptors(framelength);

    errval = ERR_OK;

error:
    if (ETH->DMASR & ETH_DMASR_TUS) {
        // Clear TUS ETHERNET DMA flag
        ETH->DMASR = ETH_DMASR_TUS;

        // Resume DMA transmission
        ETH->DMATPDR = 0;
    }
    return errval;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Network interface low-level input frame function.
/// @param  netif: The structure of the network interface.
/// @retval pbuf: Pointer to a data buffer structure.
////////////////////////////////////////////////////////////////////////////////
static struct pbuf* low_level_input(struct netif* netif)
{
    struct pbuf* p   = NULL;
    struct pbuf* q   = NULL;
    uint16_t     len = 0;
    FrameTypeDef frame;
    uint8_t*     buffer;
    __IO ETH_DMADESCTypeDef* dmarxdesc;
    uint32_t                 bufferoffset    = 0;
    uint32_t                 payloadoffset   = 0;
    uint32_t                 byteslefttocopy = 0;
    uint32_t                 i               = 0;

    if (!ETH_CheckFrameReceived())
        return p;

    // get received frame
    frame = ETH_Get_Received_Frame();

    // Obtain the size of the packet and put it into the "len" variable.
    len    = frame.len;
    buffer = (u8*)frame.buf;

    // PRINT_INFO("receive frame %d len buffer : %s\n", len, buffer);
    if (len > 0) {
        /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }

    if (p != NULL) {
        dmarxdesc    = frame.ptrDesc;
        bufferoffset = 0;

        for (q = p; q != NULL; q = q->next) {
            byteslefttocopy = q->len;
            payloadoffset   = 0;

            while ((byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE) {
                // Copy data to pbuf
                memcpy((uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset),
                       (ETH_RX_BUF_SIZE - bufferoffset));

                // Point to next descriptor
                dmarxdesc = (ETH_DMADESCTypeDef*)(dmarxdesc->BUF2NDADDR);
                buffer    = (uint8_t*)(dmarxdesc->BUF1ADDR);

                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset   = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset    = 0;
            }

            // Copy remaining data in pbuf
            memcpy((uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset),
                   byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }
    else {
        return NULL;
    }

    // Release descriptors to DMA
    // Point to first descriptor
    dmarxdesc = frame.ptrDesc;

    // Set Own bit in Rx descriptors: gives the buffers back to DMA
    for (i = 0; i < DMA_RX_FRAME_infos->cnt; i++) {
        dmarxdesc->CS |= ETH_DMA_RDES_OWN;
        dmarxdesc = (ETH_DMADESCTypeDef*)(dmarxdesc->BUF2NDADDR);
    }

    // Clear Segment_Count
    DMA_RX_FRAME_infos->cnt = 0;

    // When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((ETH->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET) {
        /* Clear RBUS ETHERNET DMA flag */
        ETH->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        ETH->DMARPDR = 0;
    }

    return p;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Network interface input frame function.
/// @param  pParams: Some parameters.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ethernetif_input(struct netif* netif)
{
    err_t        err = ERR_OK;
    struct pbuf* p   = NULL;

    do {
        // move received packet into a new pbuf
        p = low_level_input(netif);

        // no packet could be read, silently ignore this
        if (p == NULL) {
            return;
        }

        // entry point to the LwIP stack
        err = netif->input(p, netif);

        if (err != ERR_OK) {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
            p = NULL;
        }
    } while (err == ERR_OK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Network interface initialization function.
/// @param  pParams: Some parameters.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
err_t ethernetif_init(struct netif* netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    netif->hostname = "lwip";  // Initialize interface hostname
#endif                         // LWIP_NETIF_HOSTNAME

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    netif->output = etharp_output;

    netif->linkoutput = low_level_output;

    low_level_init(netif);  // Initialize the hardware

    return ERR_OK;
}

/// @}

/// @}

/// @}
