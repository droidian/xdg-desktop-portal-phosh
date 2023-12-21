/*
 * Copyright © 2023 Guido Günther
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Authors:
 *       Guido Günther <agx@sigxcpu.org>
 */

#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define PMP_TYPE_EXTERNAL_WIN (pmp_external_win_get_type ())

G_DECLARE_FINAL_TYPE (PmpExternalWin, pmp_external_win, PMP, EXTERNAL_WIN, GObject)

PmpExternalWin *pmp_external_win_new_from_handle (const char *handle_str);
void pmp_external_win_set_parent_of (PmpExternalWin *self, GdkSurface *surface);

G_END_DECLS
