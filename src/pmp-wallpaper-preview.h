/*
 * Copyright © 2019 Red Hat, Inc
 *             2023-2024 The Phosh Developers
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Heavily based on the xdg-desktop-portal-gnome
 *
 * Authors:
 *       Felipe Borges <feborges@redhat.com>
 *       Guido Günther <agx@sigxcpu.org>
 */

#pragma once

#include <gtk/gtk.h>

#define PMP_TYPE_WALLPAPER_PREVIEW (pmp_wallpaper_preview_get_type ())
G_DECLARE_FINAL_TYPE (PmpWallpaperPreview, pmp_wallpaper_preview, PMP, WALLPAPER_PREVIEW, GtkBox);

PmpWallpaperPreview *pmp_wallpaper_preview_new (void);
void                 pmp_wallpaper_preview_set_image (PmpWallpaperPreview *self,
                                                      const gchar         *image_uri);
