add_library(KPim${KF_MAJOR_VERSION}GAPIBlogger)
add_library(KPim${KF_MAJOR_VERSION}::GAPIBlogger ALIAS KPim${KF_MAJOR_VERSION}GAPIBlogger)

target_sources(KPim${KF_MAJOR_VERSION}GAPIBlogger PRIVATE
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

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}GAPIBlogger PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim${KF_MAJOR_VERSION}GAPIBlogger BASE_NAME kgapiblogger)


target_include_directories(KPim${KF_MAJOR_VERSION}GAPIBlogger INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI>")

target_link_libraries(KPim${KF_MAJOR_VERSION}GAPIBlogger
PUBLIC
    KPim${KF_MAJOR_VERSION}::GAPICore
PRIVATE
    Qt::Network
)

set_target_properties(KPim${KF_MAJOR_VERSION}GAPIBlogger PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIBlogger
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}GAPIBlogger
    EXPORT KPim${KF_MAJOR_VERSION}GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapiblogger_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/KGAPI/Blogger"
    COMPONENT Devel
)

install(FILES
    ${kgapiblogger_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapiblogger_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi/blogger"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIBlogger
    LIB_NAME KPim${KF_MAJOR_VERSION}GAPIBlogger
    DEPS "GAPICore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

if (BUILD_QCH)
    ecm_add_qch(
        KPim${KF_MAJOR_VERSION}GAPIBlogger_QCH
        NAME KPim${KF_MAJOR_VERSION}GAPIBlogger
        BASE_NAME KPim${KF_MAJOR_VERSION}GAPIBlogger
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapiblogger_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPIBLOGGER_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
