set(kgapiblogger_SRCS
    blog.cpp
    blogfetchjob.cpp
    bloggerservice.cpp
    comment.cpp
    commentapprovejob.cpp
    commentdeletecontentjob.cpp
    commentdeletejob.cpp
    commentfetchjob.cpp
    page.cpp
    pagecreatejob.cpp
    pagedeletejob.cpp
    pagefetchjob.cpp
    pagemodifyjob.cpp
    post.cpp
    postcreatejob.cpp
    postdeletejob.cpp
    postfetchjob.cpp
    postmodifyjob.cpp
    postpublishjob.cpp
    postsearchjob.cpp
)

ecm_generate_headers(kgapiblogger_CamelCase_HEADERS
    HEADER_NAMES
    PageModifyJob
    Post
    PostCreateJob
    PostDeleteJob
    PostFetchJob
    PostModifyJob
    PostPublishJob
    PostSearchJob
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

    PREFIX KGAPI/Blogger
    REQUIRED_HEADERS kgapiblogger_HEADERS
)

add_library(KF5GAPIBlogger
    ${kgapiblogger_SRCS}
)

generate_export_header(KF5GAPIBlogger BASE_NAME kgapiblogger)

add_library(KF5::GAPIDrive ALIAS KF5GAPIBlogger)
target_include_directories(KF5GAPIBlogger INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KGAPI>")

target_link_libraries(KF5GAPIBlogger
PUBLIC
    KF5::GAPICore
)

set_target_properties(KF5GAPIBlogger PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIBlogger
)

install(TARGETS
    KF5GAPIBlogger
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapidrive_CamelCase_HEADERS}
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/KGAPI/Blogger"
    COMPONENT Devel
)

install(FILES
    ${kgapidrive_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapiblogger_export.h"
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/kgapi/blogger"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIBlogger
    LIB_NAME KF5GAPIBlogger
    DEPS "GAPICore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KF5_INCLUDE_INSTALL_DIR}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
