#ifndef SYNTAX_H
#define SYNTAX_H

//#include <QtGlobal>

#ifdef Q_CC_CLANG
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif
#ifdef Q_CC_GNU
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif

#if defined (QT_QML_LIB) && !defined (NO_QML_CALLBACK)
#   include <QJSValue>
#else
#   define NO_QML_CALLBACK
#endif

#if __cplusplus >= 201103L && !defined (NO_STD_FUNC)
#   include <functional>
#else
#    define NO_STD_FUNC
#endif

/*
 * NO_NORMAL
 * NO_NORMAL_ASYNC
 * NO_META_METHOD
 * NO_QML_CALLBACK
 * NO_SLOT_CALLBACK
 * NO_STD_FUNC
 * NO_SLOT
 */

#include "p_share.h"
#include "method_normal.h"
#include "method_async.h"
#include "method_metamethod.h"
#include "method_jscallback.h"
#include "method_slotcallback.h"
#include "method_stdfunc.h"
#include "method_slot.h"
#include "decl_signal.h"
//signals:
//  void callNumberSignal(int number);


#define REMOTE_METHOD_DECL_P(ret, name, count, oe, ...) \
    ret name(__NAMEVALUE(count, __VA_ARGS__)); \
    ret name ## Callback(__NAMEVALUE(count, __VA_ARGS__));

#define __REMOTE_METHOD_DECL_P3(ret, name, count, oe, ...) \
    public: \
    METHOD_DECL_P_NORMAL        (ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_DECL_P_ASYNC         (ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_DECL_P_META_METHOD   (ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_DECL_P_QML_CALLBACK  (ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_DECL_P_STD_FUNC      (ret, name, count, __SEP(count), __VA_ARGS__) \
    public Q_SLOTS: \
    METHOD_DECL_P_SLOT          (ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_DECL_P_SLOT_CALLBACK (ret, name, count, __SEP(count), __VA_ARGS__) \
    Q_SIGNALS: \
    METHOD_DECL_P_SIGNAL_##oe   (ret, name, count, __SEP(count), __VA_ARGS__)

#define __REMOTE_METHOD_IMPL_P3(class, ret, name, count, oe, ...) \
    METHOD_IMPL_P_NORMAL_##oe       (class, ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_IMPL_P_ASYNC             (class, ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_IMPL_P_META_METHOD_##oe  (class, ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_IMPL_P_QML_CALLBACK_##oe (class, ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_IMPL_P_SLOT_CALLBACK_##oe(class, ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_IMPL_P_STD_FUNC_##oe     (class, ret, name, count, __SEP(count), __VA_ARGS__) \
    METHOD_IMPL_P_SLOT_##oe         (class, ret, name, count, __SEP(count), __VA_ARGS__)

#define __REMOTE_METHOD_DECL_P2(count, oe, ...) \
    __REMOTE_METHOD_DECL_P3(       __RETTYPE(oe, __VA_ARGS__), __FUNCNAME(count, __VA_ARGS__), count, oe, __PARAMS(count, __VA_ARGS__))

#define __REMOTE_METHOD_IMPL_P2(class, count, oe, ...) \
    __REMOTE_METHOD_IMPL_P3(class, __RETTYPE(oe, __VA_ARGS__), __FUNCNAME(count, __VA_ARGS__), count, oe, __PARAMS(count, __VA_ARGS__))

//User macros
#define N_REMOTE_METHOD_DECL(...) \
    __REMOTE_METHOD_DECL_P2(       VA_NARGS(__VA_ARGS__), VA_ODDEVEN(__VA_ARGS__), __VA_ARGS__)

#define N_REMOTE_METHOD_IMPL(class, ...) \
    __REMOTE_METHOD_IMPL_P2(class, VA_NARGS(__VA_ARGS__), VA_ODDEVEN(__VA_ARGS__), __VA_ARGS__)


//Property
#define N_PROPERTY_DECL(type, name, read, write, notify) \
    private:        \
    Q_PROPERTY(type name READ read WRITE write NOTIFY notify USER true)        \
    type m_##name;\
    public:                                 \
        type read() const; \
    public Q_SLOTS:                         \
    void write(type name); \
    Q_SIGNALS:                              \
    void notify(type name);

#define N_PROPERTY_IMPL(class, type, name, read, write, notify)    \
    type class::read() const{    \
        return m_##name;    \
    } \
    void class::write(type name){ \
        m_##name = name;    \
        invokeOnPeer(#write, name); \
        emit notify(name);  \
    }

//Constructors
#define N_CLASS_DECLARE(class) \
    Q_INVOKABLE  class(QObject *parent = 0); \
    class(NoronAbstractHub *hub, QObject *parent = 0);

#ifdef NORONSHAREDOBJECT_H
#   define N_CLASS_IMPL(class) \
        class::class(QObject *parent) : NoronSharedObject(parent) \
        {  \
            setPeerName(#class); \
        }    \
        class::class(NoronAbstractHub *hub, QObject *parent) : NoronSharedObject(parent)    \
        {   \
            if(hub){    \
                setHub(hub);    \
                hub->addSharedObject(this); \
            }   \
            setPeerName(#class); \
        }
#else
#ifdef NORONPEER_H
#   define N_CLASS_IMPL(class) \
        class::class(QObject *parent) : NoronPeer(parent) \
        {  \
            setPeerName(#class); \
        }    \
        class::class(NoronAbstractHub *hub, QObject *parent) : NoronPeer(parent)    \
        {   \
            if(hub)    \
                setHub(hub);    \
            setPeerName(#class); \
        }
#else
#   error "No NORONSHAREDOBJECT_H nor NORONPEER_H are defined";
#endif
#endif

#ifdef Q_CC_CLANG
#   pragma clang diagnostic pop
#endif
#ifdef Q_CC_GNU
#   pragma GCC diagnostic pop
#endif

#endif // SYNTAX_H