{#
SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>

SPDX-License-Identifier: LGPL-2.1-only
SPDX-License-Identifier: LGPL-3.0-only
SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
#}

{% import 'macros/properties.j2' as properties -%}
{% import 'macros/dependencies.j2' as dependencies -%}

#pragma once

#include <{{ api.export_macro | lower() }}.h>
#include <QSharedDataPointer>

{{ dependencies.render_includes(schema.dependencies, True, True, False) }}

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::{{ api.namespace }} {

{{ dependencies.render_forward_declarations(schema.dependencies) }}

/**
 * {{ schema.description | wordwrap(80) | replace('\n', '\n * ') }}
 *
 * @see {{ api.reference_base_uri }}#{{ schema.name | lower }}
{%- if api.since %}
 * @since {{ api.since }}
{%- endif %}
 **/
class {{ api.export_macro }} {{ schema.name }} {
public:
    {{ dependencies.render_enums_declarations(schema.nested_enums) }}

    /** Constructs a new {{ schema.name }} **/
    explicit {{ schema.name }}();
    {{ schema.name }}(const {{ schema.name }} &);
    {{ schema.name }}({{ schema.name }} &&) noexcept;
    {{ schema.name }} &operator=(const {{ schema.name }} &);
    {{ schema.name }} &operator=({{ schema.name }} &&) noexcept;
    /** Destructor. **/
    ~{{ schema.name }}();

    bool operator==(const {{ schema.name }} &) const;
    bool operator!=(const {{ schema.name }} &) const;

    static {{ schema.name }} fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    {{ properties.render_accessors_declarations(schema.properties) }}

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // {{ schema.name }}

} // namespace KGAPI2::{{ api.namespace }}
