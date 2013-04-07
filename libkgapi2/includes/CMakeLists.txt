macro(install_headers _dir)
  install(DIRECTORY ${_dir} DESTINATION ${INCLUDE_INSTALL_DIR}/LibKGAPI2)
endmacro()

install_headers(AccountInfo)
install_headers(Calendar)
install_headers(Contacts)
install_headers(Drive)
install_headers(Latitude)
install_headers(StaticMaps)
install_headers(Tasks)
install_headers(Ui)

set(LibKGAPI_core_NICEHDRS
    Account
    AuthJob
    CreateJob
    DeleteJob
    FetchJob
    Job
    ModifyJob
    Object
    Types
)

install(FILES ${LibKGAPI_core_NICEHDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/LibKGAPI2)
