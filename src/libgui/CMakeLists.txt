include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} )

# needed for CentOS 6 to use KXMLGUIClient::setXMLFile and KXMLGUIClient::setLocalXMLFile
set_source_files_properties(text_editor.cpp PROPERTIES COMPILE_FLAGS -fno-access-control)

# for messages extraction
FILE(GLOB RC_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../data/app_data/*.rc")
ADD_CUSTOM_COMMAND(
   OUTPUT  rc.cpp
   COMMAND extractrc ${RC_FILES} >> rc.cpp
   DEPENDS ${RC_FILES}
)
SET(gui_STAT_SRCS
    device_gui.cpp
    editor.cpp
    object_view.cpp
    config_gen.cpp
    register_view.cpp
    device_editor.cpp
    watch_view.cpp
    project.cpp
    hex_editor.cpp
    project_editor.cpp
    project_manager.cpp
    global_config.cpp
    config_center.cpp
    editor_manager.cpp
    new_dialogs.cpp
    log_view.cpp
    gui_prog_manager.cpp
    gui_debug_manager.cpp
    breakpoint_view.cpp
    project_wizard.cpp
    toplevel_ui.cpp
    project_manager_ui.cpp
    text_editor.cpp
    main_global.cpp
    toplevel.cpp
)

if(NOT QT_ONLY)
  SET(gui_STAT_SRCS ${gui_STAT_SRCS}
    console.cpp
  )
endif(NOT QT_ONLY)

automoc(${gui_STAT_SRCS})
add_library(gui STATIC ${gui_STAT_SRCS})
