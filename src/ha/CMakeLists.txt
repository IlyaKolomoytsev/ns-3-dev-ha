set(source_files
    model/ha-phy.cc
    model/ha-header.cc
    model/ha-mpdu.cc
    model/ha-transmission-parameters.cc
    model/ha-ppdu.cc
    model/ha-interference-model.cc
)

set(header_files
    model/ha-phy.h
    model/ha-header.h
    model/ha-mpdu.h
    model/ha-transmission-parameters.h
    model/ha-ppdu.h
    model/ha-interference-model.h
    model/ha-units.h
    model/ha-phy-common.h
)

set(test_files
    test/interference-model.h
    test/interference-model.cc
)

build_lib(
    LIBNAME ha
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    LIBRARIES_TO_LINK
    ${libenergy}
    ${libspectrum}
    ${gsl_libraries}
    TEST_SOURCES ${test_files}
    GENERATE_EXPORT_HEADER
)
