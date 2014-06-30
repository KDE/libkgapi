/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KGAPI2_GMAIL_LABEL_H
#define KGAPI2_GMAIL_LABEL_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QStringList>

namespace KGAPI2
{
namespace Gmail
{

class KGAPI_EXPORT Label : public Object
{
public:
    enum LabelVisibility {
        LabelHide,
        LabelShow,
        LabelShowIfUnread
    };

    enum LabelMessageVisibility {
        LabelMessageShow,
        LabelMessageHide
    };

    enum LabelType {
        LabelSystemType,
        LabelUserType
    };

    explicit Label();
    virtual ~Label();

    QString id();
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    LabelMessageVisibility labelMessageVisibility() const;
    void setLabelMessageVisibility(LabelMessageVisibility visibility);

    LabelVisibility labelVisibility() const;
    void setLabelVisibility(LabelVisibility visibility);

    LabelType type() const;
    void setType(LabelType type);

    static LabelPtr fromJSON(const QByteArray &rawData);
    static ObjectsList fromJSONFeed(const QByteArray &rawData);
    static QByteArray toJSON(const LabelPtr &label);

private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace Gmail
} // namespace KGAPI2