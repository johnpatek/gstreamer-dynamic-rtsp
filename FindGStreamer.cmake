if(WIN32)
    find_path(
        GSTREAMER_INCLUDE_DIR gst/gst.h
        HINTS 
        ENV GSTREAMER_DIR
        PATH_SUFFIXES include/gstreamer-1.0)
    find_path(
        GLIB_INCLUDE_DIR glib.h
        HINTS 
        ENV GSTREAMER_DIR
        PATH_SUFFIXES include/glib-2.0)
    find_path(
        GSTREAMER_LIBRARY_DIR gstreamer-1.0.lib
        HINTS 
        ENV GSTREAMER_DIR
        PATH_SUFFIXES lib)
    find_path(
        GLIB_CONFIG_INCLUDE_DIR glibconfig.h
        HINTS 
        ENV GSTREAMER_DIR
        PATH_SUFFIXES lib/glib-2.0/include)
    file(GLOB_RECURSE GSTREAMER_LIBRARIES ${GSTREAMER_LIBRARY_DIR}/*.dll.a)
    # GStreamer::GLib 
    add_library(GStreamer::GLib INTERFACE IMPORTED)
    set_target_properties(GStreamer::GLib PROPERTIES
        INTERFACE_LINK_DIRECTORIES ${GSTREAMER_LIBRARY_DIR}
        INTERFACE_LINK_LIBRARIES glib-2.0)
    target_include_directories(GStreamer::GLib INTERFACE ${GLIB_INCLUDE_DIR} ${GLIB_CONFIG_INCLUDE_DIR})
    # GStreamer::GStreamer
    add_library(GStreamer::GStreamer INTERFACE IMPORTED)
    set_target_properties(GStreamer::GStreamer PROPERTIES
        INTERFACE_LINK_DIRECTORIES ${GSTREAMER_LIBRARY_DIR}
        INTERFACE_INCLUDE_DIRECTORIES ${GSTREAMER_INCLUDE_DIR})
    target_link_libraries(GStreamer::GStreamer INTERFACE gstreamer-1.0 GStreamer::GLib ${GSTREAMER_LIBRARIES})
    
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)
    pkg_check_modules(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0)
    pkg_check_modules(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0)
    pkg_check_modules(gstreamer-rtsp REQUIRED IMPORTED_TARGET gstreamer-rtsp-1.0)
    pkg_check_modules(gstreamer-rtsp-server REQUIRED IMPORTED_TARGET gstreamer-rtsp-server-1.0)
    
    add_library(GStreamer::GLib INTERFACE IMPORTED)
    set_target_properties(GStreamer::GLib PROPERTIES
        INTERFACE_LINK_LIBRARIES PkgConfig::glib)
    
    add_library(GStreamer::GStreamer INTERFACE IMPORTED)
    set_target_properties(GStreamer::GStreamer PROPERTIES
        INTERFACE_LINK_LIBRARIES PkgConfig::gstreamer)

    add_library(GStreamer::GStreamerApp INTERFACE IMPORTED)
    set_target_properties(GStreamer::GStreamerApp PROPERTIES
        INTERFACE_LINK_LIBRARIES PkgConfig::gstreamer-app)
    endif()