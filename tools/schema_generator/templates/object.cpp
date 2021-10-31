{#
SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>

SPDX-License-Identifier: LGPL-2.1-only
SPDX-License-Identifier: LGPL-3.0-only
SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
#}

{% import "macros/properties.j2" as properties -%}
{% import "macros/dependencies.j2" as dependencies -%}
{% import "macros/serializers.j2" as serializers -%}

#include "{{ schema.name | lower() }}.h"
{{ dependencies.render_includes(schema.dependencies, False, False, True)}}

#include <QSharedData>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <algorithm>

namespace KGAPI2::{{ api.namespace }} {

class {{ schema.name }}::Private : public QSharedData {
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    {{ properties.render_equality_operator(schema.properties) }}

    {{ properties.render_properties_definitions(schema.properties) }}
};

{{ schema.name }}::{{ schema.name }}()
    : d(new Private)
{}

{{ schema.name }}::{{ schema.name }}(const {{ schema.name }} &) = default;
{{ schema.name }}::{{ schema.name }}({{ schema.name }} &&) noexcept = default;
{{ schema.name }} &{{ schema.name }}::operator=(const {{ schema.name }} &) = default;
{{ schema.name }} &{{ schema.name }}::operator=({{ schema.name }} &&) noexcept = default;
{{ schema.name }}::~{{ schema.name }}() = default;

bool {{ schema.name }}::operator==(const {{ schema.name }} &other) const {
    return *d == *other.d;
}

bool {{ schema.name }}::operator!=(const {{ schema.name }} &other) const {
    return !(*this == other);
}

{{ properties.render_accessor_definitions(schema) }}

{{ serializers.render_json_parser(schema, is_ptr=False) }}
{{ serializers.render_json_serializer(schema) }}

} // namespace KGAPI2::{{ api.namespace }}

