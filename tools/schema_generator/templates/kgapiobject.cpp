{#
SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>

SPDX-License-Identifier: LGPL-2.1-only
SPDX-License-Identifier: LGPL-3.0-only
SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
#}

{% import 'macros/properties.j2' as properties -%}
{% import 'macros/dependencies.j2' as dependencies -%}

#include "{{ schema.name | lower() }}.h"
{{ dependencies.render_includes(schema.dependencies, False, False, True) }}

#include <algorithm>

namespace KGAPI2::{{ api.namespace }} {

class {{ schema.name }}::Private {
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    {{ properties.render_properties_definitions(schema.properties) }}
};

{{ schema.name }}::{{ schema.name }}()
    : d(new Private)
{}

{{ schema.name }}::~{{ schema.name }}() = default;

{{ properties.render_accessor_definitions(schema) }}

} // namespace KGAPI2::{{ api.namespace }}

