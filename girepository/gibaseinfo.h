/* GObject introspection: GIBaseInfo
 *
 * Copyright (C) 2005 Matthias Clasen
 * Copyright (C) 2008,2009 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GIBASEINFO_H__
#define __GIBASEINFO_H__

#if !defined (__GIREPOSITORY_H_INSIDE__) && !defined (GI_COMPILATION)
#error "Only <girepository.h> can be included directly."
#endif

#include <glib-object.h>
#include <gitypelib.h>
#include <gitypes.h>

G_BEGIN_DECLS

struct _GIBaseInfoStub {
  /* <private> */
  gint32 dummy1;
  gint32 dummy2;
  gpointer dummy3;
  gpointer dummy4;
  gpointer dummy5;
  guint32  dummy6;
  guint32  dummy7;
  gpointer padding[4];
};

/* GIBaseInfo */

/**
 * GIAttributeIter:
 *
 * An opaque structure used to iterate over attributes
 * in a #GIBaseInfo struct.
 */
typedef struct {
  /* <private> */
  gpointer data;
  gpointer data2;
  gpointer data3;
  gpointer data4;
} GIAttributeIter;

GIBaseInfo *           g_base_info_ref              (GIBaseInfo   *info);
void                   g_base_info_unref            (GIBaseInfo   *info);
GIInfoType             g_base_info_get_type         (GIBaseInfo   *info);
const gchar *          g_base_info_get_name         (GIBaseInfo   *info);
const gchar *          g_base_info_get_namespace    (GIBaseInfo   *info);
gboolean               g_base_info_is_deprecated    (GIBaseInfo   *info);
const gchar *          g_base_info_get_attribute    (GIBaseInfo   *info,
                                                     const gchar  *name);
gboolean               g_base_info_iterate_attributes (GIBaseInfo      *info,
                                                       GIAttributeIter *iterator,
                                                       char           **name,
                                                       char          **value);
GIBaseInfo *           g_base_info_get_container    (GIBaseInfo   *info);
GTypelib *             g_base_info_get_typelib      (GIBaseInfo   *info);
gboolean               g_base_info_equal            (GIBaseInfo   *info1,
                                                     GIBaseInfo   *info2);
GIBaseInfo *           g_info_new                   (GIInfoType    type,
						     GIBaseInfo   *container,
						     GTypelib     *typelib,
						     guint32       offset);


G_END_DECLS

#endif  /* __GIBASEINFO_H__ */
