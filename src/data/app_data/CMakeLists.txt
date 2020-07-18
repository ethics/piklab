
find_package(SharedMimeInfo REQUIRED)

kde4_install_icons(${ICON_INSTALL_DIR})

install(FILES breakpoint_active.png breakpoint_disabled.png breakpoint_invalid.png breakpoint_reached.png
              program_counter.png program_counter_disabled.png
        DESTINATION ${DATA_INSTALL_DIR}/piklab/pics)

install(FILES piklabui.rc hexeditorpartui.rc katepartui.rc DESTINATION ${DATA_INSTALL_DIR}/piklab )

install(PROGRAMS piklab.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(PROGRAMS piklab.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes( ${XDG_MIME_INSTALL_DIR} )
