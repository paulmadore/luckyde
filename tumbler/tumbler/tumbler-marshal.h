#ifndef __TUMBLER_MARSHAL_H__
#define __TUMBLER_MARSHAL_H__

#ifndef __tumbler_marshal_MARSHAL_H__
#define __tumbler_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:STRING,INT,STRING (./tumbler-marshal.list:1) */
extern void tumbler_marshal_VOID__STRING_INT_STRING (GClosure     *closure,
                                                     GValue       *return_value,
                                                     guint         n_param_values,
                                                     const GValue *param_values,
                                                     gpointer      invocation_hint,
                                                     gpointer      marshal_data);

/* VOID:UINT,STRING,INT,STRING (./tumbler-marshal.list:2) */
extern void tumbler_marshal_VOID__UINT_STRING_INT_STRING (GClosure     *closure,
                                                          GValue       *return_value,
                                                          guint         n_param_values,
                                                          const GValue *param_values,
                                                          gpointer      invocation_hint,
                                                          gpointer      marshal_data);

/* VOID:UINT,BOXED,INT,STRING,STRING (./tumbler-marshal.list:3) */
extern void tumbler_marshal_VOID__UINT_BOXED_INT_STRING_STRING (GClosure     *closure,
                                                                GValue       *return_value,
                                                                guint         n_param_values,
                                                                const GValue *param_values,
                                                                gpointer      invocation_hint,
                                                                gpointer      marshal_data);

/* VOID:STRING,INT,STRING (./tumbler-marshal.list:4) */

/* VOID:UINT,BOXED,STRING (./tumbler-marshal.list:5) */
extern void tumbler_marshal_VOID__UINT_BOXED_STRING (GClosure     *closure,
                                                     GValue       *return_value,
                                                     guint         n_param_values,
                                                     const GValue *param_values,
                                                     gpointer      invocation_hint,
                                                     gpointer      marshal_data);

/* VOID:UINT,STRING (./tumbler-marshal.list:6) */
extern void tumbler_marshal_VOID__UINT_STRING (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);

/* VOID:UINT,POINTER,INT,STRING (./tumbler-marshal.list:7) */
extern void tumbler_marshal_VOID__UINT_POINTER_INT_STRING (GClosure     *closure,
                                                           GValue       *return_value,
                                                           guint         n_param_values,
                                                           const GValue *param_values,
                                                           gpointer      invocation_hint,
                                                           gpointer      marshal_data);

G_END_DECLS

#endif /* __tumbler_marshal_MARSHAL_H__ */

#endif /* !__TUMBLER_MARSHAL_H__ */
