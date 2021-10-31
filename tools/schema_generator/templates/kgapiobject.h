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
#include "object.h"

{{ dependencies.render_includes(schema.dependencies, True, True, False) }}

#include <QSharedPointer>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::{{ api.namespace }} {

class {{ schema.name }};
using {{ schema.name }}Ptr = QSharedPointer<{{ schema.name }}>;

{{ dependencies.render_forward_declarations(schema.dependencies) }}

/**
 * {{ schema.description | wordwrap(80) | replace('\n', '\n * ') }}
 *
 * @see {{ api.reference_base_uri }}#{{ schema.name | lower }}
{%- if api.since %}
 * @since {{ api.since }}
{%- endif %}
 **/
class {{ api.export_macro }} {{ schema.name }} : public KGAPI2::Object {

public:
    {{ dependencies.render_enums_declarations(schema.nested_enums) }}

    /** Construcuts a new {{ schema.name }} **/
    explicit {{ schema.name }}();
    /** Destructor. **/
    ~{{ schema.name }}();

    bool operator==(const {{ schema.name }} &) const;
    bool operator!=(const {{ schema.name }} &) const;

    {{ properties.render_accessors_declarations(schema.properties) }}

    static {{ schema.name }}Ptr fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

private:
    class Private;
    std::unique_ptr<Private> d;
}; // {{ schema.name }}

} // namespace KGAPI2::{{ api.namespace }}
