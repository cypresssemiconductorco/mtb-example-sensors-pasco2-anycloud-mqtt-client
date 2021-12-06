/******************************************************************************
 * File Name:   mqtt_client_config.c
 *
 * Description: This file contains the configuration structures used by
 *              the MQTT client for MQTT connect operation.
 *
 * Related Document: See README.md
 *
 * ===========================================================================
 * Copyright (C) 2021 Infineon Technologies AG. All rights reserved.
 * ===========================================================================
 *
 * ===========================================================================
 * Infineon Technologies AG (INFINEON) is supplying this file for use
 * exclusively with Infineon's sensor products. This file can be freely
 * distributed within development tools and software supporting such
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON
 * WHATSOEVER.
 * ===========================================================================
 */

#include <stdio.h>
#include "mqtt_client_config.h"
#include "cy_mqtt_api.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
/* MQTT Broker/Server details */
cy_mqtt_broker_info_t broker_info =
{
    .hostname = MQTT_BROKER_ADDRESS,
    .hostname_len = sizeof(MQTT_BROKER_ADDRESS) - 1,
    .port = MQTT_PORT
};

#if (MQTT_SECURE_CONNECTION)
/* MQTT client credentials to be used in case of a secure connection. */
static cy_awsport_ssl_credentials_t credentials =
{
    /* Configure the client certificate. */
#ifdef CLIENT_CERTIFICATE
    .client_cert = (const char *)CLIENT_CERTIFICATE,
    .client_cert_size = sizeof(CLIENT_CERTIFICATE),
#else
    .client_cert = NULL,
    .client_cert_size = 0,
#endif

    /* Configure the client private key. */
#ifdef CLIENT_PRIVATE_KEY
    .private_key = (const char *)CLIENT_PRIVATE_KEY,
    .private_key_size = sizeof(CLIENT_PRIVATE_KEY),
#else
    .private_key = NULL,
    .private_key_size = 0,
#endif

    /* Configure the Root CA certificate of the MQTT Broker/Server. */
#ifdef ROOT_CA_CERTIFICATE
    .root_ca = (const char *)ROOT_CA_CERTIFICATE,
    .root_ca_size = sizeof(ROOT_CA_CERTIFICATE),
#else
    .root_ca = NULL,
    .root_ca_size = 0,
#endif

    /* Application Layer Protocol Negotiation (ALPN) is used to implement
     * MQTT with TLS Client Authentication from client devices.
     */
#ifdef MQTT_ALPN_PROTOCOL_NAME
    .alpnprotos = (const char *)MQTT_ALPN_PROTOCOL_NAME,
    .alpnprotoslen = sizeof(MQTT_ALPN_PROTOCOL_NAME),
#else
    .alpnprotos = NULL,
    .alpnprotoslen = 0,
#endif

    /* Server Name Indication (SNI) hostname to be sent as a part of the
     * Client Hello message sent during TLS handshake as specified by the
     * MQTT Broker.
     */
#ifdef MQTT_SNI_HOSTNAME
    .sni_host_name = (const char *)MQTT_SNI_HOSTNAME,
    .sni_host_name_size = sizeof(MQTT_SNI_HOSTNAME)
#else
    .sni_host_name = NULL,
    .sni_host_name_size = 0
#endif
};

/* Pointer to the security details of the MQTT connection. */
cy_awsport_ssl_credentials_t *security_info = &credentials;

#else
/* Pointer to the security details of the MQTT connection. */
cy_awsport_ssl_credentials_t *security_info = NULL;
#endif /* #if (MQTT_SECURE_CONNECTION) */

#if ENABLE_LWT_MESSAGE
/* Last Will and Testament (LWT) message structure. The MQTT broker will
 * publish the LWT message if this client disconnects unexpectedly.
 */
static cy_mqtt_publish_info_t will_msg_info =
{
    .qos = CY_MQTT_QOS2,
    .topic = MQTT_WILL_TOPIC_NAME,
    .topic_len = (uint16_t)(sizeof(MQTT_WILL_TOPIC_NAME) - 1),
    .payload = MQTT_WILL_MESSAGE,
    .payload_len = (size_t)(sizeof(MQTT_WILL_MESSAGE) - 1),
    .retain = false,
    .dup = false
};
#endif /* ENABLE_LWT_MESSAGE */

/* MQTT connection information structure */
cy_mqtt_connect_info_t connection_info =
{
    .client_id = NULL,
    .client_id_len = 0,
    .username = NULL,
    .username_len = 0,
    .password = NULL,
    .password_len = 0,
    .clean_session = true,
    .keep_alive_sec = MQTT_KEEP_ALIVE_SECONDS,
#if ENABLE_LWT_MESSAGE
    .will_info = &will_msg_info
#else
    .will_info = NULL
#endif /* ENABLE_LWT_MESSAGE */
};

/* Check for a valid QoS setting - QoS 0, QoS 1, or QoS 2. */
#if ((MQTT_MESSAGES_QOS != 0) && (MQTT_MESSAGES_QOS != 1) && (MQTT_MESSAGES_QOS != 2))
    #error "Invalid QoS setting! MQTT_MESSAGES_QOS must be either 0 or 1."
#endif


/* [] END OF FILE */
