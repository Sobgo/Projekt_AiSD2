find_package(Doxygen)

if(DOXYGEN_FOUND)
	set(DOXYGEN_HTML_OUTPUT ${PROJECT_BINARY_DIR}/docs/html)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md)
    set(DOXYGEN_PROJECT_NAME "Atak z trzeciego wymiaru")
    set(DOXYGEN_PROJECT_BRIEF "Projekt zaliczeniowy na Algorytmy i Struktury Danych 2")
    set(DOXYGEN_EXTRACT_LOCAL_CLASSES NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

    set(CUSTOM_DOXYGEN_CSS
        "${PROJECT_SOURCE_DIR}/doxygen-awesome-css/doxygen-awesome.css"
        "${PROJECT_SOURCE_DIR}/doxygen-awesome-css/doxygen-awesome-sidebar-only.css"
        "./custom.css"
    )

    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CUSTOM_DOXYGEN_CSS})

    doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md)
endif()
