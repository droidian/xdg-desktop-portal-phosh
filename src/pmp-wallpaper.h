/*
 * Copyright © 2019 Red Hat, Inc
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Authors:
 *       Felipe Borges <feborges@redhat.com>
 *       Guido Günther <agx@sigxcpu.org>
 */

#pragma once

#include <gio/gio.h>

G_BEGIN_DECLS

gboolean pmp_wallpaper_init (GDBusConnection *bus, GError **error);

G_END_DECLS
