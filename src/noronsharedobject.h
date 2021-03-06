/**************************************************************************
**
** This file is part of Noron.
** https://github.com/HamedMasafi/Noron
**
** Noron is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Noron is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Noron.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#ifndef NORONSHAREDOBJECT_H
#define NORONSHAREDOBJECT_H

#include <QtCore/QMutex>
#include <QtCore/QSet>

#define NORON_SHARED_OBJECT
#include "syntax/syntax_sharedobject.h"
#include "noronabstracthub.h"
#include "noronpeer.h"
#undef NORON_SHARED_OBJECT

NORON_BEGIN_NAMESPACE

//TODO: generate private class for me :-)
class NORON_EXPORT NoronSharedObject : public NoronPeer
{
    Q_OBJECT

    NoronAbstractHub *_activeHub;
    NoronAbstractHub *_deactiveHub;
    bool _autoDelete;
    QMutex hubsLock;

public:
    NoronSharedObject(QObject *parent = 0);
    NoronSharedObject(NoronAbstractHub *hub, QObject *parent = 0);

    void attachHub(NoronAbstractHub *hub);
    void detachHub(NoronAbstractHub *hub);
    bool setActiveHub(NoronAbstractHub *hub);
    bool setAllHubsActiveExcept(NoronAbstractHub *hub);

    QSet<NoronAbstractHub*> hubs;

//    void setHub(NoronAbstractHub* hub) /*Q_DECL_OVERRIDE*/;

    //BC
//    virtual const QString peerName() /*Q_DECL_OVERRIDE*/;
    virtual void hubAdded(NoronAbstractHub *hub);
    virtual void hubRemoved(NoronAbstractHub *hub);

    bool autoDelete() const;
    void setAutoDelete(bool autoDelete);

private slots:
    void hub_statusChanged(NoronAbstractHub::Status status);

protected:
    qlonglong invokeOnPeer(
            QString methodName,
            QVariant val0 = QVariant(),
            QVariant val1 = QVariant(),
            QVariant val2 = QVariant(),
            QVariant val3 = QVariant(),
            QVariant val4 = QVariant(),
            QVariant val5 = QVariant(),
            QVariant val6 = QVariant(),
            QVariant val7 = QVariant(),
            QVariant val8 = QVariant(),
            QVariant val9 = QVariant());

    void sync(NoronAbstractHub *hub);
signals:
#ifdef NORON_CLIENT
    void attached();
    void detached();
#endif
};

NORON_END_NAMESPACE

#endif // NORONSHAREDOBJECT_H
