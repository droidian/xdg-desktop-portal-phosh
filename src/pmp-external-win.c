/*
 * Copyright © 2023 Guido Günther
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Authors:
 *       Guido Günther <agx@sigxcpu.org>
 */


#include "pmp-config.h"

#include "pmp-external-win.h"

#include <gdk/wayland/gdkwayland.h>


struct _PmpExternalWin {
  GObject     parent;

  char       *xdg_foreign_handle;
  GdkSurface *parent_surface;
};
G_DEFINE_TYPE (PmpExternalWin, pmp_external_win, G_TYPE_OBJECT)


static void
pmp_external_win_dispose (GObject *object)
{
  PmpExternalWin *self = PMP_EXTERNAL_WIN (object);

  g_clear_pointer (&self->xdg_foreign_handle, g_free);
  g_clear_object (&self->parent_surface);

  G_OBJECT_CLASS (pmp_external_win_parent_class)->dispose (object);
}


static void
pmp_external_win_class_init (PmpExternalWinClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = pmp_external_win_dispose;
}


static void
pmp_external_win_init (PmpExternalWin *self)
{
}


#define HANDLE_PREFIX_WAYLAND "wayland:"
#define HANDLE_PREFIX_X11 "x11:"

PmpExternalWin *
pmp_external_win_new_from_handle (const char *handle_str)
{
  PmpExternalWin *self = NULL;

  if (!handle_str)
    return NULL;

  if (strlen (handle_str) == 0)
    return NULL;

  self = g_object_new (PMP_TYPE_EXTERNAL_WIN, NULL);

  if (g_str_has_prefix (handle_str, HANDLE_PREFIX_WAYLAND)) {
    self->xdg_foreign_handle = g_strdup (handle_str + strlen (HANDLE_PREFIX_WAYLAND));
  } else if (g_str_has_prefix (handle_str, HANDLE_PREFIX_X11)) {
    g_warning ("Handling X11 parents not yet implemented: '%s'", handle_str);
    return NULL;
  } else {
    g_warning ("Unknown external window handle string: '%s'", handle_str);
    return NULL;
  }

  return g_steal_pointer (&self);
}


void
pmp_external_win_set_parent_of (PmpExternalWin *self, GdkSurface *surface)
{
  GdkToplevel *toplevel;

  g_clear_object (&self->parent_surface);

  toplevel = GDK_TOPLEVEL (surface);

  if (!gdk_wayland_toplevel_set_transient_for_exported (toplevel, self->xdg_foreign_handle)) {
    g_warning ("Failed to set portal window transient for external parent");
    return;
  }

  g_set_object (&self->parent_surface, surface);
}
