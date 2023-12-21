/*
 * Copyright © 2019 Red Hat, Inc
 * Copyright © 2023 Guido Günther
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Heavily based on the xdg-desktop-portal-gnome
 *
 * Authors:
 *       Felipe Borges <feborges@redhat.com>
 *       Guido Günther <agx@sigxcpu.org>
 */

#include "pmp-config.h"

#include <string.h>

#include <gio/gio.h>
#include <glib/gi18n.h>

#include "pmp-wallpaper-dialog.h"
#include "pmp-wallpaper-preview.h"


enum {
  RESPONSE,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

struct _PmpWallpaperDialog {
  AdwWindow            parent;

  GtkWidget           *stack;
  PmpWallpaperPreview *desktop_preview;

  gchar               *picture_uri;
};

G_DEFINE_TYPE (PmpWallpaperDialog, pmp_wallpaper_dialog, ADW_TYPE_WINDOW)

static void
pmp_wallpaper_dialog_apply (PmpWallpaperDialog *self)
{
  g_signal_emit (self, signals[RESPONSE], 0, GTK_RESPONSE_APPLY);
}

static void
pmp_wallpaper_dialog_cancel (PmpWallpaperDialog *self)
{
  g_signal_emit (self, signals[RESPONSE], 0, GTK_RESPONSE_CANCEL);
}

static void
pmp_wallpaper_dialog_finalize (GObject *object)
{
  PmpWallpaperDialog *self = PMP_WALLPAPER_DIALOG (object);

  g_clear_pointer (&self->picture_uri, g_free);

  G_OBJECT_CLASS (pmp_wallpaper_dialog_parent_class)->finalize (object);
}

static void
pmp_wallpaper_dialog_init (PmpWallpaperDialog *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
pmp_wallpaper_dialog_class_init (PmpWallpaperDialogClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = pmp_wallpaper_dialog_finalize;

  signals[RESPONSE] = g_signal_new ("response",
                                    G_TYPE_FROM_CLASS (klass),
                                    G_SIGNAL_ACTION | G_SIGNAL_RUN_LAST,
                                    0,
                                    NULL, NULL,
                                    NULL,
                                    G_TYPE_NONE, 1, G_TYPE_INT);

  g_type_ensure (PMP_TYPE_WALLPAPER_PREVIEW);

  gtk_widget_class_set_template_from_resource (widget_class,
                                               "/mobi/phosh/portal/pmp-wallpaper-dialog.ui");
  gtk_widget_class_bind_template_child (widget_class, PmpWallpaperDialog, stack);
  gtk_widget_class_bind_template_child (widget_class, PmpWallpaperDialog, desktop_preview);

  gtk_widget_class_bind_template_callback (widget_class, pmp_wallpaper_dialog_cancel);
  gtk_widget_class_bind_template_callback (widget_class, pmp_wallpaper_dialog_apply);
}

static void
on_image_loaded_cb (GObject      *source_object,
                    GAsyncResult *result,
                    gpointer      data)
{
  PmpWallpaperDialog *self = data;
  GFileIOStream *stream = NULL;
  GFile *image_file = G_FILE (source_object);
  g_autoptr (GFile) tmp = g_file_new_tmp ("XXXXXX", &stream, NULL);
  g_autoptr (GError) error = NULL;
  gchar *contents = NULL;
  gsize length = 0;

  g_object_unref (stream);

  if (!g_file_load_contents_finish (image_file, result, &contents, &length, NULL, &error)) {
    g_warning ("Failed to load image: %s", error->message);

    return;
  }

  if (!g_file_replace_contents (tmp, contents, length, NULL, FALSE,
                                G_FILE_CREATE_REPLACE_DESTINATION, NULL, NULL, &error)) {
    g_warning ("Failed to store image: %s", error->message);
    return;
  }

  self->picture_uri = g_file_get_uri (tmp);
  pmp_wallpaper_preview_set_image (self->desktop_preview, self->picture_uri);
}

PmpWallpaperDialog *
pmp_wallpaper_dialog_new (const gchar *picture_uri, const gchar *app_id)
{
  PmpWallpaperDialog *self;
  g_autoptr (GFile) image_file = g_file_new_for_uri (picture_uri);

  self = g_object_new (PMP_WALLPAPER_TYPE_DIALOG, NULL);

  g_file_load_contents_async (image_file,
                              NULL,
                              on_image_loaded_cb,
                              self);

  return self;
}

const gchar *
pmp_wallpaper_dialog_get_uri (PmpWallpaperDialog *dialog)
{
  return dialog->picture_uri;
}
