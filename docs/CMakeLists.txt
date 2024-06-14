find_package(Doxygen)

if(DOXYGEN_FOUND)
	set(DOXYGEN_HTML_OUTPUT ${PROJECT_BINARY_DIR}/docs/html)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md)
    set(DOXYGEN_PROJECT_NAME "Atak z trzeciego wymiaru")
    set(DOXYGEN_PROJECT_BRIEF "Projekt zaliczeniowy na Algorytmy i Struktury Danych 2")
    set(DOXYGEN_EXTRACT_LOCAL_CLASSES NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

    include(FetchContent)
    FetchContent_Declare(
    DoxygenAwesome
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
    GIT_TAG        v2.3.3
    )
    FetchContent_MakeAvailable(DoxygenAwesome)
    set(CUSTOM_DOXYGEN_CSS
        "${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css"
        "${doxygenawesome_SOURCE_DIR}/doxygen-awesome-sidebar-only.css"
        "./custom.css"
    )
    set(DOXYGEN_TREEVIEW_WIDTH 350)

    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CUSTOM_DOXYGEN_CSS})

    doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md ${CUSTOM_DOXYGEN_CSS})
endif()
