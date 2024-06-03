FIND_PACKAGE( Doxygen )

IF(DOXYGEN_FOUND)
	SET(DOXYGEN_HTML_OUTPUT ${PROJECT_BINARY_DIR}/docs/html)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md)
    set(DOXYGEN_PROJECT_NAME "Atak z trzeciego wymiaru")
    set(DOXYGEN_PROJECT_BRIEF "Projekt zaliczneiowy na Algorytmy i Struktury Danych 2")
    set(DOXYGEN_EXTRACT_LOCAL_CLASSES NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

    doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md)
ENDIF()
