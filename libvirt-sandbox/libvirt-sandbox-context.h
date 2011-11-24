/*
 * libvirt-sandbox-context.h: libvirt sandbox context
 *
 * Copyright (C) 2011 Red Hat
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

#if !defined(__LIBVIRT_SANDBOX_H__) && !defined(LIBVIRT_SANDBOX_BUILD)
#error "Only <libvirt-sandbox/libvirt-sandbox.h> can be included directly."
#endif

#ifndef __LIBVIRT_SANDBOX_CONTEXT_H__
#define __LIBVIRT_SANDBOX_CONTEXT_H__

G_BEGIN_DECLS

#define GVIR_SANDBOX_TYPE_CONTEXT            (gvir_sandbox_context_get_type ())
#define GVIR_SANDBOX_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_SANDBOX_TYPE_CONTEXT, GVirSandboxContext))
#define GVIR_SANDBOX_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_SANDBOX_TYPE_CONTEXT, GVirSandboxContextClass))
#define GVIR_SANDBOX_IS_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_SANDBOX_TYPE_CONTEXT))
#define GVIR_SANDBOX_IS_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_SANDBOX_TYPE_CONTEXT))
#define GVIR_SANDBOX_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_SANDBOX_TYPE_CONTEXT, GVirSandboxContextClass))

#define GVIR_SANDBOX_TYPE_CONTEXT_HANDLE      (gvir_sandbox_context_handle_get_type ())

typedef struct _GVirSandboxContext GVirSandboxContext;
typedef struct _GVirSandboxContextPrivate GVirSandboxContextPrivate;
typedef struct _GVirSandboxContextClass GVirSandboxContextClass;

struct _GVirSandboxContext
{
    GObject parent;

    GVirSandboxContextPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirSandboxContextClass
{
    GObjectClass parent_class;

};

GType gvir_sandbox_context_get_type(void);

GVirSandboxContext *gvir_sandbox_context_new(GVirConnection *connection,
                                             GVirSandboxConfig *config);

GVirSandboxConfig *gvir_sandbox_context_get_config(GVirSandboxContext *ctxt);
GVirSandboxCleaner *gvir_sandbox_context_get_cleaner(GVirSandboxContext *ctxt);

gboolean gvir_sandbox_context_start(GVirSandboxContext *ctxt, GError **error);
gboolean gvir_sandbox_context_stop(GVirSandboxContext *ctxt, GError **error);

GVirDomain *gvir_sandbox_context_get_domain(GVirSandboxContext *ctxt);
GVirConnection *gvir_sandbox_context_get_connection(GVirSandboxContext *ctxt);

GVirSandboxConsole *gvir_sandbox_context_get_console(GVirSandboxContext *ctxt);

G_END_DECLS

#endif /* __LIBVIRT_SANDBOX_CONTEXT_H__ */
