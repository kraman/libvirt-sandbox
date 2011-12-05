/*
 * libvirt-sandbox-config-network-address.c: libvirt sandbox configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>
#include <string.h>
#include <sys/utsname.h>

#include "libvirt-sandbox/libvirt-sandbox.h"

/**
 * SECTION: libvirt-sandbox-config-network_address
 * @short_description: Address information for an interface
 * @include: libvirt-sandbox/libvirt-sandbox.h
 *
 * Provides an object to store information about a network interface address
 *
 * The GVirSandboxConfigNetworkAddress object stores the information required
 * to configure an address on a network interface. This compises the primary
 * address, broadcast address and network address.
 */

#define GVIR_SANDBOX_CONFIG_NETWORK_ADDRESS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_SANDBOX_TYPE_CONFIG_NETWORK_ADDRESS, GVirSandboxConfigNetworkAddressPrivate))

struct _GVirSandboxConfigNetworkAddressPrivate
{
    GInetAddress *primary;
    GInetAddress *broadcast;
    GInetAddress *netmask;
};

G_DEFINE_TYPE(GVirSandboxConfigNetworkAddress, gvir_sandbox_config_network_address, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_PRIMARY,
    PROP_BROADCAST,
    PROP_NETMASK,
};

enum {
    LAST_SIGNAL
};

//static gint signals[LAST_SIGNAL];


static void gvir_sandbox_config_network_address_get_property(GObject *object,
                                                             guint prop_id,
                                                             GValue *value,
                                                             GParamSpec *pspec)
{
    GVirSandboxConfigNetworkAddress *config = GVIR_SANDBOX_CONFIG_NETWORK_ADDRESS(object);
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;

    switch (prop_id) {
    case PROP_PRIMARY:
        g_value_set_object(value, priv->primary);
        break;

    case PROP_BROADCAST:
        g_value_set_object(value, priv->broadcast);
        break;

    case PROP_NETMASK:
        g_value_set_object(value, priv->netmask);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_sandbox_config_network_address_set_property(GObject *object,
                                                             guint prop_id,
                                                             const GValue *value,
                                                             GParamSpec *pspec)
{
    GVirSandboxConfigNetworkAddress *config = GVIR_SANDBOX_CONFIG_NETWORK_ADDRESS(object);
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;

    switch (prop_id) {
    case PROP_PRIMARY:
        if (priv->primary)
            g_object_unref(priv->primary);
        priv->primary = g_value_dup_object(value);
        break;

    case PROP_BROADCAST:
        if (priv->broadcast)
            g_object_unref(priv->broadcast);
        priv->broadcast = g_value_dup_object(value);
        break;

    case PROP_NETMASK:
        if (priv->netmask)
            g_object_unref(priv->netmask);
        priv->netmask = g_value_dup_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_sandbox_config_network_address_finalize(GObject *object)
{
    GVirSandboxConfigNetworkAddress *config = GVIR_SANDBOX_CONFIG_NETWORK_ADDRESS(object);
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;

    if (priv->primary)
        g_object_unref(priv->primary);
    if (priv->netmask)
        g_object_unref(priv->netmask);
    if (priv->broadcast)
        g_object_unref(priv->broadcast);

    G_OBJECT_CLASS(gvir_sandbox_config_network_address_parent_class)->finalize(object);
}


static void gvir_sandbox_config_network_address_class_init(GVirSandboxConfigNetworkAddressClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_sandbox_config_network_address_finalize;
    object_class->get_property = gvir_sandbox_config_network_address_get_property;
    object_class->set_property = gvir_sandbox_config_network_address_set_property;

    g_object_class_install_property(object_class,
                                    PROP_PRIMARY,
                                    g_param_spec_object("primary",
                                                        "Primary",
                                                        "Primary address",
                                                        G_TYPE_INET_ADDRESS,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));
    g_object_class_install_property(object_class,
                                    PROP_BROADCAST,
                                    g_param_spec_object("broadcast",
                                                        "Broadcast",
                                                        "Broadcast address",
                                                        G_TYPE_INET_ADDRESS,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));
    g_object_class_install_property(object_class,
                                    PROP_NETMASK,
                                    g_param_spec_object("netmask",
                                                        "Netmask",
                                                        "Netmask address",
                                                        G_TYPE_INET_ADDRESS,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirSandboxConfigNetworkAddressPrivate));
}


static void gvir_sandbox_config_network_address_init(GVirSandboxConfigNetworkAddress *config)
{
    config->priv = GVIR_SANDBOX_CONFIG_NETWORK_ADDRESS_GET_PRIVATE(config);
}


/**
 * gvir_sandbox_config_network_address_new:
 * @primary: the primary address
 * @netmask: the netmask address (optional)
 * @broadcast: the broadcast address (optional)
 *
 * Create a new network address config. Only the @primary parameter
 * is required to be non-NULL. The @netmask and @broadcast addresses
 * will be automatically filled in, if not otherwise specified
 *
 * Returns: (transfer full): a new sandbox network_address object
 */
GVirSandboxConfigNetworkAddress *gvir_sandbox_config_network_address_new(GInetAddress *primary,
                                                                         GInetAddress *network,
                                                                         GInetAddress *broadcast)
{
    return GVIR_SANDBOX_CONFIG_NETWORK_ADDRESS(g_object_new(GVIR_SANDBOX_TYPE_CONFIG_NETWORK_ADDRESS,
                                                            "primary", primary,
                                                            "network", network,
                                                            "broadcast", broadcast,
                                                            NULL));
}


/**
 * gvir_sandbox_config_network_address_set_primary:
 * @config: (transfer none): the sandbox network address config
 * @addr: (transfer none): the primary address
 *
 * Sets the interface primary address
 */
void gvir_sandbox_config_network_address_set_primary(GVirSandboxConfigNetworkAddress *config,
                                                     GInetAddress *addr)
{
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;
    if (priv->primary)
        g_object_unref(priv->primary);
    priv->primary = g_object_ref(addr);
}


/**
 * gvir_sandbox_config_network_address_get_primary:
 * @config: (transfer none): the sandbox network address config
 *
 * Retrieves the primary address
 *
 * Returns: (transfer none): the primary address
 */
GInetAddress *gvir_sandbox_config_network_address_get_primary(GVirSandboxConfigNetworkAddress *config)
{
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;
    return priv->primary;
}


/**
 * gvir_sandbox_config_network_address_set_network:
 * @config: (transfer none): the sandbox network address config
 * @addr: (transfer none): the network address
 *
 * Sets the interface network address
 */
void gvir_sandbox_config_network_address_set_netmask(GVirSandboxConfigNetworkAddress *config,
                                                     GInetAddress *addr)
{
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;
    if (priv->netmask)
        g_object_unref(priv->netmask);
    priv->netmask = g_object_ref(addr);
}


/**
 * gvir_sandbox_config_network_address_get_netmask:
 * @config: (transfer none): the sandbox network address config
 *
 * Retrieves the network address
 *
 * Returns: (transfer none): the network address
 */
GInetAddress *gvir_sandbox_config_network_address_get_netmask(GVirSandboxConfigNetworkAddress *config)
{
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;
    return priv->netmask;
}


/**
 * gvir_sandbox_config_network_address_set_broadcast:
 * @config: (transfer none): the sandbox network address config
 * @addr: (transfer none): the broadcast address
 *
 * Sets the interface broadcast address
 */
void gvir_sandbox_config_network_address_set_broadcast(GVirSandboxConfigNetworkAddress *config,
                                                       GInetAddress *addr)
{
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;
    if (priv->broadcast)
        g_object_unref(priv->broadcast);
    priv->broadcast = g_object_ref(addr);
}


/**
 * gvir_sandbox_config_network_address_get_broadcast:
 * @config: (transfer none): the sandbox network address config
 *
 * Retrieves the broadcast address
 *
 * Returns: (transfer none): the broadcast address
 */
GInetAddress *gvir_sandbox_config_network_address_get_broadcast(GVirSandboxConfigNetworkAddress *config)
{
    GVirSandboxConfigNetworkAddressPrivate *priv = config->priv;
    return priv->broadcast;
}