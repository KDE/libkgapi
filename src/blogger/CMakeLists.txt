add_library(KPim6GAPIBlogger)
add_library(KPim6::GAPIBlogger ALIAS KPim6GAPIBlogger)

target_sources(KPim6GAPIBlogger PRIVATE
    ${libkgapi_debug_SRCS}
    blog.cpp
    blogfetchjob.cpp
    blogfetchjob.h
    bloggerservice.cpp
    bloggerservice.h
    blog.h
    commentapprovejob.cpp
    commentapprovejob.h
    comment.cpp
    commentdeletecontentjob.cpp
    commentdeletecontentjob.h
    commentdeletejob.cpp
    commentdeletejob.h
    commentfetchjob.cpp
    commentfetchjob.h
    comment.h
    page.cpp
    pagecreatejob.cpp
    pagecreatejob.h
    pagedeletejob.cpp
    pagedeletejob.h
    pagefetchjob.cpp
    pagefetchjob.h
    page.h
    pagemodifyjob.cpp
    pagemodifyjob.h
    post.cpp
    postcreatejob.cpp
    postcreatejob.h
    postdeletejob.cpp
    postdeletejob.h
    postfetchjob.cpp
    postfetchjob.h
    post.h
    postmodifyjob.cpp
    postmodifyjob.h
    postpublishjob.cpp
    postpublishjob.h
    postsearchjob.cpp
    postsearchjob.h
)

ecm_generate_headers(kgapiblogger_CamelCase_HEADERS
    HEADER_NAMES
    Blog
    BlogFetchJob
    Comment
    CommentApproveJob
    CommentDeleteContentJob
    CommentDeleteJob
    CommentFetchJob
    Page
    PageCreateJob
    PageDeleteJob
    PageFetchJob
    PageModifyJob
    Post
    PostCreateJob
    PostDeleteJob
    PostFetchJob
    PostModifyJob
    PostPublishJob
    PostSearchJob
    PREFIX KGAPI/Blogger
    REQUIRED_HEADERS kgapiblogger_HEADERS
)

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6GAPIBlogger PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim6GAPIBlogger BASE_NAME kgapiblogger)


target_include_directories(KPim6GAPIBlogger INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI>")

target_link_libraries(KPim6GAPIBlogger
PUBLIC
    KPim6::GAPICore
PRIVATE
    Qt::Network
)

set_target_properties(KPim6GAPIBlogger PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIBlogger
)

install(TARGETS
    KPim6GAPIBlogger
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapiblogger_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI/Blogger"
    COMPONENT Devel
)

install(FILES
    ${kgapiblogger_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapiblogger_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi/blogger"
    COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KPim6GAPIBlogger_QCH
        NAME KPim6GAPIBlogger
        BASE_NAME KPim6GAPIBlogger
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapiblogger_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPIBLOGGER_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
   )
endif()
