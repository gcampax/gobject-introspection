/* GObject introspection: Virtual Function implementation
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

#include <string.h>

#include <glib.h>

#include <girepository.h>
#include "girepository-private.h"
#include "gitypelib-internal.h"

/**
 * SECTION:givfuncinfo
 * @Short_description: Struct representing a virtual function
 * @Title: GIVFuncInfo
 *
 * GIVfuncInfo represents a virtual function. A property belongs to
 * either a #GIObjectInfo or a #GIInterfaceInfo.
 */

GIVFuncInfo *
_g_base_info_find_vfunc (GIRealInfo   *rinfo,
			 guint32       offset,
			 gint          n_vfuncs,
			 const gchar  *name)
{
  /* FIXME hash */
  Header *header = (Header *)rinfo->typelib->data;
  gint i;

  for (i = 0; i < n_vfuncs; i++)
    {
      VFuncBlob *fblob = (VFuncBlob *)&rinfo->typelib->data[offset];
      const gchar *fname = (const gchar *)&rinfo->typelib->data[fblob->name];

      if (strcmp (name, fname) == 0)
        return (GIVFuncInfo *) g_info_new (GI_INFO_TYPE_VFUNC, (GIBaseInfo*) rinfo,
                                           rinfo->typelib, offset);

      offset += header->vfunc_blob_size;
    }

  return NULL;
}

/**
 * g_vfunc_info_get_flags:
 * @info: a #GIVFuncInfo
 *
 * Obtain the flags for this virtual function info. See #GIVFuncInfoFlags for
 * more information about possible flag values.
 *
 * Returns: the flags
 */
GIVFuncInfoFlags
g_vfunc_info_get_flags (GIVFuncInfo *info)
{
  GIVFuncInfoFlags flags;
  GIRealInfo *rinfo = (GIRealInfo *)info;
  VFuncBlob *blob;

  g_return_val_if_fail (info != NULL, 0);
  g_return_val_if_fail (GI_IS_VFUNC_INFO (info), 0);

  blob = (VFuncBlob *)&rinfo->typelib->data[rinfo->offset];

  flags = 0;

  if (blob->must_chain_up)
    flags = flags | GI_VFUNC_MUST_CHAIN_UP;

  if (blob->must_be_implemented)
    flags = flags | GI_VFUNC_MUST_OVERRIDE;

  if (blob->must_not_be_implemented)
    flags = flags | GI_VFUNC_MUST_NOT_OVERRIDE;

  return flags;
}

/**
 * g_vfunc_info_get_offset:
 * @info: a #GIVFuncInfo
 *
 * Obtain the offset of the function pointer in the class struct. The value
 * 0xFFFF indicates that the struct offset is unknown.
 *
 * Returns: the struct offset or 0xFFFF if it's unknown
 */
gint
g_vfunc_info_get_offset (GIVFuncInfo *info)
{
  GIRealInfo *rinfo = (GIRealInfo *)info;
  VFuncBlob *blob;

  g_return_val_if_fail (info != NULL, 0);
  g_return_val_if_fail (GI_IS_VFUNC_INFO (info), 0);

  blob = (VFuncBlob *)&rinfo->typelib->data[rinfo->offset];

  return blob->struct_offset;
}

/**
 * g_vfunc_info_get_signal:
 * @info: a #GIVFuncInfo
 *
 * Obtain the signal for the virtual function if one is set.
 * The signal comes from the object or interface to which
 * this virtual function belongs.
 *
 * Returns: the signal or %NULL if none set
 */
GISignalInfo *
g_vfunc_info_get_signal (GIVFuncInfo *info)
{
  GIRealInfo *rinfo = (GIRealInfo *)info;
  VFuncBlob *blob;

  g_return_val_if_fail (info != NULL, 0);
  g_return_val_if_fail (GI_IS_VFUNC_INFO (info), 0);

  blob = (VFuncBlob *)&rinfo->typelib->data[rinfo->offset];

  if (blob->class_closure)
    return g_interface_info_get_signal ((GIInterfaceInfo *)rinfo->container, blob->signal);

  return NULL;
}

/**
 * g_vfunc_info_get_invoker:
 * @info: a #GIVFuncInfo
 *
 * If this virtual function has an associated invoker method, this
 * method will return it.  An invoker method is a C entry point.
 *
 * Not all virtuals will have invokers.
 *
 * Returns: (transfer full): the #GIVFuncInfo or %NULL. Free it with
 * g_base_info_unref() when done.
 */
GIFunctionInfo *
g_vfunc_info_get_invoker (GIVFuncInfo *info)
{
  GIRealInfo *rinfo = (GIRealInfo *)info;
  VFuncBlob *blob;
  GIBaseInfo *container;
  GIInfoType parent_type;

  g_return_val_if_fail (info != NULL, 0);
  g_return_val_if_fail (GI_IS_VFUNC_INFO (info), 0);

  blob = (VFuncBlob *)&rinfo->typelib->data[rinfo->offset];

  /* 1023 = 0x3ff is the maximum of the 10 bits for invoker index */
  if (blob->invoker == 1023)
    return NULL;

  container = rinfo->container;
  parent_type = g_base_info_get_type (container);
  if (parent_type == GI_INFO_TYPE_OBJECT)
    return g_object_info_get_method ((GIObjectInfo*)container, blob->invoker);
  else if (parent_type == GI_INFO_TYPE_INTERFACE)
    return g_interface_info_get_method ((GIInterfaceInfo*)container, blob->invoker);
  else
    g_assert_not_reached ();
}

