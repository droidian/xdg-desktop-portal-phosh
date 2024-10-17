/*
 * Copyright © 2019 Red Hat, Inc
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Authors:
 *       Matthias Clasen <mclasen@redhat.com>
 */

#pragma once

#include <gio/gio.h>

G_BEGIN_DECLS

gboolean pmp_settings_init (GDBusConnection *bus, GError **error);

G_END_DECLS
