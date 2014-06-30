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

#include "label.h"

#include <qjson/serializer.h>
#include <qjson/parser.h>
#include <QVariant>

using namespace KGAPI2;
using namespace KGAPI2::Gmail;

class Label::Private
{
public:
    Private();
    ~Private();

    static LabelPtr fromJSON(const QVariant &json);
    template<typename T>
    static QString enumToString(T val);

    template<typename T>
    static T stringToEnum(const QString &val);

    QString id;
    QString name;
    LabelMessageVisibility msgVisibility;
    LabelVisibility lblVisibility;
    LabelType type;
};

Label::Private::Private()
    : msgVisibility(Gmail::Label::LabelMessageShow)
    , lblVisibility(Gmail::Label::LabelShow)
    , type(Gmail::Label::LabelUserType)
{
}

Label::Private::~Private()
{
}

LabelPtr Label::Private::fromJSON(const QVariant &json)
{
    LabelPtr label(new Label);
    const QVariantMap map = json.toMap();

    label->d->id = map[QLatin1String("id")].toString();
    label->d->name = map[QLatin1String("name")].toString();
    label->d->lblVisibility = stringToEnum<Label::LabelVisibility>(
        map[QLatin1String("labelListVisibility")].toString());
    label->d->msgVisibility = stringToEnum<Label::LabelMessageVisibility>(
        map[QLatin1String("messageListVisibility")].toString());
    label->d->type = stringToEnum<Label::LabelType>(
        map[QLatin1String("type")].toString());
}

template<>
QString Label::Private::enumToString(Label::LabelVisibility val)
{
    switch (val) {
    case Label::LabelHide:
        return QLatin1String("labelHide");
    case Label::LabelShow:
        return QLatin1String("labelShow");
    case Label::LabelShowIfUnread:
        return QLatin1String("labelShowIfUnread");
    default:
        Q_ASSERT(false);
        return QString();
    }
}

template<>
QString Label::Private::enumToString(Label::LabelMessageVisibility val)
{
    switch (val) {
    case Label::LabelMessageHide:
        return QLatin1String("hide");
    case Label::LabelMessageShow:
        return QLatin1String("show");
    default:
        Q_ASSERT(false);
        return QString();
    }
}

template<>
QString Label::Private::enumToString(Label::LabelType val)
{
    switch (val) {
    case Label::LabelSystemType:
        return QLatin1String("system");
    case Label::LabelUserType:
        return QLatin1String("user");
    default:
        Q_ASSERT(false);
        return QString();
    }
}

template<>
Label::LabelVisibility Label::Private::stringToEnum(const QString &val)
{
    if (val == QLatin1String("labelHide")) {
        return Label::LabelHide;
    } else if (val == QLatin1String("labelShow")) {
        return Label::LabelShow;
    } else if (val == QLatin1String("labelShowIfUnread")) {
        return Label::LabelShowIfUnread;
    } else {
        Q_ASSERT(false);
        // Default value?
        return Label::LabelShow;
    }
}

template<>
Label::LabelMessageVisibility Label::Private::stringToEnum(const QString &val)
{
    if (val == QLatin1String("hide")) {
        return Label::LabelMessageHide;
    } else if (val == QLatin1String("show")) {
        return Label::LabelMessageShow;
    } else {
        Q_ASSERT(false);
        return Label::LabelMessageShow;
    }
}

template<>
Label::LabelType Label::Private::stringToEnum(const QString &val)
{
    if (val == QLatin1String("system")) {
        return Label::LabelSystemType;
    } else if (val == QLatin1String("user")) {
        return Label::LabelUserType;
    } else {
        Q_ASSERT(false);
        return Label::LabelUserType;
    }
}

Label::Label()
    : Object()
    , d(new Private)
{
}

Label::~Label()
{
    delete d;
}

QString Label::id()
{
    return d->id;
}

void Label::setId(const QString &id)
{
    d->id = id;
}

QString Label::name() const
{
    return d->name;
}

void Label::setName(const QString &name)
{
    d->name = name;
}

Label::LabelMessageVisibility Label::labelMessageVisibility() const
{
    return d->msgVisibility;
}

void Label::setLabelMessageVisibility(Label::LabelMessageVisibility visibility)
{
    d->msgVisibility = visibility;
}

Label::LabelVisibility Label::labelVisibility() const
{
    return d->lblVisibility;
}

void Label::setLabelVisibility(Label::LabelVisibility visibility)
{
    d->lblVisibility = visibility;
}

Label::LabelType Label::type() const
{
    return d->type;
}

void Label::setType(Label::LabelType type)
{
    d->type = type;
}

LabelPtr Label::fromJSON(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariant json = parser.parse(rawData, &ok);
    if (!ok || json.isNull()) {
        return LabelPtr();
    }

    return Private::fromJSON(json);
}

ObjectsList Label::fromJSONFeed(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap map = parser.parse(rawData, &ok);
    if (!ok || map.isEmpty()) {
        return ObjectsList();
    }

    ObjectsList list;
    const QVariantList labels = map[QLatin1String("labels")].toList();
    Q_FOREACH (const QVariant &lbl, labels) {
        list << Private::fromJSON(lbl);
    }

    return list;
}

QByteArray Label::toJSON(const LabelPtr &label)
{
    const QVariantMap map;

    map[QLatin1String("id")] = label->d->id;
    map[QLatin1String("name")] = label->d->name;
    map[QLatin1String("messageListVisibility")] = Private::enumToString(label->d->msgVisibility);
    map[QLatin1String("labelListVisibility")] = Private::enumToString(label->d->msgVisibility);

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
